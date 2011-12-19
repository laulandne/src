
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>


#ifdef ASYS_X11
#include <X11/Xutil.h>
#endif // ASYS_X11_BASED

#ifdef ASYS_WINDOWS
#include <guilib/alib_win.h>
#define ASYS_NEED_MAP_VIDEO 1
#endif // ASYS_WINDOWS

#ifdef ASYS_DOS
#include <guilib/alib_dos.h>
#define ASYS_NEED_MAP_VIDEO 1
#endif // ASYS_DOS

#ifdef ASYS_AMIGA
#include <graphics/gfx.h>
#include <exec/memory.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/exec_protos.h>
#endif // ASYS_AMIGA


#include <guilib/APrivateBitmap.h>
#include <guilib/ADisplay.h>
#include <guilib/AWindow.h>

#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/AConvertingBitLine.h>


#ifndef ASYS_DOS
#define mapVideoMem(a,b) (a)
#endif // ASYS_DOS


////////////////////////////////////////////////////////////////////////////////
//  Global junk
////////////////////////////////////////////////////////////////////////////////

static unsigned char bitMask8[8]={ 1,2,4,8,16,32,64,128 };

static unsigned char bitMask24[24]={
 1,2,4,8,16,32,64,128,
 1,2,4,8,16,32,64,128,
 1,2,4,8,16,32,64,128
};

static unsigned char offset24[24]={
 1,1,1,1,1,1,1,1,
 2,2,2,2,2,2,2,2,
 3,3,3,3,3,3,3,3
};


#define ABitmapMin(a,b) ((a)<(b)?(a):(b))


////////////////////////////////////////////////////////////////////////////////
//  APrivateBitmap Class
////////////////////////////////////////////////////////////////////////////////

// TODO: Probably needs a lot more work...
APrivateBitmap::APrivateBitmap(unsigned char *theBits,AWindowAbstract *someWindow)
{
  DEBUG_OUT<<"APrivateBitmap::APrivateBitmap(theBits,someWindow)\n";
  init();
  width=someWindow->getWidth();  height=someWindow->getHeight();
  depth=someWindow->getParent()->getDepth();  nPlanes=someWindow->getParent()->getNPlanes();
  calc();
  plane[0]=theBits;
  needFree[0]=false;
  if(!trueColor) colors=new APalette((unsigned int)bits2Bytes(depth));
  createPrivateBits(true);
}


APrivateBitmap::APrivateBitmap(ADisplay *someDisplay, ADisplayMode *d)
{
  DEBUG_OUT<<"APrivateBitmap::APrivateBitmap(someDisplay,d)\n";
  init();
  width=someDisplay->getWidth();  height=someDisplay->getHeight();
  depth=someDisplay->getDepth();  nPlanes=someDisplay->getNPlanes();
  x=0;  y=0;
  interleaved=false;
  bitmapType=someDisplay->getBitmapType();
  if(d) { planeSize=d->vMemSize;  bytesPerLine=d->bytesPerLine; }
  calc();
  foreground=1;  background=0;
  //NOTE: all planes will point to the same area of memory
  for(unsigned int t=0;t<nPlanes;t++) {
    if(d) plane[t]=mapVideoMem((unsigned char *)d->vMemPtr,planeSize);
    if(!plane[t]) plane[t]=(unsigned char *)malloc(planeSize);
    needFree[t]=false;
  }
  if(someDisplay->getColors()) colors=new APalette(0,depth);
  createPrivateBits(true);
}


APrivateBitmap::APrivateBitmap()
{
  DEBUG_OUT<<"APrivateBitmap::APrivateBitmap()\n";
  init();
  // NOTE: Bounds are invalid here...parent class will take care of it...
}


APrivateBitmap::APrivateBitmap(unsigned int w, unsigned int h, unsigned int d,unsigned int np)
{
  DEBUG_OUT<<"APrivateBitmap::APrivateBitmap("<<w<<","<<h<<","<<d<<","<<np<<")\n";
  init();
  width=w;  height=h;  depth=d;  nPlanes=np;
  calc();
  bytesPerLine=calcBPL(4);  // Force to multiple of 32-bits
  planeSize=(unsigned long)bytesPerLine*(unsigned long)height;
  for(unsigned int t=0;t<np;t++) {
    // TODO: Doesn't this round everything up to 1024?
    unsigned long num=planeSize/1024;
    if((num*1024)<planeSize) num++;
    plane[t]=(unsigned char *)acalloc((int)num,1024);
    if(!plane[t]) {
      aDefaultStringHelper.fatal("cons: Couldn't get memory for bitmap plane!");
    }
    needFree[t]=true;
  }
  if(d<9) {
    colors=new APalette(2<<(d-1));
  }
}


APrivateBitmap::APrivateBitmap(unsigned char *data, unsigned int w, unsigned int h,unsigned int d, unsigned int np)
{
  DEBUG_OUT<<"APrivateBitmap::APrivateBitmap("<<w<<","<<h<<","<<d<<","<<np<<")\n";
  init();
  width=w;  height=h;  depth=d;  nPlanes=np;
  calc();
  bytesPerLine=calcBPL(4);  // Force to multiple of 32-bits
  planeSize=(unsigned long)bytesPerLine*(unsigned long)height;
  for(unsigned int t=0;t<np;t++) {
    plane[t]=data;
    needFree[t]=false;
  }
}


APrivateBitmap::APrivateBitmap(ABitmap *bm)
{
  DEBUG_OUT<<"APrivateBitmap::APrivateBitmap(ABitmap *bm)\n";
  init();
  width=bm->getWidth();  height=bm->getHeight();  depth=bm->getDepth();  nPlanes=bm->getNPlanes();
  bytesPerLine=calcBPL(4);  // Force to multiple of 32-bits
  planeSize=(unsigned long)bytesPerLine*(unsigned long)height;
  calc();
  for(unsigned int t=0;t<nPlanes;t++) {
    // TODO: Doesn't this round everything up to 1024?
    unsigned long num=planeSize/1024;
    if((num*1024)<planeSize) num++;
    plane[t]=(unsigned char *)acalloc((int)num,1024);
    if(!plane[t]) {
      aDefaultStringHelper.fatal("cons: Couldn't get memory for bitmap plane!");
    }
    memcpy(plane[t],bm->getPlane(t),planeSize);
    needFree[t]=true;
  }
  if(depth<9) {
    colors=new APalette(bm->getColors());
  }
}


APrivateBitmap::~APrivateBitmap()
{
  DEBUG_OUT<<"APrivateBitmap::~APrivateBitmap()\n";
  freeBitmap();
}


void APrivateBitmap::freeBitmap()
{
  DEBUG_OUT<<"APrivateBitmap::freeBitmap()\n";
  if(privateBitsDone) freePrivateBits();
  for(unsigned int t=0;t<nPlanes;t++) {
    if(plane[t]) if(needFree[t]) free(plane[t]);
    plane[t]=(unsigned char *)NULL;
    needFree[t]=false;
  }
  if(ownColors) {
    if(colors) delete colors;
    colors=(APalette *)NULL;
    ownColors=false;
  }
}


void APrivateBitmap::duplicate(APrivateBitmap *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  copyFields(source);
  for(unsigned int t=0;t<nPlanes;t++) {
    plane[t]=source->plane[t];
    needFree[t]=false; // Since we don't own them, source does...
  }
  colors=source->colors;
  ownColors=false; // Since we don't own them, source does...
}


