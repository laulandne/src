
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/AWindowAbstract.h>
#include <guilib/ADisplay.h>
#include <guilib/AEvent.h>
#include <guilib/APrivateBitmap.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>


#ifdef ASYS_X11_BASED
#include <X11/Xutil.h>
#endif // ASYS_X11_BASED

#ifdef ASYS_DOS
#include <guilib/alib_dos.h>
#endif // ASYS_DOS

#ifdef ASYS_WINDOWS
#include <guilib/alib_win.h>
#endif // ASYS_DOS

#ifdef ASYS_X11
#include <guilib/alib_x11.h>
#endif // ASYS_DOS

#ifdef ASYS_GLUT
#include <guilib/alib_glut.h>
#endif // ASYS_GLUT

#ifdef ASYS_AMIGA
#include <guilib/alib_amiga.h>
#include <exec/libraries.h>
#include <intuition/intuition.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>
#include <clib/exec_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/wb_protos.h>
#endif // ASYS_AMIGA

#ifdef ASYS_MAC
#ifdef ALIB_NO_PCH
//#include <Windows.h>
//#include <Fonts.h>
#include <Carbon/Carbon.h>
#endif // ALIB_NO_PCH
#include <guilib/alib_carbon.h>
#endif // ASYS_MAC

#ifdef ASYS_SDL
#include <SDL.h>
#endif // ASYS_SDL


#ifdef ASYS_OS2
MRESULT EXPENTRY windowFunc(HWND handle, ULONG mess,
  MPARAM parm1, MPARAM parm2);
#endif // ASYS_OS2

#ifdef ASYS_AMIGA
extern Library *WorkbenchBase;
#endif // ASYS_AMIGA


//#define DEBUG_WINDOW_RESIZES_MOVES 1
#define PEDANTIC_WINDOW_CRAP 1
#define DEBUG_VERBOSE 1


////////////////////////////////////////////////////////////////////////////////
//  AWindowAbstract Class
////////////////////////////////////////////////////////////////////////////////

unsigned int AWindowAbstract::numWindows=0;
bool AWindowAbstract::needToDrawCursor=true;
//AWindowAbstract *AWindowAbstract::defaultWindow=(AWindowAbstract *)NULL;
//ADisplay *AWindowAbstract::defaultDisplay=(ADisplay *)NULL;


AWindowAbstract::AWindowAbstract()
{
  DEBUG_OUT<<"AWindowAbstract()\n";
  init();
  /*
  if(!defaultDisplay) defaultDisplay=new ADisplay;
  parent=defaultDisplay;
  unsigned int newWidth=640,newHeight=480;  // arbitrary
  if(parent) {
    newWidth=parent->getWidth()/2;  newHeight=parent->getHeight()/2;
  }
  openWin((char *)"New Window",newWidth,newHeight);
  */
}


void AWindowAbstract::init()
{
  AFrameAbstract::init();
  x=0;  y=0;
  width=0;  height=0;
  name=(char *)NULL;
  fontHeight=0;
  oldFontHeight=0;
  penX=0;  penY=0;
  oldPenX=0;  oldPenY=0;
  foreground=0;  background=0;
  borderHeight=0;  borderWidth=0;
  avgCharWidth=0;
  sizeCorrected=false;
  moveCorrected=false;
  syncLock=false;
  hexNums=false;
  sigHexDigits=0;
  parent=(ADisplay *)NULL;
#ifdef ALL_WINDOW_OUTPUT_TO_DEBUG
  debugOutput=true;
#else
  debugOutput=false;
#endif // ALL_WINDOW_OUTPUT_TO_DEBUG
  textOutput=false;
  more=false;
  useMore=false;
  moreLineNum=0;  numRows=0;
  scrollRows=0;  normalScrollRows=2;
  haveFocus=false;
  charMapped=false;
  charMap=(char *)NULL;
  mapRows=0;  mapCols=0;
  offscreenImage=NULL;
#ifdef ASYS_WINDOWS
  ahWnd=(HWND)NULLHANDLE;
  ahDC=(HDC)NULLHANDLE;
  textProp=true;
#ifndef USE_DIRECTDRAW
  fixedFont=(HFONT)NULLHANDLE;
  propFont=(HFONT)NULLHANDLE;
  pred=0;  pgreen=0;  pblue=0;
  bred=0;  bgreen=0;  bblue=0;
  hPen=(HPEN)NULLHANDLE;
  backBrush=(HBRUSH)NULLHANDLE;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  ahWnd=(HWND)NULL;  ahWndFrame=(HWND)NULL;
  winDC=(HDC)NULL;
  winHPS=(HPS)NULL;
  bitsDC=(HDC)NULL;
  bitsPS=(HPS)NULL;
#endif // ASYS_OS2
#ifdef ASYS_X11
  TheWin=0;
  TheGC=0;
  xfs=NULL;
#endif // ASYS_X11
#ifdef ASYS_AMIGA
  TheWin=NULL;
  TheRP=NULL;
  appwin=NULL;
  awport=NULL;
  appsignal=0;
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
  TheWP=(CWindowPtr)NULL;
#ifdef USE_QUARTZ
  TheContext=NULL;
#else
  TheGP=(CGrafPtr)NULL;
#endif // USE_QUARTZ
  // FIXME: fcolor, bcolor...
#endif // ASYS_MAC
#ifdef ASYS_PALM
  textProp=true;
  normalScrollRows=1;  // FIXME: a kludge...
  yOffset=0;  // FIXME: a kludge...
  for(unsigned int t=0;t<256;t++) {
    glyphOffset[t]=0;
  }
#endif // ASYS_PALM
}


