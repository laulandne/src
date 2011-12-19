
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#ifdef ASYS_AMIGA
#include <alib/aapp.h>
#include <alib/adebug.h>
#include <guilib/aevent.h>
#include <guilib/awindow.h>
#endif // ASYS_AMIGA

#ifdef ASYS_AMIGA
#include <graphics/gfxbase.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <libraries/asl.h>
#include <workbench/workbench.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#endif // ASYS_AMIGA


#ifdef ASYS_AMIGA
Library *WorkbenchBase=NULL;
Library *AslBase=NULL;
Library *GfxBase=NULL;
Library *IntuitionBase=NULL;
#endif


#ifdef ASYS_AMIGA
bool AEvent::amigaHandleEvent(unsigned long signals)
{
  bool retVal=true;
  IntuiMessage *msg=NULL;
  type=A_EVENT_NONE;
#ifdef USE_JOYSTICK
  // This only reads the first stick (not the mouseport)
  if(amigaJoystick(0)) { type=A_EVENT_KEYPRESS; retVal=false; }
#endif // USE_JOYSTICK
  if(signals&winsig) {
    msg=(IntuiMessage *)GetMsg(theWin->getAmyWindow()->UserPort);
    while(msg) {
      unsigned int mclass=msg->Class;
      code=msg->Code; x=msg->MouseX;  y=msg->MouseY;
      ReplyMsg((Message *)msg);
      switch(mclass) {
        case IDCMP_CLOSEWINDOW:
          type=A_EVENT_DESTROY; retVal=false;
          break;
        case IDCMP_MOUSEBUTTONS:
          type=A_EVENT_MOUSEUP; retVal=false;
          break;
        case IDCMP_NEWSIZE:  case IDCMP_CHANGEWINDOW:
          theWin->sync(); type=A_EVENT_RESIZE; retVal=false;
          break;
        case IDCMP_REFRESHWINDOW:
          BeginRefresh(theWin->getAmyWindow());
          EndRefresh(theWin->getAmyWindow(),true);
          theWin->sync(); type=A_EVENT_REDRAW; retVal=false;
          break;
        case IDCMP_VANILLAKEY:
          type=A_EVENT_KEYPRESS; retVal=false;
          break;
        default:
          dBug<<"Unhandled amiga event! ("<<mclass<<")\n";
          break;
      }  // switch
      msg=(IntuiMessage *)GetMsg(theWin->getAmyWindow()->UserPort);
    }  // while
  }  // if
  if(signals&appsig) {
    void *data=theWin->handleAppMessage(signals);
    if(data) { type=A_EVENT_DRAGNDROP; retVal=false; }
    else dBug<<"NULL drop.\n";
  }
  return retVal;
}
#endif // ASYS_AMIGA


#ifdef ASYS_AMIGA
#ifdef USE_JOYSTICK
// FIXME: This doesn't handle the second fire button for some reason...
unsigned long AEvent::amigaJoystick(unsigned int joynum)
{
  joynum=!joynum;  // This is so stick#0 is not the mouse
  UWORD joy;
  if(joynum) joy=custom.joy1dat;  else joy=custom.joy0dat;
  static UBYTE ret=0;
  enum joydirs {
    UP=1,DOWN=2,LEFT=4,RIGHT=8,FIRE1=16,FIRE2=32
  };
  ret+=(joy>>1^joy)&0x0100?UP:0;
  ret+=(joy>>1^joy)&0x0001?DOWN:0;
  ret+=joy&0x0200?LEFT:0;
  ret+=joy&0x0002?RIGHT:0;
  if(joynum) {
    ret+=!(ciaa.ciapra&0x0080)?FIRE1:0;
    ret+=!(custom.potinp&0x4000)?FIRE2:0;
  }
  else {
    ret+=!(ciaa.ciapra&0x0040)?FIRE1:0;
    ret+=!(custom.potinp&0x0400)?FIRE2:0;
  }
  if(ret) {
    if(ret&UP) { code=A_KEY_UP; /* dBug<<"joy up\n"; */ }
    if(ret&DOWN) { code=A_KEY_DOWN; /* dBug<<"joy down\n"; */ }
    if(ret&LEFT) { code=A_KEY_LEFT; /* dBug<<"joy left\n"; */ }
    if(ret&RIGHT) { code=A_KEY_RIGHT; /* dBug<<"joy right\n"; */ }
    if(ret&FIRE1) { code=' '; /* dBug<<"joy fire1\n"; */ }
    /* if(ret&FIRE2) { code='\t'; dBug<<"joy fire2\n"; } */

  }
  data=(char *)&ret;
  return code;
}
#endif // USE_JOYSTICK
#endif // ASYS_AMIGA