void APrivateBitmap::copyFields(APrivateBitmap *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  width=source->width;  height=source->height;
  depth=source->depth;  nPlanes=source->nPlanes;
  interleaved=source->interleaved;
  calc();
  // ...since whatever was there is now invalid...
  for(unsigned int t=0;t<nPlanes;t++) {
    plane[t]=(unsigned char *)NULL; needFree[t]=false;
  }
  colors=(APalette *)NULL;
}


void APrivateBitmap::writeLine(unsigned int lineNum, unsigned char *lineData,
  unsigned int lineLen, unsigned int planeNum)
{
  unsigned int copyLen=ABitmapMin(lineLen,bytesPerLine);
  if(bitmapType==A_BITMAP_CHUNKY) planeNum=0;
  unsigned char *src=lineData;
  unsigned char *dest=plane[planeNum]+
    (unsigned long)((unsigned long)bytesPerLine*(unsigned long)lineNum);
  if(!checkLine(lineNum)) return;
  if(!checkPlane(planeNum)) return;
  memcpyl(dest,src,copyLen);
}


void APrivateBitmap::readLine(unsigned int lineNum, unsigned char *lineData,
  unsigned int lineLen, unsigned int planeNum)
{
  unsigned int copyLen=ABitmapMin(lineLen,bytesPerLine);
  if(bitmapType==A_BITMAP_CHUNKY) planeNum=0;
  unsigned char *dest=lineData;
  unsigned char *src=plane[planeNum]+
    (unsigned long)((unsigned long)bytesPerLine*(unsigned long)lineNum);
  if(!checkLine(lineNum)) return;
  if(!checkPlane(planeNum)) return;
  memcpyl(dest,src,copyLen);
}


AConvertingBitLine *APrivateBitmap::getLine(unsigned int row)
{
  unsigned char *source;
  if(!checkLine(row)) return (AConvertingBitLine *)NULL;
  AConvertingBitLine *temp=new AConvertingBitLine((ABitmap *)this);
  if(!temp) {
    DEBUG_OUT<<"Couldn't new in getLine!\n";
    return (AConvertingBitLine *)NULL;
  }
  for(unsigned int p=0;p<nPlanes;p++) {
    if(!plane[p]) { delete temp; return (AConvertingBitLine *)NULL; }
    source=plane[p]+
      (unsigned long)((unsigned long)bytesPerLine*(unsigned long)row);
    temp->setPlane(p,source);
  }
  // Just copy the palette...
  temp->colors=colors;
  ownColors=false;
  return temp;
}


AConvertingBitLine *APrivateBitmap::getCopyOfLine(unsigned int row)
{
  unsigned char *source;
  if(!checkLine(row)) return (AConvertingBitLine *)NULL;
  AConvertingBitLine *temp=new AConvertingBitLine(width,depth,nPlanes);
  if(!temp) {
    DEBUG_OUT<<"Couldn't new in getCopyOfLine!\n";
    return (AConvertingBitLine *)NULL;
  }
  for(unsigned int c=0;c<bytesPerLine;c++) {
    for(unsigned int p=0;p<nPlanes;p++) {
      //if(!plane[p]) { delete temp; return (AConvertingBitLine *)NULL; }
      source=plane[p]+c+
        (unsigned long)((unsigned long)bytesPerLine*(unsigned long)row);
      temp->plane[p][c]=*source;
    }
  }
  // Just copy the palette...
  temp->colors=colors;
  ownColors=false;
  return temp;
}


void APrivateBitmap::copyLineTo(AConvertingBitLine *dest,unsigned int row)
{
  if(!dest) { DEBUG_OUT<<"no dest!\n"; return; }
  bool err=false;
  if(dest->depth!=depth) err=true;
  if(dest->nPlanes!=nPlanes) err=true;
  if(dest->bitmapType!=bitmapType) err=true;
  if(err) { DEBUG_OUT<<"mismatched lines in copyLineTo\n"; return; }
  if(!checkLine(row)) return;
  for(unsigned int p=0;p<nPlanes;p++) {
    dest->plane[p]=plane[p]+
      (unsigned long)((unsigned long)bytesPerLine*(unsigned long)row);
  }
}


void APrivateBitmap::copyToLine(AConvertingBitLine *dest,unsigned int row)
{
  if(!dest) { DEBUG_OUT<<"no dest!\n"; return; }
  bool err=false;
  if(dest->depth!=depth) err=true;
  if(dest->nPlanes!=nPlanes) err=true;
  if(dest->bitmapType!=bitmapType) err=true;
  if(err) { DEBUG_OUT<<"mismatched lines in copyToLine\n"; return; }
  if(!checkLine(row)) return;
  char bite;
  for(unsigned int c=0;c<bytesPerLine;c++) {
    for(unsigned int p=0;p<nPlanes;p++) {
      bite=plane[p][c+(unsigned long)((unsigned long)bytesPerLine*(unsigned long)row)];
      dest->plane[p][c]=bite;
      //DEBUG_OUT<<bite<<"+";
    }
  }
  //DEBUG_OUT<<"\n";
}


void APrivateBitmap::copyFromLine(AConvertingBitLine *source,unsigned int row)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  bool err=false;
  if(source->depth!=depth) err=true;
  if(source->nPlanes!=nPlanes) err=true;
  if(source->bitmapType!=bitmapType) err=true;
  if(err) { DEBUG_OUT<<"mismatched lines in copyFromLine\n"; return; }
  if(!checkLine(row)) return;
  char bite;
  for(unsigned int c=0;c<bytesPerLine;c++) {
    for(unsigned int p=0;p<nPlanes;p++) {
      bite=source->plane[p][c];
      plane[p][c+(unsigned long)((unsigned long)bytesPerLine*(unsigned long)row)]=bite;
      //DEBUG_OUT<<bite<<"-";
    }
  }
  //DEBUG_OUT<<"\n";
}


// NOTE: Be very careful what you pass to this function...
// be safe and set it back to what it was...
void APrivateBitmap::setPlane(unsigned int p, unsigned char *data)
{
  if(plane[p]) {
    if(needFree[p]) free(plane[p]);
    /*
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Can't setPlane "<<p<<", data already there!\n";
#endif // DEBUG_VERBOSE
    return;
    */
  }
  plane[p]=data;
  needFree[p]=false;
}


void APrivateBitmap::copyPlane(APrivateBitmap *source,unsigned int p)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  unsigned int theight=ABitmapMin(height,source->getHeight());
  unsigned int tlen;
  if(p>source->getNPlanes()) {
    DEBUG_OUT<<"Can't copy plane "<<p<<"!\n";
    return;
  }
  for(unsigned int t=0;t<theight;t++) {
    AConvertingBitLine *l=source->getLine(t);
    tlen=ABitmapMin(bytesPerLine,l->getBytesPerLine());
    writeLine(t,l->getPlane(p),tlen,0);
    //delete l;
  }
}