// FIXME: Figure out border handling better
// FIXME: Figure out default font size better
void AWindowAbstract::openWin(const char *newName,unsigned int newWidth, unsigned int newHeight, int newx, int newy)
{
  if(!parent) { DEBUG_OUT<<"No parent for window!\n"; err.set(); return; }
  if(!newName) newName="Untitled";
  DEBUG_OUT<<"openWin: "<<newName<<","<<newWidth<<","<<newHeight<<"\n";
  width=newWidth;  height=newHeight;
  // if passed zeros for size...
  if(!width) width=parent->getWidth();
  if(!height) height=parent->getHeight();
  // Clip window to display
  if(width>(parent->getWidth())) width=parent->getWidth();
  if(height>(parent->getHeight())) height=parent->getHeight();
  //DEBUG_OUT<<"window size: "<<newName<<","<<newWidth<<","<<newHeight<<"\n";
  // Position window
  int nx,ny;
  if(newx) nx=newx; else nx=(parent->getWidth()/2)-(width/2);
  if(newy) ny=newy; else ny=(parent->getHeight()/2)-(height/2);
  if(nx<0) nx=parent->getWidth()+nx;
  if(ny<0) ny=parent->getHeight()+ny;
  x=nx;  y=ny;
  DEBUG_OUT<<"window loc: "<<x<<","<<y<<"\n";
  penX=0;  penY=0;
  background=0;  foreground=1;
  name=strdup(newName);
  // These are assumed for now...
  borderWidth=0;  borderHeight=0;
  fontHeight=12;  // arbitrary
  if(parent->getHeight()<400) fontHeight=9;  // arbitrary
  avgCharWidth=parent->getAvgCharWidth();
  debugOutput=false; textOutput=true;
  hexNums=false;  sigHexDigits=1;
  more=false;
  numRows=height/fontHeight;
  moreLineNum=0;
  scrollRows=normalScrollRows;
  sizeCorrected=false;
  syncLock=false;
  bool needInitialClear=true;
  bool needInitialColorSet=true;
  bool defaultBlackOnWhite=true;
#ifdef ASYS_DOS
  if(parent->getMouseSupp()) {
    parent->hideMouse();    parent->showMouse();
  }
#endif // ASYS_DOS
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  ahWnd=parent->getDdrawWin();
#else
  if(!alib_ahInst) {
    DEBUG_OUT<<"alib_ahInst is NULL!\n";
    err.set();
    return;
  }
#ifdef ASYS_WIN32CE
  DWORD wstyle=WS_SYSMENU|WS_CAPTION|WS_VISIBLE;
#else
  DWORD wstyle=WS_SYSMENU|WS_CAPTION|WS_VISIBLE|WS_OVERLAPPEDWINDOW;
#endif // ASYS_WIN32CE
#ifdef ASYS_WIN32CE
  x=CW_USEDEFAULT;  y=CW_USEDEFAULT;
  width=CW_USEDEFAULT;  height=CW_USEDEFAULT;
#endif // ASYS_WIN32CE
#ifdef UNICODE
  ahWnd=CreateWindow(L"ALibWClass",aThisApp.toWide(name),
    wstyle,x,y,width,height,(HWND)NULLHANDLE,
    (HMENU)NULLHANDLE,alib_ahInst,(LPSTR)NULL);
#else
  ahWnd=CreateWindow("ALibWClass",name,
    wstyle,x,y,width,height,(HWND)NULLHANDLE,
    (HMENU)NULLHANDLE,alib_ahInst,(LPSTR)NULL);
#endif // UNICODE
  if(ahWnd==(HWND)NULLHANDLE) {
    DEBUG_OUT<<"Couldn't CreateWindow!\n";
    err.set();
    return;
  }
#ifndef ASYS_WIN32CE
  SetWindowPos(ahWnd,(HWND)NULLHANDLE,x,y,width,height,SWP_DRAWFRAME);
#endif // !ASYS_WIN32CE
  ShowWindow(ahWnd,SW_SHOW);
  SetForegroundWindow(ahWnd);
  SetFocus(ahWnd);
  UpdateWindow(ahWnd);
#endif // USE_DIRECTDRAW
  //SetWindowLong(ahWnd,0,(long)this);
  alib_ahWnd=ahWnd;  // For when we need ANY window handle
  ahDC=GetDC(ahWnd);  // TODO: Cache a DC!  Use it!
#ifndef USE_DIRECTDRAW
  // Assume this for now
  fontHeight=13;
  textProp=true;
  pred=0x00;  pgreen=0x00;  pblue=0x00;
  bred=0xff;  bgreen=0xff;  bblue=0xff;
  hPen=(HPEN)NULLHANDLE;  backBrush=(HBRUSH)NULLHANDLE;
#else
  fontHeight=8;
  textProp=false;
#endif // USE_DIRECTDRAW
#ifdef ASYS_WIN32CE
#if CE_MAJOR_VER >2
  if(alib_aygshellHandle) {
    alib_sinfo.cbSize=sizeof(SIPINFO);
    SHSipInfo(SPI_GETSIPINFO,0,&alib_sinfo,0);
  }
#endif
#endif // ASYS_WIN32CE
  // FIXME: How do you get the default background color?!?
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  unsigned long flFlags;
  flFlags=FCF_TITLEBAR|FCF_SIZEBORDER|FCF_MINMAX|FCF_ICON|
    FCF_SYSMENU|FCF_TASKLIST|FCF_NOBYTEALIGN|FCF_MENU;
  if(!WinRegisterClass(aThisApp.ahAB,(PSZ)name,(PFNWP)windowFunc,CS_SIZEREDRAW,0)) {
    DEBUG_OUT<<"Couldn't WinRegisterClass!\n"
    err.set();
    return;
  }
  ahWnd=0;
  // We load menu resource #1
  ahWndFrame=WinCreateStdWindow(HWND_DESKTOP,WS_VISIBLE,&flFlags,
    (PSZ)name,(PSZ)name,WS_VISIBLE,(HMODULE)NULL,1,&ahWnd);
  // FIXME: Check that we actually got our window.
  winDC=WinOpenWindowDC(ahWnd);
  //WinSendMsg(ahWndFrame,WM_SETICON,(void *)
  //  WinQuerySysPointer(HWND_DESKTOP,SPTR_APPICON,false),NULL);
  borderWidth=(unsigned int)(WinQuerySysValue(HWND_DESKTOP,SV_CXSIZEBORDER)*2);
  borderHeight=(unsigned int)(WinQuerySysValue(HWND_DESKTOP,SV_CYSIZEBORDER)*2
    +WinQuerySysValue(HWND_DESKTOP,SV_CYTITLEBAR)
    +WinQuerySysValue(HWND_DESKTOP,SV_CYMENU));
  //borderHeight-=44;  borderWidth-=8;
  //DEBUG_OUT<<"border is: "<<borderWidth<<","<<borderHeight<<"\n";
  winHPS=WinGetPS(ahWnd);
#ifdef ASYS_OS2_32
  DEVOPENSTRUC dop={0L,"DISPLAY",(PDRIVDATA)NULL,0L,0L,0L,0L,0L,0L};
  bitsDC=DevOpenDC(aThisApp.ahAB,OD_MEMORY,"*",
    5L,(PDEVOPENDATA)&dop,(HDC)NULL);
#else
  bitsDC=DevOpenDC(aThisApp.ahAB,OD_MEMORY,(PSZ)"*",
    5L,(PDEVOPENDATA)NULL,(HDC)NULL);
#endif // ASYS_OS2_32
  SIZEL sizl={0,0};
  bitsPS=GpiCreatePS(aThisApp.ahAB,bitsDC,&sizl,GPIA_ASSOC|PU_PELS);
  //GpiSetBackMix(winHPS,BM_OVERPAINT);
  // This isn't really true...
  fontHeight=16;
#endif // ASYS_OS2
#ifdef ASYS_MAC
  // assume these for now
  // FIXME: There must be a better way of doing this.
  borderWidth=14;  borderHeight=14;
  // Kinda a silly way of doing this
  unsigned char macTitle[256];
  char *sname=strdup(name);
  CopyCStringToPascal(sname,macTitle);
  // FIXME: Maybe let other WDEF's be used?
  int macWinProc=documentProc+8;
  Rect windowBox;
  windowBox.top=y;  windowBox.left=x;
  windowBox.right=x+width;  windowBox.bottom=windowBox.top+height;
  bool hasCloseBox=true;
#if TARGET_API_MAC_OSX
  UInt32 attr=0;
  attr|=kWindowStandardDocumentAttributes;
#ifdef USE_CARBON_EVENTS
  attr|=kWindowStandardHandlerAttribute;
#endif // USE_CARBON_EVENTS
  attr|=kWindowLiveResizeAttribute|kWindowAsyncDragAttribute;
#if !__LP64__
  CreateNewWindow(kDocumentWindowClass,attr,&windowBox,&TheWP);
#endif
  CFStringRef theString=CFStringCreateWithBytes(NULL,(const UInt8 *)name,strlen(name),kCFStringEncodingASCII,false);
#if !__LP64__
  SetWindowTitleWithCFString(TheWP,theString);
#endif
  // TODO: Should we release theString here?
#else
  TheWP=(CWindowPtr)NewCWindow(NULL,&windowBox,(ConstStr255Param)macTitle,true,macWinProc,(WindowPtr)(-1L),hasCloseBox,0L);
#endif // TARGET_API_MAC_OSX
  if(!TheWP) { DEBUG_OUT<<"Couldn't NewWindow!\n"; err.set(); return; }
#if TARGET_API_MAC_CARBON
#ifdef USE_QUARTZ
#if !__LP64__
  QDBeginCGContext(GetWindowPort(TheWP),&TheContext);
#endif
  // TODO: Error check TheContext
  //CGContextSetFillColorSpace(TheContext,genericColorSpace);
  //CGContextSetStrokeColorSpace(TheContext,genericColorSpace);
  CGContextSaveGState(TheContext);
  CGContextTranslateCTM(TheContext,0,0);
  CGContextScaleCTM(TheContext,1.0f,-1.0f);
#else
  TheGP=GetWindowPort(TheWP);
  MacSetPort(TheGP);
#endif // USE_QUARTZ
#else
  TheGP=(CGrafPort *)TheWP;
  MacSetPort(TheGP);
#endif // TARGET_API_MAC_CARBON
#ifdef USE_QUARTZ
#else
  TextFont(applFont);  PenNormal();
#endif // USE_QUARTZ
  fontHeight=14;
  // set to what we actually got
#if TARGET_API_MAC_CARBON
#ifdef USE_QUARTZ
#else
  Rect theRect;
  GetPortBounds(TheGP,&theRect);
  width=theRect.right-theRect.left;
  height=theRect.bottom-theRect.top;
#endif // USE_QUARTZ
#else
  width=TheWP->portRect.right-TheWP->portRect.left;
  height=TheWP->portRect.bottom-TheWP->portRect.top;
#endif // TARGET_API_MAC_CARBON
  //EnableScreenUpdates();
#if !__LP64__
  MacShowWindow(TheWP);
  //ActivateWindow(TheWP,true);
  //SelectWindow(TheWP);
  //BringToFront(TheWP);
  //ShowWindow(TheWP);
#endif
#ifdef USE_CARBON_EVENTS
#else
  SetEventMask(everyEvent);
#endif
#ifdef USE_QUARTZ
#else
  TextMode(srcCopy);  PenMode(srcCopy);
#endif // USE_QUARTZ
#ifdef USE_CARBON_EVENTS
  EventHandlerUPP winHandler;
  winHandler=NewEventHandlerUPP(alib_carbWinEventHandler);
  EventTypeSpec theEvents[64];
  int i=0;
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowClose;  i++;
#if !__LP64__
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowUpdate;  i++;
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowHandleContentClick;  i++;
#endif
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowActivated;  i++;
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowResizeCompleted;  i++;
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowDragCompleted;  i++;
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowFocusAcquired;  i++;
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowFocusRelinquish;  i++;
  theEvents[i].eventClass=kEventClassWindow;  theEvents[i].eventKind=kEventWindowBoundsChanged;  i++;
  theEvents[i].eventClass=kEventClassKeyboard;  theEvents[i].eventKind=kEventRawKeyDown;  i++;
  theEvents[i].eventClass=kEventClassKeyboard;  theEvents[i].eventKind=kEventRawKeyUp;  i++;
  theEvents[i].eventClass=kEventClassKeyboard;  theEvents[i].eventKind=kEventRawKeyRepeat;  i++;
  theEvents[i].eventClass=kEventClassMouse;  theEvents[i].eventKind=kEventMouseDown;  i++;
  theEvents[i].eventClass=kEventClassMouse;  theEvents[i].eventKind=kEventMouseUp;  i++;
  theEvents[i].eventClass=kEventClassMouse;  theEvents[i].eventKind=kEventMouseDragged;  i++;
  theEvents[i].eventClass=kEventClassMouse;  theEvents[i].eventKind=kEventMouseMoved;  i++;
  theEvents[i].eventClass=kEventClassApplication;  theEvents[i].eventKind=kEventAppQuit;  i++;
  theEvents[i].eventClass=kEventClassApplication;  theEvents[i].eventKind=kEventAppAvailableWindowBoundsChanged;  i++;
  theEvents[i].eventClass=kEventClassCommand;  theEvents[i].eventKind=kEventCommandProcess;  i++;
  theEvents[i].eventClass=kEventClassAppleEvent;  theEvents[i].eventKind=kEventAppleEvent;  i++;
  EventTargetRef theTarget;
#if !__LP64__
  theTarget=GetWindowEventTarget(TheWP);
#endif
  InstallEventHandler(theTarget,winHandler,i,theEvents,NULL,NULL);
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  TheWin=NULL;  TheRP=NULL;
  // AppWindow support
  awport=NULL;  appwin=NULL;  appsignal=0;
  // FIXME: specify NewLook menus under 3.x...
  ULONG iFlags=IDCMP_CLOSEWINDOW|IDCMP_NEWSIZE|IDCMP_CHANGEWINDOW|
    IDCMP_REFRESHWINDOW|IDCMP_MOUSEBUTTONS|IDCMP_VANILLAKEY|IDCMP_RAWKEY;
   ULONG flags=WINDOWSIZING|WINDOWDRAG|WINDOWDEPTH|WINDOWCLOSE|
    ACTIVATE|GIMMEZEROZERO;
  if(!parent->getScreen()) {
    DEBUG_OUT<<"Parent doens't have a screen!\n";
    err.set(); return;
  }
  if(aThisApp.version>35) {
    DEBUG_OUT<<"(Going to OpenWindowTags...)\n";
    TheWin=OpenWindowTags(NULL,WA_CloseGadget,true,
      WA_Title,(ULONG)name,WA_GimmeZeroZero,true,WA_ScreenTitle,(ULONG)name,
      WA_SizeGadget,true,WA_DragBar,true,WA_DepthGadget,true,
      WA_Activate,true,WA_AutoAdjust,true,
      WA_Left,x,WA_Top,y,WA_Width,width,WA_Height,height,
      WA_MinWidth,50,WA_MinHeight,50,WA_MaxWidth,~0,WA_MaxHeight,~0,
      WA_CustomScreen,(ULONG)parent->getScreen(),WA_IDCMP,iFlags,
      TAG_DONE);
  }
  else {
    x=0;  y=0;
    struct NewWindow nw;
    nw.LeftEdge=x;  nw.TopEdge=y;  nw.Width=width;  nw.Height=height;
    nw.DetailPen=1;  nw.BlockPen=2; nw.Title=(UBYTE *)name;
    nw.IDCMPFlags=iFlags;  nw.Flags=flags;
    nw.FirstGadget=NULL; nw.BitMap=NULL; nw.CheckMark=NULL;
    nw.Screen=parent->getScreen(); nw.Type=CUSTOMSCREEN;
    nw.MinWidth=50; nw.MinHeight=50; nw.MaxWidth=~0; nw.MaxHeight=~0;
    DEBUG_OUT<<"Going to OpenWindow...)\n";
    TheWin=OpenWindow(&nw);
  }
  if(!TheWin) {
    DEBUG_OUT<<"Amiga window failed to open!\n"; err.set(); return;
  }
  else DEBUG_OUT<<"Got TheWin!\n";
  TheRP=TheWin->RPort;
  if(aThisApp.version>36) {
    DEBUG_OUT<<"Going to try to AddAppWindowA...\n";
    awport=CreateMsgPort();
    if(awport) appwin=AddAppWindowA(1,(ULONG)TheWin,TheWin,awport,NULL);
    if(appwin) appsignal=1L<<awport->mp_SigBit;
  }
  setForeground(1);
  ScreenToFront(parent->getScreen());
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  Display *theDisplay=parent->getDisplay();
  XSetWindowAttributes attr;
  unsigned long mask=0;
  Visual *vis=NULL;
  unsigned int theDepth=0;
#ifdef USE_OPENGL
  vis=parent->getVisInfo()->visual;
  attr.background_pixel=0;
  attr.border_pixel=0;
  attr.colormap=XCreateColormap(theDisplay,RootWindow(theDisplay,DefaultScreen(theDisplay)),vis,AllocNone);
  mask=CWBackPixel|CWBorderPixel|CWColormap;
  theDepth=parent->getVisInfo()->depth;
#else
  attr.border_pixel=BlackPixel(theDisplay,0);
  attr.background_pixel=WhitePixel(theDisplay,0);
  attr.backing_store=Always;
  mask=CWBackPixel|CWBorderPixel|CWBackingStore;
  if(parent->getCMap()!=(Colormap)0) {
    attr.colormap=parent->getCMap();
    mask|=CWColormap;
  }
  vis=CopyFromParent;
  theDepth=parent->getDepth();
#endif
  DEBUG_OUT<<"Going to XCreateWindow...\n";
  int blackColor=BlackPixel(theDisplay,DefaultScreen(theDisplay));
  //TheWin=XCreateWindow(theDisplay,RootWindow(theDisplay,0),x,y,width,height,0,0,CopyFromParent,CopyFromParent,mask,&attr);
  TheWin=XCreateSimpleWindow(theDisplay,RootWindow(theDisplay,0),x,y,width,height,0,blackColor,blackColor);
  XWMHints wmhints;
  wmhints.flags=StateHint;
  wmhints.initial_state=NormalState;
  DEBUG_OUT<<"Going to XSetWMHints...\n";
  //XSetWMHints(theDisplay,TheWin,&wmhints);
  XSizeHints hints;
  hints.flags=USPosition|USSize;
  hints.x=x;  hints.y=y;
  hints.width=width;  hints.height=height;
  hints.base_width=width;  hints.base_height=height;
  DEBUG_OUT<<"Going to XSetNormalHints...\n";
  //XSetNormalHints(theDisplay,TheWin,&hints);
  char *st=name;
  XTextProperty tp;
  XStringListToTextProperty(&st,1,&tp);
  //int fudgedArgc=aThisApp.argc;
  //if(fudgedArgc>2) fudgedArgc=2;
  //DEBUG_OUT<<"Going to XSetWMProperties...\n";
  //XSetWMProperties(theDisplay,TheWin,&tp,&tp,aThisApp.argv,fudgedArgc,&hints,&wmhints,(XClassHint *)NULL);
  DEBUG_OUT<<"Going to XSelectInput...\n";
  XSelectInput(theDisplay,TheWin,ButtonPressMask|StructureNotifyMask|KeyPressMask|ButtonReleaseMask|FocusChangeMask|PointerMotionMask);
  unsigned long gcmask=GCForeground|GCBackground|GCFunction;
  XGCValues gcvals;
  gcvals.foreground=BlackPixel(theDisplay,0);
  gcvals.background=WhitePixel(theDisplay,0);
  gcvals.function=GXcopy;
  DEBUG_OUT<<"Going to XCreateGC...\n";
  TheGC=XCreateGC(theDisplay,TheWin,gcmask,&gcvals);
  DEBUG_OUT<<"Going to XSetGraphicsExposures...\n";
  //XSetGraphicsExposures(theDisplay,TheGC,false);
  DEBUG_OUT<<"Going to XMapWindow...\n";
  XMapWindow(theDisplay,TheWin);
  for(;;) {
    XEvent e;
    XNextEvent(theDisplay,&e);
    if(e.type==MapNotify) break;
  }
  //DEBUG_OUT<<"Going to XSync...\n";
  XSync(parent->getDisplay(),false);
  // Assume these for now
  fontHeight=14;
  borderWidth=8; borderHeight=16;
  //waitForFirstFocus();
#endif // ASYS_X11
#ifdef ASYS_PALM
  fontHeight=FntCharHeight();
  avgCharWidth=FntAverageCharWidth();
  background=1;  foreground=0;
#endif // ASYS_PALM
#ifdef ASYS_SDL
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
  if(!SDL_SetVideoMode(1440,900,0,SDL_OPENGL)) {
    DEBUG_OUT<<"SDL_SetVideoMode returned error!\n"; err.set(); exit(5);
  }
  SDL_WM_SetCaption(name,name);
#endif // ASYS_SDL
#ifdef ASYS_GLUT
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(65.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,0.0);  /* viewing transform  */
#endif // ASYS_GLUT
  numRows=height/fontHeight;
  focus();
#ifdef ASYS_WINDOWS
  sync();
#endif // ASYS_WINDOWS
#ifdef DEBUG_VERBOSE
   DEBUG_OUT<<"Window border is "<<borderWidth<<","<<borderHeight<<"\n";
#endif // DEBUG_VERBOSE
  // This is because resize might change x,y
  unsigned int tx=x,ty=y,twidth=width,theight=height;
  resize(twidth,theight);
  move(tx,ty);
  sync();
  //aThisApp.lastWindow=this;
  //parent->addWindow(this);
  depth=parent->getDepth();
  nPlanes=parent->getNPlanes();
  calc();
  if(!boundsValid()) {
    DEBUG_OUT<<"Window open, boundsValid failed!\n";
  }
  //setCharMapping(true);
  if(needInitialColorSet) {
    if(defaultBlackOnWhite) {
      setForeground(0x000000L);
      setBackground(0xffffffL);
    }
    else {
      setForeground(0xffffffL);
      setBackground(0x000000L);
    }
  }
  if(needInitialClear) clear();
 // aDefaultAWindow=this;
#ifdef DEBUG_VERBOSE
  //print();
#endif // DEBUG_VERBOSE
}


AWindowAbstract::~AWindowAbstract()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"~AWindowAbstract()\n";
#endif // DEBUG_VERBOSE
  //boundsValid();
  //newline();
  //clear();
  //parent->removeWindow(this);
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  if(fixedFont) { DeleteObject(fixedFont);  fixedFont=NULL; }
  if(propFont) { DeleteObject(propFont);  propFont=NULL; }
  if(ahDC) { ReleaseDC(ahWnd,ahDC); ahDC=NULL; }
  if(ahWnd) { DestroyWindow(ahWnd); ahWnd=NULL; }
#endif // USE_DIRECTDRAW
  ahDC=(HDC)NULLHANDLE;
  ahWnd=(HWND)NULLHANDLE;
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
  //CGContextRestoreGState(TheContext);
  //CGContextSynchronize(TheContext);
  //QDEndCGContext(GetWindowPort(TheWP),&TheContext);
