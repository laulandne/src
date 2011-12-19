
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/AApp.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>

#include <guilib/ADisplay.h>


#ifdef ASYS_WINDOWS
#include <guilib/alib_win.h>
#endif // ASYS_DOS

#ifdef ASYS_MAC
#include <guilib/alib_carbon.h>
#endif // ASYS_MAC

#ifdef ASYS_COCOA
#include <guilib/alib_cocoa.h>
#endif // ASYS_COCOA

#ifdef ASYS_GLUT
#include <guilib/alib_glut.h>
#endif // ASYS_GLUT

#ifdef ASYS_DOS
#include <guilib/alib_dos.h>
#endif // ASYS_DOS

#ifdef ASYS_AMIGA
#include <guilib/alib_amiga.h>
#include <exec/libraries.h>
#include <clib/exec_protos.h>
#endif // ASYS_AMIGA


#ifdef ALIB_HAVE_SYS_UNISTD_H
//#include <sys/unistd.h>
#endif // ALIB_HAVE_SYS_UNISTD_H
#ifdef ALIB_HAVE_SYS_STAT_H
//#include <sys/stat.h>
#endif // ALIB_HAVE_SYS_STAT_H
#ifdef ALIB_HAVE_STAT_H
#include <stat.h>
#endif // ALIB_HAVE_STAT_H
#ifdef ALIB_HAVE_LIBC_H
#include <libc.h>
#endif // ALIB_HAVE_LIBC_H
#ifdef ALIB_HAVE_UNISTD_H
#include <unistd.h>
#endif // ALIB_HAVE_UNISTD_H
#ifdef ALIB_HAVE_DIRECT_H
#include <direct.h>
#endif // ALIB_HAVE_DIRECT_H
#ifdef ALIB_HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif // ALIB_HAVE_SYS_UTSNAME_H
#ifdef ALIB_HAVE_SIGNAL_H
//#include <signal.h>
#endif // ALIB_HAVE_SIGNAL_H

#ifdef ALIB_HAVE_CURSES_H
#include <curses.h>
//#include <signal.h>
#include <termios.h>
#endif // ALIB_HAVE_CURSES_H

#

// FIXME: This object is referenced by many other objects to
// get system specific info that doesn't fit anywhere else.
// This is basically a kludge for the moment.
AApp aThisApp;


#ifdef ALIB_HAVE_CURSES_H
extern void rl_ttyset(int reset);
#endif // ALIB_HAVE_CURSES_H


////////////////////////////////////////////////////////////////////////////////
//  AApp Class
////////////////////////////////////////////////////////////////////////////////

AApp::AApp()
{
  // (The DEBUG_OUT object might not have been created yet...)
  DEBUG_OUT<<"AApp()\n";
  // Set application defaults
  init();
#ifdef ASYS_WIN32CE
#if CE_MAJOR_VER >2
  alib_aygshellHandle=LoadLibrary(L"aygshell.dll");
#endif
#endif // ASYS_WIN32CE
#ifdef ASYS_WIN16
  // All this doesn't REALLY belong here, but in start instead
#ifndef REALLY_OLD_WINDOWS
  alib_mmSystemHandle=LoadLibrary("mmsystem.dll");
  if((int)alib_mmSystemHandle<33) {
    //DEBUG_OUT<<"Couldn't load mmsystem.dll!\n";
    alib_mmSystemHandle=(HINSTANCE)NULLHANDLE;
    //return;
  }
  if(alib_mmSystemHandle) {
    // First dynamically resolve the functions we need
    /*
    if(!_dll_joyGetDevCaps) {
      char *funcName="JOYGETDEVCAPS";
      char *modName="MMSYSTEM";
      _dll_joyGetDevCaps=(MMRESULT (pascal *)(UINT,LPJOYCAPS,UINT))GET_PROC(modName,funcName);
      if(!_dll_joyGetDevCaps) {
        //DEBUG_OUT<<"Couldn't getProc joyGetDevCaps!\n";
      }
    }
    */
  }
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN16
#ifdef ASYS_OS2
#ifdef __EMX__
  // If we run a binary compiled with EMX for os2 on dos
  if(!(_emx_env&0x0200)) {
    aStdErrStream<<"This version of ALib for OS2 only!\n";
    //DEBUG_OUT<<"This version of ALib for OS2 only!\n";
    exit(5);
  }
#endif // __EMX__
#endif // ASYS_OS2
#ifdef ASYS_DOS
#ifdef __EMX__
  // If we run a binary compiled with EMX for dos on os2
  if(_emx_env&0x0200) {
    aStdErrStream<<"This version of ALib for DOS only!\n";
    //DEBUG_OUT<<"This version of ALib for DOS only!\n";
    exit(5);
  }
#endif // __EMX__
#endif // ASYS_DOS
  DEBUG_OUT<<"AApp() done.\n";
}


