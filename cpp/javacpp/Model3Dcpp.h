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


#include <javacpp/JColor.h>
#include <javacpp/JGraphics.h>
#include <javacpp/JInputStream.h>

#include <portable/nick3d/AMatrix3D.h>


/** The representation of a 3D model */
class Model3D {
  public:
    //
    float *vert;
    int vert_length;
    int *tvert;
    int tvert_length;
    int nvert, maxvert;
    int *con;
    int con_length;
    int ncon, maxcon;
    boolean transformed;
    AMatrix3D mat;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    JColor *gr;
    //
    Model3D();
    /** Create a 3D model by parsing an input stream */
    Model3D(JInputStream *is);
    void init();
    /** Add a vertex to this model */
    int addVert(float x, float y, float z);
    /** Add a line from vertex p1 to vertex p2 */
    void add(int p1, int p2);
    /** Transform all the points in this model */
    void transform();
    /** eliminate duplicate lines */
    void compress();
    /** Paint this model to a graphics context.  It uses the matrix associated
      with this model to map from model space to screen space.
      The next version of the browser should have double buffering,
      which will make this *much* nicer */
    void paint(JGraphics *g);
    /** Find the bounding box of this model */
    void findBB();
    //
  private:
    //
    /* Quick Sort implementation
    */
    void quickSort(int a[], int left, int right);
    void swap(int a[], int i, int j);
};

