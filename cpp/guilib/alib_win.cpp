
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <guilib/alib_win.h>

#ifdef ASYS_WINDOWS
#include <iostream>

#include <guilib/AApp.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include <guilib/awindow.h>
#include <guilib/adisplay.h>
#include <guilib/aevent.h>
#endif // ASYS_WINDOWS


#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
#include <ddraw.h>
#endif // USE_DIRECTDRAW
#ifdef USE_DIRECTSOUND
#include <mmsystem.h>
#include <dsound.h>
#endif // USE_DIRECTSOUND
#ifdef USE_DIRECTINPUT
#include <dinput.h>
#endif // USE_DIRECTINPUT
#ifdef USE_DIRECT3D
#include <d3d.h>
//#include <d3d8.h>
//#include <d3d9.h>
#endif // USE_DIRECT3D
#endif // ASYS_WINDOWS

#ifdef ASYS_WINDOWS
#ifdef ASYS_WIN16
#ifndef REALLY_OLD_WINDOWS
// Only for on Win16 where we link run time...
HINSTANCE alib_mmSystemHandle=NULL;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN16
// The next three are as passed to WinMain()
HINSTANCE alib_ahInst=0;
HINSTANCE alib_ahPrevInst=0;
int alib_aNShow=0;
void *alib_aCmdLine=NULL;
WPARAM alib_awParam=0;   // Word param event loop return value
HWND alib_ahWnd=0;
HMENU alib_menuHandle=0;  // Won't always be legit! (see activateMenu)
unsigned long alib_lastModifier;  // if non-zero, modifier key held down
#ifdef ASYS_WIN32CE
#if CE_MAJOR_VER >2
SIPINFO alib_sinfo;
HINSTANCE alib_aygshellHandle;
#endif
#endif // ASYS_WIN32CE
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
//JOYCAPS alib_joyc;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN32CE
unsigned int alib_qtVer=0,alib_qtRev=0;
#ifdef ASYS_WIN16
#ifdef __SC__
extern "C" long _acrtused_winc;
long _acrtused_winc=0;
#endif
#endif
#endif // ASYS_WINDOWS


#ifdef ASYS_WINDOWS
extern "C" LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message,
  WPARAM wParam, LPARAM lParam);
extern "C" LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT message,
  WPARAM wParam, LPARAM lParam);
#endif // ASYS_WINDOWS

#ifdef ASYS_WINDOWS
bool alib_HandleExtraKeys(unsigned int val,AEventType eventType);
void alib_fixKeyCode();
void alib_about();
#endif // ASYS_WINDOWS


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_WINDOWS
#ifndef ALIB_LIBRARY
#ifdef UNICODE
//int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPWSTR lpCmdLine, int nCmdShow)
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
#else
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
#endif // UNICODE
{
  DEBUG_OUT<<"(WinMain)\n";
  alib_ahInst=hInstance;
  alib_ahPrevInst=hPrevInstance;
  alib_aNShow=nCmdShow;
  alib_aCmdLine=(void *)lpCmdLine;
  unsigned int thisArgc=1;
  char **thisArgv=(char **)&alib_aCmdLine;
  //alib_start(thisArgc,thisArgv);
  // Call client's main() (Whose name has been "mangled")
  //return (int)alib_myMain(alib_argc,alib_argv);
  //extern int __cdecl main(int argc,char **argv);
  return main(aThisApp.argc,aThisApp.argv);
}
#endif // ALIB_LIBRARY
#endif // ASYS_WINDOWS


#ifdef ASYS_WINDOWS
#ifndef ALIB_LIBRARY
#ifdef ALIB_DLL
int APIENTRY LibMain(HANDLE hdll,DWORD reason, LPVOID reserved)
{
  switch(reason) {
    case DLL_THREAD_ATTACH:
      // FIXME: thread init
      break;
    case DLL_THREAD_DETACH:
      // FIXME: thread cleanup
      break;
    case DLL_PROCESS_ATTACH:
      // FIXME: process init
      break;
    case DLL_PROCESS_DETACH:
      // FIXME: process cleanup
      break;
  }
  return 1;
}
#endif // ALIB_DLL
#endif // ALIB_LIBRARY
#endif // ASYS_WINDOWS


#ifdef ASYS_WIN32CE
#ifdef OLD_PEGASUS
bool GetVersionEx(OSVERSIONINFO *v)
{
  v->dwMajorVersion=1;
  v->dwMinorVersion=0;
  v->dwBuildNumber=0;
  v->dwPlatformId=VER_PLATFORM_WIN32_CE;
  int t;
  for(t=0;t<128;t++) v->szCSDVersion[t]=0;
  return true;
}
#endif // OLD_PEGASUS
#endif // ASYS_WIN32CE