AApp::~AApp()
{
  DEBUG_OUT<<"~AApp()\n";
  // Remove list of possible display modes (see display.cpp)
    //if(dmList) delete dmList;
    //dmList=(AList *)NULL;
#ifdef ASYS_MAC
    /*
    if(myWorld) DisposeGWorld(myWorld);
    myWorld=NULL;
    */
#if TARGET_CARBON
  // TODO: we arbitrarily don't do this on classic so we don't require macos 8
#ifdef MAC_SYSTEM_8
#ifndef MAC_OLD_HEADERS
#if !__LP64__
    if(alib_usingAppearance) UnregisterAppearanceClient();
#endif // __LP64__
#endif // MAC_OLD_HEADERS
#endif // MAC_SYSTEM_8
#endif // TARGET_CARBON
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
    //The DEBUG_OUT object may not still be around...
    //DEBUG_OUT<<"Closing Amiga system libraries...\n";
    if(AslBase) CloseLibrary(AslBase);
    if(WorkbenchBase) CloseLibrary(WorkbenchBase);
    if(IntuitionBase) CloseLibrary(IntuitionBase);
    if(GfxBase) CloseLibrary((Library *)GfxBase);
#endif // ASYS_AMIGA
#ifdef ASYS_OS2
    WinDestroyMsgQueue(aThisApp.ahMQ);
    WinTerminate(aThisApp.ahAB);
#endif // ASYS_OS2
#ifdef ASYS_DOS
    REGS16 rin,rout;
    if(setMode) {
      DEBUG_OUT<<"Restoring your original video mode ("<<myHex(origMode)<<")\n";
      rin.h.ah=0;  rin.h.al=(char)origMode;
      int86(0x10,&rin,&rout);
      // Clear screen to attrib
      if(defaultAttrib) {
        // FIXME: assume page 0
        rin.h.ah=0x06;  rin.h.al=0;
        rin.h.bh=defaultAttrib;
        rin.h.ch=0;  rin.h.cl=0;
        // FIXME: assume text screen is 80x25
        rin.h.dh=25;  rin.h.dl=80;
        int86(0x10,&rin,&rout);
      }
    }
    // Hide mouse
    if(aThisApp.hasMouse) {
      DEBUG_OUT<<"Hiding mouse...\n";
#ifdef __SC__
      rin.x.ax=2;
#else
      rin.w.ax=2;
#endif
      int86(0x33,&rin,&rout);
    }
#endif // ASYS_DOS
#ifdef ASYS_WIN32CE
#if CE_MAJOR_VER >2
  if(alib_aygshellHandle) FreeLibrary(alib_aygshellHandle);
  alib_aygshellHandle=(HINSTANCE)NULL;
#endif
#endif // ASYS_WIN32CE
#ifdef ASYS_WIN16
#ifndef REALLY_OLD_WINDOWS
    if((int)alib_mmSystemHandle>32) FreeLibrary(alib_mmSystemHandle);
    alib_mmSystemHandle=(HINSTANCE)NULLHANDLE;
    //_dll_joyGetDevCaps=0;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN16
#ifdef ASYS_WINDOWS
#ifdef USE_QUICKTIME
  TerminateQTML();
#endif // USE_QUICKTIME
#endif // ASYS_WINDOWS
    //The DEBUG_OUT object may not still be around...
    //DEBUG_OUT<<"-----------------------\n";
#ifdef ALIB_HAVE_CURSES_H
  mvcur(0,COLS-1,LINES-1,0);
  endwin();
  fflush(NULL);
  rl_ttyset(1);
#endif // ALIB_HAVE_CURSES_H
}


void AApp::init()
{
#ifdef __MWERKS__
  // FIXME: Seed with more "random" number...
  //srand(0xffff);
#endif // __MWERKS__
#ifdef HAS_RANDOMIZE
  randomize();
#endif // HAS_RANDOMIZE
  // Init all data members to known values
  version=0;  versionMinor=0;
  argc=0;
  for(unsigned int tt=0;tt<ALIB_MAX_ARGV;tt++) {
    argv[tt]=(char *)NULL;
  }
  littleEndian=false;
  for(unsigned int mm=0;mm<A_MAX_MENUS;mm++) {
    menuItem[mm]=false; itemEnabled[mm]=false;
  }
  sync=false;
  //dmList=new AList;
  //videoHardware=0;
  //defaultDepth=8;  defaultPlanes=1;
  allowRawDecode=false;
  joyNotKeys=false;
  whichJoy=0;
  //needToDrawCursor=true;
  hasOpenGL=false;
  has3DHardware=false;
  //defaultDisplay=(ADisplay *)NULL;
  //defaultWindow=(AWindowAbstract *)NULL;
  //defaultAudioDevice=(AAudioDevice *)NULL;
  //defaultLoop=NULL;
  //quitASAP=false;
#ifdef ASYS_WIN32CE
#if CE_MAJOR_VER >2
  alib_aygshellHandle=(HINSTANCE)NULL;
#endif
#endif // ASYS_WIN32CE
#ifdef ASYS_WIN16
  alib_mmSystemHandle=(HANDLE)NULLHANDLE;
#endif // ASYS_WIN16
#ifdef ASYS_WINDOWS
  //pathCharacter=ATEXT("\\");
  //pathRootCharacter=ATEXT("\\");
  alib_ahInst=(HINSTANCE)NULLHANDLE;
  alib_ahPrevInst=(HINSTANCE)NULLHANDLE;
  alib_ahWnd=(HWND)NULLHANDLE;
  alib_aNShow=0;
  alib_awParam=(WPARAM)0;
  //alib_lastModifier=A_MOD_NONE;
#ifdef ASYS_MFC
  theWindow=NULL;
#endif // ASYS_MFC
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
  //pathCharacter=ATEXT(":");
  //pathRootCharacter=ATEXT("::");
  alib_inBackground=false;
  alib_appleMenu=(MenuHandle)NULL;  alib_fileMenu=(MenuHandle)NULL;
  alib_editMenu=(MenuHandle)NULL;
  alib_qtVer=0;  alib_qtRev=0;
  alib_dmVer=0;  alib_dmRev=0;
  alib_cbonVer=0;  alib_cbonRev=0;
  alib_hasGX=false;
  alib_hasQD3D=false;
  alib_hasARTA=false;
  alib_hasQuartz=false;
  alib_navServices=false;
  alib_blueBox=false;
  alib_bugHunt=false;
  alib_usingAppearance=false;
  alib_usingOT=false;
  alib_useAppleEvents=false;
  //alib_myWorld=NULL;
#endif // ASYS_MAC
#ifdef ASYS_OS2
  pathCharacter=ATEXT("\\");
  pathRootCharacter=ATEXT("\\");
  ahAB=(HAB)NULL;
  ahMQ=(HMQ)NULL;
  //aQMsg=(QMSG)NULL;
  ahWnd=(HWND)NULL;
#endif // ASYS_OS2
#ifdef ASYS_DOS
  pathCharacter=ATEXT("\\");
  pathRootCharacter=ATEXT("\\");
  hasMouse=false;
  origMode=0;
  defaultAttrib=0;
  hercGfx=false;  hasMono=false;
  monoMode=false;  hasHerc=false;
  setMode=false;
  vesaMajor=0;  vesaMinor=0;
  center1x=0;  center1y=0;
  center2x=0;  center2y=0;
  extendedKey=false;
  needToDrawCursor=false;
#endif // ASYS_DOS
#ifdef ASYS_POSIX
  //fullScreenOnly=true;
  //needToDrawCursor=false;
#ifdef USE_ASYNC_STDIO_INPUT
  input_fd= -1;
#endif // USE_ASYNC_STDIO_INPUT
#endif // ASYS_POSIX
#ifdef ASYS_X11
  visualWarned=false;
  hasJoystick=false;
  joyfd= -1;
  numJoyAxes=0;
  numJoyButtons=0;
  axe0middle=0;  axe1middle=0;
#endif // ASYS_X11
#ifdef ASYS_AMIGA
  pathCharacter=ATEXT("/");
  pathRootCharacter=ATEXT(":");
  oldDir=(BPTR)NULL;
  hasECS=false;  hasAGA=false;  hasPicasso=false;
#endif //ASYS_AMIGA
#ifdef ASYS_PALM
  hasThreePointFive=false;
  hasIntl=false;
  hasJapanese=false;
  hasWireless=false;
  hasColor=false;
  hasHighRes=false;
  hasVFS=false;
  isHandspring=false;
  isSony=false;
#endif // ASYS_PALM
}


