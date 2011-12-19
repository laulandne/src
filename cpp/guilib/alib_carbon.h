#ifndef PLATFORM_ALIB_CARBON_H
#define PLATFORM_ALIB_CARBON_H

#include <guilib/ATypeDefs.h>
#include <guilib/APlatform.h>


class ADialogAbstract;


#ifdef ASYS_MAC
//#include <ConditionalMacros.h>
//#ifdef ALIB_NO_PCH
#undef true
//#include <Types.h>
//#include <AppleEvents.h>
//#include <Menus.h>
#include <Carbon/Carbon.h>
//#endif // ALIB_NO_PCH
#endif // ASYS_MAC


#ifdef ASYS_CARBON
#ifndef TARGET_API_CARBON
#define TARGET_API_CARBON 1
#endif // TARGET_API_CARBON
#endif // ASYS_CARBON


#ifdef ASYS_MAC
/*
#ifdef MAC_OLD_NAMES
#define MacSetPort SetPort
#define MacDrawText DrawText
#define MacLineTo LineTo
#define MacMoveWindow MoveWindow
#define MacSetRect SetRect
#define MacShowWindow ShowWindow
#define MacEnableMenuItem EnableMenuItem
#define MacFindWindow FindWindow
#define MacEnableMenuItem EnableItem
#define DisableMenuItem DisableItem
#endif // MAC_OLD_NAMES
*/
#if !TARGET_API_CARBON
#undef MacSetPort
#define MacSetPort(x) SetPort((GrafPort *)x)
#undef MacShowWindow
#define MacShowWindow(x) ShowWindow((GrafPort *)x)
#undef SelectWindow
#define SelectWindow(x) SelectWindow((GrafPort *)x)
#undef MoveWindow
#define MoveWindow(x,y,z,w) MoveWindow((GrafPort *)x,y,z,w)
#undef SizeWindow
#define SizeWindow(x,y,z,w) SizeWindow((GrafPort *)x,y,z,w)
#undef DrawGrowIcon
#define DrawGrowIcon(x) DrawGrowIcon((GrafPort *)x)
#endif
#endif // ASYS_MAC


#ifdef ASYS_MAC
#if __MC68K__
#undef NewAEEventHandlerUPP
#define NewAEEventHandlerUPP(x) (x)
#else
#if TARGET_API_CARBON
//#define NewAEEventHandlerUPP(x) ((AEEventHandlerUPP)x)
#else
//#undef NewAEEventHandlerUPP
//#define NewAEEventHandlerUPP(x) ((RoutineDescriptor *)x)
#endif // TARGET_API_CARBON
#endif // __MC68K__
#ifdef _LATITUDE_
#define NewAEEventHandlerUPP(x) (x)
#endif // _LATITUDE_
class AEvent;
#define PASCAL
#endif // ASYS_MAC


#ifdef ASYS_MAC
extern OSErr PASCAL ALibAEOpenHandler(const AppleEvent *messagein, AppleEvent *reply,
  unsigned long refIn);
extern OSErr PASCAL ALibAEOpenDocHandler(const AppleEvent *messagein, AppleEvent *reply,
  unsigned long refIn);
extern OSErr PASCAL ALibAEPrintHandler(const AppleEvent *messagein, AppleEvent *reply,
  unsigned long refIn);
extern OSErr PASCAL ALibAEQuitHandler(const AppleEvent *messagein, AppleEvent *reply,
  unsigned long refIn );
#endif // ASYS_MAC


#ifdef ASYS_MAC
extern void alib_about();
extern void macInitAppleEvents();
extern void macInitToolbox();
extern void macHighLevel(EventRecord *AERecord);
extern void macWaitForHighLevel();
extern void macHandleAEDescList(ADialogAbstract *dialog,AEDescList docList);
void macMenus(AEvent& ev, unsigned long choice,EventRecord *macEvent);
char *debugMacEventType(unsigned int theType);
#ifdef USE_CARBON_EVENTS
OSStatus alib_dispatchCarbonEvent(EventRef theEvent);
pascal OSStatus alib_carbWinEventHandler(EventHandlerCallRef nextHand,EventRef theEvent,void *userData);
pascal OSStatus alib_carbAppEventHandler(EventHandlerCallRef nextHand,EventRef theEvent,void *userData);
pascal OSStatus alib_carbCtlEventHandler(EventHandlerCallRef nextHand,EventRef theEvent,void *userData);
pascal OSStatus alib_carbMenuEventHandler(EventHandlerCallRef nextHand,EventRef theEvent,void *userData);
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC


#ifdef ASYS_MAC
// Will be false if we are currently the forground app.
extern bool alib_inBackground;
// Handles to the three standard menus
extern MenuHandle alib_appleMenu,alib_fileMenu,alib_editMenu;
// Extensions and their versions, will be 0 if not present
extern unsigned int alib_qtVer,alib_qtRev;
extern unsigned int alib_dmVer,alib_dmRev;
extern unsigned int alib_cbonVer,alib_cbonRev;
extern bool alib_hasGX,alib_hasQD3D,alib_hasARTA,alib_hasQuartz;
extern bool alib_navServices;
extern bool alib_blueBox;
//extern GWorldPtr alib_myWorld;
extern bool alib_bugHunt;
extern bool alib_usingAppearance;
extern bool alib_usingOT;
extern bool alib_useAppleEvents;
#ifdef __INTEL__
extern HINSTANCE alib_ahInst;
extern HINSTANCE alib_ahPrevInst;
extern int alib_aNShow;
extern void *alib_aCmdLine;
extern WPARAM alib_awParam;   // Word param event loop return value
extern HWND alib_ahWnd;
#endif // __INTEL__
#endif // ASYS_MAC


#endif // PLATFORM_ALIB_CARBON_H
