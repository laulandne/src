
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

TODO:

As is there are HUGE memory leaks here...

We want to have a default image for all gfx and sprites, but
we also want to delete all gfx and sprite data whenever we set up
and at various other times...but we end up deleting the same image
more than once (the default one)...so we really need to define
a default image for EACH AND EVERY gfx and sprite slot.

Instead we just don't delete anything for now...

*/


#include <portable/nickcpp/APalette.h>

#include <portable/nickcpp/AStringHelper.h>

#include <elib/EVideoIOAbstract.h>
#include <elib/EMemMapAbstract.h>
#include <elib/EHandler.h>
#include <elib/ERam.h>
#include <elib/ERom.h>

#include <cpulib/ECPUAbstract.h>

//#include <guilib/AWindow.h>
//#include <guilib/ADisplay.h>
//#include <guilib/AEvent.h>
//#include <guilib/APrivateBitmap.h>
#include <portable/nickcpp/ANullStream.h>


#include <math.h>


//#define SHOW_VBLANK 1

//#define SCALE_EVERYTHING 1


// Close to NTSC standard...
#define A_MAX_FIELD_RASTER 263
#define A_VBLANK_SIZE      21


// Useful triplet breaking macros...
// These are for getting 8 bit colors out of a 24 bit triplet
#define getRGBRed24(x)   ((unsigned int)((x>>16)&0xff))
#define getRGBGreen24(x) ((unsigned int)((x>>8)&0xff))
#define getRGBBlue24(x)  ((unsigned int)(x&0xff))
// These are for getting 4 bit colors out of a 12 bit triplet
#define getRGBRed12(x)   ((unsigned int)((x>>8)&0xf))
#define getRGBGreen12(x) ((unsigned int)((x>>4)&0xf))
#define getRGBBlue12(x)  ((unsigned int)(x&0xf))
// These are for getting 3 bit colors out of a 9 bit triplet
#define getRGBRed9(x)    ((unsigned int)((x>>6)&0x7))
#define getRGBGreen9(x)  ((unsigned int)((x>>3)&0x7))
#define getRGBBlue9(x)   ((unsigned int)(x&0x7))


////////////////////////////////////////////////////////////////////////////////
//  EVideoIOAbstract Class
////////////////////////////////////////////////////////////////////////////////

EVideoIOAbstract::EVideoIOAbstract(EHandler *parent,unsigned int ncol,unsigned int nrow, unsigned int depthHint) : EMemIO(parent)
{
  DEBUG_OUT<<"EVideoIOAbstract::EVideoIOAbstract("<<ncol<<","<<nrow<<") Setting up video.\n";
  init();
  letter='v';
  setName("video_memory");
  redrawLock=true;
  screenAddr=0x0400;  // an arbitrary default
  tileWidth=8;  // an arbitrary default
  tileHeight=8;  // an arbitrary default
  spriteAddr=0;
  location=screenAddr;
  tilesAcross=ncol;  tilesDown=nrow;
  colors=new APalette(pow((float)2,(float)depthHint));
  // first rough guess...
  emuWidth=tilesAcross*tileHeight;  emuHeight=tilesDown*tileWidth;
  emuDepth=depthHint; emuNPlanes=1;  // an arbitrary default
/*
  videod=aDefaultADisplay;
  if(videod) { if(videod->getWidth()<emuWidth) videod=NULL; }
  if(videod) { if(videod->getHeight()<emuHeight) videod=NULL; }
  if(!videod) videod=new ADisplay(emuWidth,emuHeight,emuDepth);
  if(videod) {
    if(videod->err.error()) {  DEBUG_OUT<<"videod has an error!\n";  exit(5); }
  }
  else { DEBUG_OUT<<"Didn't get videod!\n"; exit(5); }
  if(err.error()) { DEBUG_OUT<<"EVideoIOAbstract has an error!\n"; return; }
*/
/*
  videow=aDefaultAWindow;
  if(!videow) videow=new AWindow(videod,"EVideoIO",(emuWidth+(borderSize*2))*scaleFactorX,(emuHeight+(borderSize*2))*scaleFactorY);
  if(videow) {
    if(videow->err.error()) {  DEBUG_OUT<<"videow has an error!\n";  exit(5); }
  }
  else { DEBUG_OUT<<"Didn't get videow!\n"; exit(5);  return;  }
  if(err.error()) { DEBUG_OUT<<"EVideoIOAbstract has an error!\n"; return; }
*/
#ifdef SCALE_EVERYTHING
  scaleFactorX=videow->getParent()->calcScaleFactorX(emuWidth+(borderSize*2));
  scaleFactorY=videow->getParent()->calcScaleFactorY(emuHeight+(borderSize*2));
#else
  scaleFactorX=1;  scaleFactorY=1;
#endif // SCALE_EVERYTHING
  if(scaleFactorX>4) scaleFactorX=4;  // For now, just a sanity check
  if(scaleFactorY>4) scaleFactorY=4;  // For now, just a sanity check
  DEBUG_OUT<<"scale factors are: "<<scaleFactorX<<"x"<<scaleFactorY<<"\n";
  //videow->resize((emuWidth+(borderSize*2))*scaleFactorX,(emuHeight+(borderSize*2))*scaleFactorY);
  //videow->centerOnDisplay();
  //unsigned int winxcoord=videod->getWidth()-width-30;
  unsigned int winxcoord=10;
  //
  /*
  if(tileImage) delete tileImage;
  tileImage=new APrivateBitmap(256*scaleFactorX,256*scaleFactorY,videow->getDepth(),videow->getNPlanes()); //TODO: Fix arb fixed size!
  tileImage->setLetter('@');
  tileImage->verifyOkay(videow);
  if(spriteImage) delete spriteImage;
  spriteImage=new APrivateBitmap(256*scaleFactorX,256*scaleFactorY,videow->getDepth(),videow->getNPlanes()); //TODO: Fix arb fixed size!
  spriteImage->setLetter('@');
  spriteImage->verifyOkay(videow);
  APalette *p=tileImage->getColors();
  if(p) {
    p->setColor(0,0x00,0x00,0x00);
    p->setColor(1,0xff,0xff,0xff);
  }
  */
  framesPerSec=60;  // 60HZ (for now)
  //
  setMemSize(tilesAcross,tilesDown,true);
  //DEBUG_OUT<<"Going to setScreenChar...\n";
  setScreenChar(location,0);
  setBackground(0);
  //DEBUG_OUT<<"Going to clearVideo...\n";
  //clearVideo();
  //DEBUG_OUT<<"Going to setColorAddr...\n";
  setColorMemAddr(0);
  //DEBUG_OUT<<"Going to setup raster...\n";
  setRasterFactor(3);
  setRasterSpeed(1);
  setMaxRaster(A_MAX_FIELD_RASTER+A_VBLANK_SIZE);
  //DEBUG_OUT<<"Going to createBackBuffer...\n";
  createBackBuffer();
  setTextMode(false);
  //videow->centerOnDisplay();
  redrawLock=false;
  DEBUG_OUT<<"EvideoIO::EVideoIO Done setting up video.\n";
}


