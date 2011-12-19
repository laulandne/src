
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/AWindow.h>
#include <guilib/ADisplay.h>
#include <guilib/AEvent.h>

#include <portable/nickcpp/ABitmap.h>
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


AWindow *aDefaultAWindow=NULL;


////////////////////////////////////////////////////////////////////////////////
//  AWindow Class
////////////////////////////////////////////////////////////////////////////////

AWindow::AWindow()
{
  DEBUG_OUT<<"AWindow()\n";
  init();
  if(!aDefaultADisplay) aDefaultADisplay=new ADisplay;
  parent=aDefaultADisplay;
  unsigned int newWidth=640,newHeight=480;  // arbitrary
  if(parent) {
    newWidth=parent->getWidth()/2;  newHeight=parent->getHeight()/2;
  }
  openWin((char *)"New Window",newWidth,newHeight);
  aDefaultAWindow=this;
}


AWindow::AWindow(const char *newName)
{
  DEBUG_OUT<<"AWindow("<<newName<<")\n";
  init();
  if(!aDefaultADisplay) aDefaultADisplay=new ADisplay;
  parent=aDefaultADisplay;
  unsigned int newWidth=640,newHeight=480;  // arbitrary
  if(!parent) {
    newWidth=parent->getWidth()/2;  newHeight=parent->getHeight()/2;
  }
  openWin(newName,newWidth,newHeight);
  aDefaultAWindow=this;
}


AWindow::AWindow(const char *newName, unsigned int newWidth, unsigned int newHeight)
{
  DEBUG_OUT<<"AWindow("<<newName<<","<<newWidth<<","<<newHeight<<")\n";
  init();
  if(!aDefaultADisplay) aDefaultADisplay=new ADisplay;
  parent=aDefaultADisplay;
  openWin(newName,newWidth,newHeight);
  aDefaultAWindow=this;
}


AWindow::AWindow(const char *newName, unsigned int newWidth, unsigned int newHeight, int newx, int newy)
{
  DEBUG_OUT<<"AWindow("<<newName<<","<<newWidth<<","<<newHeight<<","<<newx<<","<<newy<<")\n";
  init();
  if(!aDefaultADisplay) aDefaultADisplay=new ADisplay;
  parent=aDefaultADisplay;
  openWin(newName,newWidth,newHeight,newx,newy);
  aDefaultAWindow=this;
}


AWindow::AWindow(unsigned int newWidth, unsigned int newHeight)
{
  DEBUG_OUT<<"AWindow("<<newWidth<<","<<newHeight<<")\n";
  init();
  if(!aDefaultADisplay) aDefaultADisplay=new ADisplay;
  parent=aDefaultADisplay;
  openWin((char *)"New Window",newWidth,newHeight);
  aDefaultAWindow=this;
}


AWindow::AWindow(ADisplay *someDisplay, const char *newName, unsigned int newWidth, unsigned int newHeight)
{
  DEBUG_OUT<<"AWindow(someDisplay,"<<newName<<","<<newWidth<<","<<newHeight<<")\n";
  init();
  parent=someDisplay;
  openWin(newName,newWidth,newHeight);
  aDefaultAWindow=this;
}


AWindow::AWindow(ADisplay *someDisplay, const char *newName, unsigned int newWidth, unsigned int newHeight, int newx, int newy)
{
  DEBUG_OUT<<"AWindow(someDisplay,"<<newName<<","<<newWidth<<","<<newHeight<<","<<newx<<","<<newy<<")\n";
  init();
  parent=someDisplay;
  openWin(newName,newWidth,newHeight,newx,newy);
  aDefaultAWindow=this;
}


AWindow::AWindow(ADisplay *someDisplay)
{
  DEBUG_OUT<<"AWindow(someDisplay)\n";
  init();
  parent=someDisplay;
  unsigned int newWidth=640,newHeight=480;  // arbitrary
  if(!parent) {
    newWidth=parent->getWidth()/2;  newHeight=parent->getHeight()/2;
  }
  openWin((char *)"New Window",newWidth,newHeight);
  aDefaultAWindow=this;
}


AWindow::AWindow(ADisplay *someDisplay, const char *newName)
{
  DEBUG_OUT<<"AWindow(someDisplay,"<<newName<<")\n";
  init();
  parent=someDisplay;
  unsigned int newWidth=640,newHeight=480;  // arbitrary
  if(!parent) {
    newWidth=parent->getWidth()/2;  newHeight=parent->getHeight()/2;
  }
  openWin(newName,newWidth,newHeight);
  aDefaultAWindow=this;
}


AWindow::AWindow(ADisplay *someDisplay, unsigned int newWidth, unsigned int newHeight)
{
  DEBUG_OUT<<"AWindow(someDisplay,"<<newWidth<<","<<newHeight<<")\n";
  init();
  parent=someDisplay;
  openWin((char *)"New Window",newWidth,newHeight);
  aDefaultAWindow=this;
}


AWindow::~AWindow()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"~AWindow()\n";
#endif // DEBUG_VERBOSE
  // NOTE: We assume the actual platform dependant ~ is taken care of by AWindowAbstract
  if(aDefaultAWindow==this) aDefaultAWindow=NULL;
}


