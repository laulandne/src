
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <cstdlib>
#include <iostream>


#include "nicksdl.h"


#ifdef NICK_USE_OPENGL
#include <glut/nickglut.h>
#endif // NICK_USE_OPENGL


#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ABitmap.h>

#include <portable/nick3d/AScene3D.h>


#include <javacpp/JApplet.h>


#ifdef NICK_USE_SDL
unsigned int nickActualWidth=0;
unsigned int nickActualHeight=0;
SDL_Surface *nicksdl_theSurface=NULL;
SDL_Event nicksdl_theEvent;
#endif // NICK_USE_SDL


#ifdef NICK_USE_SDL_DRAWING
unsigned int nickColorRed=0;
unsigned int nickColorGreen=0;
unsigned int nickColorBlue=0;
unsigned long nickColorLong=0;
ABitmap *nicksdl_theFont=NULL;
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL
bool nickAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  DEBUG_OUT<<"nickAppInit(...)\n";
  bool ret=true;
  DEBUG_OUT<<"Going to call SDL_Init...\n";
  if(SDL_Init(SDL_INIT_VIDEO)<0) {
    printf("Unable to init SDL: %s\n",SDL_GetError());
    return false;
  }
  atexit(SDL_Quit);
  DEBUG_OUT<<"Going to call SDL_SetVideoMode...\n";
  Uint32 flags=SDL_HWSURFACE|SDL_DOUBLEBUF;
#ifdef NICK_USE_OPENGL
  flags|=SDL_OPENGL;
#endif // NICK_USE_OPENGL
  nicksdl_theSurface=SDL_SetVideoMode(theWidth,theHeight,theDepth,flags);
  if(!nicksdl_theSurface) {
    printf("Unable to set %dx%dx%d video: %s\n",theWidth,theHeight,theDepth,SDL_GetError());
    return false;
  }
  //
  SDL_LockSurface(nicksdl_theSurface);
  nickActualWidth=nicksdl_theSurface->w;
  nickActualHeight=nicksdl_theSurface->h;
  SDL_UnlockSurface(nicksdl_theSurface);
#ifdef NICK_USE_OPENGL
  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, theDepth);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  if(ret) ret=nickAppInitOpenGL(theWidth,theHeight,theDepth,argc,argv);
#endif // NICK_USE_OPENGL
  DEBUG_OUT<<"nickAppInit done.\n";
  return ret;
}
#endif // NICK_USE_SDL


#ifdef NICK_USE_SDL
void nickRegisterEventHandlers()
{
  DEBUG_OUT<<"nickRegisterEventHandlers(...)\n";
}
#endif // NICK_USE_SDL


#ifdef NICK_USE_SDL
void nickRunEventLoop()
{
  DEBUG_OUT<<"nickRunEventLoop(...)\n";
  DEBUG_OUT<<"Going to enter loop...\n";
  nickEvent je;
  int x,y;
  bool done=false;
  while(!done){
    while(SDL_PollEvent(&nicksdl_theEvent)) {
      switch(nicksdl_theEvent.type) {
        case SDL_QUIT:
          done=true;
          break;
        case SDL_KEYUP:
          je.val=nicksdl_theEvent.key.keysym.sym;
          break;
        case SDL_MOUSEBUTTONDOWN:
          x=nicksdl_theEvent.button.x;
          y=nicksdl_theEvent.button.y;
          je.x=x;  je.y=y;
          aDefaultApplet->mouseDown(&je,x,y);
          break;
        case SDL_MOUSEBUTTONUP:
          x=nicksdl_theEvent.button.x;
          y=nicksdl_theEvent.button.y;
          je.x=x;  je.y=y;
          aDefaultApplet->mouseUp(&je,x,y);
          break;
        case SDL_MOUSEMOTION:
          x=nicksdl_theEvent.motion.x;
          y=nicksdl_theEvent.motion.y;
          je.x=x;  je.y=y;
          aDefaultApplet->mouseDrag(&je,x,y);
          break;
        case SDL_VIDEORESIZE:
          x=nicksdl_theEvent.resize.w;
          y=nicksdl_theEvent.resize.h;
          je.x=x;  je.y=y;
          //aDefaultApplet->resize(x,y);
          break;
        case SDL_VIDEOEXPOSE:
          aDefaultApplet->repaint();
          break;
        case SDL_ACTIVEEVENT:
          aDefaultApplet->repaint();
          break;
      }
    }
    aDefaultApplet->repaint();
  }
  DEBUG_OUT<<"Done with loop.\n";
}
#endif // NICK_USE_SDL