#ifdef ASYS_WINDOWS
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message,
  WPARAM wParam, LPARAM lParam)
{
  // If we aren't ready...just let Windows handle it.
  if(!alib_curEvent) {
    //DEBUG_OUT<<"MainWndProc returning early, no alib_curEvent!\n";
    return DefWindowProc(hWnd,message,wParam,lParam);
  }
  alib_ahWnd=hWnd;
  RECT rect;
  HDC hdc,hdc2;
  PAINTSTRUCT ps;
  UINT theCode;
  switch(message) {
    //case WM_CREATE:
      //CREATESTRUCT *lpcs=(LPCREATESTRUCT)lParam;
      //SetWindowLong(hWnd,0,(long)lpcs->lpCreateParams);
      //break;
    case WM_MOVE:
      alib_curEvent->type=A_EVENT_MOVED;
      aThisApp.sync=TRUE;
      break;
    case WM_SIZE:
      alib_curEvent->type=A_EVENT_RESIZE;
      aThisApp.sync=TRUE;
      break;
#ifdef ASYS_WIN32CE
    //case WM_HIBERNATE:
      //break;
#endif // ASYS_WIN32CE
#ifdef ASYS_WIN32CE
    //case WM_HELP:
      //break;
#endif // ASYS_WIN32CE
    case WM_ERASEBKGND:
      hdc2=(HDC)wParam;
      //AWindow *that=(AWindow *)GetWindowLong(hWnd,0);
      //that->paintBackground(hdc2);
      alib_curEvent->type=A_EVENT_REDRAW;
      aThisApp.sync=TRUE;
      break;
    case WM_PAINT:
      if(GetUpdateRect(hWnd,&rect,FALSE)) {
        hdc=BeginPaint(hWnd,&ps);
        //AWindow *that=(AWindow *)GetWindowLong(hWnd,0);
        //that->paintBackground(hdc);
        EndPaint(hWnd,&ps);
      }
      alib_curEvent->type=A_EVENT_REDRAW;
      aThisApp.sync=TRUE;
      break;
    case WM_COMMAND:
      alib_curEvent->type=A_EVENT_MENU;
#ifdef ASYS_WIN32
      theCode=LOWORD(wParam);
#else
      theCode=wParam;
#endif // ASYS_WIN32
      switch(theCode) {
        case 102:  // Open
          alib_curEvent->type=A_EVENT_MENU;
          alib_curEvent->code=A_MENU_OPEN;
          break;
        case 108:  // Exit
          alib_curEvent->type=A_EVENT_DESTROY;
          PostQuitMessage(0);
          break;
        case 903:  // About Alib...
          // We'll handle this for the client.
          alib_curEvent->type=A_EVENT_NONE;
          alib_about();
          break;
        case 904:  // About...
          // We'll handle this for the client.
          alib_curEvent->type=A_EVENT_NONE;
          alib_about();
          break;
        default:
          // FIXME: "User" menus (anything but exit and about)
          // aren't implemented yet.
          //alib_userMenu(0);
          break;
      }
      break;
    //case WM_CHAR:
      // FIXME: Can't set modifier here since TranslateMessage()
      // has already used that data to get us the right keycode
      //alib_curEvent->type=A_EVENT_KEYUP;
      //alib_curEvent->code=(char)wParam;
      //alib_curEvent->modifier=0;
      //break;
    case WM_KEYUP:
      if(!alib_HandleExtraKeys(wParam,A_EVENT_KEYUP)) {
        alib_curEvent->type=A_EVENT_KEYUP;
        alib_curEvent->code=(char)wParam;
        alib_curEvent->modifier=0;
        alib_fixKeyCode();
        //return DefWindowProc(hWnd,message,wParam,lParam);
      }
      break;
    case WM_MOUSEMOVE:
      alib_curEvent->type=A_EVENT_MOUSEMOVE;  alib_curEvent->code=1;
      alib_curEvent->x=LOWORD(lParam);
      alib_curEvent->y=HIWORD(lParam);
      if(wParam&MK_SHIFT) alib_curEvent->modifier|=A_MOD_SHIFT;
      if(wParam&MK_CONTROL) alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_KEYDOWN:
      if(!alib_HandleExtraKeys(wParam,A_EVENT_KEYDOWN)) {
        alib_curEvent->type=A_EVENT_KEYDOWN;
        alib_curEvent->code=(char)wParam;
        alib_curEvent->modifier=0;
        alib_fixKeyCode();
        //return DefWindowProc(hWnd,message,wParam,lParam);
      }
      break;
    case WM_LBUTTONUP:
      alib_curEvent->type=A_EVENT_MOUSEUP;  alib_curEvent->code=1;
      alib_curEvent->x=LOWORD(lParam);
      alib_curEvent->y=HIWORD(lParam);
      if(wParam&MK_SHIFT) alib_curEvent->modifier|=A_MOD_SHIFT;
      if(wParam&MK_CONTROL) alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_LBUTTONDOWN:
      alib_curEvent->type=A_EVENT_MOUSEDOWN;  alib_curEvent->code=1;
      alib_curEvent->x=LOWORD(lParam);
      alib_curEvent->y=HIWORD(lParam);
      if(wParam&MK_SHIFT) alib_curEvent->modifier|=A_MOD_SHIFT;
      if(wParam&MK_CONTROL) alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
#ifndef ASYS_WIN32CE
    case WM_MBUTTONUP:
      alib_curEvent->type=A_EVENT_MOUSEUP;  alib_curEvent->code=2;
      alib_curEvent->x=LOWORD(lParam);
      alib_curEvent->y=HIWORD(lParam);
      if(wParam&MK_SHIFT) alib_curEvent->modifier|=A_MOD_SHIFT;
      if(wParam&MK_CONTROL) alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_RBUTTONUP:
      alib_curEvent->type=A_EVENT_MOUSEUP;  alib_curEvent->code=3;
      alib_curEvent->x=LOWORD(lParam);
      alib_curEvent->y=HIWORD(lParam);
      if(wParam&MK_SHIFT) alib_curEvent->modifier|=A_MOD_SHIFT;
      if(wParam&MK_CONTROL) alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_MBUTTONDOWN:
      alib_curEvent->type=A_EVENT_MOUSEDOWN;  alib_curEvent->code=2;
      alib_curEvent->x=LOWORD(lParam);
      alib_curEvent->y=HIWORD(lParam);
      if(wParam&MK_SHIFT) alib_curEvent->modifier|=A_MOD_SHIFT;
      if(wParam&MK_CONTROL) alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
    case WM_RBUTTONDOWN:
      alib_curEvent->type=A_EVENT_MOUSEDOWN;  alib_curEvent->code=3;
      alib_curEvent->x=LOWORD(lParam);
      alib_curEvent->y=HIWORD(lParam);
      if(wParam&MK_SHIFT) alib_curEvent->modifier|=A_MOD_SHIFT;
      if(wParam&MK_CONTROL) alib_curEvent->modifier|=A_MOD_CONTROL;
      break;
#endif // ASYS_WIN32CE
    case WM_DESTROY:
      // FIXME: Do we HAVE to call PostQuitMessage?
      alib_curEvent->type=A_EVENT_DESTROY;
      PostQuitMessage(0);
      break;
#ifndef ASYS_WIN16
#ifndef ASYS_WIN386
    case WM_SETTINGCHANGE:
      alib_curEvent->type=A_EVENT_RESIZE;
#ifdef ASYS_WIN32CE
#if CE_MAJOR_VER >2
      if(alib_aygshellHandle) {
        alib_sinfo.cbSize=sizeof(SIPINFO);
        SHSipInfo(SPI_GETSIPINFO,lParam,&alib_sinfo,0);
      }
#endif
#endif // ASYS_WIN32CE
      break;
#endif // ASYS_WIN386
#endif // ASYS_WIN16
    default:
      return DefWindowProc(hWnd,message,wParam,lParam);
      //break;
  }
  return (LRESULT)NULL;
}
#endif // ASYS_WINDOWS


