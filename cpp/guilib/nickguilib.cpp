
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#ifdef NICK_USE_GUILIB


#include <cstdlib>
#include <iostream>


#include "nickguilib.h"

#include <portable/nickcpp/APalette.h>

#include <portable/nick3d/AScene3D.h>


#include <javacpp/JApplet.h>


unsigned int nickActualWidth=0;
unsigned int nickActualHeight=0;
unsigned int nickColorRed=0;
unsigned int nickColorGreen=0;
unsigned int nickColorBlue=0;
unsigned long nickColorLong=0;


void translateEventGuilib(AEvent *e);

void JAppletMousedownGuilib(AEvent *e);
void JAppletMouseupGuilib(AEvent *e);
void JAppletMousemoveGuilib(AEvent *e);
void JAppletMousedragGuilib(AEvent *e);
void JAppletRedrawGuilib(AEvent *e);
void JAppletResizeGuilib(AEvent *e);
void JAppletDestroyGuilib(AEvent *e);
void JAppletKeyupGuilib(AEvent *e);
void JAppletKeydownGuilib(AEvent *e);
void JAppletKeymoveGuilib(AEvent *e);


bool nickAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  DEBUG_OUT<<"nickAppInit(...)\n";
  DEBUG_OUT<<"About to call start...\n";
  aThisApp.start(argc,argv);
  DEBUG_OUT<<"returned from start, creating display...\n";
  ADisplay *d=new ADisplay(theWidth,theHeight,theDepth);
  DEBUG_OUT<<"creating window...\n";
  AWindow *w=new AWindow(d,argv[0],theWidth,theHeight);
  aDefaultAWindow=w;
  nickActualWidth=w->getWidth();
  nickActualHeight=w->getHeight();
  DEBUG_OUT<<"AWindow is "<<nickActualWidth<<"x"<<nickActualHeight<<"\n";
  DEBUG_OUT<<"nickAppInit done.\n";
  return true;
}


void nickRegisterEventHandlers()
{
  DEBUG_OUT<<"nickRegisterEventHandlers(...)\n";
  aDefaultApplet->loop->regEvent(A_EVENT_REDRAW,JAppletRedrawGuilib);
  aDefaultApplet->loop->regEvent(A_EVENT_RESIZE,JAppletResizeGuilib);
  aDefaultApplet->loop->regEvent(A_EVENT_DESTROY,JAppletDestroyGuilib);
  aDefaultApplet->loop->regEvent(A_EVENT_BREAK,JAppletDestroyGuilib);
  aDefaultApplet->loop->regEvent(A_EVENT_MOUSEDOWN,JAppletMousedownGuilib);
  aDefaultApplet->loop->regEvent(A_EVENT_MOUSEUP,JAppletMouseupGuilib);
  aDefaultApplet->loop->regEvent(A_EVENT_MOUSEMOVE,JAppletMousemoveGuilib);
  aDefaultApplet->loop->regEvent(A_EVENT_KEYDOWN,JAppletKeydownGuilib);
  aDefaultApplet->loop->regEvent(A_EVENT_KEYUP,JAppletKeyupGuilib);
}


void nickRunEventLoop()
{
  DEBUG_OUT<<"nickRunEventLoop(...)\n";
  DEBUG_OUT<<"Going to apploop.run...\n";
  aDefaultApplet->loop->run();
  DEBUG_OUT<<"Back from apploop.run.\n";
}


void translateEventGuilib(AEvent *e)
{
  DEBUG_OUT<<"JApplet::translateEventGuilib()\n";
  if(!e) return;
  aDefaultApplet->event.x=e->x;
  aDefaultApplet->event.y=e->y;
  aDefaultApplet->event.type=e->type;
  aDefaultApplet->event.val=e->code;
}


#ifdef NICK_USE_GUILIB
void JAppletMousedownGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletMousedownGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
    aDefaultApplet->mouseDown(aDefaultApplet->getEvent(),e->x,e->y);
    aDefaultApplet->buttonPressed=true;
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


#ifdef NICK_USE_GUILIB
void JAppletMouseupGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletMouseupGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
    aDefaultApplet->mouseUp(aDefaultApplet->getEvent(),e->x,e->y);
    aDefaultApplet->buttonPressed=false;
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


#ifdef NICK_USE_GUILIB
void JAppletMousemoveGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletMousemoveGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
    aDefaultApplet->mouseMove(aDefaultApplet->getEvent(),e->x,e->y);
    if(aDefaultApplet->buttonPressed) JAppletMousedragGuilib(e);
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


#ifdef NICK_USE_GUILIB
void JAppletRedrawGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletRedrawGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
    aDefaultApplet->repaint();
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


#ifdef NICK_USE_GUILIB
void JAppletResizeGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletResizeGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
    //nickActualWidth=e->x;  nickActualHeight=e->y;
    DEBUG_OUT<<"Now "<<nickActualWidth<<" by "<<nickActualHeight<<"\n";
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


#ifdef NICK_USE_GUILIB
void JAppletMousedragGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletDragGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
    aDefaultApplet->mouseDrag(aDefaultApplet->getEvent(),e->x,e->y);
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


#ifdef NICK_USE_GUILIB
void JAppletDestroyGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletDestroyGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
    exit(0);
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


#ifdef NICK_USE_GUILIB
void JAppletKeydownGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletKeydownGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


