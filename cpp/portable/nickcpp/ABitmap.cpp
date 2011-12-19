
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

//#include <guilib/ADisplay.h>
//#include <guilib/AWindow.h>

#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/AConvertingBitLine.h>

#include "ABitmap.h"


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
//  ABitmap Class
////////////////////////////////////////////////////////////////////////////////

/*
// TODO: Probably needs a lot more work...
ABitmap::ABitmap(unsigned char *theBits,AWindowAbstract *someWindow)
{
  DEBUG_OUT<<"ABitmap::ABitmap(theBits,someWindow)\n";
  init();
  width=someWindow->getWidth();  height=someWindow->getHeight();
  depth=someWindow->getParent()->getDepth();  nPlanes=someWindow->getParent()->getNPlanes();
  calc();
  plane[0]=theBits;
  needFree[0]=false;
  if(!trueColor) colors=new APalette((unsigned int)bits2Bytes(depth));
  //createPrivateBits(true);
}
*/


/*
ABitmap::ABitmap(ADisplay *someDisplay, ADisplayMode *d)
{
  DEBUG_OUT<<"ABitmap::ABitmap(someDisplay,d)\n";
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
  //createPrivateBits(true);
}
*/


ABitmap::ABitmap()
{
  DEBUG_OUT<<"ABitmap::ABitmap()\n";
  init();
  // NOTE: Bounds are invalid here...parent class will take care of it...
}


ABitmap::ABitmap(unsigned int w, unsigned int h, unsigned int d,unsigned int np)
{
  DEBUG_OUT<<"ABitmap::ABitmap("<<w<<","<<h<<","<<d<<","<<np<<")\n";
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
      DEBUG_OUT<<"cons: Couldn't get memory for bitmap plane "<<t<<" ("<<num<<",1024)!!\n";
      exit(5);
      return;
    }
    needFree[t]=true;
  }
  if(d<9) {
    colors=new APalette(2<<(d-1));
  }
}


ABitmap::ABitmap(unsigned char *data, unsigned int w, unsigned int h,unsigned int d, unsigned int np)
{
  DEBUG_OUT<<"ABitmap::ABitmap(data,"<<w<<","<<h<<","<<d<<","<<np<<")\n";
  init();
  width=w;  height=h;  depth=d; nPlanes=np;
  calc();
  bytesPerLine=calcBPL(4);  // Force to multiple of 32-bits
  planeSize=(unsigned long)bytesPerLine*(unsigned long)height;
  for(unsigned int t=0;t<np;t++) {
    plane[t]=data;
    needFree[t]=false;
  }
}


ABitmap::~ABitmap()
{
  DEBUG_OUT<<"ABitmap::~ABitmap()\n";
  freeBitmap();
}