#ifdef ASYS_WINDOWS
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT message,
  WPARAM wParam, LPARAM lParam)
{
  bool retVal=FALSE;
  WORD id;
  switch(message) {
    case WM_INITDIALOG:
      retVal=TRUE;
      break;
    case WM_COMMAND:
#ifdef ASYS_WIN32
      id=LOWORD(wParam);
#else
      id=(WORD)wParam;
#endif // ASYS_WIN32
      if((id==IDOK)||(id==IDCANCEL)) {
        EndDialog(hDlg,TRUE); retVal=FALSE;
      }
      break;
    default:
      // Is this right?
      //return DefDlgProc(hDlg,message,wParam,lParam);
      break;
  }
  return retVal;
}
#endif // ASYS_WINDOWS


#ifdef ASYS_WINDOWS
void alib_fixKeyCode()
{
  // fix non-alpha, non-numeric, key codes in our event...
#ifdef ASYS_WIN32CE
  DEBUG_OUT<<"fixKeyCode: "<<alib_curEvent->code<<" ";
  DEBUG_OUT<<alib_curEvent->modifier<<" ";
  DEBUG_OUT<<alib_lastModifier;
  DEBUG_OUT<<"\n";
#endif // ASYS_WIN32CE
  unsigned long newCode=alib_curEvent->code&0xff;
  if(alib_lastModifier==A_MOD_SHIFT) {
    if((newCode<58)&&(newCode>47)) {
      // shifted number keys,etc
      //DEBUG_OUT<<"(highcode shifted)\n";
      newCode-=16;
      switch(newCode) {
        case '\"': newCode='@'; break;
        case '&': newCode='^'; break;
        case '\'': newCode='&'; break;
        case '(': newCode='*'; break;
        case ')': newCode='('; break;
        case ' ': newCode=')'; break;
        default: break;
      }
    }
    if(newCode>127) {
      newCode-=128;
      switch(newCode) {
        case '=': newCode='_'; break;
        case ';': newCode='+'; break;
        case '@': newCode='~'; break;
        case '[': newCode='{'; break;
        case ']': newCode='}'; break;
        case '^': newCode='\"'; break;
        case '\\': newCode='|'; break;
        default: break;
      }
    }
  }
  else {
    if((newCode>='A')&&(newCode<='Z')) newCode+=32;
    if(newCode>127) {
      //DEBUG_OUT<<"(highcode unshifted)\n";
      newCode-=128+16;
      switch(newCode) {
        case '+': newCode='='; break;
        case '*': newCode=';'; break;
        case 'N': newCode='\''; break;
        case 'K': newCode='['; break;
        case 'M': newCode=']'; break;
        case '0': newCode='`'; break;
        case 'L': newCode='\\'; break;
        default: break;
      }
    }
  }
  alib_curEvent->code=newCode;
#ifdef ASYS_WIN32CE
  DEBUG_OUT<<"Code now: "<<newCode<<"\n";
#endif // ASYS_WIN32CE
}
#endif // ASYS_WINDOWS