#endif // USE_QUARTZ
  if(TheWP) {
    //DisposeWindow(TheWP);
  }
  TheWP=(CWindowPtr)NULL;
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  if(TheWin) {
    if(appwin) RemoveAppWindow(appwin);
    if(awport) {
      AppMessage *amsg;
      // Reply to any pending app messages
      amsg=(AppMessage *)GetMsg(awport);
      while(amsg) {
        ReplyMsg((Message *)amsg);
        amsg=(AppMessage *)GetMsg(awport);
      }
      DeleteMsgPort(awport);
    }
    CloseWindow((Window *)TheWin);
  }
  TheWin=NULL;  TheRP=NULL;  awport=NULL;  appwin=NULL;  appsignal=0;
#endif // ASYS_AMIGA
#ifdef ASYS_OS2
  GpiDestroyPS(bitsPS);
  DevCloseDC(bitsDC);
  WinReleasePS(winHPS);
  if(ahWndFrame) WinDestroyWindow(ahWndFrame);
  //if(ahWnd) WinDestroyWindow(ahWnd);
  ahWnd=(HWND)NULL;  ahWndFrame=(HWND)NULL;
#endif // ASYS_OS2
#ifdef ASYS_X11
  //XSync(parent->getDisplay(),false);
  DEBUG_OUT<<"Going to XFreeGC...\n";
  if(TheGC) XFreeGC(parent->getDisplay(),TheGC);
  else DEBUG_OUT<<"(no TheGC)\n";
  if(TheWin) {
    //DEBUG_OUT<<"Going to XUnmapWindow...\n";
    //XUnmapWindow(parent->getDisplay(),TheWin);
    //XSync(parent->getDisplay(),false);
    DEBUG_OUT<<"Going to XDestroyWindow...\n";
    XDestroyWindow(parent->getDisplay(),TheWin);
    XSync(parent->getDisplay(),false);
  }
  else DEBUG_OUT<<"(no TheWin)\n";
  TheWin=0;  TheGC=0;
  DEBUG_OUT<<"Going to XFreeFont...\n";
  if(xfs) XFreeFont(parent->getDisplay(),xfs);
  else DEBUG_OUT<<"(no xfs)\n";
  xfs=NULL;
#endif // ASYS_X11
  parent=(ADisplay *)NULL;
  if(name) { free(name); name=NULL; }
}


// FIXME: Clear to background color, not just an aribtrary one
void  AWindowAbstract::clear(bool preserveMap)
{
#ifdef ASYS_DOS
  clear(0,0,width,height);
#endif // ASYS_DOS
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  clear(0,0,width,height);
#else
  //drawBackground();
  clear(0,0,width,height);
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
#if TARGET_API_CARBON
  Rect theRect;
  GetPortBounds(TheGP,&theRect);
  EraseRect(&theRect);
#else
  EraseRect(&TheGP->portRect);
#endif
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  SetRast(TheRP,background);
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  XSetWindowBackground(parent->getDisplay(),TheWin,background);
  XClearWindow(parent->getDisplay(),TheWin);
  //XSync(parent->getDisplay(),false);
#endif // ASYS_X11
#ifdef ASYS_OS2
  clear(0,0,width,height);
#endif // ASYS_OS2
#ifdef ASYS_PALM
  clear(0,0,width,height);
#endif // ASYS_PALM
#ifdef ASYS_POSIX
#ifdef USE_ANSI_ON_STDIO
  printf("\033[0;0f\033[0J");
#endif // USE_ANSI_ON_STDIO
#endif // ASYS_POSIX
#ifdef ASYS_GLUT
  DEBUG_OUT<<"glClear()\n";
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif // ASYS_GLUT
  penX=0;  penY=0;
  //if(!preserveMap) { if(charMapped) clearCharMap(); }
}


// FIXME: rwidth and rheight ARE width and height, NOT relative coords!
void AWindowAbstract::clear(unsigned int rx,unsigned int ry,
  unsigned int rwidth, unsigned int rheight)
{
#ifdef ASYS_AMIGA
  SetAPen(TheRP,background);
  RectFill(TheRP,rx,ry,rx+rwidth,ry+rheight);
  SetAPen(TheRP,foreground);
#endif // ASYS_AMIGA
#ifdef ASYS_DOS
  if(!parent) return;
  if(!parent->getTheBits()) return;
  if(haveFocus) parent->getTheBits()->clear(rx,ry,rwidth,rheight);
#endif // ASYS_MAC
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
#endif // USE_QUARTZ
  Rect r;
  r.top=ry;  r.left=rx;  r.right=rx+rwidth+1;  r.bottom=ry+rheight+1;
#ifdef USE_QUARTZ
#else
  EraseRect(&r);
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_X11
  XSetWindowBackground(parent->getDisplay(),TheWin,background);
  XClearArea(parent->getDisplay(),TheWin,rx,ry,rwidth+1,rheight+1,false);
#endif // ASYS_X11
#ifdef ASYS_OS2
  //GpiErase(winHPS);
  unsigned long tfore=foreground;
  setForeground(background);
  POINTL coords;
  coords.x=rx;  coords.y=ry;
  GpiMove(winHPS,&coords);  coords.x=rwidth;  coords.y=rheight;
  GpiBox(winHPS,DRO_FILL,&coords,0,0);
  setForeground(tfore);
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(offscreenImage) offscreenImage->clear(rx,ry,rwidth,rheight);
#else
  HDC hdc=GetDC(ahWnd);
  //SetBkColor(hdc,RGB(bred,bgreen,bblue));
  SelectObject(hdc,backBrush);
  RECT r;
  r.left=rx;  r.top=ry;
  r.right=rx+rwidth;  r.bottom=ry+rheight;
  FillRect(hdc,&r,backBrush);
  ReleaseDC(ahWnd,hdc);
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_POSIX
#ifdef USE_ANSI_ON_STDIO
  unsigned int tpenX=penX+avgCharWidth;
  unsigned int tx=tpenX,ty=penY;
  moveTo(rx,ry);
  unsigned int tw=rwidth>>3,th=rheight>>3;
  th++;  // FIXME: why?
  char spaces[81];
  unsigned int t;
  for(t=0;t<80;t++) spaces[t]=' ';
  spaces[tw]=0;
  for(t=0;t<th;t++) {
    *this<<spaces;
    moveTo(rx,ry+(t*8));
  }
  moveTo(tx,ty);
#endif // USE_ANSI_ON_STDIO
#endif // ASYS_POSIX
#ifdef ASYS_PALM
  RectangleType r;
  r.topLeft.x=rx;  r.topLeft.y=ry+yOffset;
  r.extent.x=rwidth;  r.extent.y=rheight;
  if(background&1) WinEraseRectangle(&r,0);
  else WinDrawRectangle(&r,0);
#endif // ASYS_PALM
}


AWindowAbstract& AWindowAbstract::hex()
{
  hexNums=true;  return *this;
}


AWindowAbstract& AWindowAbstract::dec()
{
  hexNums=false;  return *this;
}


AWindowAbstract& AWindowAbstract::operator<<(double c)
{
  sprintf(strBuf,ATEXT("%g"),c);
  *this<<strBuf;
  return *this;
}


// FIXME: Need to derive from streams somehow...instead of this kludge
AWindowAbstract& AWindowAbstract::operator<<(unsigned long c)
{
  if(hexNums) sprintf(strBuf,ATEXT("%08lx"),c);
  else sprintf(strBuf,ATEXT("%ld"),c);
  *this<<strBuf;
  return *this;
}


// FIXME: Need to derive from streams somehow...instead of this kludge
AWindowAbstract& AWindowAbstract::operator<<(long c)
{
  if(hexNums) sprintf(strBuf,ATEXT("%08lx"),c);
  else sprintf(strBuf,ATEXT("%ld"),c);
  *this<<strBuf;
  return *this;
}


// FIXME: Need to derive from streams somehow...instead of this kludge
AWindowAbstract& AWindowAbstract::operator<<(unsigned int c)
{
  if(hexNums) {
    char *format=(char *)NULL;
    switch(sigHexDigits) {
      case 2: format=(char *)"%02x"; break;
      case 4: format=(char *)"%04x"; break;
      case 6: format=(char *)"%06x"; break;
      case 8: format=(char *)"%08x"; break;
      default: format=(char *)"%x"; break;
    }
    sprintf(strBuf,format,c);
  }
  else sprintf(strBuf,(char *)"%d",c);
  *this<<strBuf;
  return *this;
}


// FIXME: Need to derive from streams somehow...instead of this kludge
AWindowAbstract& AWindowAbstract::operator<<(char c)
{
  if(c==A_KEY_DELETE) {
    // FIXME: This doesn't work well with prop fonts...
    penX-=avgCharWidth;
    *this<<(char *)" ";
    penX-=avgCharWidth;
    return *this;
  }
  if(c=='\n') { newline();  return *this; }
  if(c==13) { newline();  return *this; }
  if(c=='\t') {
    operator<<(' ');
    c=' ';
  }
  // if(c=='\r') { newline();  return *this; }
  strBuf[0]=c; strBuf[1]=0; *this<<strBuf;
  return *this;
}


// FIXME: Need to derive from streams somehow...instead of this kludge
AWindowAbstract& AWindowAbstract::operator<<(int c)
{
  return *this<<(unsigned int)c;
}


AWindowAbstract& AWindowAbstract::operator<<(const char *sstr)
{
  if(!sstr) return *this;
  if(debugOutput) DEBUG_OUT<<sstr;
  if(!textOutput) return *this;
  char *str2=strdup(sstr),*str=str2;
  unsigned int len=strlen(str),t=0,tt=0;
  bool nl;
  while(t<len) {
    nl=false;
    tt=t;
    while(tt<len) {
      if(str[tt]=='\r')  { str[tt]=' '; }  // FIXME: Not really...
      if(str[tt]=='\t')  {
        operator<<(' ');
        str[tt]=' ';
      }
      if(str[tt]=='\n') {
        str[tt]=0;
        nl=true;
        tt=len;
      }
      tt++;
    }
    drawText(str+t,strlen(str+t));
    if(nl) { newline(); }
    t+=strlen(str+t)+1;
  }
  //free(str2);
  return *this;
}


void AWindowAbstract::drawText(const char *str,unsigned int len,bool remember)
{
  if(!str) return;
  if(!len) return;
#ifdef ASYS_DOS
  //if(!haveFocus) return;
#endif // ASYS_DOS
  unsigned int strWidth=howWideIsThis(str,len);
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  HDC hdc=GetDC(ahWnd);
  SetTextColor(hdc,RGB(pred,pgreen,pblue));
  SetBkColor(hdc,RGB(bred,bgreen,bblue));
  // Some primative font handling...
  if(!propFont) {
    //propFont=(HFONT)GetStockObject(SYSTEM_FONT);
    LOGFONT lFont;
    lFont.lfCharSet=DEFAULT_CHARSET;
    lFont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
    lFont.lfEscapement=0;
#ifdef UNICODE
    wcscpy(lFont.lfFaceName,L"Helvetica");
#else
    strcpy(lFont.lfFaceName,"Helvetica");
#endif // UNICODE
    lFont.lfHeight=fontHeight;
    lFont.lfItalic=false;
    lFont.lfOrientation=0;
    lFont.lfOutPrecision=OUT_DEFAULT_PRECIS;
    lFont.lfPitchAndFamily=FF_DONTCARE|VARIABLE_PITCH;
    lFont.lfQuality=DEFAULT_QUALITY;
    lFont.lfStrikeOut=false;
    lFont.lfUnderline=false;
    lFont.lfWeight=FW_DONTCARE;
    lFont.lfWidth=0;
    propFont=CreateFontIndirect(&lFont);
  }
  if(!fixedFont) {
    LOGFONT lFont;
    lFont.lfCharSet=DEFAULT_CHARSET;
    lFont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
    lFont.lfEscapement=0;
#ifdef UNICODE
    wcscpy(lFont.lfFaceName,L"Courier");
#else
    strcpy(lFont.lfFaceName,"Courier");
#endif // UNICODE
    lFont.lfHeight=fontHeight;
    lFont.lfItalic=false;
    lFont.lfOrientation=0;
    lFont.lfOutPrecision=OUT_DEFAULT_PRECIS;
    lFont.lfPitchAndFamily=FF_DONTCARE|FIXED_PITCH;
    lFont.lfQuality=DEFAULT_QUALITY;
    lFont.lfStrikeOut=false;
    lFont.lfUnderline=false;
    lFont.lfWeight=FW_DONTCARE;
    lFont.lfWidth=0;
    fixedFont=CreateFontIndirect(&lFont);
  }
  if(textProp) {
    if(propFont) SelectObject(hdc,propFont);
  }
  else {
    if(fixedFont) SelectObject(hdc,fixedFont);
  }
#ifdef ASYS_WIN32CE
  ExtTextOut(hdc,penX,penY,0,NULL,aThisApp.toWide(str),len,NULL);
#else
  TextOut(hdc,penX,penY,str,len);
#endif // ASYS_WIN32CE
#ifdef ASYS_WIN16
  if((aThisApp.version==3)&&(!aThisApp.versionMinor)) {
    DWORD mySize;
    mySize=GetTextExtent(hdc,str,len);
    strWidth=LOWORD(mySize);
  }
  else {
#endif // ASYS_WIN16
#ifndef REALLY_OLD_WINDOWS
    SIZE mySize;
#ifdef ASYS_WIN32CE
    GetTextExtentExPoint(hdc,aThisApp.toWide(str),len,NULL,NULL,NULL,&mySize);
#else
    GetTextExtentPoint(hdc,str,len,&mySize);
#endif // ASYS_WIN32CE
    strWidth=mySize.cx;
#endif // REALLY_OLD_WINDOWS
#ifdef ASYS_WIN16
  }
#endif // ASYS_WIN16
  ReleaseDC(ahWnd,hdc);
#else
  unsigned int tx=penX;
  for(unsigned int t=0;t<len;t++) {
    drawImageChar(str[t]);
    penX+=8;  // FIXME: don't assume width
  }
  penX=tx;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  POINTL coords;
  short tpy=height-penY-borderHeight-fontHeight;
  coords.x=penX;  coords.y=tpy;
  //GpiSetBackMix(winHPS,BM_OVERPAINT);
  GpiCharStringAt(winHPS,&coords,len,(PCH)str);
#endif // ASYS_OS2
#ifdef ASYS_AMIGA
  if(!TheRP) return;
  Move(TheRP,penX,penY+fontHeight); Text(TheRP,str,len);
#endif // ASYS_AMIGA
#ifdef ASYS_DOS
  /*
  if(len!=1) {  // FIXME: to avoid runaway recursion...
    if(strWidth>width) {
      drawTextLetterByLetter(str);
      free(str);  // FIXME: is this right?
      return;
    }
  }
  */
  if(parent->getBiosSupp()) {
    unsigned long theColor=foreground;
    // FIXME: Take background into account
    REGS16 rin,rout;
    unsigned int theCol=(penX+x)/avgCharWidth;
    unsigned int theRow=(penY+y)/fontHeight;
    for(unsigned int tt=0;tt<len;tt++) {
      if(str[tt]>29) {
        rin.h.ah=0x2;  rin.h.bh=0;
        rin.h.dh=(char)theRow;   rin.h.dl=(char)(theCol+tt);
        int86(0x10,&rin,&rout);
        rin.h.ah=0x9;  rin.h.bh=0; rin.h.bl=(char)theColor;
        rin.h.al=str[tt];    RIN_W_CX=1;
        int86(0x10,&rin,&rout);
      }
    }
  }
  else  {
    unsigned int tx=penX;
    for(unsigned int t=0;t<len;t++) {
      drawImageChar(str[t]);
      penX+=8;
    }
    penX=tx;
  }
#endif // ASYS_DOS
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
#endif // USE_QUARTZ
  setForeground(foreground);  setBackground(background);
#ifdef USE_QUARTZ
#else
  MoveTo(penX,penY+fontHeight);
  //char *sstr=awstrdups(str);
  MacDrawText(str,0,len);
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_X11
  XDrawImageString(parent->getDisplay(),TheWin,TheGC,penX,penY+fontHeight-1,str,len);
#endif // ASYS_X11
#ifdef ASYS_POSIX
  unsigned int tpenX=penX+avgCharWidth;
#ifdef USE_ANSI_ON_STDIO
  printf("\033[%d;%df",penY>>3,tpenX>>3);
#endif // USE_ANSI_ON_STDIO
  //DEBUG_OUT<<"text "<<penX<<","<<penY<<":";
  for(unsigned int t=0;t<len;t++) {
    printf("%c",str[t]);
    //DEBUG_OUT<<str[t];
  }
  //DEBUG_OUT<<"\n";
#endif // ASYS_POSIX
#ifdef ASYS_PALM
  if(textProp) {
    if(foreground&1) WinEraseChars(str,len,penX,penY+yOffset);
    else WinDrawChars(str,len,penX,penY+yOffset);
  }
  else {
    unsigned int q=0,fix=0;
    for(unsigned int t=0;t<len;t++) {
      fix=glyphOffset[str[t]];
      if(foreground&1) WinEraseChars(&(str[t]),1,penX+q+fix,penY+yOffset);
      else WinDrawChars(&(str[t]),1,penX+q+fix,penY+yOffset);
      q+=avgCharWidth;
    }
  }
#endif // ASYS_PALM
  if(remember&&charMapped) {
    unsigned int offset=((penY/fontHeight)*mapCols)+(penX/avgCharWidth);
    for(unsigned int t=0;t<len;t++) {
      if(offset<mapSize) {
        charMap[offset]=str[t];
        //DEBUG_OUT<<str[t]<<" offset is "<<offset<<"\n";
      }
      offset++;
    }
  }
  penX+=strWidth;
  if(penX>width) { penX=0; newline(); }
  return;
}