// FIXME: This function needs to be cleaned up a lot
void APrivateBitmap::copyBlock(APrivateBitmap *source, unsigned int sx, unsigned int sy,
    unsigned int dx,unsigned int dy,unsigned bw,unsigned bh)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  unsigned int sourceBytesPerLine=source->bytesPerLine;
  unsigned int drawDepth=source->nPlanes;
  unsigned int drawHeight=bh;
  unsigned int ssx=sx;
  unsigned int drawWidth=bw;
  unsigned int row,p;
  unsigned char *src,*dest;
  // FIXME: Minimal clipping...take dx,dy into account!
  if(drawHeight>height) drawHeight=height;
  if(drawDepth>nPlanes) drawDepth=nPlanes;
  drawWidth=(drawWidth/source->minBlit)*source->maxBlit;
  if(drawWidth>bytesPerLine) drawWidth=bytesPerLine;
  dx=(dx/minBlit)*maxBlit;
  ssx=(ssx/source->minBlit)*source->maxBlit;
  if(interleaved) {
    unsigned int ssy=sy/2;  dy=dy/2;
    drawHeight=bh/2;
    sourceBytesPerLine*=2;
    // Even lines
    dest=plane[0]+(dy*bytesPerLine)+dx;
    src=(source->plane[0])+(ssy*sourceBytesPerLine)+ssx;
    for(row=0;row<drawHeight;row++) {
      memcpy(dest,src,drawWidth);
      dest+=bytesPerLine; src+=sourceBytesPerLine;
    }
    // Odd lines
    // Borland C++ 3.1 has a problem with the 8192 constant...
    dest=plane[0]+(dy*bytesPerLine)+dx+8192;
    src=(source->plane[0])+(ssy*sourceBytesPerLine)+ssx+(sourceBytesPerLine/2);
    for(row=0;row<drawHeight;row++) {
      memcpy(dest,src,drawWidth);
      dest+=bytesPerLine; src+=sourceBytesPerLine;
    }
    return;
  }
  switch(bitmapType) {
    case A_BITMAP_PLANES:
      for(p=0;p<drawDepth;p++) {
        dest=plane[p]+(dy*bytesPerLine)+dx;
        src=(source->plane[p])+(source->y*sourceBytesPerLine)+ssx;
        for(row=0;row<drawHeight;row++) {
          memcpy(dest,src,drawWidth);
          dest+=bytesPerLine; src+=sourceBytesPerLine;
        }
      }
      break;
    case A_BITMAP_CHUNKY:
      dest=plane[0]+(dy*bytesPerLine)+dx;
      src=(source->plane[0])+(sy*sourceBytesPerLine)+ssx;
      for(row=0;row<drawHeight;row++) {
        memcpy(dest,src,drawWidth);
        dest+=bytesPerLine; src+=sourceBytesPerLine;
      }
      break;
    default:
      aDefaultStringHelper.fatal("bitmap::draw unknown bitmap format!\n");
      break;
  }
}


void APrivateBitmap::init()
{
  ABitmap::init();
  initPrivateBits();
  privateBitsDone=false;
  privateLock=false;
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  winBits=(HBITMAP)NULLHANDLE;
  bmi=(BITMAPINFO *)NULL;
  extra=(char *)NULL;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  winBits=(HBITMAP)NULL;
#ifdef ASYS_OS2_32
  bmi=(BITMAPINFO2 *)NULL;
#else
  bmi=(BITMAPINFO *)NULL;
#endif // ASYS_OS2_32
#endif // ASYS_OS2
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  //ThePM=(__PixMapPtr)NULL;
  ctab=(ColorTable **)NULL;
  theWorld=(GWorldPtr)NULL;
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  TheBM=NULL;
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  ThePM=(Pixmap)0;  TheImage=(XImage *)NULL;  xbits=(unsigned char *)NULL;
  xbitsize=0;  xplanesize=0;  xbpl=0;
#endif // ASYS_X11
#ifdef ASYS_DOS
  displayPrivate=false;
  needPlaneSwitch=false;  needBankSwitch=false;
#endif // ASYS_DOS
#ifdef ASYS_DOS16
  pageFunc=(void *)NULL;
#endif // ASYS_DOS16
#ifdef ASYS_PALM
  bits=(BitmapType *)NULL;
  win=(WinHandle)NULL;
#endif // ASYS_PALM
}


void APrivateBitmap::initPrivateBits()
{
  DEBUG_OUT<<"APrivateBitmap::initPrivateBits()\n";
  privateLock=false;
  verifyLock=false;
  okayDokey=false;
  privateBitsDone=false;
  allPlanesSame=false;
  //needsUnmapping=false;
  //bitsHint=0;
  for(int t=0;t<NUM_PLANES;t++) {
    plane[t]=(unsigned char *)NULL;
  }
  foreground=0;  background=0xffffffL;
  colors=(APalette *)NULL;
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  winBits=0;  bmi=(BITMAPINFO *)NULL;
  extra=(char *)NULL;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  winBits=0;
#ifdef ASYS_OS2_32
  bmi=(BITMAPINFO2 *)NULL;
#else
  bmi=(BITMAPINFO *)NULL;
#endif // ASYS_OS2_32
  point[0].x=0;  point[0].y=0;
  point[1].x=0;  point[1].y=0;
  point[2].x=0;  point[2].y=0;
#endif // ASYS_OS2
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  //ThePM=(__PixMapPtr)NULL;
  ctab=(CTabHandle)NULL;
  theWorld=(GWorldPtr)NULL;
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  TheBM=NULL;
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  ThePM=(Pixmap)0;  TheImage=(XImage *)NULL;  xbits=(unsigned char *)NULL;
  xbitsize=0;  xplanesize=0;  xbpl=0;
#endif // ASYS_X11
#ifdef ASYS_DOS
  displayPrivate=false;
  needPlaneSwitch=false;  needBankSwitch=false;
#endif // ASYS_DOS
#ifdef ASYS_PALM
  bits=(BitmapType *)NULL;
  win=(WinHandle)NULL;
#endif // ASYS_PALM
}


void APrivateBitmap::freePrivateBits()
{
  DEBUG_OUT<<"APrivateBitmap::freePrivateBits()\n";
  if(!privateBitsDone) {
    //DEBUG_OUT<<"Can't free privateBits that aren't done!\n";
    return;
  }
  //if(parent) { DEBUG_OUT<<"(won't fpbs, have a parent)\n"; return; }
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  if(winBits) DeleteObject(winBits);
  if(bmi) free(bmi);
  bmi=(BITMAPINFO *)NULL;  winBits=(HBITMAP)NULLHANDLE;
  extra=(char *)NULL;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  if(winBits) GpiDeleteBitmap(winBits);
  if(bmi) free(bmi);
#ifdef ASYS_OS2_32
  bmi=(BITMAPINFO2 *)NULL;
#else
  bmi=(BITMAPINFO *)NULL;
#endif // ASYS_OS2_32
  winBits=(HBITMAP)NULL;
#endif // ASYS_OS2
#ifdef ASYS_MAC
  //if(ThePM) free((char *)ThePM);
  //ThePM=(PixMapPtr)NULL;
  // FIXME: should I free the ctab here?
#ifdef USE_QUARTZ
#else
  ctab=(CTabHandle)NULL;
  if(theWorld) DisposeGWorld(theWorld);
  theWorld=(GWorldPtr)NULL;
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  // FIXME: use 3.x FreeBitmap (or whatever its called)
  if(TheBM) {
    for(unsigned int t=0;t<8;t++)  {
      if(TheBM->Planes[t]) FreeMem(TheBM->Planes[t],planeSize);
      TheBM->Planes[t]=NULL;
    }
    free(TheBM);
  }
  TheBM=NULL;
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  if(TheImage) XDestroyImage(TheImage);
  // I Think X frees these for us in XDestroyImage
  //if(xbits) free(xbits);
  ThePM=(Pixmap)0;  TheImage=(XImage *)NULL;  xbits=(unsigned char *)NULL;
  xbitsize=0; xplanesize=0;  xbpl=0;
#endif // ASYS_X11
#ifdef ASYS_PALM
  if(win) WinDeleteWindow(win,false);
#ifdef __PALMOS_H__
  if(aThisApp.hasThreePointFive) {
    if(bits) BmpDelete(bits);
  }
#endif // __PALMOS_H__
  bits=(BitmapType *)NULL;
#endif // ASYS_PALM
  privateBitsDone=false;
  privateLock=false;  // Just in case...
}


