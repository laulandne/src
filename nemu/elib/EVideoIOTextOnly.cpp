
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/APalette.h>

#include <portable/nickcpp/AStringHelper.h>

#include <elib/EVideoIOTextOnly.h>
#include <elib/EMemMapAbstract.h>
#include <elib/EHandler.h>
#include <elib/ERom.h>

#include <cpulib/ECPUAbstract.h>

//#include <guilib/AWindow.h>
//#include <guilib/ADisplay.h>
//#include <guilib/AEvent.h>
//#include <guilib/APrivateBitmap.h>
#include <portable/nickcpp/ANullStream.h>


#include <math.h>


//#define SHOW_VBLANK 1

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
//  EVideoIOTextOnly Class
////////////////////////////////////////////////////////////////////////////////

EVideoIOTextOnly::EVideoIOTextOnly(EHandler *parent,unsigned int ncol,unsigned int nrow, unsigned int depthHint) : EVideoIOAbstract(parent,ncol,nrow,depthHint)
{
  DEBUG_OUT<<"EVideoIOTextOnly::EVideoIOTextOnly...\n";
  theFontSize=tileHeight*scaleFactorY;
  if(theFontSize>24) theFontSize=24;  // For now...
  if(theFontSize<10) theFontSize=10;
  //theFontName=videow->queryFontAvail("courier",theFontSize);
  //videow->setFont(theFontName,theFontSize);
}


EVideoIOTextOnly::~EVideoIOTextOnly()
{
  DEBUG_OUT<<"Destroying EVideoIOTextOnly\n";
  //if(!err.error()) freeVideo();
  //err.set();
}


void EVideoIOTextOnly::init()
{
  EVideoIOAbstract::init();
}



BYTE EVideoIOTextOnly::write8(ULONG addr,BYTE value)
{
  unsigned int ea=addr-location;
  if(ea>memsize) return 0;
  if(addr>=spriteRegisters) // Gack!  Very C-64 specific!
  {
    if(spriteHandler) spriteHandler->write8(addr,value);
  }
  mem[ea]=value;
  drawChar(ea);
  return value;
}


void EVideoIOTextOnly::drawChar(ULONG ea)
{
  if(screenValidAddrs[ea]) {
    DEBUG_OUT<<ea<<"\n";
    char value=mem[ea];
    unsigned int col=screenColAddrs[ea]-1;
    unsigned int row=screenRowAddrs[ea]-1;
    unsigned int sx=tiles[value].sx;
    unsigned int sy=tiles[value].sy;
    unsigned int sw=tiles[value].width;
    unsigned int sh=tiles[value].height;
    unsigned int dx=(col*tileWidth+borderSize)*scaleFactorX;
    unsigned int dy=(row*tileHeight+borderSize)*scaleFactorY;
    //videow->moveTo(dx,dy);
    char c=convertToAscii(value);
    if(c==32) {
      // We handled space differently so we can correctly use it to erase non-prop fonts
      //videow->clear(dx,dy,tileWidth*scaleFactorX,tileHeight*scaleFactorX);
      //videow->setForeground(backgroundColor);
      //videow->drawFilledRect(tileWidth*scaleFactorX,tileHeight*scaleFactorX);
    }
    else {
      //videow->setForeground(defaultForegroundColor);
      //videow->drawText(&c,1,false);
    }
  }
  else DEBUG_OUT<<"invalid evideo write at "<<ea<<"\n";
}


void EVideoIOTextOnly::redraw()
{
  DEBUG_OUT<<"redraw() starting...\n";
  DEBUG_OUT<<"redraw() going to resize...\n";
  //videow->resize((emuWidth+(borderSize*2))*scaleFactorX,(emuHeight+(borderSize*2))*scaleFactorY);
  DEBUG_OUT<<"redraw() going to setForeground...\n";
  //videow->setForeground(defaultForegroundColor);
  DEBUG_OUT<<"redraw() going to setBackground...\n";
  //videow->setBackground(backgroundColor);
  DEBUG_OUT<<"redraw() going to clear...\n";
  //videow->clear();
  theFontSize=tileHeight*scaleFactorY;
  if(theFontSize>24) theFontSize=24;  // For now...
  if(theFontSize<10) theFontSize=10;
  DEBUG_OUT<<"redraw() going to queryFontAvail...\n";
  //theFontName=videow->queryFontAvail("courier",theFontSize);
  DEBUG_OUT<<"redraw() going to setFont...\n";
  //videow->setFont(theFontName,theFontSize);
  if(redrawLock) { DEBUG_OUT<<"(redrawLock was set)\n"; return; }
  DEBUG_OUT<<"memsize is "<<memsize<<"\n";
  for(unsigned int i=0;i<memsize;i++) {
    drawChar(i);
  }
  DEBUG_OUT<<"redraw() going to drawSprites...\n";
  drawSprites();
  DEBUG_OUT<<"redraw() going to drawBorder...\n";
  drawBorder();
  DEBUG_OUT<<"redraw() done.\n";
}