void ABitmap::freeBitmap()
{
  DEBUG_OUT<<"ABitmap::freeBitmap()\n";
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


// FIXME: Only works for 8 bits per pixel
void ABitmap::writePixel(unsigned int x, unsigned int y)
{
  if(x>width) x=width;
  if(y>height) y=height;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned char *dest=plane[0]+tx+(y*bytesPerLine);
  // FIXME: If maxBlit>1 this will only draw a portion of the pixel
  // FIXME: If minBlit>1 this will draw multiple pixels
  *dest=(unsigned char)foreground;
}


// This will return true if we think the planeSize is too big
// for us to handle (i.e. more than 64k on a 16bit os)
// FIXME: Actually checks more and should be called "checkSanity" or something...
bool ABitmap::tooBig()
{
  bool ret=false;
  //DEBUG_OUT<<"tooBig is checking a planeSize of "<<planeSize<<"\n";
  // General checking...actually should be called "tooSmall"...
  if(!width) ret=true;
  if(!height) ret=true;
  if(!depth) ret=true;
  if(!nPlanes) ret=true;
  if(!maxBlit) ret=true;
  if(!minBlit) ret=true;
  if(ret) {
    DEBUG_OUT<<"tooBig failed because of zero value somewhere!!!\n";
  }
  else {
    // Now we check on the size of the planes
    unsigned long memSize=width*height*maxBlit/minBlit;  // esitmate memory size
    // FIXME: check if we have enough free memory for the sucker...
    if(sizeof(int)==2) {
      //if(planeSize>65535L) ret=true;
      if(memSize>65535L) ret=true;
    }
    if(ret) {
      DEBUG_OUT<<"tooBig failed, memSize of "<<memSize<<" is too big!!!\n";
      DEBUG_OUT<<"tooBig failed, planeSize of "<<planeSize<<" is too big!!!\n";
    }
  }
  if(!boundsValid()) ret=true;  // check dimensions...
  return ret;
}


void ABitmap::memcpyl(unsigned char *dest, unsigned char *src, unsigned long len)
{
  if(!dest) return;
  if(!src) return;
  for(unsigned long t=0;t<len;t++) { *dest=*src; src++; dest++; }
}


void ABitmap::calcColors()
{
  if((!trueColor)&&(!colors)) colors=new APalette(1<<depth);
  if((trueColor)&&(colors)) { delete colors;  colors=(APalette *)NULL; }
  if(colors) colors->resize(1<<depth);
}



bool ABitmap::setColor(unsigned int index,unsigned long triplet)
{
  return setColor(index,getRGBRed24(triplet),getRGBGreen24(triplet),getRGBBlue24(triplet));
}


bool ABitmap::setColor(unsigned int index,AColor *aColor)
{
  if(!aColor) return false;
  return setColor(index,aColor->red,aColor->green,aColor->blue);
}


bool ABitmap::setColor(unsigned int index,unsigned int red,unsigned int green,
  unsigned int blue)
{
  if(colors) colors->setColor(index,red,green,blue);
  else return false;
  return true;
}


bool ABitmap::checkLine(unsigned int row)
{
  if(row>height)  {
    DEBUG_OUT<<"checkLine "<<row<<" failed!\n";
    return false;
  }
  return true;
}


bool ABitmap::checkPlane(unsigned int p)
{
  if(nPlanes<p)  {
    DEBUG_OUT<<"checkPlane "<<p<<" failed!\n";
    return false;
  }
  return true;
}


void ABitmap::duplicate(ABitmap *source)
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


void ABitmap::copyFields(ABitmap *source)
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


void ABitmap::writeLine(unsigned int lineNum, unsigned char *lineData,
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


void ABitmap::readLine(unsigned int lineNum, unsigned char *lineData,
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


AConvertingBitLine *ABitmap::getLine(unsigned int row)
{
  unsigned char *source;
  if(!checkLine(row)) return (AConvertingBitLine *)NULL;
  AConvertingBitLine *temp=new AConvertingBitLine(this);
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


AConvertingBitLine *ABitmap::getCopyOfLine(unsigned int row)
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


void ABitmap::copyLineTo(AConvertingBitLine *dest,unsigned int row)
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


void ABitmap::copyToLine(AConvertingBitLine *dest,unsigned int row)
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


void ABitmap::copyFromLine(AConvertingBitLine *source,unsigned int row)
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


unsigned long ABitmap::bits2Bytes(unsigned int bits)
{
  unsigned long bytes=1;
  for(unsigned int t=0;t<bits;t++) bytes*=2;
  return bytes;
}


void ABitmap::reorderRGB()
{
  switch(bitmapType) {
    case A_BITMAP_CHUNKY: reorderRGBChunky(); break;
    case A_BITMAP_PLANES: reorderRGBPlanes(); break;
    default:
      DEBUG_OUT<<"Bad bitmap type in reorderRGB()!\n";
      break;
  }
  needsRGBReorder=false;
}


void ABitmap::reorderRGBChunky()
{
  DEBUG_OUT<<"Going to reorder RGB chunkies...\n";
  if(!twentyFourBits) { DEBUG_OUT<<"Can't reorder non-24bits!\n"; return; }
  if(bitmapType!=A_BITMAP_CHUNKY) { DEBUG_OUT<<"Can't reorder non-chunky!\n"; return; }
  unsigned char r,g,b;
  for(unsigned int tt=0;tt<planeSize;tt+=3) {
    r=plane[0][tt]; g=plane[0][tt+1]; b=plane[0][tt+2];
    plane[0][tt]=g; plane[0][tt+1]=b; plane[0][tt+2]=r;
  }
  touch();
}


// This is used when the RGB bits in a truecolor bitmap are
// expected to be in a different order than we normally have them...
// This should work for any trueColor depth, not just 24 like the function above...
void ABitmap::reorderRGBPlanes()
{
  DEBUG_OUT<<"Going to reorder RGB planes...\n";
  if(bitmapType!=A_BITMAP_PLANES) { DEBUG_OUT<<"Can't reorder non-planes!\n"; return; }
  unsigned char *temp;
  unsigned int t;
  unsigned int offset1a=0,offset2a=0,numa=0;
  unsigned int offset1b=0,offset2b=0,numb=0;
  switch(nPlanes) {
    case 24:
      offset1a=0; offset2a=8; numa=8;
      offset1b=8; offset2b=16; numb=8;
      break;
    default:
      DEBUG_OUT<<"reorderRGBPlanes not implemented for "<<nPlanes<<" planes!\n";
     break;
  }
  if(numa) {
    for(t=0;t<numa;t++) {
      temp=plane[t+offset1a];
      plane[t+offset1a]=plane[t+offset2a];
      plane[t+offset2a]=temp;
    }
  }
  if(numb) {
    for(t=0;t<numa;t++) {
      temp=plane[t+offset1b];
      plane[t+offset1b]=plane[t+offset2b];
      plane[t+offset2b]=temp;
    }
  }
  touch();
}


// NOTE: Be very careful what you pass to this function...
// be safe and set it back to what it was...
void ABitmap::setPlane(unsigned int p, unsigned char *data)
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
  touch();
}


void ABitmap::copyPlane(ABitmap *source,unsigned int p)
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


void *ABitmap::acalloc(unsigned int num,unsigned int size)
{
  void *foo=NULL;
  if(size&&num) {
    //DEBUG_OUT<<"Going to ::calloc(num="<<num<<",size="<<size<<")\n";
   foo=(void *)::calloc(num,size);
    //DEBUG_OUT<<"Got back from the ::calloc\n";
    if(!foo) DEBUG_OUT<<"(calloc failed)\n";
  }
  else DEBUG_OUT<<"acalloc passed 0 parameter!\n";
  return foo;
}


void ABitmap::oval(int tx,int ty,int twidth, int theight)
{
  // Quick, dirty, and wrong...
  fillRect(tx,ty,twidth,theight);
}


void ABitmap::fillOval(int tx,int ty,int twidth, int theight)
{
  // Quick, dirty, and wrong...
  fillRect(tx,ty,twidth,theight);
}


void ABitmap::fillRect(int tx,int ty,int twidth, int theight)
{
  for(int t=ty;t<(ty+theight);t++) line(tx,t,tx+twidth,t);
}


// This works for any type of frameBuffer thanks to writePixel
void ABitmap::line(unsigned int xu1, unsigned int yu1, unsigned int xu2, unsigned int yu2)
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
    writePixel(x,y);
    while(err>=0) {
      if(swap) x+=s1; else y+=s2;
      err-=2*deltax;
    }
    if(swap) y+=s2; else x+=s1;
    err+=2*deltay;
  }
}


// FIXME: Only works for byte-per-pixel modes,
// else it will return more than one pixel
unsigned int ABitmap::readPixel(unsigned int x, unsigned int y)
{
  if(eightBits) {
    unsigned char *src=plane[0]+(y*bytesPerLine);
    unsigned int tx=(x/minBlit)*maxBlit;
    return *(src+tx);
  }
  else return (unsigned int)background;
}


void ABitmap::setForeground(unsigned long n,bool rgb)
{
  if(colors) n=n%colors->getNumColors();
  else DEBUG_OUT<<"ABitmap::setForeground but no colors!\n";
  foreground=n;
}


void ABitmap::setBackground(unsigned long n,bool rgb)
{
  if(colors) n=n%colors->getNumColors();
  else DEBUG_OUT<<"ABitmap::setBackground but no colors!\n";
  background=n;
}


// FIXME: This function basically works, but due to bugs will read/write
// FIXME: past the edge of bitmap...working on it.
// FIXME: Won't scroll negative dx for non 8bit dos...
// FIXME: Doesn't work in dos interleaved modes
void ABitmap::scroll(unsigned int tx,unsigned int ty,unsigned int tw,unsigned int th, int dx, int dy)
{
  unsigned int bpl=bytesPerLine;
  /*
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Scroll bitmap: "<<tx<<" "<<ty<<" "<<tw<<" "<<th<<" "
    <<bpl<<"("<<dx<<","<<dy<<")\n";
#endif // DEBUG_VERBOSE
  */
  tw=(tw/minBlit)*maxBlit;  tx=(tx/minBlit)*maxBlit;
  dx=(dx/minBlit)*maxBlit;
  unsigned int absdy=dy;
  if(dy<0) absdy=-dy;
  unsigned int absdx=dx;
  if(dx<0) absdx=-dx;
  if(th>height) th=height;
  if(tw>bpl) tw=bpl;
  tw-=absdx;
  th-=absdy;
  int incy,incx;
  unsigned int q,iq,t,i;
  unsigned char *src,*dest;
  for(unsigned int p=0;p<nPlanes;p++) {
    incy=bpl,incx=1,iq=0;
    dest=plane[p]+(ty*bpl)+tx;
    src=dest;
    if(dy>0) { incy=-(int)bpl; dest+=(th*bpl)+(absdy*bpl); src+=(th*bpl); }
    if(dy<0) { src+=(absdy*bpl); }
    if(dx>0) { dest+=tw+absdx; src+=tw; incx=-1; iq=tw; }
    if(dx<0) { src+=absdx; }
    for(t=0;t<th;t++) {
      q=iq;
      // Could use memcpy, or memmove here...
      for(i=0;i<tw;i++) { *(dest+q)=*(src+q); q+=incx; }
      src+=incy;  dest+=incy;
    }
  }
}


void ABitmap::clear(bool preserveMap)
{
  clear(0,0,width,height);
}


// FIXME: Doesn't work with more than 8 bits per pixel...
void ABitmap::clear(unsigned int dx, unsigned int dy,unsigned int tw,unsigned int sheight)
{
  tw=(tw/minBlit)*maxBlit;  dx=(dx/minBlit)*maxBlit;
  //DEBUG_OUT<<"maxBlit="<<maxBlit<<" minBlit="<<minBlit<<"\n";
  //DEBUG_OUT<<"ABitmap::clear "<<dx<<","<<dy<<" : "<<tw<<","<<sheight<<"\n";
  for(unsigned int p=0;p<nPlanes;p++) {
    unsigned char *dest=plane[p]+(dy*bytesPerLine)+dx;
    for(unsigned int row=0;row<sheight;row++) {
      memset(dest,(char)background,tw); dest+=bytesPerLine;
    }
  }
}


void ABitmap::scale(ABitmap *dest,int xFactor, int yFactor)
{
  DEBUG_OUT<<"ABitmap::scale not implemented!\n";
  exit(5);
}


// NOTE: For now, we'll only do enlargements
// NOTE: Assumes bitmap is probably 8 or 24 bit...
void ABitmap::scale(int xFactor, int yFactor)
{
  DEBUG_OUT<<"Scale: "<<width<<"x"<<height<<" "<<depth<<"x"<<nPlanes<<"\n";
  DEBUG_OUT<<"Bitmap: "<<bytesPerLine<<" "<<planeSize<<"\n";
  if((xFactor==1)&&(yFactor==1)) return;
  if(maxBlit==1) { if(depth<9) convertEightBits(); else convertTwentyFourBits(); }
  char bite;
  unsigned int p;
  for(p=0;p<nPlanes;p++) {
    for(unsigned int r=0;r<height;r++) {
      for(unsigned int c=0;c<bytesPerLine;c++) {
        bite=plane[p][r*bytesPerLine+c];
        //DEBUG_OUT<<bite<<"?";
      }
      //DEBUG_OUT<<"\n";
    }
  }
  unsigned int newWidth=width*xFactor,newHeight=height*yFactor;
  ABitmap *tBits=new ABitmap(newWidth,newHeight,depth,nPlanes);
  tBits->colors=colors;  tBits->ownColors=false;
  AConvertingBitLine *sLine=new AConvertingBitLine(width,depth,nPlanes);
  AConvertingBitLine *dLine=new AConvertingBitLine(newWidth,depth,nPlanes);
  DEBUG_OUT<<"Going to enter line loop...\n";
  for(unsigned int l=0;l<height;l++) {
    copyToLine(sLine,l);
    dLine->scaleLine(sLine,xFactor);
    for(int s=0;s<yFactor;s++) {
      tBits->copyFromLine(dLine,(l*yFactor)+s);
    }
  }
  DEBUG_OUT<<"Done looping, going to dup temp bitmap...\n";
  duplicate(tBits);
  // Steal the planes from the temp bitmap
  for(p=0;p<nPlanes;p++) {
    needFree[p]=true;
    tBits->needFree[p]=false;
  }
  ownColors=true;
  tBits->ownColors=false;
  //delete tBits;
  tBits=NULL;
  DEBUG_OUT<<"Scaled: "<<width<<"x"<<height<<" "<<depth<<"x"<<nPlanes<<"\n";
  DEBUG_OUT<<"Bitmap: "<<bytesPerLine<<" "<<planeSize<<"\n";
  for(p=0;p<nPlanes;p++) {
    for(unsigned int r=0;r<height;r++) {
      for(unsigned int c=0;c<bytesPerLine;c++) {
        bite=plane[p][r*bytesPerLine+c];
        //DEBUG_OUT<<bite<<"?";
      }
      //DEBUG_OUT<<"\n";
    }
  }
  modified=true;
}


void ABitmap::rotate(ABitmap *dest,int angle)
{
  DEBUG_OUT<<"ABitmap::scale not implemented!\n";
  exit(5);
}


// FIXME: This could be a heck more efficient!
void ABitmap::flip(bool dx, bool dy)
{
  DEBUG_OUT<<"flip("<<dx<<","<<dy<<")\n";
  ABitmap tBits(width,height,depth,nPlanes);
  if(dx) {
    // TODO: Probably need to convert to chunky 8 or 24 here...
    for(unsigned int p=0;p<nPlanes;p++) {
      tBits.copyPlane(this,p);
      for(unsigned int row=0;row<height;row++) {
        AConvertingBitLine *tLine=tBits.getLine(row);
        if(tLine) {
          tLine->flipLine();
          // we already fliped the bits, since the line shares the planes
          //writeLine(row,tLine->getPlane(0),tLine->getBytesPerLine(),p);
          //delete tLine;
        }
      }
    }
  }
  if(dy) {
    for(unsigned int p=0;p<nPlanes;p++) {
      tBits.copyPlane(this,p);
      for(unsigned int row=0;row<height;row++) {
        AConvertingBitLine *tLine=tBits.getLine(row);
        if(tLine) {
          writeLine(height-row-1,tLine->getPlane(0),tLine->getBytesPerLine(),p);
          //delete tLine;
        }
      }
    }
  }
  modified=true;
}


// This is mainly to catch the times when we create a palette but don't really need one
// FIXME: like a lot of other functions this makes the distiction between truecolor and
// indexed by looking at the depth...so we can't have X style 8 bit truecolor visuals!!!
void ABitmap::checkPaletteSize()
{
  if(depth<9) {
    // An indexed bitmap that should have a palette
    if(!colors) {
      DEBUG_OUT<<"validatePalette: indexed needs one, creating it\n";
      colors=new APalette((unsigned int)bits2Bytes(depth));
    }
    if(!colors) {
      DEBUG_OUT<<"ValidatePalette Failed! Couldn't create palette!";
    }
  }
  else {
    // A true color bitmap that shouldn't have a palette
    if(colors) {
      DEBUG_OUT<<"validatePalette: trueColor has one, destroying it.\n";
      delete colors;
    }
    colors=(APalette *)NULL;
  }
}


void ABitmap::convert(unsigned int suggestDepth,ABitmapType suggestType,
  APalette *suggestPalette)
{
  DEBUG_OUT<<"+++convert(...)\n";
  bool convertDone=false;
  bool sameType=false,needNewPalette=false,reducePalette=false;
  if((suggestDepth==1)&&(depth==1)) {  /* bitmapType=suggestType; */  return; }
  if(suggestType==bitmapType) sameType=true;
  // Check if we actually need to do anything...
  if((sameType)&&(depth==suggestDepth)) {
    if(!suggestPalette) {
      DEBUG_OUT<<"Useless convert (type and depth match).\n";  return;
    }
  }
  // Figure out our goal for number of bits per pixel and planes
  unsigned int p,goalBits=depth,goalPlanes=1;
  if((suggestDepth)&&(suggestDepth!=depth)) goalBits=suggestDepth;
  if(suggestType==A_BITMAP_CHUNKY) {
    if((goalBits>2)&&(goalBits<4)) goalBits=4;
    if((goalBits>4)&&(goalBits<8)) goalBits=8;
    if((goalBits>8)&&(goalBits<16)) goalBits=16;
    if((goalBits>16)&&(goalBits<24)) goalBits=24;
    if((goalBits>24)&&(goalBits<32)) goalBits=32;
  }
  // How many bits will our temp area be?
  unsigned int expandedBits=8;
  if(goalBits>8) expandedBits=24;
  if((expandedBits==8)&&(goalBits<depth)) {
    needNewPalette=true; reducePalette=true;
  }
  if(suggestType==A_BITMAP_PLANES) goalPlanes=goalBits;
  DEBUG_OUT<<"Converting "<<depth<<" bit ("<<(int)bitmapType<<") to "
    <<goalBits<<" bit ("<<(int)suggestType<<")...\n";
  if(suggestPalette) {
    needNewPalette=true;
    DEBUG_OUT<<"suggestPalette given...\n";
  }
  DEBUG_OUT<<"expandedBits are "<<expandedBits<<"...\n";
  DEBUG_OUT<<"goalBits are "<<goalBits<<"...\n";
  DEBUG_OUT<<"goalPlanes are "<<goalPlanes<<"...\n";
  if(sameType) DEBUG_OUT<<"sameType is set...\n";
  if(needNewPalette) DEBUG_OUT<<"needNewPalette is set...\n";
  if(reducePalette) DEBUG_OUT<<"reducePalette is set...\n";
  if(needNewPalette&&(!suggestPalette)) {
    DEBUG_OUT<<"We need a new palette...\n";
    if((!colors)&&(depth<9)) {
      // Create a quick fake palette to use...
      DEBUG_OUT<<"(Creating fake palette...)\n";
      calcColors();
      if(colors) { ownColors=false; }
    }
    computeHistogram(depth);
  }
  if(suggestType==A_BITMAP_PLANES) {
    // This special cases adding planes to match our goal...
    bool willFatten=false;
    if((sameType)&&(goalPlanes>nPlanes)) willFatten=true;
    if(goalPlanes>8) willFatten=false;
    if(willFatten) {
      DEBUG_OUT<<"Fattening planes...\n";
      // Fatten planes
      for(unsigned int t=nPlanes;t<goalPlanes;t++) {
        plane[t]=(unsigned char *)malloc((int)planeSize);
        if(plane[t]) {
          DEBUG_OUT<<"Added plane "<<t<<"...\n";
          needFree[t]=true;
          // FIXME:what if planeSize is greater than 64k for dos?
          memset(plane[t],0,(int)planeSize);
        }
        else //DEBUG_OUT.warning("Couldn't alloc plane ")<<t<<"!\n"
        ;
      }
      convertDone=true;
    }
  }
  if(!convertDone) {
    // Get temp work areas...
    ABitmap *tempBitmap=new ABitmap(width,height,goalBits,goalPlanes);
    if(!tempBitmap) { //DEBUG_OUT.warning("No tempBitmap!\n");
      return; }
    AConvertingBitLine expandedLine(width,expandedBits,1);
    AConvertingBitLine tempLine(width,goalBits,goalPlanes);
    if(needNewPalette) {
      tempBitmap->calcColors();
      if(colors) { if(colors->isHBrite()) tempBitmap->colors->setHBrite(); }
      if(suggestPalette) {
        DEBUG_OUT<<"Building palette from suggestion...\n";
        DEBUG_OUT<<"suggestPalette->getNumColors()=="<<suggestPalette->getNumColors()<<"\n";
        if(tempBitmap->colors) {
          tempBitmap->colors->copy(colors);
          tempBitmap->colors->createEmptyMapAndHistogram();
          suggestPalette->createEmptyMapAndHistogram();
          tempBitmap->colors->createSimpleMap(suggestPalette);
        }
      }
      else {
        DEBUG_OUT<<"Rebuilding palette to fit...\n";
        tempBitmap->colors->remap(colors);
      }
      if(tempBitmap->colors) {
        DEBUG_OUT<<"tempBitmap->colors->getNumColors()=="<<tempBitmap->colors->getNumColors()<<"\n";
        expandedLine.colors=tempBitmap->colors;  expandedLine.ownColors=false;
        tempLine.colors=tempBitmap->colors;  tempLine.ownColors=false;
      }
      if(colors) DEBUG_OUT<<"colors->getNumColors()=="<<colors->getNumColors()<<"\n";
    }
    unsigned int q=0;
    if(true) {
      DEBUG_OUT<<"Convert inner loop...\n";
      AConvertingBitLine *origLine=getLine(0);
      if(!origLine) { //DEBUG_OUT.warning("Bad origLine!\n");
        return; }
      for(unsigned int y=0;y<height;y++) {
        copyLineTo(origLine,y);
        //origLine->debugDump();
        if(expandedBits==8) {
          if(bitmapType==A_BITMAP_CHUNKY)
            expandedLine.chunky2Chunky8(origLine);
          else expandedLine.planes2Chunky8(origLine);
          if(needNewPalette) expandedLine.remapChunky8(tempBitmap->colors);
          if(suggestType==A_BITMAP_CHUNKY)
            tempLine.eightBits2Chunky(&expandedLine);
          else tempLine.eightBits2Planes(&expandedLine);
        }
        else {
          if(bitmapType==A_BITMAP_CHUNKY)
            expandedLine.chunky2Chunky24(origLine);
          else expandedLine.planes2Chunky24(origLine);
          //expandedLine.debugDump();
          if(suggestType==A_BITMAP_CHUNKY)
            tempLine.twentyFourBits2Chunky(&expandedLine);
          else tempLine.twentyFourBits2Planes(&expandedLine);
          //tempLine.debugDump();

        }
        for(p=0;p<goalPlanes;p++) {
          tempBitmap->writeLine(y,tempLine.getPlane(p),tempLine.getBytesPerLine(),p);
        }
        q++;
        if(q==10) q=0;
      }
      if(origLine) { delete origLine; origLine=(AConvertingBitLine *)NULL; }
      DEBUG_OUT<<"Done inner loop.\n";
    }
    else DEBUG_OUT<<"pixelsArePrecious not set, pixels not actually converted.\n";
    // Free old planes which we aren't going to use
    DEBUG_OUT<<"Dumping old pixels...\n";
    for(unsigned int t=0;t<nPlanes;t++) {
      if(plane[t]) {
        if(needFree[t]) {
          //free(plane[t]);
          plane[t]=(unsigned char *)NULL;
          needFree[t]=false;
        }
        else //DEBUG_OUT.warning("No needFree for plane ")<<t<<"!\n"
        ;
      }
    }
    // Change passed bitmap to match what it now is.
    DEBUG_OUT<<"Grabbing new pixels...\n";
    for(p=0;p<goalPlanes;p++) {
      plane[p]=tempBitmap->getPlane(p); needFree[p]=true;
    }
    for(p=0;p<goalPlanes;p++) tempBitmap->stealPlane(p);
    if(colors) {
      if(needNewPalette) {
        DEBUG_OUT<<"Grabbing new palette...\n";
        if(suggestPalette) colors->copy(suggestPalette);
        else colors->copy(tempBitmap->colors);
      }
    }
    if(tempBitmap) { delete tempBitmap; tempBitmap=(ABitmap *)NULL; }
  }
  DEBUG_OUT<<"cleaning up...\n";
  nPlanes=goalPlanes;  depth=goalBits;
  calc();
  bytesPerLine=calcBPL(4);  // Force to multiple of 32-bits
  planeSize=bytesPerLine*height;
  calcColors();
  DEBUG_OUT<<"convert done: depth="<<depth<<" nPlanes="<<nPlanes
    <<" bytesPerLine="<<bytesPerLine<<"\n";
  touch();
  DEBUG_OUT<<"---convert(...)\n";
}


void ABitmap::computeHistogram(unsigned int eBits)
{
  unsigned int bitz=8;
  if(eBits>8) bitz=24;
  AConvertingBitLine *origLine=(AConvertingBitLine *)NULL;
  AConvertingBitLine expandedLine(width,bitz,1);
  origLine=getLine(0);
  if(!origLine) { DEBUG_OUT<<"Bad origLine!\n";  return; }
  origLine->resetHistogram();
  DEBUG_OUT<<"Starting "<<bitz<<"-bit histogram...\n";
  DEBUG_OUT<<"(eBits is "<<eBits<<")\n";
  unsigned int q=0;
  for(unsigned int y=0;y<height;y++) {
    copyLineTo(origLine,y);
    if(bitz==8) {
      if(bitmapType==A_BITMAP_CHUNKY) expandedLine.chunky2Chunky8(origLine);
      else expandedLine.planes2Chunky8(origLine);
    }
    else {
      if(bitmapType==A_BITMAP_CHUNKY) expandedLine.chunky2Chunky24(origLine);
      else expandedLine.planes2Chunky24(origLine);
    }
    expandedLine.cumulativeHistogram(origLine);
    if(!q) {
      DEBUG_OUT<<"Colors: "<<y<<"\n";
    }
    q++;  if(q==10) q=0;
  }
  if(origLine->colors) {
    //origLine->colors->setBitsHint(bitsHint);
    origLine->colors->finishHistogram();
  }
  DEBUG_OUT<<"Histogram completed.\n";
}


// A useful shorthand function...
void ABitmap::convertEightBits()
{
  if((depth==8)&&(bitmapType==A_BITMAP_CHUNKY)) return;
  convert(8,A_BITMAP_CHUNKY,(APalette *)NULL);
}


// A useful shorthand function...
void ABitmap::convertTwentyFourBits()
{
  if((depth==24)&&(bitmapType==A_BITMAP_CHUNKY)) return;
  convert(24,A_BITMAP_CHUNKY,(APalette *)NULL);
}


void ABitmap::init()
{
  width=0;  height=0;  depth=0;  nPlanes=0;
  bitmapType=A_BITMAP_NONE;
  bytesPerLine=0;  planeSize=0;  minBlit=1;  maxBlit=1;
  x=0;  y=0;
  eightBits=false;  twentyFourBits=false;  trueColor=false;
  modified=false;  interleaved=false;  compressed=false;
  colors=(APalette *)NULL;  ownColors=false;
  for(unsigned int t=0;t<NUM_PLANES;t++) {
    plane[t]=(unsigned char *)NULL; needFree[t]=false;
  }
  foreground=0;  background=0xffffffL;
  needsRGBReorder=false;
  lineOffset=(unsigned int *)NULL;
  verifyLock=false;
  okayDokey=false;
  allPlanesSame=false;
  //needsUnmapping=false;
  needsRGBReorder=false;
  modified=false;
  //updateImmediately=false;
  //bitsHint=0;
  foreground=0;  background=0;
}


// This will reverse the bits from LSB to MSB (or vice versa) in the
// bytes that make up a pixel
void ABitmap::snarfAndMungeBits(char *bits,unsigned int bitsize,unsigned int unit,bool mirror)
{
  //DEBUG_OUT<<"*** Snarfing and munging bits: "<<unit<<" "<<mirror<<"\n";
  if(!bits) { DEBUG_OUT<<"no bits!\n"; return; }
  calc();
  if(bitmapType!=A_BITMAP_PLANES) { DEBUG_OUT<<"Can't snarf chunky bits!\n"; return; }
  if(!mirror) { DEBUG_OUT<<"Useless call to snarfAndMungeBits.\n"; return; }
  for(unsigned long tt=0;tt<bitsize;tt++) {
    unsigned char b=0,c=bits[tt];
    if(c&0x80) b|=0x01;
    if(c&0x40) b|=0x02;
    if(c&0x20) b|=0x04;
    if(c&0x10) b|=0x08;
    if(c&0x08) b|=0x10;
    if(c&0x04) b|=0x20;
    if(c&0x02) b|=0x40;
    if(c&0x01) b|=0x80;
    bits[tt]=b;
  }
}


void ABitmap::adjustRGBChunky()
{
  DEBUG_OUT<<"Going to adjust RGB chunky...\n";
  if(!twentyFourBits) { DEBUG_OUT<<"Can't adjust non-24bits!\n"; return; }
  if(bitmapType!=A_BITMAP_CHUNKY) { DEBUG_OUT<<"Can't adjust plane bits!\n"; return; }
  unsigned char r,g,b;
  for(unsigned int tt=0;tt<planeSize;tt+=3) {
    r=plane[0][tt]; g=plane[0][tt+1]; b=plane[0][tt+2];
#ifdef ASYS_WINDOWS
    // Nothing to do...CE likes the format as is...
#ifdef ASYS_WIN32CE
#else
    plane[0][tt]=b; plane[0][tt+1]=g; plane[0][tt+2]=r;
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
    plane[0][tt]=b; plane[0][tt+1]=g; plane[0][tt+2]=r;
#endif // ASYS_OS2
#ifdef ASYS_DOS
    plane[0][tt]=b; plane[0][tt+1]=g; plane[0][tt+2]=r;
#endif // ASYS_DOS
  }
  touch();
}


// This is used when the RGB bits in a truecolor bitmap are
// expected to be in a different order than we normally have them...
void ABitmap::adjustRGBPlanes()
{
  DEBUG_OUT<<"Going to adjust RGB planes...\n";
  if(bitmapType!=A_BITMAP_PLANES) { DEBUG_OUT<<"Can't adjust chunky bits!\n"; return; }
  unsigned char *temp;
  unsigned int t,offset1=0,offset2=0,num=0;
  switch(nPlanes) {
    case 15:
      offset1=0; offset2=5; num=5;
      break;
    case 16:
      offset1=0; offset2=6; num=5;
      break;
    case 24:
      offset1=0; offset2=8; num=8;
      break;
    default:
      //DEBUG_OUT.warning("adjustRGBPlanes not implemented for ")<<nPlanes<<" planes!\n";
      break;
  }
  if(!num) return;
  for(t=0;t<num;t++) {
    temp=plane[t+offset1]; plane[t+offset1]=plane[t+offset2]; plane[t+offset2]=temp;
  }
  touch();
}


/*
// The idea is that this function will modify the bitmap if needed so
// it can be shown on that display
bool ABitmap::verifyOkay(ADisplay *dis)
{
  DEBUG_OUT<<"ABitmap::verifyOkay(dis)\n";
  if(!dis) { DEBUG_OUT<<"no dis!\n"; return false; }
  if(bitmapType==A_BITMAP_NONE) {
    //DEBUG_OUT.warning("Type==NONE!!!\n");
    okayDokey=false;  return okayDokey;
  }
  if(verifyLock) return true;
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
  if(bitmapType!=targetType) please=true;
  if(depth!=dis->getDepth()) { targetDepth=dis->getDepth(); please=true; }
  if(targetType==A_BITMAP_PLANES) {
    if(nPlanes!=dis->getNPlanes()) { targetDepth=dis->getNPlanes(); please=true; }
  }
  if(dis->canUseAnyImage()) please=false;
  DEBUG_OUT<<"(please is "<<please<<")\n";
  if(please) {
    convert(targetDepth,targetType,targetColors);
    //freePrivateBits();  createPrivateBits(true);
  }
  okayDokey=true;
  update();
  verifyLock=false;
  modified=false;
  DEBUG_OUT<<"---verifyOkay(dis)\n";
  return okayDokey;
}
*/


/*
// The idea is that this function will modify the bitmap if needed so
// it can be shown on that window
bool ABitmap::verifyOkay(AWindowAbstract *win)
{
  if(!win) { DEBUG_OUT<<"no win!\n"; return false; }
  ADisplay *dis=win->getParent();
  if(dis) return verifyOkay(dis); else return false;
}
*/


// Rebuild the private bits if they were stale
void ABitmap::update()
{
  touch();
}


// A way to say that you did something to the bits and privateBits are probably stale now
void ABitmap::touch()
{
  modified=true;  okayDokey=false;
  //freePrivateBits(); setPrivateBits(true);
}


// FIXME: This function needs to be cleaned up a lot
void ABitmap::copyBlock(ABitmap *source, unsigned int sx, unsigned int sy,unsigned int dx,unsigned int dy,unsigned bw,unsigned bh)
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
      DEBUG_OUT<<"bitmap::draw unknown bitmap format!\n";
      exit(5);
      break;
  }
  touch();
}