#ifdef NICK_USE_SDL_DRAWING
void nickDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  int x1=xu1, x2=xu2, y1=yu1, y2=yu2;
  int x,y,deltax,deltay,temp,err,i,swap,s1,s2;
  x=x1;  y=y1;
  // FIXME: The unsigned coords might mess the next 4 lines up a little...
  deltax=abs(x2-x1);
  deltay=abs(y2-y1);
  if((x2-x1)<0) s1= -1; else s1=1;
  if((y2-y1)<0) s2= -1; else s2=1;
  if(deltay>deltax) {
    temp=deltax;  deltax=deltay;  deltay=temp;  swap=1;
  }
  else swap=0;
  err=2*deltay-deltax;
  for(i=1;i<=deltax;i++) {
    nickWritePixel(x,y);
    while(err>=0) {
      if(swap) x+=s1; else y+=s2;
      err-=2*deltax;
    }
    if(swap) y+=s2; else x+=s1;
    err+=2*deltay;
  }
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
// NOTE: pixel format is assumed...bad!
void nickWritePixel(unsigned int x,unsigned int y)
{
  if(x>nicksdl_theSurface->w) x=nicksdl_theSurface->w;
  if(y>nicksdl_theSurface->h) y=nicksdl_theSurface->h;
  unsigned int minBlit=1;
  unsigned int maxBlit=nicksdl_theSurface->format->BytesPerPixel;
  unsigned int bytesPerLine=nicksdl_theSurface->format->BytesPerPixel*nicksdl_theSurface->w;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned char *dest=(unsigned char *)nicksdl_theSurface->pixels+tx+(y*bytesPerLine);
  dest[2]=nickColorRed;
  dest[1]=nickColorGreen;
  dest[0]=nickColorBlue;
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
// NOTE: pixel format is assumed...bad!
unsigned long nickReadPixel(unsigned int x,unsigned int y)
{
  if(x>nicksdl_theSurface->w) x=nicksdl_theSurface->w;
  if(y>nicksdl_theSurface->h) y=nicksdl_theSurface->h;
  unsigned int minBlit=1;
  unsigned int maxBlit=nicksdl_theSurface->format->BytesPerPixel;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned int bytesPerLine=nicksdl_theSurface->format->BytesPerPixel*nicksdl_theSurface->w;
  unsigned char *src=(unsigned char *)nicksdl_theSurface->pixels+tx+(y*bytesPerLine);
  return makeRGB24(src[2],src[1],src[0]);
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
nickDrawableBitmap *nickGetDrawableBitmap(ABitmap *bm)
{
  return (nickDrawableBitmap *)bm;
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
void nickDrawImage(nickDrawableBitmap *tbm,unsigned int x,unsigned int y)
{
  ABitmap *bm=(ABitmap *)tbm;
  if(bm->getBitmapType()!=A_BITMAP_CHUNKY) { DEBUG_OUT<<"Can't draw, not chunky!\n"; exit(5); }
  if(bm->getDepth()!=24) { DEBUG_OUT<<"Can't draw, not 24 bit!\n"; exit(5); }
  unsigned int dw=bm->getWidth();
  if(dw>nicksdl_theSurface->w) dw=nicksdl_theSurface->w;
  unsigned int dh=bm->getHeight();
  if(dh>nicksdl_theSurface->h) dh=nicksdl_theSurface->h;
  //DEBUG_OUT<<"dw,dh="<<dw<<","<<dh<<"\n";
  unsigned int minBlit=1;
  unsigned int maxBlit=nicksdl_theSurface->format->BytesPerPixel;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned int dbpl=nicksdl_theSurface->format->BytesPerPixel*nicksdl_theSurface->w;
  unsigned int sbpl=bm->getBytesPerLine();
  //DEBUG_OUT<<"maxBlit="<<maxBlit<<" tx="<<tx<<" dbpl="<<dbpl<<" sbpl="<<sbpl<<"\n";
  unsigned char *isrc=bm->getPlane(0);
  unsigned char *idest=(unsigned char *)nicksdl_theSurface->pixels+tx+(y*dbpl);
  unsigned char *src=isrc;
  unsigned char *dest=idest;
  for(unsigned int cy=0;cy<dh-1;cy++) {
    //DEBUG_OUT<<"Copying line "<<cy<<"\n";
    src+=sbpl;
    dest+=dbpl;
    memcpy(dest,src,sbpl);
  }
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
void nickCopyBlock(nickDrawableBitmap *tbm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
  ABitmap *bm=(ABitmap *)tbm;
  if(bm->getBitmapType()!=A_BITMAP_CHUNKY) { DEBUG_OUT<<"Can't draw, not chunky!\n"; exit(5); }
  if(bm->getDepth()!=24) { DEBUG_OUT<<"Can't draw, not 24 bit!\n"; exit(5); }
  unsigned int dw=bm->getWidth();
  if(dw>nicksdl_theSurface->w) dw=nicksdl_theSurface->w;
  unsigned int dh=bm->getHeight();
  if(dh>nicksdl_theSurface->h) dh=nicksdl_theSurface->h;
  //DEBUG_OUT<<"dw,dh="<<dw<<","<<dh<<"\n";
  unsigned int minBlit=1;
  unsigned int maxBlit=nicksdl_theSurface->format->BytesPerPixel;
  unsigned int tx=(dx/minBlit)*maxBlit;
  unsigned int dbpl=nicksdl_theSurface->format->BytesPerPixel*nicksdl_theSurface->w;
  unsigned int sbpl=bm->getBytesPerLine();
  //DEBUG_OUT<<"maxBlit="<<maxBlit<<" tx="<<tx<<" dbpl="<<dbpl<<" sbpl="<<sbpl<<"\n";
  unsigned char *src=bm->getPlane(0);
  unsigned char *dest=(unsigned char *)nicksdl_theSurface->pixels+tx+(dy*dbpl);
  for(unsigned int cy=0;cy<dh-1;cy++) {
    //DEBUG_OUT<<"Copying line "<<cy<<"\n";
    src+=sbpl;
    dest+=dbpl;
    memcpy(dest,src,sbpl);
  }
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
void nickDrawString(char *st,unsigned int x,unsigned int y)
{
  DEBUG_OUT<<"nickDrawString not implemented!\n";
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
void nickLoadFont(ABitmap *bm,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh)
{
  DEBUG_OUT<<"nickLoadFont not implemented!\n";
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
APalette *nickGetCurPalette()
{
  //DEBUG_OUT<<"nickGetCurPalette not implemented!\n";
  return NULL;
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
void nickClearDisplay()
{
  SDL_FillRect(nicksdl_theSurface,0,SDL_MapRGB(nicksdl_theSurface->format,nickColorRed,nickColorGreen,nickColorBlue));
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL
void nickLockDisplay()
{
  SDL_LockSurface(nicksdl_theSurface);
}
#endif // NICK_USE_SDL


#ifdef NICK_USE_SDL
void nickUnlockDisplay()
{
  SDL_UnlockSurface(nicksdl_theSurface);
}
#endif // NICK_USE_SDL


#ifdef NICK_USE_SDL
void nickFlipDisplay()
{
#ifdef NICK_USE_OPENGL
  glFlush();
  SDL_GL_SwapBuffers();
  //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#endif // NICK_USE_OPENGL
  SDL_Flip(nicksdl_theSurface);
}
#endif // NICK_USE_SDL


#ifdef NICK_USE_SDL_DRAWING
void nickSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
  nickColorLong=((r<<16)&0xff0000)+((b<<8)&0xff00)+(g&0xff);
  nickColorRed=r;
  nickColorGreen=g;
  nickColorBlue=b;
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
void nickSetColorLong(unsigned long c)
{
  nickColorLong=c;
  nickColorRed=getRGBRed24(c);
  nickColorGreen=getRGBGreen24(c);
  nickColorBlue=getRGBBlue24(c);
}
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
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
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
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
#endif // NICK_USE_SDL_DRAWING


#ifdef NICK_USE_SDL_DRAWING
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
#endif // NICK_USE_SDL_DRAWING