EVideoIOAbstract::~EVideoIOAbstract()
{
  DEBUG_OUT<<"Destroying EVideoIOAbstract\n";
  if(!err.error()) freeVideo();
  err.set();
}


void EVideoIOAbstract::init()
{
  // Init all data members
  attrMem=(EMemAbstract *)NULL;
  unsigned int t;
  tilesAcross=0;  tilesDown=0;
  screenAddr=0;  tileAddr=0;  attrAddr=0;  spriteAddr=0;
  //tileImage=(APrivateBitmap *)NULL;
  //spriteImage=(APrivateBitmap *)NULL;
  //backBuffer=(APrivateBitmap *)NULL;
  tileWidth=0;  tileHeight=0;  imageTilesAcross=0;  imageTilesDown=0;
  screenRowAddrs=(BYTE *)NULL;
  screenColAddrs=(BYTE *)NULL;
  screenValidAddrs=(BYTE *)NULL;
  roundedSize=0;  ramSize=0;
  redrawLock=false;
  raster=0;  maxRaster=0;
  causeInt=false;  intCaused=false;
  whichInt=0;
  rasterInt=0;
  redrawLock=false;
  vblankStart=0;
  rasterFactor=1;  // This can't be zero due to division...
  rasterSpeed=0;
  framesPerSec=0;
  for(t=0;t<A_MAX_NUMBER_GFX;t++) {
    tiles[t].init();
  }
  numSprites=0;
  for(t=0;t<A_MAX_NUMBER_SPRITES;t++) {
    sprites[t].init();
  }
  spriteRegisters=0;
  spriteHandler=(EMemIO *)NULL;
  bitmapMode=false;
  for(t=0;t<32;t++) { plane[t]=0; planeActive[t]=false; }
  displayList=false;
  theDisplayList=(unsigned int *)NULL;
  passedRasterInt=false;
  emuWidth=0;  emuHeight=0;
  emuDepth=0; emuNPlanes=0;
  trueColor=false;
  //videow=NULL;
  //videod=NULL;
  colors=NULL;
  scaleFactorX=1;
  scaleFactorY=1;
  scaled=false;
  borderSize=0;
  borderColor=0xffffff;
  backgroundColor=0x000000;
  defaultForegroundColor=0xffffff;
  ntsc=true;
}


// TODO: Major problem freeing the image arrays...
// this is due to my setting [0] to point to defaultImage
// in the constructor to fix attempting to draw a bunch of NULL's
// at the initial redraw for systems like the commodore 64...a better
// fix is to have a default set, etc, blah blah blah...you know.
void EVideoIOAbstract::freeVideo()
{
  if(err.error()) { DEBUG_OUT<<"EVideoIO with error!\n"; return; }
  if(handler) {
    //if(io->getMap()) io->getMap()->clearVideoMap();
  }
  if(mem) free(mem);
  mem=(BYTE *)NULL;
  if(screenRowAddrs) free(screenRowAddrs);
  screenRowAddrs=(BYTE *)NULL;
  if(screenColAddrs) free(screenColAddrs);
  screenColAddrs=(BYTE *)NULL;
  if(screenValidAddrs) free(screenValidAddrs);
  screenValidAddrs=(BYTE *)NULL;
  if(attrMem) delete attrMem;
  attrMem=(EMemAbstract *)NULL;
  /*
  if(tileImage) {
    if(!tileImage->err.error()) {
      //tileImage->setError();
      //delete tileImage;
    }
  }
  tileImage=(APrivateBitmap *)NULL;
  if(spriteImage) {
    if(!spriteImage->err.error()) {
      //spriteImage->setError();
      delete spriteImage;
    }
  }
  spriteImage=(APrivateBitmap *)NULL;
  if(backBuffer) {
    if(!backBuffer->err.error()) {
      //backBuffer->err.set();
      delete backBuffer;
    }
  }
  */
  //backBuffer=(APrivateBitmap *)NULL;
  if(colors) {
    delete colors;
  }
  colors=(APalette *)NULL;
}


BYTE EVideoIOAbstract::write8(ULONG addr,BYTE value)
{
  unsigned int ea=addr-location;
  if(ea>memsize) return 0;
  if(screenValidAddrs[ea]) {
    unsigned int col=screenColAddrs[ea]-1;
    unsigned int row=screenRowAddrs[ea]-1;
    unsigned int sx=tiles[value].sx;
    unsigned int sy=tiles[value].sy;
    unsigned int sw=tiles[value].width;
    unsigned int sh=tiles[value].height;
    unsigned int dx=(col*tileWidth+borderSize)*scaleFactorX;
    unsigned int dy=(row*tileHeight+borderSize)*scaleFactorY;
    //videow->copyBlock(tiles[value].source,sx,sy,dx,dy,sw,sh);
  }
  else DEBUG_OUT<<"invalid evideo write at "<<ea<<"\n";
  if(addr>=spriteRegisters) // Gack!  Very C-64 specific!
  {
    if(spriteHandler) spriteHandler->write8(addr,value);
  }
  mem[ea]=value;
  return value;
}


void EVideoIOAbstract::clearVideo()
{
  //videow->clear();
  // Clear video memory
  //for(unsigned int t=0;t<memsize;t++) {
    //write8(t+location,0);  attrMem->write8(t,1);
  //}
  redraw();
}


void EVideoIOAbstract::redraw()
{
  //videow->resize((emuWidth+(borderSize*2))*scaleFactorX,(emuHeight+(borderSize*2))*scaleFactorY);
  //videow->setForeground(defaultForegroundColor);
  //videow->setBackground(backgroundColor);
  //videow->clear();
  if(redrawLock) { DEBUG_OUT<<"(redrawLock was set)\n"; return; }
  for(unsigned int i=0;i<memsize;i++) {
    if(screenValidAddrs[i]) write8(location+i,read8(location+i));
  }
  drawSprites();
  drawBorder();
}


void EVideoIOAbstract::focus()
{
  // TODO
}


void EVideoIOAbstract::reallyFocus()
{
  // TODO
}


void EVideoIOAbstract::createBackBuffer()
{
  /*
  if(backBuffer) free(backBuffer);
  // TODO: remove assumptions about backBuffer in nupac.cpp!!!
  backBuffer=new APrivateBitmap(emuWidth*scaleFactorX,emuHeight*scaleFactorY,emuDepth,emuNPlanes);
  if(!backBuffer) { DEBUG_OUT<<"Couldn't createBackBuffer!\n"; return; }
  if(backBuffer->err.error()) { DEBUG_OUT<<"Error creating backBuffer!\n"; return; }
  backBuffer->verifyOkay(videow);
  DEBUG_OUT<<"createBackBuffer: buffer is "<<emuWidth*scaleFactorX<<"x"<<emuHeight*scaleFactorY<<"x";
  DEBUG_OUT<<emuDepth<<"x"<<emuNPlanes<<"\n";
  */
}