/*
void AApp::initDefaultDisplay()
{
  //if(defaultDisplay) return;
  //defaultDisplay=new ADisplay;
}
*/


/*
void AApp::initDefaultAudioDevice()
{
  if(defaultAudioDevice) return;
  defaultAudioDevice=new AAudioDevice;
}
*/


// NOTE: Return value is not necessarily accurate...just a decent guess
char *AApp::getNativeCPUType()
{
  char *ret=NULL;
  //
#if TARGET_CPU_PPC
  ret=ATEXT("ppc");
#endif // TARGET_CPU_PPC
#if TARGET_CPU_68K
  ret=ATEXT("m68k");
#endif // TARGET_CPU_68K
#if TARGET_CPU_X86
  ret=ATEXT("x86");
#endif // TARGET_CPU_X86
#if TARGET_CPU_MIPS
  ret=ATEXT("mips");
#endif // TARGET_CPU_MIPS
#if TARGET_CPU_SPARC
  ret=ATEXT("sparc");
#endif // TARGET_CPU_SPARC
#if TARGET_CPU_ALPHA
  ret=ATEXT("alpha");
#endif // TARGET_CPU_ALPHA
  //
#ifdef _M_IX86
  ret=ATEXT("x86");
#endif // _M_IX86
#ifdef _M_AMD64
  ret=ATEXT("amd64");
#endif // _M_AMD64
#ifdef _M_ALPHA
  ret=ATEXT("alpha");
#endif // _M_ALPHA
#ifdef _M_AXP64
  ret=ATEXT("alpha");
#endif // _M_AXP64
#ifdef _M_MRX000
  ret=ATEXT("mips");
#endif // _M_MRX000
#ifdef _MIPS_
  ret=ATEXT("mips");
#endif // _MIPS_
#ifdef _M_PPC
  ret=ATEXT("ppc");
#endif // _M_PPC
#ifdef _PPC_
  ret=ATEXT("ppc");
#endif // _PPC_
#ifdef _MPPC_
  ret=ATEXT("ppc");
#endif // _MPPC_
#ifdef _M_IA64
  ret=ATEXT("ia64");
#endif // _M_IA64
#ifdef _IA64_
  ret=ATEXT("ia64");
#endif // _IA64_
#ifdef SHx
  ret=ATEXT("sh3");
#endif // SHx
#ifdef _M_ARM
  ret=ATEXT("arm");
#endif // _M_ARM
#ifdef ARM
  ret=ATEXT("arm");
#endif // ARM
#ifdef _68K_
  ret=ATEXT("m68k");
#endif // _68K_
  //
#ifdef ASYS_OS2
  // Check for PowerPC?
  if(!ret) ret=ATEXT("x86");
#endif // ASYS_OS2
#ifdef ASYS_MSDOS
  ret=ATEXT("i386");
#endif // ASYS_MSDOS
#ifdef ASYS_PALM
  // Check for ARM?
  if(!ret) ret=ATEXT("m68k");
#endif // ASYS_PALM
#ifdef ASYS_AMIGA
  // Check for PowerPC
  if(!ret) ret=ATEXT("m68k");
#endif // ASYS_AMIGA
#ifdef ASYS_AMIGASDK
  ret=ATEXT("Elate VP");
#endif // ASYS_AMIGASDK
#ifdef ASYS_BE
  // Check for x86
  if(!ret) ret=ATEXT("ppc");
#endif // ASYS_BE
#ifdef ASYS_KOS
  // Hardcoded for now
  ret=ATEXT("sh4");
#endif // ASYS_KOS
  //
#ifdef ALIB_HAVE_SYS_UTSNAME_H
  struct utsname myUName;
  //if(!uname(&myUName)) { ret=strdup(myUName.machine); }
  uname(&myUName);  ret=strdup(myUName.machine);
#endif // ALIB_HAVE_SYS_UTSNAME_H
  //
  if(!ret) ret=(char *)"unknown";
  return ret;
}


