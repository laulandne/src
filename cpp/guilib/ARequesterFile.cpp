
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ARequesterFile.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <guilib/AApp.h>


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
//  ARequesterFile Class
///////////////////////////////////////////////////////////////////////////////

// FIXME: Amiga and Mac don't handle files not in current dir well
// FIXME: Other systems have yet to be written...
char *ARequesterFile::doTheHardWork(char *name,char *msg)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ARequesterFile::doTheHardWork()...\n";
#endif // DEBUG_VERBOSE
  char *returnValue=(char *)NULL;
  bool done=false;
#ifdef ASYS_MAC
#ifdef MAC_USE_NAVIGATION
  DEBUG_OUT<<"MAC_USE_NAVIGATION is set\n";
  if(alib_navServices) {
    DEBUG_OUT<<"alib_navServices is set\n";
    OSErr theErr=noErr;
    NavDialogOptions options;
    memset((void *)&options,0,sizeof(NavDialogOptions));
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Going to NavGetDefaultDialogOptions...\n";
#endif // DEBUG_VERBOSE
    theErr=NavGetDefaultDialogOptions(&options);
    if(theErr!=noErr) {
      DEBUG_OUT<<"Got an error from NavGetDefaultDialogOptions!\n";
    }
    else {
      strcpy((char *)options.clientName,msg);
      options.preferenceKey=0;
      options.dialogOptionFlags+=kNavDontAutoTranslate;
      NavReplyRecord nreply;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Going to NavGetFile...\n";
#endif // DEBUG_VERBOSE
      theErr=NavGetFile((AEDesc *)NULL,&nreply,&options,(NavEventUPP)NULL,
        (NavPreviewUPP)NULL,(NavObjectFilterUPP)NULL,
        (NavTypeList **)NULL,(void *)NULL);
      if(theErr!=noErr) {
        DEBUG_OUT<<"Got an error from NavGetFile!\n";
      }
      if(nreply.validRecord) {
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"nreply was valid.\n";
#endif // DEBUG_VERBOSE
        macHandleAEDescList(this,nreply.selection);
        // TODO: We can get more than one...
        DEBUG_OUT<<"Back from macHandleAEDescList, here's what we got...\n";
        for(unsigned int q=0;q<argc;q++) DEBUG_OUT<<q<<" "<<argv[q]<<"\n";
        returnValue=argv[0];
        DEBUG_OUT<<"returnValue is "<<returnValue<<"\n";
      }
      else {
        DEBUG_OUT<<"nreply was not valid.\n";
      }
    }
    done=true;
  }
  else {
    DEBUG_OUT<<"alib_navServices is not set\n";
  }
#else
  DEBUG_OUT<<"MAC_USE_NAVIGATION is not set\n";
  SFReply reply;
  static Point where;
  static unsigned char filename[64];
  SFTypeList typeList;
  if(!done) {
    SFGetFile(where,NULL,
      (FileFilterUPP)NULL,-1,typeList,(DlgHookUPP)NULL,&reply);
    if(reply.good) {
      //HSetVol((char *)NULL,reply.vRefNum,0);
      SetVol((char *)NULL,reply.vRefNum);
      strcpy(filename,p2cstr(reply.fName));
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"ask_file returns: "<<filename<<"\n";
#endif // DEBUG_VERBOSE
      returnValue=filename;
      done=true;
    }
    else DEBUG_OUT<<"SFGetFile, reply.good, was bad!\n";
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"askFile's retval="<<aThisApp.myHex((unsigned long)returnValue)<<"\n";
#endif // DEBUG_VERBOSE
  }
#endif // MAC_USE_NAVIGATION
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  if(aThisApp.version>35) {
    done=true;
    if(AslBase) {
      struct TagItem frtags[]={
        ASLFR_TitleText,(ULONG)"Select a file to Open",
        ASLFR_PositiveText,(ULONG)"Open",
        TAG_DONE
      };
      FileRequester *fr=(FileRequester *)
        AllocAslRequest(ASL_FileRequest,frtags);
      if(fr) {
        bool ret=AslRequest(fr,NULL);
        // Memory leak here?
        if(ret) {
#ifdef DEBUG_VERBOSE
          DEBUG_OUT<<"Selected File: "<<(char *)fr->fr_File<<(ULONG)fr->fr_File
            <<"\n";
          DEBUG_OUT<<"Selected Drawer: "<<(char *)fr->fr_Drawer
            <<(ULONG)fr->fr_Drawer<<"\n";
#endif // DEBUG_VERBOSE
          returnValue=strdup(fr->rf_File);
          if(fr->fr_Drawer) {
            if(strlen(fr->fr_Drawer)) {
              BPTR theDir=Lock(fr->fr_Drawer,ACCESS_READ);
              BPTR oldDir;
              if(theDir) { oldDir=CurrentDir(theDir);  UnLock(theDir); }
              else DEBUG_OUT<<"Couldn't lock drawer!\n";
            } //strlen
            else DEBUG_OUT<<"Drawer is null...\n";
          } //fr->fr_
          else DEBUG_OUT<<"No drawer for askFile...\n";
        } //ret
        else DEBUG_OUT<<"askFile failed...\n";
        FreeAslRequest(fr);
      } // fr
    } //Asl
    else DEBUG_OUT<<"Didn't get ASL!\n";
  } //aThisApp
#endif // ASYS_AMIGA
#ifdef ASYS_WINDOWS
  done=true;