// FIXME: This function needs to be cleaned up a lot
void ABitmap::draw(ABitmap *source, unsigned int dx, unsigned int dy)
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

int ABitmap::getPenX()
{
  std::cerr<<"ABitmap getPenX called!\n"; exit(5);
}


int ABitmap::getPenY()
{
  std::cerr<<"ABitmap getPenY called!\n"; exit(5);
}


void ABitmap::scroll(int dx,int dy)
{
  std::cerr<<"ABitmap scroll called!\n"; exit(5);
}


void ABitmap::moveTo(unsigned int newX, unsigned int newY)
{
  std::cerr<<"ABitmap moveTo called!\n"; exit(5);
}


void ABitmap::lineTo(unsigned int newX, unsigned int newY)
{
  std::cerr<<"ABitmap lineTo called!\n"; exit(5);
}


void ABitmap::eraseRect(unsigned int rwidth,unsigned int rheight)
{
  std::cerr<<"ABitmap eraseRect called!\n"; exit(5);
}


void ABitmap::drawRect(unsigned int rwidth,unsigned int rheight)
{
  std::cerr<<"ABitmap drawRect called!\n"; exit(5);
}


void ABitmap::drawFilledRect(unsigned int rwidth,unsigned int rheight)
{
  std::cerr<<"ABitmap drawFilledRect called!\n"; exit(5);
}
