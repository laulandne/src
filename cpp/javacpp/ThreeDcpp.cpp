
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


#include <javacpp/ThreeDcpp.h>
//#include <javacpp/JUrl.h>


#ifdef NICK_USE_GUILIB
#include <guilib/nickguilib.h>
#endif // NICK_USE_GUILIB

#ifdef NICK_USE_SDL
#include <sdl/nicksdl.h>
#endif // NICK_USE_SDL

#ifdef NICK_USE_OPENGL
#include <glut/nickglut.h>
#endif // NICK_USE_OPENGL


ThreeD::ThreeD()
{
  DEBUG_OUT<<"ThreeD::ThreeD()\n";
  init();
}


ThreeD::~ThreeD()
{
}


void ThreeD::init() {
  DEBUG_OUT<<"ThreeD::init()\n";
  //JApplet::init();
  md=NULL;
  xfac=1.0;
  prevx=0;  prevy=0;
  painted=true;
  message=NULL;
  //mdname = (JString)getParameter((JString)"model");
  //try {
      scalefudge =1.0; /*Float.valueOf(getParameter("scale")).floatValue();*/
  //}catch(Exception e){};
  amat.yRotBy(20);
  amat.xRotBy(20);
  if (mdname == null)
      mdname = (JString)"model.obj";
  //resize(size().width <= 20 ? 400 : size().width,
  //       size().height <= 20 ? 400 : size().height);
}


void ThreeD::run() {
  runBegin();
  runContinued();
}


void ThreeD::runBegin() {
  DEBUG_OUT<<"ThreeD::runBegin()\n";
  //try {
      //JThread::currentThread()->setPriority(JThread_MIN_PRIORITY);
  	//JUrl *url=new JUrl(getDocumentBase(), mdname);
      //JInputStream *is = url->openStream();
      //Model3D *m = new Model3D (is);
      //md = m;
  //try {
      //if (is != null)
  	//is->close();
  //} catch(Exception e) {
  //}
      runContinued();
}


void ThreeD::runContinued() {
  DEBUG_OUT<<"ThreeD::runContinued()\n";
  if(!md) { std::cerr<<"No md!\n";  return; }
      md->calcBoundingBox();
      //md->compress();
      float xw = md->getXWidth(); //md->xmax - md->xmin;
      float yw = md->getYWidth(); //md->ymax - md->ymin;
      float zw = md->getZWidth(); //md->zmax - md->zmin;
      if (yw > xw)
  	xw = yw;
      if (zw > xw)
  	xw = zw;
      float f1 = nickActualWidth / xw;
      float f2 = nickActualHeight / xw;
      xfac = 0.7f * (f1 < f2 ? f1 : f2) * scalefudge;
  //} catch(Exception e) {
     // md = null;
    //  message = e.toString();
  //}
  repaint();
}


void ThreeD::start() {
  DEBUG_OUT<<"ThreeD::start()\n";
  //if (md == null /*&& message == null*/)
      //if(!defaultThread) defaultThread=new JThread(this);
  	//if(defaultThread) defaultThread->start();
  JApplet::start();
}


void ThreeD::stop() {
  DEBUG_OUT<<"ThreeD::stop()\n";
  //JApplet::stop();
}


boolean ThreeD::mouseDown(nickEvent *e, int x, int y) {
  DEBUG_OUT<<"ThreeD::mouseDown()\n";
  prevx = x;
  prevy = y;
  return true;
}


boolean ThreeD::mouseDrag(nickEvent *e, int x, int y) {
  DEBUG_OUT<<"ThreeD::mouseDrag()\n";
  tmat.unit();
  float xtheta = (prevy - y) * 360.0f / nickActualWidth;
  float ytheta = (x - prevx) * 360.0f / nickActualHeight;
  tmat.xRotBy(xtheta);
  tmat.yRotBy(ytheta);
  amat.mult(&tmat);
  if (painted) {
      painted = false;
      repaint();
  }
  prevx = x;
  prevy = y;
  return true;
}


void ThreeD::paint(/*JGraphics *g*/) {
  DEBUG_OUT<<"ThreeD::paint()\n";
  //if(!g) {
  //DEBUG_OUT<<"no g!\n";
    //return;
  //}
#ifdef NICK_USE_OPENGL
  //glBegin(GL_LINES);
#endif // NICK_USE_OPENGL
  nickLockDisplay();
  nickSetColorLong(0x000000);
  nickClearDisplay();
  if (md != null) {
      md->getModelToDrawMatrix()->unit();
      md->getModelToDrawMatrix()->translate(-(md->getBMin()->x + md->getBMax()->x) / 2,
  		     -(md->getBMin()->y + md->getBMax()->y) / 2,
  		     -(md->getBMin()->z + md->getBMax()->z) / 2);
      md->getModelToDrawMatrix()->mult(&amat);
//      md->mat.scale(xfac, -xfac, 8 * xfac / size().width);
      md->getModelToDrawMatrix()->scale(xfac, -xfac, 16 * xfac / nickActualWidth);
      md->getModelToDrawMatrix()->translate(nickActualWidth / 2, nickActualHeight / 2, 8);
      md->setStale(true);
      md->transformVertsToDraw();
      nickDrawMesh3D(md,0.0,0.0,0.0);
      setPainted();
  } /*else if (message != null) {
      g->drawString((JString)"Error in model:", 3, 20);
      g->drawString(message, 10, 40);
  } */
  else {
  DEBUG_OUT<<"no md!\n";
  }
#ifdef NICK_USE_OPENGL
  //glEnd();
#endif // NICK_USE_OPENGL
  nickUnlockDisplay();
  nickFlipDisplay();
}


/*synchronized*/ void ThreeD::setPainted() {
  painted = true;
  //notifyAll();
}


/*
int main(int argc,char **argv)
{
  aThisApp.start(argc,argv);
  ThreeD td;
  td.run();
  td.start();
  return 0;
}
*/