void EVideoIOAbstract::setLocation(ULONG ad)
{
  DEBUG_OUT<<"EVideoIOAbstract::setLocation...\n";
  //if(ad==location) { DEBUG_OUT<<"EVideoIOAbstract::setLocation already there!\n"; return; }
  if(!handler) { DEBUG_OUT<<"EVideoIOAbstract::setLocation no handler!\n"; return; }
  if(!handler->getMap()) { DEBUG_OUT<<"EVideoIOAbstract::setLocation no map!\n"; return; }
  if(handler->getMap()->err.error()) return;
  location=ad;
  handler->getMap()->mapVideo(this);
}


void EVideoIOAbstract::setScreenChar(unsigned int s,unsigned int c,bool force)
{
  DEBUG_OUT<<"EVideoIOAbstract::setScreenChar("<<s<<","<<c<<")\n";
  if((s==screenAddr)&&(c==tileAddr)) { if(!force) return; } // already there...
  bool rd=force;
  if(s!=screenAddr) {
    DEBUG_OUT<<"screen addr changing to "<<s<<"\n";
    setLocation(s);
    screenAddr=s;
    rd=true;
  }
  if(c!=tileAddr) {
   tileAddr=c;
   rd=true;
  }
  if(rd) redraw();
}


void EVideoIOAbstract::setVideoSize(unsigned int c,unsigned int r)
{
  DEBUG_OUT<<"EVideoIOAbstract::setVideoSize("<<c<<","<<r<<")\n";
  if((c!=tilesAcross)||(r!=tilesDown)) {
    if(c) tilesAcross=c;
    if(r) tilesDown=r;
    unsigned int w=tilesAcross*tileWidth,h=tilesDown*tileHeight;
    oldScaleFactorX=scaleFactorX;
    oldScaleFactorY=scaleFactorY;
    //scaleFactorX=videow->getParent()->calcScaleFactorX(w+(borderSize*2));
    //scaleFactorY=videow->getParent()->calcScaleFactorY(h+(borderSize*2));
    if((scaleFactorX!=oldScaleFactorY)||(scaleFactorX!=oldScaleFactorY)) scaleFactorChanged();
    DEBUG_OUT<<"going to resize to "<<w*scaleFactorX<<","<<h*scaleFactorY<<"\n";
    emuWidth=w*scaleFactorX;  emuHeight=h*scaleFactorY;
    //videow->resize((w+(borderSize*2))*scaleFactorX,(h+(borderSize*2))*scaleFactorY);
    //videow->centerOnDisplay();
    setOffsets();
  }
  redraw();
}


void EVideoIOAbstract::setViewableSize(unsigned int w,unsigned int h)
{
  DEBUG_OUT<<"EVideoIOAbstract::setViewableSize("<<w<<","<<h<<")\n";
  oldScaleFactorX=scaleFactorX;
  oldScaleFactorY=scaleFactorY;
  //scaleFactorX=videow->getParent()->calcScaleFactorX(w+(borderSize*2));
  //scaleFactorY=videow->getParent()->calcScaleFactorY(h+(borderSize*2));
  if((scaleFactorX!=oldScaleFactorY)||(scaleFactorX!=oldScaleFactorY)) scaleFactorChanged();
  DEBUG_OUT<<"going to resize to "<<w*scaleFactorX<<","<<h*scaleFactorY<<"\n";
  emuWidth=w*scaleFactorX;  emuHeight=h*scaleFactorY;
  //videow->resize((w+(borderSize*2))*scaleFactorX,(h+(borderSize*2))*scaleFactorY);
  //videow->centerOnDisplay();
  redraw();
}


void EVideoIOAbstract::setMemSize(unsigned int c,unsigned int r,bool really)
{
  DEBUG_OUT<<"EVideoIOAbstract::setMemSize("<<c<<","<<r<<")\n";
  unsigned int newsize=c*r;
  unsigned int k=newsize/1024;
  unsigned int ktry=k*1024;
  if(ktry<newsize) { k++;  ktry=k*1024; }
  if(ktry<newsize) { k++;  ktry=k*1024; }
  newsize=ktry;
  bool will=really;
  if(memsize!=newsize) will=true;
  if(will) {
    resize(newsize);
    if(attrMem) attrMem->resize(newsize);
    else {
      attrMem=new ERam(memsize);
      if(attrMem) {
        attrMem->setName("color_mem");
        attrMem->setShowInMap(false);
      }
    }
    setColorMemAddr(attrAddr);
    initOffsets();
    DEBUG_OUT<<"set memsize to "<<memsize<<".\n";
  }
  setVideoSize(c,r);
}


void EVideoIOAbstract::setDefaultForeground(ULONG val)
{
  defaultForegroundColor=val;
  setScreenChar(screenAddr,tileAddr,true);
  //videow->setForeground(val);
  redraw();
}


void EVideoIOAbstract::setBackground(ULONG val)
{
  std::cerr<<"setBackground("<<aDefaultStringHelper.myHex8(val)<<")\n";
  backgroundColor=val;
  setScreenChar(screenAddr,tileAddr,true);
  //videow->setBackground(val);
  redraw();
}


void EVideoIOAbstract::setBorder(unsigned int bwidth,ULONG bclr)
{
  borderSize=bwidth;  borderColor=bclr;
  //redraw();
  drawBorder();
}


void EVideoIOAbstract::scaleFactorChanged()
{
  // TODO: Regenerate the tile and sprite data
  DEBUG_OUT<<"scaleFactorChanged()!\n";
  scaleFactorX=oldScaleFactorX;
  scaleFactorY=oldScaleFactorY;
}


BYTE EVideoIOAbstract::write8ColorMem(ULONG address,BYTE val)
{
  if(!attrMem) return val;
  ULONG off=address-attrAddr;
  attrMem->write8(off,val);
  write8(location+off,read8(location+off));
  return val;
}


BYTE EVideoIOAbstract::read8ColorMem(ULONG address)
{
  if(!attrMem) return 0;
  return attrMem->read8(address-attrAddr);
}


void EVideoIOAbstract::setColorMemAddr(ULONG addr)
{
  // Color's location MUST always be 0 unless it is mapped itself
  // (which it isn't yet...instead the EVideoIO writes to it for us
  // TODO: why?
  //color->setLocation(addr);
  attrAddr=addr;
  //redraw();
}


void EVideoIOAbstract::initOffsets()
{
  if(!memsize) { DEBUG_OUT<<"Can't initOffsets, zero size!\n"; return; }
  ramSize=memsize;
  DEBUG_OUT<<"InitOffsets: ramSize is "<<ramSize<<"\n";
  unsigned int k=memsize/1024;
  unsigned int rtry=k*1024;
  if(rtry<memsize) { k++; rtry=k*1024; }
  if(rtry<memsize) { k++; rtry=k*1024; }
  roundedSize=rtry;
  DEBUG_OUT<<"roundedSize is "<<roundedSize<<"\n";
  if(screenRowAddrs) free(screenRowAddrs);
  screenRowAddrs=(BYTE *)calloc(k,1024);
  if(!screenRowAddrs) { DEBUG_OUT<<"No memory for screenRowAddrs!\n";  exit(5); }
  if(screenColAddrs) free(screenColAddrs);
  screenColAddrs=(BYTE *)calloc(k,1024);
  if(!screenColAddrs) { DEBUG_OUT<<"No memory for screenColAddrs!\n";  exit(5); }
  if(screenValidAddrs) free(screenValidAddrs);
  screenValidAddrs=(BYTE *)calloc(k,1024);
  if(!screenValidAddrs) { DEBUG_OUT<<"No memory for screenValidAddrs!\n";  exit(5); }
  setValid();
  setOffsets();
}


