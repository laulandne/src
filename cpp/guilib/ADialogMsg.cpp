
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ADialogMsg.h>
#include <guilib/AEvent.h>
#include <guilib/AWindow.h>

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
//  ADialogMsg Class
///////////////////////////////////////////////////////////////////////////////

ADialogMsg::ADialogMsg(char *theName,char *theMsg)
{
  init();
  show(theName,theMsg);
}


char *ADialogMsg::doTheHardWork(char *name,char *msg)
{
  returnValue=(char *)"OK";
  if(!name) name=(char *)"Warning!";
  if(!msg) msg=name;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"AMsgWindow("<<name<<"): "<<msg<<"\n";
#endif // DEBUG_VERBOSE
  bool done=false;
  bool remind=false;
  bool waitAround=true;
  char *reminder=(char *)"Press return to continue";
#ifdef ASYS_PALM
  FrmCustomAlert(1000,msg,NULL,NULL);
  done=true;
#endif // ASYS_PALM
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  // FIXME: check to see if we've loaded the font image...if we
  // haven't, don't try opening a window to display the message...
#else
  // We ignore the returned value...
#ifdef UNICODE
  MessageBox((HWND)NULLHANDLE,aThisApp.toWide(msg),aThisApp.toWide2(name),MB_OK|MB_ICONEXCLAMATION);
#else
  MessageBox((HWND)NULLHANDLE,msg,name,MB_OK|MB_ICONEXCLAMATION);
#endif // UNICODE
  done=true;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  // We ignore the returned value...
  WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,(PSZ)msg,
    (PSZ)name,0,MB_OK|MB_ICONEXCLAMATION);
  done=true;
#endif // ASYS_OS2
#ifdef ASYS_MAC
#if TARGET_CARBON
  // TODO: we arbitrarily don't do this on classic so we don't require macos 8
#ifdef MAC_SYSTEM_8
#ifndef MAC_OLD_HEADERS
  // NOTE: This'll work under System 7.x if you have the Appearance extension
  if(alib_usingAppearance) {
    AlertStdAlertParamRec myAlertRec={
      false,
      false,
      (ModalFilterUPP)nil,
      (unsigned char *)-1,
      (unsigned char *)nil,
      (unsigned char *)nil,
      kAlertStdAlertOKButton,
      0,
      kWindowDefaultPosition
    };
    SInt16 ignoreHit;
    // If the name ends in a newline, remove it...
    unsigned int len=strlen(name);
    //if(name[len-1]=='\n') name[len-1]=0;    // TODO: Is the -1 right
    unsigned char namep[256];
    CopyCStringToPascal(name,namep);
    // If the message ends in a newline, remove it...
    len=strlen(msg);
    //if(msg[len-1]=='\n') msg[len-1]=0;   // TODO: Is the -1 right
    unsigned char msgp[256];
    CopyCStringToPascal(msg,msgp);
#if !__LP64__
    StandardAlert(kAlertCautionAlert,(unsigned char *)namep,(unsigned char *)msgp,&myAlertRec,&ignoreHit);
    done=true;
#endif
  }
#endif // MAC_OLD_HEADERS
#endif // MAC_SYSTEM_8
#endif // TARGET_CARBON
#endif // ASYS_MAC
  if(!done) {
    // Count how many lines, etc.
    //DEBUG_OUT<<"Msg is "<<awstrlen(msg)<<" characters.\n";
    unsigned int lines=countLinesInMsg(msg);
    unsigned int twidth=strlen(msg)*10;
    unsigned int theight=lines*12+30;
    AWindow w(name,twidth,theight);
    twidth=strlen(msg)*w.getAvgCharWidth()+6;
    theight=(lines+6)*w.getFontHeight();
    w.resize(twidth,theight);
    // TODO: if our window turns out big, pick a big font too.
    DEBUG_OUT<<"fontHeight is "<<w.getFontHeight()<<"\n";
    DEBUG_OUT<<"lines is "<<lines<<"\n";
    DEBUG_OUT<<"height is "<<w.getHeight()<<"\n";
    DEBUG_OUT<<"width is "<<w.getWidth()<<"\n";
    w.clear();
    unsigned int corr=1;
#ifdef ASYS_AMIGA
    corr+=2;
#endif // ASYS_AMIGA
    lines+=corr;
    w.center(msg,w.getHeight()/2-((lines*w.getFontHeight())/2));
    w.newline();
    if(remind) w.center(reminder);
    if(!waitAround) return returnValue;
    bool working=true;
    AEvent e;
    while(working) {
      //w.focus();
      e.wait(&w);
      switch(e.type) {
        case A_EVENT_MOUSEUP:
        case A_EVENT_KEYUP:
        case A_EVENT_DESTROY:
        case A_EVENT_BREAK:
          working=false;
          break;
        case A_EVENT_REDRAW:
        case A_EVENT_DISPLAY:
        case A_EVENT_MOVED:
        case A_EVENT_RESIZE:
          w.clear();
          w.center(msg,w.getHeight()/2-((lines*w.getFontHeight())/2));
          if(remind) w.center(reminder);
          break;
        default:
          //w.clear();
          w.center(msg,w.getHeight()/2-((lines*w.getFontHeight())/2));
          if(remind) w.center(reminder);
          break;
      }
    }
  }
  return returnValue;
}

