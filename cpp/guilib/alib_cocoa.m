/*

    Despite the name, this is for NextStep, OpenStep, Cocoa and GnuStep
    
    Objective-C glue and interface to C++

*/


#ifdef ASYS_COCOA
#ifdef ASYS_NEXT
#import <AppKit/AppKit.h>
#else
#if TARGET_OS_IPHONE
#include <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif // ASYS_NEXT
#endif // ASYS_NEXT
#import <platform/alib_cocoa.h>
#endif // ASYS_COCOA


#ifdef ASYS_SDL
#include <alib/atypedefs.h>
#import <Cocoa/Cocoa.h>
#import <platform/alib_cocoa.h>
#endif // ASYS_SDL


#ifdef ASYS_COCOA
//void *alib_curEvent=NULL;
// Will be false if we are currently the forground app.
bool alib_inBackground;
// Handles to the three standard menus
// Extensions and their versions, will be 0 if not present
unsigned int alib_qtVer,alib_qtRev;
bool alib_hasQuartz;
bool alib_bugHunt;
bool alib_useAppleEvents;
#ifdef _LATITUDE_
_qd qd;
#endif // _LATITUDE_
int alib_version=0,alib_versionMinor=0;
bool alib_useDebugger=false;
#ifdef __INTEL__
HINSTANCE alib_ahInst;
HINSTANCE alib_ahPrevInst;
int alib_aNShow;
void *alib_aCmdLine;
WPARAM alib_awParam;   // Word param event loop return value
HWND alib_ahWnd;
#endif // __INTEL__
#endif // ASYS_COCOA

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_COCOA
int cocoaMain(int argc, char *argv[]) {
  int retVal=0;
#if TARGET_OS_IPHONE
  NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
  retVal = UIApplicationMain(argc, argv, nil, nil);
  [pool release];
#else
  retVal=NSApplicationMain(argc, argv);
#endif // TARGET_OS_IPHONE
  return retVal;
}
#endif // ASYS_COCOA


////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_COCOA
void AApp_cocoa_start()
{
  alib_bugHunt=false;
  alib_inBackground=false;
  // Query system
  // Assume at least this...
  alib_version=8;  alib_versionMinor=0;
  SInt32 response=0;
#ifdef DEBUG_VERBOSE
  //dBug<<"MacOS Version: "<<version<<".";
  //if(versionMinor<10) dBug<<"0";
  //dBug<<versionMinor<<"\n";
#endif // DEBUG_VERBOSE
#ifndef REALLY_OLD_MAC
#ifdef DEBUG_VERBOSE
  //dBug<<"-----------------------\n";
  //dBug<<"Looking for some extensions...\n";
#endif // DEBUG_VERBOSE
#ifdef USE_QUARTZ
  //dBug<<"Using Quartz for drawing.\n";
#else
  //dBug<<"Using QuickDraw for drawing.\n";
  //GDHandle gfxDevice=GetGDevice();
#endif // USE_QUARTZ
  //useAppleEvents=true;
  // FIXME: check for apple events avail...
  alib_useDebugger=true;
  // FIXME: check for macsbug avail...
  alib_qtVer=0;  alib_qtRev=0;
#ifdef USE_QUICKTIME
  if(!Gestalt(gestaltQuickTimeVersion,&response)) {
    alib_qtVer=(response>>24)&0xff;  alib_qtRev=(response>>20)&0xf;
#ifdef DEBUG_VERBOSE
    //dBug<<"QuickTime "<<alib_qtVer<<"."<<alib_qtRev<<" present.\n";
#endif // DEBUG_VERBOSE
  }
#ifdef DEBUG_VERBOSE
  //else dBug<<"(No QuickTime...dang it!)\n";
#endif // DEBUG_VERBOSE
#endif // USE_QUICKTIME
#endif // REALLY_OLD_MAC
}
#endif // ASYS_COCOA


////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_SDL
void cocoa_sdl_start()
{
#ifdef ASYS_DARWIN
    [NSApplication sharedApplication];
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
 
    NSArray *args = [[NSProcessInfo processInfo] arguments];
    unsigned count, limit = [args count];
 
    for (count = 0; count < limit; count++)
    {
        NSAutoreleasePool *loopPool = [[NSAutoreleasePool alloc] init];
        NSString *fileContents;
        NSString *fileName;
 
        fileName = [args objectAtIndex:count];
        fileContents = [[[NSString alloc] initWithContentsOfFile:fileName] autorelease];
        // this is equivalent to using stringWithContentsOfFile:
 
        /* Process the file, creating and autoreleasing more objects. */
 
        [loopPool release];
    }
 
    /* Do whatever cleanup is needed. */
    /*[pool release];*/
 
    /*exit (EXIT_SUCCESS);*/
#endif // ASYS_DARWIN
}
#endif // ASYS_SDL
