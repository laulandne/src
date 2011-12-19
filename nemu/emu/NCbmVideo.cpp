
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


//#define CHECK_C64_MAPPING 1


#include <emu/NCbmHandler.h>
#include <emu/NCbmIO.h>
#include <emu/NCbmVideo.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/AEvent.h>
//#include <guilib/APrivateBitmap.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EKeys.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>
#include <elib/EMemMapAbstract.h>

#include <cpulib/Em6502.h>


#define NCBM_WANT_KERN_TABLE
#include <emu/generated/NCbm_kern_tab.h>


#define HIGH_BYTECBM(x)  ((x&0xff00)>>8)
#define LOW_BYTECBM(x)  (x&0xff)


///////////////////////////////////////////////////////////////////////////////
//  NCbmVideo Class
///////////////////////////////////////////////////////////////////////////////

NCbmVideo::NCbmVideo(NCbmHandler *parent,unsigned int ncol,unsigned int nrow) : EVideoIOMultiMode(parent,ncol,nrow,4)
{
  DEBUG_OUT<<"NCbmVideo created.\n";
  // illegal values to force first set
  screenAddr=0xffff;  tileAddr=0xffff;  spriteAddr=0xffff;
  setBorder(16,0xffffff);  // initially a 16 bit white border for debugging
  unsigned int mappedCharAddr=0;
  unsigned int t;
  unsigned int sa=0,ca=0;
  multicolorMode=false;
  tileHeight=8;
  DEBUG_OUT<<"Checking cbm machine type...\n";
  switch(type) {
    case N_CBM_IO_C128:
    case N_CBM_IO_C65:
    case N_CBM_IO_C64:
      setBackground(0x000080);
      setDefaultForeground(0x8080ff);
      setNumSprites(8);
      for(t=0;t<16;t++) { sprites[t].width=24;sprites[t].height=21; }
      setColorMemAddr(0xd800);
      break;
    case N_CBM_IO_B:
      setBackground(0x000000);
      setDefaultForeground(0x00ff00);
      sa=0xfd000;
      setColorMemAddr(0xfd800);
      if(handler->getVariation()==1) {
        // 80 col
        ca=0;  tileHeight=16;
        tilesAcross=80;
        setMemSize(80,nrow,true);
      }
      else {
        setNumSprites(8);
        for(t=0;t<16;t++) { sprites[t].width=24;sprites[t].height=21; }
      }
      break;
    case N_CBM_IO_TED:
      setBackground(0x000000);
      setDefaultForeground(0x00ff00);
      sa=0x0c00;  ca=0;
      break;
    case N_CBM_IO_PET:
      setBackground(0xffffff);
      setDefaultForeground(0x000080);
      sa=0x8000;  ca=0;
      if(handler->getVariation()==1) {
        // 80 col
        tilesAcross=80;
        setMemSize(80,nrow,true);
      }
      break;
    case N_CBM_IO_VIC:
      setBackground(0xffffff);
      setDefaultForeground(0x0080);
      tilesAcross=22;  tilesDown=22;
      setMemSize(22,23,true);
      setColorMemAddr(0x9400);
      mappedCharAddr=0x8000;
      break;
    default:
      setColorMemAddr(0);
      break;
  }
  DEBUG_OUT<<"Done checking cbm machine type.\n";
  if(mappedCharAddr) {
    DEBUG_OUT<<"mappedCharAddr was set...\n";
    //decodeMemTiles(tileImage,handler->getMap(),mappedCharAddr,tileWidth,tileHeight,16,16);
    tileAddr=mappedCharAddr;
    //setupTileGfx(tileImage,tileWidth,tileHeight,16,16);
  }
  redrawLock=false;
  DEBUG_OUT<<"Going to setValid...\n";
  setValid();
  setMaxRaster(0x150);
  DEBUG_OUT<<"Going to setScreenChar...\n";
  setScreenChar(sa,ca);
  if(colors) {
    switch(type) {
      case N_CBM_IO_VIC:
        // TODO: Put real vic rgb values here...
        colors->setColor(0,0,0,0);
        colors->setColor(1,255,255,255);
        colors->setColor(2,220,60,84);
        colors->setColor(3,140,252,232);
        colors->setColor(4,220,0,220);
        colors->setColor(5,88,224,88);
        colors->setColor(6,92,32,204);
        colors->setColor(7,252,252,92);
        colors->setColor(8,216,112,56);
        colors->setColor(9,168,84,0);
        colors->setColor(10,244,96,112);
        colors->setColor(11,200,200,255);
        colors->setColor(12,164,160,160);
        colors->setColor(13,200,252,160);
        colors->setColor(14,116,136,236);
        colors->setColor(15,204,204,204);
        break;
      default:
        // NOTE: These are c-64 colors
        colors->setColor(0,0,0,0);
        colors->setColor(1,255,255,255);
        colors->setColor(2,220,60,84);
        colors->setColor(3,140,252,232);
        colors->setColor(4,220,0,220);
        colors->setColor(5,88,224,88);
        colors->setColor(6,92,32,204);
        colors->setColor(7,252,252,92);
        colors->setColor(8,216,112,56);
        colors->setColor(9,168,84,0);
        colors->setColor(10,244,96,112);
        colors->setColor(11,120,120,120);
        colors->setColor(12,164,160,160);
        colors->setColor(13,200,252,160);
        colors->setColor(14,116,136,236);
        colors->setColor(15,204,204,204);
        break;
    }
  }
  DEBUG_OUT<<"Done NCbmVideo::NCbmVideo\n";
}


NCbmVideo::~NCbmVideo()
{
  //if(!error()) freeVideo();
  //err.set();
}


