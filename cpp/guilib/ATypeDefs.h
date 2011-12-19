#ifndef ALIB_ATYPEDEFS_H
#define ALIB_ATYPEDEFS_H


#include <guilib/APlatform.h>


#define DEBUG_VERBOSE 1
#define DEBUG 1


//#include <guilib/Sections.h>


//#define ALIB_16BIT_INTS 1
//#define ALIB_SMALL_MEM 1
//#define DECODERLIB_NO_DECODERS 1
//#define DECODERLIB_ONLY_BASIC 1

#define ALIB_HAVE_FILEIO 1
#define ALIB_HAVE_STRINGIO 1
#define ALIB_HAVE_UNIX_IO 1


////////////////////////////////////////////////////////////////////////////////
//  Generic platform assumptions
////////////////////////////////////////////////////////////////////////////////

#define ALIB_HAVE_ALPHASORT 1
#define ALIB_HAVE_CHDIR 1
//#define ALIB_HAVE_UNDERSCORE_CHDIR 1
#define ALIB_HAVE_CLOCK 1
//#define ALIB_HAVE_UNDERSCORE_CLOSE 1
#define ALIB_HAVE_EXIT 1
#define ALIB_HAVE_FSTAT 1
//#define ALIB_HAVE_UNDERSCORE_FSTAT 1
#define ALIB_HAVE_GETCWD 1
//#define ALIB_HAVE_UNDERSCORE_GETCWD 1
//#define ALIB_HAVE_GETWD 1
//#define ALIB_HAVE_UNDERSCORE_LSEEK 1
//#define ALIB_HAVE_UNDERSCORE_OPEN 1
//#define ALIB_HAVE_UNDERSCORE_READ 1
#define ALIB_HAVE_RENAME 1
#define ALIB_HAVE_SCANDIR 1
#define ALIB_HAVE_SSCANF 1
//#define ALIB_HAVE_UNDERSCORE_STAT 1
#define ALIB_HAVE_STAT 1
//#define ALIB_HAVE_TELL 1
//#define ALIB_HAVE_UNDERSCORE_TELL 1
//#define ALIB_HAVE_UNDERSCORE_WRITE 1
//#define ALIB_HAVE_GETPID 1
#define ALIB_HAVE_STRNCASECMP 1
//#define ALIB_HAVE_UNDERSCORE_STRNICMP 1

#define ALIB_HAVE_DIRENT 1
//#define ALIB_HAVE_DIRECT 1
#define ALIB_HAVE_DIRENT_D_TYPE 1
//#define ALIB_HAVE_WCHAR_T 1

#define ALIB_HAVE_CSTDLIB 1
#define ALIB_HAVE_DIRENT_H 1
//#define ALIB_HAVE_DIRECT_H 1
#define ALIB_HAVE_SYS_DIR_H 1
//#define ALIB_HAVE_FCNTL_H 1
#define ALIB_HAVE_SYS_FCNTL_H 1
//#define ALIB_HAVE_LIBC_H 1
#define ALIB_HAVE_MATH_H 1
//#define ALIB_HAVE_STAT_H 1
#define ALIB_HAVE_SYS_STAT_H 1
#define ALIB_HAVE_STDIO_H 1
#define ALIB_HAVE_STDLIB_H 1
#define ALIB_HAVE_STRING_H 1
#define ALIB_HAVE_TIME_H 1
#define ALIB_HAVE_SYS_UNISTD_H 1
#define ALIB_HAVE_UNISTD_H 1
//#define ALIB_HAVE_SYS_UTSNAME_H 1
//#define ALIB_HAVE_WCHAR_H 1
#define ALIB_HAVE_SIGNAL_H 1
#define ALIB_HAVE_CTYPE_H 1
#define ALIB_HAVE_IOSTREAM 1
#define ALIB_HAVE_LIMITS 1
//#define ALIB_HAVE_CWCHAR 1
// #define ALIB_HAVE_ENDIAN_H 1
// #define ALIB_HAVE_MACHINE_ENDIAN_H 1


////////////////////////////////////////////////////////////////////////////////
//  Overrides by compiler
////////////////////////////////////////////////////////////////////////////////