// NOTE: Return value is not necessarily accurate...just a decent guess
char *AApp::getNativeOSType()
{
  char *ret=(char *)"unknown";
#ifdef ASYS_PALM
  ret=ATEXT("palm");
#endif // ASYS_PALM
#ifdef ASYS_OS2
  ret=ATEXT("os2");
#endif // ASYS_OS2
#ifdef ASYS_MSDOS
  ret=ATEXT("msdos");
#endif // ASYS_MSDOS
#ifdef ASYS_WINDOWS
  ret=ATEXT("windows");
#endif // ASYS_WINDOWS
#ifdef ASYS_AMIGA
  ret=ATEXT("amiga");
#endif // ASYS_AMIGA
#ifdef ASYS_AMIGASDK
  ret=ATEXT("amigasdk");
#endif // ASYS_AMIGASDK
#ifdef ASYS_KOS
  ret=ATEXT("KallistiOS");
#endif // ASYS_KOS
#ifdef ASYS_BE
  ret=ATEXT("BeOS");
#endif // ASYS_BE
#ifdef ASYS_MAC
#if TARGET_CPU_PPC
  ret=ATEXT("powermac");
#else
  ret=ATEXT("mac68k");
#endif // TARGET_CPU_PPC
#endif // ASYS_MAC
#ifdef ALIB_HAVE_SYS_UTSNAME_H
  struct utsname myUName;
  //if(!uname(&myUName)) { ret=strdup(myUName.sysname); }
  uname(&myUName);  ret=strdup(myUName.sysname);
#endif // ALIB_HAVE_SYS_UTSNAME_H
  return ret;
}


// NOTE: Return value is not necessarily accurate...just a decent guess
char *AApp::getNativeOSVersion()
{
  char *ret=(char *)"unknown";
#ifdef ALIB_HAVE_SYS_UTSNAME_H
  struct utsname myUName;
  //if(!uname(&myUName)) { ret=strdup(myUName.release); }
  uname(&myUName);  ret=strdup(myUName.release);
#endif // ALIB_HAVE_SYS_UTSNAME_H
  return ret;
}


void AApp::yieldToOtherProcesses()
{
  // the idea is you call this when you're in the middle of something
  // that'll take a long time...call this and you won't starve out other
  // processes (NOTE: only needed on a cooperatively multitasking system,
  // don't bother if you're on unix, etc)
#ifdef ASYS_MAC
#ifndef __LP64__
  unsigned long limit=GetCaretTime();
#endif // __LP64__
  EventRecord event;
  // Should the 0 be limit instead?
#ifdef USE_CARBON_EVENTS
#else
  WaitNextEvent(everyEvent,&event,0,(RgnHandle)nil);
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC
}

#ifdef ALIB_HAVE_CURSES_H
void rl_ttyset(int reset);
#endif // ALIB_HAVE_CURSES_H

#ifdef ALIB_HAVE_CURSES_H
void rl_ttyset(int reset)
{
  DEBUG_OUT<<"rl_ttyset("<<reset<<")\n";
  static struct termios old;
  struct termios new1;
  if(!reset) {
    tcgetattr(0,&old);
    new1=old;
    new1.c_lflag&=~(ECHO|ICANON);
    new1.c_iflag&=~(ISTRIP|INPCK);
    tcsetattr(0,TCSANOW,&new1);
  }
  else {
    tcsetattr(0,TCSANOW,&old);
  }
}
#endif // ALIB_HAVE_CURSES_H


/*
#ifdef ALIB_HAVE_SIGNAL_H
void die(int s);

void die(int s)
{
  DEBUG_OUT<<"(die)\n";
  signal(SIGINT,SIG_IGN);
#ifdef ALIB_HAVE_CURSES_H
  mvcur(0,COLS-1,LINES-1,0);
  endwin();
  fflush(NULL);
  rl_ttyset(1);
#endif // ALIB_HAVE_CURSES_H
  exit(0);
}
#endif // ALIB_HAVE_SIGNAL_H
*/

void AApp::start(int aargc, char **aargv/*,unsigned outSize*/)
{
  DEBUG_OUT<<"AApp::start("<<argc<<",argv)\n";
  /*
  if(outSize!=sizeof(AApp)) {
    DEBUG_OUT<<"AApp::start, sizeof mismatch!\n";
	exit(5);
  }
  */
#ifdef ASYS_WINDOWS
#ifdef __GNUC__
  handleArgs(aargc,aargv);
#else
  if(aargv) aDefaultStringHelper.parseString(*aargv,ATEXT("Alib App"));
  handleArgs(aargc,aargv);
#endif // __GNUC__
#else
  handleArgs(aargc,aargv);
#endif // ASYS_WINDOWS
  start();
}


// NOTE: We assume cmdline doesn't include the command name itself...
void AApp::start(char *cmdline)
{
  DEBUG_OUT<<"AApp::start("<<cmdline<<")\n";
  if(cmdline) aDefaultStringHelper.parseString(cmdline,(char *)"Alib App");
  handleArgs(aDefaultStringHelper.getArgc(),aDefaultStringHelper.getAllArgv());
  start();
}