void APrivateBitmap::copyPrivateBits(APrivateBitmap *source)
{
  DEBUG_OUT<<"APrivateBitmap::copyPrivateBits()\n";
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  source->updatePrivateBits();
  okayDokey=source->okayDokey;
  needsRGBReorder=source->needsRGBReorder;
#ifdef ASYS_X11
  ThePM=source->ThePM;  TheImage=source->TheImage;  xbpl=source->xbpl;
  xbits=source->xbits;  xbitsize=source->xbitsize;  xplanesize=source->xplanesize;
#endif // ASYS_X11
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  //ThePM=source->ThePM;
  ctab=source->ctab;
  theWorld=source->theWorld;
  if(!theWorld) { DEBUG_OUT<<"cpb: no theWorld!\n"; }
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_OS2
  winBits=source->winBits;  bmi=source->bmi;
  point[2]->x=x;  point[2]->y=source->height-y-height;
  point[3]->x=x+width;  point[3]->y=source->height-y;
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  winBits=source->winBits;  bmi=source->bmi;
  extra=source->extra;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_AMIGA
  TheBM=source->TheBM;
#endif
#ifdef ASYS_PALM
  bits=source->bits;
  win=source->win;
#endif // ASYS_PALM
  privateBitsDone=source->privateBitsDone;
}


// FIXME: This function assumes only the bits will change and not
// the dimensions, depth, etc of the bitmap!
void APrivateBitmap::updatePrivateBits()
{
  DEBUG_OUT<<"APrivateBitmap::updatePrivateBits()\n";
  if(privateLock) {
    //DEBUG_OUT.warning("Already updating privateBits!\n"); return;
  }
  if(!privateBitsDone) {
    // may have to set privateLock to false here...
    createPrivateBits(true);
    DEBUG_OUT<<"---updatePB() (createPrivateBits)\n";
    return;
   }
  privateLock=true;
#ifdef ASYS_X11
  DEBUG_OUT<<"updating X bits...\n";
  ADisplay *aDisplay=new ADisplay;
  Display *theDisplay=aDisplay->getDisplay();
  XFreePixmap(theDisplay,ThePM);
  for(unsigned int t=0;t<nPlanes;t++) {
    for(unsigned int tt=0;tt<height;tt++) {
      if(ImageByteOrder(theDisplay))
        memcpy(xbits+(t*xplanesize)+tt*xbpl,
          plane[nPlanes-t-1]+tt*bytesPerLine,bytesPerLine);
      else
        memcpy(xbits+(t*xplanesize)+tt*xbpl,
          plane[nPlanes-t-1]+tt*bytesPerLine,bytesPerLine);
    }
    snarfAndMungeBits((char *)xbits+(t*xplanesize),xplanesize, BitmapUnit(theDisplay),!BitmapBitOrder(theDisplay));
  }
  TheImage->data=(char *)xbits;
  TheImage->byte_order=ImageByteOrder(theDisplay);
  TheImage->bitmap_unit=BitmapUnit(theDisplay);
  ThePM=XCreatePixmap(theDisplay,DefaultRootWindow(theDisplay),width,height,depth);
  delete aDisplay;
#endif // ASYS_X11
#ifdef ASYS_OS2
  GpiDeleteBitmap(winBits);
  HPS hps=WinGetPS(HWND_DESKTOP);
#ifdef ASYS_OS2_32
  winBits=GpiCreateBitmap(hps,(BITMAPINFOHEADER2 *)
    bmi,CBM_INIT,(PBYTE)plane[0],(BITMAPINFO2 *)bmi);
#else
  winBits=GpiCreateBitmap(hps,(BITMAPINFOHEADER *)
    bmi,CBM_INIT,(PBYTE)plane[0],(BITMAPINFO *)bmi);
#endif // ASYS_OS2_32
  ERRORID err=WinGetLastError(aThisApp.ahAB);
  // FIXME: I should interpret common errors here.
  if(err) //DEBUG_OUT.warning("Problem with bitmap (")<<aThisApp.myHex(err)<<" in PMERR.H)!\n";
  WinReleasePS(hps);
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  okayDokey=true;
  HDC hdc=GetDC((HWND)NULLHANDLE);
#ifdef ASYS_WIN32CE
  if(extra) {
    for(unsigned int t=0;t<planeSize;t++) extra[t]=plane[0][t];
  }
  else DEBUG_OUT<<"extra is NULL!\n";
#else
  SetDIBits(hdc,winBits,0,height,(LPSTR)plane[0],bmi,DIB_RGB_COLORS);
#endif // ASYS_WIN32CE
  ReleaseDC((HWND)NULLHANDLE,hdc);
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_AMIGA
  if(nPlanes>8) convert(8,A_BITMAP_PLANES,(APalette *)NULL);
  // FIXME: is there a better/safer way to do this under 3.x?
  for(unsigned int t=0;t<nPlanes;t++)
    if((plane[t])&&(TheBM->Planes[t]))
      memcpy(TheBM->Planes[t],plane[t],planeSize);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
  OSErr err;
  macBlastBits();
  unsigned int ret=0;
#ifdef USE_QUARTZ
#else
  if(theWorld) {
    Rect bitmapBox;
    bitmapBox.top=0;  bitmapBox.left=0;
    bitmapBox.right=width; bitmapBox.bottom=height;
    DEBUG_OUT<<"Going to UpdateGWorld...\n";
    ret=UpdateGWorld(&theWorld,depth,&bitmapBox,(ColorTable **)NULL,(GDevice **)NULL,clipPix);
    if(ret&gwFlagErr) {
      err=QDError();
      if(err!=noErr) {
        DEBUG_OUT<<"upb: A Quickdraw error ("<<err<<") occured!\n";
      }
    }
  }
  else DEBUG_OUT<<"upb: No GWorld!\n";
#endif // USE_QUARTZ
#endif // ASYS_MAC
  privateLock=false;
  modified=false;
  //DEBUG_OUT<<"---updatePB()\n";
}


