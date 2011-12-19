
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream

/*
 * @(#)ThreeD.java  1.4 96/12/06
 *
 * Copyright (c) 1994-1996 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Sun grants you ("Licensee") a non-exclusive, royalty free, license to use,
 * modify and redistribute this software in source and binary code form,
 * provided that i) this copyright notice and license appear on all copies of
 * the software; and ii) Licensee does not utilize the software in a manner
 * which is disparaging to Sun.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY
 * IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE
 * LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS
 * LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT,
 * INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF
 * OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * This software is not designed or intended for use in on-line control of
 * aircraft, air traffic, aircraft navigation or aircraft communications; or in
 * the design, construction, operation or maintenance of any nuclear
 * facility. Licensee represents and warrants that it will not use or
 * redistribute the Software for such purposes.
 */

/* A set of classes to parse, represent and display 3D wireframe models
   represented in Wavefront .obj format. */


#include <iostream>

#include <string.h>

#include <portable/nickcpp/ANullStream.h>

#include <javacpp/Model3Dcpp.h>
#include <javacpp/JMath.h>
#include <javacpp/JSystem.h>
#include <javacpp/JStreamTokenizer.h>


static JMath Math;
static JSystem System;


/** The representation of a 3D model */


void Model3D::init()
{
  DEBUG_OUT<<"Model3D::init()\n";
  vert=NULL;
  tvert=NULL;
  vert_length=0;
  tvert_length=0;
  nvert=0;  maxvert=0;
  con=NULL;
  con_length=0;
  ncon=0;  maxcon=0;
  transformed=false;
  xmin=0.0;  xmax=0.0;  ymin=0.0;  ymax=0.0;  zmin=0.0;  zmax=0.0;
  gr=NULL;
}


Model3D::Model3D () {
  DEBUG_OUT<<"Model3D::Model3D()\n";
  init();
  //mat = new Matrix3D ();
  mat.xRotBy(20);
  mat.yRotBy(30);
}


/** Create a 3D model by parsing an input stream */
Model3D::Model3D (JInputStream *is) /*throws IOException, FileFormatException*/ {
  DEBUG_OUT<<"Model3D::Model3D(JInputStream)\n";
  init();
  if(!is) return;
  //this();
  JStreamTokenizer *st = new JStreamTokenizer(is);
  st->eolIsSignificant(true);
  st->commentChar('#');
scan:
  while (true) {
      switch (st->nextToken()) {
        default:
  	break;
        case JStreamTokenizer_TT_EOF:
  	  return;
  			break;
  	  case JStreamTokenizer_TT_EOL:
  	break;
        case JStreamTokenizer_TT_WORD:
  	if (!strcmp("v",st->sval)/*"v".equals(st->sval)*/) {
  	    double x = 0, y = 0, z = 0;
  	    if (st->nextToken() == JStreamTokenizer_TT_NUMBER) {
  		x = st->nval;
  		if (st->nextToken() == JStreamTokenizer_TT_NUMBER) {
  		    y = st->nval;
  		    if (st->nextToken() == JStreamTokenizer_TT_NUMBER)
  			z = st->nval;
  		}
  	    }
  	    addVert((float) x, (float) y, (float) z);
  	    while (st->ttype != JStreamTokenizer_TT_EOL &&
  		    st->ttype != JStreamTokenizer_TT_EOF)
  		st->nextToken();
  	} else if (!strcmp("f",st->sval)/*"f".equals(st->sval)*/ || !strcmp("fo",st->sval)/*"fo".equals(st->sval)*/ || !strcmp("l",st->sval)/*"l".equals(st->sval)*/) {
  	    int start = -1;
  	    int prev = -1;
  	    int n = -1;
  	    while (true)
  		if (st->nextToken() == JStreamTokenizer_TT_NUMBER) {
  		    n = (int) st->nval;
  		    if (prev >= 0)
  			add(prev - 1, n - 1);
  		    if (start < 0)
  			start = n;
  		    prev = n;
  		} else if (st->ttype == '/')
  		    st->nextToken();
  		else
  		    break;
  	    if (start >= 0)
  		add(start - 1, prev - 1);
  	    if (st->ttype != JStreamTokenizer_TT_EOL)
  		break;
  	} else {
  	    while (st->nextToken() != JStreamTokenizer_TT_EOL
  		    && st->ttype != JStreamTokenizer_TT_EOF);
  	}
      }
  }
  is->close();
  //if (st->ttype != JStreamTokenizer_TT_EOF)
      //throw new FileFormatException(st->toString());
}


