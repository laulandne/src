#ifndef NICKGLUT_H
#define NICKGLUT_H


#ifdef NICK_USE_OPENGL


#define GLUT


#ifdef __APPLE__
#ifdef ASYS_MAC
#include <AGL/agl.h>
#endif // ASYS_MAC
#define FOUND_GL_HEADER 1
#ifdef NICK_USE_GLUT
#include <GLUT/glut.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif // NICK_USE_GLUT
#endif // __APPLE__


#ifdef _WIN32
#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <GL/glut.h>
#define FOUND_GL_HEADER 1
#endif // _WIN32


#ifndef FOUND_GL_HEADER
#ifdef NICK_USE_GLUT
#include <GL/glut.h>
#endif // NICK_USE_GLUT
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#endif // FOUND_GL_HEADER


class ABitmap;
class APalette;

class AFace3D;
class AVertexI3D;
class AVertexF3D;
class AMesh3D;
class AScene3D;

class nickDrawableBitmap;


extern "C" {


#ifdef NICK_USE_GLUT
struct nickEvent {
  int x,y;
  int val;
  int type;
};
#endif // NICK_USE_GLUT


extern unsigned int nickActualWidth;
extern unsigned int nickActualHeight;


bool nickAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);
bool nickAppInitGLUT(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);
bool nickAppInitOpenGL(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);

void nickLockDisplay();
void nickUnlockDisplay();
void nickFlipDisplay();

void nickSetColorRGB(unsigned int r,unsigned int g,unsigned int b);
void nickSetColorLong(unsigned long c);

void nickClearDisplay();

void nickWritePixel(unsigned int x,unsigned int y);
unsigned long nickReadPixel(unsigned int x,unsigned int y);

void nickDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2);

nickDrawableBitmap *nickGetDrawableBitmap(ABitmap *bm);
void nickDrawImage(nickDrawableBitmap *bm,unsigned int x,unsigned int y);
void nickCopyBlock(nickDrawableBitmap *bm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh);

void nickDrawString(char *st,unsigned int x,unsigned int y);
void nickLoadFont(nickDrawableBitmap *bm,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh);

APalette *nickGetCurPalette();

void nickDrawFace3D(AFace3D *f,AVertexI3D *dvs);
void nickDrawFaceTextured3D(AMesh3D *m,AFace3D *f,AVertexI3D *dvs);
void nickDrawMesh3D(AMesh3D *m,float x,float y,float z);
void nickDrawScene3D(AScene3D *s);

void nickRegisterEventHandlers();
void nickRunEventLoop();

};


#endif // NICK_USE_OPENGL


#endif // NICKGLUT_H
