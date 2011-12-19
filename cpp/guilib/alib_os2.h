#ifndef PLATFORM_ALIB_OS2_H
#define PLATFORM_ALIB_OS2_H

/*

    ALib Project - A Platform-Independent General Purpose Class Library
    Copyright (C) 2006 Nicholas Lauland

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
    
    This is for both 16 and 32-bit versions of OS/2

*/

#include <alib/atypedefs.h>
#include <alib/aplatform.h>

class AEvent;

#ifdef ASYS_OS2
//extern HINSTANCE alib_ahInst;
//extern HINSTANCE alib_ahPrevInst;
extern int alib_aNShow;
extern void *alib_aCmdLine;
extern MPARAM alib_aParam1,alib_aParam2;
extern HWND alib_ahWnd;
//extern HMENU alib_menuHandle;  // Won't always be legit! (see activateMenu)
extern unsigned long alib_lastModifier;  // if non-zero, modifier key held down
#endif // ASYS_OS2


#endif // PLATFORM_ALIB_OS2_H