#ifdef __MWERKS__
#undef ALIB_HAVE_STRNCASECMP
//#undef ALIB_HAVE_GETWD
#undef ALIB_HAVE_SYS_STAT_H
#define ALIB_HAVE_STAT_H 1
#undef ALIB_HAVE_SYS_UNISTD_H
#undef ALIB_HAVE_FSTAT
#undef ALIB_HAVE_STAT
//#define ALIB_HAVE_UNDERSCORE_FSTAT 1
//#define ALIB_HAVE_UNDERSCORE_STAT 1
#undef ALIB_HAVE_SYS_FCNTL_H
#define ALIB_HAVE_FCNTL_H 1
#undef ALIB_HAVE_DIRENT_H
#undef ALIB_HAVE_SYS_DIR_H
#undef ALIB_HAVE_ALPHASORT
#undef ALIB_HAVE_SCANDIR
#undef ALIB_HAVE_DIRENT
#endif // __MWERKS__

#ifdef __GNUC__
#if __GNUC__ > 2
#undef ALIB_HAVE_IOSTREAM
#endif
#endif // __GNUC__

#ifdef _MSC_VER
#define ALIB_HAVE_IO_H 1
#undef ALIB_HAVE_DIRENT_H
#undef ALIB_HAVE_SYS_DIR_H
#undef ALIB_HAVE_SYS_UNISTD_H
#undef ALIB_HAVE_UNISTD_H
#undef ALIB_HAVE_SYS_FCNTL_H
#define ALIB_HAVE_FCNTL_H 1
#define ALIB_HAVE_UNDERSCORE_TELL 1
#undef ALIB_HAVE_CHDIR
#define ALIB_HAVE_UNDERSCORE_CHDIR 1
#undef ALIB_HAVE_GETCWD
#define ALIB_HAVE_UNDERSCORE_GETCWD 1
#define ALIB_HAVE_DIRECT_H 1
#define ALIB_HAVE_UNDERSCORE_OPEN 1
#define ALIB_HAVE_UNDERSCORE_CLOSE 1
#define ALIB_HAVE_UNDERSCORE_READ 1
#define ALIB_HAVE_UNDERSCORE_WRITE 1
#define ALIB_HAVE_UNDERSCORE_LSEEK 1
#undef ALIB_HAVE_ALPHASORT
#undef ALIB_HAVE_SCANDIR
#undef ALIB_HAVE_DIRENT
#undef ALIB_HAVE_STRNCASECMP
#define ALIB_HAVE_UNDERSCORE_STRNICMP 1
#endif // _MSC_VER

#ifdef ASYS_CYGWIN
#undef ALIB_HAVE_DIRENT_D_TYPE
#undef ALIB_HAVE_SYS_DIR_H
#define __USE_W32_SOCKETS 1
//#define USE_SYS_TYPES_FD_SET 1
#endif // ASYS_CYGWIN

#ifdef ASYS_DJGPP
#undef ALIB_HAVE_SYS_UNISTD_H
#undef ALIB_HAVE_ALPHASORT
#undef ALIB_HAVE_SCANDIR
#undef ALIB_HAVE_DIRENT_D_TYPE
#endif // ASYS_DJGPP

#ifdef __WATCOM_CPLUSPLUS__
#undef ALIB_HAVE_CSTDLIB
#undef ALIB_HAVE_SYS_UNISTD_H
#undef ALIB_HAVE_SYS_FCNTL_H
#define ALIB_HAVE_FCNTL_H 1
#undef ALIB_HAVE_DIRENT_D_TYPE
#undef ALIB_HAVE_ALPHASORT
#undef ALIB_HAVE_SCANDIR
#undef ALIB_HAVE_SYS_DIR_H
#undef ALIB_HAVE_DIRENT_H
#define ALIB_HAVE_DIRECT_H 1
#ifdef ASYS_WIN386
#define UINT_ALREADY_DEFINED 1
#endif // ASYS_WIN386
#endif // __WATCOM_CPLUSPLUS__

