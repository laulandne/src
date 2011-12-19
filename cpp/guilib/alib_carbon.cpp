
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#ifdef ASYS_MAC
#include <iostream>

#include <portable/nickcpp/ANullStream.h>

#include <guilib/alib_carbon.h>

#include <guilib/AApp.h>
#include <guilib/AWindow.h>
#include <guilib/AEvent.h>
#include <guilib/ADisplay.h>
#include <guilib/AApplicationLoop.h>
#include <guilib/ADialogAbstract.h>
#include <portable/nickcpp/AStringHelper.h>
#endif // ASYS_MAC


#ifdef ASYS_MAC
#ifdef ALIB_NO_PCH
//#include <Gestalt.h>
//#include <Windows.h>
//#include <Dialogs.h>
#include <Carbon/Carbon.h>
#endif // ALIB_NO_PCH
#endif // ASYS_MAC


#ifdef ASYS_MAC
void macInitAppleEvents();
void macInitToolbox();
void macHighLevel(EventRecord *AERecord);
void macWaitForHighLevel();
void macHandleAEDescList(AEDescList docList);
#endif // ASYS_MAC


#ifdef ASYS_MAC
// Will be false if we are currently the forground app.
bool alib_inBackground;
// Handles to the three standard menus
MenuHandle alib_appleMenu,alib_fileMenu,alib_editMenu;
// Extensions and their versions, will be 0 if not present
unsigned int alib_qtVer,alib_qtRev;
unsigned int alib_dmVer,alib_dmRev;
unsigned int alib_cbonVer,alib_cbonRev;
bool alib_hasGX,alib_hasQD3D,alib_hasARTA,alib_hasQuartz;
bool alib_navServices;
bool alib_blueBox;
//GWorldPtr alib_myWorld;
bool alib_bugHunt;
bool alib_usingAppearance;
bool alib_usingOT;
bool alib_useAppleEvents;
#ifdef _LATITUDE_
_qd qd;
#endif // _LATITUDE_
#ifdef __INTEL__
HINSTANCE alib_ahInst;
HINSTANCE alib_ahPrevInst;
int alib_aNShow;
void *alib_aCmdLine;
WPARAM alib_awParam;   // Word param event loop return value
HWND alib_ahWnd;
#endif // __INTEL__
#endif // ASYS_MAC



////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_MAC
OSErr PASCAL ALibAEOpenHandler(const AppleEvent *messagein, AppleEvent *reply,
  unsigned long refIn)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ALibAEOpenHandler.\n";
#endif // DEBUG_VERBOSE
  return noErr;
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
OSErr PASCAL ALibAEOpenDocHandler(const AppleEvent *messagein, AppleEvent *reply,
  unsigned long refIn)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ALibAEOpenDocHandler.\n";
#endif // DEBUG_VERBOSE
  AEDescList docList;
  OSErr myErr=AEGetParamDesc(messagein,keyDirectObject,typeAEList,&docList);
  if(myErr!=noErr) { DEBUG_OUT<<"AEGetParamDesc error...\n"; return myErr; }
  // Inside mac says to check for required params here...
  // TODO: Bad hack here to get some place to squirrel away argc and argv...
  ADialogAbstract dialog;
  macHandleAEDescList(&dialog,docList);
  AEDisposeDesc(&docList);
  return(noErr);
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
OSErr PASCAL ALibAEPrintHandler(const AppleEvent *messagein, AppleEvent *reply,
  unsigned long refIn)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ALibAEPrintHandler.\n";
#endif // DEBUG_VERBOSE
  return(errAEEventNotHandled);
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
OSErr PASCAL ALibAEQuitHandler(const AppleEvent *messagein, AppleEvent *reply,
  unsigned long refIn )
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ALibAEQuitHandler!\n";
#endif // DEBUG_VERBOSE
  if(alib_curEvent) alib_curEvent->type=A_EVENT_DESTROY;
  alib_curEvent->setQuitASAP();
#ifdef USE_CARBON_EVENTS
  DEBUG_OUT<<"Going to QuitApplicationEventLoop...\n";
#if !__LP64__
  QuitApplicationEventLoop();
#endif // __LP64__
#endif // USE_CARBON_EVENTS
  return(noErr);
}
#endif // ASYS_MAC


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_MAC
void macWaitForHighLevel()
{
#ifndef USE_CARBON_EVENTS
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"macWaitForHighLevel!\n";
#endif // DEBUG_VERBOSE
  // Wait until we get a high-level event
  // which should be our openDoc or openApp from the Finder (or whomever)
  // NOTE: we can't use AEvent, because we don't have a window at this point
  EventRecord event;
  // Idle until window manager says there's an event for our process
  bool working=true;
  while(working) {
    GetNextEvent(everyEvent,&event);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"macWaitForHighLevel native event type "<<(int)event.what<<"\n";
#endif // DEBUG_VERBOSE
    if(event.what==kHighLevelEvent) { working=false; macHighLevel(&event); }
  }
#endif // USE_CARBON_EVENTS
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
void macInitToolbox()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"macInitMacToolbox...\n";
#endif // DEBUG_VERBOSE
  //
  IBNibRef nibRef;
  WindowRef window;
  OSStatus err;
  bool gotNib=false;
  // Create a Nib reference passing the name of the nib file (without the .nib extension)
  // CreateNibReference only searches into the application bundle.
  err=CreateNibReference(CFSTR("main"),&nibRef);
  if(err) {
    std::cerr<<"Couldn't CreateNibReference!\n";
  }
  else {
    std::cerr<<"CreateNibReference was good...\n";
    gotNib=true;
  }
  //require_noerr( err, CantGetNibRef );
  // Once the nib reference is created, set the menu bar. "MainMenu" is the name of the menu bar
  // object. This name is set in InterfaceBuilder when the nib is created.
  //err=SetMenuBarFromNib(nibRef,CFSTR("MenuBar"));
  //require_noerr( err, CantSetMenuBar );
  // Then create a window. "MainWindow" is the name of the window object. This name is set in
  // InterfaceBuilder when the nib is created.
  //err=CreateWindowFromNib(nibRef,CFSTR("MainWindow"),&window);
  //require_noerr( err, CantCreateWindow );
  // We don't need the nib reference anymore.
  if(gotNib) {
    std::cerr<<"Going to DisposeNibReference...\n";
    DisposeNibReference(nibRef);
  }
  // The window was created hidden so show it.
  //ShowWindow(window);
  // Call the event loop
  //RunApplicationEventLoop();
  //
#if !TARGET_API_CARBON
  //MaxApplZone();
  //MoreMasters();
  InitGraf(&qd.thePort);
  qd.randSeed=TickCount();
  InitWindows();
  InitFonts();
  InitDialogs(NULL);
  InitMenus();
  TEInit();
#endif // TARGET_API_CARBON
#ifdef USE_QUARTZ
#else
  InitCursor();
#endif // USE_QUARTZ
  FlushEvents(everyEvent,0);
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
void macInitAppleEvents()
{
  // Init apple events
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"macInitAppleEvents()...\n";
#endif // DEBUG_VERBOSE
  OSErr errCode=noErr;
  SInt32 aeThere;
  alib_useAppleEvents=false;
  if(Gestalt(gestaltAppleEventsAttr,&aeThere)==noErr) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Initing AppleEvents...\n";
#endif // DEBUG_VERBOSE
    alib_useAppleEvents=true;
    AEEventHandlerUPP openAppHandler=NewAEEventHandlerUPP((AEEventHandlerProcPtr)ALibAEOpenHandler);
    errCode=AEInstallEventHandler(kCoreEventClass,kAEOpenApplication,openAppHandler,0,false);
#ifdef DEBUG
    if(errCode!=noErr) DEBUG_OUT<<"Couldn't install openApp event handler!\n";
#endif // DEBUG
    if(errCode!=noErr) alib_useAppleEvents=false;
    AEEventHandlerUPP openDocHandler=NewAEEventHandlerUPP((AEEventHandlerProcPtr)ALibAEOpenDocHandler);
    errCode=AEInstallEventHandler(kCoreEventClass,kAEOpenDocuments,openDocHandler,0,false);
#ifdef DEBUG
    if(errCode!=noErr) DEBUG_OUT<<"Couldn't install openDocs event handler!\n";
#endif // DEBUG
    if(errCode!=noErr) alib_useAppleEvents=false;
    AEEventHandlerUPP quitAppHandler=NewAEEventHandlerUPP((AEEventHandlerProcPtr)ALibAEQuitHandler);
    errCode=AEInstallEventHandler(kCoreEventClass,kAEQuitApplication,quitAppHandler,0,false);
#ifdef DEBUG
    if(errCode!=noErr) DEBUG_OUT<<"Couldn't install quitApp event handler!\n";
#endif // DEBUG
    if(errCode!=noErr) alib_useAppleEvents=false;
    AEEventHandlerUPP printDocHandler=NewAEEventHandlerUPP((AEEventHandlerProcPtr)ALibAEPrintHandler);
    errCode=AEInstallEventHandler(kCoreEventClass,kAEPrintDocuments,printDocHandler,0,false);
#ifdef DEBUG
    if(errCode!=noErr) DEBUG_OUT<<"Couldn't install printDocs event handler!\n";
#endif // DEBUG
    if(errCode!=noErr) alib_useAppleEvents=false;
  }
  else DEBUG_OUT<<"Gestalt says no AppleEvents!\n";
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Done.\n";
#endif // DEBUG_VERBOSE
}
#endif


