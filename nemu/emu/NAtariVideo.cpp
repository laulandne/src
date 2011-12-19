
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariHandler.h>
#include <emu/NAtariIO.h>
#include <emu/NAtariVideo.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ESoundIOPokey.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOPia6520.h>

#include <cpulib/Em6502.h>


////////////////////////////////////////////////////////////////////////////////
//  NAtariVideo Class
////////////////////////////////////////////////////////////////////////////////

NAtariVideo::NAtariVideo(NAtariHandler *parent,unsigned int ncols,unsigned int nrows) : EVideoIOMultiMode(parent,ncols,nrows,4)
{
  DEBUG_OUT<<"NAtariVideo created.\n";
  if(type==N_ATARI_IO_ATARI5200) {
    //decodeMemTiles(tileImage,handler->getMap(),0xf800,8,8,16,8);
    //decodeMemTiles(tileImage,handler->getMap(),0xf800,8,8,16,8,EVIDEO_INVERSE,8);
  }
  else {
    //decodeMemTiles(tileImage,handler->getMap(),0xe000,8,8,16,8);
    //decodeMemTiles(tileImage,handler->getMap(),0xe000,8,8,16,8,EVIDEO_INVERSE,8);
  }
  //setupTileGfx(tileImage,8,8,16,16);
  setValid();
  setMaxRaster(130);  // NTSC
  if(!err.error()) {
    setOffsets();
    setValid();
    redrawLock=false;
    for(unsigned int t=0;t<8;t++) {
      setSpriteActive(t,false);
      sprites[t].x=t*16; sprites[t].y=0;
      sprites[t].width=32;  sprites[t].height=200;
    }
  }
}


NAtariVideo::~NAtariVideo()
{
  DEBUG_OUT<<"Destroing Natarivideo\n";
  //if(!error()) freeVideo();
  //err.set();
}