void AApp::start()
{
  DEBUG_OUT<<"AApp::start()\n";
//#ifdef ALIB_HAVE_SIGNAL_H
 // signal(SIGINT,die);
//#endif // ALIB_HAVE_SIGNAL_H
//#ifdef ALIB_HAVE_CURSES_H
 // rl_ttyset(0);
//#endif // ALIB_HAVE_CURSES_H
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"-----------------------\n";
  DEBUG_OUT<<"ALib Libraries\n";
#ifdef UNICODE
  DEBUG_OUT<<" (UNICODE)";
#else
  DEBUG_OUT<<" (ANSI)";
#endif // UNICODE
  DEBUG_OUT<<"\n";
  // NOTE: Only compilers I've personally tested are here...
  DEBUG_OUT<<"Compiled using ";
  bool done=false;
#ifdef __GNUC__
  DEBUG_OUT<<"Gnu GCC";  done=true;
#endif // __GNUC__
#ifdef __MINGW32_VERSION
  DEBUG_OUT<<"(MingW version "<<__MINGW32_VERSION<<")";  done=true;
#endif // __MINGW32_VERSION
#ifdef __SC__
  DEBUG_OUT<<"Symantec C++";  done=true;
#endif // __SC__
#ifdef __WATCOMC__
  DEBUG_OUT<<"Watcom C++";  done=true;
#endif // __WATCOMC__
#ifdef __MWERKS__
  DEBUG_OUT<<"Metrowerks CodeWarrior "<<aThisApp.myHex((unsigned long)__MWERKS__);
  done=true;
#endif // __MWERKS__
#ifdef __BORLANDC__
  DEBUG_OUT<<"Borland C++";  done=true;
#endif // __BORLANDC__
#ifdef _MSC_VER
  // Because sometimes you have to lie and say you're
  // using Microsoft's compiler, but you aren't.
  if(!done) {
    DEBUG_OUT<<"Microsoft C++ "<<_MSC_VER;
    done=true;
  }
#endif // _MSC_VER
#ifdef __IBMCPP__
  DEBUG_OUT<<"IBM C++";  done=true;
#endif
  if(!done) DEBUG_OUT<<"Unknown compiler";
  DEBUG_OUT<<" on "<<__DATE__<<" at "<<__TIME__<<"\n";
#ifdef UNIVERSAL_INTERFACES_VERSION
  DEBUG_OUT<<"Built using Apple Universal Interfaces "
    <<aDefaultStringHelper.myHex((unsigned long)UNIVERSAL_INTERFACES_VERSION)<<"\n";
#endif // UNIVERSAL_INTERFACES_VERSION
  //
#if TARGET_OS_MAC
  DEBUG_OUT<<"Target OS: Mac OS\n";
#endif // TARGET_OS_MAC
#if TARGET_OS_WIN32
  DEBUG_OUT<<"Target OS: Win32\n";
#endif // TARGET_OS_WIN32
#if TARGET_OS_UNIX
  DEBUG_OUT<<"Target OS: Unix\n";
#endif // TARGET_OS_UNIX
  //
#if TARGET_CPU_PPC
  DEBUG_OUT<<"Target CPU: PPC\n";
#endif // TARGET_CPU_PPC
#if TARGET_CPU_PPC64
  DEBUG_OUT<<"Target CPU: PPC64\n";
#endif // TARGET_CPU_PPC64
#if TARGET_CPU_68K
  DEBUG_OUT<<"Target CPU: m68k\n";
#endif // TARGET_CPU_68K
#if TARGET_CPU_X86
  DEBUG_OUT<<"Target CPU: x86\n";
#endif // TARGET_CPU_X86
#if TARGET_CPU_X86_64
  DEBUG_OUT<<"Target CPU: x86_64\n";
#endif // TARGET_CPU_X86_64
#if TARGET_CPU_MIPS
  DEBUG_OUT<<"Target CPU: mips\n";
#endif // TARGET_CPU_MIPS
#if TARGET_CPU_SPARC
  DEBUG_OUT<<"Target CPU: sparc\n";
#endif // TARGET_CPU_SPARC
#if TARGET_CPU_ALPHA
  DEBUG_OUT<<"Target CPU: alpha\n";
#endif // TARGET_CPU_ALPHA
  //
#if TARGET_RT_MAC_CFM
  DEBUG_OUT<<"Target RT: Code Fragment Manager\n";
#endif // TARGET_RT_CFM
#if TARGET_RT_MAC_MACHO
  DEBUG_OUT<<"Target RT: Mach-O runtime\n";
#endif // TARGET_RT_MACHO
#if TARGET_RT_64_BIT
  DEBUG_OUT<<"Target RT: 64-bit runtime\n";
#endif // TARGET_RT_64_BIT
#if TARGET_RT_LITTLE_ENDIAN
  DEBUG_OUT<<"Target RT: Little endian\n";
#endif // TARGET_RT_LITTLE_ENDIAN
#if TARGET_RT_BIG_ENDIAN
  DEBUG_OUT<<"Target RT: Big endian\n";
#endif // TARGET_RT_BIG_ENDIAN
  //
#if TARGET_API_MAC_OS8
  DEBUG_OUT<<"Target API: Mac OS 8\n";
#endif // TARGET_API_MAC_OS8
#if TARGET_API_MAC_CARBON
#ifndef ASYS_WIN32
  DEBUG_OUT<<"Target API: Mac OS Carbon\n";
#endif // ASYS_WIN32
#endif // TARGET_API_MAC_CARBON
#if TARGET_API_MAC_OSX
  DEBUG_OUT<<"Target API: Mac OS X\n";
#endif // TARGET_API_MAC_OSX
  //
#ifdef WINVER
  DEBUG_OUT<<"WINVER is "<<aDefaultStringHelper.myHex((unsigned long)WINVER)<<"\n";
#endif // WINVER
#endif // DEBUG_VERBOSE
#ifdef USE_NETPBM
  DEBUG_OUT<<"Using NetPBM commands to decode images.\n";
#endif // USE_NETPBM
#ifdef ALIB_SMALL_MEM
  DEBUG_OUT<<"NOTE: memory is small...\n";
#endif // ALIB_SMALL_MEM
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Host: "<<getNativeOSType()<<" "<<getNativeOSVersion()
    <<" "<<getNativeCPUType();
  unsigned int think=0;
#ifdef A_LITTLE_ENDIAN
  DEBUG_OUT<<" (Little-Endian)";  think=21;
#endif // A_LITTLE_ENDIAN
#ifdef A_BIG_ENDIAN
  DEBUG_OUT<<" (Big-Endian)";  think=12;
#endif // A_BIG_ENDIAN
  DEBUG_OUT<<"\n";
#endif // DEBUG_VERBOSE
  // This code fiddles around with a long int to figure out
  // the host's Endian-ness.  This won't handle REALLY weird cases
  // like some VAX's I've heard of...(0x12345678 becomes 56,78,12,34)
  littleEndian=false;
  unsigned long l=0x12345678L;
  unsigned char *a;
  a=(unsigned char *)&l;
#ifdef DEBUG_VERBOSE
  //for(unsigned int t=0;t<4;t++) DEBUG_OUT<<(int)a[t]<<"\n";
#endif // DEBUG_VERBOSE
  if(a[0]==0x78) littleEndian=true;
  if(!think) { //DEBUG_OUT.warning("Couldn't guess endian!\n")
    ; }
  else {
    bool agree=false;
    if((littleEndian)&&(think==21)) agree=true;
    if((!littleEndian)&&(think==12)) agree=true;
    if(!agree) {
      DEBUG_OUT<<"(Endian detection did NOT agree!)\n";
      DEBUG_OUT<<"(We think littleEndian should be "<<(int)littleEndian<<")\n";
    }
  }
#ifdef __LP64__
  DEBUG_OUT<<"NOTE: __LP64__ is set...\n";
#endif
  // Check size of integer types
  //bool warnInt=false,warnShort=false,warnLong=false;
  //if(sizeof(UINT8)!=1) { DEBUG_OUT<<"NOTE: UINT8's aren't 8-bits...\n"; warnInt=true; }
  //if(sizeof(UINT16)!=2) { DEBUG_OUT<<"NOTE: UINT16's aren't 16-bits...\n"; warnShort=true; }
  //if(sizeof(UINT32)!=4) { DEBUG_OUT<<"NOTE: UINT32's aren't 32-bits...\n"; warnLong=true; }
  //if(sizeof(float)!=4) {
    //DEBUG_OUT<<"NOTE: float isn't 4 bytes!\n";
  //}
  //if(sizeof(double)!=8) {
    //DEBUG_OUT<<"NOTE: double isn't 4 bytes!\n";
  //}
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"-----------------------\n";
#endif // DEBUG_VERBOSE
#ifdef ASYS_AMIGA
  amiga_start();
#endif // ASYS_AMIGA
#ifdef ASYS_DOS
  dos_start();
#endif // ASYS_DOS
#ifdef ASYS_MAC
  mac_start();
#endif // ASYS_MAC
#ifdef ASYS_COCOA
  cocoa_start();
#endif // ASYS_COCOA
#ifdef ASYS_OS2
  os2_start();
#endif // ASYS_OS2
#ifdef ASYS_PALM
  palm_start();
#endif // ASYS_PALM
#ifdef ASYS_WINDOWS
  win_start();
#endif // ASYS_WINDOWS
#ifdef ASYS_X11
  x11_start();
#endif // ASYS_X11
#ifdef ASYS_GLUT
  glut_start(aDefaultStringHelper.getArgc(),aDefaultStringHelper.getAllArgv());
#endif // ASYS_GLUT
#ifdef ASYS_SDL
  //if(SDL_Init(SDL_INIT_EVERYTHING)) { DEBUG_OUT<<"SDL_Init returned error!\n"; exit(5); }
  //atexit(SDL_Quit);
#ifdef ASYS_DARWIN
  //cocoa_sdl_start();
#endif // ASYS_DARWIN
#endif // ASYS_SDL
  DEBUG_OUT<<"AApp::start() done.\n";
}