#ifdef __SC__
#define ALIB_HAVE_IO_H 1
#undef ALIB_HAVE_ALPHASORT
#undef ALIB_HAVE_SCANDIR
//#define ALIB_HAVE_UNDERSCORE_GETCWD 1
///#undef ALIB_HAVE_GETCWD
#undef ALIB_HAVE_CSTDLIB
#undef ALIB_HAVE_IOSTREAM
#undef ALIB_HAVE_STRNCASECMP
//#undef ALIB_HAVE_CHDIR
//#define ALIB_HAVE_UNDERSCORE_CHDIR
#define ALIB_HAVE_DIRECT_H 1
#undef ALIB_HAVE_DIRENT_H
#undef ALIB_HAVE_DIRENT
#undef ALIB_HAVE_DIRECT
#define bool char
#define true 1
#define false 0
#define ALIB_NO_POSH 1
#endif // __SC__

#ifdef __MWERKS__
#define strdup(x) strcpy((char *)malloc(strlen(x)+1),x)
#endif // __MWERKS__


////////////////////////////////////////////////////////////////////////////////
//  Overrides by platform
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_PALM
#define ALIB_NO_POSH 1
#define INT32_IS_LONG 1
#undef ALIB_HAVE_STRINGIO
#undef ALIB_HAVE_FILEIO
#undef ALIB_HAVE_MATH_H
#undef ALIB_HAVE_STDIO_H
#undef ALIB_HAVE_STDLIB_H
#undef ALIB_HAVE_STRING_H
#undef ALIB_HAVE_SIGNAL_H
#undef ALIB_HAVE_CTYPE_H
#undef ALIB_HAVE_STAT_H
#undef ALIB_HAVE_IOSTREAM
#undef ALIB_HAVE_LIMITS
//#define UINT_ALREADY_DEFINED 1
#define USHORT_ALREADY_DEFINED 1
#define ULONG_ALREADY_DEFINED 1
#ifndef __fourbyteints__
#define ALIB_16BIT_INTS 1
#endif // __fourbyteints__
#define ALIB_SMALL_MEM 1
#endif // ASYS_PALM

#ifdef ASYS_PALM
#ifdef __GNUC__
#include <PalmOS.h>
#endif // __GNUC__
#ifdef __PALMOS_H__
// For palm os 3.5+ sdk (but not 6.x or higher)
#if PALMOS_SDK_VERSION > 0x5ff
#define ALIB_PALMOS6 1
#endif // PALMOS_SDK_VERSION
#ifndef ALIB_PALMOS6
#include <PalmCompatibility.h>
#endif // ALIB_PALMOS6
//#include <StdioPalm.h>
#else
// For palm os 3.1 sdk
#include <Pilot.h>
#include <PalmStdio.h>
#endif // __PALMOS_H__
#include <guilib/alib_palm.h>
#include <CharAttr.h>
#include <TxtGlue.h>
//#undef ALIB_HAVE_UNDERSCORE_STAT
#undef ALIB_HAVE_UNISTD_H
#undef ALIB_HAVE_SYS_UNISTD_H
#undef ALIB_HAVE_STAT_H
#undef ALIB_HAVE_WCHAR_H
#undef ALIB_HAVE_CSTDLIB
#undef ALIB_HAVE_SYS_STAT_H
#undef ALIB_HAVE_TIME_H
#undef ALIB_HAVE_MATH_H
#undef ALIB_HAVE_FCNTL_H
#undef ALIB_HAVE_FSTAT
#undef ALIB_HAVE_STAT
//#undef ALIB_HAVE_GETWD
#undef ALIB_HAVE_GETCWD
#undef ALIB_HAVE_CHDIR
#undef ALIB_HAVE_SSCANF
#undef ALIB_HAVE_CLOCK
//#define DECODERLIB_NO_DECODERS 1
#define DECODERLIB_ONLY_BASIC 1
#endif // ASYS_PALM

#ifdef ASYS_AMIGA
#include <dos/dos.h>
#define BYTE_ALREADY_DEFINED 1
#undef ALIB_HAVE_IOSTREAM
#endif // ASYS_AMIGA