void APrivateBitmap::createPrivateBits(bool yeah)
{
  DEBUG_OUT<<"APrivateBitmap::createPrivateBits()\n";
  if(!yeah) { DEBUG_OUT<<"Denying call to createPrivateBits!\n";  return; }
  if(privateLock) { DEBUG_OUT<<"Already creating privateBits!\n"; return; }
  if(privateBitsDone) freePrivateBits();
  privateLock=true;
  checkPaletteSize();
#ifdef ASYS_X11
  DEBUG_OUT<<"Creating x bits...\n";
  ADisplay *aDisplay=(ADisplay *)NULL;
  if(aDefaultADisplay) aDisplay=aDefaultADisplay;
  else aDisplay=new ADisplay;
  Display *theDisplay=aDisplay->getDisplay();
  Screen *s=DefaultScreenOfDisplay(theDisplay);
  Window dw=DefaultRootWindow(theDisplay);
  Visual *v=DefaultVisualOfScreen(s);
  unsigned int sdepth=DefaultDepthOfScreen(s);
  DEBUG_OUT<<"sdepth is "<<sdepth<<"\n";
  if(bitmapType==A_BITMAP_CHUNKY) convert(sdepth,A_BITMAP_PLANES,(APalette *)NULL);
  if(sdepth!=depth) convert(sdepth,A_BITMAP_PLANES,(APalette *)NULL);
  if(needsRGBReorder) reorderRGBPlanes();
  if(nPlanes>8) adjustRGBPlanes();
  DEBUG_OUT<<"Image is "<<width<<"x"<<height<<" "<<depth<<"("<<nPlanes<<")\n";
  unsigned int fg=WhitePixelOfScreen(s);
  unsigned int bg=BlackPixelOfScreen(s);
  DEBUG_OUT<<"BitmapPad is "<<BitmapPad(theDisplay)<<" BitmapUnit is "<<BitmapUnit(theDisplay)<<"\n";
  TheImage=XCreateImage(theDisplay,v,depth,XYPixmap,0,(char *)NULL,width,height,BitmapPad(theDisplay),0);
  xbpl=TheImage->bytes_per_line;
  xplanesize=xbpl*height;
  xbitsize=xplanesize*nPlanes;
  DEBUG_OUT<<"xplanesize is "<<xplanesize<<"\n";
  unsigned int cbpl=bytesPerLine;
  if(xbpl<cbpl) cbpl=xbpl;
  DEBUG_OUT<<"X says bytesPerLine is "<<xbpl<<" so xbitsize is "<<xbitsize<<"\n";
  xbits=(unsigned char *)malloc(xbitsize);
  if(!xbits) {
    aDefaultStringHelper.fatal("Couldn't get xbits!\n");
  }
  for(unsigned int t=0;t<nPlanes;t++) {
    for(unsigned int tt=0;tt<height;tt++) {
      memcpy(xbits+(t*xplanesize)+tt*xbpl,plane[nPlanes-t-1]+tt*bytesPerLine,cbpl);
      //DEBUG_OUT<<"plane "<<t<<" line "<<tt<<":";
      //for(unsigned q=0;q<cbpl;q++) DEBUG_OUT<<" "<<(int)plane[nPlanes-t-1][q];
      //DEBUG_OUT<<"\n";
    }
    snarfAndMungeBits((char *)xbits+(t*xplanesize),xplanesize,BitmapUnit(theDisplay),!BitmapBitOrder(theDisplay));
  }
  TheImage->data=(char *)xbits;
  TheImage->byte_order=ImageByteOrder(theDisplay);
  TheImage->bitmap_unit=BitmapUnit(theDisplay);
  DEBUG_OUT<<"Going to XCreatePixmap...\n";
  ThePM=XCreatePixmap(theDisplay,dw,width,height,sdepth);
  XGCValues gcv;  gcv.foreground=fg;  gcv.background=bg;
  GC theGC=XCreateGC(theDisplay,dw,(GCForeground|GCBackground),&gcv);
  DEBUG_OUT<<"Going to XPutImage...\n";
  XPutImage(theDisplay,ThePM,theGC,TheImage,0,0,0,0,width,height);
  XFlushGC(theDisplay,theGC);
  XSync(theDisplay,false);
  DEBUG_OUT<<"Done with x private bits.\n";
#endif // ASYS_X11
#ifdef ASYS_DOS
  // DOS can only handle 4 or less planes
  if(nPlanes>4) convert(0,A_BITMAP_CHUNKY,(APalette *)NULL);
  if(needsRGBReorder) reorderRGBChunky();
  //if(depth>8) adjustRGBChunky();
#endif // ASYS_DOS
#ifdef ASYS_OS2
  // FIXME: This doesn't handle all the depths and modes it should yet.
  // Could do some more work here.
  point[2].x=0;  point[2].y=0;  point[3].x=0;  point[3].y=0;
  DEBUG_OUT<<"createPrivateBits: original depth="<<depth<<" nPlanes="<<nPlanes<<"\n";
  if(nPlanes>1) {
    unsigned int goal=4;
    if(nPlanes>4) goal=8;
    if(nPlanes>8) goal=24;
    convert(goal,A_BITMAP_CHUNKY,(APalette *)NULL);
  }
  if(depth==1) {
    DEBUG_OUT<<"Changing 2 color bitmap to 16 colors...\n";
    APalette tempPal(colors);
    convert(4,A_BITMAP_CHUNKY,(APalette *)NULL);
    colors->setColor(0,tempPal.red(0),tempPal.green(0),tempPal.blue(0));
    for(unsigned int t=1;t<16;t++)
      colors->setColor(t,tempPal.red(1),tempPal.green(1),tempPal.blue(1));
  }
  if(needsRGBReorder) reorderRGBChunky();
  if(twentyFourBits) adjustRGBChunky();
  DEBUG_OUT<<"spb: now depth="<<depth<<" nPlanes="<<nPlanes<<"\n";
  HPS hps=WinGetPS(HWND_DESKTOP);
  int numColors=0;
  if(colors) numColors=colors->getNumColors();
  if(depth>8) numColors=0;
  unsigned int twidth=width,theight=height,oldBPL=bytesPerLine;
  // FIXME: I've never tested this part...
  if(twidth&0x3) {
    DEBUG_OUT<<"(Padding bitmap to even number of longwords...)\n";
    twidth=(unsigned int)((twidth&0xfffffffcL)+1);
    bytesPerLine=(twidth/minBlit)*maxBlit;
    planeSize=bytesPerLine*height;
  }
#ifdef ASYS_OS2_32
  if(!bmi) {
    bmi=(BITMAPINFO2 *)malloc(sizeof(BITMAPINFOHEADER2)+
      (sizeof(RGB2)*numColors));
  }
  bmi->cbFix=sizeof(BITMAPINFOHEADER2);
#else
  if(!bmi) {
    bmi=(BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER)+
      (sizeof(RGB)*numColors));
  }
  bmi->cbFix=sizeof(BITMAPINFOHEADER);
#endif // ASYS_OS2_32
  bmi->cx=(USHORT)twidth;  bmi->cy=(USHORT)theight;
  bmi->cPlanes=(char)nPlanes;  bmi->cBitCount=(char)depth;
#ifdef ASYS_OS2_32
  bmi->ulCompression=BCA_UNCOMP; // NONE
  bmi->cbImage=0;  // Probably not right...
  bmi->cxResolution=150;  bmi->cyResolution=150;  //Huh?
  bmi->usUnits=BRU_METRIC;  // Huh? Again...
  bmi->usReserved=0;
  bmi->usRecording=BRA_BOTTOMUP; // Where's BRA_TOPDOWN when you need it?
  bmi->usRendering=BRH_NOTHALFTONED;
  bmi->cSize1=0;  bmi->cSize2=0;
  bmi->ulColorEncoding=BCE_RGB;
  bmi->cclrUsed=0;  // All colors in palette used (for now)
  bmi->cclrImportant=0;  // All of them (for now)
#endif // ASYS_OS2_32
  if(colors) {
    for(unsigned int t=0;t<(unsigned int)numColors;t++) {
      bmi->argbColor[t].bRed=(char)colors->red(t);
      bmi->argbColor[t].bGreen=(char)colors->green(t);
      bmi->argbColor[t].bBlue=(char)colors->blue(t);
#ifdef ASYS_OS2_32
      bmi->argbColor[t].fcOptions=PC_RESERVED;
#endif // ASYS_OS2_32
    }
  }