void EVideoIOAbstract::setValid()
{
  DEBUG_OUT<<"Using default setValid...\n";
  if(!screenValidAddrs) { DEBUG_OUT<<"(screenValidAddrs was NULL, giving up!)\n"; return; }
  unsigned int t;
  for(t=0;t<roundedSize;t++) {
    screenValidAddrs[t]=0;
  }
  for(t=0;t<ramSize;t++) {
    screenValidAddrs[t]=1;
  }
}


void EVideoIOAbstract::setOffsets()
{
  DEBUG_OUT<<"Using default setOffsets...\n";
  if(!screenRowAddrs) { DEBUG_OUT<<"(screenRowAddrs was NULL, giving up!)\n"; return; }
  if(!screenColAddrs) { DEBUG_OUT<<"(screenColAddrs was NULL, giving up!)\n"; return; }
  unsigned int ad;
  for(unsigned int y=0;y<tilesDown;y++) {
    //DEBUG_OUT<<y<<": ";
    for(unsigned int x=0;x<tilesAcross;x++) {
      ad=(y*tilesAcross)+x;
      screenRowAddrs[ad]=(BYTE)(y+1);
      screenColAddrs[ad]=(BYTE)(x+1);
      //DEBUG_OUT<<(int)screenColAddrs[ad]<<","<<(int)screenRowAddrs[ad]<<" ";
    }
    //DEBUG_OUT<<"\n";
  }
  //for(unsigned int t=0;t<memsize;t++) {
    //DEBUG_OUT<<t<<": "<<(int)screenColAddrs[t]<<","<<(int)screenRowAddrs[t]<<"\n";
  //}
}


void EVideoIOAbstract::vregs()
{
  if(!rasterFactor) rasterFactor=1;  // Correct this if it got set to zero somehow...
  DEBUG_OUT<<"video info:\n";
  my_vregs();
  DEBUG_OUT<<"location "<<location<<"\n";
  DEBUG_OUT<<"screenAddr "<<screenAddr<<"\n";
  DEBUG_OUT<<"tileAddr "<<tileAddr<<"\n";
  DEBUG_OUT<<"attrAddr "<<attrAddr<<"\n";
  DEBUG_OUT<<"size "<<memsize<<"\n";
  DEBUG_OUT<<"actualSize "<<actualSize<<"\n";
  DEBUG_OUT<<"tiles "<<tilesAcross<<"x"<<tilesDown<<" (";
  if(bitmapMode) DEBUG_OUT<<"bitmap"; else DEBUG_OUT<<"text";
  DEBUG_OUT<<" mode)\n";
  DEBUG_OUT<<"tilesize "<<tileWidth<<"x"<<tileHeight<<" (";
  DEBUG_OUT<<imageTilesAcross<<" by "<<imageTilesDown<<")\n";
  DEBUG_OUT<<"raster="<<raster<<" ("<<raster/rasterFactor<<")\n";
  DEBUG_OUT<<"max="<<maxRaster<<" ("<<maxRaster/rasterFactor<<")\n";
  DEBUG_OUT<<"vblank="<<vblankStart<<" ("<<vblankStart/rasterFactor<<")\n";
  DEBUG_OUT<<"causeInt="<<causeInt<<"\n";
  DEBUG_OUT<<"whichInt="<<whichInt<<"\n";
  DEBUG_OUT<<"rasterInt="<<rasterInt<<" ("<<rasterInt/rasterFactor<<")\n";
  DEBUG_OUT<<"rasterFactor="<<rasterFactor<<"\n";
  DEBUG_OUT<<"rasterSpeed="<<rasterSpeed<<"\n";
  if(numSprites) {
    DEBUG_OUT<<"Sprites: "<<numSprites<<"\n";
    unsigned int t;
    DEBUG_OUT<<"A ";  for(t=0;t<numSprites;t++) DEBUG_OUT<<sprites[t].active<<" ";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"G ";  for(t=0;t<numSprites;t++) DEBUG_OUT<<sprites[t].gfx<<" ";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"C ";  for(t=0;t<numSprites;t++) DEBUG_OUT<<sprites[t].color<<" ";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"X ";  for(t=0;t<numSprites;t++) DEBUG_OUT<<sprites[t].x<<" ";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"Y ";  for(t=0;t<numSprites;t++) DEBUG_OUT<<sprites[t].y<<" ";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"W ";  for(t=0;t<numSprites;t++) DEBUG_OUT<<sprites[t].width<<" ";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"H ";  for(t=0;t<numSprites;t++) DEBUG_OUT<<sprites[t].height<<" ";
    DEBUG_OUT<<"\n";
  }
}


void EVideoIOAbstract::my_vregs()
{
}


void EVideoIOAbstract::io_cycle()
{
  // NOTE!  The order of these are critical!
  raster+=rasterSpeed;
#ifdef SHOW_VBLANK
  if(raster>vblankStart) DEBUG_OUT<<"v";
#endif
  if(raster>maxRaster) {
    raster=0;
    drawSprites();
    passedRasterInt=false;
  }
  // Should actually check if we missed it due to factor or something...
  if((raster>rasterInt)&&(!passedRasterInt)) {
    if(causeInt) {
#ifdef SHOW_VBLANK
      DEBUG_OUT<<"%";
#endif
      intCaused=true;
      handler->getCPU()->cause_int(whichInt);
      passedRasterInt=true;
    }
  }
}


void EVideoIOAbstract::setRasterInt(unsigned long v,unsigned int wh)
{
  rasterInt=v*rasterFactor;
  whichInt=wh; causeInt=true;
  DEBUG_OUT<<"Raster Int set "<<whichInt<<" at "<<rasterInt<<"\n";
}


void EVideoIOAbstract::unsetRasterInt()
{
  DEBUG_OUT<<"Raster Int disabled.\n";
  causeInt=false;
}


unsigned long EVideoIOAbstract::getRaster()
{
  return raster/rasterFactor;
}


void EVideoIOAbstract::setMaxRaster(unsigned long v)
{
  maxRaster=v*rasterFactor;
  vblankStart=maxRaster-(A_VBLANK_SIZE*rasterFactor);
}


bool EVideoIOAbstract::getIntCaused()
{
  bool t=intCaused;
  DEBUG_OUT<<"Checking for video int ("<<t<<")\n";
  //intCaused=false;
  return t;
}


void EVideoIOAbstract::clearInt()
{
  intCaused=false;
}


