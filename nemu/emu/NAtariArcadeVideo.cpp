
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariArcadeHandler.h>
#include <emu/NAtariArcadeIO.h>
#include <emu/NAtariArcadeVideo.h>

#include <elib/ESoundIOPokey.h>
#include <elib/EKeys.h>
#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/APrivateBitmap.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em6502.h>


///////////////////////////////////////////////////////////////////////////////////
//  NAtariArcadeVideo Class
////////////////////////////////////////////////////////////////////////////////////

NAtariArcadeVideo::NAtariArcadeVideo(NAtariArcadeHandler *parent,unsigned int ncol,unsigned int nrow) : EVideoIOMultiMode(parent,ncol,nrow)
{
  DEBUG_OUT<<"NAtariArcadeVideo created.\n";
  redrawLock=true;
  //if(tileImage) delete tileImage;
  setNumSprites(16);
  /*
  tileImage=new APrivateBitmap(320,200,1);
  APalette *p=tileImage->getColors();
  p->setColor(0,0x00,0x00,0x00);
  p->setColor(1,0xff,0xff,0xff);
  */
  ERom *rom=(ERom *)handler->getMap()->findArea("centiped.212");
  if(rom) {
    /*
    decodeRomTiles(tileImage,rom,512,8,8,16,16);
    rotateTiles(tileImage,8,8,16,16,EVIDEO_ROTATE90);
    flipTiles(tileImage,8,8,16,16,false,true);
    setupSpriteGfx(tileImage,16,8,8,16);
    setupTileGfx(tileImage,8,8,16,16);
    */
  }
  else {
    err.set();
    DEBUG_OUT<<"Couldn't find gfx rom!!!\n";
    return;
  }
  setOffsets();
  setValid();
  if(type==N_ATARIARCADE_IO_MILIPEDE) setLocation(0x1000);
  redrawLock=false;
  setRasterFactor(30);
  setRasterInt(getVBlankStart(),E6502_INT_IRQ);
  for(unsigned int t=0;t<16;t++) {
    setSpriteActive(t,true);
    sprites[t].x=t*16; sprites[t].y=0;
    sprites[t].width=16;  sprites[t].height=8;
  }
}


NAtariArcadeVideo::~NAtariArcadeVideo()
{
}


void NAtariArcadeVideo::setValid()
{
  DEBUG_OUT<<"Using pede setValid...\n";
  if(!screenValidAddrs) { DEBUG_OUT<<"(screenValidAddrs was NULL, giving up!)\n"; return; }
  DEBUG_OUT<<"roundedSize is "<<roundedSize<<" and ramSize is "<<ramSize<<"\n";
  unsigned int t;
  for(t=0;t<0x400;t++) {
    screenValidAddrs[t]=1;
  }
  for(t=0;t<0x3bf;t++) {
    screenValidAddrs[t]=1;
  }
}


void NAtariArcadeVideo::setOffsets()
{
  DEBUG_OUT<<"Using pede setOffsets...\n";
  if(!screenRowAddrs) { DEBUG_OUT<<"(screenRowAddrs was NULL, giving up!)\n"; return; }
  if(!screenColAddrs) { DEBUG_OUT<<"(screenColAddrs was NULL, giving up!)\n"; return; }
  ramSize=roundedSize;
  DEBUG_OUT<<"roundedSize is "<<roundedSize<<" and ramSize is "<<ramSize<<"\n";
  DEBUG_OUT<<"tilesAcross is "<<tilesAcross<<" and tilesDown is "<<tilesDown<<"\n";
  unsigned int base,ad;
  for(base=0;base<roundedSize;base++) {
    screenRowAddrs[base]=0;
    screenColAddrs[base]=0;
  }
  for(unsigned int y=0;y<tilesDown;y++) {
    for(unsigned int x=0;x<tilesAcross;x++) {
      ad=y*tilesAcross+x;
      screenRowAddrs[ad]=(29-ad%tilesAcross)+1;
      screenColAddrs[ad]=(ad/tilesAcross)+1;
      write8(ad,x);
    }
  }
}

