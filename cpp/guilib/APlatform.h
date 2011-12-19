#ifndef ALIB_APLATFORM_H
#define ALIB_APLATFORM_H


#define DEBUG 1


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

// Flag to see if we've already chosen a platform or still have to
#ifdef ASYS_PLATFORM_CHOSEN
#undef ASYS_PLATFORM_CHOSEN
#endif // ASYS_PLATFORM_CHOSEN


// Check all the ones that are possible and if any are defined
#ifdef ASYS_AMIGA
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_AMIGASDK
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_BE
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_DOS
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_DOS16
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_DOS32
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_ALLEGRO
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_GBA
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_KOS
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_MICROWIN
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_SDL
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_FLTK
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_GNOME
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_KDE
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_MAC
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_NLM
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_OS2
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_OS2_32
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_OS2_16
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_PALM
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_POSIX
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_POWERMAC
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_QNXPHOTON
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_VXD
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_WIN16
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_WIN32
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_WIN32CE
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_WIN64
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_WIN386
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_X11
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_CBUILDER
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_COCOA
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_DOTNET
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_MFC32
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef ASYS_MFC16
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_VISUALAGE
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_DOS
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_MFC
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_OS2
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_PC_BASED
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_UNIX_BASED
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_X11_BASED
#define ASYS_PLATFORM_CHOSEN 1
#endif
#ifdef  ASYS_WINDOWS
#define ASYS_PLATFORM_CHOSEN 1
#endif


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

// If we STILL haven't defined a platform, we guess
// based on compiler, cpu, and other about the current environment
// This is really only for CodeWarrior
#ifndef ASYS_PLATFORM_CHOSEN
#ifdef __MWERKS__
#define ASYS_MAC 1
#define ASYS_PLATFORM_CHOSEN 1
#ifdef __INTEL__
#define ASYS_WIN32 1
#undef ASYS_MAC
#endif // __INTEL__
#ifdef __POWERPC__
#ifdef __BEOS__
#define ASYS_BE 1
#undef ASYS_MAC
#endif // __BEOS__
#endif // __POWERPC__
#ifdef __US_PREFIX_H__
// This works for palm os 3.1 cw sdk
#define ASYS_PALM 1
#undef ASYS_MAC
#endif // __US_PREFIX_H__
#ifdef __PILOT_H__
// This works for palm os 3.1 cw sdk
#define ASYS_PALM 1
#undef ASYS_MAC
#endif // __PILOT_H__
#ifdef __PALMOS_H__
// This works for palm os 3.5+ cw sdk
#define ASYS_PALM 1
#undef ASYS_MAC
#endif // __PALMOS_H__
#endif // __MWERKS__
#endif // ASYS_PLATFORM_CHOSEN


// If its Carbon, then it must be a Mac
#ifdef ASYS_CARBON
#ifndef ASYS_MAC
#define ASYS_MAC 1
#endif // ASYS_MAC
#define ASYS_PLATFORM_CHOSEN 1
#endif // ASYS_CARBON

// If its a Power Mac, then its still a Mac
#ifdef ASYS_POWERMAC
#ifndef ASYS_MAC
#define ASYS_MAC 1
#endif // ASYS_MAC
#endif // ASYS_POWERMAC


// This is only for 386 extenders for Windows 3.1 win16 api, pretty rare
#ifdef __WINDOWS_386__
#ifndef ASYS_WIN386
#define ASYS_WIN386
#endif // ASYS_WIN386
#define ASYS_PLATFORM_CHOSEN 1
#endif // __WINDOWS_386__


#ifdef WIN32
#ifndef ASYS_WIN32
#define ASYS_WIN32
#endif // ASYS_WIN32
#define ASYS_PLATFORM_CHOSEN 1
#endif // WIN32


// If we get here and there isn't a platform,
// we just assume "posix" which basically means
// "generic standard c text only" here.
#ifndef ASYS_PLATFORM_CHOSEN
#define ASYS_POSIX 1
#define ASYS_PLATFORM_CHOSEN 1
#endif // ASYS_PLATFORM_CHOSEN


// Correct Windows flag for all Windows platforms
#ifdef  ASYS_WIN16
#define ASYS_WINDOWS 1
#define OLD_WINDOWS 1
#endif
#ifdef  ASYS_WIN32
#define ASYS_WINDOWS 1
#endif
#ifdef  ASYS_WIN32CE
#define ASYS_WINDOWS 1
#endif
#ifdef  ASYS_WIN64
#define ASYS_WINDOWS 1
#endif
#ifdef  ASYS_WIN386
#define ASYS_WINDOWS 1
#endif