#ifdef ASYS_MAC
void macHandleAEDescList(ADialogAbstract *dialog,AEDescList docList)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"macHandleAEDescList...\n";
#endif // DEBUG_VERBOSE
  if(!alib_useAppleEvents) {
#ifdef DEBUG
    DEBUG_OUT<<"can't, useAppleEvents is false!\n";
#endif // DEBUG
    return;
  }
  unsigned long itemsInList;
  OSErr myErr=AECountItems(&docList,(long *)&itemsInList);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<itemsInList<<" items passed in appleEvent.\n";
#endif // DEBUG_VERBOSE
  FSSpec myFSS;
  AEKeyword keywd;
  DescType returnedType;
  Size actualSize;
  char fname[256];
  for(unsigned int i=1;i<=itemsInList;i++) {
#if !__LP64__
    myErr=AEGetNthPtr(&docList,i,typeFSS,&keywd,&returnedType,&myFSS,sizeof(myFSS),&actualSize);
#endif // __LP64__
    if(myErr!=noErr) {
#ifdef DEBUG
      DEBUG_OUT<<"AEGetNthPtr error...\n";
#endif // DEBUG
      return;
    }
    // work with the fss
    FSRef newRef;
    char fullPathBuf[1024];
#if !__LP64__
    myErr=FSpMakeFSRef(&myFSS,&newRef);
#endif // __LP64__
#if TARGET_CARBON
    myErr=FSRefMakePath(&newRef,(UInt8 *)fullPathBuf,1023);
#else
    // TODO: Is there any way to do this in classic MacOS?
    fullPathBuf[0]=0;
#endif // TARGET_CARBON
    DEBUG_OUT<<"fullPathBuf is "<<fullPathBuf<<"\n";
#ifdef DEBUG_VERBOSE
#if !__LP64__
    DEBUG_OUT<<"vRefNum="<<myFSS.vRefNum<<" parID="<<myFSS.parID<<"\n";
#endif // __LP64__
#endif // DEBUG_VERBOSE
#if TARGET_CARBON
    // FIXME: This is deprecated in 10.4, what should I use?
    DEBUG_OUT<<"Going to HSetVol...\n";
#if !__LP64__
    HSetVol(0,myFSS.vRefNum,myFSS.parID);
#endif // __LP64__
#else
    short wdref;
    DEBUG_OUT<<"Going to OpenWD...\n";
    myErr=OpenWD(myFSS.vRefNum,myFSS.parID,0,&wdref);
    if(myErr!=noErr) {
#ifdef DEBUG
      DEBUG_OUT<<"Couldn't OpenWD ("<<(int)myErr<<")\n";
#endif // DEBUG
    }
    else {
      DEBUG_OUT<<"Going to SetVol...\n";
      myErr=SetVol(0,wdref);
      if(myErr!=noErr) {
#ifdef DEBUG
        DEBUG_OUT<<"Couldn't SetVOL ("<<(int)myErr<<")\n";
#endif // DEBUG
      }
      DEBUG_OUT<<"Going to CloseWD...\n";
      CloseWD(wdref);
    }
#endif // TARGET_CARBON
    bool hasSpaces=false;
#ifdef TARGET_CARBON
    for(unsigned int t=0;t<strlen(fullPathBuf);t++) {
      fname[t]=fullPathBuf[t];
      if(fname[t]==' ') hasSpaces=true;
    }
    fname[strlen(fullPathBuf)]=0;
#else
    for(unsigned int t=0;t<myFSS.name[0];t++) {
      fname[t]=myFSS.name[t+1];
      if(fname[t]==' ') hasSpaces=true;
    }
    fname[myFSS.name[0]]=0;
#endif // TARGET_CARBON
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<i<<" : '"<<fname<<"'\n";
#if !__LP64__
    DEBUG_OUT<<"vRefNum="<<myFSS.vRefNum<<" parID="<<myFSS.parID<<"\n";
#endif // __LP64__
#endif // DEBUG_VERBOSE
   AStringHelper shelper;
   DEBUG_OUT<<"Going to parse string...\n";
   shelper.parseString(fname,(char *)NULL,true);
   DEBUG_OUT<<"Back from parse string...\n";
    // TODO: Bug here...unless argv's reset, it just keeps appending strings...
    for(unsigned int tt=0;tt<shelper.getArgc();tt++) {
      dialog->argv[dialog->argc]=strdup(shelper.getArgv(tt));
      dialog->argc++;
    }
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"AApp::handleAEDescList, docList was:\n";
  for(unsigned int q=0;q<dialog->argc;q++) DEBUG_OUT<<q<<" "<<dialog->argv[q]<<"\n";
#endif // DEBUG_VERBOSE
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
void macHighLevel(EventRecord *AERecord)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"macHighLevel...\n";
#endif // DEBUG_VERBOSE
  OSErr errCode=noErr;
  errCode=AEProcessAppleEvent(AERecord);
  switch(errCode) {
    case noErr: break;
	case errAEEventNotHandled: //DEBUG_OUT.warning("Apple Event was not handled!\n"); break;
	default: DEBUG_OUT<<"macHighLevel errCode="<<(unsigned long)errCode<<"!\n"; break;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Done.\n";
#endif // DEBUG_VERBOSE
}
#endif // ASYS_MAC


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_MAC
void AEvent::macHandleEvent(EventRecord& macEvent)
{
  DEBUG_OUT<<"macHandleEvent event.what is: "<<macEvent.what<<" ("<<debugMacEventType(macEvent.what)<<")\n";
  Point eventPoint;
  eventPoint.h=macEvent.where.h;  eventPoint.v=macEvent.where.v;
#ifdef USE_QUARTZ
#else
  MacSetPort(theWin->getMacGrafPtr());
  GlobalToLocal(&eventPoint);
#endif // USE_QUARTZ
  x=eventPoint.h;  y=eventPoint.v;
  switch(macEvent.what) {
    case nullEvent:
      type=A_EVENT_NONE;
      break;
    case updateEvt:
      theWin->getParent()->sync();
	    theWin->sync();
#if TARGET_API_MAC_CARBON
#ifdef USE_QUARTZ
#else
      MacSetPort(GetWindowPort((WindowPtr)macEvent.message));
#endif // USE_QUARTZ
#else
      MacSetPort((WindowPtr)macEvent.message);
#endif // TARGET_API_MAC_CARBON
#ifdef USE_QUARTZ
#else
      BeginUpdate((WindowPtr)macEvent.message);
      //theWin->drawBackground((void *)macEvent.message);
      DrawGrowIcon((WindowPtr)macEvent.message);
      EndUpdate((WindowPtr)macEvent.message);
#endif // USE_QUARTZ
      type=A_EVENT_REDRAW;
      break;
    case activateEvt:
      type=A_EVENT_UNFOCUS;
      break;
    case keyUp:
      type=A_EVENT_KEYUP;
      code=macEvent.message&0xffff;
      break;
    case keyDown: case autoKey:
      type=A_EVENT_KEYDOWN;
      code=macEvent.message&0xffff;
      break;
    case mouseUp :
      macMouseUp(&macEvent,(long *)&macEvent.where);
      break;
    case mouseDown:
      macMouseDown(&macEvent,(long *)&macEvent.where);
      break;
#if !TARGET_CARBON
    case diskEvt:
      // floppy inserted
      if(((macEvent.message>>16)&0xffff)!=noErr) DIBadMount(eventPoint,macEvent.message);
      break;
#endif // TARGET_CARBON
    case osEvt:
      DEBUG_OUT<<"osEvt event...\n";
      // FIXME: Is this handled right?
      type=A_EVENT_NONE;
      switch((macEvent.message>>24)&0x00FF) {
        case suspendResumeMessage:
          alib_inBackground=!(macEvent.message&resumeFlag);
          if(alib_inBackground) {
            type=A_EVENT_UNFOCUS;  macEvent.modifiers=0;
            macEvent.message=(long)theWin->getMacWindowPtr();
          }
          else {
            type=A_EVENT_UNFOCUS;  macEvent.modifiers=activeFlag;
#if !__LP64__
            macEvent.message=(long)FrontWindow();
#endif // __LP64__
          }
          break;
        default:
          break;
      }
      break;
    case kHighLevelEvent:
      alib_curEvent=this;
      macHighLevel(&macEvent);
      break;
    default:
      DEBUG_OUT<<"macHandleEvent unknown mac event: "<<macEvent.what<<"\n";
      type=A_EVENT_UNKNOWN;
      break;
  }
  if(type==A_EVENT_UNFOCUS) {
    if(macEvent.modifiers&activeFlag) {
      // Does this work?
#if !__LP64__
      MacShowWindow((WindowPtr)macEvent.message);
      SelectWindow((WindowPtr)macEvent.message);
#endif // __LP64__
#if TARGET_API_MAC_CARBON
#ifdef USE_QUARTZ
#else
      MacSetPort(GetWindowPort((WindowPtr)macEvent.message));
#endif // USE_QUARTZ
#else
      MacSetPort((GrafPort *)macEvent.message);
#endif // TARGET_API_MAC_CARBON
      if((WindowPtr)macEvent.message==(WindowPtr)theWin->getMacWindowPtr()) type=A_EVENT_FOCUS;
    }
  }
  modifier=0;
  if(macEvent.modifiers&shiftKey)  modifier|=A_MOD_SHIFT;
  if(macEvent.modifiers&controlKey)  modifier|=A_MOD_CONTROL;
  if(type==A_EVENT_KEYUP) {
    unsigned int keyCode=(macEvent.message&keyCodeMask)>>8;
    code=macEvent.message&charCodeMask;
    //DEBUG_OUT<<"code: "<<code<<" mac keycode: "<<keyCode<<"\n";
    if((macEvent.modifiers&cmdKey)&&(macEvent.what!=autoKey)) {
      unsigned long mchoice=0;
#if !__LP64__
	    mchoice=MenuKey(code);
#endif // __LP64__
      macMenus(*this,mchoice,&macEvent);
    }
    else if((macEvent.modifiers&cmdKey)&&(code=='.')) {
      FlushEvents(keyDownMask+autoKeyMask,0);
      type=A_EVENT_BREAK;
    }
    else {
      // Handle other keys
      switch(code) {
        case 1: code=A_KEY_HOME; break;
        case 3: code=13; break;
        case 5: code=A_KEY_INSERT; break;
        case 16:
          switch(keyCode) {
            case 122: code=A_KEY_F1; break;
            case 120: code=A_KEY_F2; break;
            case 99: code=A_KEY_F3; break;
            case 118: code=A_KEY_F4; break;
            case 96: code=A_KEY_F5; break;
            case 97: code=A_KEY_F6; break;
            case 98: code=A_KEY_F7; break;
            case 100: code=A_KEY_F8; break;
            case 101: code=A_KEY_F9; break;
            case 109: code=A_KEY_F10; break;
            default: break;
          }
          break;
        case 30: code=A_KEY_UP; break;
        case 31: code=A_KEY_DOWN; break;
        case 28: code=A_KEY_LEFT; break;
        case 29: code=A_KEY_RIGHT; break;
        default: break;
      }
    }
  }
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
// FIXME: Make sure we don't handle any events that aren't for us.
void AEvent::macMouseDown(EventRecord *macEvent, long *longWhere)
{
  Point myPoint;
  // TODO: Are these reversed on intel vs ppc?
  myPoint.h=((*longWhere)>>16)&0xffff;
  myPoint.v=(*longWhere)&0xffff;
  x=myPoint.h;  y=myPoint.v;
  DEBUG_OUT<<"macMouseDown x="<<x<<" y="<<y<<"\n";
  Rect parentRect;
  parentRect.top=22; parentRect.left=4;
  parentRect.bottom=theWin->getParent()->getHeight();
  parentRect.right=theWin->getParent()->getWidth();
  WindowPtr macWinPtr;
  unsigned int part=0;
#if !__LP64__
  part=MacFindWindow(myPoint,&macWinPtr);
#endif // __LP64__
  if(macWinPtr) DEBUG_OUT<<"Got a window pointer\n";  else DEBUG_OUT<<"Got null window pointer\n";
  unsigned long longMenuWhere=0;
  unsigned long lGrowWhere=0;
  bool clicked=false;
  switch(part) {
    case inDesk:
	  DEBUG_OUT<<"inDesk\n";
      // Is there a DeskClick() I should call here?
      break;
    case inMenuBar:
      // FIXME: enable or disable menus here for DA's vs. our windows
 	  DEBUG_OUT<<"inMenuBar\n";
#if !__LP64__
      longMenuWhere=MenuSelect(macEvent->where);
#endif // __LP64__
      macMenus(*this,longMenuWhere,macEvent);
      break;
#if !TARGET_API_CARBON
    case inSysWindow:
	  DEBUG_OUT<<"inSysWindow\n";
      SystemClick(macEvent,macWinPtr);
      break;
#endif // TARGET_API_CARBON
    case inContent:
	  DEBUG_OUT<<"inContent\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
      type=A_EVENT_MOUSEDOWN;
      break;
    case inDrag:
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
	    DragWindow(macWinPtr,myPoint,&parentRect);
#endif // __LP64__
	    type=A_EVENT_MOVED;
	    aThisApp.yieldToOtherProcesses();
      break;
    case inGrow:
	    DEBUG_OUT<<"inGrow\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
  	  lGrowWhere=GrowWindow(macWinPtr,myPoint,&parentRect);
#endif // __LP64__
	    if(lGrowWhere) theWin->resize(lGrowWhere&0xffff,(lGrowWhere&0xffff0000)>>16);
	    theWin->sync(); type=A_EVENT_RESIZE;
	    aThisApp.yieldToOtherProcesses();
      break;
    case inZoomIn:
    case inZoomOut:
	    DEBUG_OUT<<"inZoomIn/Out\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
		  clicked=TrackBox(macWinPtr,macEvent->where,part);
#endif
        if(clicked) {
#if TARGET_API_CARBON
		Rect tRect;
#ifdef USE_QUARTZ
#else
		GetWindowPortBounds(macWinPtr,&tRect);
          MacSetPort(GetWindowPort(macWinPtr));
          EraseRect(&tRect);
#endif // USE_QUARTZ
#if !__LP64__
          ZoomWindow(macWinPtr,part,true);
          GetWindowPortBounds(macWinPtr,&tRect);
          InvalWindowRect(macWinPtr,&tRect);
#endif // __LP64__
#else
          MacSetPort(macWinPtr);
          EraseRect(&(macWinPtr->portRect));
          ZoomWindow(macWinPtr,part,true);
          InvalRect(&(macWinPtr->portRect));
#endif // TARGET_API_CARBON
        }
        /*
        if(part==inZoomIn)
          theWin->resize(macWS->userState.right-macWS->userState.left,
            macWS->userState.bottom-macWS->userState.top);
        else
          theWin->resize(macWS->stdState.right-macWS->stdState.left,
            macWS->stdState.bottom-macWS->stdState.top);
        */
      theWin->sync(); type=A_EVENT_RESIZE;
      aThisApp.yieldToOtherProcesses();
      break;
    case inGoAway:
  	  DEBUG_OUT<<"inGoAway\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
	    if(TrackGoAway(macWinPtr,myPoint)) type=A_EVENT_DESTROY;
#endif // __LP64__
      break;
    case inCollapseBox:
  	  DEBUG_OUT<<"inCollapseBox\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
	    break;
    case inProxyIcon:
  	  DEBUG_OUT<<"inProxyIcon\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
	  break;
#if TARGET_API_MAC_OSX
   case inToolbarButton:
  	  DEBUG_OUT<<"inToolbarButton\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
	  break;
   case inStructure:
  	  DEBUG_OUT<<"inStructure\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
	  break;
#endif
    default:
      DEBUG_OUT<<"MacFindWindow returned unknown part!\n";
      break;
  }
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
// FIXME: Make sure we don't handle any events that aren't for us.
void AEvent::macMouseUp(EventRecord *macEvent, long *longWhere)
{
  Point myPoint;
  // TODO: Are these reversed on intel vs ppc?
  myPoint.h=((*longWhere)>>16)&0xffff;
  myPoint.v=(*longWhere)&0xffff;
  x=myPoint.h;  y=myPoint.v;
  DEBUG_OUT<<"macMouseUp x="<<x<<" y="<<y<<"\n";
  Rect parentRect;
  parentRect.top=22; parentRect.left=4;
  parentRect.bottom=theWin->getParent()->getHeight();
  parentRect.right=theWin->getParent()->getWidth();
  WindowPtr macWinPtr;
  unsigned int part=0;
#if !__LP64__
  MacFindWindow(myPoint,&macWinPtr);
#endif // __LP64__
  if(macWinPtr) DEBUG_OUT<<"Got a window pointer\n";  else DEBUG_OUT<<"Got null window pointer\n";
  unsigned long longMenuWhere=0;
  unsigned long lGrowWhere=0;
#ifdef MAC_USE_GETWINPORT
  //WindowPeek wPeek=(WindowPeek)NULL;
#endif // MAC_USE_GETWINPORT
  //WStateData *macWS=(WStateData *)NULL;
  bool clicked=false;
  switch(part) {
    case inDesk:
	    DEBUG_OUT<<"inDesk\n";
      // Is there a DeskClick() I should call here?
      break;
    case inMenuBar:
	    DEBUG_OUT<<"inMenuBar\n";
      // FIXME: enable or disable menus here for DA's vs. our windows
#if !__LP64__
      longMenuWhere=MenuSelect(macEvent->where);
#endif // __LP64__
      macMenus(*this,longMenuWhere,macEvent);
      break;
#if !TARGET_API_CARBON
    case inSysWindow:
	    DEBUG_OUT<<"inSysWindow\n";
      SystemClick(macEvent,macWinPtr);
      break;
#endif // TARGET_API_CARBON
    case inContent:
	    DEBUG_OUT<<"inContent\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
      type=A_EVENT_MOUSEUP;
      break;
    case inDrag:
	    DEBUG_OUT<<"inDrag\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
	    DragWindow(macWinPtr,myPoint,&parentRect);
#endif // __LP64__
	    type=A_EVENT_MOVED;
	    aThisApp.yieldToOtherProcesses();
      break;
    case inGrow:
	    DEBUG_OUT<<"inGrow\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
  	  lGrowWhere=GrowWindow(macWinPtr,myPoint,&parentRect);
#endif // __LP64__
	    if(lGrowWhere) theWin->resize(lGrowWhere&0xffff,(lGrowWhere&0xffff0000)>>16);
	    theWin->sync(); type=A_EVENT_RESIZE;
	    aThisApp.yieldToOtherProcesses();
      break;
    case inZoomIn:
    case inZoomOut:
	    DEBUG_OUT<<"inZoomIn/Out\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
        // wPeek=(WindowPeek)macWinPtr;
        // macWS=(WStateData *)(*(wPeek->dataHandle));
#if !__LP64__
		clicked=TrackBox(macWinPtr,macEvent->where,part);
#endif // __LP64__
        if(clicked) {
#if TARGET_API_MAC_CARBON
          Rect tRect;
#if !__LP64__
          GetWindowPortBounds(macWinPtr,&tRect);
#endif // __LP64__
#ifdef USE_QUARTZ
#else
          MacSetPort(GetWindowPort(macWinPtr));
          EraseRect(&tRect);
#endif // USE_QUARTZ
#if !__LP64__
          ZoomWindow(macWinPtr,part,true);
          GetWindowPortBounds(macWinPtr,&tRect);
          InvalWindowRect(macWinPtr,&tRect);
#endif // __LP64__
#else
          MacSetPort(macWinPtr);
          EraseRect(&(macWinPtr->portRect));
          ZoomWindow(macWinPtr,part,true);
          InvalRect(&(macWinPtr->portRect));
#endif // TARGET_API_MAC_CARBON
        }
        /*
        if(part==inZoomIn)
          theWin->resize(macWS->userState.right-macWS->userState.left,
            macWS->userState.bottom-macWS->userState.top);
        else
          theWin->resize(macWS->stdState.right-macWS->stdState.left,
            macWS->stdState.bottom-macWS->stdState.top);
        */
        theWin->sync(); type=A_EVENT_RESIZE;
        aThisApp.yieldToOtherProcesses();
      break;
    case inGoAway:
	    DEBUG_OUT<<"inGoAway\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
      if(TrackGoAway(macWinPtr,myPoint)) type=A_EVENT_DESTROY;
#endif // __LP64__
      break;
    case inCollapseBox:
	    DEBUG_OUT<<"inCollapseBox\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
	  break;
    case inProxyIcon:
  	  DEBUG_OUT<<"inProxyIcon\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
	  break;
#if TARGET_API_MAC_OSX
   case inToolbarButton:
	   DEBUG_OUT<<"inToolbarButton\n";
#if !__LP64__
     if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
     break;
   case inStructure:
     DEBUG_OUT<<"inStructure\n";
#if !__LP64__
      if(macWinPtr!=FrontWindow()) SelectWindow(macWinPtr);
#endif // __LP64__
	  break;
#endif
    default:
      DEBUG_OUT<<"MacFindWindow returned unknown part!\n";
      break;
  }
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
// FIXME: Some code is commented out since "User" menus aren't
// implemented yet.  That is, anything except quit and about.
void macMenus(AEvent& ev, unsigned long choice,
  EventRecord *macEvent)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"macMenus!\n";
#endif // DEBUG_VERBOSE
  unsigned int menu,item;
  menu=(choice>>16)&0xffff;
  item=(choice)&0xffff;
  Str255 mname;
  GrafPtr SavePort;
#if !TARGET_API_MAC_CARBON
  WindowPeek frontWindow;
#endif // TARGET_API_MAC_CARBON
  switch(menu) {
#if !TARGET_API_MAC_CARBON
    case 0:  // Not in any of our menus
      SystemClick(macEvent,(GrafPort *)ev.getWindow()->getMacWindowPtr());
      break;
#endif // TARGET_API_MAC_CARBON
    case 1:  // Apple Menu
      switch(item) {
        case 1:  // About...
        case 2:  // About ALib...
          if(aThisApp.menuItem[A_MENU_ABOUT])
          { ev.type=A_EVENT_MENU;  ev.code=A_MENU_ABOUT; }
          else alib_about();
          break;
        default:
#ifdef USE_QUARTZ
#else
          GetPort(&SavePort);
#endif // USE_QUARTZ
#if !__LP64__
          GetMenuItemText(alib_appleMenu,item,mname);
#endif // __LP64__
#if !TARGET_API_MAC_CARBON
          int num=OpenDeskAcc(mname);
#endif // TARGET_API_MAC_CARBON
#ifdef USE_QUARTZ
#else
          MacSetPort(SavePort);
#endif // USE_QUARTZ
          aThisApp.yieldToOtherProcesses();
          break;
      }
      break;
    case 2:  // File menu
      switch(item) {
        case 1:  // New
          if(aThisApp.menuItem[A_MENU_NEW]) {
            ev.type=A_EVENT_MENU;  ev.code=A_MENU_NEW;
          }
          break;
        case 2:  // Open
          if(aThisApp.menuItem[A_MENU_OPEN]) {
            ev.type=A_EVENT_MENU;  ev.code=A_MENU_OPEN;
          }
          break;
        case 3:  // Close
#if !TARGET_API_MAC_CARBON
          frontWindow=(WindowPeek)FrontWindow();
          if(frontWindow==0L) ;  // What does this mean?
          else {
            if(frontWindow->windowKind<0)
              CloseDeskAcc(frontWindow->windowKind);
            else {
#endif // TARGET_API_MAC_CARBON
              if(aThisApp.menuItem[A_MENU_CLOSE])
              { ev.type=A_EVENT_MENU;  ev.code=A_MENU_CLOSE; }
              else ev.type=A_EVENT_DESTROY;
#if !TARGET_API_MAC_CARBON
            }
          }
#endif // TARGET_API_MAC_CARBON
          // FIXME: Should check for windows other than current one...
          break;
        case 5:  // Save
          if(aThisApp.menuItem[A_MENU_SAVE]) {
            ev.type=A_EVENT_MENU;  ev.code=A_MENU_SAVE;
          }
          break;
        case 6:  // Save As
          if(aThisApp.menuItem[A_MENU_SAVEAS]) {
            ev.type=A_EVENT_MENU;  ev.code=A_MENU_SAVEAS;
          }
          break;
        case 9:  // Print
          if(aThisApp.menuItem[A_MENU_PRINT]) {
            ev.type=A_EVENT_MENU;  ev.code=A_MENU_PRINT;
          }
          break;
        case 11:  // Quit
          if(aThisApp.menuItem[A_MENU_QUIT])
          { ev.type=A_EVENT_MENU;  ev.code=A_MENU_QUIT; }
          else ev.type=A_EVENT_BREAK;
          break;
        default: break;
      }
      break;
    case 3:  // Edit menu
      switch(item) {
        case 1:  // Undo item
          //if(!SystemEdit(undoCmd)) if(menuItem[A_MENU_UNDO])
          { ev.type=A_EVENT_MENU;  ev.code=A_MENU_UNDO; }
          break;
        case 3:  // Cut item
          //if(!SystemEdit(cutCmd)) if(menuItem[A_MENU_CUT])
          { ev.type=A_EVENT_MENU;  ev.code=A_MENU_CUT; }
          break;
        case 4:  // Copy item
          //if(!SystemEdit(copyCmd)) if(menuItem[A_MENU_COPY])
          { ev.type=A_EVENT_MENU;  ev.code=A_MENU_COPY; }
          break;
        case 5:  // Paste item
          //if(!SystemEdit(pasteCmd)) if(menuItem[A_MENU_PASTE])
          { ev.type=A_EVENT_MENU;  ev.code=A_MENU_PASTE; }
          break;
        case 7:  // Clear item
          //if(!SystemEdit(clearCmd)) if(menuItem[A_MENU_CLEAR])
          { ev.type=A_EVENT_MENU;  ev.code=A_MENU_CLEAR; }
          break;
        default: break;
      }
      break;
    default:
      DEBUG_OUT<<"MENU: Unknown "<<menu<<","<<item<<".\n";
      //userMenu(0);
      break;
  }
#if !__LP64__
  HiliteMenu(0);
#endif // __LP64__
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
void alib_about()
{
  bool done=false;
  DEBUG_OUT<<"alib_about()..."<<aThisApp.getNativeCPUType()<<" "<<aThisApp.getNativeOSType()<<" "<<aThisApp.getNativeOSVersion()<<"\n";
  unsigned int aNum=133;
  if(!strcmp(aThisApp.getNativeCPUType(),"ppc")) aNum=134;
#if TARGET_CARBON
  DEBUG_OUT<<"TARGET_CARBON...\n";
  aNum=135;
#endif // TARGET_CARBON
#if !__LP64__
  Alert(aNum,(ModalFilterUPP)NULL);
#endif // __LP64__
  done=true;
  if(!done) {
    AWindow w(ATEXT("About..."),300,150);
    bool working=true;
    w.clear();
    w.moveTo(0,50);
    w.center("ALib Libraries");
    w.moveTo(0,70);
    w.center("Version");
    //w.moveTo(0,85).center(ALIB_VERSION);
    w.moveTo(0,100);
    w.center("By Nicholas Lauland");
    AEvent e;
    while(working) {
      //w.focus();
      e.wait(&w);
      if(e.type==A_EVENT_DESTROY) working=false;
    };
  }
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
void AApp::mac_start()
{
  DEBUG_OUT<<"mac_start()...\n";
  alib_bugHunt=false;
#ifdef USE_MAC_BUGHUNT
  InitBugHunt();
  bugHunt=true;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Running with bugHunt!\n";
#endif // DEBUG_VERBOSE
#endif // USE_MAC_BUGHUNT
  alib_inBackground=false;
  // Query system
  // Assume at least this...
  version=8;  versionMinor=0;
  bool hasColorQD=true;
  SInt32 response=0;
#if TARGET_CARBON
  // FIXME: Should use this for all System 7 or higher...not just Carbon
  if(!Gestalt(gestaltSystemVersion,&response)) {
    unsigned int version1=(response&0xff00)>>8;
    version=(10*((version1&0x00f0)>>4))+(version1&0x000f);
    versionMinor=(10*((response&0x00f0)>>4))+(response&0x000f);
  }
#else
  SysEnvRec macEnv;
  macEnv.hasColorQD=hasColorQD;
  SysEnvirons(1,&macEnv);
  unsigned long ver=macEnv.systemVersion;
  version=(ver&0xff00)>>8;
  versionMinor=(10*((ver&0x00f0)>>4))+(ver&0x000f);
#endif // TARGET_CARBON
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"MacOS Version: "<<version<<".";
  if(versionMinor<10) DEBUG_OUT<<"0";
  DEBUG_OUT<<versionMinor<<"\n";
#endif // DEBUG_VERBOSE
#ifndef REALLY_OLD_MAC
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"-----------------------\n";
  //DEBUG_OUT<<"Looking for some extensions...\n";
#endif // DEBUG_VERBOSE
  if(!hasColorQD) {
#if !__LP64__
    Alert(131,(ModalFilterUPP)NULL);
#endif // __LP64__
    //AMsgWindow mw("This version of ALIB requires Color Quickdraw!");
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"This version of ALIB requires Color Quickdraw!\n";
#endif // DEBUG_VERBOSE
    exit(5);
  }
  if(version<7) {
#if !__LP64__
    Alert(132,(ModalFilterUPP)NULL);
#endif // __LP64__
    //AMsgWindow mw("This version of ALIB requires System 7!");
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"This version of ALIB requires System 7 or higher!\n";
#endif // DEBUG_VERBOSE
    exit(5);
  }
#ifdef USE_OPENGL
  DEBUG_OUT<<"Using OpenGL for 3D.\n";
#endif
#ifdef USE_CARBON_EVENTS
  DEBUG_OUT<<"Using Carbon Events model.\n";
#else
  DEBUG_OUT<<"Using Classic Event model.\n";
#endif // USE_CARBON_EVENTS
#ifdef USE_QUARTZ
  DEBUG_OUT<<"Using Quartz for drawing.\n";
#else
  DEBUG_OUT<<"Using QuickDraw for drawing.\n";
  GDHandle gfxDevice=GetGDevice();
#endif // USE_QUARTZ
  //useAppleEvents=true;
  // FIXME: check for apple events avail...
  // FIXME: check for macsbug avail...
#if TARGET_CARBON
  // TODO: we arbitrarily don't do this on classic so we don't require macos 8
#ifdef MAC_SYSTEM_8
#ifndef MAC_OLD_HEADERS
  if(!Gestalt(gestaltAppearanceAttr,&response)) {
    if(response&0x1) {
      alib_usingAppearance=true;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Using Appearance Manager.\n";
#endif // DEBUG_VERBOSE
    }
  }
#if !__LP64__
  if(alib_usingAppearance) RegisterAppearanceClient();
#endif // __LP64__
#endif // MAC_OLD_HEADERS
#endif // MAC_SYSTEM_8
#endif // TARGET_CARBON
  alib_usingOT=false;
#ifdef MAC_USE_OT
  if(!Gestalt(gestaltOpenTpt,&response)) {
    if(response&0x11) {
      alib_usingOT=true;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Using OpenTransport for TCP/IP networking.\n";
#endif // DEBUG_VERBOSE
    }
    else //DEBUG_OUT.warning("Wanted to use OT, but Gestalt says no!\n");
  }
#endif // MAC_USE_OT
  alib_qtVer=0;  alib_qtRev=0;
#ifdef USE_QUICKTIME
  if(!Gestalt(gestaltQuickTimeVersion,&response)) {
    alib_qtVer=(response>>24)&0xff;  alib_qtRev=(response>>20)&0xf;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"QuickTime "<<alib_qtVer<<"."<<alib_qtRev<<" present.\n";
#endif // DEBUG_VERBOSE
  }
#ifdef DEBUG_VERBOSE
  else DEBUG_OUT<<"(No QuickTime...dang it!)\n";
#endif // DEBUG_VERBOSE
#endif // USE_QUICKTIME
  alib_dmVer=0;  alib_dmRev=0;
#ifdef MAC_USE_DISPLAY_MANAGER
  if(!Gestalt(gestaltDisplayMgrVers,&response)) {
    alib_dmVer=(response>>16)&0xffff;  alib_dmRev=(response>>8)&0xff;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Display Manager "<<alib_dmVer<<".";
    if(alib_dmRev<10) DEBUG_OUT<<"0";
    DEBUG_OUT<<alib_dmRev<<" present.\n";
#endif // DEBUG_VERBOSE
  }
#endif // MAC_USE_DISPLAY_MANAGER
  if(!Gestalt('bbox',&response)) {
    if(response&0x01) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Running in \"MacOS/Classic\" under MacOS X...\n";
#endif // DEBUG_VERBOSE
      alib_blueBox=true;
    }
  }
  alib_cbonVer=0;  alib_cbonRev=0;
  if(!Gestalt('cbon',&response)) {
    alib_cbonVer=(response>>8)&0xff;
    alib_cbonRev=(10*((response&0x00f0)>>4))+(response&0x000f);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"CarbonLib "<<alib_cbonVer<<".";
    if(alib_cbonRev<10) DEBUG_OUT<<"0";
    DEBUG_OUT<<alib_cbonRev<<" present.\n";
#endif // DEBUG_VERBOSE
  }
#if TARGET_CARBON
  else {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"CarbonLib 0.99 (actually 1.0d or earlier)...\n";
#endif // DEBUG_VERBOSE
    alib_cbonVer=0;  alib_cbonRev=99;
  }
#endif // TARGET_CARBON
#if TARGET_CARBON
  alib_navServices=true;
#else
  alib_navServices=false;
#endif // TARGET_CARBON
  // FIXME: there's got to be a better way to tell if Navigation Services is there!
  if((version>=8)&&(versionMinor>=50)) {
    alib_navServices=true;
  }
  if(alib_navServices) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Using Navigation Services instead of Standard File.\n";
#endif // DEBUG_VERBOSE
  }
#endif // REALLY_OLD_MAC
  // Menu inits
  // FIXME: Put the menus in a resource?
  /*
#if !__LP64__
  ClearMenuBar();
#endif // __LP64__
#ifdef __GNUC__
#else
  alib_appleMenu=NewMenu(1,(unsigned char *)"\p\024");  MacInsertMenu(alib_appleMenu,0);
  alib_fileMenu=NewMenu(2,(unsigned char *)"\pFile");  MacInsertMenu(alib_fileMenu,0);
  alib_editMenu=NewMenu(3,(unsigned char *)"\pEdit");  MacInsertMenu(alib_editMenu,0);
#if TARGET_CARBON
  MacInsertMenuItem(alib_appleMenu,(unsigned char *)"\pAbout...",0);
  MacInsertMenuItem(alib_appleMenu,(unsigned char *)"\pAbout ALib...",1);
#else
  MacAppendMenu(alib_appleMenu,(unsigned char *)"\pAbout...");
  MacAppendMenu(alib_appleMenu,(unsigned char *)"\pAbout ALib...");
  AppendResMenu(alib_appleMenu,'DRVR');
#endif // TARGET_CARBON
  MacAppendMenu(alib_fileMenu,(unsigned char *)"\p(New/N;(Open.../O;Close/W");
  MacAppendMenu(alib_fileMenu,(unsigned char *)"\p(-;(Save.../S;(Save As...");
  MacAppendMenu(alib_fileMenu,(unsigned char *)"\p(-;(Page Setup...;(Print.../P");
  if(version<10) MacAppendMenu(alib_fileMenu,(unsigned char *)"\p(-;Quit/Q");
  MacAppendMenu(alib_editMenu,(unsigned char *)"\p(Undo/Z;(-;(Cut/X;(Copy/C;(Paste/V;(-;(Clear/B");
  MacDrawMenuBar();
#endif // __GNUC__
  */
  for(int t=0;t<A_MAX_MENUS;t++) {
     menuItem[t]=false; itemEnabled[t]=false;
  }
  menuItem[A_MENU_CLOSE]=false;  itemEnabled[A_MENU_CLOSE]=true;
  menuItem[A_MENU_QUIT]=false;  itemEnabled[A_MENU_QUIT]=true;
  menuItem[A_MENU_ABOUT]=false;  itemEnabled[A_MENU_ABOUT]=true;
  DEBUG_OUT<<"mac_start() done.\n";
}
#endif // ASYS_MAC


#ifdef ASYS_MAC
#ifdef USE_CARBON_EVENTS
pascal OSStatus alib_carbWinEventHandler(EventHandlerCallRef nextHand,EventRef theEvent,void *userData)
{
  //DEBUG_OUT<<"alib_carbWinEventHandler...\n";
  return alib_dispatchCarbonEvent(theEvent);
}
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC


#ifdef ASYS_MAC
#ifdef USE_CARBON_EVENTS
pascal OSStatus alib_carbAppEventHandler(EventHandlerCallRef nextHand,EventRef theEvent,void *userData)
{
  //DEBUG_OUT<<"alib_carbAppEventHandler...\n";
  return alib_dispatchCarbonEvent(theEvent);
}
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC


#ifdef ASYS_MAC
#ifdef USE_CARBON_EVENTS
pascal OSStatus alib_carbCtlEventHandler(EventHandlerCallRef nextHand,EventRef theEvent,void *userData)
{
  //DEBUG_OUT<<"alib_carbCtlEventHandler...\n";
  return alib_dispatchCarbonEvent(theEvent);
}
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC


#ifdef ASYS_MAC
#ifdef USE_CARBON_EVENTS
pascal OSStatus alib_carbMenuEventHandler(EventHandlerCallRef nextHand,EventRef theEvent,void *userData)
{
  //DEBUG_OUT<<"alib_carbMenuEventHandler...\n";
  return alib_dispatchCarbonEvent(theEvent);
}
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC


#ifdef ASYS_MAC
#ifdef USE_CARBON_EVENTS
OSStatus alib_dispatchCarbonEvent(EventRef theEvent)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"alib_dispatchCarbonEvent...";
#endif // DEBUG_VERBOSE
  OSStatus ret=eventNotHandledErr;
  if(!alib_curEvent) {
    //DEBUG_OUT.warning("No alib_curEvent!\n");
    DEBUG_OUT<<"lost event...no alib_curEvent!\n";
    return ret;
  }
  alib_curEvent->type=A_EVENT_NONE;
  alib_curEvent->code=0;
  alib_curEvent->x=0;
  alib_curEvent->y=0;
  Point myPoint;
  char myChar;
  HICommand myCommand;
  unsigned int theCommandID;
  char c1,c2,c3,c4;
  UInt32 c=GetEventClass(theEvent);
  UInt32 k=GetEventKind(theEvent);
  switch(c) {
    case kEventClassWindow:
      DEBUG_OUT<<"kEventClassWindow...";
      switch(k) {
        case kEventWindowActivated:
          DEBUG_OUT<<"kEventWindowActivated!";
          alib_curEvent->type=A_EVENT_FOCUS;
          //ret=noErr;
          break;
        case kEventWindowFocusAcquired:
          DEBUG_OUT<<"kEventWindowFocusAcquired!";
          alib_curEvent->type=A_EVENT_FOCUS;
          //ret=noErr;
          break;
        case kEventWindowFocusRelinquish:
          DEBUG_OUT<<"kEventWindowFocusRelinquish!";
          alib_curEvent->type=A_EVENT_UNFOCUS;
          //ret=noErr;
          break;
        case kEventWindowPaint:
          DEBUG_OUT<<"kEventWindowPaint!";
          alib_curEvent->type=A_EVENT_REDRAW;
          ret=noErr;
          break;
#if !__LP64__
        case kEventWindowUpdate:
          DEBUG_OUT<<"kEventWindowUpdate!";
          alib_curEvent->type=A_EVENT_REDRAW;
          ret=noErr;
          break;
        case kEventWindowDrawContent:
          DEBUG_OUT<<"kEventWindowDrawContent!";
          alib_curEvent->type=A_EVENT_REDRAW;
          ret=noErr;
          break;
        case kEventWindowHandleContentClick:
          DEBUG_OUT<<"kEventWindowHandleContentClick!";
          alib_curEvent->type=A_EVENT_MOUSEDOWN;
          GetEventParameter(theEvent,kEventParamMouseLocation,typeQDPoint,NULL,sizeof(Point),NULL,&myPoint);
          alib_curEvent->x=myPoint.h;
          alib_curEvent->y=myPoint.v;
          ret=noErr;
          break;
#endif // __LP64__
        case kEventWindowDragCompleted:
          DEBUG_OUT<<"kEventWindowDragCompleted!";
          alib_curEvent->type=A_EVENT_MOVED;
          // TODO: Update window x, y
          //ret=noErr;
          break;
        case kEventWindowResizeCompleted:
          DEBUG_OUT<<"kEventWindowResizeCompleted!";
          alib_curEvent->type=A_EVENT_RESIZE;
          // TODO: Update window size
          //ret=noErr;
          break;
        case kEventWindowBoundsChanged:
          DEBUG_OUT<<"kEventWindowBoundsChanged!";
          alib_curEvent->type=A_EVENT_RESIZE;
          // TODO: Update window size
          //ret=noErr;
          break;
        case kEventWindowClose:
          DEBUG_OUT<<"kEventWindowClose!";
          alib_curEvent->type=A_EVENT_DESTROY;
          //ret=noErr;
          break;
        default:
          DEBUG_OUT<<"fell through to default!";
          //DEBUG_OUT.warning("unknown window event");
          break;
      }
      break;
    case kEventClassCommand:
      DEBUG_OUT<<"kEventClassCommand...";
      switch(k) {
        case kEventCommandProcess:
          DEBUG_OUT<<"kEventCommandProcess!";
          alib_curEvent->type=A_EVENT_MENU;
          GetEventParameter(theEvent,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&myCommand);
          theCommandID=myCommand.commandID;
          c1=(theCommandID>>24)&0xff;
          c2=(theCommandID>>16)&0xff;
          c3=(theCommandID>>8)&0xff;
          c4=(theCommandID)&0xff;
          DEBUG_OUT<<" commandID was "<<theCommandID<<" ("<<c1<<c2<<c3<<c4<<")";
          switch(theCommandID) {
            default:
              DEBUG_OUT<<" (theCommandID handling not implemented yet!)";
              alib_curEvent->code=A_MENUEVENT_NONE;
              break;
          }
          break;
        default:
          DEBUG_OUT<<"fell through to default!";
          //DEBUG_OUT.warning("unknown command event");
          break;
      }
      break;
    case kEventClassAppleEvent:
      DEBUG_OUT<<"kEventClassAppleEvent...";
      switch(k) {
        case kEventAppleEvent:
          DEBUG_OUT<<"kEventAppleEvent!";
          alib_curEvent->type=A_EVENT_UNKNOWN;
          // TODO: Handle apple event
          //ret=noErr;
          break;
        default:
          DEBUG_OUT<<"fell through to default!";
          //DEBUG_OUT.warning("unknown appleEvent");
          break;
      }
      break;
    case kEventClassMouse:
      DEBUG_OUT<<"kEventClassMouse...";
      switch(k) {
        case kEventMouseDown:
          DEBUG_OUT<<"kEventMouseDown!";
          alib_curEvent->type=A_EVENT_MOUSEDOWN;
          GetEventParameter(theEvent,kEventParamMouseLocation,typeQDPoint,NULL,sizeof(Point),NULL,&myPoint);
          alib_curEvent->x=myPoint.h;
          alib_curEvent->y=myPoint.v;
          //ret=noErr;
          break;
        case kEventMouseUp:
          DEBUG_OUT<<"kEventMouseUp!";
          alib_curEvent->type=A_EVENT_MOUSEUP;
          GetEventParameter(theEvent,kEventParamMouseLocation,typeQDPoint,NULL,sizeof(Point),NULL,&myPoint);
          alib_curEvent->x=myPoint.h;
          alib_curEvent->y=myPoint.v;
          //ret=noErr;
          break;
        case kEventMouseMoved:
          DEBUG_OUT<<"kEventMouseMoved!";
          alib_curEvent->type=A_EVENT_MOUSEMOVE;
          GetEventParameter(theEvent,kEventParamMouseLocation,typeQDPoint,NULL,sizeof(Point),NULL,&myPoint);
          alib_curEvent->x=myPoint.h;
          alib_curEvent->y=myPoint.v;
          //ret=noErr;
          break;
        case kEventMouseDragged:
          DEBUG_OUT<<"kEventMouseDragged!";
          alib_curEvent->type=A_EVENT_MOUSEMOVE;
          GetEventParameter(theEvent,kEventParamMouseLocation,typeQDPoint,NULL,sizeof(Point),NULL,&myPoint);
          alib_curEvent->x=myPoint.h;
          alib_curEvent->y=myPoint.v;
          //ret=noErr;
          break;
        default:
          DEBUG_OUT<<"fell through to default!";
          //DEBUG_OUT.warning("unknown mouse event");
          break;
      }
      break;
    case kEventClassKeyboard:
      DEBUG_OUT<<"kEventClassKeyboard...";
      switch(k) {
        case kEventRawKeyDown:
          DEBUG_OUT<<"kEventRawKeyDown!";
          alib_curEvent->type=A_EVENT_KEYDOWN;
          GetEventParameter(theEvent,kEventParamKeyMacCharCodes,typeChar,NULL,sizeof(char),NULL,&myChar);
          alib_curEvent->code=myChar;
          ret=noErr;
          break;
       case kEventRawKeyUp:
          DEBUG_OUT<<"kEventRawKeyUp!";
          alib_curEvent->type=A_EVENT_KEYUP;
          GetEventParameter(theEvent,kEventParamKeyMacCharCodes,typeChar,NULL,sizeof(char),NULL,&myChar);
          alib_curEvent->code=myChar;
          ret=noErr;
          break;
       case kEventRawKeyRepeat:
          DEBUG_OUT<<"kEventRawKeyRepeat!";
          alib_curEvent->type=A_EVENT_KEYDOWN;
          GetEventParameter(theEvent,kEventParamKeyMacCharCodes,typeChar,NULL,sizeof(char),NULL,&myChar);
          alib_curEvent->code=myChar;
          ret=noErr;
          break;
       default:
          DEBUG_OUT<<"fell through to default!";
          //DEBUG_OUT.warning("unknown keyboard event");
          break;
      }
      break;
    default:
      DEBUG_OUT<<"kEventClass fell through to default!";
      break;
  }
  DEBUG_OUT<<"\n";
  if(!aDefaultAApplicationLoop) aDefaultAApplicationLoop=new AApplicationLoop;
  aDefaultAApplicationLoop->dispatchEvent(alib_curEvent);
  return ret;
}
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC


/*
#ifdef ASYS_MAC
#ifdef USE_CARBON_EVENTS
void alib_debugCarbonEvent(EventRef theEvent)
{
  UInt32 c=GetEventClass(theEvent);
  UInt32 k=GetEventKind(theEvent);
  DEBUG_OUT<<"Carbon Event class was ";
  switch(c) {
    case kEventClassMouse: DEBUG_OUT<<"mouse"; break;
    case kEventClassKeyboard: DEBUG_OUT<<"keyboard"; break;
    case kEventClassApplication: DEBUG_OUT<<"application"; break;
    case kEventClassAppleEvent: DEBUG_OUT<<"appleEvent"; break;
    case kEventClassMenu: DEBUG_OUT<<"menu"; break;
    case kEventClassWindow: DEBUG_OUT<<"window"; break;
    case kEventClassControl: DEBUG_OUT<<"control"; break;
    case kEventClassCommand: DEBUG_OUT<<"command"; break;
    case kEventClassSystem: DEBUG_OUT<<"system"; break;
    default: DEBUG_OUT<<"unknown"; break;
  }
  DEBUG_OUT<<", kind was "<<k<<" (";
  switch(c) {
    case kEventClassWindow:
      switch(k) {
        case kEventWindowActivated: DEBUG_OUT<<"activate"; break;
        case kEventWindowFocusAcquired: DEBUG_OUT<<"focus"; break;
        case kEventWindowFocusRelinquish: DEBUG_OUT<<"unfocus"; break;
        case kEventWindowPaint: DEBUG_OUT<<"paint"; break;
#if !__LP64__

        case kEventWindowUpdate: DEBUG_OUT<<"update"; break;
        case kEventWindowDrawContent: DEBUG_OUT<<"drawContent"; break;
        case kEventWindowHandleContentClick: DEBUG_OUT<<"click"; break;
#endif // __LP64__
        case kEventWindowDragCompleted: DEBUG_OUT<<"moved"; break;
        case kEventWindowResizeCompleted: DEBUG_OUT<<"resized"; break;
        case kEventWindowBoundsChanged: DEBUG_OUT<<"resized/moved"; break;
        case kEventWindowClose: DEBUG_OUT<<"close"; break;
        default: DEBUG_OUT<<"unknown"; break;
      }
      break;
    case kEventClassCommand:
      switch(k) {
        case kEventCommandProcess: DEBUG_OUT<<"command"; break;
        default: DEBUG_OUT<<"unknown"; break;
      }
      break;
    case kEventClassAppleEvent:
      switch(k) {
        case kEventAppleEvent: DEBUG_OUT<<"appleEvent"; break;
        default: DEBUG_OUT<<"unknown"; break;
      }
      break;
    case kEventClassMouse:
      switch(k) {
        case kEventMouseDown: DEBUG_OUT<<"click"; break;
        case kEventMouseUp: DEBUG_OUT<<"unclick"; break;
        case kEventMouseMoved: DEBUG_OUT<<"moved"; break;
        case kEventMouseDragged: DEBUG_OUT<<"dragged"; break;
        default: DEBUG_OUT<<"unknown"; break;
      }
      break;
    case kEventClassKeyboard:
      switch(k) {
        case kEventRawKeyDown: DEBUG_OUT<<"keyDown"; break;
        case kEventRawKeyUp: DEBUG_OUT<<"keyUp"; break;
        default: DEBUG_OUT<<"unknown"; break;
      }
      break;
    default: DEBUG_OUT<<"unknown"; break;
  }
  DEBUG_OUT<<")\n";
}
#endif // USE_CARBON_EVENTS
#endif // ASYS_MAC
*/


#ifdef ASYS_MAC
char *debugMacEventType(unsigned int theType)
{
  char *ret=NULL;
  switch(theType) {
    case nullEvent: ret="nullEvent"; break;
    case mouseDown: ret="mouseDown"; break;
    case mouseUp: ret="mouseUp"; break;
    case keyDown: ret="keyDown"; break;
    case keyUp: ret="keyUp"; break;
    case autoKey: ret="autoKey"; break;
    case updateEvt: ret="updateEvt"; break;
    case diskEvt: ret="diskEvt"; break;
    case activateEvt: ret="activateEvt"; break;
    case osEvt: ret="osEvt"; break;
    case kHighLevelEvent: ret="kHighLevelEvent"; break;
    default:
      //DEBUG_OUT.warning("Unknown event in debugMacEventType!");
      break;
  }
  return ret;
}
#endif // ASYS_MAC


