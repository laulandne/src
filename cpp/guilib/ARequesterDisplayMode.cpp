
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ARequesterDisplayMode.h>

#include <portable/nickcpp/AList.h>


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
//  ARequesterDisplayMode Class
///////////////////////////////////////////////////////////////////////////////

void ARequesterDisplayMode::init()
{
  ARequestAbstract::init();
  chosenMode=(ADisplayMode *)NULL;
}


ADisplayMode *ARequesterDisplayMode::getSelectedMode()
{
  return chosenMode;
}


char *ARequesterDisplayMode::doTheHardWork(char *name,char *msg)
{
  ADisplayMode *returnValue=(ADisplayMode *)NULL;
  char *chosenModeName=(char *)"Default Video Mode";
#ifdef ASYS_AMIGA
  // Earlier versions had ASL, but not the screen requester
  if(aThisApp.version>37) {
    if(AslBase) {
      struct TagItem mrtags[]={
        ASLSM_TitleText,(ULONG)"Select desired video mode",
        ASLSM_PositiveText,(ULONG)"Select",
        TAG_DONE
      };
      ScreenModeRequester *mr=(ScreenModeRequester *)
        AllocAslRequest(ASL_ScreenModeRequest,mrtags);
      if(mr) {
        bool ret=AslRequest(mr,NULL);
        ADatabase *l=aThisApp.dmList;
        if(ret&&!l->isEmpty()) {
          ADisplayMode *cur=NULL;
          for(l->reset();!l->atEnd();l->advance()) {
            cur=(ADisplayMode *)l->info();
            if(cur->mode==mr->sm_DisplayID) returnValue=cur;
          }
        }
        FreeAslRequest(mr);
      }
    }
  }
#endif // ASYS_AMIGA
  // Search through list, comparing current mode...return that node of list.
  chosenMode=returnValue;
  return chosenModeName;
}

