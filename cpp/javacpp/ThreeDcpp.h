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


#include <javacpp/JApplet.h>
//#include <javacpp/JThread.h>
//#include <javacpp/nickEvent.h>

#include <portable/nick3d/AMesh3D.h>
#include <portable/nick3d/AMatrix3D.h>


class ThreeD : public JApplet {
public:
  //
  ThreeD();
  virtual ~ThreeD();
  virtual void paint(/*JGraphics *g*/);
  virtual void init();
  virtual void run();
  void runBegin();  // split run up into two funcs so we can jump in middle
  void runContinued();  // split run up into two funcs so we can jump in middle
  virtual void start();
  virtual void stop();
  virtual bool mouseDown(nickEvent *e,int x,int y);
  virtual bool mouseDrag(nickEvent *e,int x,int y);
  virtual void setPainted();
  //
  AMesh3D *md;
  boolean painted;
  float xfac;
  int prevx, prevy;
  //float xtheta, ytheta;
  float scalefudge;
  AMatrix3D amat, tmat;
  JString mdname;
  JString message;
};