// FIXME: This "more" crap is still under construction...
// FIXME: Shouldn't the clearing of the scrolled-out-of area be in scroll()?
AWindowAbstract& AWindowAbstract::newline()
{
  //if(debugOutput) DEBUG_OUT<<"\n";
  if(!textOutput) return *this;
#ifdef ASYS_DOS
  //if(!haveFocus) return *this;
#endif // ASYS_DOS
#ifdef ASYS_POSIX
  printf("\n");
#endif // ASYS_POSIX
  penX=0;
  if(useMore) {
          if(more) {
            scrollRows=2;
            moreLineNum++;
            if((moreLineNum+6)>numRows) {
              moreLineNum=0;
              *this<<(char *)"\n"<<(char *)"[more]";
              AEvent e;
              bool working=true;
              while(working) {
                switch(e.type) {
                  case A_EVENT_KEYUP:
                    working=false;
                    // FIXME: somehow let app know user wants to stop...
                    //if(e.code==A_KEY_ESC)
                    break;
                  case A_EVENT_MOUSEUP:
                    working=false;
                    break;
                  case A_EVENT_DESTROY:
                  case A_EVENT_BREAK:
                    working=false;
                    break;
                  default: e.wait(this); break;
                }
              }
              *this<<(char *)"\n";
            }
          }
  else scrollRows=normalScrollRows;
  }
  unsigned int theight=height-borderHeight;
  bool scrolled=false;
  if(penY>(theight-(2*fontHeight))) {
    scrolled=true;
    scroll(0,-((int)(fontHeight*scrollRows)));
    penY-=fontHeight*(scrollRows-1);
    clear(0,height-(fontHeight*scrollRows),width,
      fontHeight*scrollRows);
      /*
#ifdef ASYS_POSIX
    clear(0,height-(fontHeight*scrollRows),width,fontHeight*scrollRows);
    //DEBUG_OUT<<penX<<" "<<penY<<" newline\n";
#endif // ASYS_POSIX
#ifdef ASYS_DOS
    parent->getTheBits()->clear(0,height-(fontHeight*scrollRows),width,
      fontHeight*scrollRows);
#endif // ASYS_DOS
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
    if(offscreenImage) offscreenImage->clear(0,height-(fontHeight*scrollRows),width,fontHeight*scrollRows);
#else
    HDC hdc=GetDC(ahWnd);
    RECT rect;
    rect.left=0;  rect.top=theight-(fontHeight*scrollRows);
    rect.right=width;  rect.bottom=theight;
    FillRect(hdc,&rect,backBrush);
    ReleaseDC(ahWnd,hdc);
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_PALM
    clear(0,height-(fontHeight*scrollRows),width,
      fontHeight*scrollRows);
#endif // ASYS_PALM
#ifdef ASYS_X11
    XClearArea(parent->getDisplay(),TheWin,0,height-(fontHeight*scrollRows),
      width,fontHeight*scrollRows,false);
#endif // ASYS_X11
#ifdef ASYS_OS2
    unsigned long tfore=foreground;
    setForeground(background);
    POINTL coords;
    coords.x=0;  coords.y=0;
    GpiMove(winHPS,&coords);  coords.x=width;  coords.y=fontHeight*scrollRows-1;
    GpiBox(winHPS,DRO_FILL,&coords,0,0);
    setForeground(tfore);
#endif // ASYS_OS2
  */
#ifdef ASYS_MAC
#if !__LP64__
    DrawGrowIcon(TheWP);
#endif
#endif // ASYS_MAC
  }
  else penY+=fontHeight;
  if(charMapped&&scrolled) {
    unsigned int moveRows=mapRows-scrollRows;
    unsigned int yy,xx;
    unsigned int soffset,doffset;
    for(yy=0;yy<moveRows;yy++) {
      for(xx=0;xx<mapCols;xx++) {
        doffset=yy*mapCols+xx;
        soffset=(yy+scrollRows)*mapCols+xx;
        if((soffset<mapSize)&&(doffset<mapSize))
          charMap[doffset]=charMap[soffset];
      }
    }
    for(yy=0;yy<scrollRows;yy++) {
      for(xx=0;xx<mapCols;xx++) {
        doffset=(yy+moveRows)*mapCols+xx;
        if(doffset<mapSize) charMap[doffset]=' ';
      }
    }
  }
  return *this;
}


unsigned int AWindowAbstract::countLines(const char *str)
{
  if(!str) return 0;
  unsigned int lret=0;
  unsigned int len=strlen(str);
  for(unsigned int t=0;t<len;t++) {
    // FIXME: This won't work right on dos text...
    if(str[t]=='\n') lret++;
    //if(str[t]=='\r') lret++;
  }
  return lret;
}


void AWindowAbstract::scroll(int dx, int dy)
{
  unsigned int tx=0,ty=0,tw=width,th=height;
#ifdef ASYS_MAC
  RgnHandle update_handle;
  Rect r;
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
  update_handle=NewRgn();
  MacSetRect(&r,tx,ty,tx+tw,ty+th);
  ScrollRect(&r,dx,dy,update_handle);
  DisposeRgn(update_handle);
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  ScrollRaster(TheRP,-dx,-dy,tx,ty,tx+tw,ty+th);
#endif // ASYS_AMIGA
#ifdef ASYS_DOS
  if(!parent) return;
  if(!parent->getTheBits()) return;
  if(haveFocus) parent->getTheBits()->scroll(tx+x,ty+y,tw,th,dx,dy);
#endif //. ASYS_DOS
#ifdef ASYS_X11
  XCopyArea(parent->getDisplay(),TheWin,TheWin,TheGC,tx,ty,tw,th,dx,dy);
  //XSync(parent->getDisplay(),false);  // FIXME: Slows everything down terribly..
#endif // ASYS_X11
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(offscreenImage) offscreenImage->scroll(tx+x,ty+y,tw,th,dx,dy);
#else
  th-=borderHeight+1;
  //ty-=1;
  HDC hdc=GetDC(ahWnd);
  BitBlt(hdc,tx,ty,tw+dx,th+dy,hdc,-dx,-dy,SRCCOPY);
  ReleaseDC(ahWnd,hdc);
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  // FIXME: This only works the way newline wants it to...it isn't general yet.
  POINTL point[3];
  point[0].x=tx+dx;  point[0].y=ty-dy;
  point[1].x=tw;  point[1].y=th;
  point[2].x=tx;  point[2].y=ty;
  ULONG bitRes=GpiBitBlt(winHPS,winHPS,3L,point,ROP_SRCCOPY,BBO_IGNORE);
  if(bitRes!=GPI_OK) DEBUG_OUT<<"GpiBitBlt failed!\n";
#endif // ASYS_OS2
#ifdef ASYS_POSIX
  //redraw();
  // FIXME: see newline on how to do this...
#endif // ASYS_POSIX
#ifdef ASYS_PALM
  RectangleType r;
  r.topLeft.x=tx;  r.topLeft.y=ty+yOffset;
  r.extent.x=tw;  r.extent.y=th;
  RectangleType v;
#ifdef __PALMOS_H__
  WinDirectionType dir;
#else
  DirectionType dir;
#endif // __PALMOS_H__
  if(dx) {
#ifdef __PALMOS_H__
    if(dx>0) dir=winLeft; else dir=winRight;
#else
    if(dx>0) dir=left; else dir=right;
#endif // __PALMOS_H__
    WinScrollRectangle(&r,dir,dx,&v);
  }
  if(dy) {
#ifdef __PALMOS_H__
    if(dy>0) dir=winUp; else dir=winDown;
#else
    if(dy>0) dir=up; else dir=down;
#endif // __PALMOS_H__
    WinScrollRectangle(&r,dir,dy,&v);
  }
#endif // ASYS_PALM
  if(charMapped) {
    // FIXME: update char map?
    // NOTE: see newline()
  }
}


unsigned int AWindowAbstract::howWideIsThis(const char *str,unsigned int len)
{
  // FIXME: font is assumed to be 8 pixels wide...this is usually wrong!
  // FIXME: Need to actually implement this on more than just the mac...
  unsigned int ret=len*avgCharWidth;
#ifdef ASYS_AMIGA
  ret=TextLength(TheRP,str,len);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  ret=TextWidth(str,0,len);
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_X11
  if(xfs) {
    ret=XTextWidth(xfs,str,len);
  }
  else ret=len*6;  // NOTE: Arbitrary guess...
#endif // ASYS_X11
#ifdef ASYS_PALM
  if(textProp) ret=FntCharsWidth(str,len);
  else ret=len*avgCharWidth;
#endif // ASYS_PALM
  return ret;
}


AWindowAbstract& AWindowAbstract::center(const char *str)
{
  if(!str) return *this;
  unsigned int strPixelWidth=howWideIsThis(str,strlen(str));
  penX=(width/2)-(strPixelWidth/2);
  *this<<str;
  return *this;
}


AWindowAbstract& AWindowAbstract::center(const char *str,unsigned int yCoord)
{
  if(!str) return *this;
  penY=yCoord;
  center(str);
  return *this;
}


// FIXME: This should be inline
AWindowAbstract& AWindowAbstract::operator<<(APrivateBitmap *someImage)
{
  this->draw(someImage,penX,penY);
  return *this;
}