/*
void EVideoIOAbstract::setupTileGfx(APrivateBitmap *timage,unsigned int gx,unsigned int gy,unsigned int gw,unsigned int gh)
{
  if(!timage) { DEBUG_OUT<<"No image to setupTileGfx\n";  return; }
  if(timage->err.error()) { DEBUG_OUT<<"Image error in setupTileGfx\n";  return; }
  DEBUG_OUT<<"EVideo::setupTileGfx\n";
  DEBUG_OUT<<"gx="<<gx<<" gy="<<gy<<" gw="<<gw<<" gh="<<gh<<"\n";
  if(!scaled) {
    scaled=true;
    DEBUG_OUT<<"(scaling...)\n";
    timage->scale(scaleFactorX,scaleFactorY);
  }
  timage->update();
  DEBUG_OUT<<"(updated, now going to verify...)\n";
  // TODO: Obviously something like this is needed...
  timage->verifyOkay(videow);
  DEBUG_OUT<<"(verified, now going to matchColors...)\n";
  videow->matchColors(timage->getColors());
  DEBUG_OUT<<"(matchedColors...)\n";
  tileWidth=gx;  tileHeight=gy;
  imageTilesAcross=gw;  imageTilesDown=gh;
  DEBUG_OUT<<"Gfx are "<<imageTilesAcross<<" by "<<imageTilesDown<<" size "<<tileWidth<<" by "<<tileHeight<<"\n";
  unsigned int x,y;
  DEBUG_OUT<<"(Going to build gfx image table...)\n";
  //unsigned int w=tilesAcross*tileWidth,h=tilesDown*tileHeight;
  for(y=0;y<gh;y++) {
    DEBUG_OUT<<y<<":";
    for(x=0;x<gw;x++) {
      DEBUG_OUT<<x<<",";
      unsigned int value=x+y*gw;
      tiles[value].source=timage;
      tiles[value].width=tileWidth;
      tiles[value].height=tileHeight;
      tiles[value].sx=x*gx*scaleFactorX;
      tiles[value].sy=y*gy*scaleFactorY;
    }
    DEBUG_OUT<<"\n";
  }
  //DEBUG_OUT<<"going to resize to "<<w*scaleFactorX<<","<<h*scaleFactorY<<"\n";
  //videow->resize((w+borderSize)*scaleFactorX,(h+borderSize)*scaleFactorY);
  //videow->centerOnDisplay();
  //videow->moveTo(0,0); videow->draw(timage);
  //pleaseWaitForKey(videow);
  //DEBUG_OUT<<"---EVideo::setupTileGfx\n";
  //showGfx();
}
*/


/*
void EVideoIOAbstract::showTileGfx()
{
  DEBUG_OUT<<"Gfx are "<<imageTilesAcross<<" by "<<imageTilesDown<<" size "<<tileWidth<<" by "<<tileHeight<<"\n";
  //AWindow *showw=new AWindow(imageTilesAcross*tileWidth*scaleFactor+64,imageTilesDown*tileHeight*scaleFactor+64);  // The 64's are arbitrary
  //showw->matchColors(videow->getColors());
  videow->clear();
  videow->moveTo(0,0);
  unsigned int x,y;
  for(y=0;y<imageTilesDown;y++) {
    for(x=0;x<imageTilesAcross;x++) {
      unsigned int value=x+y*imageTilesAcross;
      unsigned int sx=tiles[value].sx;
      unsigned int sy=tiles[value].sy;
      unsigned int sw=tiles[value].width;
      unsigned int sh=tiles[value].height;
      unsigned int dx=x*tileWidth*scaleFactorX;
      unsigned int dy=y*tileHeight*scaleFactorY;
      videow->copyBlock(tiles[value].source,sx,sy,dx,dy,sw,sh);
      DEBUG_OUT<<tiles[value].width<<"x"<<tiles[value].height<<" @";
      DEBUG_OUT<<tiles[value].sx<<","<<tiles[value].sy<<"\n";
    }
  }
  bool drawThemLines=false;
  if(videow->getParent()->getDepth()<4) drawThemLines=false;
  if(drawThemLines) {
    videow->setForeground(0xff0000);
    for(y=0;y<imageTilesDown;y++) {
      for(x=0;x<imageTilesAcross;x++) {
        videow->line(x*tileWidth*scaleFactorX,0,x*tileWidth*scaleFactorX,tileHeight*imageTilesDown*scaleFactorY);
      }
      videow->line(0,y*tileHeight*scaleFactorY,tileWidth*imageTilesAcross*scaleFactorY,y*tileHeight*scaleFactorY);
    }
  }
  showPalette(videow);
  *videow<<"\nTiles: Press any key to return...\n";
  pleaseWaitForKey(videow);
  videow->clear();
  videow->moveTo(0,0);
  //videow->draw(tileImage);
  *videow<<"\nTiles: Press any key to return...\n";
  pleaseWaitForKey(videow);
  videow->clear();
  videow->moveTo(0,0);
  //redraw();
}
*/


/*
// TODO: this doesn't work for the general case...
void EVideoIOAbstract::showSpriteGfx()
{
  if(!numSprites) { DEBUG_OUT<<"(No sprites)\n"; return; }
  DEBUG_OUT<<"spriteGfx are "<<imageSpritesAcross<<" by "<<imageSpritesDown<<" size "<<spriteWidth<<" by "<<spriteHeight<<"\n";
  //AWindow *showw=new AWindow(imageSpritesAcross*spriteWidth*scaleFactor+64,imageSpritesDown*spriteHeight*scaleFactor+64);  // The 64's are arbitrary
  //showw->matchColors(videow->getColors());
  videow->clear();
  videow->moveTo(0,0);
  unsigned int y,x;
  for(y=0;y<imageSpritesDown;y++) {
    for(x=0;x<imageSpritesAcross;x++) {
      unsigned int value=x+y*imageSpritesAcross;
      unsigned int sx=sprites[value].sx;
      unsigned int sy=sprites[value].sy;
      unsigned int sw=sprites[value].width;
      unsigned int sh=sprites[value].height;
      unsigned int dx=x*spriteWidth*scaleFactorX;
      unsigned int dy=y*spriteHeight*scaleFactorY;
      videow->copyBlock(sprites[value].source,sx,sy,dx,dy,sw,sh);
      DEBUG_OUT<<sprites[value].width<<"x"<<sprites[value].height<<" @";
      DEBUG_OUT<<sprites[value].sx<<","<<sprites[value].sy<<"\n";
    }
  }
  bool drawThemLines=true;
  if(videow->getParent()->getDepth()<4) drawThemLines=false;
  if(drawThemLines) {
    videow->setForeground(0xff0000);
    for(y=0;y<imageSpritesDown;y++) {
      for(x=0;x<imageSpritesAcross;x++) {
        videow->line(x*spriteWidth*scaleFactorX,0,x*spriteWidth*scaleFactorX,spriteHeight*imageSpritesDown*scaleFactorY);
      }
      videow->line(0,y*spriteHeight*scaleFactorY,spriteWidth*imageSpritesAcross*scaleFactorX,y*spriteHeight*scaleFactorY);
    }
  }
  showPalette(videow);
  *videow<<"\nSprites: Press any key to return...\n";
  pleaseWaitForKey(videow);
  videow->clear();
  videow->moveTo(0,0);
  //redraw();
}
*/


