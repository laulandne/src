
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


/*

    This is for both 16 and 32-bit versions of OS/2

*/


#ifdef ASYS_OS2
#include <alib/adebug.h>
#include <alib/aapp.h>
#include <alib/aversion.h>

#include <guilib/awindow.h>
#include <guilib/aevent.h>

#include <guilib/alib_os2.h>
#endif // ASYS_OS2


#ifdef ASYS_OS2
//HINSTANCE alib_ahInst=0;
//HINSTANCE alib_ahPrevInst=0;
int alib_aNShow=0;
void *alib_aCmdLine=NULL;
MPARAM alib_aParam1=0,alib_aParam2=0;
HWND alib_ahWnd=0;
//HMENU alib_menuHandle=0;  // Won't always be legit! (see activateMenu)
unsigned long alib_lastModifier;  // if non-zero, modifier key held down
#endif // ASYS_OS2


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


#ifdef ASYS_OS2
MRESULT EXPENTRY windowFunc(HWND handle, ULONG mess,
  MPARAM parm1, MPARAM parm2)
{
  // If we aren't ready...just let PM handle it.
  if(!alib_curEvent)
    return WinDefWindowProc(handle,mess,parm1,parm2);
  aThisApp.ahWnd=handle;
  HPS hps;
  unsigned short flags;
  unsigned long theCode;
  bool ignore=TRUE;
  switch(mess) {
    case WM_MOVE:
      alib_curEvent->type=A_EVENT_MOVED;
      aThisApp.sync=TRUE;
      break;
    case WM_SIZE:
      alib_curEvent->type=A_EVENT_RESIZE;
      aThisApp.sync=TRUE;
      break;
    case WM_ERASEBACKGROUND:
      // if we return TRUE, the system will clear the window
      alib_curEvent->type=A_EVENT_REDRAW;
      return (MRESULT)TRUE;
      //break;
    case WM_PAINT:
      hps=WinBeginPaint(handle,0,0);
      WinEndPaint(hps);
      alib_curEvent->type=A_EVENT_REDRAW;
      aThisApp.sync=TRUE;
      break;
    case WM_COMMAND:
      switch(SHORT1FROMMP(parm1)) {
        case 108:  // Exit
          alib_curEvent->type=A_EVENT_DESTROY;
          WinSendMsg(handle,WM_CLOSE,0L,0L);
          break;
        case 903:  // About ALib...
          // We'll handle this for the client.
          alib_curEvent->type=A_EVENT_NONE;
          //aThisApp.about();
          break;
        case 904:  // About...
          // We'll handle this for the client.
          alib_curEvent->type=A_EVENT_NONE;
          //aThisApp.about();
          break;
        default:
          // FIXME: "User" menus (anything but exit and about)
          // aren't implemented yet.
          //aThisApp.userMenu(0);
          break;
      }
      break;
    case WM_CHAR:
      alib_curEvent->type=A_EVENT_NONE;
      flags=SHORT1FROMMP(parm1);
      theCode=CHAR1FROMMP(parm2);
      ignore=FALSE;
      if(flags&KC_PREVDOWN) { /*dBug<<"(KC_PREVDOWN)";*/ ignore=TRUE; }
      if(!(flags&KC_KEYUP)) { /*dBug<<"(KC_KEYDOWN)";*/ ignore=TRUE; }
      if(!ignore) {
        //theCode=CHARMSG(&mess)->chr;
        if(theCode) {
          alib_curEvent->code=theCode;
          alib_curEvent->type=A_EVENT_KEYPRESS;
          //dBug<<"WM_CHAR: "<<(char)theCode<<" "<<theCode<<","<<flags;
          //dBug<<" ("<<parm1<<","<<parm2<<")\n";
        }
      }
      if(ignore) {
        //dBug<<": "<<(char)theCode<<" "<<theCode<<","<<flags;
        //dBug<<" ("<<parm1<<","<<parm2<<")\n";
      }
      return (MRESULT)TRUE;
      //break;
    case WM_BUTTON1UP:
      alib_curEvent->type=A_EVENT_MOUSEUP;  alib_curEvent->code=1;
      alib_curEvent->x=SHORT1FROMMP(parm1);
      alib_curEvent->y=SHORT2FROMMP(parm1);
      if(SHORT2FROMMP(parm2)&KC_SHIFT)
        alib_curEvent->modifier|=A_MOD_SHIFT;
      if(SHORT2FROMMP(parm2)&KC_CTRL)
        alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_BUTTON2UP:
      alib_curEvent->type=A_EVENT_MOUSEUP;  alib_curEvent->code=2;
      alib_curEvent->x=SHORT1FROMMP(parm1);
      alib_curEvent->y=SHORT2FROMMP(parm1);
      if(SHORT2FROMMP(parm2)&KC_SHIFT)
        alib_curEvent->modifier|=A_MOD_SHIFT;
      if(SHORT2FROMMP(parm2)&KC_CTRL)
        alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_BUTTON3UP:
      alib_curEvent->type=A_EVENT_MOUSEUP;  alib_curEvent->code=3;
      alib_curEvent->x=SHORT1FROMMP(parm1);
      alib_curEvent->y=SHORT2FROMMP(parm1);
      if(SHORT2FROMMP(parm2)&KC_SHIFT)
        alib_curEvent->modifier|=A_MOD_SHIFT;
      if(SHORT2FROMMP(parm2)&KC_CTRL)
        alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
     case WM_BUTTON1DOWN:
      alib_curEvent->type=A_EVENT_MOUSEDOWN;  alib_curEvent->code=1;
      alib_curEvent->x=SHORT1FROMMP(parm1);
      alib_curEvent->y=SHORT2FROMMP(parm1);
      if(SHORT2FROMMP(parm2)&KC_SHIFT)
        alib_curEvent->modifier|=A_MOD_SHIFT;
      if(SHORT2FROMMP(parm2)&KC_CTRL)
        alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_BUTTON2DOWN:
      alib_curEvent->type=A_EVENT_MOUSEDOWN;  alib_curEvent->code=2;
      alib_curEvent->x=SHORT1FROMMP(parm1);
      alib_curEvent->y=SHORT2FROMMP(parm1);
      if(SHORT2FROMMP(parm2)&KC_SHIFT)
        alib_curEvent->modifier|=A_MOD_SHIFT;
      if(SHORT2FROMMP(parm2)&KC_CTRL)
        alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_BUTTON3DOWN:
      alib_curEvent->type=A_EVENT_MOUSEDOWN;  alib_curEvent->code=3;
      alib_curEvent->x=SHORT1FROMMP(parm1);
      alib_curEvent->y=SHORT2FROMMP(parm1);
      if(SHORT2FROMMP(parm2)&KC_SHIFT)
        alib_curEvent->modifier|=A_MOD_SHIFT;
      if(SHORT2FROMMP(parm2)&KC_CTRL)
        alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
   case WM_DESTROY:
      alib_curEvent->type=A_EVENT_DESTROY;
      return WinDefWindowProc(handle,mess,parm1,parm2);
      //break;
    case WM_CLOSE:
      alib_curEvent->type=A_EVENT_DESTROY;
      return WinDefWindowProc(handle,mess,parm1,parm2);
      //break;
    default:
      // Let the system handle it.
      return WinDefWindowProc(handle,mess,parm1,parm2);
      //break;
  }
  return (MRESULT)FALSE;
}
#endif // AYS_OS2