void AWindowAbstract::copyBlock(APrivateBitmap *someImage, unsigned int sx, unsigned int sy,unsigned int dx,unsigned int dy,unsigned bw,unsigned bh)
{
 DEBUG_OUT<<"AWindowAbstract::copyBlock(APrivateBitmap *someImage,...)\n";
  if(!someImage) { DEBUG_OUT<<"NULL Image pointer to draw!\n"; return; }
  if(!someImage->isOkay()) { if(!someImage->verifyOkay(this)) return; }
  someImage->verifyOkay(this);
#ifdef ASYS_X11
  Pixmap pbits=someImage->getPBits();
  //XImage *ximage=someImage->getImage();
  if(!pbits) { aDefaultStringHelper.fatal("NULL pbits!\n"); }
  DEBUG_OUT<<"Going to XCopyArea...\n";
  XCopyArea(parent->getDisplay(),pbits,TheWin,TheGC,sx,sy,bw,bh,dx,dy);
  //XPutImage(parent->getDisplay(),TheWin,TheGC,ximage,sx,sy,dx,dy,bw,bh);
  //XSync(parent->getDisplay(),false);  // FIXME: Slows everything down terribly..
#endif // ASYS_X11
#ifdef ASYS_DOS
  if(!parent) return;
  if(!parent->getTheBits()) return;
  if(haveFocus) {
    parent->getTheBits()->draw(someImage,atX+x,atY+y);
  }
#endif // ASYS_DOS
#ifdef ASYS_AMIGA
  if(!TheRP) { aDefaultStringHelper.fatal("NULL TheRP!\n"); }
  BltBitMapRastPort(someImage->getBits(),someImage->getX(),
    someImage->getY(),TheRP,dx,dy,someImage->getWidth(),
    someImage->getHeight(),0xc0);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
  //DEBUG_OUT<<"Gonna draw...\n";
  //CGrafPtr origPort;
  //GDHandle origDev;
#ifdef USE_QUARTZ
#else
  //DEBUG_OUT<<"Gonna MacSetPort...\n";
  MacSetPort(TheGP);
  GWorldPtr theWorld=NULL;
  //DEBUG_OUT<<"Gonna getTheWorld...\n";
  theWorld=someImage->getTheWorld();
  if(!theWorld) { aDefaultStringHelper.fatal("NULL theWorld!\n"); }
  //GetGWorld(&origPort,&origDev);
  //DEBUG_OUT<<"Gonna SetGWorld...\n";
  //SetGWorld(theWorld,NULL);
  ForeColor(blackColor);  BackColor(whiteColor);
#endif // USE_QUARTZ
  Rect sr,dr;
  dr.top=dx;  dr.bottom=dy+bh;
  dr.left=dy;  dr.right=dx+bw;
  sr.top=sx;  sr.bottom=someImage->getY()+bh;
  sr.left=sy;  sr.right=someImage->getX()+bw;
  /*
  if(someImage->getOkay()) {
    CGrafPtr cg=(CGrafPtr)TheWP;
    PixMapHandle pmh=cg->portPixMap;
    CTabHandle wctab=(**pmh).pmTable;
    CTabHandle ctab=someImage->getCTab();
    if((ctab)&&(wctab)) (**ctab).ctSeed=(**wctab).ctSeed;
  }
  */
#ifdef USE_QUARTZ
#else
  //DEBUG_OUT<<"Gonna GetGWorldPixMap...\n";
  PixMapHandle myWorldPM=NULL;
  myWorldPM=GetGWorldPixMap(theWorld);
  if(!myWorldPM) { aDefaultStringHelper.fatal("NULL myWorldPM\n"); }
  bool good=false;
  //DEBUG_OUT<<"Gonna LockPixels...\n";
  good=LockPixels(myWorldPM);
  if(!good) { DEBUG_OUT<<"draw: couldn't lock gWorld pixmap!\n"; return; }
  BitMap *srcBits=(BitMap *)(*myWorldPM);
  //BitMap *srcBits=((BitMap *)(someImage->getPBits());
  //DEBUG_OUT<<"Gonna CopyBits...\n";
#if TARGET_API_CARBON
  CopyBits(srcBits,(BitMap *)*GetPortPixMap(TheGP),&sr,&dr,srcCopy,(OpaqueRgnHandle *)NULL);
#else
  CopyBits(srcBits,(BitMap *)TheGP,&sr,&dr,srcCopy,NULL);
#endif
  //DEBUG_OUT<<"Gonna UnlockPixels...\n";
  UnlockPixels(myWorldPM);
  //SetGWorld(origPort,origDev);
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_OS2
  HBITMAP theOldBits=GpiSetBitmap(bitsPS,someImage->getBits());
  unsigned int tty=height-someImage->getHeight()-dy-borderHeight;
  tty-=2;
  POINTL *point=someImage->getPoints();
  point[0].x=dx;  point[0].y=tty;
  point[1].x=dx+someImage->getWidth();
  point[1].y=tty+someImage->getHeight();
  ULONG bitRes=GpiBitBlt(winHPS,bitsPS,3L,point,ROP_SRCCOPY,BBO_IGNORE);
  if(bitRes!=GPI_OK) DEBUG_OUT<<"GpiBitBlt failed!\n";
  GpiSetBitmap(bitsPS,theOldBits);
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(offscreenImage) offscreenImage->draw(someImage,dx+x,dy+y);
#else
  HDC hdc=GetDC(ahWnd);
  HDC theBitsDC=CreateCompatibleDC(hdc);
  HBITMAP theBits=NULL;
  theBits=someImage->getBits();
  if(!theBits) { aDefaultStringHelper.fatal("NULL theBits!\n"); }
  HBITMAP theOldBits=(HBITMAP)SelectObject(theBitsDC,theBits);
  int bitRes=BitBlt(hdc,dx,dy,someImage->getWidth(),someImage->getHeight(),theBitsDC,someImage->getX(),someImage->getY(),SRCCOPY);
  //if(bitRes==0) DEBUG_OUT<<"BitBlt failed ("<<bitRes<<")!\n";
  SelectObject(theBitsDC,theOldBits);
  DeleteDC(theBitsDC);
  ReleaseDC(ahWnd,hdc);
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_POSIX
  unsigned int tpenX=penX+avgCharWidth;
#ifdef USE_ANSI_ON_STDIO
  printf("\033[%d;%df",penY>>3,tpenX>>3);
#endif // USE_ANSI_ON_STDIO
  printf("%c",someImage->getLetter());
  //DEBUG_OUT<<someImage->getLetter();
#endif // ASYS_POSIX
  penX=dx;  penY=dy;
}


void AWindowAbstract::draw(APrivateBitmap *someImage, unsigned int atX, unsigned int atY)
{
  DEBUG_OUT<<"AWindowAbstract::draw(APrivateBitmap *someImage,...)\n";
  if(!someImage) { DEBUG_OUT<<"NULL Image pointer to draw!\n"; return; }
  copyBlock(someImage,0,0,atX,atY,someImage->getWidth(),someImage->getHeight());
}


void AWindowAbstract::moveTo(unsigned int newX, unsigned int newY)
{
  penX=newX;  penY=newY;
}


void AWindowAbstract::lineTo(unsigned int newX, unsigned int newY)
{
#ifdef ASYS_AMIGA
  Move(TheRP,penX,penY);
  Draw(TheRP,newX,newY);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
  MoveTo(penX,penY);
  MacLineTo(newX,newY);
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  HDC hDC=GetDC(ahWnd);
  SelectObject(hDC,hPen);
  //SetMapMode(hDC,MM_TEXT);
#ifdef ASYS_WIN32CE
  POINT points[2];
  points[0].x=penX;  points[0].y=penY;
  points[1].x=newX;  points[1].y=newY;
  Polyline(hDC,points,2);
#else
#ifdef ASYS_WIN16
  MoveTo(hDC,penX,penY);
#else
  POINT oldLoc;
  MoveToEx(hDC,penX,penY,&oldLoc);
#endif // ASYS_WIN16
  LineTo(hDC,newX,newY);
#endif // ASYS_WIN32CE
  ReleaseDC(ahWnd,hDC);
#else
  if(offscreenImage) offscreenImage->line(penX+x,penY+y,newX+x,newY+y);
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_DOS
  if(!parent) return *this;
  if(!parent->getTheBits()) return *this;
  if(haveFocus) parent->getTheBits()->line(penX+x,penY+y,newX+x,newY+y);
#endif // ASYS_DOS
#ifdef ASYS_X11
  XDrawLine(parent->getDisplay(),TheWin,TheGC,penX,penY,newX,newY);
#endif // ASYS_X11
#ifdef ASYS_OS2
  // FIXME: Fix y
  POINTL s,e;
  s.x=penX;  s.y=penY;  e.x=newX;  e.y=newY;
  GpiMove(winHPS,&s);     GpiLine(winHPS,&e);
#endif // ASYS_OS2
#ifdef ASYS_PALM
  if(foreground&1) WinEraseLine(penX,penY+yOffset,newX,newY+yOffset);
  else WinDrawLine(penX,penY+yOffset,newX,newY+yOffset);
#endif // ASYS_PALM
#ifdef ASYS_GLUT
  glBegin(GL_LINES);
  glVertex3f(glutTransX(penX),glutTransY(penY),0.0);
  glVertex3f(glutTransX(newX),glutTransY(newY),0.0);
  glEnd();
#endif // ASYS_GLUT
}


// FIXME: Need "optimized" OS/2 or X or DOS versions.
void AWindowAbstract::line(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY)
{
  bool done=false;
#ifdef ASYS_AMIGA
  Move(TheRP,startX,startY);
  Draw(TheRP,endX,endY);
  done=true;
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
#endif // USE_QUARTZ
#ifdef USE_QUARTZ
#else
  MoveTo(startX,startY);
  MacLineTo(endX,endY);
#endif // USE_QUARTZ
  done=true;
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  HDC hDC=GetDC(ahWnd);
  SelectObject(hDC,hPen);
  //SetMapMode(hDC,MM_TEXT);
#ifdef ASYS_WIN32CE
  POINT points[2];
  points[0].x=startX;  points[0].y=startY;
  points[1].x=endX;  points[1].y=endY;
  Polyline(hDC,points,2);
#else
#ifdef ASYS_WIN16
  MoveTo(hDC,startX,startY);
#else
  POINT oldLoc;
  MoveToEx(hDC,startX,startY,&oldLoc);
#endif // ASYS_WIN16
  LineTo(hDC,endX,endY);
#endif // ASYS_WIN32CE
  ReleaseDC(ahWnd,hDC);
  done=true;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_PALM
  if(foreground&1) WinEraseLine(startX,startY+yOffset,endX,endY+yOffset);
  else WinDrawLine(startX,startY+yOffset,endX,endY+yOffset);
  done=true;
#endif // ASYS_PALM
  if(!done) {
    moveTo(startX,startY);
    lineTo(endX,endY);
  }
}


// FIXME: rwidth and rheight ARE width and height, NOT coords!
void AWindowAbstract::drawRect(unsigned int rwidth, unsigned int rheight)
{
  //DEBUG_OUT<<"***drawRect: "<<penX<<","<<penY<<","<<rwidth<<","<<rheight<<"\n";
  bool didIt=false;
#ifdef ASYS_X11
  XDrawRectangle(parent->getDisplay(),TheWin,TheGC,penX,penY,
    rwidth,rheight);
  didIt=true;
#endif // ASYS_X11
#ifdef ASYS_OS2
  setForeground(foreground);
  POINTL coords;
  coords.x=penX;  coords.y=penY;
  GpiMove(winHPS,&coords);  coords.x=penX+rwidth;  coords.y=penY+rheight;
  GpiBox(winHPS,DRO_FILL,&coords,0,0);
  didIt=true;
#endif // ASYS_OS2
  if(!didIt) {
    unsigned ox=penX,oy=penY;
    line(ox,oy,ox+rwidth,oy);
    line(ox+rwidth,oy,ox+rwidth,oy+rheight);
    line(ox+rwidth,oy+rheight,ox,oy+rheight);
    line(ox,oy+rheight,ox,oy);
  }
}


// FIXME: rwidth and rheight ARE width and height, NOT coords!
void AWindowAbstract::drawFilledRect(unsigned int rwidth, unsigned int rheight)
{
  // FIXME: actually fill the rectangle...
  bool didIt=false;
#ifdef ASYS_AMIGA
  RectFill(TheRP,penX,penY,penX+rwidth,penY+rheight);
  didIt=true;
#endif // ASYS_AMIGA
  if(!didIt) {
    UINT rx=penX,ry=penY;
    for(UINT t=0;t<rheight;t++) {
      line(rx,ry+t,rx+rwidth,ry+t);
    }
    //drawRect(rwidth,rheight);
  }
}


// FIXME: rwidth and rheight ARE width and height, NOT coords!
void AWindowAbstract::eraseRect(unsigned int rwidth, unsigned int rheight)
{
  clear(penX,penY,rwidth,rheight);
}


void AWindowAbstract::writePixel(unsigned int atX, unsigned int atY)
{
  // a zero len line might not draw anything, depending on the os...
#ifdef ASYS_DOS
  if(!parent) return;
  if(!parent->getTheBits()) return;
  if(haveFocus) parent->getTheBits()->writePixel(x+atX,y+atY);
#endif // ASYS_DOS
#ifdef ASYS_AMIGA
  line(atX,atY,atX,atY);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
  line(atX,atY,atX,atY);
#endif // ASYS_MAC
#ifdef ASYS_X11
  line(atX,atY,atX,atY);
#endif // ASYS_X11
#ifdef ASYS_OS2
  line(atX,atY,atX,atY);
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(offscreenImage) offscreenImage->writePixel(x+atX,y+atY);
#else
  line(atX,atY,atX,atY);
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_PALM
  line(atX,atY+yOffset,atX,atY+yOffset);
#endif // ASYS_PALM
}


bool AWindowAbstract::fixSizeMove(unsigned int tx,unsigned int ty,unsigned int twidth,
  unsigned int theight, bool sizeNotMove)
{
#ifdef DEBUG_WINDOW_RESIZES_MOVES
  DEBUG_OUT<<"***Original parameters: "<<tx<<","<<ty<<","<<twidth<<","<<theight<<"\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
  //bool moveFix=false,sizeFix=false,extraMoveFix=false;
  bool moveFix=true,sizeFix=true,extraMoveFix=true;
  unsigned int oy=ty,ox=tx;
#ifdef PEDANTIC_WINDOW_CRAP
  // First add borders if we haven't yet...
  if(!sizeCorrected) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Border sizes added...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    twidth+=borderWidth;  theight+=borderHeight;
    sizeCorrected=true;
    //sizeFix=true;
  }
  if(!moveCorrected) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Offset sizes added...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    tx+=parent->getXOffset();  ty+=parent->getYOffset();
    moveCorrected=true;
    //sizeFix=true;
  }
  if((ty+theight)>parent->getHeight()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Height clipped1...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    theight=parent->getHeight();
    sizeFix=true;
  }
  if((tx+twidth)>parent->getWidth()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Width clipped1...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    twidth=parent->getWidth();
    sizeFix=true;
  }
  if(ty<parent->getYOffset()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Y nudged1...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    ty+=parent->getYOffset();
    moveFix=true;
  }
  if(tx<parent->getXOffset()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** X nudged1...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    tx+=parent->getXOffset();
    moveFix=true;
  }
  // Then clip the size to the screen with the factors taken into account.
  if((ty+theight)>parent->getHeight()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Y clipped2...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    ty=parent->getHeight()-theight;
    moveFix=true;
  }
  if((tx+twidth)>parent->getWidth()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** X clipped2...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    tx=parent->getWidth()-twidth;
    moveFix=true;
  }
  if(ty<parent->getYOffset()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Y nudged2...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    ty+=parent->getYOffset();
    moveFix=true;
  }
  if(tx<parent->getXOffset()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** X nudged2...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    tx+=parent->getXOffset();
    moveFix=true;
  }
  if((ty+theight)>parent->getHeight()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Height clipped2...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    theight=parent->getHeight();
    sizeFix=true;
  }
  if((tx+twidth)>parent->getWidth()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"*** Width clipped2...\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    twidth=parent->getWidth();
    sizeFix=true;
  }