/*
void EVideoIOAbstract::showPalette(AWindow *showw)
{
  if(!colors) return;
  unsigned int nColors=colors->getNumColors();
  unsigned int ty=showw->getPenY()+16;
  for(unsigned int t=0;t<nColors;t++) {
    unsigned int r=colors->red(t);
    unsigned int g=colors->green(t);
    unsigned int b=colors->blue(t);
    showw->setForeground((r<<16)|(g<<8)|(b),true);
    showw->moveTo(t*16,ty);
    showw->drawFilledRect(16,16);
  }
  showw->setForeground(0,true);
}
*/


/*
void EVideoIOAbstract::pleaseWaitForKey(AWindow *sw)
{
  AEvent e;
  e.type=A_EVENT_NONE;
  bool working=true;
  bool closing=false;
  while(working) {
    switch(e.type) {
      case A_EVENT_KEYUP:
      case A_EVENT_MOUSEUP:
        working=false;
        break;
      case A_EVENT_DESTROY:
      case A_EVENT_BREAK:
        working=false;
        closing=true;
        break;
      default: e.wait(sw); break;
    }
  }
  //if(w->error()) { err.set();  io->setError();  return; }
  if(closing) { err.set(); handler->err.set(); return; }
  //w->clear();
  //redraw();
}
*/


/*
// TODO: For now gx MUST be a multiple of 8
// NOTE: off is offset from start of ERom, and doesn't include the rom's base address...
void EVideoIOAbstract::decodeRomTiles(APrivateBitmap *i, ERom *r, ULONG off, unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown, unsigned int copyMode, unsigned int startRow)
{
  if(!i) { DEBUG_OUT<<"No image to decode rom into!\n"; return; }
  if(!r) { DEBUG_OUT<<"No rom to decode image from!\n"; return; }
  DEBUG_OUT<<"EVideoIOAbstract::decodeRomTiles "<<"gwidth="<<gwidth<<" gheight="<<gheight<<" gacross="<<gacross<<" gdown="<<gdown<<"\n";
  DEBUG_OUT<<"Image info:";
  DEBUG_OUT<<" width="<<i->getWidth()<<" height="<<i->getHeight();
  DEBUG_OUT<<" depth="<<i->getDepth()<<" nplanes="<<i->getNPlanes()<<"\n";
  //i->verifyOkay(videow);
  unsigned char *dest=i->getPlane(0);
  unsigned char bite=0;
  ULONG offset=0,soff;
  ULONG bpl=i->getBytesPerLine();
  ULONG ps=bpl*i->getHeight();
  ULONG tile_byte_width=gwidth/8;
  ULONG tile_bytes_total=tile_byte_width*gheight;
  ULONG ramSize=r->getSize();
  ULONG raddr=0,waddr=0;
  DEBUG_OUT<<"tile_byte_width="<<tile_byte_width<<" tile_bytes_total="<<tile_bytes_total<<" bpl="<<bpl<<" ps="<<ps<<"\n";
  ULONG base=r->getLocation();
  for(unsigned int y=0;y<gdown;y++) {
    for(unsigned int x=0;x<gacross;x++) {
      offset=x*tile_byte_width+(y+startRow)*gheight*bpl;
      soff=off+x*tile_bytes_total+y*gheight*gacross;
      for(unsigned int t=0;t<tile_bytes_total;t++) {
        raddr=soff+t;
        waddr=offset+(bpl*(t%gheight))+t/gheight;
        if(raddr<ramSize) { bite=r->read8(base+raddr); }
        if(copyMode&EVIDEO_INVERSE) bite=~bite;
        if(waddr<ps) dest[waddr]=bite;
        //DEBUG_OUT<<"raddr="<<raddr<<" waddr="<<waddr<<" bite="<<bite<<"\n";
      }
      DEBUG_OUT<<"x="<<x<<" y="<<y<<" raddr="<<raddr<<" waddr="<<waddr<<"\n";
    }
  }
  //i->scale(scaleFactorX,scaleFactorY);
  i->touch();
}
*/


/*
// TODO: For now gwidth MUST be a multiple of 8
// TODO: For now depth MUST be 1
void EVideoIOAbstract::decodeMemTiles(APrivateBitmap *i, EMemAbstract *r, ULONG base,unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown,unsigned int copyMode, unsigned int startRow)
{
  if(!i) { DEBUG_OUT<<"No image to decode mapped rom into!\n"; return; }
  if(i->err.error()) { DEBUG_OUT<<"Image to decode mapped rom into has error!\n"; return; }
  if(!r) { DEBUG_OUT<<"No mmap to decode image from!\n"; return; }
  DEBUG_OUT<<"EVideoIOAbstract::decodeMemTiles "<<"gwidth="<<gwidth<<" gheight="<<gheight<<" gacross="<<gacross<<" gdown="<<gdown<<"\n";
  DEBUG_OUT<<"base="<<base<<" copyMode="<<copyMode<<" startRow="<<startRow<<"\n";
  DEBUG_OUT<<"Image info:";
  DEBUG_OUT<<" width="<<i->getWidth()<<" height="<<i->getHeight();
  DEBUG_OUT<<" depth="<<i->getDepth()<<" nplanes="<<i->getNPlanes()<<"\n";
  //i->verifyOkay(videow);
  unsigned char *dest=i->getPlane(0);
  unsigned char bite=0;
  ULONG offset=0,soff;
  ULONG bpl=i->getBytesPerLine();
  ULONG ps=bpl*i->getHeight();
  ULONG tile_byte_width=gwidth/8;
  ULONG tile_bytes_total=tile_byte_width*gheight;
  ULONG ramSize=r->getSize();
  ULONG raddr=0,waddr=0;
  ULONG off=0;
  DEBUG_OUT<<"tile_byte_width="<<tile_byte_width<<" tile_bytes_total="<<tile_bytes_total<<" bpl="<<bpl<<" ps="<<ps<<"\n";
  for(unsigned int y=0;y<gdown;y++) {
    for(unsigned int x=0;x<gacross;x++) {
      offset=x*tile_byte_width+(y+startRow)*gheight*bpl;
      soff=off+x*tile_bytes_total+y*gheight*gacross;
      for(unsigned int t=0;t<tile_bytes_total;t++) {
        raddr=soff+t;
        waddr=offset+(bpl*(t%gheight))+t/gheight;
        if(raddr<ramSize) { bite=r->read8(base+raddr); }
        if(copyMode&EVIDEO_INVERSE) bite=~bite;
        if(waddr<ps) dest[waddr]=bite;
        //DEBUG_OUT<<"raddr="<<raddr<<" waddr="<<waddr<<" bite="<<bite<<"\n";
      }
      DEBUG_OUT<<"x="<<x<<" y="<<y<<" raddr="<<raddr<<" waddr="<<waddr<<"\n";
    }
  }
  //i->scale(scaleFactorX,scaleFactorY);
  i->touch();
}
*/