void AApp::handleArgs(int aargc,char **aargv,bool maybeWideChars)
{
  bool copyEm=true;
  unsigned int t=0;
#ifdef ASYS_MAC
  copyEm=false;
  argc=1;
  argv[0]=ATEXT("Alib App");
  for(int i=1;i<ALIB_MAX_ARGV;i++) argv[i]=(char *)NULL;
  short appleEvents=1,howMany=0;
  macInitToolbox();
  macInitAppleEvents();
  if(!alib_useAppleEvents) appleEvents=0;
#if! TARGET_CARBON
#if! TARGET_RT_MAC_CFM
  // Only for old 68k arch
  CountAppFiles(&appleEvents,&howMany);
  if(!appleEvents) {
    // File open request
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Got file open message from Finder...\n";
#endif // DEBUG_VERBOSE
    argc+=howMany;
    AppFile fstuff;
    for(int a=1;a<howMany+1;a++) {
      GetAppFiles(a,&fstuff);
      char *ts=P2CStr(fstuff.fName);  argv[a]=strdup(ts);
      DEBUG_OUT<<"arg "<<a<<": "<<argv[a]<<"\n";
      SetVol(nil,fstuff.vRefNum);  ClrAppFiles(a);
    }
  }
#endif // !TARGET_RT_MAC_CFM
#endif // !TARGET_CARBON
  if(appleEvents) macWaitForHighLevel();
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  if(!aargc) {
    // Started from workbench
    copyEm=false;
    WBStartup *wb=(WBStartup *)aargv;
    argc=wb->sm_NumArgs;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Workbench says "<<argc<<" args...\n";
#endif // DEBUG_VERBOSE
    for(unsigned int tt=0;t<argc;tt++) {
      // use strdup because workbench may (will?) free the args...
      argv[tt]=strdup((char *)wb->sm_ArgList[tt].wa_Name);
      DEBUG_OUT<<tt<<": "<<argv[tt]<<"\n";
      CurrentDir(wb->sm_ArgList[tt].wa_Lock);
    }
  }
#endif // ASYS_AMIGA
#ifdef ASYS_COCOA
  copyEm=false;
  argc=aargc;
  unsigned int tt=0;
  t=0;
  while(t<(unsigned int)aargc) {
    if(!strcmp(aargv[t],"-_NSMachLaunch")) { argc-=2; t+=2; }
    else if(!strcmp(aargv[t],"-MachLaunch")) { argc-=2; t+=2; }
    else { argv[tt]=aargv[t]; tt++; t++; }
  }
  for(tt=0;tt<argc;tt++) DEBUG_OUT<<tt<<":"<<argv[tt]<<"\n";
#endif // ASYS_COCOA
  if(copyEm) {
    DEBUG_OUT<<"handleArgs...\n";
    argc=aargc;
    for(t=0;t<argc;t++) {
      argv[t]=aargv[t];
      DEBUG_OUT<<t<<":"<<argv[t]<<"\n";
    }
  }
}


