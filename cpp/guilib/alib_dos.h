#ifndef PLATFORM_ALIB_DOS_H
#define PLATFORM_ALIB_DOS_H

/*

    ALib Project - A Platform-Independent General Purpose Class Library
    Copyright (C) 2007 Nicholas Lauland

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    -------------------------------------------------------------------


*/


#ifdef ASYS_DOS
#define RIN_W_AX rin.w.ax
#define RIN_W_BX rin.w.bx
#define RIN_W_CX rin.w.cx
#define RIN_W_DX rin.w.dx
#define ROUT_W_AX rin.w.ax
#define ROUT_W_BX rin.w.bx
#define ROUT_W_CX rin.w.cx
#define ROUT_W_DX rin.w.dx
#ifdef ASYS_DJGPP
#include <dos.h>
#include <conio.h>
#define REGS16 REGS
#define MK_FP(seg,ofs) ((((int)(seg))<<4)+((int)(ofs)))
#endif // ASYS_DJGPP
#ifdef __WATCOM_CPLUSPLUS__
#include <i86.h>
#include <conio.h>
#define REGS16 REGS
#define inportb(x) inp(x)
#define outportb(x,y) outp(x,y)
#ifdef ASYS_DOS32
#define int86(x,y,z) int386(x,y,z)
#endif // ASYS_DOS32
#endif // __WATCOM_CPLUSPLUS__
#ifdef __SC__
#include <dos.h>
#include <conio.h>
#define REGS16 REGS
#define inportb(x) inp(x)
#define outportb(x,y) outp(x,y)
#undef RIN_W_AX
#undef RIN_W_BX
#undef RIN_W_CX
#undef RIN_W_DX
#undef ROUT_W_AX
#undef ROUT_W_BX
#undef ROUT_W_CX
#undef ROUT_W_DX
#define RIN_W_AX rin.x.ax
#define RIN_W_BX rin.x.bx
#define RIN_W_CX rin.x.cx
#define RIN_W_DX rin.x.dx
#define ROUT_W_AX rout.x.ax
#define ROUT_W_BX rout.x.bx
#define ROUT_W_CX rout.x.cx
#define ROUT_W_DX rout.x.dx
#endif // __SC__
#endif // ASYS_DOS


#ifdef ASYS_DOS
bool dosMyKbhit();
#ifdef USE_JOYSTICK
unsigned short dosCenterJoy(unsigned short bit);
#endif // USE_JOYSTICK
unsigned char *mapVideoMem(unsigned char *ptr,unsigned long size=4096);
unsigned char *mapOtherMem(unsigned char *ptr,unsigned long size=4096);
void unmapVideoMem(unsigned char *ptr,unsigned long size=4096);
void unmapOtherMem(unsigned char *ptr,unsigned long size=4096);
#endif // ASYS_DOS


#ifdef ASYS_DOS
struct AVesaInfo {
  char sig[4];
  unsigned short version;
  unsigned short string_off,string_seg;
  unsigned char cap[4];
  unsigned short mode_off,mode_seg;
  unsigned short memory;
  // more private stuff here
  char dummy[242];
};
#endif // ASYS_DOS


#ifdef ASYS_DOS
struct AVesaModeInfo {
  // These are mandatory...
  unsigned short attrib;
  unsigned char wina,winb;
  unsigned short gran,size,aseg,bseg;
  unsigned short (*winfunc)();
  unsigned short bytesperline;
  // The rest are optional...
  unsigned short xres,yres;
  unsigned char xc,yc;
  unsigned char planecount,bitsperpixel;
  unsigned char banks,model,bsize,pages;
  // FIXME: Add vesa 2.0 stuff...
  // more private stuff here
  char dummy[227];
};
#endif // ASYS_DOS


#endif // PLATFORM_ALIB_DOS_H