#ifndef REALLY_OLD_WINDOWS
  bool callGetOpenFileName=true;
#ifdef USE_DIRECTDRAW
  callGetOpenFileName=false;
#endif // USE_DIRECTDRAW
  OPENFILENAME ofn;
  ofn.lStructSize=sizeof(OPENFILENAME);
  ofn.hwndOwner=(HWND)NULLHANDLE;
  ofn.hInstance=(HINSTANCE)NULLHANDLE;
#ifdef UNICODE
  static wchar_t buffer[256];
  buffer[0]=0;
  ofn.lpstrFilter=(wchar_t *)NULL;
  ofn.lpstrFilter=L"All Files (*.*)\0*.*\0";
  ofn.lpstrInitialDir=(wchar_t *)NULL;
  ofn.lpstrTitle=(wchar_t *)NULL;
  ofn.lpstrInitialDir=(wchar_t *)NULL;
  ofn.lpstrTitle=(wchar_t *)NULL;
  ofn.lpstrFileTitle=(wchar_t *)NULL;
  ofn.lpTemplateName=(wchar_t *)NULL;
  ofn.lpstrDefExt=(wchar_t *)NULL;
  ofn.lpstrFile=buffer;
#else
  static char buffer[256];
  buffer[0]=0;
  ofn.lpstrFilter=(char *)NULL;
  ofn.lpstrFilter="All Files (*.*)\0*.*\0";
  ofn.lpstrInitialDir=(char *)NULL;
  ofn.lpstrTitle=(char *)NULL;
  ofn.lpstrInitialDir=(char *)NULL;
  ofn.lpstrTitle=(char *)NULL;
  ofn.lpstrFileTitle=(char *)NULL;
  ofn.lpTemplateName=(char *)NULL;
  ofn.lpstrDefExt=(char *)NULL;
  ofn.lpstrFile=buffer;
#endif // UNICODE
  ofn.lpstrCustomFilter=0;
  ofn.nMaxCustFilter=0;
  ofn.nFilterIndex=0;
  ofn.nMaxFile=255;
  ofn.nMaxFileTitle=0;
  ofn.Flags=OFN_NOVALIDATE;
#ifndef ASYS_WIN16
  if(aThisApp.version>3) ofn.Flags|=OFN_EXPLORER;
  if(aThisApp.version>3) ofn.Flags|=OFN_LONGNAMES;
#endif
  ofn.nFileOffset=0;
  ofn.nFileExtension=0;
  ofn.lCustData=0;
  ofn.lpfnHook=0;
//#ifdef ASYS_WIN16
  //if(_dll_GetOpenFileName) {
//#else
  if(callGetOpenFileName) {
//#endif // ASYS_WIN16
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"About to call GetOpenFileName...\n";
#endif // DEBUG_VERBOSE
    if(GetOpenFileName(&ofn)) {
#ifdef UNICODE
      returnValue=aThisApp.toNarrow(buffer);
#else
      returnValue=buffer;
#endif // UNICODE
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Open file: '"<<returnValue<<"'\n";
#endif // DEBUG_VERBOSE
    }
    else {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Open file: returned NULL.\n";
#endif // DEBUG_VERBOSE
      //DEBUG_OUT<<"The extended error is "<<CommDlgExtendedError()<<"\n";
    }
  }
//#ifdef ASYS_WIN16
  //else DEBUG_OUT<<"Couldn't find Common Dialog lib!\n";
//#endif // ASYS_WIN16
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2_32
  done=true;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Setting up for WinFileDlg...\n";
#endif // DEBUG_VERBOSE
  static char flub[CCHMAXPATH];
  char pszFullFile[CCHMAXPATH]="*.*";
  FILEDLG pfdFiledlg;
  memset(&pfdFiledlg,0,sizeof(FILEDLG));
  pfdFiledlg.cbSize=sizeof(FILEDLG);
  pfdFiledlg.fl=FDS_CENTER|FDS_OPEN_DIALOG;
  pfdFiledlg.pszTitle="Open File";
  strcpy(pfdFiledlg.szFullFile, pszFullFile);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to call WinFileDlg...\n";
#endif // DEBUG_VERBOSE
  // FIXME: For some reason this doesn't work with IBM's compiler...
  AWindow foo(10,10);
  HWND hwndDlg=WinFileDlg(HWND_DESKTOP,foo.getOS2Window(),&pfdFiledlg);
  if(hwndDlg&&(pfdFiledlg.lReturn==DID_OK)) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"WinFileDlg was happy.\n";
#endif // DEBUG_VERBOSE
    strcpy(flub,pfdFiledlg.szFullFile);
    returnValue=flub;
  }
  else {
    if(!hwndDlg) DEBUG_OUT<<"hwndDlg was NULL!\n";
    if(pfdFiledlg.lReturn!=DID_OK) DEBUG_OUT<<"lReturn was not DID_OK!\n";
    returnValue=(char *)NULL;
  }
#endif // ASYS_OS2_32
  if(!done) {
    fputs("\nPlease type the pathname of a file:\n",stdout);
    static char flub[1024];
    int t;
    for(t=0;t<1024;t++) flub[t]=0;
    char *tret=fgets(flub,1024,stdin);
    for(t=0;t<1024;t++) {
      if(flub[t]=='\n') flub[t]=0;
    }
    if(tret) returnValue=flub;
    else returnValue=NULL;
  }
  currentlyShown=false;
  return returnValue;
}