// Correct DOS flag for all DOS platforms
#ifdef  ASYS_DOS16
#define ASYS_DOS 1
#endif
#ifdef  ASYS_DOS32
#define ASYS_DOS 1
#endif


// Correct OS/2 flag for all OS/2 platforms
#ifdef  ASYS_OS2_16
#define ASYS_OS2 1
#endif
#ifdef  ASYS_OS2_32
#define ASYS_OS2 1
#endif


#ifdef ASYS_KDE
#define ASYS_X11_BASED 1
#endif // ASYS_KDE
#ifdef ASYS_GNOME
#define ASYS_X11_BASED 1
#endif // ASYS_GNOME

// For now we treat these two as just X toolkits,
// later we'll add cross-platform support (mostly Windows)
#ifdef ASYS_FLTK
#define ASYS_X11_BASED 1
#endif // ASYS_FLTK


// Various unix and related platforms
#ifdef ASYS_DARWIN
#define ASYS_UNIX_BASED 1
#endif // ASYS_DARWIN
#ifdef ASYS_LINUX
#define ASYS_UNIX_BASED 1
#endif // ASYS_LINUX
#ifdef ASYS_BSD
#define ASYS_UNIX_BASED 1
#endif // ASYS_BSD

#ifdef ASYS_X11
#define ASYS_X11_BASED 1
#endif // ASYS_X11

#ifdef ASYS_X11_BASED
#define ASYS_UNIX_BASED 1
#endif // ASYS_X11_BASED

// We pretend cygwin is unix
#ifdef ASYS_CYGWIN
#ifdef ASYS_UNIX_BASED
#undef ASYS_UNIX_BASED
#endif // ASYS_UNIX_BASED
#endif // ASYS_CYGWIN


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_MAC
// Common to all mac systems
#define MAC_USE_OT 1
#define USE_QUICKTIME 1
#define MAC_SYSTEM_8 1
#define MAC_USE_DISPLAY_MANAGER 1
#define MAC_USE_SOUND_MANAGER 1
#define MAC_USE_QT_MUSIC 1
// Only MacOS X
#ifdef __GNUC__
#ifndef DONT_USE_OPENGL
#define USE_OPENGL 1
#endif // DONT_USE_OPENGL
//#define USE_QUARTZ 1
#define USE_CARBON_EVENTS 1
#endif // __GNUC__
#ifdef __GNUC__
#define USE_UMBRELLA_HEADERS 1
#undef MAC_USE_OT
#ifndef ASYS_UNIX_BASED
#define ASYS_UNIX_BASED 1
#endif // ASYS_UNIX_BASED
#ifndef ASYS_DARWIN
#define ASYS_DARWIN 1
#endif // ASYS_DARWIN
#endif // __GNUC__
// Only 64-bit MacOS X
#if __LP64__
#ifndef USE_QUARTZ
#define USE_QUARTZ 1
#endif // USE_QUARTZ
#ifdef MAC_USE_SOUND_MANAGER
#undef MAC_USE_SOUND_MANAGER
#endif // MAC_USE_SOUND_MANAGER
#ifdef MAC_USE_QT_MUSIC
#undef MAC_USE_QT_MUSIC
#endif // MAC_USE_QT_MUSIC
#endif // __LP64__
// Correct Carbon flag for older universal headers
#if TARGET_API_MAC_CARBON
#ifndef TARGET_CARBON
#define TARGET_CARBON 1
#endif // TARGET_CARBON
#endif // TARGET_API_MAC_CARBON
#endif // ASYS_MAC


#ifdef ASYS_WINDOWS
//#define USE_DIRECTX 1
#ifndef DONT_USE_OPENGL
//#define USE_OPENGL 1
#endif // DONT_USE_OPENGL
#ifdef USE_DIRECTX
#define USE_DIRECT3D 1
#define USE_DIRECTDRAW 1
#define USE_DIRECTSOUND 1
#define USE_DIRECTINPUT 1
#endif // USE_DIRECTX
#endif // ASYS_WINDOWS


#ifdef __GNUC__
// This assumes we've got a system() call, etc.
#define USE_NETPBM 1
//#define USE_LIBBFD 1
#endif // __GNUC__


#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG 1
#endif
#endif


#endif // ALIB_APLATFORM_H