#ifdef NICK_USE_GUILIB
void JAppletKeyupGuilib(AEvent *e)
{
  DEBUG_OUT<<"JAppletKeyupGuilib()\n";
  if(aDefaultApplet) {
    translateEventGuilib(e);
  }
  else DEBUG_OUT<<"no applet!\n";
}
#endif // NICK_USE_GUILIB


void nickWritePixel(unsigned int x,unsigned int y)
{
  aDefaultAWindow->writePixel(x,y);
}


unsigned long nickReadPixel(unsigned int x,unsigned int y)
{
  return aDefaultAWindow->readPixel(x,y);
}


void nickDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  aDefaultAWindow->line(xu1,yu1,xu2,yu2);
}


nickDrawableBitmap *nickGetDrawableBitmap(ABitmap *bm)
{
  return (nickDrawableBitmap *)(new APrivateBitmap(bm));
}


void nickDrawImage(nickDrawableBitmap *bm,unsigned int x,unsigned int y)
{
  aDefaultAWindow->draw((APrivateBitmap *)bm,x,y);
}


void nickCopyBlock(nickDrawableBitmap *bm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
  aDefaultAWindow->copyBlock((APrivateBitmap *)bm,sx,sy,dx,dy,bw,bh);
}


void nickDrawString(char *st,unsigned int x,unsigned int y)
{
  DEBUG_OUT<<"nickDrawString not implemented!\n";
}


void nickLoadFont(APrivateBitmap *bm,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh)
{
  DEBUG_OUT<<"nickLoadFont not implemented!\n";
}


APalette *nickGetCurPalette()
{
  return aDefaultAWindow->getParent()->getColors();
}


void nickClearDisplay()
{
  aDefaultAWindow->setBackground(nickColorLong,true);
  aDefaultAWindow->clear();
}


void nickLockDisplay()
{
  DEBUG_OUT<<"nickLockDisplay not implemented!\n";
}


void nickUnlockDisplay()
{
  DEBUG_OUT<<"nickUnlockDisplay not implemented!\n";
}


void nickFlipDisplay()
{
  DEBUG_OUT<<"nickFlipDisplay not implemented!\n";
}


void nickSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
  nickColorLong=((r<<16)&0xff0000)+((b<<8)&0xff00)+(g&0xff);
  nickColorRed=r;
  nickColorGreen=g;
  nickColorBlue=b;
  //DEBUG_OUT<<"setForeground("<<nickColorLong<<",true)\n";
  aDefaultAWindow->setForeground(nickColorLong,true);
}


void nickSetColorLong(unsigned long c)
{
  nickColorLong=c;
  nickColorRed=getRGBRed24(c);
  nickColorGreen=getRGBGreen24(c);
  nickColorBlue=getRGBBlue24(c);
  //DEBUG_OUT<<"setForeground("<<nickColorLong<<",true)\n";
  aDefaultAWindow->setForeground(nickColorLong,true);
}


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
  //if((z1>0)&&(z2>0)) {
    clr=makeRGB24((int)(c1*fact*f->fRed),(int)(c1*fact*f->fGreen),(int)(c1*fact*f->fBlue));
    nickSetColorLong(clr);
    nickDrawLine(x1,y1,x2,y2);
  //}
  //if((z2>0)&&(z3>0)) {
    clr=makeRGB24((int)(c2*fact*f->fRed),(int)(c2*fact*f->fGreen),(int)(c2*fact*f->fBlue));
    nickSetColorLong(clr);
    nickDrawLine(x2,y2,x3,y3);
  //}
  //if((z3>0)&&(z1>0)) {
    clr=makeRGB24((int)(c3*fact*f->fRed),(int)(c3*fact*f->fGreen),(int)(c3*fact*f->fBlue));
    nickSetColorLong(clr);
    nickDrawLine(x3,y3,x1,y1);
  //}
}


void nickDrawMesh3D(AMesh3D *m,float x,float y,float z)
{
  DEBUG_OUT<<"nickDrawMesh3D Mesh has "<<m->getNumFaces()<<" faces...\n";
  nickLockDisplay();
  nickSetColorLong(0x404040);
  nickClearDisplay();
  AVertexI3D *dvs=m->getDrawVerts();
  for(unsigned int t=0;t<m->getNumFaces();t++) {
    AFace3D *f=m->getFaces()[t];
    //DEBUG_OUT<<"face: "<<f->a<<","<<f->b<<","<<f->c<<"\n";
    nickDrawFace3D(f,dvs);
  }
  nickUnlockDisplay();
  nickFlipDisplay();
}


void nickDrawScene3D(AScene3D *s)
{
  unsigned int nshapes=s->getNumMeshes();
  DEBUG_OUT<<"nickDrawScene3D Scene has "<<nshapes<<" meshes...\n";
  nickLockDisplay();
  nickSetColorLong(0x404040);
  nickClearDisplay();
  for(unsigned int t=0;t<nshapes;t++) {
    AMesh3D *m=s->getMeshes()[t];
    AVertexI3D *dvs=m->getDrawVerts();
    for(unsigned int tt=0;tt<m->getNumFaces();tt++) {
      AFace3D *f=m->getFaces()[tt];
      //DEBUG_OUT<<"face: "<<f->a<<","<<f->b<<","<<f->c<<"\n";
      nickDrawFace3D(f,dvs);
    }
  }
  nickUnlockDisplay();
  nickFlipDisplay();
}


#endif // NICK_USE_GUILIB
