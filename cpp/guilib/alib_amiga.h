#ifndef PLATFORM_ALIB_AMIGA_H
#define PLATFORM_ALIB_AMIGA_H

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


*/


#include <alib/atypedefs.h>
#include <alib/aplatform.h>


#ifdef ASYS_AMIGA
#define A_BIG_ENDIAN 1
#include <exec/libraries.h>
extern "C" {
extern Library *WorkbenchBase;
extern Library *AslBase;
extern Library *GfxBase;
extern Library *IntuitionBase;
};
#endif


#ifdef ASYS_AMIGA
/*
// for some reason my compiler is having trouble with this header...so we dup its defs here...
//#include <utility/tagitem.h>
#define TAG_DONE   (0L)
#define TAG_END       (0L)
#define TAG_USER   ((ULONG)(1L<<31))
typedef ULONG Tag;
struct TagItem
{
    Tag      ti_Tag;    // identifies the type of data
    ULONG ti_Data;    // type-specific data
};
*/
#endif // ASYS_AMIGA


#endif // PLATFORM_ALIB_AMIGA_H