/** Add a vertex to this model */
int Model3D::addVert(float x, float y, float z) {
  DEBUG_OUT<<"Model3D::addVert("<<x<<","<<y<<","<<z<<")\n";
  int i = nvert;
  if (i >= maxvert)
  //DEBUG_OUT<<"i >= maxvert!\n";
      if (vert == null) {
  DEBUG_OUT<<"vert == null!\n";
  	maxvert = 100;
  	vert = new float[maxvert * 3];  vert_length=maxvert*3;
      } else {
  DEBUG_OUT<<"vert != null!\n";
  	maxvert *= 2;
  	float *nv = new float[maxvert * 3];
  	System.arraycopy(vert, 0, nv, 0, vert_length/*vert.length*/);
  	vert = nv;  vert_length=maxvert*3;
      }
  i *= 3;
  vert[i] = x;
  vert[i + 1] = y;
  vert[i + 2] = z;
  return nvert++;
}


/** Add a line from vertex p1 to vertex p2 */
void Model3D::add(int p1, int p2) {
  DEBUG_OUT<<"Model3D::add("<<p1<<","<<p2<<")\n";
  int i = ncon;
  if (p1 >= nvert || p2 >= nvert) {
  DEBUG_OUT<<"p1 or p2 was >=nvert!\n";
      return;
  }
  if (i >= maxcon)
 // DEBUG_OUT<<"i >= maxcont!\n";
      if (con == null) {
  DEBUG_OUT<<"con == null!\n";
  	maxcon = 100;
  	con = new int[maxcon];  con_length=maxcon;
      } else {
  DEBUG_OUT<<"con != null!\n";
  	maxcon *= 2;
  	int *nv = new int[maxcon];
  	System.arraycopy(con, 0, nv, 0, con_length/*con.length*/);
        con_length=maxcon;
  	con = nv;
      }
  if (p1 > p2) {
      int t = p1;
      p1 = p2;
      p2 = t;
  }
  con[i] = (p1 << 16) | p2;
  DEBUG_OUT<<"con["<<i<<"]="<<con[i]<<"!\n";
  ncon = i + 1;
}


/** Transform all the points in this model */
void Model3D::transform() {
  DEBUG_OUT<<"Model3D::transform()\n";
  if (transformed || nvert <= 0) {
  DEBUG_OUT<<"transformed or nvert <=0\n";
      return;
  }
  if (tvert == null || tvert_length < nvert * 3) {
      tvert = new int[nvert*3];  tvert_length=nvert*3;
  }
  mat.transform(vert, tvert, nvert);
  transformed = true;
}


/* Quick Sort implementation */
void Model3D::quickSort(int a[], int left, int right)
{
    if(!a) return;
      int leftIndex = left;
      int rightIndex = right;
      int partionElement;
      if ( right > left)
      {

         /* Arbitrarily establishing partition element as the midpoint of
          * the array.
          */
         partionElement = a[ ( left + right ) / 2 ];

         // loop through the array until indices cross
         while( leftIndex <= rightIndex )
         {
            /* find the first element that is greater than or equal to
             * the partionElement starting from the leftIndex.
             */
            while( ( leftIndex < right ) && ( a[leftIndex] < partionElement ) )
               ++leftIndex;

            /* find an element that is smaller than or equal to
             * the partionElement starting from the rightIndex.
             */
            while( ( rightIndex > left ) &&
                   ( a[rightIndex] > partionElement ) )
               --rightIndex;

            // if the indexes have not crossed, swap
            if( leftIndex <= rightIndex )
            {
               swap(a, leftIndex, rightIndex);
               ++leftIndex;
               --rightIndex;
            }
         }

         /* If the right index has not reached the left side of array
          * must now sort the left partition.
          */
         if( left < rightIndex )
            quickSort( a, left, rightIndex );

         /* If the left index has not reached the right side of array
          * must now sort the right partition.
          */
         if( leftIndex < right )
            quickSort( a, leftIndex, right );

      }
}


