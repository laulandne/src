
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#ifdef ASYS_PALM
#include <alib/adebug.h>
#include <alib/aapp.h>
#include <guilib/aevent.h>
#include <guilib/alib_palm.h>
#endif // ASYS_PALM


#ifdef ASYS_PALM
FormPtr alibPalmFormP=NULL;
#endif // ASYS_PALM


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_PALM
// DESCRIPTION: This routine checks that a ROM version is meet your
//  minimum requirement.
Err palmRomVersionCompatible(DWord requiredVersion, Word launchFlags)
{
  DWord romVersion;
  // See if we're on in minimum required version of the ROM or later.
  FtrGet(sysFtrCreator,sysFtrNumROMVersion,&romVersion);
  if(romVersion<requiredVersion)
  {
    if((launchFlags&(sysAppLaunchFlagNewGlobals|sysAppLaunchFlagUIApp))==
      (sysAppLaunchFlagNewGlobals|sysAppLaunchFlagUIApp))
    {
      FrmAlert(1001);
      // Pilot 1.0 will continuously relaunch this app unless we switch to
      // another safe one.
      if(romVersion<version20)
      {
        //Err err;
        AppLaunchWithCommand(sysFileCDefaultApp,sysAppLaunchCmdNormalLaunch,NULL);
      }
    }
    return sysErrRomIncompatible;
  }
  return 0;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: This routine returns a pointer to an object in the current
//  form.
VoidPtr palmGetObjectPtr(Word objectID)
{
  FormPtr frmP;
  frmP=FrmGetActiveForm();
  return FrmGetObjectPtr(frmP,FrmGetObjectIndex(frmP,objectID));
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: This routine loads form resources and set the event
//  handler for the form loaded.
Boolean palmAppHandleEvent( EventPtr eventP)
{
#ifdef DEBUG_VERBOSE
  dBug<<"palmAppHandleEvent...\n";
#endif // DEBUG_VERBOSE
  Word formId;
  if(eventP->eType==frmLoadEvent)
  {
    // Load the form resource.
    formId=eventP->data.frmLoad.formID;
    alibPalmFormP=FrmInitForm(formId);
    FrmSetActiveForm(alibPalmFormP);
    //WinEraseWindow();
    // Set the event handler for the form.  The handler of the currently
    // active form is called by FrmHandleEvent each time is receives an
    // event.
    switch(formId)
    {
      case 1000:
        FrmSetEventHandler(alibPalmFormP,palmMainFormHandleEvent);
        break;
      default:
        //  ErrFatalDisplay("Invalid Form Load Event");
        break;
    }
    return true;
  }
  return false;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: This routine is the event loop for the application.
void palmAppEventLoopOnce(void)
{
#ifdef DEBUG_VERBOSE
  dBug<<"palmAppEventLoopOnce...\n";
#endif // DEBUG_VERBOSE
  Word error;
  EventType event;
  //do {
    EvtGetEvent(&event,evtWaitForever);
    if(!SysHandleEvent(&event))
      if(!MenuHandleEvent(0,&event,&error))
        if(!palmAppHandleEvent(&event))
          FrmDispatchEvent(&event);
  //} while (event.eType != appStopEvent);
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION:  Get the current application's preferences.
Err palmAppStart(void)
{
  StarterPreferenceType prefs;
  Word prefsSize;
  // Read the saved preferences / saved-state information.
  prefsSize=sizeof(StarterPreferenceType);
  if(PrefGetAppPreferences(appFileCreator,appPrefID,&prefs,&prefsSize,true)!=
    noPreferenceFound)
  {
    // TODO: read prefs here...
  }
  return 0;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: Save the current state of the application.
void palmAppStop(void)
{
  StarterPreferenceType prefs;
  // Write the saved preferences / saved-state information.  This data
  // will be backed up during a HotSync.
  PrefSetAppPreferences(appFileCreator,appPrefID,appPrefVersionNum,
    &prefs,sizeof(prefs),true);
  if(alibPalmFormP) FrmDeleteForm(alibPalmFormP);
  alibPalmFormP=NULL;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: This is the main entry point for the application.
DWord palmStarterPalmMain(Word cmd,Ptr cmdPBP,Word launchFlags)
{
  Err error=palmRomVersionCompatible(version20,launchFlags);
  if(error) return error;
  char *aGlobalPalmAppName="Nick App";
  switch (cmd)
  {
    case sysAppLaunchCmdNormalLaunch:
      error=palmAppStart();
      if(error) return error;
      FrmGotoForm(1000);
      //char *aGlobalPalmAppName="Nick App";
      //start(0,(char **)&aGlobalPalmAppName);
      // Call client's main() (Whose name has been "mangled")
      //return (int)myMain(0,(char **)&aGlobalPalmAppName);
#ifdef DEBUG_VERBOSE
      dBug<<"About to call main()...\n";
#endif // DEBUG_VERBOSE
      extern int main(int argc,char **argv);
      return (int)main(0,(char **)&aGlobalPalmAppName);
      palmAppStop();
      break;
    default:
      break;
  }
  return 0;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: This is the main entry point for the application.
DWord PilotMain(Word cmd,Ptr cmdPBP,Word launchFlags)
{
  return palmStarterPalmMain(cmd,cmdPBP,launchFlags);
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: This routine initializes the MainForm form.
void palmMainFormInit(FormPtr frmP)
{
#ifdef DEBUG_VERBOSE
  dBug<<"palmMainFormInit\n";
#endif // DEBUG_VERBOSE
  // nothing to do
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: This routine performs the menu command specified.
Boolean palmMainFormDoCommand(Word command)
{
  Boolean handled=false;
#ifdef DEBUG_VERBOSE
  dBug<<"palmMainFormDoCommand "<<command<<"\n";
#endif // DEBUG_VERBOSE
  switch(command)
    {
    case 1000:
      handled=true;
      break;
    }
  return handled;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
// DESCRIPTION: This routine is the event handler for the
Boolean palmMainFormHandleEvent(EventPtr eventP);
Boolean palmMainFormHandleEvent(EventPtr eventP)
{
  Boolean handled=false;
  FormPtr frmP;
#ifdef DEBUG_VERBOSE
  dBug<<"palmMainFormHandleEvent: "<<(int)eventP->eType<<"\n";
#endif // DEBUG_VERBOSE
  alib_curEvent->type=A_EVENT_NONE;
  alib_curEvent->x=eventP->screenX;
  alib_curEvent->y=eventP->screenY;
  switch(eventP->eType)
    {
    case menuEvent:
      handled=palmMainFormDoCommand(eventP->data.menu.itemID);
      break;
    case frmOpenEvent:
      frmP=FrmGetActiveForm();
      palmMainFormInit(frmP);
      //FrmDrawForm(frmP);
      //WinEraseWindow();
      handled=true;
      alib_curEvent->type=A_EVENT_REDRAW;
      break;
    case frmCloseEvent:
    case winExitEvent:
    case appStopEvent:
      alib_curEvent->type=A_EVENT_DESTROY;
      palmAppStop();
      break;
    case penMoveEvent:
      alib_curEvent->type=A_EVENT_MOUSEMOVE;
      alib_curEvent->code=0;
      alib_curEvent->modifier=0;  // FIXME: handle palm modifiers...
      break;
    case penDownEvent:
      alib_curEvent->type=A_EVENT_MOUSEDOWN;
      alib_curEvent->code=0;
      alib_curEvent->modifier=0;  // FIXME: handle palm modifiers...
      break;
    case penUpEvent:
      alib_curEvent->type=A_EVENT_MOUSEUP;
      alib_curEvent->code=0;
      alib_curEvent->modifier=0;  // FIXME: handle palm modifiers...
      break;
    case keyDownEvent:
      // NOTE: we send a key up, not a key down like you might expect...
      alib_curEvent->type=A_EVENT_KEYUP;
      alib_curEvent->code=eventP->data.keyDown.chr;
      alib_curEvent->modifier=0;  // FIXME: handle palm modifiers...
    break;
    case frmUpdateEvent:
    //case winEnterEvent:
      alib_curEvent->type=A_EVENT_REDRAW;
      break;
    default:
      break;
    }
    return handled;
}
#endif // ASYS_PALM


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_PALM
int sscanf(char *s,char *f,unsigned int *val)
{
  if(!s) return 0;
  if(!f) return 0;
  if(!val) return 0;
  bool done=false;
  if(!strcmp(f,"&d")) {
    // single decimal integer
    *val=StrAToI(s);
    done=true;
    return 1;
  }
  if(!strcmp(f,"&x")) {
    // single hexidecimal integer
    dBug.warning("alib_palm fake sscanf need to implement hex!\n");
    return 0;
    done=true;
  }
  if(!done) {
    dBug.warning("alib_palm fake sscanf with unknown fmt!\n");
    return 0;
  }
  return 0;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int fscanf(FILE *fd,char *f,char *val)
{
  if(!fd) return 0;
  if(!f) return 0;
  if(!val) return 0;
  bool done=false;
  //
  if(!done) {
    dBug.warning("alib_palm fake fscanf with unknown fmt!\n");
    return 0;
  }
  return 0;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int chdir(char *dir)
{
  // Don't do anything since we don't have directories, per se
  return 0;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
char *getcwd(char *dir,unsigned int len)
{
  if(!dir) return NULL;
  // We just return / so they get something that looks ok
  dir[0]='/';
  dir[1]=0;
  return dir;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int fstat(int fd,struct stat *s)
{
  if(!s) return -1;
  dBug.warning("Need to implement alib_palm fake fstat!\n");
  return -1;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int stat(char *filename,struct stat *s)
{
  if(!filename) return -1;
  if(!s) return -1;
  dBug.warning("Need to implement alib_palm fake stat!\n");
  return -1;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int fputc(int c,FILE *fd)
{
  //dBug.warning("Need to implement alib_palm fake fputc!\n");
  return c;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int fputs(char *str,FILE *fd)
{
  //dBug.warning("Need to implement alib_palm fake fputs!\n");
  return EOF;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int fgetc(FILE *fd)
{
  dBug.warning("Need to implement alib_palm fake fgetc!\n");
  return 0;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
char *fgets(char *str,unsigned int len,FILE *fd)
{
  dBug.warning("Need to implement alib_palm fake fgets!\n");
  return NULL;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
unsigned int wcslen(wchar_t *s)
{
  if(!s) return 0;
  unsigned int ret=0;
  while(s[ret]) ret++;
  return ret;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int sin(float f)
{
  // TODO: Actually implement this...maybe a crude lookup table?
  dBug.warning("Need to implement alib_palm fake sin!\n");
  return 0;
}
#endif // ASYS_PALM


#ifdef ASYS_PALM
int cos(float f)
{
  // TODO: Actually implement this...maybe a crude lookup table?
  dBug.warning("Need to implement alib_palm fake cos!\n");
  return 0;
}
#endif // ASYS_PALM


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ASYS_PALM
void AApp::palm_start()
{
  dBug<<"palm_start()...\n";
  unsigned long value;
  Err osErr;
  osErr=FtrGet(sysFtrCreator,sysFtrNumROMVersion,&value);
  version=sysGetROMVerMajor(value);
  versionMinor=sysGetROMVerMinor(value);
  dBug<<"PalmOS version: "<<version<<"."<<versionMinor<<"\n";
  if(value>0x03503000) {
    dBug<<"Version 3.5 features present.\n";
    hasThreePointFive=true;
  }
  hasIntl=false;
  osErr=FtrGet(sysFtrCreator,sysFtrNumIntlMgr,&value);
  if(!osErr) {
    if(value) {
      dBug<<"International features present.\n";
      hasIntl=true;
    }
  }
  hasJapanese=false;
  osErr=FtrGet(sysFtrCreator,sysFtrNumEncoding,&value);
  if(!osErr) {
    if(value==charEncodingCP932) {
      dBug<<"Japanese features present.\n";
      hasJapanese=true;
    }
  }
  hasWireless=false;
  /*
  DmSearchStateType searchState;
  UInt cardNo;
  LocalID dbID;
  osErr=DmGetNextDatabaseByTypeCreator(true,&searchState,
    sysFileTApplication,sysFileCClipper,true,&cardNo,&dbID);
  if(!osErr) {
    dBug<<"Wireless features present.\n";
    wirelessFeature=true;
  }
  */
  hasColor=false;
#ifdef __PALMOS_H__
  if(hasThreePointFive) {
    UInt32 tw,th,td;
    Boolean tc;
    Err terr;
    terr=WinScreenMode(winScreenModeGet,&tw,&th,&td,&tc);
    //dBug<<"winScreenModeGet:"<<tw<<","<<th<<","<<td<<","<<tc<<"\n";
    if(tc) {
      dBug<<"Device supports color.\n";
      hasColor=true;
    }
  }
#endif // __PALMOS_H__
  DWord id, chip;
  Word revision;
  Err err;
  err=FtrGet(sysFtrCreator,sysFtrNumProcessorID,&id);
  if(!err) {
    chip=id&sysFtrNumProcessorMask;
    revision=id&0x0ffff;
    switch(chip) {
      case sysFtrNumProcessor328:
        dBug<<"Traditional Dragonball cpu.\n";
        break;
      case sysFtrNumProcessorEZ:
        dBug<<"Dragonball EZ cpu.\n";
        break;
      default:
        dBug<<"Unknown cpu type.\n";
        break;
    }
  }
}
#endif // ASYS_PALM