#endif // PENDANTIC_WINDOW_CRAP
#ifdef ASYS_OS2
  // Force a move when we resize...
  moveFix=true; extraMoveFix=true;
#endif // ASYS_OS2
  // We're done.
#ifdef DEBUG_WINDOW_RESIZES_MOVES
  DEBUG_OUT<<"***Actual parameters: "<<tx<<","<<ty<<","<<twidth<<","<<theight<<"\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
  if(moveFix||(!sizeNotMove)) reallyMove(tx,ty);
  if(sizeFix||(sizeNotMove)) reallyResize(twidth,theight);
  if(extraMoveFix) reallyMove(tx,ty);
  return (bool)(!(moveFix||sizeFix));
}


// FIXME: handle menu bar and border better
bool AWindowAbstract::resize(unsigned int newWidth, unsigned int newHeight)
{
#ifdef DEBUG_WINDOW_RESIZES_MOVES
  DEBUG_OUT<<"AWindowAbstract::resize("<<newWidth<<","<<newHeight<<")\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
  sync();
  bool ret=fixSizeMove(x,y,newWidth,newHeight,true);
  return ret;
}


// FIXME: handle menu bar and border better.
bool AWindowAbstract::move(int newX, int newY)
{
#ifdef DEBUG_WINDOW_RESIZES_MOVES
  DEBUG_OUT<<"AWindowAbstract::move("<<newX<<","<<newY<<")\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
  sync();
  if(newX<0) { newX=parent->getWidth()+newX; }
  if(newY<0) { newY=parent->getHeight()+newY; }
  bool ret=fixSizeMove(newX,newY,width,height,false);
  return ret;
}


void AWindowAbstract::reallyResize(unsigned int twidth, unsigned int theight)
{
#ifdef DEBUG_WINDOW_RESIZES_MOVES
  DEBUG_OUT<<"***AWindowAbstract::reallyResize("<<twidth<<","<<theight<<")\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
  width=twidth;  height=theight;
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef USE_DIRECTDRAW
  SetWindowPos(ahWnd,(HWND)NULLHANDLE,x,y,twidth,theight,SWP_DRAWFRAME);
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
#if !__LP64__
  SizeWindow(TheWP,twidth,theight,true);
#endif
#endif // ASYS_MAC
#ifdef ASYS_OS2
  theight+=2;
  WinSetWindowPos(ahWndFrame,HWND_TOP,(SHORT)0,(SHORT)0,(SHORT)twidth,
    (SHORT)theight,SWP_ACTIVATE|SWP_SIZE|SWP_SHOW);
#endif // ASYS_OS2
#ifdef ASYS_X11
  XResizeWindow(parent->getDisplay(),TheWin,twidth,theight);
  XSync(parent->getDisplay(),false);
  //waitForFirstValidEvent();
#endif // ASYS_X11
#ifdef ASYS_AMIGA
  if(TheWin) {
    if(aThisApp.version>35)
      ChangeWindowBox((Window *)TheWin,x,y,twidth,theight);
    else {
      sync();
      int dwidth=twidth-width,dheight=theight-height;
      SizeWindow(TheWin,dwidth,dheight);
    }
  }
#endif // ASYS_AMIGA
  sync();
  //if(charMapped) resizeCharMap();
}


void AWindowAbstract::reallyMove(unsigned int tx, unsigned int ty)
{
#ifdef DEBUG_WINDOW_RESIZES_MOVES
  DEBUG_OUT<<"***AWindowAbstract::reallyMove("<<tx<<","<<ty<<")\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
  x=tx;  y=ty;
#ifdef ASYS_MAC
#if !__LP64__
  MacMoveWindow(TheWP,tx,ty,true);
#endif
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  if(TheWin) {
    if(aThisApp.version>35)
      ChangeWindowBox((Window *)TheWin,tx,ty,width,height);
    else {
      sync();
      int dx=tx-x,dy=ty-y;
      MoveWindow(TheWin,dx,dy);
    }
  }
#endif // ASYS_AMIGA
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef USE_DIRECTDRAW
  SetWindowPos(ahWnd,(HWND)NULLHANDLE,tx,ty,width,height,SWP_DRAWFRAME);
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  sync();
  unsigned short tty=(unsigned short)(parent->getHeight()-ty-height);
  if(tty>parent->getHeight()) {
#ifdef DEBUG_WINDOW_RESIZES_MOVES
    DEBUG_OUT<<"***AWindowAbstract::reallyMove clipped tty ("<<tty<<")\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
    tty=0;
  }
#ifdef DEBUG_WINDOW_RESIZES_MOVES
  DEBUG_OUT<<"***AWindowAbstract::reallyMove Y was "<<ty<<" now "<<tty<<"\n";
#endif // DEBUG_WINDOW_RESIZES_MOVES
  WinSetWindowPos(ahWndFrame,HWND_TOP,(SHORT)tx,
    tty,(SHORT)0,(SHORT)0,SWP_ACTIVATE|SWP_MOVE|SWP_SHOW);
#endif // ASYS_OS2
#ifdef ASYS_X11
  XMoveWindow(parent->getDisplay(),TheWin,tx,ty);
  XSync(parent->getDisplay(),false);
  //waitForFirstValidEvent();
#endif // ASYS_X11
  sync();
}


// NOTE: This method won't do anything on truecolor displays...
bool AWindowAbstract::setColor(unsigned int colorNum, unsigned int r,unsigned int g, unsigned int b)
{
  // FIXME: later change colors on a per-window basis.
  bool retVal=parent->setColor(colorNum,r,g,b);
#ifdef ASYS_X11
  XSetWindowColormap(parent->getDisplay(),TheWin,parent->getCMap());
#endif // ASYS_X11
  return retVal;
}


// NOTE: This method won't do anything on truecolor displays...
bool AWindowAbstract::setColor(unsigned int colorNum, unsigned long l)
{
  // FIXME: later change colors on a per-window basis.
  bool retVal=parent->setColor(colorNum,l);
#ifdef ASYS_X11
  XSetWindowColormap(parent->getDisplay(),TheWin,parent->getCMap());
#endif // ASYS_X11
  return retVal;
}


// NOTE: This method won't do anything on truecolor displays...
bool AWindowAbstract::setColor(unsigned int index,AColor *aColor)
{
  // FIXME: later change colors on a per-window basis.
  bool retVal=parent->setColor(index,aColor);
#ifdef ASYS_X11
  XSetWindowColormap(parent->getDisplay(),TheWin,parent->getCMap());
#endif // ASYS_X11
  return retVal;
}

APalette *AWindowAbstract::getColors()
{
  // FIXME: later change colors on a per-window basis.
  return parent->getColors();
}


// NOTE: This method won't do anything on truecolor displays...
bool AWindowAbstract::matchColors(APalette *someColors)
{
  // FIXME: later change colors on a per-window basis.
  if(!someColors) return false;
  bool retVal=parent->matchColors(someColors);
#ifdef ASYS_X11
  XSetWindowColormap(parent->getDisplay(),TheWin,parent->getCMap());
#endif // ASYS_X11
  return retVal;
}


// FIXME: No Mac version yet
// FIXME: handle title bar and border better
AWindowAbstract& AWindowAbstract::sync()
{
  if(syncLock) { DEBUG_OUT<<"(syncLock)\n"; return *this; }
  syncLock=true;
  // Here we check the actual width and update the various
  // properties: width,height, etc.
  // Fix negative coords...
  //if(a_abs((int)x)!=(int)x) x=-((int)x);
  //if(a_abs((int)y)!=(int)y) y=-((int)y);
  //fontHeight=8;
  avgCharWidth=8;
  if(!width) width=640;
  if(!height) height=200;
  depth=parent->getDepth();
  nPlanes=parent->getNPlanes();
#ifdef ASYS_PALM
  x=0;  y=0;
  SWord tw,th;
  WinGetWindowExtent(&tw,&th);
  width=tw;
  height=th-yOffset;
#endif // ASYS_PALM
#ifdef ASYS_MAC
  // FIXME: Need to do more here...
#if TARGET_API_MAC_CARBON
  Rect theRect;
#ifdef USE_QUARTZ
#else
  GetPortBounds(GetWindowPort(TheWP),&theRect);
#endif // USE_QUARTZ
  width=theRect.right-theRect.left;
  height=theRect.bottom-theRect.top;
#else
  width=TheWP->portRect.right-TheWP->portRect.left;
  height=TheWP->portRect.bottom-TheWP->portRect.top;
#endif // TARGET_API_MAC_CARBON
#endif // ASYS_MAC
#ifdef ASYS_DOS
  unsigned int pwidth=parent->getWidth();
  unsigned int pheight=parent->getHeight();
  // FIXME: these force the window to fullscreen, use the below code to
  // let it just use part of the screen (but still no multiple windows)
  x=0;  y=0;  width=pwidth;  height=pheight;
  /*
  if((unsigned int)x>pwidth) x=0;
  if((unsigned int)y>pheight) y=0;
  if(width>pwidth) width=pwidth;
  if(height>pheight) height=pheight;
  */
  // FIXME: Clip, make sure window doesn't hang off display here...
  fontHeight=8;
  // This isn't always right...
  if(pheight>200) fontHeight=14;
  if(pheight>350) fontHeight=16;
  if(pheight<200) fontHeight=1;
#endif // ASYS_DOS
#ifdef ASYS_AMIGA
  // FIXME: is there a better way to do this under 3.x?
  if(TheWin) {
    width=((Window *)TheWin)->Width;  height=((Window *)TheWin)->Height;
    x=((Window *)TheWin)->LeftEdge;  y=((Window *)TheWin)->TopEdge;
  }
  else { x=0; y=0; width=0; height=0; }
#endif // ASYS_AMIGA
#ifdef ASYS_OS2
  SWP winPos;
  WinQueryWindowPos(ahWndFrame,&winPos);
  // FIXME: Why doesn't this work?
  //width=winPos.cx;
  //height=winPos.cy;
  //x=winPos.x;
  //y=parent->getHeight()-(winPos.y+height);
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
  // FIXME: Make sure we actually have a window
  RECT clientRect,winRect;
  GetClientRect(ahWnd,&clientRect);
  GetWindowRect(ahWnd,&winRect);
#ifdef USE_DIRECTDRAW
  unsigned int pwidth=parent->getWidth();
  unsigned int pheight=parent->getHeight();
  // FIXME: these force the window to fullscreen, use the below code to
  // let it just use part of the screen (but still no multiple windows)
  x=0;  y=0;  width=pwidth;  height=pheight;
  /*
  if((unsigned int)x>pwidth) x=0;
  if((unsigned int)y>pheight) y=0;
  if(width>pwidth) width=pwidth;
  if(height>pheight) height=pheight;
  */
  // FIXME: Clip, make sure window doesn't hang off display here...
  fontHeight=8;
#else
  unsigned int twidth=clientRect.right-clientRect.left;
  unsigned int theight=clientRect.bottom-clientRect.top;
#ifdef ASYS_WIN32CE
#if CE_MAJOR_VER >2
  twidth=alib_sinfo.rcVisibleDesktop.right-alib_sinfo.rcVisibleDesktop.left;
  theight=alib_sinfo.rcVisibleDesktop.bottom-alib_sinfo.rcVisibleDesktop.top;
  DEBUG_OUT<<"sip is at r:"<<alib_sinfo.rcVisibleDesktop.right;
  DEBUG_OUT<<" l:"<<alib_sinfo.rcVisibleDesktop.left;
  DEBUG_OUT<<" b:"<<alib_sinfo.rcVisibleDesktop.bottom;
  DEBUG_OUT<<" t:"<<alib_sinfo.rcVisibleDesktop.top<<"\n";
  DEBUG_OUT<<"So twidth x theight is "<<twidth<<" x "<<theight<<"\n";
#endif
#endif // ASYS_WIN32CE
  borderWidth=(winRect.right-winRect.left)-twidth;
  borderHeight=(winRect.bottom-winRect.top)-theight;
  x=winRect.left;  y=winRect.top;
  width=twidth;  height=theight;
  width+=borderWidth;
  height+=borderHeight;
  //DEBUG_OUT<<"border: "<<borderWidth<<" x "<<borderHeight<<"\n";
  //DEBUG_OUT<<"xy: "<<x<<" x "<<y<<"\n";
  //DEBUG_OUT<<"size: "<<width<<" x "<<height<<"\n";
  //sizeCorrected=true;
  // This is for really small windows...
  if(theight<borderHeight) {
    //DEBUG_OUT<<"sync hack: theight is "<<theight;
    //DEBUG_OUT<<"   borderHeight is "<<borderHeight<<"\n";
    borderHeight+=borderWidth+(borderWidth/2)+2;
  }
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_X11
  //DEBUG_OUT<<"(AWindowAbstract::sync)\n";
  // FIXME: Make sure we actually have a window
  //XSync(parent->getDisplay(),false);
  Window root;
  int xr,yr;
  unsigned int wr,hr,br,dr;
  // Does this take the window border into account?
  XGetGeometry(parent->getDisplay(),TheWin,&root,&xr,&yr,&wr,&hr,&br,&dr);
  // FIXME: is this a rect, should it be width=wr-xr, etc?
  //x=xr;  y=yr;
  width=wr;  height=hr;
  /*
  XSizeHints *hints=XAllocSizeHints();
  long sup;
  int i=XGetWMSizeHints(parent->getDisplay(),TheWin,hints,&sup,
    XA_WM_SIZE_HINTS);
  DEBUG_OUT<<i<<" "<<sup<<","<<hints->min_width<<" "<<hints->min_height;
  DEBUG_OUT<<","<<hints->max_width<<" "<<hints->max_height;
  DEBUG_OUT<<","<<hints->width_inc<<" "<<hints->height_inc;
  DEBUG_OUT<<","<<hints->base_width<<" "<<hints->base_height;
  DEBUG_OUT<<"\n";
  */
  /*
  int tx,ty,tw,th,tg;
  XSizeHints *hints=XAllocSizeHints();
  XWMGeometry(parent->getDisplay(),0,NULL,NULL,0,hints,&tx,&ty,&tw,&th,&tg);
  DEBUG_OUT<<tx<<" "<<ty<<" "<<tw<<" "<<th<<"\n";
  */
#endif // ASYS_X11
#ifdef ASYS_GLUT
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(65.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,0.0);  /* viewing transform  */
#endif // ASYS_GLUT
  numRows=height/fontHeight;
  syncLock=false;
  calc();
  DEBUG_OUT<<"sync: "<<width<<"x"<<height<<" at "<<x<<","<<y<<" d="<<depth<<" np="<<nPlanes<<"\n";
  return *this;
}


