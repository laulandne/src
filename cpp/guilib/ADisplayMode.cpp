
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ADisplayMode.h>
#include <guilib/AWindowAbstract.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/AList.h>


#ifdef ASYS_AMIGA
#include <guilib/alib_amiga.h>
#include <intuition/screens.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#endif // ASYS_AMIGA

#ifdef ASYS_DOS
#include <guilib/adisplay.h>
#include <guilib/alib_dos.h>
#endif // ASYS_DOS

#ifdef ASYS_WIN16
#ifndef DEVMODE
#define DEVMODE void
#endif // DEVMODE
#endif // ASYS_WIN16


#define DEBUG_VERBOSE 1


////////////////////////////////////////////////////////////////////////////////
//  Global junk
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_AMIGA
enum ADisplayAmigaVals {
  //
  A_VIDEO_OCS=2,A_VIDEO_ECS=3,A_VIDEO_AGA=4,A_VIDEO_ALIEN=5
};
#endif // ASYS_AMIGA


#ifdef ASYS_AMIGA
// These are the standard Amiga OCS modes
ADisplayMode aSomeModes[]={
  { LORES_KEY,320,200,5,5,5,40,0,0,A_VIDEO_OCS,4,A_BITMAP_PLANES,A_DISPLAYMODE_FLAG_LESS },
  { LORESLACE_KEY,320,400,5,5,5,40,0,0,A_VIDEO_OCS,4,A_BITMAP_PLANES,A_DISPLAYMODE_FLAG_LESS },
  { EXTRAHALFBRITE_KEY,320,200,6,6,6,40,0,0,A_VIDEO_OCS,4,A_BITMAP_PLANES,0 },
  { EXTRAHALFBRITELACE_KEY,320,400,6,6,6,40,0,0,A_VIDEO_OCS,4,A_BITMAP_PLANES,0 },
  { HAM_KEY,320,200,12,12,6,40,0,0,A_VIDEO_OCS,4,A_BITMAP_PLANES,0 },
  { HAMLACE_KEY,320,400,12,12,6,40,0,0,A_VIDEO_OCS,4,A_BITMAP_PLANES,0 },
  { HIRES_KEY,640,200,4,4,4,80,0,0,A_VIDEO_OCS,4,A_BITMAP_PLANES,A_DISPLAYMODE_FLAG_LESS },
  { HIRESLACE_KEY,640,400,4,4,4,80,0,0,A_VIDEO_OCS,4,A_BITMAP_PLANES,A_DISPLAYMODE_FLAG_LESS },
  { 0,0,0,0,0,0,0,0,0,0,0,0 }
};
#endif // ASYS_AMIGA


#ifdef ASYS_DOS
enum ADisplayDOSVals {
  A_FLAG_TEXT=64, // This is a text mode, not graphics
  A_FLAG_INTER=128, // Mode has lines interleaved (ala CGA)
  //
  A_VIDEO_MONO=2,A_VIDEO_HERC=3,A_VIDEO_CGA=4,
  A_VIDEO_EGA=5,A_VIDEO_MCGA=6,A_VIDEO_VGA=7,A_VIDEO_SVGA=8
};
#endif // ASYS_DOS


#ifdef ASYS_DOS
// These are the standard CGA/EGA/VGA modes
ADisplayMode aSomeModes[]={
  { 0x04,320,200,2,2,1,80,0xb8000000L,16000L,A_VIDEO_CGA,0,A_BITMAP_CHUNKY,A_FLAG_INTER },
  { 0x06,640,200,1,1,1,80,0xb8000000L,16000L,A_VIDEO_CGA,0,A_BITMAP_PLANES,A_FLAG_INTER },
  { 0x0d,320,200,4,4,4,40,0xa0000000L,8000L,A_VIDEO_EGA,2,A_BITMAP_PLANES,A_DISPLAYMODE_FLAG_LESS },
  { 0x0e,640,200,4,4,4,80,0xa0000000L,16000L,A_VIDEO_EGA,2,A_BITMAP_PLANES,A_DISPLAYMODE_FLAG_LESS },
  { 0x0f,640,350,1,1,1,80,0xa0000000L,28000L,A_VIDEO_EGA,2,A_BITMAP_PLANES,0 },
  { 0x10,640,350,4,4,4,80,0xa0000000L,28000L,A_VIDEO_EGA,2,A_BITMAP_PLANES,A_DISPLAYMODE_FLAG_LESS },
  { 0x11,640,480,1,1,1,80,0xa0000000L,38400L,A_VIDEO_MCGA,6,A_BITMAP_PLANES,0 },
  { 0x12,640,480,4,4,4,80,0xa0000000L,38400L,A_VIDEO_VGA,6,A_BITMAP_PLANES,A_DISPLAYMODE_FLAG_LESS },
  { 0x13,320,200,8,8,1,320,0xa0000000L,64000L,A_VIDEO_MCGA,6,A_BITMAP_CHUNKY,0 },
  { 0,0,0,0,0,0,0,0,0,0,0 }
};
#endif // ASYS_DOS


#ifdef ASYS_DOS
// These are the standard VESA SVGA modes
ADisplayMode aSomeVesaModes[]={
  { 0x100,640,400,8,8,1,0,0,0,0,0,0 },
  { 0x101,640,480,8,8,1,0,0,0,0,0,0 },
  { 0x102,800,600,4,4,4,0,0,0,0,0,0 },
  { 0x103,800,600,8,8,1,0,0,0,0,0,0 },
  { 0x104,1024,768,4,4,4,0,0,0,0,0,0 },
  { 0x105,1024,768,8,8,1,0,0,0,0,0,0 },
  { 0x106,1280,1024,4,4,4,0,0,0,0,0,0 },
  { 0x107,1280,1024,8,8,1,0,0,0,0,0,0 },
  { 0x10d,320,200,15,16,1,0,0,0,0,0,0 },
  { 0x10e,320,200,16,16,1,0,0,0,0,0,0 },
  { 0x10f,320,200,24,24,1,0,0,0,0,0,0 },
  { 0x110,640,480,15,16,1,0,0,0,0,0,0 },
  { 0x111,640,480,16,16,1,0,0,0,0,0,0 },
  { 0x112,640,480,24,24,1,0,0,0,0,0,0 },
  { 0x113,800,600,15,16,1,0,0,0,0,0,0 },
  { 0x114,800,600,16,16,1,0,0,0,0,0,0 },
  { 0x115,800,600,24,24,1,0,0,0,0,0,0 },
  { 0x116,1024,768,15,16,1,0,0,0,0,0,0 },
  { 0x117,1024,768,16,16,1,0,0,0,0,0,0 },
  { 0x118,1024,768,24,24,1,0,0,0,0,0,0 },
  { 0x119,1280,1024,15,16,1,0,0,0,0,0,0 },
  { 0x11a,1280,1024,16,16,1,0,0,0,0,0,0 },
  { 0x11b,1280,1024,24,24,1,0,0,0,0,0,0 },
  { 0x11c,1600,1200,15,16,1,0,0,0,0,0,0 },
  { 0x11d,1600,1200,16,16,1,0,0,0,0,0,0 },
  { 0x11e,1600,1200,24,24,1,0,0,0,0,0,0 },
  { 0,0,0,0,0,0,0,0,0,0,0 }
};
#endif // ASYS_DOS


