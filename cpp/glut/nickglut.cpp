
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "nickglut.h"

#include <javacpp/JApplet.h>

#include <portable/nickcpp/APalette.h>

#include <portable/nick3d/AScene3D.h>


#define NICK_LINES_NOT_TRIANGLES 1


#ifdef NICK_USE_GLUT
unsigned int nickActualWidth=0;
unsigned int nickActualHeight=0;
#endif // NICK_USE_GLUT

#ifdef NICK_USE_OPENGL
unsigned int nickColorRed=0;
unsigned int nickColorGreen=0;
unsigned int nickColorBlue=0;
unsigned long nickColorLong=0;
float nickColorRedF=0.0;
float nickColorGreenF=0.0;
float nickColorBlueF=0.0;
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_GLUT
void JAppletDisplayGlut();
void JAppletReshapeGlut(int w, int h);
void JAppletMouseGlut(int button, int state, int x, int y);
void JAppletMotionGlut(int x, int y);
void JAppletKeyboardGlut(unsigned char key, int x, int y);
void JAppletSpecialGlut(int key, int x, int y);
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
bool nickAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  bool ret=false;
  ret=nickAppInitGLUT(theWidth,theHeight,theDepth,argc,argv);
  if(ret) ret=nickAppInitOpenGL(theWidth,theHeight,theDepth,argc,argv);
  return ret;
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
bool nickAppInitGLUT(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  DEBUG_OUT<<"nickAppInitGLUT(...)\n";
  DEBUG_OUT<<"(Going to call glutInit...)\n";
  glutInit(&argc, argv);
  DEBUG_OUT<<"(Going to call glutInitDisplayMode...)\n";
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  DEBUG_OUT<<"(Going to call glutInitWindowSize...)\n";
  glutInitWindowSize(theWidth, theHeight);
  DEBUG_OUT<<"(Going to call glutCreateWindow...)\n";
  glutCreateWindow(argv[0]);
  {
  /* Check that renderer has the GL_EXT_texture_object
     extension or supports OpenGL 1.1 */
    char *version = (char *) glGetString(GL_VERSION);
    double gl_vers = strtod(version, NULL);
    if ((gl_vers < 1.1) && !glutExtensionSupported("GL_EXT_texture_object")) {
      fprintf(stderr, "GL version must be 1.1 or greater.\n");
      return false;  // Just exit if the support is not there.
    }
  }
  DEBUG_OUT<<"nickAppInitGLUT done.\n";
  return true;
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_OPENGL
bool nickAppInitOpenGL(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  DEBUG_OUT<<"nickAppInitOpenGL(...)\n";
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClearDepth(1.0);
  glPolygonOffset(1.0, 1.0);
  glEnable(GL_CULL_FACE);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glViewport(0, 0, theWidth, theHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(65.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,0.0);  /* viewing transform  */
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, theWidth, theHeight, 0, 0, 1);
  glMatrixMode (GL_MODELVIEW);
  DEBUG_OUT<<"nickAppInitOpenGL done.\n";
  nickActualWidth=theWidth;
  nickActualHeight=theHeight;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  return true;
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_GLUT
void nickRegisterEventHandlers()
{
  DEBUG_OUT<<"nickRegisterEventHandlers(...)\n";
  glutDisplayFunc(JAppletDisplayGlut);
  glutReshapeFunc(JAppletReshapeGlut);
  glutMouseFunc(JAppletMouseGlut);
  glutMotionFunc(JAppletMotionGlut);
  glutKeyboardFunc(JAppletKeyboardGlut);
  glutSpecialFunc(JAppletSpecialGlut);
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void nickRunEventLoop()
{
  DEBUG_OUT<<"nickRunEventLoop(...)\n";
  DEBUG_OUT<<"Going to call glutMainLoop...\n";
  glutMainLoop();
  DEBUG_OUT<<"Back from glutMainLoop.\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void translateEventGlut(unsigned int code,unsigned int x,unsigned int y)
{
  DEBUG_OUT<<"JApplet::translateEventGlut("<<code<<","<<x<<","<<y<<")\n";
  aDefaultApplet->event.x=x;
  aDefaultApplet->event.y=y;
  //aDefaultApplet->event.type=type;
  aDefaultApplet->event.val=code;
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void JAppletDisplayGlut()
{
  DEBUG_OUT<<"JAppletDisplayGlut()\n";
  if(aDefaultApplet) {
    translateEventGlut(0,0,0);
    aDefaultApplet->repaint();
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void JAppletReshapeGlut(int w, int h)
{
  DEBUG_OUT<<"JAppletReshapeGlut()\n";
  if(aDefaultApplet) {
    translateEventGlut(0,w,h);
    nickActualWidth=w;  nickActualHeight=h;
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, nickActualWidth, nickActualHeight, 0, 0, 1);
    glMatrixMode (GL_MODELVIEW);
    DEBUG_OUT<<"Now "<<nickActualWidth<<" by "<<nickActualHeight<<"\n";
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void JAppletMouseGlut(int button, int state, int x, int y)
{
  DEBUG_OUT<<"JAppletMouseGlut()\n";
  if(aDefaultApplet) {
    translateEventGlut(button,x,y);
    aDefaultApplet->mouseDown(aDefaultApplet->getEvent(),x,y);
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void JAppletMotionGlut(int x, int y)
{
  DEBUG_OUT<<"JAppletMotionGlut()\n";
  if(aDefaultApplet) {
    translateEventGlut(0,x,y);
    aDefaultApplet->mouseDrag(aDefaultApplet->getEvent(),x,y);
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void JAppletKeyboardGlut(unsigned char key, int x, int y)
{
  DEBUG_OUT<<"JAppletKeyboardGlut()\n";
  if(aDefaultApplet) {
    translateEventGlut(key,x,y);
    // ???
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void JAppletSpecialGlut(int key, int x, int y)
{
  DEBUG_OUT<<"JAppletSpecialGlut()\n";
  if(aDefaultApplet) {
    translateEventGlut(0,x,y);
    // ???
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_OPENGL
void nickWritePixel(unsigned int x,unsigned int y)
{
  DEBUG_OUT<<"nickWritePixel not implemented!\n";
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
unsigned long nickReadPixel(unsigned int x,unsigned int y)
{
  DEBUG_OUT<<"nickReadPixel not implemented!\n";
  return 0;
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  glVertex3f(xu1,yu1,0.0);
  glVertex3f(xu2,yu2,0.0);
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
nickDrawableBitmap *nickGetDrawableBitmap(ABitmap *bm)
{
  return (nickDrawableBitmap *)bm;
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickDrawImage(nickDrawableBitmap *bm,unsigned int x,unsigned int y)
{
  DEBUG_OUT<<"nickDrawImage not implemented!\n";
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickCopyBlock(nickDrawableBitmap *bm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
  DEBUG_OUT<<"nickCopyBlock not implemented!\n";
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickDrawString(char *st,unsigned int x,unsigned int y)
{
  DEBUG_OUT<<"nickDrawString not implemented!\n";
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickLoadFont(ABitmap *bm,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh)
{
  DEBUG_OUT<<"nickLoadFont not implemented!\n";
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
APalette *nickGetCurPalette()
{
  //DEBUG_OUT<<"nickGetCurPalette not implemented!\n";
  return NULL;
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
// TODO: Should the alpha be something other than 0.0?
void nickClearDisplay()
{
  //DEBUG_OUT<<"nickClearDisplay: "<<nickColorRedF<<","<<nickColorGreenF<<","<<nickColorBlueF<<"\n";
  glClearColor(nickColorRedF,nickColorGreenF,nickColorBlueF,1.0f);
  //glClearColor(1.0f,0.0f,0.0f,1.0f);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glFlush();
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_GLUT
void nickLockDisplay()
{
  //DEBUG_OUT<<"nickLockDisplay not implemented!\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void nickUnlockDisplay()
{
  //DEBUG_OUT<<"nickUnlockDisplay not implemented!\n";
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_GLUT
void nickFlipDisplay()
{
  glFlush();
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
#endif // NICK_USE_GLUT


#ifdef NICK_USE_OPENGL
void nickSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
  nickColorLong=((r<<16)&0xff0000)+((b<<8)&0xff00)+(g&0xff);
  nickColorRed=r;
  nickColorGreen=g;
  nickColorBlue=b;
  nickColorRedF=(float)nickColorRed/255.0;
  nickColorGreenF=(float)nickColorGreen/255.0;
  nickColorBlueF=(float)nickColorBlue/255.0;
  glColor3f(nickColorRedF,nickColorGreenF,nickColorBlueF);
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickSetColorLong(unsigned long c)
{
  nickColorLong=c;
  nickColorRed=getRGBRed24(c);
  nickColorGreen=getRGBGreen24(c);
  nickColorBlue=getRGBBlue24(c);
  nickColorRedF=(float)nickColorRed/255.0;
  nickColorGreenF=(float)nickColorGreen/255.0;
  nickColorBlueF=(float)nickColorBlue/255.0;
  glColor3f(nickColorRedF,nickColorGreenF,nickColorBlueF);
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickDrawFace3D(AFace3D *f,AVertexI3D *dvs)
{
  int x1,y1,x2,y2,x3,y3;
  //float z1,z2,z3;
  float c1,c2,c3;
  unsigned int clr;
  float fact=255.0;
  //c1=(z1+z2)/2;  c2=(z2+z3)/2;  c3=(z3+z1)/2;
  c1=1.0;  c2=1.0;  c3=1.0;
  //
  //z1=dvs[f->a].z;
  //z2=dvs[f->b].z;
  //z3=dvs[f->c].z;
  //
  x1=dvs[f->a].x;
  //if(x1<0.0) x1=-x1;
  y1=dvs[f->a].y;
  //if(y1<0.0) y1=-y1;
  x2=dvs[f->b].x;
  //if(x2<0.0) x2=-x2;
  y2=dvs[f->b].y;
  //if(y2<0.0) y2=-y2;
  x3=dvs[f->c].x;
  //if(x3<0.0) x3=-x3;
  y3=dvs[f->c].y;
  //if(y3<0.0) y3=-y3;
  //
  //DEBUG_OUT<<"drawFace3D: ("<<x1<<","<<y1<<") ("<<x2<<","<<y2<<") ("<<x3<<","<<y3<<")\n";
  //DEBUG_OUT<<"face colors: "<<f->fRed<<","<<f->fGreen<<","<<f->fBlue<<"\n";
  //
#ifdef NICK_LINES_NOT_TRIANGLES
  //if((z1>0)&&(z2>0)) {
  //clr=makeRGB24((int)(c1*fact*f->fRed),(int)(c1*fact*f->fGreen),(int)(c1*fact*f->fBlue));
  //nickSetColorLong(clr);
  nickSetColorRGB(c1*fact*f->fRed,c1*fact*f->fGreen,c1*fact*f->fBlue);
  nickDrawLine(x1,y1,x2,y2);
  //}
  //if((z2>0)&&(z3>0)) {
  //clr=makeRGB24((int)(c2*fact*f->fRed),(int)(c2*fact*f->fGreen),(int)(c2*fact*f->fBlue));
  //nickSetColorLong(clr);
  nickSetColorRGB(c2*fact*f->fRed,c2*fact*f->fGreen,c2*fact*f->fBlue);
  nickDrawLine(x2,y2,x3,y3);
  //}
  //if((z3>0)&&(z1>0)) {
  //clr=makeRGB24((int)(c3*fact*f->fRed),(int)(c3*fact*f->fGreen),(int)(c3*fact*f->fBlue));
  //nickSetColorLong(clr);
  nickSetColorRGB(c3*fact*f->fRed,c3*fact*f->fGreen,c3*fact*f->fBlue);
  nickDrawLine(x3,y3,x1,y1);
  //}
#else
  nickSetColorRGB(c1*fact*f->fRed,c1*fact*f->fGreen,c1*fact*f->fBlue);
  glVertex3f(dvs[f->a].x,dvs[f->a].y,0);
  glVertex3f(dvs[f->b].x,dvs[f->b].y,0);
  glVertex3f(dvs[f->c].x,dvs[f->c].y,0);
#endif // NICK_LINES_NOT_TRIANGLES
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickDrawFaceTextured3D(AMesh3D *m,AFace3D *f,AVertexI3D *dvs)
{
  int x1,y1,x2,y2,x3,y3;
  float c1,c2,c3;
  unsigned int clr;
  float fact=255.0;
  c1=1.0;  c2=1.0;  c3=1.0;
  x1=dvs[f->a].x;
  y1=dvs[f->a].y;
  x2=dvs[f->b].x;
  y2=dvs[f->b].y;
  x3=dvs[f->c].x;
  y3=dvs[f->c].y;
  //
  //DEBUG_OUT<<"drawFace3D: ("<<x1<<","<<y1<<") ("<<x2<<","<<y2<<") ("<<x3<<","<<y3<<")\n";
  //DEBUG_OUT<<"face colors: "<<f->fRed<<","<<f->fGreen<<","<<f->fBlue<<"\n";
  //
  nickSetColorRGB(c1*fact*f->fRed,c1*fact*f->fGreen,c1*fact*f->fBlue);
  glVertex3f(dvs[f->a].x,dvs[f->a].y,0);
  glVertex3f(dvs[f->b].x,dvs[f->b].y,0);
  glVertex3f(dvs[f->c].x,dvs[f->c].y,0);
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickDrawMesh3D(AMesh3D *m,float x,float y,float z)
{
  DEBUG_OUT<<"nickDrawMesh3D Mesh has "<<m->getNumFaces()<<" faces...\n";
  AVertexI3D *dvs=m->getDrawVerts();
  if(m->getNumTextures()) {
    glBegin(GL_TRIANGLES);
    //nickLockDisplay();
    //nickSetColorLong(0x404040);
    //nickClearDisplay();
    for(unsigned int t=0;t<m->getNumFaces();t++) {
      AFace3D *f=m->getFaces()[t];
      //DEBUG_OUT<<"face: "<<f->a<<","<<f->b<<","<<f->c<<"\n";
      nickDrawFaceTextured3D(m,f,dvs);
    }
    glEnd();
    //nickUnlockDisplay();
    //nickFlipDisplay();
  }
  else {
#ifdef NICK_LINES_NOT_TRIANGLES
    glBegin(GL_LINES);
#else
    glBegin(GL_TRIANGLES);
#endif // NICK_LINES_NOT_TRIANGLES
    //nickLockDisplay();
    //nickSetColorLong(0x404040);
    //nickClearDisplay();
    for(unsigned int t=0;t<m->getNumFaces();t++) {
      AFace3D *f=m->getFaces()[t];
      //DEBUG_OUT<<"face: "<<f->a<<","<<f->b<<","<<f->c<<"\n";
      nickDrawFace3D(f,dvs);
    }
    glEnd();
    //nickUnlockDisplay();
    //nickFlipDisplay();
  }
}
#endif // NICK_USE_OPENGL


#ifdef NICK_USE_OPENGL
void nickDrawScene3D(AScene3D *s)
{
  unsigned int nshapes=s->getNumMeshes();
  DEBUG_OUT<<"nickDrawScene3D Scene has "<<nshapes<<" meshes...\n";
#ifdef NICK_LINES_NOT_TRIANGLES
  glBegin(GL_LINES);
#else
  glBegin(GL_TRIANGLES);
#endif // NICK_LINES_NOT_TRIANGLES
  //nickLockDisplay();
  //nickSetColorLong(0x404040);
  //nickClearDisplay();
  for(unsigned int t=0;t<nshapes;t++) {
    AMesh3D *m=s->getMeshes()[t];
    AVertexI3D *dvs=m->getDrawVerts();
    for(unsigned int tt=0;tt<m->getNumFaces();tt++) {
      AFace3D *f=m->getFaces()[tt];
      //DEBUG_OUT<<"face: "<<f->a<<","<<f->b<<","<<f->c<<"\n";
      nickDrawFace3D(f,dvs);
    }
  }
  glEnd();
  //nickUnlockDisplay();
  //nickFlipDisplay();
}
#endif // NICK_USE_OPENGL
