#ifndef NICKGUILIB_H
#define NICKGUILIB_H

#ifdef NICK_USE_GUILIB


#include <guilib/ADisplay.h>
#include <guilib/AWindow.h>
#include <guilib/AEvent.h>
#include <guilib/AApplicationLoop.h>
#include <guilib/AApp.h>
#include <guilib/APrivateBitmap.h>


class nickDrawableBitmap;

class AFace3D;
class AVertexF3D;
class AVertexI3D;
class AMesh3D;
class AScene3D;


extern "C" {


struct nickEvent {
  int x,y;
  int val;
  int type;
};


extern unsigned int nickActualWidth;
extern unsigned int nickActualHeight;


bool nickAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);

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
void nickDrawMesh3D(AMesh3D *m,float x,float y,float z);
void nickDrawScene3D(AScene3D *s);

void nickRegisterEventHandlers();
void nickRunEventLoop();

};



#endif // NICK_USE_GUILIB

#endif // NICKGUILIB_H
