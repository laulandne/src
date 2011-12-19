
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAppleIO.h>
#include <emu/NAppleHandler.h>
#include <emu/NAppleVideo.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/APrivateBitmap.h>

#include <portable/ndecoder/AImageDecoder.h>
#include <portable/ndecoder/ASmartImageDecoder.h>

#include <cpulib/Em6502.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>
#include <elib/EIOVia6522.h>

#include <portable/nickcpp/AStringHelper.h>


unsigned int NApple_off[] = {
  0x400, 0x480, 0x500, 0x580, 0x600, 0x680, 0x700, 0x780,
  0x428, 0x4A8, 0x528, 0x5A8, 0x628, 0x6A8, 0x728, 0x7A8,
  0x450, 0x4D0, 0x550, 0x5D0, 0x650, 0x6D0, 0x750, 0x7d0
};


//////////////////////////////////////////////////////////////////////////////
//  NAppleVideo Class
//////////////////////////////////////////////////////////////////////////////

NAppleVideo::NAppleVideo(NAppleHandler *parent,unsigned int ncols,unsigned int nrows) : EVideoIOMultiMode(parent,ncols,nrows,4)
{
  DEBUG_OUT<<"NAppleVideo created.\n";
  // illegal values to force first set
  screenAddr=0xffff;  tileAddr=0xffff;  spriteAddr=0xffff;
  APrivateBitmap *timage=(APrivateBitmap *)NULL;
  DEBUG_OUT<<"Loading characters from pcx file...\n";
  AImageDecoder *dec=(AImageDecoder *)NULL;
  char *fname=(char *)NULL;
  switch(type) {
    case N_APPLE_IO_APPLE2E:
    case N_APPLE_IO_APPLE2GS:
      fname="apple2e.pcx";
      break;
    default:
      fname="apple2.pcx";
      break;
  }
  dec=ASmartImageDecoder::pickImageDecoder(fname);
  if(!dec) { aDefaultStringHelper.fatal("Couldn't load char image!\n"); exit(5); }
  if(dec->err.error()) { aDefaultStringHelper.fatal("Error loading char image!\n"); exit(5); }
  /*
  timage=dec->createNewBitmapObject();
  if(!timage) { aDefaultStringHelper.fatal("Couldn't create char image!\n"); exit(5); }
  if(timage->err.error()) { aDefaultStringHelper.fatal("Error creating char image!\n"); exit(5); }
  setupTileGfx(timage,8,8,16,16);
  */
  DEBUG_OUT<<"Done reading chars from pcx file.\n";
  setupVideoMode();
  setOffsets();
  setValid();
  setScreenChar(0x400,0);
  reset();
  redrawLock=false;
}


NAppleVideo::~NAppleVideo()
{
  DEBUG_OUT<<"Destroing Napplevideo\n";
  //if(!error()) freeVideo();
  //err.set();
}


void NAppleVideo::reset()
{
  EVideoIOAbstract::reset();
  EMemAbstract *c=getColorMem();
  unsigned int s=c->getSize();
  for(unsigned int t=0;t<s;t++) c->write8(t,0xf);
  setBackground(0);
  sText=false;  sPage2=false;
  sMixed=false;  sHires=false;
  s80col=false;
  newVideo=false;  bitmapMode=false;
}


void NAppleVideo::setSplitScreen(bool f)
{
  sMixed=f;
  setupVideoMode();
}


void NAppleVideo::setGfxHigh(bool f)
{
  sHires=f;
  setupVideoMode();
}


void NAppleVideo::setGfx(bool f)
{
  sText=f;
  setupVideoMode();
}


void NAppleVideo::setPage2(bool f)
{
  sPage2=f;
  setupVideoMode();
}


void NAppleVideo::setupVideoMode()
{
  int mode=0;
  if(sHires) mode+=2;
  if(sMixed) mode++;
  switch(mode) {
    case 0:
      DEBUG_OUT<<"(video is now fullscreen text)\n";
      setTextMode();
      break;
    case 1:
      DEBUG_OUT<<"(video is now splitscreen low-res)\n";
      setBitmapMode();
      break;
    case 2:
      DEBUG_OUT<<"(video is now fullscreen high-res)\n";
      setBitmapMode();
      break;
    case 3:
      DEBUG_OUT<<"(video is now splitscreen high-res)\n";
      setBitmapMode();
      break;
    default:
      DEBUG_OUT<<"(video is now unknown!!!)\n";
      break;
  }
}


void NAppleVideo::setValid()
{
  DEBUG_OUT<<"Using apple setValid...\n";
  if(!screenValidAddrs) { std::cerr<<"no screenValidAddrs!\n"; return; }
  DEBUG_OUT<<"roundedSize is "<<roundedSize<<" and ramSize is "<<ramSize<<"\n";
  unsigned int t;
  for(t=0;t<roundedSize;t++) {
    screenValidAddrs[t]=1;
  }
  // Correct extra bytes...
  for(t=0;t<8;t++) {
    screenValidAddrs[0x078+t]=0;  screenValidAddrs[0x0f8+t]=0;
    screenValidAddrs[0x178+t]=0;  screenValidAddrs[0x1f8+t]=0;
    screenValidAddrs[0x278+t]=0;  screenValidAddrs[0x2f8+t]=0;
    screenValidAddrs[0x378+t]=0;  screenValidAddrs[0x3f8+t]=0;
  }
}


void NAppleVideo::setOffsets()
{
  DEBUG_OUT<<"Using apple setOffsets...\n";
  if(!screenRowAddrs) { std::cerr<<"no screenRowAddrs!\n"; return; }
  if(!screenColAddrs) { std::cerr<<"no screenColAddrs!\n"; return; }
  ramSize=roundedSize;
  unsigned int base,ad;
  for(unsigned int y=0;y<tilesDown;y++) {
    base=NApple_off[y];
    //DEBUG_OUT<<y<<" base is "<<base<<"\n";
    for(unsigned int x=0;x<tilesAcross;x++) {
      ad=base+x;
      //DEBUG_OUT<<x<<","<<y<<" ad is "<<ad<<"\n";
      screenRowAddrs[ad-location]=(BYTE)(y+1);
      screenColAddrs[ad-location]=(BYTE)(x+1);
    }
  }
}


void NAppleVideo::my_vregs()
{
  if(sMixed) DEBUG_OUT<<"SplitScreen";  else DEBUG_OUT<<"FullScreen";
  DEBUG_OUT<<", ";
  if(sHires) DEBUG_OUT<<"sHires"; else DEBUG_OUT<<"sTextLow";
  DEBUG_OUT<<", ";
  if(s80col) DEBUG_OUT<<"80 column"; else DEBUG_OUT<<"40 column";
  DEBUG_OUT<<", ";
  if(newVideo) DEBUG_OUT<<"New Video"; else DEBUG_OUT<<"Standard Video";
  DEBUG_OUT<<"\n";
}


int NAppleVideo::convertToAscii(int c)
{
  char v=EVideoIOAbstract::convertToAscii(c);
  if(c==160) v=' ';
  return v;
}


BYTE NAppleVideo::write8(ULONG addr,BYTE value)
{
  return EVideoIOTextOnly::write8(addr,value);
}