#ifdef ASYS_WINDOWS
bool alib_HandleExtraKeys(unsigned int val,AEventType eventType)
{
  bool ret=TRUE;
  bool joyNotKeys=false;
  unsigned int whichJoy=0;
  switch(val) {
    case 38:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_UP;
      break;
    case 37:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_LEFT;
      break;
    case 40:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_DOWN;
      break;
    case 39:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_RIGHT;
      break;
    case 19:
      alib_curEvent->type=A_EVENT_BREAK;
      break;
    case 112:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F1;
      break;
    case 113:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F2;
      break;
    case 114:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F3;
      break;
    case 115:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F4;
      break;
    case 116:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F5;
      break;
    case 117:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F6;
      break;
    case 118:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F7;
      break;
    case 119:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F8;
      break;
    case 120:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F9;
      break;
    case 121:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_F10;
      break;
    case 16:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_LSHIFT;
          if(eventType==A_EVENT_KEYDOWN) alib_lastModifier=A_MOD_SHIFT;
          else alib_lastModifier=A_MOD_NONE;
          DEBUG_OUT<<"shift key "<<(int)eventType<<"\n";
      break;
    case 17:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_LCONTROL;
          if(eventType==A_EVENT_KEYDOWN) alib_lastModifier=A_MOD_CONTROL;
          else alib_lastModifier=A_MOD_NONE;
          DEBUG_OUT<<"control key "<<(int)eventType<<"\n";
      break;
    case 45:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_INSERT;
      break;
    case 36:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_HOME;
      break;
    case 35:  // end
    case 33:  // page up
      if(joyNotKeys) { joyNotKeys=FALSE; DEBUG_OUT<<"joystick keys off\n"; }
      else { joyNotKeys=TRUE; DEBUG_OUT<<"joystick keys on\n"; }
      break;
    case 34:  // page down
      if(whichJoy) { whichJoy=0; DEBUG_OUT<<"keys are joystick #1\n"; }
      else { whichJoy=1; DEBUG_OUT<<"keys are joystick #2\n"; }
      break;
    case 93:
      alib_curEvent->type=eventType;
      alib_curEvent->code=A_KEY_LLOGO;
      break;
    default:
#ifdef ASYS_WIN32CE
      //DEBUG_OUT<<"Extra key not handled: "<<val<<"\n";
#endif // ASYS_WIN32CE
      ret=FALSE;
      break;
  }
  return ret;
}
#endif // ASYS_WINDOWS


#ifdef ASYS_WINDOWS
void alib_about()
{
  bool done=false;
#ifdef ASYS_WIN16
  FARPROC lpProcAbout=(FARPROC)
    MakeProcInstance((FARPROC)AboutDlgProc,alib_ahInst);
  DialogBox(alib_ahInst,"ALibAbout",alib_ahWnd,lpProcAbout);
  FreeProcInstance(lpProcAbout);
  done=true;
#endif // ASYS_WIN16
#ifdef ASYS_WIN32
#ifndef ASYS_WIN32CE
  DialogBox(alib_ahInst,ATEXT("ALibAbout"),alib_ahWnd,(DLGPROC)AboutDlgProc);
  done=true;
#endif // ASYS_WIN32CE
#endif // ASYS_WIN32
  if(!done) {
/*
    AWindow tw(ATEXT("About..."),300,150);
    bool working=true;
    tw.moveTo(0,50).center("ALib Libraries");
    tw.moveTo(0,70).center("Version");
    //tw.moveTo(0,85).center(ALIB_VERSION);
    tw.moveTo(0,100).center("By Nicholas Lauland");
    AEvent e;
    while(working) {
      //w.focus();
      e.wait(&tw);
      if(e.type==A_EVENT_DESTROY) working=false;
    };
*/
  }
}
#endif // ASYS_WINDOWS


