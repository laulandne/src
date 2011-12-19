
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ARequestAbstract.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>


#ifdef ASYS_WINDOWS
#include <commdlg.h>
#endif // ASYS_WINDOWS

#ifdef ASYS_WINDOWS
extern "C" LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT message,
  WPARAM wParam, LPARAM lParam);
#endif // ASYS_WINDOWS

#ifdef ASYS_OS2_32
MRESULT EXPENTRY aboutDialogFunc(HWND handle,ULONG mess,MPARAM parm1,
  MPARAM parm2);
#endif // ASYS_OS2_32
#ifdef ASYS_OS2_16
MRESULT EXPENTRY aboutDialogFunc(HWND handle,USHORT mess,MPARAM parm1,
  MPARAM parm2);
#endif // ASYS_OS2_16


#ifdef ASYS_MAC
#include <guilib/alib_carbon.h>
#if TARGET_API_MAC_CARBON
#ifndef _LATITUDE_
#define MAC_USE_NAVIGATION 1
#endif // _LATITUDE_
#endif // TARGET_API_MAC_CARBON
#ifdef ALIB_NO_PCH
#ifndef MAC_USE_NAVIGATION
#include <StandardFile.h>
#endif // MAC_USE_NAVIGATION
#endif // ALIB_NO_PCH
#endif // ASYS_MAC

#ifdef ASYS_AMIGA
#include <guilib/alib_amiga.h>
#include <exec/libraries.h>
#include <libraries/asl.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/asl_protos.h>
#include <clib/dos_protos.h>
#endif // ASYS_AMIGA


#ifdef ASYS_AMIGA
extern Library *AslBase;
#endif // ASYS_AMIGA


///////////////////////////////////////////////////////////////////////////////
//  ARequestAbstract Class
///////////////////////////////////////////////////////////////////////////////

ARequestAbstract::ARequestAbstract()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ARequestAbstract()\n";
#endif // DEBUG_VERBOSE
  init();
#ifdef ASYS_WIN16
#ifndef REALLY_OLD_WINDOWS
  commDlgHandle=(HINSTANCE)NULLHANDLE;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to load commdlg.dll...\n";
#endif // DEBUG_VERBOSE
  commDlgHandle=LoadLibrary("commdlg.dll");
  if((int)commDlgHandle<HINSTANCE_ERROR) {
    DEBUG_OUT<<"Couldn't load commdlg.dll!\n";
    return;
  }
/*
  // First dynamically resolve the functions we need
  if(!_dll_GetOpenFileName) {
    _dll_GetOpenFileName=(bool (pascal *)(OPENFILENAME FAR *))GET_PROC("COMMDLG","GETOPENFILENAME");
    if(!_dll_GetOpenFileName) {
      DEBUG_OUT<<"Couldn't getProc GetOpenFileName!\n";
    }
  }
*/
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Got lib and func.\n";
#endif // DEBUG_VERBOSE
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN16
}


ARequestAbstract::~ARequestAbstract()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"~ARequestAbstract()\n";
#endif // DEBUG_VERBOSE
#ifdef ASYS_WIN16
#ifndef REALLY_OLD_WINDOWS
  // This might not be right for Win32 where handles aren't ints...
  if((int)commDlgHandle>32) FreeLibrary(commDlgHandle);
  commDlgHandle=(HINSTANCE)NULLHANDLE;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN16
}