#ifdef ASYS_AMIGA
void AApp::amiga_start()
{
  dBug<<"amiga_start()...\n";
  //dBug<<"Opening Amiga system libraries...\n";
  GfxBase=OpenLibrary((unsigned char *)"graphics.library",33);
  if(!GfxBase) exit(5);
  IntuitionBase=OpenLibrary((unsigned char *)"intuition.library",33);
  if(!IntuitionBase) exit(5);
  AslBase=OpenLibrary((unsigned char *)"asl.library",36);
  // NOTE: Ok if we don't get asl
  WorkbenchBase=OpenLibrary((unsigned char *)"workbench.library",36);
  // NOTE: Ok if we don't get workbench
  // FIXME: read system version from version.library
  /*
struct Library *VersionBase;
if (VersionBase = OpenLibrary("version.library",0))
{
       if (VersionBase->lib_Version >= 38)
       {         */  /* user is running at least V38 Workbench */   /*    }
       else
       {       */    /* user is running at most V37 Workbench */  /*     }
}
else
{   */  /* can't tell what the user is running, assume the minimum version
       * that your application supports        */ /*
}
*/
  // Read system version from graphics.library
  struct GfxBase *gBase=(struct GfxBase *)GfxBase;
  version=gBase->LibNode.lib_Version;
  versionMinor=gBase->LibNode.lib_Revision;
  dBug<<"Amiga System version "<<version<<"."<<versionMinor<<" (";
  switch(version) {
    case 33: dBug<<"1.2"; break;
    case 34: case 35: dBug<<"1.3"; break;
    case 36: dBug<<"2.0"; break;
    case 37: dBug<<"2.04"; break;
    case 38: dBug<<"2.1"; break;
    case 39: dBug<<"3.0"; break;
    case 40: dBug<<"3.1"; break;
    default: if(version<33) dBug<<"1.x"; else dBug<<"???";
  }
  dBug<<")\n";
  if(version>35) {
    Library *WorkbenchBase=OpenLibrary((unsigned char *)"workbench.library",36);
    Library *AslBase=OpenLibrary((unsigned char *)"asl.library",36);
  }
  dBug<<"-----------------------\n";
  // All this stuff is to satisfy my curiousity...
  if(gBase->DisplayFlags&NTSC) dBug<<"NTSC system.\n";
  if(gBase->DisplayFlags&PAL) dBug<<"PAL system.\n";
  if(gBase->DisplayFlags&REALLY_PAL) dBug<<"REALLY_PAL system.\n";
  if(version>35) {
    // Amiga custom chips
    // A machine can have one ECS chip but not both, but always both AGA chips
    hasECS=false;  hasAGA=false;
    bool agnusFlag=false;
    dBug<<"Custom chips found: ";
    // DMA/Coprocessor chip
    if(gBase->ChipRevBits0&GFXF_AA_ALICE) {
      dBug<<"ALICE, "; hasAGA=true;
    }
    else {
      if(gBase->ChipRevBits0&GFXF_HR_AGNUS) {
        dBug<<"ECS_AGNUS, "; agnusFlag=true;
      }
    }
    if(hasAGA) hasECS=true;
    if(!hasECS) { if(!agnusFlag) dBug<<"AGNUS, "; }
    // Graphics chip
    if(gBase->ChipRevBits0&GFXF_AA_MLISA) dBug<<"MLISA "; else
    if(gBase->ChipRevBits0&GFXF_AA_LISA) dBug<<"LISA "; else
    if(gBase->ChipRevBits0&GFXF_HR_DENISE) {
      dBug<<"ECS_DENISE ";  if(agnusFlag) hasECS=true;
    }
    else dBug<<"DENISE ";
    dBug<<"\n";
  }
  if(getenv("ALIB_ECS")) { hasECS=true; dBug<<"Forcing ECS...\n"; }
  if(getenv("ALIB_AGA")) { hasAGA=true; dBug<<"Forcing AGA...\n"; }
  if(getenv("ALIB_OCS")) {
    hasECS=false; hasAGA=false; dBug<<"Forcing OCS...\n";
  }
  dBug<<"GfxBase info:\n";
  dBug<<"  DisplayFlags: "<<(long)gBase->DisplayFlags<<"\n";
  if(version>35) {
    //dBug<<"  ChipRevBits0: "<<(long)gBase->ChipRevBits0<<"\n";
  }
  if(version>38) {
    dBug<<"  MonitorFlags: "<<(long)gBase->MonitorFlags<<"\n";
    dBug<<"  GfxFlags: "<<(long)gBase->GfxFlags<<"\n";
    dBug<<"  WantChips: "<<(long)gBase->WantChips<<"\n";
    dBug<<"  BoardMemType: "<<(long)gBase->BoardMemType<<"\n";
    dBug<<"  MemType: "<<(long)gBase->MemType<<"\n";
    dBug<<"  Bugs: "<<(long)gBase->Bugs<<"\n";
    dBug<<"  ColorMask: "<<aThisApp.myHex(gBase->ColorMask)<<"\n";
  }
  // NOTE: If gfxlib is v40 then we have chunky-to-planar routines...
  oldDir=CurrentDir((BPTR)NULL);
  CurrentDir(oldDir);
}
#endif // ASYS_AMIGA