#ifdef ASYS_WINDOWS
#define WIN32_LEAN_AND_MEAN 1
#define __USE_W32_SOCKETS 1
#include <windows.h>
//#include <mmsystem.h>
//#include <winsock.h>
#ifndef NULLHANDLE
#define NULLHANDLE NULL
#endif // NULLHANDLE
#define ALREADY_HAVE_UINT64 1
#define ALIB_HAVE_WCHAR_T 1
#endif // ASYS_WINDOWS

#ifdef ASYS_WIN16
#define ALIB_16BIT_INTS 1
#define ALIB_SMALL_MEM 1
#endif // ASYS_WIN16

#ifdef ASYS_DOS16
#define ALIB_16BIT_INTS 1
#define ALIB_SMALL_MEM 1
#endif // ASYS_DOS16

#ifdef ASYS_NEXT
#undef ALIB_HAVE_GETCWD
#undef ALIB_HAVE_SYS_UNISTD_H
#define ALIB_HAVE_LIBC_H 1
#undef ALIB_HAVE_DIRENT_D_TYPE
#undef ALIB_HAVE_DIRENT
#define ALIB_HAVE_DIRECT 1
#define ALIB_HAVE_SYS_UTSNAME_H 1
#endif // ASYS_NEXT

#ifdef ASYS_WIN32CE
#undef ALIB_HAVE_IOSTREAM
#undef ALIB_HAVE_SIGNAL_H
#undef ALIB_HAVE_UNIX_IO
#undef ALIB_HAVE_SYS_STAT_H
#undef ALIB_HAVE_DIRECT_H
#undef ALIB_HAVE_TIME_H
#undef ALIB_HAVE_CLOCK
#undef ALIB_HAVE_UNDERSCORE_GETCWD
#undef ALIB_HAVE_UNDERSCORE_CHDIR
#undef ALIB_HAVE_UNDERSCORE_TELL
#undef ALIB_HAVE_FSTAT
#undef ALIB_HAVE_STAT
#undef ALIB_HAVE_FCNTL_H
#undef ALIB_HAVE_IO_H
#undef ALIB_HAVE_WCHAR_H
#undef ALIB_HAVE_CSTDLIB
#undef USE_NETPBM
#include <guilib/alib_win.h>
#endif // ASYS_WIN32CE

#ifdef ASYS_OS2
#define INCL_PM 1
#include <os2.h>
#define BYTE_ALREADY_DEFINED 1
#endif // ASYS_OS2

#ifdef ASYS_SOLARIS
#undef ALIB_HAVE_SCANDIR
#undef ALIB_HAVE_SYS_DIR_H
#undef ALIB_HAVE_DIRENT_D_TYPE
#undef ALIB_HAVE_ALPHASORT
#define ALIB_HAVE_FCNTL_H 1
#define ALIB_HAVE_SYS_UTSNAME_H 1
#endif // ASYS_SOLARIS

#ifdef ASYS_NEXT
//typedef unsigned char bool;
#undef ALIB_HAVE_IOSTREAM
#endif // ASYS_NEXT

#ifdef __linux__
#define ALIB_HAVE_ENDIAN_H 1
#define ALIB_HAVE_SYS_UTSNAME_H 1
#endif // __linux__

#ifdef ASYS_DARWIN
#define ALIB_HAVE_MACHINE_ENDIAN_H 1
#define ALIB_HAVE_SYS_UTSNAME_H 1
#endif // ASYS_DARWIN

#ifdef ASYS_CARBON
#ifndef __MWERKS__
//#include <Carbon/Carbon.h>
#endif // __MWERKS__
#define ALIB_HAVE_MACHINE_ENDIAN_H 1
#endif // ASYS_CARBON

#ifdef ASYS_COCOA
#ifndef __MWERKS__
//#include <Cocoa/Cocoa.h>
#endif // __MWERKS__
#undef ALIB_HAVE_CSTDLIB
#define ALIB_HAVE_MACHINE_ENDIAN_H 1
#endif // ASYS_COCOA

#ifdef ASYS_UNIX_BASED
#define ALIB_HAVE_GETPID 1
#ifndef ALIB_HAVE_SYS_UTSNAME_H
#define ALIB_HAVE_SYS_UTSNAME_H 1
#endif
#endif // ASYS_UNIX_BASED