void Model3D::swap(int a[], int i, int j)
{
    if(!a) return;
      int T;
      T = a[i];
      a[i] = a[j];
      a[j] = T;
}


/** eliminate duplicate lines */
void Model3D::compress() {
  int limit = ncon;
  int *c = con;
  quickSort(con, 0, ncon - 1);
  int d = 0;
  int pp1 = -1;
  for (int i = 0; i < limit; i++) {
      int p1 = c[i];
      if (pp1 != p1) {
  	c[d] = p1;
  	d++;
      }
      pp1 = p1;
  }
  ncon = d;
}


/** Paint this model to a graphics context.  It uses the matrix associated
  with this model to map from model space to screen space.
  The next version of the browser should have double buffering,
  which will make this *much* nicer */
void Model3D::paint(JGraphics *g) {
  DEBUG_OUT<<"Model3D::paint()\n";
  if(!g) {
  DEBUG_OUT<<"no g!\n";
    return;
  }
  if (vert == null || nvert <= 0) {
  DEBUG_OUT<<"no vert or nvert<=0!\n";
      return;
  }
  transform();
  if (gr == null) {
      gr = new JColor[16];
      for (int i = 0; i < 16; i++) {
  	int grey = (int) (170*(1-Math.pow(i/15.0, 2.3)));
  	gr[i].set(0xff-grey, 0xff-grey, 0xff-grey);
      }
  }
  int lg = 0;
  int lim = ncon;
  int *c= con;
  int *v= tvert;
  if (lim <= 0 || nvert <= 0) {
  DEBUG_OUT<<"lim or nvert is <=0!\n";
      return;
  }
  for (int i = 0; i < lim; i++) {
      int T = c[i];
      int p1 = ((T >> 16) & 0xFFFF) * 3;
      int p2 = (T & 0xFFFF) * 3;
  DEBUG_OUT<<"p1="<<p1<<" p2="<<p2<<"\n";
      int grey = v[p1 + 2] + v[p2 + 2];
      if (grey < 0)
  	grey = 0;
      if (grey > 15)
  	grey = 15;
      if (grey != lg) {
     	  lg = grey;
  	  g->setColor(&gr[grey]);
      }
    DEBUG_OUT<<"grey is "<<grey<<"\n";
  	DEBUG_OUT<<"v[p1]="<<v[p1]<<" v[p1+1]="<<v[p1+1]<<" v[p2]="<<v[p2]<<" v[p2+1]="<<v[p2+1]<<"\n";
  		g->drawLine(v[p1], v[p1 + 1],
  	       v[p2], v[p2 + 1]);
  }
}


/** Find the bounding box of this model */
void Model3D::findBB() {
  if (nvert <= 0)
      return;
  float *v = vert;
  float txmin = v[0], txmax = txmin;
  float tymin = v[1], tymax = tymin;
  float tzmin = v[2], tzmax = tzmin;
  for (int i = nvert * 3; (i -= 3) > 0;) {
      float x = v[i];
      if (x < txmin)
  	txmin = x;
      if (x > txmax)
  	txmax = x;
      float y = v[i + 1];
      if (y < tymin)
  	tymin = y;
      if (y > tymax)
  	tymax = y;
      float z = v[i + 2];
      if (z < tzmin)
  	tzmin = z;
      if (z > tzmax)
  	tzmax = z;
  }
  xmax = txmax;
  xmin = txmin;
  ymax = tymax;
  ymin = tymin;
  zmax = tzmax;
  zmin = tzmin;
}