#ifdef ASYS_COCOA
void AApp::cocoa_start()
{
  DEBUG_OUT<<"cocoa_start()...\n";
  DEBUG_OUT<<"Calling obj-c startup...\n";
  AApp_cocoa_start();
  DEBUG_OUT<<"back from obj-c.\n";
  version=alib_version;  versionMinor=alib_versionMinor;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"MacOS Version: "<<version<<".";
  if(versionMinor<10) DEBUG_OUT<<"0";
  DEBUG_OUT<<versionMinor<<"\n";
#endif // DEBUG_VERBOSE
  //useDebugger=alib_useDebugger;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"QuickTime "<<alib_qtVer<<"."<<alib_qtRev<<" present.\n";
#endif // DEBUG_VERBOSE
  // Menu inits
  for(UINT t=0;t<A_MAX_MENUS;t++) {
     menuItem[t]=false; itemEnabled[t]=false;
  }
  menuItem[A_MENU_CLOSE]=false;  itemEnabled[A_MENU_CLOSE]=true;
  menuItem[A_MENU_QUIT]=false;  itemEnabled[A_MENU_QUIT]=true;
  menuItem[A_MENU_ABOUT]=false;  itemEnabled[A_MENU_ABOUT]=true;
}
#endif // ASYS_COCOA



// FIXME: Not fully implemented yet!
bool AApp::activateMenu(char *menuName,char *itemName)
{
  if(!strcmp("File",menuName)) {
    if(!strcmp("Open",itemName)) return turnMenuNumOn(A_MENU_OPEN);
    return false;
  }
  return false;
}


// FIXME: Not fully implemented yet!
bool AApp::disactivateMenu(char *menuName,char *itemName)
{
  if(!strcmp("File",menuName)) {
    if(!strcmp("Open",itemName)) return turnMenuNumOff(A_MENU_OPEN);
    return false;
  }
  return false;
}


bool AApp::turnMenuNumOn(unsigned int menuNum)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"turnMenuNumOn("<<menuNum<<")...\n";
#endif // DEBUG_VERBOSE
  if(menuNum<A_MENU_LAST) {
    itemEnabled[menuNum]=true;
    menuItem[menuNum]=true;
#ifdef ASYS_MAC
    unsigned int macItem=0;
    switch(menuNum) {
      case A_MENU_OPEN: macItem=2; break;
      default:
        break;
    }
    if(macItem)
#if TARGET_CARBON
#if !__LP64__
      MacEnableMenuItem(alib_fileMenu,macItem);
#else
      ;
#endif // __LP64__
#else
      EnableItem(alib_fileMenu,macItem);
#endif // TARGET_CARBON
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
    alib_menuHandle=GetMenu(alib_ahWnd);
    alib_menuHandle=GetSubMenu(alib_menuHandle,0);
#endif // ASYS_WIN32CE
    unsigned int winItem=0;
    switch(menuNum) {
      case A_MENU_OPEN: winItem=1; break;
      default:
        break;
    }
    int res=false;
#ifndef ASYS_WIN32CE
    if(winItem!=0) res=EnableMenuItem(alib_menuHandle,winItem,MF_BYPOSITION|MF_ENABLED);
#endif // ASYS_WIN32CE
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"EnableMenuItem returned "<<res<<"\n";
#endif // DEBUG_VERBOSE
#endif // ASYS_WINDOWS
  }
  return false;
}


bool AApp::turnMenuNumOff(unsigned int menuNum)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"turnMenuNumOff("<<menuNum<<")...\n";
#endif // DEBUG_VERBOSE
  if(menuNum<A_MENU_LAST) {
    itemEnabled[menuNum]=false;
    menuItem[menuNum]=false;
#ifdef ASYS_MAC
    unsigned int macItem=0;
    switch(menuNum) {
      case A_MENU_OPEN: macItem=2; break;
      default:
        break;
    }
    if(macItem)
#if TARGET_CARBON
#if !__LP64__
      DisableMenuItem(alib_fileMenu,macItem);
#else
      ;
#endif // __LP64__
#else
      DisableItem(alib_fileMenu,macItem);
#endif // TARGET_CARBON
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
    alib_menuHandle=GetMenu(alib_ahWnd);
    alib_menuHandle=GetSubMenu(alib_menuHandle,0);
#endif // ASYS_WIN32CE
    unsigned int winItem=0;
    switch(menuNum) {
      case A_MENU_OPEN: winItem=1; break;
      default:
        break;
    }
    int res=false;
#ifndef ASYS_WIN32CE
    if(winItem!=0) res=EnableMenuItem(alib_menuHandle,winItem,MF_BYPOSITION|MF_DISABLED);
#endif // ASYS_WIN32CE
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"EnableMenuItem returned "<<res<<"\n";
#endif // DEBUG_VERBOSE
#endif // ASYS_WINDOWS
  }
  return false;
}


void AApp::systemBeep()
{
  bool done=false;
#ifdef __SC__
#ifndef USE_QUICKTIME
  if(!done) {
    //sound_beep(400);
    done=true;
  }
#endif // USE_QUICKTIME
#endif // __SC__
#ifdef ASYS_OS2
  if(!done) {
    DosBeep(400,400);
    done=true;
  }
#endif // ASYS_OS2
#ifdef ASYS_MAC
#if !__LP64__
  SysBeep(30);
#endif // __LP64__
#endif // AYS_MAC
#ifdef ASYS_PALM
  SndPlaySystemSound(sndInfo);
#endif // ASYS_PALM
  if(!done) DEBUG_OUT<<"BEEEEEEEPPPPPP!\n";
}


