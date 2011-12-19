
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "ANativeFilesystem.h"


#ifdef ASYS_MAC
#ifndef __GNUC__
#define MAC_USE_MOREFILES 1
#include <IterateDirectory.h>
#endif // __GNUC__
#ifdef ALIB_NO_PCH
//#include <guilib/alib_carbon.h>
#endif // ALIB_NO_PCH
#endif // ASYS_MAC


#ifdef ASYS_DOS
#ifdef __SC__
#include <dos.h>
#endif
#endif // ASYS_DOS


#ifdef ASYS_WIN32
//#include <guilib/alib_win.h>
#endif // ASYS_WIN32


////////////////////////////////////////////////////////////////////////////////
//  ANativeFilesystem Class
////////////////////////////////////////////////////////////////////////////////


ANativeFilesystem::ANativeFilesystem()
{
  theDirents=NULL;
}