#ifdef ASYS_OS2
#ifdef ASYS_OS2_32
MRESULT EXPENTRY aboutDialogFunc(HWND handle,ULONG mess,MPARAM parm1,
  MPARAM parm2)
#else
MRESULT EXPENTRY aboutDialogFunc(HWND handle,USHORT mess,MPARAM parm1,
  MPARAM parm2)
#endif // ASYS_OS2_32
{
  MRESULT res=(MRESULT)0;
  switch(mess) {
    case 1:
      WinDismissDlg(handle,1);
      res=(MRESULT)TRUE;
      break;
    default:
      res=WinDefDlgProc(handle,mess,parm1,parm2);
      break;
  }
  return res;
}
#endif // ASYS_OS2



#ifdef ASYS_OS2
void AApp::os2_start()
{
  dBug<<"os2_start()...\n";
  //newlineSize=2;
  // System init
  ahAB=WinInitialize(0);
  ahMQ=WinCreateMsgQueue(ahAB,0);
  versionMinor=_osminor;
  version=_osmajor/10;
#ifdef DEBUG_VERBOSE
  dBug<<"OS/2 version: "<<version<<"."<<versionMinor<<"\n";
#endif // DEBUG_VERBOSE
  mustFlipBitmaps=true;
}
#endif // ASYS_OS2