////////////////////////////////////////////////////////////////////////////////
// Some fake stdio/stdlib func for windows ce
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_WIN32CE
int myChdir(char *dir)
{
  // Don't do anything since we don't have directories, per se
  return 0;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
char *myGetcwd(char *dir,unsigned int len)
{
  if(!dir) return NULL;
  // We just return / so they get something that looks ok
  dir[0]='/';
  dir[1]=0;
  return dir;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
int myFstat(int fd,struct stat *s)
{
  if(!s) return -1;
  DEBUG_OUT.warning("Need to implement alib_ce fake fstat!\n");
  return -1;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
int myStat(char *filename,struct stat *s)
{
  if(!filename) return -1;
  if(!s) return -1;
  DEBUG_OUT.warning("Need to implement alib_ce fake stat!\n");
  return -1;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
bool myIsdigit(char c)
{
        switch(c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
                return true;
        default:
                break;
        }
        return false;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
FILE *myFopen(const char *n, const char *m)
{
  if(!n) return NULL;
  if(!m) return NULL;
  UINT mode=0,mode2;
  if(m[0]=='r') { mode=GENERIC_READ; mode2=OPEN_EXISTING; }
  if(m[0]=='w') { mode=GENERIC_WRITE; mode2=CREATE_ALWAYS; }
  if(!mode) return NULL;
  HANDLE hFile=CreateFile(aThisApp.toWide((char *)n),mode,0,NULL,mode2,FILE_ATTRIBUTE_NORMAL,NULL);
  if(hFile==INVALID_HANDLE_VALUE) return NULL;
  return (FILE *)hFile;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
int myFclose(FILE *f)
{
  if(!f) return 0;
  if(f==INVALID_HANDLE_VALUE) return 0;
  CloseHandle((HANDLE)f);
  return 0;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
size_t myFread(void *p, size_t s, size_t n, FILE *f)
{
  if(!f) return 0;
  if(f==INVALID_HANDLE_VALUE) return 0;
  if(!p) return 0;
  DWORD nBytesRead=0;
  BOOL bResult=ReadFile((HANDLE)f,p,s*n,&nBytesRead,NULL);
  return nBytesRead;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
size_t myFwrite(const void *p, size_t s, size_t n, FILE *f)
{
  if(!f) return 0;
  if(f==INVALID_HANDLE_VALUE) return 0;
  if(!p) return 0;
  DWORD nBytesRead=0;
  BOOL bResult=WriteFile((HANDLE)f,p,s*n,&nBytesRead,NULL);
  return nBytesRead;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
int myFflush(FILE *f)
{
  if(!f) return 0;
  if(f==INVALID_HANDLE_VALUE) return 0;
  return 0;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
int myFseek(FILE *f, long n, int w)
{
  if(!f) return 0;
  if(f==INVALID_HANDLE_VALUE) return 0;
  UINT ww=0;
  if(w==SEEK_SET) ww=FILE_BEGIN;
  if(w==SEEK_CUR) ww=FILE_CURRENT;
  if(w==SEEK_END) ww=FILE_END;
  return SetFilePointer((HANDLE)f,n,NULL,ww);
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
long  myFtell(FILE *f)
{
  if(!f) return 0;
  if(f==INVALID_HANDLE_VALUE) return 0;
  return SetFilePointer((HANDLE)f,0,NULL,FILE_CURRENT);
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
int myFputc(int c, FILE *f)
{
  if(!f) return 0;
  if(f==INVALID_HANDLE_VALUE) return 0;
  char t=c;
  myFwrite(&t,1,1,f);
  return 1;
}
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
int myFgetc(FILE *f)
{
  if(!f) return 0;
  if(f==INVALID_HANDLE_VALUE) return 0;
  char t;
  myFread(&t,1,1,f);
  return 1;
}
#endif // ASYS_WIN32CE


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


#ifdef ASYS_WINDOWS
void AApp::win_start()
{
  DEBUG_OUT<<"win_start()...\n";
  //newlineSize=2;
#ifdef ASYS_WIN16
  DWORD dwVersion=GetVersion();
  version=LOBYTE(LOWORD(dwVersion));
  versionMinor=HIBYTE(LOWORD(dwVersion));
  int dversion=HIBYTE(HIWORD(dwVersion));
  int dversionMinor=LOBYTE(HIWORD(dwVersion));
  DEBUG_OUT<<"DOS Version: "<<dversion<<"."<<dversionMinor<<"\n";
  DEBUG_OUT<<"Windows claims version "<<version<<"."<<versionMinor<<"\n";
  if(version==3) {
    // Because Windows '95 says its Windows version 3.95...
    if(versionMinor>90) { version=4;  versionMinor=0; }
  }
#else
  UINT platformID=VER_PLATFORM_WIN32_WINDOWS;
#ifdef OLD_WINDOWS
  // FIXME: Win32 ver 3.1 function here...
  version=3;  versionMinor=1;
#else
  OSVERSIONINFO osVer;
  osVer.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
  GetVersionEx(&osVer);
  version=osVer.dwMajorVersion;
  versionMinor=osVer.dwMinorVersion;
  platformID=osVer.dwPlatformId;
#endif // OLD_WINDOWS
  bool doesUnicode=false;
  switch(platformID) {
    case VER_PLATFORM_WIN32s:
      DEBUG_OUT<<"Running on Windows 3.1/Win32s";
      version=3;  versionMinor=1; break;
    case VER_PLATFORM_WIN32_WINDOWS:
      DEBUG_OUT<<"Running on Windows 95/98/ME";
      break;
    case VER_PLATFORM_WIN32_NT:
      DEBUG_OUT<<"Running on Windows NT/2k/XP/Vista";
      doesUnicode=true; break;
    case VER_PLATFORM_WIN32_CE:
      DEBUG_OUT<<"Running on Windows CE"; doesUnicode=true;
      break;
    default:
      DEBUG_OUT<<"Running on an unknown Windows platform";
      break;
  }
#ifndef OLD_WINDOWS
  DEBUG_OUT<<"  build: "<<(int)(osVer.dwBuildNumber&0xffff)<<" ("<<osVer.szCSDVersion<<")";
#endif // OLD_WINDOWS
  DEBUG_OUT<<"\n";
#ifdef UNICODE
  if(!doesUnicode) {
    MessageBox((HWND)NULLHANDLE,
      L"This application requires Unicode, which this operating system doesn't fully support!",
        L"Warning!",MB_OK|MB_ICONEXCLAMATION);
  }
#endif // UNICODE
#endif // ASYS_WIN16
  DEBUG_OUT<<"Windows Version: "<<((long)version)<<"."<<((long)versionMinor)<<"\n";
  //DEBUG_OUT<<"-----------------------\n";
#ifdef USE_DIRECTDRAW
  DEBUG_OUT<<"Using DirectDraw version "<<(DIRECTDRAW_VERSION>>8)<<"."<<(DIRECTDRAW_VERSION&0xff)<<"\n";
#endif // USE_DIRECTDRAW
#ifdef USE_DIRECT3D
#ifndef DIRECT3D_VERSION
  // assume at least version 7 arbitrarily
#define DIRECT3D_VERSION 0x700
#endif // DIRECT3D_VERSION
  DEBUG_OUT<<"Using Direct3D version "<<(DIRECT3D_VERSION>>8)<<"."<<(DIRECT3D_VERSION&0xff)<<"\n";
#endif // USE_DIRECT3D
#ifdef USE_DIRECTSOUND
  DEBUG_OUT<<"Using DirectSound version "<<(DIRECTSOUND_VERSION>>8)<<"."<<(DIRECTSOUND_VERSION&0xff)<<"\n";
#endif // USE_DIRECTSOUND
#ifdef USE_DIRECTINPUT
  DEBUG_OUT<<"Using DirectInput...version "<<(DIRECTINPUT_VERSION>>8)<<"."<<(DIRECTINPUT_VERSION&0xff)<<"\n";
#endif // USE_DIRECTINPUT
#ifdef USE_QUICKTIME
  DEBUG_OUT<<"Using QuickTime if present.\n";
#endif // USE_QUICKTIME
#ifdef USE_OPENGL
  DEBUG_OUT<<"Using OpenGL for 3D.\n";
#endif
  // Init system
  if(!alib_ahPrevInst) {
    WNDCLASS wc;
    wc.style=0;
#ifndef ASYS_WIN32CE
    //wc.style=CS_SAVEBITS;
    wc.style=CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
#endif // ASYS_WIN32CE
#ifdef REALLY_OLD_WINDOWS
    wc.lpfnWndProc=(LONG (FAR PASCAL *)())MainWndProc;
#else
    wc.lpfnWndProc=(WNDPROC)MainWndProc;
#endif // REALLY_OLD_WINDOWS
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    //wc.cbClsExtra=sizeof(AWindow *);  // Save room to store pointer
    //wc.cbWndExtra=sizeof(AWindow *);  // Save room to store pointer
    wc.hInstance=alib_ahInst;
    wc.hIcon=LoadIcon(alib_ahInst,MAKEINTRESOURCE(1));
#ifdef UNICODE
    wc.lpszMenuName=(wchar_t *)NULL;
#else
    wc.lpszMenuName=(char *)NULL;
#endif // UNICODE
    wc.hCursor=(HICON)NULLHANDLE;
#ifndef ASYS_WIN32CE
    wc.lpszMenuName=ATEXT("ALibMenu");
    wc.hCursor=LoadCursor((HINSTANCE)NULLHANDLE,IDC_ARROW);
    wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
#else
    wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
#endif // ASYS_WIN32CE
#ifdef UNICODE
    wc.lpszClassName=L"ALibWClass";
#else
    wc.lpszClassName="ALibWClass";
#endif // UNICODE
    if(!RegisterClass((WNDCLASS *)&wc)) {
      DEBUG_OUT<<"Couldn't register window class!\n";
      exit(5);
    }
  }
  else DEBUG_OUT<<"(There was an alib_ahPrevInst)\n";
#ifdef USE_DIRECTDRAW
  DEBUG_OUT<<"Going to open defaultDisplay...\n";
  initDefaultDisplay();
#endif // USE_DIRECTDRAW
  alib_qtVer=0;  alib_qtRev=0;
#ifdef USE_QUICKTIME
  DEBUG_OUT<<"Going to InitializeQTML...\n";
  OSErr err=InitializeQTML(0);
  if(err) {
    DEBUG_OUT<<"(Didn't find QuickTime!)\n";
  }
  else {
    long response;
    if(!Gestalt(gestaltQuickTimeVersion,&response)) {
      qtVer=(response>>24)&0xff;  qtRev=(response>>20)&0xf;
      DEBUG_OUT<<"QuickTime "<<qtVer<<"."<<qtRev<<" present.\n";
    }
  }
#endif // USE_QUICKTIME
}
#endif // ASYS_WINDOWS


#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
// NOTE: We create an HWND that all the AWindows will use instead
// of opening their own.  Thus, if you use DirectDraw, you're fullscreen
// with only one "real" window...
bool ADisplay::initDirectDraw(ADisplayMode *tryMode)
{
  bool ret=true;
  width=tryMode->width;
  height=tryMode->height;
  depth=tryMode->depth;
  DEBUG_OUT<<"Going to initDirectDraw...\n";
  if(aThisApp.defaultDisplay) {
    if(!areDefault) {
      DEBUG_OUT<<"Copying defaultDisplay's surfaces, etc\n";
      ddPtr=aThisApp.defaultDisplay->getDdPtr();
      ddrawWin=aThisApp.defaultDisplay->getDdrawWin();
      offscreenPtr=aThisApp.defaultDisplay->getOffscreenPtr();
      surfPtr=aThisApp.defaultDisplay->getSurfPtr();
      winPalette=aThisApp.defaultDisplay->getWinPalette();
      ddrawBits=aThisApp.defaultDisplay->getDdrawBits();
      canPageFlip=aThisApp.defaultDisplay->getCanPageFlip();
      didInitDDraw=true;
      return true;
    }
    else DEBUG_OUT<<"Wow!  We're the defaultDisplay!\n";
  }
  // First get the DirectDraw object...
  DirectDrawCreate(0,&ddPtr,0);
  if(!ddPtr) {
    DEBUG_OUT.warning("Couldn't open DirectDraw!\n");
    return false;
  }
  changedMode=true;  // to be safe, for now...
  // Now, open the window...
  DEBUG_OUT<<"Going to open the "<<width<<"x"<<height<<" ddrawWin...\n";
  DWORD wstyle=WS_OVERLAPPEDWINDOW;
  ddrawWin=CreateWindow(ATEXT("ALibWClass"),name,
    wstyle,0,0,width,height,(HWND)NULLHANDLE,
    (HMENU)NULLHANDLE,alib_ahInst,(LPSTR)NULL);
  if(ddrawWin==(HWND)NULLHANDLE) {
    errorFlag=true; DEBUG_OUT<<"Couldn't CreateWindow!\n"; return false;
  }
  SetWindowPos(ddrawWin,(HWND)NULLHANDLE,0,0,width,height,SWP_DRAWFRAME);
  ShowWindow(ddrawWin,SW_SHOW);
  UpdateWindow(ddrawWin);
  // Set coop level (fullscreen or whatever)...
  DEBUG_OUT<<"Going to SetCooperativeLevel...\n";
  //ULONG coopFlags=DDSCL_NORMAL;
  ULONG coopFlags=DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN|DDSCL_MULTITHREADED|DDSCL_NOWINDOWCHANGES;
  HRESULT retVal=ddPtr->SetCooperativeLevel(ddrawWin,coopFlags);
  if(retVal!=DD_OK) {
    errorFlag=true; DEBUG_OUT<<"Couldn't SetCooperativeLevel!\n"; return false;
  }
  // Set the video mode to what we want...
  DEBUG_OUT<<"Going to SetDisplayMode("<<width<<","<<height<<","<<depth<<")...\n";
  retVal=ddPtr->SetDisplayMode(width,height,depth);
  if(retVal!=DD_OK) {
    DEBUG_OUT<<"First try for SetDisplay mode didn't work.\n";
    DEBUG_OUT<<"Tweaking width/height/depth to find something that works.\n";
    if(depth<8) depth=8;
    if(depth==24) depth=32;
    if(width<640) width=640;
    if(height<480) height=480;
    DEBUG_OUT<<"Going to SetDisplayMode("<<width<<","<<height<<","<<depth<<")...\n";
    retVal=ddPtr->SetDisplayMode(width,height,depth);
  }
  if(retVal!=DD_OK) {
    errorFlag=true; DEBUG_OUT<<"Couldn't SetDisplayMode!\n"; return false;
  }
  changedMode=true;
  // Match our vals to what we actually got...
  sync();
  // Now create the palette...
  if((!staticColor)&&(!trueColor)) {
    unsigned int num=0;
    if(colors) num=colors->getNumColors();
    else DEBUG_OUT<<"DirectDraw expected display palette, but didn't find it!\n";
    unsigned int palFlags=DDPCAPS_1BIT;
    unsigned int closestColors=2;
    if(num>2) { closestColors=4; palFlags=DDPCAPS_2BIT; }
    if(num>4) { closestColors=16; palFlags=DDPCAPS_4BIT; }
    if(num>16) { closestColors=256; palFlags=DDPCAPS_8BIT; }
    if(num>256) {
      DEBUG_OUT.warning("DirectDraw with more than 256 indexed colors!\n");
    }
    DEBUG_OUT<<"numColors is "<<num<<" closestColors is "<<closestColors<<"\n";
    PALETTEENTRY *cols=(PALETTEENTRY *)malloc(
      closestColors*sizeof(PALETTEENTRY));
    if(colors) {
      for(unsigned int t=0;t<closestColors;t++) {
        cols[t].peRed=colors->red(t);
        cols[t].peGreen=colors->green(t);
        cols[t].peBlue=colors->blue(t);
        cols[t].peFlags=PC_RESERVED;
      }
    }
    if(!trueColor) {
      DEBUG_OUT<<"Going to try to create DDraw palette...\n";
      retVal=ddPtr->CreatePalette(palFlags,cols,&winPalette,NULL);
      //DEBUG_OUT<<"after create palette.\n";
    }
    else DEBUG_OUT.warning("DirectDraw trueColor palette!\n");
    if(retVal==DD_OK) {
      DEBUG_OUT<<"got the palette.\n";
    }
    else {
      DEBUG_OUT<<"failed to create palette...\n";
      winPalette=NULL;
    }
    free(cols);
    cols=NULL;
  }
  // Now create the surfaces...
  numPages=1;
  DEBUG_OUT<<"Going to create 1st surface...trying 2 back buffers...\n";
  canPageFlip=false;
  DDSURFACEDESC sDesc;
  memset(&sDesc,0,sizeof(DDSURFACEDESC));
  // We ask for the best first...2 back buffers of real video mem...
  sDesc.dwSize=sizeof(DDSURFACEDESC);
  sDesc.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
  sDesc.dwBackBufferCount=2;
  sDesc.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|
    DDSCAPS_COMPLEX|DDSCAPS_VIDEOMEMORY;
  DEBUG_OUT<<"(CreateSurface)\n";
  retVal=ddPtr->CreateSurface(&sDesc,&surfPtr,NULL);
  DEBUG_OUT<<"(ok)\n";
  if(retVal==DD_OK) {
    DEBUG_OUT<<"(got it)\n";
    canPageFlip=true;
	numPages=3;
  }
  if(retVal!=DD_OK) {
    // didn't get it, let's scale back...try for one back buffer...
    DEBUG_OUT<<"Didn't get 2 back buffers, trying just 1...\n";
    sDesc.dwBackBufferCount=1;
    DEBUG_OUT<<"(CreateSurface)\n";
    retVal=ddPtr->CreateSurface(&sDesc,&surfPtr,NULL);
    DEBUG_OUT<<"(ok)\n";
    if(retVal==DD_OK) {
      DEBUG_OUT<<"(got it)\n";
      canPageFlip=true;
	  numPages=2;
    }
  }
  if(retVal!=DD_OK) {
    // didn't get either 1 or 2 back buffers...try a simple surface
    DEBUG_OUT<<"Couldn't get 1 either, trying a simple surface...\n";
    sDesc.dwFlags=DDSD_CAPS;
    sDesc.dwBackBufferCount=0;
    sDesc.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
    DEBUG_OUT<<"(CreateSurface)\n";
    retVal=ddPtr->CreateSurface(&sDesc,&surfPtr,NULL);
    DEBUG_OUT<<"(ok)\n";
    if(retVal==DD_OK) {
      DEBUG_OUT<<"(got it)\n";
	  numPages=1;
    }
  }
  if(retVal!=DD_OK) DEBUG_OUT.warning("DirectDraw couldn't get 1st surface!\n");
  DEBUG_OUT<<"Got first surface.\n";
  // Now get the other surface...
  DEBUG_OUT<<"Going to create 2nd surface...\n";
  if(canPageFlip) {
    // we got a real back buffer, grab it...
    DEBUG_OUT<<"Trying to get attached surface...\n";
    DDSCAPS theCaps;
    theCaps.dwCaps=DDSCAPS_BACKBUFFER;
    DEBUG_OUT<<"(GetAttachedSurface)\n";
    retVal=surfPtr->GetAttachedSurface(&theCaps,&offscreenPtr);
    DEBUG_OUT<<"(ok)\n";
    if(retVal==DD_OK) {
      DEBUG_OUT<<"(got it)\n";
    }
  }
  else {
    // we didn't get a back buffer, create another simple surface...
    DEBUG_OUT<<"Creating another simple surface...\n";
    memset(&sDesc,0,sizeof(DDSURFACEDESC));
    sDesc.dwSize=sizeof(DDSURFACEDESC);
    sDesc.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
    sDesc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;
    sDesc.dwWidth=width;
    sDesc.dwHeight=height;
    DEBUG_OUT<<"(CreateSurface)\n";
    retVal=ddPtr->CreateSurface(&sDesc,&offscreenPtr,NULL);
    DEBUG_OUT<<"(ok)\n";
    if(retVal==DD_OK) {
      DEBUG_OUT<<"(got it)\n";
    }
  }
  if(retVal!=DD_OK) DEBUG_OUT.warning("DirectDraw couldn't get 2nd surface!\n");
  DEBUG_OUT<<"Got the 2nd surface...\n";
  if(surfPtr) surfPtr->SetPalette(winPalette);
  DEBUG_OUT<<"Got DirectDraw.\n";
  didInitDDraw=true;
  return ret;
}
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32

