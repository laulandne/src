
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/APalette.h>

#include <portable/nickcpp/AStringHelper.h>

#include <elib/EVideoIOMultiMode.h>
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
//  EVideoIOMultiMode Class
////////////////////////////////////////////////////////////////////////////////

EVideoIOMultiMode::EVideoIOMultiMode(EHandler *parent,unsigned int ncol,unsigned int nrow, unsigned int depthHint) : EVideoIOTextOnly(parent,ncol,nrow,depthHint)
{
  DEBUG_OUT<<"EVideoIOMultiMode::EVideoIOMultiMode...\n";
}


EVideoIOMultiMode::~EVideoIOMultiMode()
{
  DEBUG_OUT<<"Destroying EVideoIOMultiMode\n";
  if(!err.error()) freeVideo();
  err.set();
}


void EVideoIOMultiMode::init()
{
  EVideoIOTextOnly::init();
}