void NCbmVideo::setScreenChar(unsigned int s,unsigned int c,bool force)
{
  DEBUG_OUT<<"NCbmVideo::setScreenChar("<<s<<","<<c<<")\n";
  if((s==screenAddr)&&(c==tileAddr)) { if(!force) return; } // already there...
  bool rd=force;
  if(s!=screenAddr) {
    DEBUG_OUT<<"screen addr changing to "<<s<<"\n";
    setLocation(s);
    screenAddr=s;
    rd=true;
  }
  if((c!=tileAddr)||force) {
    tileAddr=c;
    DEBUG_OUT<<"char addr changing to "<<tileAddr<<"\n";
    unsigned int romOffset=0;
    bool useRom=false;
    char *charRomName="char.rom";
    switch(type) {
      case N_CBM_IO_TED:
      case N_CBM_IO_PET:
      case N_CBM_IO_B:
        useRom=true;  romOffset=0;
        break;
      case N_CBM_IO_VIC:
        if(tileAddr==0x8000) { useRom=true; romOffset=0; }
        if(tileAddr==0x8800) { useRom=true; romOffset=2048; }
        break;
      case N_CBM_IO_C128:
      case N_CBM_IO_C65:
      case N_CBM_IO_C64:
        // TODO: let this be disabled on the c128 and c65 (right?)
        if(tileAddr==0x1000) { useRom=true; romOffset=0; }
        if(tileAddr==0x1800) { useRom=true; romOffset=2048; }
        if(tileAddr==0x9000) { useRom=true; romOffset=0; }
        if(tileAddr==0x9800) { useRom=true; romOffset=2048; }
        break;
      default:
        break;
    }
    /*
    if(tileImage) delete tileImage;
    tileImage=new APrivateBitmap(16*tileWidth*2,16*tileHeight*2,1);
    APalette *p=tileImage->getColors();
    if(p) {
      // TODO: Here's where the charactesr are set to b&w only
      p->setColor(0,backgroundColor);
      p->setColor(1,borderColor);
    }
    */
    if(useRom) {
      DEBUG_OUT<<"decoding video rom...\n";
      DEBUG_OUT<<"romOffset is "<<romOffset<<"\n";
      ERom *rom=(ERom *)handler->getMap()->findArea(charRomName);
      if(!rom) { DEBUG_OUT<<"Didn't find char.rom!\n"; exit(5); }
      //decodeRomTiles(tileImage,rom,romOffset,tileWidth,tileHeight,16,16);
    }
    else {
      DEBUG_OUT<<"decoding video address...\n";
      //io->getMap()->saveCurrentMap();
      //io->getMap()->setAllRam();
      //decodeMemTiles(tileImage,handler->getMap()->findArea("ram"),tileAddr,tileWidth,tileHeight,16,16);
      //io->getMap()->restoreSavedMap();
    }
    //setupTileGfx(tileImage,tileWidth,tileHeight,16,16);
    rd=true;
  }
  if(rd) redraw();
}


// NOTE: same as setScreenChar, but sprite pointers are handled too.
void NCbmVideo::setScreenCharSprites(unsigned int saddr,unsigned int caddr,bool force)
{
  if((saddr==screenAddr)&&(caddr==tileAddr)) { if(!force) return; }  // already there...
  setScreenChar(saddr,caddr,force);
  setSpriteRegisters(handler->getIO(),saddr+1016);
  unsigned int sp=saddr&0xc000;
  //if(sp==spriteAddr) return;  // already there...
  spriteAddr=sp;
  //DEBUG_OUT<<"decoding sprite address ("<<spriteAddr<<")...\n";
  //io->getMap()->saveCurrentMap();
  //io->getMap()->setAllRam();
  //decodeMemory(images,io->getMap(),spriteAddr,24,21,16,16);
  //io->getMap()->restoreSavedMap();
  //setupSpriteGfx(images,24,21,16,16);
}


void NCbmVideo::my_vregs()
{
  if(multicolorMode) DEBUG_OUT<<"Multicolor mode is on\n";
}


int NCbmVideo::convertToAscii(int c)
{
  return EVideoIOAbstract::convertToAscii(c);
}


void NCbmVideo::setVicDoubleHeightMode(bool on)
{
  unsigned int tsaddr=screenAddr,tcaddr=tileAddr;
  if(on) {
    //if(tileHeight==8) {
      // switch from 8x8 to 8x16
      DEBUG_OUT<<"Vic chip changing to 8x16 mode!\n";
      tileHeight=16;
      screenAddr=0; tileAddr=0;  // otherwise setScreenChar won't do anything
      setScreenChar(tsaddr,tcaddr);
    //}
    //else DEBUG_OUT<<"Vic chip already 8x16?\n";
  }
  else {
    //if(tileHeight==16) {
      // switch from 8x16 to 8x8
      DEBUG_OUT<<"Vic chip changing to 8x8 mode!\n";
      tileHeight=8;
      screenAddr=0; tileAddr=0;  // otherwise setScreenChar won't do anything
      setScreenChar(tsaddr,tcaddr);
    //}
    //else DEBUG_OUT<<"Vic chip already 8x8?\n";
  }
}


void NCbmVideo::setBitmapMode(bool on)
{
  if(on) {
    DEBUG_OUT<<"Video chip changing to bitmap mode!\n";
    bitmapMode=true;
    //TODO
  }
  else {
    DEBUG_OUT<<"Video chip changing to text mode!\n";
    bitmapMode=false;
    //TODO
  }
}


BYTE NCbmVideo::write8(ULONG addr,BYTE value)
{
  return EVideoIOTextOnly::write8(addr,value);
}