/*
void EVideoIOAbstract::rotateTiles(APrivateBitmap *i,unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown,unsigned int copyMode)
{
  if(!i) { DEBUG_OUT<<"No image to rotate tiles!\n"; return; }
  i->convertEightBits();
  unsigned char *stile=(unsigned char *)malloc(gwidth*gheight);
  if(!stile) { DEBUG_OUT<<"Couldn't alloc stile!\n"; return; }
  unsigned char *dtile=(unsigned char *)malloc(gwidth*gheight);
  if(!dtile) { DEBUG_OUT<<"Couldn't alloc dtile!\n"; return; }
  DEBUG_OUT<<"Going to rotate tiles...\n";
  for(unsigned int ty=0;ty<gdown;ty++) {
    for(unsigned int tx=0;tx<gacross;tx++) {
      readTile(i,stile,tx*gwidth,ty*gheight,gwidth,gheight);
      rotateTile(stile,dtile,gwidth,gheight,copyMode);
      writeTile(i,dtile,tx*gwidth,ty*gheight,gwidth,gheight);
    }
  }
  free(stile);
  free(dtile);
  i->touch();
  i->verifyOkay(videow);
  DEBUG_OUT<<"Done rotating.\n";
}
*/


/*
void EVideoIOAbstract::flipTiles(APrivateBitmap *i, unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown, bool dx, bool dy,unsigned int copyMode)
{
  if(!i) { DEBUG_OUT<<"No image to flip tiles!\n"; return; }
  i->convertEightBits();
  unsigned char *stile=(unsigned char *)malloc(gwidth*gheight);
  if(!stile) { DEBUG_OUT<<"Couldn't alloc stile!\n"; return; }
  unsigned char *dtile=(unsigned char *)malloc(gwidth*gheight);
  if(!dtile) { DEBUG_OUT<<"Couldn't alloc dtile!\n"; return; }
  DEBUG_OUT<<"Going to flip tiles...\n";
  for(unsigned int ty=0;ty<gdown;ty++) {
    for(unsigned int tx=0;tx<gacross;tx++) {
      readTile(i,stile,tx*gwidth,ty*gheight,gwidth,gheight);
      flipTile(stile,dtile,dx,dy,gwidth,gheight,copyMode);
      writeTile(i,dtile,tx*gwidth,ty*gheight,gwidth,gheight);
    }
  }
  free(stile);
  free(dtile);
  i->touch();
  i->verifyOkay(videow);
  DEBUG_OUT<<"Done flipping.\n";
}
*/


/*
void EVideoIOAbstract::readTile(APrivateBitmap *i,unsigned char *tile, unsigned int x,unsigned int y,unsigned int gwidth,unsigned int gheight)
{
  if(!i) { DEBUG_OUT<<"No image to read tile!\n"; return; }
  if(!tile) { DEBUG_OUT<<"No tile to write into!\n"; return; }
  i->convertEightBits();
  unsigned char *bits=i->getPlane(0);
  ULONG bpl=i->getBytesPerLine();
  unsigned char c;
  for(unsigned int ty=0;ty<gheight;ty++) {
    for(unsigned int tx=0;tx<gwidth;tx++) {
      c=bits[(ty+y)*bpl+tx+x];
      tile[ty*gwidth+tx]=c;
    }
  }
}
*/


/*
void EVideoIOAbstract::writeTile(APrivateBitmap *i,unsigned char *tile, unsigned int x,unsigned int y,unsigned int gwidth,unsigned int gheight)
{
  if(!i) { DEBUG_OUT<<"No image to write tile!\n"; return; }
  if(!tile) { DEBUG_OUT<<"No tile to read from!\n"; return; }
  i->convertEightBits();
  unsigned char *bits=i->getPlane(0);
  ULONG bpl=i->getBytesPerLine();
  unsigned char c;
  for(unsigned int ty=0;ty<gheight;ty++) {
    for(unsigned int tx=0;tx<gwidth;tx++) {
      c=tile[ty*gwidth+tx];
      bits[(ty+y)*bpl+tx+x]=c;
    }
  }
}
*/


/*
void EVideoIOAbstract::rotateTile(unsigned char *stile,unsigned char *dtile, unsigned int gwidth, unsigned int gheight, unsigned int copyMode)
{
  if(!stile) { DEBUG_OUT<<"No stile!\n"; return; }
  if(!dtile) { DEBUG_OUT<<"No dtile!\n"; return; }
  // TODO: This only works on 8 bit chunky pixels!
  unsigned char c;
  for(unsigned int ty=0;ty<gheight;ty++) {
    for(unsigned int tx=0;tx<gwidth;tx++) {
      c=stile[ty*gwidth+tx];
      if(copyMode&EVIDEO_INVERSE) c=~c;
      dtile[tx*gheight+ty]=c;
    }
  }
}
*/


/*
void EVideoIOAbstract::flipTile(unsigned char *stile,unsigned char *dtile, bool dx, bool dy, unsigned int gwidth, unsigned int gheight, unsigned int copyMode)
{
  if(!stile) { DEBUG_OUT<<"No stile!\n"; return; }
  if(!dtile) { DEBUG_OUT<<"No dtile!\n"; return; }
  // TODO: This only works on 8 bit chunky pixels!
  // TODO: This only does dy flips because that's all I need right now...
  unsigned char c;
  for(unsigned int ty=0;ty<gheight;ty++) {
    for(unsigned int tx=0;tx<gwidth;tx++) {
      c=stile[ty*gwidth+tx];
      if(copyMode&EVIDEO_INVERSE) c=~c;
      dtile[(gheight-ty-1)*gwidth+tx]=c;
    }
  }
}
*/


bool EVideoIOAbstract::inVBlank()
{
  if(raster>vblankStart) return true; else return false;
}


bool EVideoIOAbstract::passedVBlank()
{
  // This isn't finished
  return inVBlank();
}


void EVideoIOAbstract::setRasterFactor(unsigned long val)
{
  if(!val) val=1;
  // Need to recalc everything...
  ULONG oldFactor=rasterFactor;
  rasterFactor=val;
  raster=(raster/oldFactor)*rasterFactor;
  maxRaster=(maxRaster/oldFactor)*rasterFactor;
  rasterInt=(rasterInt/oldFactor)*rasterFactor;
  vblankStart=(vblankStart/oldFactor)*rasterFactor;
}


void EVideoIOAbstract::setRasterSpeed(unsigned long val)
{
  rasterSpeed=val;
}