#ifdef ASYS_NEXT
#define strdup(x) strcpy((char *)malloc(strlen(x)+1),x)
#define false 0
#define true 0xff
#endif // ASYS_NEXT

#ifdef ASYS_PALM
#ifdef fclose
#undef fclose
#define fclose(stream)  FileClose(stream)
#endif // fclose
#define sprintf StrPrintF
#define strlen(x) StrLen(x)
#define strcpy(d,s) StrCopy(d,s)
#define strncpy(d,s,n) StrNCopy(d,s,n)
#define strcmp(x,y) StrCompare(x,y)
#define strncmp(x,y,z) StrNCompare(x,y,z)
#define isdigit(c) TxtGlueCharIsDigit(c)
#define labs(x) abs(x)
#define abs(x) (x>=0 ? x : -x)
#define pow(x) (x)
#ifndef ALIB_PALMOS6
#define exit(x) {\
  EventType newEvent; \
  newEvent.eType = keyDownEvent;\
  newEvent.data.keyDown.chr = launchChr;\
  newEvent.data.keyDown.modifiers = commandKeyMask;\
  EvtAddEventToQueue (&newEvent);\
}
#endif // ALIB_PALMOS6
#define memcpy(d,s,n) MemMove(d,s,n)
// NOTE reversed 2nd and 3rd parameters in memset macro
#define memset(d,c,n) MemSet(d,n,c)
#define malloc(x) MemPtrNew(x)
#define calloc(x,y) memset(malloc(x*y),0,x*y)
#define free(x) MemPtrFree(x)
//#undef awfstat
//#define awfstat _stat
#endif // __ASYS_PALM

#ifdef ASYS_WIN32CE
#define strdup(x) strcpy((char *)malloc(strlen(x)+1),x)
#define calloc(x,y) memset(malloc(x*y),0,x*y)
#endif // ASYS_WIN32CE

#ifdef ASYS_POSIX
#ifdef ALIB_HAVE_CURSES_H
#define USE_ANSI_ON_STDIO 1
#endif // ALIB_HAVE_CURSES_H
#endif // ASYS_POSIX


////////////////////////////////////////////////////////////////////////////////
//  Basic types
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#ifdef ALIB_HAVE_STDIO_H
#include <stdio.h>
#endif // ALIB_HAVE_STDIO_H
#ifdef ALIB_HAVE_STDLIB_H
#include <stdlib.h>
#endif // ALIB_HAVE_STDLIB_H
#ifdef ALIB_HAVE_CSTDLIB
#ifdef __cplusplus
#include <cstdlib>
#endif // __cplusplus
#endif // ALIB_HAVE_CSTDLIB
#ifdef ALIB_HAVE_STRING_H
#include <string.h>
#endif // ALIB_HAVE_STRING_H
#ifdef ALIB_HAVE_SYS_UNISTD_H
//#include <sys/unistd.h>
#endif // ALIB_HAVE_SYS_UNISTD_H
#ifdef ALIB_HAVE_UNISTD_H
//#include <unistd.h>
#endif // ALIB_HAVE_UNISTD_H
#ifdef ALIB_HAVE_WCHAR_H
#include <wchar.h>
#endif // ALIB_HAVE_WCHAR_H
#ifdef ALIB_HAVE_CWCHAR
#include <cwchar>
#endif // ALIB_HAVE_CWCHAR
#ifdef ALIB_HAVE_DIRECT_H
#include <direct.h>
#endif // ALIB_HAVE_DIRECT_H


/*
#ifdef UNICODE
typedef wchar_t  char;
#define ATEXT(x)  L##x
#define AWLPSTR LPWSTR
#ifdef ASYS_WIN32CE
#define ALPSTR_WINMAIN LPWSTR
#else
#define ALPSTR_WINMAIN LPSTR
#endif // ASYS_WIN32CE
#else
#endif // UNICODE
*/


#define ATEXT(a) a
#define AWTEXT(x) L##x


