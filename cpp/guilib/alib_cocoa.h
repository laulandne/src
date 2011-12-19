#ifndef ALIB_ALIB_COCOA_H
#define ALIB_ALIB_COCOA_H

/*

    Despite the name, this is for NextStep, OpenStep, Cocoa and GnuStep
    
    Objective-C glue and interface to C++

*/


// NOTE: Do NOT put or include ANY Objective-C in this file!
// C++ code includes it too!
// Oops!  Can't use C++ either. Since Objective-C includes it also!

// ONLY C HERE!


#ifdef ASYS_COCOA
// Will be false if we are currently the forground app.
extern bool alib_inBackground;
// Handles to the three standard menus
//extern MenuHandle alib_appleMenu,alib_fileMenu,alib_editMenu;
// Extensions and their versions, will be 0 if not present
extern unsigned int alib_qtVer,alib_qtRev;
//extern unsigned int alib_dmVer,alib_dmRev;
extern unsigned int alib_cbonVer,alib_cbonRev;
//extern bool alib_hasGX,alib_hasQD3D,alib_hasARTA
extern bool alib_hasQuartz;
//extern bool alib_navServices;
//extern bool alib_blueBox;
//extern GWorldPtr alib_myWorld;
extern bool alib_bugHunt;
//extern bool alib_usingAppearance;
//extern bool alib_usingOT;
extern bool alib_useAppleEvents;
extern int alib_version;
extern int alib_versionMinor;
extern bool alib_useDebugger;
#ifdef __INTEL__
extern HINSTANCE alib_ahInst;
extern HINSTANCE alib_ahPrevInst;
extern int alib_aNShow;
extern void *alib_aCmdLine;
extern WPARAM alib_awParam;   // Word param event loop return value
extern HWND alib_ahWnd;
#endif // __INTEL__
#endif // ASYS_COCOA


#ifdef ASYS_COCOA
#ifdef __cplusplus
extern "C"
#endif
int cocoaMain(int argc, char *argv[]);
#endif // ASYS_COCOA

#ifdef ASYS_COCOA
#ifdef __cplusplus
extern "C"
#endif
void AApp_cocoa_start();
#endif // ASYS_COCOA

#ifdef ASYS_SDL
#ifdef __cplusplus
extern "C"
#endif
void cocoa_sdl_start();
#endif // ASYS_SDL


#endif // ALIB_ALIB_COCOA_H