AWindowAbstract& AWindowAbstract::reallyFocus()
{
  parent->reallyFocus();
  return focus();
}


// FIXME: No OS/2 version yet
AWindowAbstract& AWindowAbstract::focus()
{
  // the parent's focus will unfocus all windows,
  // so we then set this window to focus
  // This is ugly, but it works.
  // FIXME: (Well, not really.)
  parent->focus();
  //if(aThisApp.focus) aThisApp.focus->haveFocus=false;
  haveFocus=true;
  //aThisApp.focusWindow=this;
#ifdef ASYS_WINDOWS
  SetFocus(ahWnd);
#ifdef ASYS_WIN32CE
  SetForegroundWindow(ahWnd);
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS
#ifdef ASYS_AMIGA
  WindowToFront(TheWin);
  ActivateWindow((Window *)TheWin);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#if !__LP64__
  SelectWindow(TheWP);
#endif
#endif // ASYS_MAC
#ifdef ASYS_OS2
  // FIXME: get this working
  // Maybe I should use ahWnd instead of ahWndFrame...
  //WinShowWindow(ahWndFrame,true);
  //WinFocusChange(HWND_DESKTOP,ahWndFrame,0);
#endif // ASYS_OS2
#ifdef ASYS_X11
  // Does this work?
  //XSync(parent->getDisplay(),false);
  XRaiseWindow(parent->getDisplay(),TheWin);
  XMapRaised(parent->getDisplay(),TheWin);
  //XSync(parent->getDisplay(),false);
#endif // ASYS_X11
#ifdef ASYS_DOS
  parent->setPaletteRegs();
#endif // ASYS_DOS
  return *this;
}


void AWindowAbstract::setForeground(unsigned long pen,bool rgb)
{
  if(parent) pen=parent->fixForeColor(pen);
  // These may or may not be used...
  unsigned int red=(int)((pen&0xff0000L)>>16);
  unsigned int green=(int)((pen&0xff00)>>8);
  unsigned int blue=(int)(pen&0xff);
#ifdef ASYS_AMIGA
  SetAPen(TheRP,pen);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
  fcolor.red=red<<8; fcolor.green=green<<8; fcolor.blue=blue<<8;
  if(depth>1) RGBForeColor(&fcolor);
  else {
    // doesn't this belong in fixForeColor?
    pen=pen%2;
    if(pen==1) ForeColor(whiteColor); else ForeColor(blackColor);
  }
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(offscreenImage) offscreenImage->setForeground(pen);
#else
  if(hPen) DeleteObject(hPen);
  hPen=CreatePen(PS_SOLID,1,RGB(red,green,blue));
  pred=red;  pgreen=green;  pblue=blue;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_DOS
  if(!parent) return;
  if(!parent->getTheBits()) return;
  if(haveFocus) parent->getTheBits()->setForeground(pen);
#endif // ASYS_DOS
#ifdef ASYS_X11
  XSetForeground(parent->getDisplay(),TheGC,pen);
#endif // ASYS_X11
#ifdef ASYS_OS2
  long tpen=pen;
  // doesn't this belong in fixForeColor?
  if(tpen==0x000000) tpen=CLR_BLACK;
  if(tpen==0xffffff) tpen=CLR_WHITE;
  //
  GpiSetColor(winHPS,tpen);
  foreground=tpen;
  pen=tpen;
  CHARBUNDLE cb;
  cb.lColor=foreground;
  cb.lBackColor=background;
  cb.usMixMode=FM_OVERPAINT;
  cb.usBackMixMode=BM_OVERPAINT;
  GpiSetAttrs(winHPS,PRIM_CHAR,CBB_COLOR|CBB_BACK_COLOR,0,&cb);
  //    CBB_MIX_MODE|CBB_BACK_MIX_MODE,0,&cb);
  GpiSetMix(winHPS,FM_OVERPAINT);
  GpiSetBackMix(winHPS,BM_OVERPAINT);
#endif // ASYS_OS2
#ifdef ASYS_PALM
#ifdef __PALMOS_H__
  if(aThisApp.hasThreePointFive) {
    WinSetForeColor(pen);
    WinSetTextColor(pen);
  }
#endif // __PALMOS_H__
#endif // ASYS_PALM
#ifdef ASYS_GLUT
  float fr=(float)red/255.0;
  float fg=(float)green/255.0;
  float fb=(float)blue/255.0;
  if(!red) fr=0.0;
  if(!green) fg=0.0;
  if(!blue) fb=0.0;
  //DEBUG_OUT<<"glColor3f("<<fr<<","<<fg<<","<<fb<<")";
  //DEBUG_OUT<<" from "<<pen<<": "<<red<<","<<green<<","<<blue<<"\n";
  glColor3f(fr,fg,fb);
#endif // ASYS_GLUT
  foreground=pen;
}


void AWindowAbstract::setBackground(unsigned long pen,bool rgb)
{
  if(parent) pen=parent->fixBackColor(pen);
  // These may or may not be used...
  unsigned int red=(int)((pen&0xff0000L)>>16);
  unsigned int green=(int)((pen&0xff00)>>8);
  unsigned int blue=(int)(pen&0xff);
#ifdef ASYS_AMIGA
  SetBPen(TheRP,pen);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
  bcolor.red=red<<8; bcolor.green=green<<8; bcolor.blue=blue<<8;
  if(depth>1) RGBBackColor(&bcolor);
  else {
    // doesn't this belong in fixBackColor?
    pen=pen%2;
    if(pen==1) BackColor(whiteColor); else BackColor(blackColor);
  }
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(offscreenImage) offscreenImage->setBackground(pen);
#else
  if(backBrush) DeleteObject(backBrush);
  backBrush=CreateSolidBrush(RGB(red,green,blue));
  bred=red;  bgreen=green;  bblue=blue;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_DOS
  if(!parent) return;
  if(!parent->getTheBits()) return;
  if(haveFocus) parent->getTheBits()->setBackground(pen);
#endif // ASYS_DOS
#ifdef ASYS_X11
  XSetWindowBackground(parent->getDisplay(),TheWin,pen);
  XSetBackground(parent->getDisplay(),TheGC,pen);
#endif // ASYS_X11
#ifdef ASYS_OS2
  long tpen=pen;
  // doesn't this belong in fixBackColor?
  if(tpen==0x000000) tpen=CLR_BLACK;
  if(tpen==0xffffff) tpen=CLR_WHITE;
  //
  GpiSetBackColor(winHPS,tpen);
  background=tpen;
  pen=tpen;
  CHARBUNDLE cb;
  cb.lColor=foreground;
  cb.lBackColor=background;
  cb.usMixMode=FM_OVERPAINT;
  cb.usBackMixMode=BM_OVERPAINT;
  GpiSetAttrs(winHPS,PRIM_CHAR,CBB_COLOR|CBB_BACK_COLOR,0,&cb);
  //  CBB_MIX_MODE|CBB_BACK_MIX_MODE,0,&cb);
  GpiSetMix(winHPS,FM_OVERPAINT);
  GpiSetBackMix(winHPS,BM_OVERPAINT);
#endif // ASYS_OS2
#ifdef ASYS_PALM
#ifdef __PALMOS_H__
  if(aThisApp.hasThreePointFive) WinSetBackColor(pen);
#endif // __PALMOS_H__
#endif // ASYS_PALM
#ifdef ASYS_GLUT
  float fr=(float)red/255.0;
  float fg=(float)green/255.0;
  float fb=(float)blue/255.0;
  if(!red) fr=0.0;
  if(!green) fg=0.0;
  if(!blue) fb=0.0;
  //DEBUG_OUT<<"glClearColor("<<fr<<","<<fg<<","<<fb<<")";
  //DEBUG_OUT<<" from "<<pen<<": "<<red<<","<<green<<","<<blue<<"\n";
  glClearColor(fr,fg,fb,0.0);
#endif // ASYS_GLUT
  background=pen;
}


// Look for an arbitrarily named and optionally sized and styled font
// The name can something like "Courier" and the search will effectively be "*courier*"
// Will return actual name of font you should pass to setFont, etc.
// TODO: Doesn't handle flags...
char *AWindowAbstract::queryFontAvail(char *name,unsigned int siz,unsigned int flags)
{
  char *ret=NULL;
  if(!name) {
    name=(char *)"helvetica";
    if(flags==A_FONT_NONPROP) {
#ifdef ASYS_X11
      name=(char *)"fixed";
#else
      name=(char *)"courier";
#endif // ASYS_X11
    }
    DEBUG_OUT<<"queryFontAvail using default font "<<name<<"\n";
  }
  //if(!flags) flags=A_FONT_NONPROP;
#ifdef ASYS_X11
  char *lc=aDefaultStringHelper.toLower(name);
  char ename[256];
  ename[0]=0;
  // We do this so we don't get the jis version of fixed...
  if(!strcmp("fixed",name)) aDefaultStringHelper.fillInString(ename,"-misc-");
  else aDefaultStringHelper.fillInString(ename,"-*-");
  aDefaultStringHelper.fillInString(ename,lc);
  const char *fweight="-*";  // We can change this as needed...
  aDefaultStringHelper.fillInString(ename,fweight);
  aDefaultStringHelper.fillInString(ename,"-r-*--");
  if(siz) {
    char szs[10];
    sprintf(szs,"%d",siz);
    aDefaultStringHelper.fillInString(ename,szs);
  }
  else aDefaultStringHelper.fillInString(ename,"*");
  aDefaultStringHelper.fillInString(ename,"-*");
  //ename="-*-courier-*-r-*--24-*";
  unsigned int t=0;
  //for(t=0;t<strlen(lc);t++) ename[t+1]=lc[t];
  DEBUG_OUT<<"Going to XListFonts...looking for "<<ename<<"\n";
  int fcount=0;
  char **xfl=XListFonts(parent->getDisplay(),ename,255,&fcount);
    DEBUG_OUT<<"(fcount was "<<fcount<<")\n";
    if(fcount) {for(t=0;t<fcount;t++) {
      DEBUG_OUT<<t<<": "<<xfl[t]<<"\n";
    }
    ret=strdup(xfl[0]);  // arbitrarily return the first match we got...
    XFreeFontNames(xfl);
  }
  else DEBUG_OUT<<"Got an fcount of 0, no such font!\n";
#endif // ASYS_X11
  if(ret) DEBUG_OUT<<"queryFontAvail found and returning '"<<ret<<"'.\n";
  return ret;
}


// FIXME: This is just the start of general font handling...much to be done
void AWindowAbstract::setFont(char *name,unsigned int size,unsigned int flags)
{
  //if(charMapped) { resizeCharMap(); }  // to be safe...
  unsigned int actualSize=size;
  if(!name) {
    name=(char *)"helvetica";
    if(flags==A_FONT_NONPROP) name=(char *)"courier";
    DEBUG_OUT<<"setFont using default font "<<name<<"\n";
  }
  if(!flags) {
    if(!strcmp("courier",name)) flags=A_FONT_NONPROP;
  }
#ifdef ASYS_WINDOWS
  if(flags==A_FONT_NONPROP) textProp=false; else textProp=true;
  avgCharWidth=parent->getAvgCharWidth();  // FIXME: Need to get actual value!
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
#endif // USE_QUARTZ
  short fnum=0;
  unsigned char m_name[256];
  CopyCStringToPascal(name,m_name);
#ifdef USE_QUARTZ
#else
  GetFNum((StringPtr)m_name,&fnum);
  if(!fnum) DEBUG_OUT<<"Mac couldn't find font!\n";
  else TextFont(fnum);
  TextSize(size);
#endif // USE_QUARTZ
  //actualSize=size+3;
  avgCharWidth=parent->getAvgCharWidth();  // FIXME: Need to get actual value!
#endif // ASYS_MAC
#ifdef ASYS_OS2
  FATTRS fat;
  fat.usRecordLength=sizeof(FATTRS);
  fat.fsSelection=0;
  fat.lMatch=0L;
  fat.idRegistry=0;
  fat.usCodePage=850;
  fat.lMaxBaselineExt=size;
  fat.lAveCharWidth=0;
  fat.fsType=0;
  fat.fsFontUse=FATTR_FONTUSE_OUTLINE;
  strcpy(fat.szFacename,name);
  GpiCreateLogFont(winHPS,(PSTR8)NULL,1L,&fat);
  GpiSetCharSet(winHPS,1L);
  avgCharWidth=parent->getAvgCharWidth();  // FIXME: Need to get actual value!
#endif // ASYS_OS2
#ifdef ASYS_POSIX
  actualSize=8;
  avgCharWidth=parent->getAvgCharWidth();
#endif // ASYS_POSIX
#ifdef ASYS_AMIGA
  // TODO: need to actually do this!
  actualSize=8;
  avgCharWidth=parent->getAvgCharWidth();
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  DEBUG_OUT<<"Going to XLoadQueryFont(,"<<name<<")...\n";
  if(xfs) XFreeFont(parent->getDisplay(),xfs);
  xfs=XLoadQueryFont(parent->getDisplay(),name);
  if(xfs) {
    DEBUG_OUT<<"Got an xfs, going to XSetFont...\n";
    XSetFont(parent->getDisplay(),TheGC,xfs->fid);
    // NOTE: Don't change the size yet...
  }
  else std::cerr<<"setFont couldn't find that font!\n";
#endif // ASYS_X11
#ifdef ASYS_PALM
  if(flags==A_FONT_NONPROP) textProp=false; else textProp=true;
  //if(size>9) FntSetFont(largeFont);
  //else FntSetFont(stdFont);
  actualSize=FntCharHeight();
  if(textProp) avgCharWidth=FntAverageCharWidth();
  else {
    // NOTE: Should be wide enough...
    avgCharWidth=FntCharWidth('X');
  }
  if(textProp) {
    for(unsigned int t=0;t<256;t++) glyphOffset[t]=0;
  }
  else {
    unsigned int otry=0,foo=avgCharWidth/2;
    for(unsigned int t=0;t<256;t++) {
      otry=foo-(TxtCharWidth(t)/2);
      if(otry>avgCharWidth) otry=0;
      glyphOffset[t]=otry;
    }
  }
#endif // ASYS_PALM
  if(oldFontHeight!=size) fontHeight=actualSize;
  sync();
  //if(charMapped) newCharMap();
  //DEBUG_OUT<<"Using fontHeight of "<<fontHeight<<"\n";
}