#ifdef ASYS_OS2_32
  winBits=GpiCreateBitmap(hps,(BITMAPINFOHEADER2 *)bmi,CBM_INIT,
    (PBYTE)plane[0],(BITMAPINFO2 *)bmi);
#else
  winBits=GpiCreateBitmap(hps,(BITMAPINFOHEADER *)bmi,CBM_INIT,
    (PBYTE)plane[0],(BITMAPINFO *)bmi);
#endif // ASYS_OS2_32
  ERRORID err=WinGetLastError(aThisApp.ahAB);
  // FIXME: I should interpret common errors here.
  if(err) {
    DEBUG_OUT<<"Bitmap error "<<aThisApp.myHex(err)<<" in PMERR.H\n";
    //DEBUG_OUT.warning("Problem with bitmap!\n");
  }
  WinReleasePS(hps);
  if(!winBits) //DEBUG_OUT.warning("Couldn't create private HBITMAP!\n");
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(nPlanes>4) convert(0,A_BITMAP_CHUNKY,(APalette *)NULL);
  if(needsRGBReorder) reorderRGBChunky();
#else
  // FIXME: This gets confused by "odd" bitmap depths
  // Need to make sure we tell it everything is okay somewhere...
  //DEBUG_OUT<<"Creating private HBITMAP...\n";
  if(nPlanes>24) convert(32,A_BITMAP_CHUNKY,(APalette *)NULL);
  else {
    if(nPlanes>16) convert(24,A_BITMAP_CHUNKY,(APalette *)NULL);
    else {
      if(nPlanes>8) convert(16,A_BITMAP_CHUNKY,(APalette *)NULL);
      else {
        if(nPlanes>4) convert(8,A_BITMAP_CHUNKY,(APalette *)NULL);
        else {
          if(nPlanes>2) convert(4,A_BITMAP_CHUNKY,(APalette *)NULL);
                else {
            if(nPlanes>1) convert(2,A_BITMAP_CHUNKY,(APalette *)NULL);
#ifndef ASYS_WIN32CE
            else {
              // CE doesn't support 16 color bitmaps...
              if(depth==2) convert(4,A_BITMAP_CHUNKY,(APalette *)NULL);
            }
#endif // ASYS_WIN32CE
          }
        }
      }
    }
  }
  DEBUG_OUT<<"createPrivateBits: d="<<depth<<" np="<<nPlanes<<"\n";
  HDC hdc=GetDC((HWND)NULLHANDLE);
  unsigned int numColors=0;
  if(colors) numColors=colors->getNumColors();
  if(depth>8) numColors=0;
  switch(depth) {
    case 1: case 4: case 8: break;
    case 15: case 16:
      //convert(24,A_BITMAP_CHUNKY,(APalette *)NULL);
      break;
    case 24: break;
    case 32:
      //convert(24,A_BITMAP_CHUNKY,(APalette *)NULL);
      break;
    default:
      DEBUG_OUT<<"Windows doesn't support bit depth!\n"; return; // break;
  }
  // Windows wants the rgb triplets in a weird order...
  if(needsRGBReorder) reorderRGBChunky();
  if(twentyFourBits) adjustRGBChunky();
  unsigned int twidth=width,theight=height;
  if(!bmi) {
    bmi=(BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER)+
      (sizeof(RGBQUAD)*numColors));
  }
  bmi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
  bmi->bmiHeader.biWidth=twidth;  bmi->bmiHeader.biHeight=theight;
  bmi->bmiHeader.biPlanes=(WORD)nPlanes;  bmi->bmiHeader.biBitCount=(WORD)depth;
  bmi->bmiHeader.biCompression=BI_RGB;
  bmi->bmiHeader.biSizeImage=0;  // Probably not right...
  bmi->bmiHeader.biXPelsPerMeter=0;  bmi->bmiHeader.biYPelsPerMeter=0;
  bmi->bmiHeader.biClrUsed=0;  // All colors in palette used (for now)
  bmi->bmiHeader.biClrImportant=0;  // All of them (for now)
  if(colors) {
    for(unsigned int t=0;t<numColors;t++) {
      bmi->bmiColors[t].rgbRed=(BYTE)colors->red(t);
      bmi->bmiColors[t].rgbGreen=(BYTE)colors->green(t);
      bmi->bmiColors[t].rgbBlue=(BYTE)colors->blue(t);
      bmi->bmiColors[t].rgbReserved=0;
    }
  }
#ifdef ASYS_WIN32CE
  if(!colors) {
    // CE 2.0+ needs bitmasks for 16 or 32 bit...
    unsigned long *cheat=(unsigned long *)(bmi->bmiColors);
    if(depth==16) {
      bmi->bmiHeader.biCompression=BI_BITFIELDS;
      cheat[0]=0x3e00;  cheat[1]=0x3e0;  cheat[2]=0x001f;
    }
    if(depth==32) {
      bmi->bmiHeader.biCompression=BI_BITFIELDS;
      cheat[0]=0x00ff0000;  cheat[1]=0x0000ff00;  cheat[2]=0x000000ff;
    }
  }
  winBits=CreateDIBSection(hdc,bmi,DIB_RGB_COLORS,(void **)&extra,NULL,0);
  if(!winBits) DEBUG_OUT<<"winBits is NULL!\n";
  if(extra) {
    for(unsigned int t=0;t<planeSize;t++) extra[t]=plane[0][t];
  }
  else DEBUG_OUT<<"extra is NULL!\n";
#else
  winBits=CreateDIBitmap(hdc,&(bmi->bmiHeader),CBM_INIT,(LPSTR)plane[0],bmi,DIB_RGB_COLORS);
  if(!winBits) DEBUG_OUT<<"winBits is NULL!\n";
#endif // ASYS_WIN32CE
  ReleaseDC((HWND)NULLHANDLE,hdc);
  if(!winBits) DEBUG_OUT<<"Couldn't create private HBITMAP!\n";
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_AMIGA
  if(!TheBM) TheBM=(BitMap *)malloc(sizeof(BitMap));
  if(!TheBM) {
    //DEBUG_OUT.warning("No memory for BitMap!!!\n");  errorFlag=true; return;
  }
  if(bitmapType==A_BITMAP_CHUNKY) convert(depth,A_BITMAP_PLANES,NULL);
  unsigned int t;
  if(nPlanes>8) convert(8,A_BITMAP_PLANES,NULL);
  for(t=0;t<8;t++) TheBM->Planes[t]=NULL;
  // FIXME: use 3.x's AllocBitmap (or whatever)
  for(t=0;t<nPlanes;t++)  {
    TheBM->Planes[t]=(PLANEPTR)AllocMem(planeSize,MEMF_CHIP);
    if(!TheBM->Planes[t]) {
      //DEBUG_OUT.warning("No CHIP mem for plane ")<<t<<"!\n";
      return;
    }
    memcpy(TheBM->Planes[t],plane[t],planeSize);
  }
  TheBM->BytesPerRow=bytesPerLine; TheBM->Rows=height;
  TheBM->Flags=0; TheBM->pad=0;
  TheBM->Depth=nPlanes;
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
  // Macs can't display true planar bitmaps, so convert it to chunky
  if(nPlanes>23) convert(24,A_BITMAP_CHUNKY,(APalette *)NULL); else
  if(nPlanes>14) convert(16,A_BITMAP_CHUNKY,(APalette *)NULL); else
  if(nPlanes>1) convert(8,A_BITMAP_CHUNKY,(APalette *)NULL);
  if(needsRGBReorder) reorderRGBChunky();
  switch(depth) {
    case 1:  case 2:  case 4:  case 8: break;
    case 15: convert(16,A_BITMAP_CHUNKY,(APalette *)NULL); break;
    case 24: convert(32,A_BITMAP_CHUNKY,(APalette *)NULL); break;
    case 16:  case 32: break;
    default:
      //DEBUG_OUT.warning("createPrivateBits: Unimplemented bit depth!\n");
      return; // break;
  }
  DEBUG_OUT<<"Creating mac bits...\n";
  OSErr err=noErr;
