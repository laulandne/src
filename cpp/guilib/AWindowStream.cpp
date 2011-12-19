
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/AWindowStream.h>

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


////////////////////////////////////////////////////////////////////////////////
//  AWindowStream Class
////////////////////////////////////////////////////////////////////////////////

AWindowStream::AWindowStream(char *nm)
{
  init();
  w=new AWindow(nm);
  if(!w) DEBUG_OUT<<"AWindowStream didn't get AWindow!\n";
  //d=new ADebug(nm);
  //if(!d) DEBUG_OUT<<"AWindowStream didn't get ADebug!\n";
}


AWindowStream::~AWindowStream()
{
  if(w) delete w;
  w=(AWindow *)NULL;
  //if(d) delete d;
  //d=(ADebug *)NULL;
}


void AWindowStream::init()
{
  w=(AWindow *)NULL;
  //d=(ADebug *)NULL;
}



AWindowStream& AWindowStream::fatal(const char *s)
{
  if(w) *w<<s;
  //if(d) d->fatal(s);
  return *this;
}


AWindowStream& AWindowStream::warning(const char *s)
{
  if(w) *w<<s;
  //if(d) d->fatal(s);
  return *this;
}


AWindowStream& AWindowStream::stop(const char *s)
{
  if(w) *w<<s;
  //if(d) d->stop(s);
  return *this;
}


AWindowStream& AWindowStream::operator<<(unsigned long s)
{
  if(w) *w<<s;
  //if(d) *d<<s;
  return *this;
}


AWindowStream& AWindowStream::operator<<(long s)
{
  if(w) *w<<s;
  //if(d) *d<<s;
  return *this;
}


AWindowStream& AWindowStream::operator<<(unsigned int s)
{
  if(w) *w<<s;
  //if(d) *d<<s;
  return *this;
}


AWindowStream& AWindowStream::operator<<(int s)
{
  if(w) *w<<s;
  //if(d) *d<<s;
  return *this;
}


AWindowStream& AWindowStream::operator<<(const char *s)
{
  if(w) *w<<s;
  //if(d) *d<<s;
  return *this;
}


AWindowStream& AWindowStream::operator<<(char s)
{
  if(w) *w<<s;
  //if(d) *d<<s;
  return *this;
}