bool AApp::setAppPrefs(char *name, char *val)
{
  bool ret=false;
  DEBUG_OUT<<"setAppPrefs("<<name<<","<<val<<")\n";
#ifdef ASYS_MAC
#if TARGET_API_MAC_OSX
  CFStringRef theString=CFStringCreateWithBytes(NULL,(const UInt8 *)name,strlen(name),kCFStringEncodingASCII,false);
  CFStringRef theVal=CFStringCreateWithBytes(NULL,(const UInt8 *)val,strlen(val),kCFStringEncodingASCII,false);
  CFPreferencesSetAppValue(theString,theVal,kCFPreferencesCurrentApplication);
  // TODO: error check here
  CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
#endif
#endif  // ASYS_MAC
#ifdef ASYS_WINDOWS
  // TODO: Add registry write here
  //DEBUG_OUT.warning("Registry write not implemented!\n");
#endif // ASYS_WINDOWS
  if(!ret) {
    DEBUG_OUT<<"(Using default prefs write routine)\n";
    //DEBUG_OUT.warning("Default prefs write not implemented!\n");
  /*
    char pathname[ALIB_MAX_NAMELEN];
    pathname[0]=0;
    char prefname[ALIB_MAX_NAMELEN];
    prefname[0]=0;
    getPathFromFullName(pathname,name);
    getNameFromFullName(prefname,name);
    DEBUG_OUT<<"path is "<<pathname<<"\n";
    DEBUG_OUT<<"pref is "<<prefname<<"\n";
    char fname[ALIB_MAX_NAMELEN];
    fname[0]=0;
    fillInString(fname,ATEXT("$HOME/."));
    fillInString(fname,pathname);
    fillInString(fname,ATEXT("rc"));
    DEBUG_OUT<<"filename would be "<<fname<<"\n";
    FILE *f=fopen(fname+2,"r");
    if(!f) f=fopen(fname,"r");
    if(f) {
      bool working=true;
      char buf[1024];
      while(working) {
        buf[0]=0;
        int tret=fscanf(f,"%s\n",buf);
        if(tret==EOF) working=false;
        else {
          DEBUG_OUT<<"line is "<<buf<<"\n";
          fname[0]=0;
          getPathFromFullName(fname,buf);
          DEBUG_OUT<<"name is "<<fname<<"\n";
          if(!strcmp(fname,prefname)) {
            fname[0]=0;
            getNameFromFullName(fname,buf);
            DEBUG_OUT<<"Looks good...value is "<<fname<<"\n";
            ret=strdup(fname);
          }
        }
      }
      fclose(f);
    }
    */
  }
  return ret;
}


char *AApp::getAppPrefs(char *name)
{
  char *ret=(char *)NULL;
  DEBUG_OUT<<"getAppPrefs("<<name<<")\n";
#ifdef ASYS_MAC
#if TARGET_API_MAC_OSX
  DEBUG_OUT<<"Going to CFStringCreateWithBytes...\n";
  CFStringRef theString=CFStringCreateWithBytes(NULL,(const UInt8 *)name,strlen(name),kCFStringEncodingASCII,false);
  CFStringRef theVal;
  DEBUG_OUT<<"Going to CFPreferencesCopyAppValue...\n";
  theVal=(CFStringRef)CFPreferencesCopyAppValue(theString,kCFPreferencesCurrentApplication);
  // TODO: error check here
  // TODO: Convert to C String and store in AApp's buffer
  if(theVal) {
    DEBUG_OUT<<"Going to CFRelease...\n";
    CFRelease(theVal);
    DEBUG_OUT<<"Back from CFRelease.\n";
  }
#endif
#endif  // ASYS_MAC
#ifdef ASYS_WINDOWS
  // TODO: Add registry read here
  //DEBUG_OUT.warning("Registry read not implemented!\n");
#endif // ASYS_WINDOWS
  // FIXME: This code is quick and dirty and full of lots of nastiness
  // The variables are reused and are named things they aren't,
  // functions are used for things they weren't never meant for...bad bad bad!
  if(!ret) {
    DEBUG_OUT<<"(Using default prefs read routine)\n";
    char pathname[ALIB_MAX_NAMELEN];
    pathname[0]=0;
    char prefname[ALIB_MAX_NAMELEN];
    prefname[0]=0;
    aDefaultStringHelper.getPathFromFullName(pathname,name);
    aDefaultStringHelper.getNameFromFullName(prefname,name);
    DEBUG_OUT<<"path is "<<pathname<<"\n";
    DEBUG_OUT<<"pref is "<<prefname<<"\n";
    char fname[ALIB_MAX_NAMELEN];
    fname[0]=0;
    aDefaultStringHelper.fillInString(fname,(char *)"$HOME/.");
    aDefaultStringHelper.fillInString(fname,pathname);
    aDefaultStringHelper.fillInString(fname,(char *)"rc");
    DEBUG_OUT<<"filename would be "<<fname<<"\n";
    FILE *f=fopen(fname+2,"r");
    if(!f) f=fopen(fname,"r");
    if(f) {
      bool working=true;
      char buf[1024];
      while(working) {
        buf[0]=0;
        int tret=fscanf(f,"%s\n",buf);
        if(tret==EOF) working=false;
        else {
          DEBUG_OUT<<"line is "<<buf<<"\n";
          fname[0]=0;
          aDefaultStringHelper.getPathFromFullName(fname,buf);
          DEBUG_OUT<<"name is "<<fname<<"\n";
          if(!strcmp(fname,prefname)) {
            fname[0]=0;
            aDefaultStringHelper.getNameFromFullName(fname,buf);
            DEBUG_OUT<<"Looks good...value is "<<fname<<"\n";
            ret=strdup(fname);
          }
        }
      }
      fclose(f);
    }
  }
  return ret;
}