#ifdef USE_QUARTZ
#else
  if(!theWorld) {
    DEBUG_OUT<<"Going to create pb "<<width<<"x"<<height<<"x"<<depth<<"("<<nPlanes<<") gWorld...\n";
    Rect bitmapBox;
    bitmapBox.top=0;  bitmapBox.left=0;
    bitmapBox.right=width; bitmapBox.bottom=height;
    if(colors) {
      DEBUG_OUT<<"(Bitmap is indexed color, so creating ctab)\n";
      unsigned int csize=(1<<depth);
      if(ctab) DisposeCTable(ctab);
      ctab=GetCTable(depth);
      if(ctab) {
        ColorSpec *specs=(**ctab).ctTable;
        for(unsigned int t=0;t<csize;t++) {
          specs[t].rgb.red=colors->red(t)<<8;
          specs[t].rgb.green=colors->green(t)<<8;
          specs[t].rgb.blue=colors->blue(t)<<8;
          specs[t].value=t+1;
        }
        CTabChanged(ctab);
        //(**ctab).ctSeed=GetCTSeed();
        //if(ThePM->pmTable) DisposeCTable(ThePM->pmTable);
        //ThePM->pmTable=ctab;
      }
      else DEBUG_OUT<<"(Couldn't get ctab)\n";
    }
    err=NewGWorld(&theWorld,depth,&bitmapBox,ctab,(GDevice **)NULL,(unsigned long)NULL);
    if(err!=noErr) {
      if(err==-108) {
        //DEBUG_OUT.warning("No memory for GWorld!\n");
      }
      else {
        DEBUG_OUT<<"QuickDraw error "<<err<<"!\n";
      }
    }
  }
#endif // USE_QUARTZ
  macBlastBits();
#endif // ASYS_MAC
#ifdef ASYS_PALM
  // Palms can't display true planar bitmaps, so convert it to chunky
  if(aThisApp.hasColor) {
    if(nPlanes>23) convert(24,A_BITMAP_CHUNKY,(APalette *)NULL); else
    if(nPlanes>14) convert(16,A_BITMAP_CHUNKY,(APalette *)NULL);
  }
  else if(nPlanes>1) convert(1,A_BITMAP_CHUNKY,(APalette *)NULL);
  if(needsRGBReorder) reorderRGBChunky();
  switch(depth) {
    case 1:  case 2:  case 4:  case 8: break;
    case 15: convert(16,A_BITMAP_CHUNKY,(APalette *)NULL); break;
    case 16:  case 32: break;
    default:
      //DEBUG_OUT.warning("createPrivateBits: Unimplemented bit depth!\n"); return; // break;
  }
  Err err;
#ifdef __PALMOS_H__
  if(aThisApp.hasThreePointFive) {
    bits=BmpCreate(width,height,depth,NULL,&err);
    // TODO: Check error
    win=WinCreateBitmapWindow(bits,&err);
    // TODO: Check error
    unsigned int bs=BmpSize(bits);
    DEBUG_OUT<<"Bitmap is "<<bs<<" bytes...\n";
    memcpy(BmpGetBits(bits),plane[0],bs);
  }
#endif // __PALMOS_H__
#endif // ASYS_PALM
  calc();
  DEBUG_OUT<<"Private bits created for bitmap.\n";
  privateBitsDone=true;
  privateLock=false;
  modified=false;
  DEBUG_OUT<<"---createPrivateBits()\n";
}


void APrivateBitmap::setVGAWrite(unsigned int p)
{
#ifdef ASYS_DOS
  outportb(0x3c4,2);  outportb(0x3c5,1<<p);
#endif // ASYS_DOS
}


void APrivateBitmap::setVGARead(unsigned int p)
{
#ifdef ASYS_DOS
  outportb(0x3ce,4);  outportb(0x3cf,p&0x3);
#endif // ASYS_DOS
}


void APrivateBitmap::setVGAAll()
{
#ifdef ASYS_DOS
  outportb(0x3c4,2);  outportb(0x3c5,0x0f);  // Write to all planes
  outportb(0x3ce,5);  outportb(0x3cf,0);  // Write mode 0
  outportb(0x3ce,1);  outportb(0x3cf,0);  // Disable fill read mode
  outportb(0x3ce,3);  outportb(0x3cf,0);  // No func, no rotate
#endif // ASYS_DOS
}


void APrivateBitmap::setVGABank(unsigned int b)
{
#ifdef ASYS_DOS
  // Assume caller knows exactly what b is representing (size, gran)
#ifdef ASYS_DOS32
  REGS16 rin,rout;
  RIN_W_AX=0x4f05;
  rin.h.bh=0x00;  rin.h.bl=0;  // mem window #0
  RIN_W_DX=(unsigned short)b;
  int86(0x10,&rin,&rout);
#endif // ASYS_DOS32
#ifdef ASYS_DOS16
  /*
  void *theFunc=pageFunc;
  asm {
    xor bx,bx;
    mov dx,b;
    call [theFunc];
  }
  */
  REGS16 rin,rout;
  rin.x.ax=0x4f05;
  rin.h.bh=0x00;  rin.h.bl=0;  // mem window #0
  rin.x.dx=b;
  int86(0x10,&rin,&rout);
#endif // ASYS_DOS16
#endif // ASYS_DOS
}


// FIXME: This function needs to be cleaned up a lot
void APrivateBitmap::draw(APrivateBitmap *source, unsigned int dx, unsigned int dy)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  unsigned int sourceBytesPerLine=source->getBytesPerLine();
  unsigned int drawWidth=source->getWidth();
  unsigned int drawHeight=source->getHeight();
  unsigned int drawDepth=source->getNPlanes();
  unsigned int ssx=source->getX();
  unsigned int ssy=source->getY();
  unsigned int row,p;
  unsigned char *src,*dest;
  // FIXME: Minimal clipping...take dx,dy into account!
  if(drawHeight>height) drawHeight=height;
  if(drawDepth>nPlanes) drawDepth=nPlanes;
  drawWidth=(drawWidth/source->getMinBlit())*source->getMaxBlit();
  if(drawWidth>bytesPerLine) drawWidth=bytesPerLine;
  dx=(dx/minBlit)*maxBlit;
  ssx=(ssx/source->getMinBlit())*source->getMaxBlit();
  if(interleaved) {
    unsigned int ssy=ssy/2;  dy=dy/2;
    drawHeight=source->getHeight()/2;
    sourceBytesPerLine*=2;
    // Even lines
    dest=plane[0]+(dy*bytesPerLine)+dx;
    src=(source->getPlane(0))+(ssy*sourceBytesPerLine)+ssx;
    for(row=0;row<drawHeight;row++) {
      memcpy(dest,src,drawWidth);
      dest+=bytesPerLine; src+=sourceBytesPerLine;
    }
    // Odd lines
    // Borland C++ 3.1 has a problem with the 8192 constant...
    dest=plane[0]+(dy*bytesPerLine)+dx+8192;
    src=(source->getPlane(0))+(ssy*sourceBytesPerLine)+ssx+(sourceBytesPerLine/2);
    for(row=0;row<drawHeight;row++) {
      memcpy(dest,src,drawWidth);
      dest+=bytesPerLine; src+=sourceBytesPerLine;
    }
    return;
  }
  switch(bitmapType) {
    case A_BITMAP_PLANES:
      for(p=0;p<drawDepth;p++) {
        dest=plane[p]+(dy*bytesPerLine)+dx;
        src=(source->getPlane(p))+(ssy*sourceBytesPerLine)+ssx;
#ifdef ASYS_DOS
        if(needPlaneSwitch) setVGAWrite(p);
#endif // ASYS_DOS
        for(row=0;row<drawHeight;row++) {
          memcpy(dest,src,drawWidth);
          dest+=bytesPerLine; src+=sourceBytesPerLine;
        }
      }
#ifdef ASYS_DOS
      if(needPlaneSwitch) setVGAAll();
#endif // ASYS_DOS
      break;
    case A_BITMAP_CHUNKY:
      dest=plane[0]+(dy*bytesPerLine)+dx;
      src=(source->getPlane(0))+(ssy*sourceBytesPerLine)+ssx;
      for(row=0;row<drawHeight;row++) {
        memcpy(dest,src,drawWidth);
        dest+=bytesPerLine; src+=sourceBytesPerLine;
      }
      break;
    default:
      //DEBUG_OUT.warning("bitmap::draw unknown bitmap format!\n");
      break;
  }
  touch();
}


