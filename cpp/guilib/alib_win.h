#ifndef PLATFORM_ALIB_WIN_H
#define PLATFORM_ALIB_WIN_H


#include "ATypeDefs.h"
#include "APlatform.h"


#ifdef ASYS_WIN32CE
#include <bldver.h>
#if CE_MAJOR_VER >2
#include <aygshell.h>
#include <sip.h>
#endif
#endif // ASYS_WIN32CE


#if CE_MAJOR_VER <3
#define ALIB_NEED_OUR_UNIX_IO 1
#endif


class AEvent;


// This is a Win 3.1 32-bit extender, not true Win32...
#ifdef __WINDOWS_386__
#define OLD_WINDOWS 1
#define DEVMODE void
#endif


// If using windows headers that don't have these defined
#ifndef VER_PLATFORM_WIN32s
#define VER_PLATFORM_WIN32s             0
#endif
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS      1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT           2
#endif


#ifdef ASYS_WINDOWS
#ifdef ASYS_WIN16
#ifndef REALLY_OLD_WINDOWS
// Only for on Win16 where we link run time...
extern HINSTANCE alib_mmSystemHandle;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN16
// The next three are as passed to WinMain()
extern HINSTANCE alib_ahInst;
extern HINSTANCE alib_ahPrevInst;
extern int alib_aNShow;
extern void *alib_aCmdLine;
extern WPARAM alib_awParam;   // Word param event loop return value
extern HWND alib_ahWnd;
extern HMENU alib_menuHandle;  // Won't always be legit! (see activateMenu)
extern unsigned long alib_lastModifier;  // if non-zero, modifier key held down
#ifdef ASYS_WIN32CE
#if CE_MAJOR_VER >2
extern SIPINFO alib_sinfo;
extern HINSTANCE alib_aygshellHandle;
#else
extern "C" bool isdigit(char c);
#endif
#endif // ASYS_WIN32CE
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
//extern JOYCAPS alib_joyc;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN32CE
extern unsigned int alib_qtVer,alib_qtRev;
#endif // ASYS_WINDOWS

#ifdef ASYS_WINDOWS
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE 3
#endif // VER_PLATFORM_WIN32_CE
#endif // ASYS_WINDOWS

#ifdef ASYS_WINDOWS
extern "C" LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message,
  WPARAM wParam, LPARAM lParam);
#ifdef __WATCOM_CPLUSPLUS__
// NOTE: Due to some stupid Watcom thing...
#define main(a,b) mainFunc(a,b)
#endif // __WATCOM_CPLUSPLUS__
extern "C" int __cdecl main(int argc,char **argv);
#endif // ASYS_WINDOWS


#ifdef ASYS_WIN32CE
int myStat(char *filename,struct stat *s);
int myFstat(int fd,struct stat *s);
char *myGetcwd(char *dir,unsigned int len);
int myChdir(char *dir);
bool myIsdigit(char c);
FILE *myFopen(const char *n, const char *m);
int myFclose(FILE *f);
size_t myFread(void *p, size_t s, size_t n, FILE *f);
size_t myFwrite(const void *p, size_t s, size_t n, FILE *f);
int myFflush(FILE *f);
int myFseek(FILE * f, long n, int w);
long  myFtell(FILE *f);
int myFputc(int c, FILE *f);
int myFgetc(FILE *f);
#endif // ASYS_WIN32CE


#ifdef ASYS_WIN32CE
#define stat(a,b) myStat(a,b)
#define fstat(a,b) myFstat(a,b)
#define getcwd(a,b) myGetcwd(a,b)
#define chdir(a) myChdir(a)
#ifdef ALIB_NEED_OUR_UNIX_IO
#define isdigit(a) myIsdigit(a)
#define fopen(a,b) myFopen(a,b)
#define fclose(a) myFclose(a)
#define fread(a,b,c,d) myFread(a,b,c,d)
#define fwrite(a,b,c,d) myFwrite(a,b,c,d)
#define fflush(a) myFflush(a)
#define fseek(a,b,c) myFseek(a,b,c)
#define ftell(a) myFtell(a)
#define fputc(a,b) myFputc(a,b)
#define fgetc(a) myFgetc(a)
#endif // ALIB_NEED_OUR_UNIX_IO
#endif // ASYS_WIN32CE


#endif // PLATFORM_ALIB_WIN_H