void AWindowAbstract::setFontSize(unsigned int size,unsigned int flags)
{
  //if(charMapped) { resizeCharMap(); }  // to be safe...
  unsigned int actualSize=size;
#ifdef ASYS_WINDOWS
  if(flags==A_FONT_NONPROP) textProp=false; else textProp=true;
  avgCharWidth=parent->getAvgCharWidth();  // FIXME: Need to get actual value!
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
  TextSize(size);
#endif // USE_QUARTZ
  //actualSize=size+3;
  avgCharWidth=parent->getAvgCharWidth();  // FIXME: Need to get actual value!
#endif // ASYS_MAC
#ifdef ASYS_OS2
  // TODO: why is this commented out?
/*
  FATTRS fat;
  fat.usRecordLength=sizeof(FATTRS);
  fat.fsSelection=0;
  fat.lMatch=0L;
  fat.idRegistry=0;
  fat.usCodePage=850;
  fat.lMaxBaselineExt=size;
  fat.lAveCharWidth=0;
  fat.fsType=0;
  fat.fsFontUse=FATTR_FONTUSE_OUTLINE;
  strcpy(fat.szFacename,name);
  GpiCreateLogFont(winHPS,(PSTR8)NULL,1L,&fat);
  GpiSetCharSet(winHPS,1L);
  */
  avgCharWidth=parent->getAvgCharWidth();  // FIXME: Need to get actual value!
#endif // ASYS_OS2
#ifdef ASYS_POSIX
  actualSize=8;
  avgCharWidth=parent->getAvgCharWidth();
#endif // ASYS_POSIX
#ifdef ASYS_AMIGA
  // TODO: need to actually do this!
  actualSize=8;
  avgCharWidth=parent->getAvgCharWidth();
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  // TODO: need to actually do this!
  actualSize=14;
  avgCharWidth=parent->getAvgCharWidth();
#endif // ASYS_X11
#ifdef ASYS_PALM
  if(flags==A_FONT_NONPROP) textProp=false; else textProp=true;
  //if(size>9) FntSetFont(largeFont);
  //else FntSetFont(stdFont);
  actualSize=FntCharHeight();
  if(textProp) avgCharWidth=FntAverageCharWidth();
  else {
    // NOTE: Should be wide enough...
    avgCharWidth=FntCharWidth('X');
  }
  if(textProp) {
    for(unsigned int t=0;t<256;t++) glyphOffset[t]=0;
  }
  else {
    unsigned int otry=0,foo=avgCharWidth/2;
    for(unsigned int t=0;t<256;t++) {
      otry=foo-(TxtCharWidth(t)/2);
      if(otry>avgCharWidth) otry=0;
      glyphOffset[t]=otry;
    }
  }
#endif // ASYS_PALM
  if(oldFontHeight!=size) fontHeight=actualSize;
  sync();
  //if(charMapped) newCharMap();
  //DEBUG_OUT<<"Using fontHeight of "<<fontHeight<<"\n";
}


// NOTE: This may be called from the event loop in response to a REDRAW event.
// NOTE: foo is system dependant and may be NULL or pretty much anything...
// NOTE: This isn't a user routine...
void AWindowAbstract::drawBackground(void *foo)
{
#ifdef ASYS_DOS
  if(!parent) return;
  if(!parent->getTheBits()) return;
  if(haveFocus) parent->getTheBits()->clear(x,y,width,height);
#endif // ASYS_DOS
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(offscreenImage) offscreenImage->clear(x,y,width,height);
#else
  HDC hdc=GetDC(ahWnd);
  //SetBkColor(hdc,RGB(bred,bgreen,bblue));
  SelectObject(hdc,backBrush);
  RECT rect;
  rect.left=0;  rect.top=0;  rect.right=width;  rect.bottom=height;
  FillRect(hdc,&rect,backBrush);
  ReleaseDC(ahWnd,hdc);
  //DEBUG_OUT<<"Window is "<<width<<" by "<<height<<"\n";
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
  //WindowPtr foowp=(WindowPtr)foo;
  //EraseRgn(foowp->visRgn);
#ifdef USE_QUARTZ
#else
  MacSetPort(TheGP);
#if TARGET_API_CARBON
  Rect theRect;
  GetPortBounds(TheGP,&theRect);
  EraseRect(&theRect);
#else
  EraseRect(&TheGP->portRect);
#endif
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  SetRast(TheRP,background);
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  XSetWindowBackground(parent->getDisplay(),TheWin,background);
  //XSync(parent->getDisplay(),false);
  XClearWindow(parent->getDisplay(),TheWin);
  //XSync(parent->getDisplay(),false);
#endif // ASYS_X11
#ifdef ASYS_OS2
  //GpiErase(winHPS);
  unsigned long tfore=foreground;
  setForeground(background);
  POINTL coords;
  coords.x=0;  coords.y=0;
  GpiMove(winHPS,&coords);  coords.x=width;  coords.y=height;
  GpiBox(winHPS,DRO_FILL,&coords,0,0);
  setForeground(tfore);
#endif // ASYS_OS2
}


#ifdef ASYS_AMIGA
char *AWindowAbstract::handleAppMessage(unsigned long signals)
{
  if(!WorkbenchBase) { DEBUG_OUT<<"No wb.\n"; err.set(); return 0; }
  if(!appwin) { DEBUG_OUT<<"No appwin.\n"; err.set(); return 0; }
  if(!awport) { DEBUG_OUT<<"No awport.\n"; err.set(); return 0; }
  if(!appsignal&signals) { DEBUG_OUT<<"Bad signals.\n"; err.set(); return 0; }
  char *retVal=NULL;
  AppMessage *amsg;
  amsg=(AppMessage *)GetMsg(awport);
  while(amsg) {
    unsigned long nargs=amsg->am_NumArgs;
    if(nargs!=1) DEBUG_OUT<<"Passed "<<nargs<<" app args!\n";
    // Use strdup because Workbench will free args!
    retVal=strdup((char *)amsg->am_ArgList[0].wa_Name);
    if(retVal) {
      DEBUG_OUT<<retVal<<" ("<<(long)retVal<<") dropped.\n";
      if(amsg->am_ArgList[0].wa_Lock)
        CurrentDir(amsg->am_ArgList[0].wa_Lock);
    }
    else //DEBUG_OUT.warning("Non-file icon dropped.\n");
    ReplyMsg((Message *)amsg);
    amsg=(AppMessage *)GetMsg(awport);
  }
  return retVal;
}
#endif // ASYS_AMIGA


#ifdef ASYS_X11
void AWindowAbstract::waitForFirstValidEvent()
{
  DEBUG_OUT<<"waitForFirstValidEvent()\n";
  AEvent e;
  bool wait=true;
  while(wait) {
    DEBUG_OUT<<"Gonna wait...\n";
    e.wait(this);
    DEBUG_OUT<<"x11type="<<x11_return_event_name(e.x11type)<<" ("<<e.x11type<<")\n";
    if(e.x11type==ConfigureNotify) wait=false;
    //if(e.x11type==MapNotify) wait=false;
    //if(e.x11type==FocusIn) wait=false;
  }
  //XSync(parent->getDisplay(),false);
}
#endif // ASYS_X11


#ifdef ASYS_X11
void AWindowAbstract::waitForFirstFocus()
{
  DEBUG_OUT<<"waitForFirstFocus()\n";
  AEvent e;
  bool wait=true;
  while(wait) {
    DEBUG_OUT<<"Gonna wait...\n";
    e.wait(this);
    DEBUG_OUT<<"x11type="<<x11_return_event_name(e.x11type)<<" ("<<e.x11type<<")\n";
    //if(e.x11type==ConfigureNotify) wait=false;
    //if(e.x11type==MapNotify) wait=false;
    if(e.x11type==FocusIn) wait=false;
  }
  //XSync(parent->getDisplay(),false);
}
#endif // ASYS_X11


#ifdef ASYS_X11
void AWindowAbstract::setXWindowHack(Window xwin)
{
  Display *theDisplay=getParent()->getDisplay();
  if(TheGC) XFreeGC(theDisplay,TheGC);
  if(TheWin) {
    XUnmapWindow(theDisplay,TheWin);
    XDestroyWindow(theDisplay,TheWin);
  }
  TheWin=xwin;
  unsigned long gcmask=GCForeground|GCBackground|GCFunction;
  XGCValues gcvals;

  gcvals.foreground=BlackPixel(theDisplay,0);
  gcvals.background=WhitePixel(theDisplay,0);
  gcvals.function=GXcopy;
  TheGC=XCreateGC(theDisplay,TheWin,gcmask,&gcvals);
  XSetGraphicsExposures(theDisplay,TheGC,false);
  //waitForFirstFocus();
  //resize(width,height);
  //focus();
}
#endif // ASYS_X11


void AWindowAbstract::drawCursor()
{
  if(!needToDrawCursor) return; // Don't bother, we don't have to
  int xadj1=0,xadj2=0,yadj1=0,yadj2=0;
  unsigned int oldX=penX,oldY=penY;
#ifdef ASYS_AMIGA
  yadj1=3;
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  yadj1=2;
#endif // ASYS_X11
#ifdef ASYS_MAC
  yadj1=4;  yadj2=2;
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
  //yadj1=1; yadj2=2;
#endif // ASYS_WINDOWS
  moveTo(penX+xadj1,penY+yadj1);
  drawFilledRect((fontHeight/2)-xadj2,fontHeight-yadj2);
  moveTo(oldX,oldY);
}


void AWindowAbstract::eraseCursor()
{
  if(!needToDrawCursor) return; // Don't bother, we don't have to
  int xadj1=0,xadj2=0,yadj1=0,yadj2=0;
  unsigned int oldX=penX,oldY=penY;
#ifdef ASYS_AMIGA
  yadj1=3;
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  yadj1=2;
#endif // ASYS_X11
#ifdef ASYS_MAC
  yadj1=4;  yadj2=2;
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
  xadj2=-1; yadj2=-1;
#endif // ASYS_WINDOWS
#ifdef ASYS_PALM
  xadj2=-1; yadj2=-1;
#endif // ASYS_PALM
  moveTo(penX+xadj1,penY+yadj1);
  eraseRect((fontHeight/2)-xadj2,fontHeight-yadj2);
  moveTo(oldX,oldY);
}


void AWindowAbstract::saveCursorPosition()
{
  oldPenX=penX;  oldPenY=penY;
}


void AWindowAbstract::restoreCursorPosition()
{
  penX=oldPenX;  penY=oldPenY;
}


void AWindowAbstract::print()
{
  DEBUG_OUT<<"AWindowAbstract "<<name<<": ";
  DEBUG_OUT<<"@"<<x<<","<<y<<" ";
  DEBUG_OUT<<width<<"x"<<height<<"x"<<depth;
  DEBUG_OUT<<"("<<nPlanes<<")\n";
  DEBUG_OUT<<"numRows="<<numRows<<" "<<"fontHeight="<<fontHeight<<"\n";
}


// FIXME: This can't do much if the window isn't charMapped...
// FIXME:: In the future we could use save unders or something like that...
void AWindowAbstract::redraw()
{
  //DEBUG_OUT<<"AWindowAbstract::redraw()...\n";
  if(charMapped) {
    clear();
    //drawCharMap();
  }
  //else DEBUG_OUT<<"Window not charMapped...\n";
}


// Err, what is this for again?
void AWindowAbstract::drawImageChar(char c)
{
  // FIXME: don't lock the font to 8x8...
  /*
  ABitmap *tFont=parent->getTheFont();
  if(tFont) {
    int cx=0,cy=0;
    c|=0x80;
    cx=c&0xf;  cy=(c&0xf0)>>4;
    cx=cx*8;  cy=cy*8;
    if(offscreenImage) offscreenImage->copyBlock(tFont,cx,cy,penX,penY,8,8);
    penX+=8;
  }
  */
}


void AWindowAbstract::unfocus()
{
  // Mostly placeholder for now.
  haveFocus=false;
}


void AWindowAbstract::centerOnDisplay()
{
  sync();
  UINT pw=parent->getWidth(),ph=parent->getHeight();
  UINT tx=(pw/2)-(width/2);
  UINT ty=(ph/2)-(height/2);
  move(tx,ty);
}


void AWindowAbstract::flip(bool dx, bool dy)
{
  std::cerr<<"AWindowAbstract flip called!\n"; exit(5);
}


void AWindowAbstract::scale(int xFactor, int yFactor)
{
  std::cerr<<"AWindowAbstract scale called!\n"; exit(5);
}


void AWindowAbstract::scroll(unsigned int tx,unsigned int ty,unsigned int tw,unsigned int th, int dx, int dy)
{
  std::cerr<<"AWindowAbstract scroll called!\n"; exit(5);
}


void AWindowAbstract::oval(int tx,int ty,int twidth, int theight)
{
  std::cerr<<"AWindowAbstract oval called!\n"; exit(5);
}


void AWindowAbstract::fillOval(int tx,int ty,int twidth, int theight)
{
  std::cerr<<"AWindowAbstract fillOval called!\n"; exit(5);
}


void AWindowAbstract::fillRect(int tx,int ty,int twidth, int theight)
{
  std::cerr<<"AWindowAbstract fillRect called!\n"; exit(5);
}


unsigned int AWindowAbstract::readPixel(unsigned int x, unsigned int y)
{
  std::cerr<<"AWindowAbstract readPixel called!\n"; exit(5);
}


void AWindowAbstract::rotate(APrivateBitmap *dest,int angle)
{
  std::cerr<<"AWindowAbstract rotate called!\n"; exit(5);
}


void AWindowAbstract::scale(APrivateBitmap *dest,int xFactor, int yFactor)
{
  std::cerr<<"AWindowAbstract scale called!\n"; exit(5);
}


#ifdef ASYS_GLUT
void AWindowAbstract::glutSetSize(unsigned int w,unsigned int h)
{
  width=w;  height=h;
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
float AWindowAbstract::glutTransX(unsigned x)
{
  return ((float)(width-x)/(float)(width>>1)-1);
}
#endif // ASYS_GLUT

#ifdef ASYS_GLUT
float AWindowAbstract::glutTransY(unsigned y)
{
  return ((float)(height-y)/(float)(height>>1)-1);
}
#endif // ASYS_GLUT