#ifdef ASYS_MAC
void APrivateBitmap::macBlastBits()
{
#ifdef USE_QUARTZ
#else
  if(theWorld) {
    CGrafPtr origPort;
    GDHandle origDev;
    PixMapHandle myWorldPM;
    Rect sr;
    sr.top=0;  sr.left=0;  sr.bottom=height;  sr.right=width;
    bool good=false;
    GetGWorld(&origPort,&origDev);
    SetGWorld(theWorld,(GDevice **)NULL);
    myWorldPM=GetGWorldPixMap(theWorld);
    good=LockPixels(myWorldPM);
    if(good) {
      ForeColor(blackColor);  BackColor(whiteColor);
#if TARGET_API_MAC_CARBON
      Rect theRect;
      GetPortBounds(theWorld,&theRect);
      EraseRect(&theRect);
#else
      EraseRect(&(theWorld->portRect));
#endif // TARGET_API_MAC_CARBON
#if __MC68K__
      SInt8 oldMode=GetMMUMode();
      SInt8 newMode=true32b;
      if(PixMap32Bit(myWorldPM)) { if(oldMode==false32b) SwapMMUMode(&newMode); }
#endif // __MC68K__
      char *ptr=NULL;
      ptr=GetPixBaseAddr(myWorldPM);
      if(ptr) {
        unsigned int rowLength=((*myWorldPM)->rowBytes)&0x3fff;
        unsigned int writeWidth=bytesPerLine;
        unsigned int writeHeight=((*myWorldPM)->bounds.bottom)-((*myWorldPM)->bounds.top);
        DEBUG_OUT<<"mbb bitmap is "<<width<<"x"<<height<<"x"<<depth<<"("<<nPlanes<<")\n";
        DEBUG_OUT<<"mbb write: width="<<writeWidth<<" height="<<writeHeight<<"\n";
        unsigned char *dest=(unsigned char *)NULL;
        unsigned char *src=(unsigned char *)NULL;
        for(unsigned int yy=0;yy<writeHeight;yy++) {
          dest=(unsigned char *)ptr+(rowLength*yy);
          src=plane[0]+(bytesPerLine*yy);
          for(unsigned int xx=0;xx<writeWidth;xx++) {
            dest[xx]=src[xx];
          }
        }
      }
      else //DEBUG_OUT.warning("mbb: Couldn't GetPixBaseAddr!\n")
      ;
#if __MC68K__
      if(oldMode==false32b) { newMode=false32b; SwapMMUMode(&newMode); }
#endif // __MC68K__
      UnlockPixels(myWorldPM);
    }
    else DEBUG_OUT<<"mbb: Couldn't lock pixels!\n";
    SetGWorld(origPort,origDev);
  }
  else DEBUG_OUT<<"mbb: No GWorld!\n";
#endif // USE_QUARTZ
}
#endif // ASYS_MAC


// The idea is that this function will modify the bitmap if needed so
// it can be shown on that display
bool APrivateBitmap::verifyOkay(ADisplay *dis)
{
  DEBUG_OUT<<"APrivateBitmap::verifyOkay(dis)\n";
  if(!dis) { DEBUG_OUT<<"no dis!\n"; return false; }
  if(bitmapType==A_BITMAP_NONE) { DEBUG_OUT<<"Type==NONE!!!\n"; okayDokey=false;  return okayDokey; }
  if(verifyLock) { DEBUG_OUT<<"(verifyLock)\n"; return true; }
  verifyLock=true;
  //bitsHint=dis->getBitsTriplet();
  APalette *targetColors=(APalette *)NULL;
  bool please=false;
  unsigned int targetDepth=depth;
  ABitmapType targetType=bitmapType;
  if(dis->isStaticColor()) {
    targetColors=dis->getColors();
    if(colors) { if(!colors->closeEnoughTo(targetColors)) please=true; }
  }
#ifdef ASYS_AMIGA
  targetType=A_BITMAP_PLANES;
  if(targetColors->isHBrite()&&colors) colors->setHBrite();
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  targetType=A_BITMAP_PLANES;
#endif // ASYS_X11
#ifdef ASYS_DOS
  targetType=dis->getBitmapType();
#endif // ASYS_DOS
#ifdef ASYS_WINDOWS
#ifdef ASYS_WIN32CE
  targetType=A_BITMAP_CHUNKY;
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
  targetType=A_BITMAP_CHUNKY;
#endif // ASYS_MAC
  if(bitmapType!=targetType) please=true;
  if(depth!=dis->getDepth()) { targetDepth=dis->getDepth(); please=true; }
  if(targetType==A_BITMAP_PLANES) {
    if(nPlanes!=dis->getNPlanes()) { targetDepth=dis->getNPlanes(); please=true; }
  }
  if(dis->canUseAnyImage()) please=false;
  DEBUG_OUT<<"(please is "<<please<<")\n";
  if(please) {
    convert(targetDepth,targetType,targetColors);
    freePrivateBits();  createPrivateBits(true);
  }
  okayDokey=true;
  if(!privateBitsDone) createPrivateBits(true); else update();
  verifyLock=false;
  modified=false;
  DEBUG_OUT<<"---verify(dis)\n";
  return okayDokey;
}


// The idea is that this function will modify the bitmap if needed so
// it can be shown on that window
bool APrivateBitmap::verifyOkay(AWindowAbstract *win)
{
  if(!win) { DEBUG_OUT<<"no win!\n"; return false; }
  ADisplay *dis=win->getParent();
  if(dis) return verifyOkay(dis); else { DEBUG_OUT<<"no dis!\n"; return false; }
}


// Rebuild the private bits if they were stale
void APrivateBitmap::update()
{
  if(modified||(!okayDokey)) {
    //updatePrivateBits();
    freePrivateBits(); createPrivateBits(true);
  }
}


// A way to say that you did something to the bits and privateBits are probably stale now
void APrivateBitmap::touch()
{
  modified=true;  okayDokey=false;
  //freePrivateBits(); setPrivateBits(true);
}