// NOTE: Handling of border as "extra" dimensions is very confusing and probably wrong in plances
// Also NOTE: size of border is probably wrong below...
void EVideoIOAbstract::drawBorder()
{
/*
  videow->sync();
  unsigned int nwidth=videow->getWidth()/scaleFactorX;
  nwidth-=(borderSize*2)/scaleFactorX;
  unsigned int nheight=videow->getHeight()/scaleFactorY;
  nheight-=(borderSize*2)/scaleFactorY;
  DEBUG_OUT<<"drawBorder "<<borderSize<<": "<<nwidth<<"x"<<nheight<<" scales: "<<scaleFactorX<<","<<scaleFactorY<<"\n";
  // NOTE: We draw wrong colors for now!!!
  videow->setForeground(borderColor);
  // top
  videow->moveTo(0,0);
  //DEBUG_OUT<<"Going to drawFilledRect...\n";
  videow->drawFilledRect((nwidth+borderSize)*scaleFactorX,borderSize*scaleFactorY);
  // left
  videow->moveTo(0,0);
  //DEBUG_OUT<<"Going to drawFilledRect...\n";
  videow->drawFilledRect(borderSize*scaleFactorX,(nheight+borderSize)*scaleFactorY);
  // bottom
  videow->moveTo(0,(nheight-borderSize)*scaleFactorY);
  //DEBUG_OUT<<"Going to drawFilledRect...\n";
  videow->drawFilledRect((nwidth+borderSize*2)*scaleFactorX,(nheight+borderSize*2)*scaleFactorY);
  // right
  videow->moveTo((nwidth-borderSize)*scaleFactorX,0);
  //DEBUG_OUT<<"Going to drawFilledRect...\n";
  videow->drawFilledRect((nwidth+borderSize*2)*scaleFactorX,(nheight+borderSize*2)*scaleFactorY);
  videow->setForeground(defaultForegroundColor);
*/
}


void EVideoIOAbstract::drawSprites()
{
  //DEBUG_OUT<<"d";
  for(unsigned int t=0;t<numSprites;t++) {
    bool doDraw=true;
    if(!sprites[t].active) doDraw=false;
    if((sprites[t].x<0)&&(sprites[t].y<0)) doDraw=false;
    unsigned int sx=sprites[t].sx;
    unsigned int sy=sprites[t].sy;
    unsigned int sw=sprites[t].width;
    unsigned int sh=sprites[t].height;
    unsigned int dx=(sprites[t].x+borderSize)*scaleFactorX;
    unsigned int dy=(sprites[t].y+borderSize)*scaleFactorY;
    //if(doDraw) videow->copyBlock(sprites[t].source,sx,sy,dx,dy,sw,sh);
  }
}


void EVideoIOAbstract::setSpriteActive(unsigned int num,bool val)
{
  if(num<numSprites) sprites[num].active=val;
  // If sprite has no size, set it to defaults...
  if(val) {
    if(!sprites[num].width) sprites[num].width=8;;
    if(!sprites[num].height) sprites[num].height=8;
  }
  drawSprites();
}


void EVideoIOAbstract::setSpriteRegisters(EMemIO *handler, ULONG addr)
{
  spriteRegisters=addr;
  spriteHandler=handler;
}


void EVideoIOAbstract::setNumSprites(unsigned int num)
{
  if(num<A_MAX_NUMBER_SPRITES) numSprites=num;
}


/*
void EVideoIOAbstract::setupSpriteGfx(APrivateBitmap *timage,unsigned int sgx,unsigned int sgy,unsigned int sgw,unsigned int sgh)
{
  if(!timage) { DEBUG_OUT<<"No image to setupSpriteGfx\n";  return; }
  if(timage->err.error()) { DEBUG_OUT<<"Image error in setupspriteGfx\n";  return; }
  DEBUG_OUT<<"going to setupSpriteGfx\n";
  if(!scaled) {
    scaled=true;
    timage->scale(scaleFactorX,scaleFactorY);
  }
  timage->update();
  timage->verifyOkay(videod);
  videow->matchColors(timage->getColors());
  spriteWidth=sgx;  spriteHeight=sgy;
  imageSpritesAcross=sgw;  imageSpritesDown=sgh;
  DEBUG_OUT<<"spriteGfx are "<<imageSpritesAcross<<" by "<<imageSpritesDown<<" size "<<spriteWidth<<" by "<<spriteHeight<<"\n";
  unsigned int x,y;
  for(y=0;y<sgh;y++) {
    for(x=0;x<sgw;x++) {
      unsigned int value=x+y*sgw;
      sprites[value].source=timage;
      sprites[value].width=spriteWidth;
      sprites[value].height=spriteHeight;
      sprites[value].sx=x*sgx*scaleFactorX;
      sprites[value].sy=y*sgy*scaleFactorY;
    }
  }
  DEBUG_OUT<<"done.\n";
}
*/


bool EVideoIOAbstract::reallyVideo()
{
  return true;
}


EVideoSprite *EVideoIOAbstract::getSprite(unsigned int num)
{
  EVideoSprite *ret=(EVideoSprite *)NULL;
  if(num<numSprites) ret=&(sprites[num]);
  return ret;
}


// NOTE: numBits is total bits RGB
bool EVideoIOAbstract::readRomPalette(char *fname,unsigned int numColors,unsigned int numBits)
{
  DEBUG_OUT<<"EVideoIO readRomPalette from "<<fname<<"\n";
  if(numBits!=8) {
    DEBUG_OUT<<"WARNING! readRomPalette not implemented for anything but 8 bits!\n";
  }
  BYTE *buf=(BYTE *)malloc(numColors);
  if(!buf) {
    DEBUG_OUT<<"Couldn't get buf!\n";
    return false;
  }
  FILE *f=fopen(fname,"rb");
  if(!f) {
    DEBUG_OUT<<"Couldn't open "<<fname<<"!\n";
    free(buf);
    return false;
  }
  fread(buf,numColors,1,f);
  // TODO: should test for a read error...
  fclose(f);
  //if(!colors) colors=new APalette(numColors);
  unsigned int ncolors=colors->getNumColors();
  bool ret=true;
  unsigned int bit0,bit1,bit2;
  unsigned int red,blue,green;
  for(unsigned int t=0;t<ncolors;t++) {
    bit0=(buf[t]>>0)&0x01;
    bit1=(buf[t]>>1)&0x01;
    bit2=(buf[t]>>2)&0x01;
    red=0x21*bit0+0x47*bit1+0x97*bit2;
    bit0=(buf[t]>>3)&0x01;
    bit1=(buf[t]>>4)&0x01;
    bit2=(buf[t]>>5)&0x01;
    green=0x21*bit0+0x47*bit1+0x97*bit2;
    bit0=0;
    bit1=(buf[t]>>6)&0x01;
    bit2=(buf[t]>>7)&0x01;
    blue=0x21*bit0+0x47*bit1+0x97*bit2;
    colors->setColor(t,red,green,blue);
  }
  free(buf);
  DEBUG_OUT<<"Done.\n";
  return ret;
}


void EVideoIOAbstract::setBitmapMode(bool really)
{
  bitmapMode=true;
  // TODO: change width/height to pixels
  if(really) {
    // TODO: dump cached gfx
    redraw();
  }
}


void EVideoIOAbstract::setMixedMode(bool really)
{
  setBitmapMode(really);
  // should be overridden so it can do more...
}


void EVideoIOAbstract::setTextMode(bool really)
{
  bitmapMode=false;
  // TODO: change width/height to chars
  if(really) {
    // TODO: create cached gfx
    redraw();
  }
}


int EVideoIOAbstract::convertToAscii(int c)
{
  int v=c&0x7f;
  if(v<32) v=v+64;
  if(v==127) v=32;
  if(c==160) v='.';
  return v;
}