// UINT8, UINT16, UINT32 and UINT64
// are guaranteed to be those bitsizes
#ifndef BYTE_ALREADY_DEFINED
typedef unsigned char BYTE;
#endif
typedef unsigned char UINT8;
typedef unsigned short UINT16;
#ifdef INT32_IS_LONG
typedef unsigned long UINT32;
#else
//typedef unsigned int UINT32;
#endif
#ifndef ALREADY_HAVE_UINT64
typedef unsigned long UINT64;
#endif // ALREADY_HAVE_UINT64
typedef signed char SBYTE;
typedef signed char SINT8;
typedef signed short SINT16;
#ifdef INT32_IS_LONG
typedef signed long SINT32;
#else
typedef signed int SINT32;
#endif


// ULONG will be widest possible native int
// (which means it might NOT be an unsigned long)
// UINT will be the standard native int
#ifndef UINT_ALREADY_DEFINED
typedef unsigned int UINT;
#endif // UINT_ALREADY_DEFINED
#ifndef USHORT_ALREADY_DEFINED
typedef unsigned short USHORT;
#endif // USHORT_ALREADY_DEFINED
#ifndef ULONG_ALREADY_DEFINED
typedef unsigned long ULONG;
#endif // ULONG_ALREADY_DEFINED


// 64 bit int utility
#ifdef __LP64__
#define a_INT64(h,l) ((h<<32)|l)
#define a_HIGH64(x) ((x>>32)&0xffffffff)
#define a_LOW64(x) (x&0xffffffff)
#else
#define a_INT64(h,l) (l)
#define a_HIGH64(x) (0)
#define a_LOW64(x) (x)
#endif


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ALIB_HAVE_UNDERSCORE_STAT
#define awstat _stat
#define awstat_struct _stat
#else
#define awstat stat
#define awstat_struct stat
#endif // ALIB_HAVE_UNDERSCORE_STAT

#ifdef ALIB_HAVE_UNDERSCORE_CHDIR
#define chdir(x) _chdir(x)
#endif // ALIB_HAVE_UNDERSCORE_CHDIR

#ifdef ALIB_HAVE_UNDERSCORE_GETCWD
#define getcwd(x,y) _getcwd(x,y)
#endif // ALIB_HAVE_UNDERSCORE_GETCWD


#define ALIB_MAX_NAMELEN 1024


#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif // min


////////////////////////////////////////////////////////////////////////////////
//  Endian
////////////////////////////////////////////////////////////////////////////////

//#ifndef ALIB_NO_POSH
//#include <guilib/posh.h>
//#if POSH_LITTLE_ENDIAN
//#define A_LITTLE_ENDIAN 1
//#endif
//#if POSH_BIG_ENDIAN
//#define A_BIG_ENDIAN 1
//#endif
//#endif // ALIB_NO_POSH

#ifdef ALIB_HAVE_ENDIAN_H
#include <endian.h>
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define A_LITTLE_ENDIAN 1
#endif // __BYTE_ORDER
#if __BYTE_ORDER == __BIG_ENDIAN
#define A_BIG_ENDIAN 1
#endif // __BYTE_ORDER
#endif // ALIB_HAVE_ENDIAN_H

/*
#ifdef ALIB_HAVE_MACHINE_ENDIAN_H
#include <machine/endian.h>
#if __DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN
#define A_LITTLE_ENDIAN 1
#endif // __DARWIN_BYTE_ORDER
#if __DARWIN_BYTE_ORDER == __DARWIN_BIG_ENDIAN
#define A_BIG_ENDIAN 1
#endif // __DARWIN_BYTE_ORDER
#endif // ALIB_HAVE_MACHINE_ENDIAN_H
*/

#ifdef ASYS_WINDOWS
#ifndef A_LITTLE_ENDIAN
#define A_LITTLE_ENDIAN 1
#endif
#endif
#ifdef ASYS_DOS
#ifndef A_LITTLE_ENDIAN
#define A_LITTLE_ENDIAN 1
#endif
#endif
#ifdef ASYS_OS2
#ifndef A_LITTLE_ENDIAN
#define A_LITTLE_ENDIAN 1
#endif
#endif


#endif // ALIB_ATYPEDEFS_H
