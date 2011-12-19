
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ADisplay.h>
#include <guilib/ABitmapInter.h>
#include <guilib/ABitmapInterFour.h>
#include <guilib/ABitmapPlanes.h>
#include <guilib/ABitmapBanksPlanes.h>
#include <guilib/ABitmapBanks.h>
#include <guilib/ABitmapText.h>
#include <guilib/ABitmapFlipped.h>

#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/AList.h>


#ifdef ASYS_AMIGA
#include <guilib/alib_amiga.h>
#include <intuition/screens.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#endif // ASYS_AMIGA

#ifdef ASYS_DOS
#include <guilib/adisplay.h>
#include <guilib/alib_dos.h>
#endif // ASYS_DOS

#ifdef ASYS_WIN16
#ifndef DEVMODE
#define DEVMODE void
#endif // DEVMODE
#endif // ASYS_WIN16


#define DEBUG_VERBOSE 1


ADisplay *aDefaultADisplay=NULL;


////////////////////////////////////////////////////////////////////////////////
//  ADisplay Class
////////////////////////////////////////////////////////////////////////////////

unsigned int ADisplay::numDisplays=0;
//ADisplay *ADisplay::defaultDisplay=(ADisplay *)NULL;
AList *ADisplay::dmList=(AList *)NULL;
unsigned int ADisplay::videoHardware=0;
unsigned int ADisplay::defaultDepth=0;
unsigned int ADisplay::defaultPlanes=0;


ADisplay::ADisplay(char *name, unsigned int dwidth,
  unsigned int dheight, unsigned int ddepth)
{
  DEBUG_OUT<<"ADisplay("<<name<<","<<dwidth<<","<<dheight<<","<<ddepth<<")\n";
  desiredWidth=dwidth;  desiredHeight=dheight;  desiredDepth=ddepth;
  init();
  openDis(name,dwidth,dheight,ddepth,0);
  aDefaultADisplay=this;
}


ADisplay::ADisplay(unsigned int dwidth, unsigned int dheight,
  unsigned int ddepth)
{
  DEBUG_OUT<<"ADisplay("<<dwidth<<","<<dheight<<","<<ddepth<<")\n";
  bool tempFlag=false;
  desiredWidth=dwidth;  desiredHeight=dheight;  desiredDepth=ddepth;
  init();
  areDefault=tempFlag; // Because init() resets it...
  openDis((char *)"New Display",dwidth,dheight,ddepth,0);
  aDefaultADisplay=this;
}


ADisplay::ADisplay(APrivateBitmap *someImage)
{
  DEBUG_OUT<<"ADisplay(someImage)\n";
  // NOTE: be sure to note the weird placement of the call to init here...
  unsigned int dwidth, dheight, ddepth, dnplanes;
  dwidth=someImage->getWidth();  dheight=someImage->getHeight();
  ddepth=someImage->getDepth();  dnplanes=someImage->getNPlanes();
  desiredWidth=dwidth;  desiredHeight=dheight;  desiredDepth=ddepth;
  init();
  // We are asking for a display that will hold this image.
  // Who knows what we'll actually get...
  openDis((char *)"Image",dwidth,dheight,ddepth,dnplanes);
  //
  // Right here...I'm not sure if we should matchColors or verify first...
  //
  // Make sure image has the right kind of pixels to fit what we got.
  // Try to match the display to the image's colors.
  matchColors(someImage);
  /*
  if(!matchColors(&someImage))
    DEBUG_OUT.Warning("!  Couldn't match colors for some reason!\n";
  else
    DEBUG_OUT<<"Matched display colors to image.\n";
  */
  aDefaultADisplay=this;
}


ADisplay::~ADisplay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"~ADisplay()\n";
#endif // DEBUG_VERBOSE
  //if(haveFocus) aThisApp.focusDisplay=(ADisplay *)NULL;
  numDisplays--;
  unfocus();
  // Close all windows
  /*
  windows.reset();
  while(!windows.atEnd()) {
    AWindowAbstract *w=(AWindowAbstract *)windows.info();
    w->unfocus();
    windows.advance();
    //delete w;
  }
  windows.reset();
  */
  // finish cleaning up
  //aThisApp.removeDisplay(this);
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
  // FIXME: We don't do anything with ddrawWin...
  if(ddPtr) {
    // TODO: for some reason this isn't working...
    //if(changedMode) {
      DEBUG_OUT<<"Going to RestoreDisplayMode...\n";
      ddPtr->RestoreDisplayMode();
    //}
  }
  DEBUG_OUT<<"Going to release surfaces...\n";
  if(!canPageFlip) {
    if(offscreenPtr) offscreenPtr->Release();
    offscreenPtr=(LPDIRECTDRAWSURFACE)NULL;
  }
  if(surfPtr) surfPtr->Release();
  surfPtr=(LPDIRECTDRAWSURFACE)NULL;
  if(winPalette) {
    DEBUG_OUT<<"Going to release palette...\n";
    winPalette->Release();
  }
  winPalette=(LPDIRECTDRAWPALETTE)NULL;
  DEBUG_OUT<<"Releasing ddraw...\n";
  if(ddPtr) ddPtr->Release();
  // FIXME: Could free ddraw.dll if we loaded
  ddPtr=(LPDIRECTDRAW)NULL;
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32
#ifdef ASYS_DOS
  hideMouse();
  // Back to text mode (or whatever), but only if we actually changed
  if(aThisApp.hercGfx) setHercText();
  else {
    unsigned int curMode=getCurVideoMode();
    //choice=&origMode;
    if(curMode!=origMode.mode) set(choice);
  }
  if(theBits) { delete theBits; theBits=(ABitmap *)NULL; }
#endif // ASYS_DOS
#ifdef ASYS_AMIGA
  if(isWorkbench) {
    if(wbColors) { matchColors(wbColors); delete wbColors; }
    else {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"No wbColors! Sigh...\n";
#endif // DEBUG_VERBOSE
    }
    wbColors=NULL;
  }
  else { if(TheScreen) CloseScreen(TheScreen); }
  TheScreen=NULL;
#endif // ASYS_AMIGA
#ifdef ASYS_X11_BASED
  // FIXME: There is no matching XCloseDisplay...
  theDisplay=(Display *)NULL;
  theScreen=(Screen *)NULL;  theVis=(Visual *)NULL;
  theCMap=(Colormap)0;  useCMap=false;
#endif // ASYS_X11_BASED
  if(colors) { delete colors; colors=(APalette *)NULL; }
  if(choice) { delete choice; choice=(ADisplayMode *)NULL; }
  //if(name) { free(name); name=NULL; }
  if(aDefaultADisplay==this) aDefaultADisplay=NULL;
}


void ADisplay::init()
{
  // Set all data members to known values
  AFrameAbstract::init();
#ifdef ASYS_MFC
  setClassName("ADisplay");
#endif // ASYS_MFC
  x=0;  y=0;  nPlanes=0;
  width=0;  height=0;  depth=0;
  // NOTE: don't touch desiredWidth/Height/Depth...openDis set them for us
  areDefault=false;
  colors=(APalette *)NULL;
  colorLock=false;
  bitmapType=A_BITMAP_NONE;
  name=(char *)NULL;
  bytesPerLine=0;
  choice=(ADisplayMode *)NULL;
  bitsTriplet=0;
  staticColor=false;  trueColor=false;  grayscale=false;
  canSetAnyColorFlag=false;
  canLockBits=false;  canUseAnyImageFlag=false;
  avgCharWidth=0;
  haveFocus=false;
  focusWindow=(AWindowAbstract *)NULL;
  xOffset=0;  yOffset=0;
  xMargin=0;  yMargin=0;
  bitsLocked=false;
  rmask=0xff0000;  gmask=0xff00;  bmask=0xff;  // Only valid for trueColor
  rshift=16;  gshift=8;  bshift=0;  // Only valid for trueColor
  rbits=16;  gbits=8;  bbits=0;  // Only valid for trueColor
  theFont=(ABitmap *)NULL;
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
  didInitDDraw=false;
  ddrawWin=(HWND)NULL;
  ddPtr=(LPDIRECTDRAW)NULL;
  surfPtr=(LPDIRECTDRAWSURFACE)NULL;
  offscreenPtr=(LPDIRECTDRAWSURFACE)NULL;
  winPalette=(LPDIRECTDRAWPALETTE)NULL;
  canPageFlip=false;
  ddrawBits=(BYTE *)NULL;
  changedMode=false;
  numPages=0;
  /*
  dDrawHandle=LoadLibrary(ATEXT("ddraw.dll"));
  if(dDrawHandle) {
    if(!_dll_DirectDrawCreate) {
      _dll_DirectDrawCreate=(HRESULT (WINAPI *)(GUID FAR *, LPDIRECTDRAW FAR *, IUnknown FAR *))GET_PROC(ATEXT("ddraw"),"DirectDrawCreate");
      if(!_dll_DirectDrawCreate) {
        DEBUG_OUT<<"Couldn't getProc DirectDrawCreate!\n";
      }
#ifdef DEBUG_VERBOSE
      else DEBUG_OUT<<"Found ddraw.dll...\n";
#endif // DEBUG_VERBOSE
    }
  }
  */
  canLockBits=true;
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32
#ifdef ASYS_AMIGA
  isWorkbench=false;
  isHAM=false;  isHBrite=false;
  triedHB=false;  triedHIRES=false;
  wbColors=NULL;  TheScreen=NULL;
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  curMode=0;
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_DOS
  interleaved=false;
  theBits=(ABitmap *)NULL;
  origMode.mode=0;
  numRows=0; numCols=0;
  vptr=0;
  vsize=0;
  wsize=64;  wgran=64;  // These are arbitrary...
  biossupp=true;  mousesupp=true;  // assume unless told differently
  canLockBits=true;
#endif // ASYS_DOS
#ifdef ASYS_X11_BASED
  // First init to known values...
  unsigned int borderAround=5,borderTop=18;
  yOffset=borderTop+borderAround;  xOffset=borderAround;
  xMargin=borderAround;  yMargin=borderAround;
  theDisplay=(Display *)NULL;  theScreen=(Screen *)NULL;  theVis=(Visual *)NULL;
  vid=0;  theCMap=0;
  useCMap=false;
  // ...then get on with the business of setting up.
  theDisplay=XOpenDisplay((char *)NULL);
  if(!theDisplay) {
    fprintf(stderr,"Couldn't get XOpenDisplay!\n");
    DEBUG_OUT<<"Couldn't get XOpenDisplay!\n";
  }
  theScreen=DefaultScreenOfDisplay(theDisplay);
#ifdef USE_OPENGL
  int screenNum=DefaultScreen(theDisplay);
  int attrib[]={ GLX_RGBA,GLX_RED_SIZE,1,GLX_GREEN_SIZE,1,GLX_BLUE_SIZE,1,GLX_DOUBLEBUFFER,GLX_DEPTH_SIZE,1,None };
  visInfo=glXChooseVisual(theDisplay,screenNum,attrib);
  if(!visInfo) {
    DEBUG_OUT<<"Couldn't get a double buffered RGB visual!\n";
    exit(1);
  }
  theVis=visInfo->visual;
#else
  theVis=DefaultVisualOfScreen(theScreen);
#endif
  vid=theVis->c_class;
  bool sc=false,tc=false;
  switch(vid) {
    case StaticColor:
      sc=true;  tc=false;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"StaticColor visual...\n";
#endif // DEBUG_VERBOSE
      break;
    case StaticGray:
      sc=true;  tc=false;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"StaticGray visual...\n";
#endif // DEBUG_VERBOSE
      break;
    case GrayScale:
      sc=false;  tc=false;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"GrayScale visual...\n";
#endif // DEBUG_VERBOSE
      break;
    case PseudoColor:
      sc=false;  tc=false;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"PseudoColor visual...\n";
#endif // DEBUG_VERBOSE
      break;
    case TrueColor:
      sc=true;  tc=true;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"TrueColor visual...\n";
#endif // DEBUG_VERBOSE
      break;
    case DirectColor:
      // For now, we treat this just like TrueColor (i.e. static)
      sc=true;  tc=true;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"DirectColor visual...\n";
#endif // DEBUG_VERBOSE
      break;
    default:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Unknown visual ID, going to try anyway...\n";
#endif // DEBUG_VERBOSE
      break;
  }
  staticColor=sc;  trueColor=tc;
  if(vid&0x1)
    theCMap=XCreateColormap(theDisplay,DefaultRootWindow(theDisplay),theVis,AllocAll);
  useCMap=false;
  if(trueColor) {
    rmask=theVis->red_mask;
    gmask=theVis->green_mask;
    bmask=theVis->blue_mask;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"trueColor visual RGB masks are : (in dec) "<<rmask;
    DEBUG_OUT<<","<<gmask;
    DEBUG_OUT<<","<<bmask<<"\n";
    DEBUG_OUT<<"bits_per_rgb="<<theVis->bits_per_rgb<<"\n";
    if(rmask>bmask) DEBUG_OUT<<"Colors are RGB\n"; else DEBUG_OUT<<"Colors are BGR\n";
#endif // DEBUG_VERBOSE
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"X display: bmpad "<<BitmapPad(theDisplay)<<"  bmunit ";
  DEBUG_OUT<<BitmapUnit(theDisplay)<<"  bmorder "<<BitmapBitOrder(theDisplay);
  DEBUG_OUT<<"  imageorder "<<ImageByteOrder(theDisplay)<<"\n";
#endif // DEBUG_VERBOSE
#endif // ASYS_X11_BASED
  // Check our video hardware if we haven't yet.
  if(videoHardware==A_VIDEO_HARDWARE_NONE) queryHardware();
  // Build display mode list if we haven't yet...
  if(!dmList) dmList=new AList;
  if(dmList) {
    if(dmList->isEmpty()) findModes();
  }
  if(trueColor) {
    calcRGBBits();
#ifdef DEBUG_VERBOSE
    if(depth==16) DEBUG_OUT<<"(Assuming 16-bit color is 565...)\n";
#endif // DEBUG_VERBOSE
  }
}


void ADisplay::openDis(char *theName,unsigned int w, unsigned int h,
  unsigned int d, unsigned int np)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"openDis()\n";
#endif // DEBUG_VERBOSE
  name=strdup(theName);
  width=w;  height=h;  nPlanes=np;  depth=d;
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
  desiredWidth=width;  desiredHeight=height;  desiredDepth=depth;
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32
  choice=query(w,h,d,np);
  if(!choice) {
    DEBUG_OUT<<"query failed!\n";
    return;
  }
  DEBUG_OUT<<"(Done with query, going to set...)\n";
  if(!set(choice)) {
    DEBUG_OUT<<"Couldn't set display to "<<choice->mode<<"...\n";
    // need to fall back here...
    return;
  }
  sync();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Display is: "<<width<<"x"<<height<<"x"<<depth
    <<" planes "<<nPlanes<<"\n";
  DEBUG_OUT<<"Offsets are: "<<xOffset<<","<<yOffset<<"\n";
  DEBUG_OUT<<"Margins are: "<<xMargin<<","<<yMargin<<"\n";
#endif // DEBUG_VERBOSE
#ifdef ASYS_DOS
  setColor(0,0,0,0);
  //for(unsigned int t=1;t<colors->getNumColors();t++) setColor(t,0xff,0xff,0xff);
  theBits=new ABitmap(this,choice);
  sync();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"At "<<aThisApp.myHex((long)vptr)<<" for "<<vsize<<" bytes, "
    <<bytesPerLine<<" bpl\n";
  if(choice->hardware==A_VIDEO_SVGA)
    DEBUG_OUT<<"VRAM window: "<<wsize<<"k@"<<wgran<<"k\n";
#endif // DEBUG_VERBOSE
  focus();
#endif
  //aThisApp.addDisplay(this);
  numDisplays++;
  DEBUG_OUT<<"ADisplay::openDis calc...\n";
  calc();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"openDis() done.\n";
#endif // DEBUG_VERBOSE
}


// NOTE: This will only be called once...
void ADisplay::queryHardware()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Checking out your video hardware...\n";
#endif // DEBUG_VERBOSE
  videoHardware=A_VIDEO_HARDWARE_STANDARD;
#ifdef ASYS_DOS
  // Init all flags
  canLockBits=true;
  bool foundMono=false,foundColor=false;
  bool foundHerc=false,foundMonoMode=false;
  bool foundBios=false,foundVGA=false,foundVESA=false;
  unsigned int m=getEquipStatus();
  switch(m) {
    case 0x00:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Switches set to EGA/VGA.\n";
#endif // DEBUG_VERBOSE
      break;
    case 0x01:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Switches set to Color 40.\n";
#endif // DEBUG_VERBOSE
      foundColor=true;
      break;
    case 0x02:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Switches set to Color 80.\n";
#endif // DEBUG_VERBOSE
      foundColor=true;
      break;
    case 0x03:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Switches set to Mono 80.\n";
#endif // DEBUG_VERBOSE
      foundMono=true;
      break;
    default:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Unknown switch setting.\n";
#endif // DEBUG_VERBOSE
      break;
  }
  // Does the mono one look like a Hercules card?
  if(foundMono) {
    unsigned long stat1,stat2=inportb(0x3ba)&0x80,loops=32000;
    while(loops) {
      loops--; stat1=inportb(0x3ba)&0x80;
      if(stat1!=stat2) { DEBUG_OUT<<"Found Hercules registers.\n"; loops=0; foundHerc=true; }
    }
  }
  REGS16 rin,rout;
  // Is an EGA/VGA bios there?
  rin.h.ah=0x12;  rin.h.bl=0x10;  rin.h.bh=0x55;  int86(0x10,&rin,&rout);
  if(rout.h.bh!=0x55) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Found EGA/VGA bios.\n";
#endif // DEBUG_VERBOSE
    foundBios=true;
    if(rout.h.bh==1) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"EGA/VGA is in MONO mode.\n";
#endif // DEBUG_VERBOSE
      foundMonoMode=true;
    }
    unsigned int temp=rout.h.bl;
    rin.h.al=0;  rin.h.ah=0x1a;  int86(0x10,&rin,&rout);
    if(rout.h.al==0x1a) foundVGA=true;
    if(!foundVGA) { if(temp<3) foundMonoMode=true; /* Less than 256k EGA */ }
  }
  // Is there a VESA bios if we found VGA?
  // Just put this on the stack for now...
  AVesaInfo aThisVesa;
  if(foundVGA) {
    // FIXME: Fill in the sig to let the bios know we handle VESA 2.0...
    if(vesaGetInfo(&aThisVesa)) foundVESA=true;
  }
  // Now that we're done checking, figure out which card it is.
  aThisApp.videoHardware=A_VIDEO_HARDWARE_STANDARD;
  if(foundMono) aThisApp.videoHardware=A_VIDEO_MONO;
  if(foundMono&&foundHerc) aThisApp.videoHardware=A_VIDEO_HERC;
  if(foundHerc) { aThisApp.hasHerc=true; aThisApp.monoMode=true; }
  if(foundMonoMode) aThisApp.monoMode=true;
  if(foundMono) aThisApp.monoMode=true;
  if(foundColor) aThisApp.videoHardware=A_VIDEO_CGA;
  if(foundBios) aThisApp.videoHardware=A_VIDEO_EGA;
  if(foundVGA) aThisApp.videoHardware=A_VIDEO_VGA;
  if(foundVESA) aThisApp.videoHardware=A_VIDEO_SVGA;
  // FIXME: set this to something close to the truth...
  aThisApp.videoMemory=4;  // 4k absolute min
  if(getenv("HERC_ONLY")) {
    aThisApp.videoHardware=A_VIDEO_HERC;
    aThisApp.hasHerc=true;  aThisApp.monoMode=true;
    aThisApp.vesaMajor=0;  aThisApp.vesaMinor=0;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Forcing HGA mode!\n";
#endif // DEBUG_VERBOSE
  }
  if(getenv("MONO_ONLY")) {
    aThisApp.videoHardware=A_VIDEO_MONO;
    aThisApp.hasHerc=false;  aThisApp.monoMode=true;
    aThisApp.vesaMajor=0;  aThisApp.vesaMinor=0;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Forcing MDA mode!\n";
#endif // DEBUG_VERBOSE
  }
  if(getenv("CGA_ONLY")) {
    aThisApp.videoHardware=A_VIDEO_CGA;
    aThisApp.hasHerc=false;  aThisApp.monoMode=false;
    aThisApp.vesaMajor=0;  aThisApp.vesaMinor=0;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Forcing CGA mode!\n";
#endif // DEBUG_VERBOSE
  }
  if(getenv("EGA_ONLY")) {
    aThisApp.videoHardware=A_VIDEO_EGA;
    aThisApp.hasHerc=false;  aThisApp.monoMode=false;
    aThisApp.vesaMajor=0;  aThisApp.vesaMinor=0;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Forcing EGA mode!\n";
#endif // DEBUG_VERBOSE
  }
  if(getenv("VGA_ONLY")) {
    aThisApp.videoHardware=A_VIDEO_VGA;
    aThisApp.hasHerc=false;  aThisApp.monoMode=false;
    aThisApp.vesaMajor=0;  aThisApp.vesaMinor=0;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Forcing VGA mode!\n";
#endif // DEBUG_VERBOSE
  }
#ifdef DEBUG_VERBOSE
  // Print what we think it is.
  char *string=(char *)NULL;
  switch(aThisApp.videoHardware) {
    case A_VIDEO_MONO: DEBUG_OUT<<"Card is a MDA.\n"; break;
    case A_VIDEO_HERC: DEBUG_OUT<<"Card is a HGA.\n"; break;
    case A_VIDEO_CGA: DEBUG_OUT<<"Card is a CGA.\n"; break;
    case A_VIDEO_EGA: DEBUG_OUT<<"Card is an EGA.\n"; break;
    case A_VIDEO_VGA: DEBUG_OUT<<"Card is a VGA.\n"; break;
    case A_VIDEO_SVGA: DEBUG_OUT<<"Card is an SVGA (VBE ";
      aThisApp.vesaMajor=(aThisVesa.version>>8)&0xff;
      aThisApp.vesaMinor=aThisVesa.version&0xff;
      aThisApp.videoMemory=aThisVesa.memory*64;
      DEBUG_OUT<<aThisApp.vesaMajor<<"."<<aThisApp.vesaMinor<<").\n";
      string=(char *)MK_FP(aThisVesa.string_seg,aThisVesa.string_off);
      DEBUG_OUT<<string<<"\n";
      DEBUG_OUT<<"Video Memory:"<<aThisApp.videoMemory<<"k\n";
      break;
    default: DEBUG_OUT<<"Couldn't figure out video card!\n"; break;
  }
  //DEBUG_OUT<<"monoMode is "<<aThisApp.monoMode<<"\n";
#endif // DEBUG_VERBOSE
#endif // ASYS_DOS
#ifdef ASYS_AMIGA
  aThisApp.videoHardware=A_VIDEO_OCS;
  if(aThisApp.hasAGA) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Amiga AGA hardware found.\n";
#endif // DEBUG_VERBOSE
    aThisApp.videoHardware=A_VIDEO_AGA;
  }
  else {
    if(aThisApp.hasECS) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Amiga ECS hardware found.\n";
#endif // DEBUG_VERBOSE
      aThisApp.videoHardware=A_VIDEO_ECS;
    }
  }
  if(aThisApp.videoHardware==A_VIDEO_OCS)  {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Amiga OCS hardware found.\n";
#endif // DEBUG_VERBOSE
  }
#endif // ASYS_AMIGA
}


// NOTE: This will only be called once...
void ADisplay::findModes()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Building list of availiable display modes.\n";
#endif // DEBUG_VERBOSE
  if(!dmList) {
    DEBUG_OUT<<"ERROR! App doesn't have display list!\n";
    return;
  }
  ADisplayMode *dm=(ADisplayMode *)NULL;
#ifdef ASYS_AMIGA
  // If we've got at least 2.0 query the display info database...
  // FIXME: Use new 3.x features of display database
  if(aThisApp.version>35) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Searching the display mode database...\n";
#endif // DEBUG_VERBOSE
    ULONG searchid=INVALID_ID;
    searchid=INVALID_ID;
    bool searching=true;
    DisplayInfoHandle dih;  MonitorInfo mi;
    NameInfo ni;  DisplayInfo di;  DimensionInfo dmi;
    DEBUG_OUT<<"(Going to call NextDisplayInfo...)\n";
    searchid=NextDisplayInfo(searchid);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Building display mode list...\n";
#endif // DEBUG_VERBOSE
    while(searching) {
      dih=FindDisplayInfo(searchid);
      bool mf,nf,dif,dmf;
      mf=GetDisplayInfoData(dih,(UBYTE *)&mi,sizeof(MonitorInfo),
        DTAG_MNTR,(unsigned int)NULL);
      dmf=GetDisplayInfoData(dih,(UBYTE *)&dmi,sizeof(DimensionInfo),
        DTAG_DIMS,(unsigned int)NULL);
      nf=GetDisplayInfoData(dih,(UBYTE *)&ni,sizeof(NameInfo),
        DTAG_NAME,(unsigned int)NULL);
      dif=GetDisplayInfoData(dih,(UBYTE *)&di,sizeof(DisplayInfo),
        DTAG_DISP,(unsigned int)NULL);
      if(mf&&dmf&&dif) {
        if((!di.NotAvailable)&&(!(di.PropertyFlags&DIPF_IS_DUALPF))) {
          dm=new ADisplayMode;
          dm->mode=searchid;
          ULONG ww=dmi.Nominal.MaxX-dmi.Nominal.MinX+1;
          ULONG hh=dmi.Nominal.MaxY-dmi.Nominal.MinY+1;
          dm->width=ww;  dm->height=hh;
          dm->bitmapType=A_BITMAP_PLANES;
          dm->flags=A_DISPLAYMODE_FLAG_LESS;
          dm->depth=dmi.MaxDepth; dm->rdepth=dmi.MaxDepth;
          dm->nPlanes=dmi.MaxDepth;
          if(di.PropertyFlags&DIPF_IS_HAM) {
            if(dm->nPlanes==6) dm->depth=12; else dm->depth=24;
            dm->flags=0;
          }
          if(di.PropertyFlags&DIPF_IS_EXTRAHALFBRITE) {
            dm->depth=6;  dm->rdepth=6;  dm->nPlanes=6;  dm->flags=0;
          }
          dm->bytesPerLine=ww/8;
          dm->vMemPtr=0;  dm->vMemSize=0;
          dm->hardware=0;
          if(di.PropertyFlags&DIPF_IS_ECS) dm->hardware|=A_VIDEO_ECS;
          if(di.PropertyFlags&DIPF_IS_AA) dm->hardware|=A_VIDEO_AGA;
          if(di.PropertyFlags&DIPF_IS_FOREIGN) dm->hardware|=A_VIDEO_ALIEN;
#ifdef DEBUG_VERBOSE
          DEBUG_OUT<<"+"<<aThisApp.myHex(dm->mode)<<"h: "<<dm->width;
          DEBUG_OUT<<"x"<<dm->height<<"x"<<dm->depth<<"("<<dm->nPlanes<<")";
#endif // DEBUG_VERBOSE
          switch(di.PaletteRange) {
            case 64:   dm->bitsTriplet=2;  break;
            case 512: dm->bitsTriplet=3;  break;
            case 4096: dm->bitsTriplet=4;  break;
            case 65535: dm->bitsTriplet=5;  break;
            default:
#ifdef DEBUG_VERBOSE
              DEBUG_OUT<<" (PaletteRange is:"<<di.PaletteRange<<")";
#endif // DEBUG_VERBOSE
              dm->bitsTriplet=8;  // Assume this...
              break;
          }
#ifdef DEBUG_VERBOSE
          DEBUG_OUT<<"\n";
#endif // DEBUG_VERBOSE
          aThisApp.dmList->insert((void *)dm);
        } //...NotAvail...
      } // if(mf&&dmf...
      searchid=NextDisplayInfo(searchid);
      if(searchid==INVALID_ID) searching=false;
    } // while
  }
  // If we don't have 2.0, add standard OCS modes...
  if(aThisApp.version<36) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Adding standard OCS modes...\n";
#endif // DEBUG_VERBOSE
    unsigned int t=0;
    while(aSomeModes[t].width) {
      dm=new ADisplayMode;
      dm->mode=aSomeModes[t].mode;
      dm->width=aSomeModes[t].width;  dm->height=aSomeModes[t].height;
      dm->depth=aSomeModes[t].depth; dm->rdepth=aSomeModes[t].rdepth;
      dm->nPlanes=aSomeModes[t].nPlanes; dm->vMemPtr=0;  dm->vMemSize=0;
      dm->hardware=A_VIDEO_OCS;  dm->bitmapType=A_BITMAP_PLANES;
      dm->bitsTriplet=aSomeModes[t].bitsTriplet;
      dm->flags=aSomeModes[t].flags;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"+"<<aThisApp.myHex(dm->mode)<<"h: "<<dm->width<<"x";
      DEBUG_OUT<<dm->height<<"x"<<dm->depth<<"("<<dm->nPlanes<<")\n";
#endif // DEBUG_VERBOSE
      aThisApp.dmList->insert((void *)dm);
      t++;
    }
  }
  // Add default screen (usually workbench)...
  struct Screen *pub=NULL;
  unsigned int pw,ph,pd;
  if(aThisApp.version>35) {
    pub=LockPubScreen(NULL);
    if(pub) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Adding default public screen...\n";
#endif // DEBUG_VERBOSE
      pw=pub->Width; ph=pub->Height; pd=pub->RastPort.BitMap->Depth;
      UnlockPubScreen(NULL,pub);
    }
  }
  else {
    pub=(Screen *)OpenWorkBench();
    if(pub) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Adding workbench screen...\n";
#endif // DEBUG_VERBOSE
      pw=pub->Width; ph=pub->Height; pd=pub->RastPort.BitMap->Depth;
    }
  }
  if(pub) {
    dm=new ADisplayMode;
    dm->mode=0;
    dm->width=pw;  dm->height=ph;
    dm->depth=pd;  dm->rdepth=pd;  dm->nPlanes=pd;
    dm->vMemPtr=0;  dm->vMemSize=0;
    dm->hardware=A_VIDEO_HARDWARE_STANDARD;
    dm->bitsTriplet=4;
    if(aThisApp.videoHardware>=A_VIDEO_AGA) dm->bitsTriplet=8;
    dm->flags=A_DISPLAYMODE_FLAG_NONE;  dm->bitmapType=A_BITMAP_PLANES;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"+"<<aThisApp.myHex(dm->mode)<<"h: "<<dm->width<<"x"<<dm->height<<"x";
    DEBUG_OUT<<dm->depth<<"("<<dm->nPlanes<<")\n";
#endif // DEBUG_VERBOSE
    aThisApp.dmList->insert((void *)dm);
  }
  DEBUG_OUT<<"(Done adding modes)\n";
  return;
#endif // ASYS_AMIGA
#ifdef ASYS_DOS
  // Add modes for VGA or lower cards...
  if(aThisApp.videoHardware==A_VIDEO_MONO) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Adding Mono text mode...\n";
#endif // DEBUG_VERBOSE
    dm=new ADisplayMode;
    dm->mode=0x07;
    dm->width=80;  dm->height=25;  dm->depth=1;  dm->rdepth=1;  dm->nPlanes=1;
    dm->bytesPerLine=160;  dm->vMemPtr=0xb0000000L;  dm->vMemSize=4096;
    dm->hardware=A_VIDEO_MONO;
    dm->bitsTriplet=0;  dm->bitmapType=A_BITMAP_CHUNKY;  dm->flags=A_FLAG_TEXT;
    aThisApp.dmList->insert((void *)dm);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"+"<<aThisApp.myHex(dm->mode)<<"h: "<<dm->width<<"x"<<dm->height<<"x";
    DEBUG_OUT<<dm->depth<<"("<<dm->nPlanes<<")\n";
#endif // DEBUG_VERBOSE
  }
  if(aThisApp.hasHerc) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Adding Hercules graphic mode...\n";
#endif // DEBUG_VERBOSE
    dm=new ADisplayMode;
    dm->mode=0x07;
    dm->width=720;  dm->height=348;  dm->depth=1;  dm->rdepth=1;  dm->nPlanes=1;
    dm->bytesPerLine=90;  dm->vMemPtr=0xb0000000L;  dm->vMemSize=32768L;
    dm->hardware=A_VIDEO_HERC;
    dm->bitsTriplet=0;  dm->bitmapType=A_BITMAP_PLANES;  dm->flags=A_FLAG_INTER;
    aThisApp.dmList->insert((void *)dm);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"+"<<aThisApp.myHex(dm->mode)<<"h: "<<dm->width<<"x"<<dm->height<<"x";
    DEBUG_OUT<<dm->depth<<"("<<dm->nPlanes<<")\n";
#endif // DEBUG_VERBOSE
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Adding modes for CGA thru VGA cards...\n";
#endif // DEBUG_VERBOSE
  unsigned int t=0;
  while(aSomeModes[t].width) {
    dm=new ADisplayMode;
    dm->mode=aSomeModes[t].mode;
    dm->width=aSomeModes[t].width;  dm->height=aSomeModes[t].height;
    dm->depth=aSomeModes[t].depth; dm->rdepth=aSomeModes[t].rdepth;
    dm->nPlanes=aSomeModes[t].nPlanes;
    dm->bytesPerLine=aSomeModes[t].bytesPerLine;
    dm->vMemPtr=aSomeModes[t].vMemPtr;  dm->vMemSize=aSomeModes[t].vMemSize;
    dm->hardware=aSomeModes[t].hardware;
    dm->bitsTriplet=aSomeModes[t].bitsTriplet;
    if((dm->bitsTriplet==2)&&(dm->hardware==A_VIDEO_EGA)&&
      (videoHardware>A_VIDEO_EGA)) dm->bitsTriplet=6;
    dm->bitmapType=aSomeModes[t].bitmapType;
    dm->flags=aSomeModes[t].flags;
    if((!aThisApp.monoMode)&&(dm->depth==1)&&(videoHardware==A_VIDEO_EGA))
      dm->hardware=A_DISPLAYMODE_FLAG_TOO_MUCH;
    if((aThisApp.monoMode)&&(dm->depth>1)) dm->hardware=A_DISPLAYMODE_FLAG_TOO_MUCH;
    if((aThisApp.monoMode)&&(dm->height<350)) dm->hardware=A_DISPLAYMODE_FLAG_TOO_MUCH;
    if(dm->hardware<=videoHardware) {
      dmList->insert((void *)dm);
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"+"<<aThisApp.myHex(dm->mode)<<"h: "<<dm->width<<"x";
      DEBUG_OUT<<dm->height<<"x"<<dm->depth<<"("<<dm->nPlanes<<")\n";
#endif // DEBUG_VERBOSE
    }
    t++;
  }
  // Add vesa modes, if present...
  if(aThisApp.vesaMajor) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Adding VESA SVGA modes...\n";
#endif // DEBUG_VERBOSE
    char p[256];
    char *ptr=(char *)&p[0];
    AVesaModeInfo *vi=(AVesaModeInfo *)ptr;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"ptr is at "<<aThisApp.myHex((long)ptr)<<"\n";
#endif // DEBUG_VERBOSE
    unsigned int tmode;
    AVesaInfo aThisVesa;
    vesaGetInfo(&aThisVesa);
    unsigned short *modes=(unsigned short *)MK_FP(aThisVesa.mode_seg,aThisVesa.mode_off);
    while(*modes!=0xffff) {
      tmode=*modes;
      bool bret=vesaGetModeInfo(*modes,vi);
      modes=(unsigned short *)MK_FP(aThisVesa.mode_seg,aThisVesa.mode_off);
      aThisVesa.mode_off+=2;
      if(bret) {
        // FIXME: Check for error here!
        //DEBUG_OUT<<"Queried the card.\n";
        // FIXME: Only use the optional part of vi if its present,
        // (otherwise look it up from our table)
        // set biossupp as appropriate
        // Now we only care about the hardware supported graphics modes
        //DEBUG_OUT<<"*"<<(long)tmode<<": "<<(long)vi->xres<<"x"<<(long)vi->yres<<"x"
        //  <<(int)vi->bitsperpixel<<"("<<(int)vi->planecount<<")\n";
        if((vi->attrib&0x11)&&(vi->xres>300)&&(vi->yres>100)) {
          dm=new ADisplayMode;
          dm->mode=tmode;
          dm->width=vi->xres;  dm->height=vi->yres;
          dm->depth=vi->bitsperpixel; dm->rdepth=vi->bitsperpixel;
          dm->nPlanes=vi->planecount; dm->bytesPerLine=vi->bytesperline;
          // FIXME: Don't assume this!  (linear buffers?!?)
          dm->vMemPtr=0xa0000000L;
          dm->hardware=A_VIDEO_SVGA;
          if((aThisApp.monoMode)&&(dm->depth>1)) dm->hardware=A_DISPLAYMODE_FLAG_TOO_MUCH;
          // FIXME: use the mask and field settings if present in vi
          dm->bitsTriplet=8;
          ULONG tval=((ULONG )vi->xres)*((ULONG )vi->yres);
          if(vi->planecount==1) {
            dm->bitmapType=A_BITMAP_CHUNKY;  dm->flags=0;
            unsigned long td=(unsigned long)vi->bitsperpixel;
            if(td==15) td=16; // because it really takes up two full bytes
            dm->vMemSize=(tval/8L)*td;
          }
          else {
            dm->bitmapType=A_BITMAP_PLANES;  dm->flags=A_DISPLAYMODE_FLAG_LESS;
            dm->vMemSize=(tval*vi->planecount)/8L;
          }
          if(dm->hardware<=aThisApp.videoHardware) {
#ifdef DEBUG_VERBOSE
            DEBUG_OUT<<"+"<<aThisApp.myHex(tmode)<<"h: "<<(int)vi->xres<<"x";
            DEBUG_OUT<<(int)vi->yres<<"x"<<(int)vi->bitsperpixel<<"(";
            DEBUG_OUT<<(int)vi->planecount<<")"<<" mem: "<<(dm->vMemSize/1024);
            DEBUG_OUT<<"k: "<<(int)vi->size<<"k@"<<(int)vi->gran<<"k\n";
#endif // DEBUG_VERBOSE
            dmList->insert((void *)dm);
          }
        }
        else {
#ifdef DEBUG_VERBOSE
          DEBUG_OUT<<"-"<<aThisApp.myHex(tmode)<<"h: "<<(int)vi->xres<<"x";
          DEBUG_OUT<<(int)vi->yres<<"x"<<(int)vi->bitsperpixel<<"(";
          DEBUG_OUT<<(int)vi->planecount<<")"<<" mem: "<<dm->vMemSize<<": ";
          DEBUG_OUT<<(int)vi->size<<"k@"<<(int)vi->gran<<"k\n";
#endif // DEBUG_VERBOSE
        }
      }
    }
  }
  return;
#endif // ASYS_DOS
#ifdef ASYS_X11_BASED
  // Add current mode...
  sync();
  dm=new ADisplayMode;
  dm->mode=1;
  dm->width=width;  dm->height=height;
  dm->depth=depth;  dm->rdepth=depth;  dm->nPlanes=nPlanes;
  dm->vMemPtr=0;  dm->vMemSize=0;
  dm->hardware=0;  dm->flags=0;
  dm->bitmapType=A_BITMAP_PLANES;
  if(theVis->c_class&0x1) dm->bitsTriplet=theVis->bits_per_rgb;
  else dm->bitsTriplet=8;
  dmList->insert((void *)dm);
  return;
#endif // ASYS_X11_BASED
#ifdef ASYS_WINDOWS
  // Add current mode...
#ifdef USE_DIRECTDRAW
  // FIXME: get real mode list and build it here...
#endif // USE_DIRECTDRAW
  sync();
  dm=new ADisplayMode;
  dm->mode=1;
  dm->width=width;  dm->height=height;
  dm->depth=depth;  dm->rdepth=depth;  dm->nPlanes=nPlanes;
  dm->vMemPtr=0;  dm->vMemSize=0;
  dm->hardware=0;  dm->flags=A_DISPLAYMODE_FLAG_NONE;
  dm->bitmapType=bitmapType;
  dm->bitsTriplet=8;  // not necessarily true, but close enough
  dmList->insert((void *)dm);
  return;
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  // Add current mode...
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"About to sync...\n";
#endif // DEBUG_VERBOSE
  sync();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"sync'd.\n";
#endif // DEBUG_VERBOSE
  dm=new ADisplayMode;
  dm->mode=1;
  dm->width=width;  dm->height=height;
  dm->depth=depth;  dm->rdepth=depth;  dm->nPlanes=nPlanes;
  dm->vMemPtr=0;  dm->vMemSize=0;
  dm->hardware=0;  dm->flags=0;
  dm->bitmapType=bitmapType;
  dm->bitsTriplet=8;  // not necessarily true, but close enough
  dmList->insert((void *)dm);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Done adding default mode.\n";
#endif // DEBUG_VERBOSE
  return;
#endif // ASYS_OS2
#ifdef ASYS_MAC
  // Add current mode...
  sync();
  dm=new ADisplayMode;
  dm->mode=1;
  dm->width=width;  dm->height=height;
  dm->depth=depth;  dm->rdepth=depth;  dm->nPlanes=nPlanes;
  dm->vMemPtr=0;  dm->vMemSize=0;
  dm->hardware=0;  dm->flags=A_DISPLAYMODE_FLAG_DEFAULT;
  dm->bitmapType=bitmapType;
  dm->bitsTriplet=16;
  if(depth==1) dm->bitsTriplet=0;
  dmList->insert((void *)dm);
  /*
  // FIXME: use GDevice and old Color QuickDraw to get video modes here
  for(int n=0;n<33;n++) {
    if(HasDepth(n)) {
      DEBUG_OUT<<"has depth="<<n<<"\n";
      dm=new ADisplayMode;
      dm->mode=1;
      dm->width=width;  dm->height=height;
      dm->depth=n;  dm->rdepth=n;  dm->nPlanes=nPlanes;
      dm->vMemPtr=0;  dm->vMemSize=0;
      dm->hardware=0;  dm->flags=A_DISPLAYMODE_FLAG_NONE;
      dm->bitmapType=bitmapType;
      dm->bitsTriplet=16;
      if(depth==1) dm->bitsTriplet=0;
      aThisApp.dmList->insert((void *)dm);
    }
  }
  */
  // FIXME: Use display manager if present to add possible modes...
  return;
#endif // ASYS_MAC
#ifdef ASYS_PALM
  sync();
  dm=new ADisplayMode;
  dm->mode=1;
  dm->width=width;  dm->height=height;
  dm->depth=depth;  dm->rdepth=depth;  dm->nPlanes=nPlanes;
  dm->vMemPtr=0;  dm->vMemSize=0;
  dm->hardware=0;  dm->flags=A_DISPLAYMODE_FLAG_NONE;
  dm->bitmapType=bitmapType;
  dm->bitsTriplet=16;
  if(depth==1) dm->bitsTriplet=0;
  dmList->insert((void *)dm);
  return;
#endif // ASYS_PALM
  // Add current mode...
  sync();
  dm=new ADisplayMode;
  dm->mode=1;
  dm->width=640;  dm->height=200;
  dm->depth=1;  dm->rdepth=1;  dm->nPlanes=1;
  dm->vMemPtr=0;  dm->vMemSize=0;
  dm->hardware=0;  dm->flags=0;
  dm->bitmapType=bitmapType;
  dm->bitsTriplet=1;  // not necessarily true, but close enough
  dmList->insert((void *)dm);
}


// NOTE: if there is a "default mode", it should be first (i.e. last) in the list
ADisplayMode *ADisplay::query(unsigned int w, unsigned int h, unsigned int d,
  unsigned int np)
{
#ifdef ASYS_DOS
  // TODO: Should fill in "Don't Care" fields (so we don't just pick a text mode)
  if(!w) w=320;
  if(!h) h=200;
  if(!d) d=8;
  if(!np) np=1;
#endif // ASYS_DOS
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
  if(!w) w=640;
  if(!h) h=480;
  if(!d) d=8;
  if(!np) np=1;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Display Query: "<<w<<"x"<<h<<"x"<<d<<" ("<<np<<")\n";
#endif // DEBUG_VERBOSE
  AList *l=dmList;
  if(!l) findModes();
  if(!l) { DEBUG_OUT<<"No list!\n"; return (ADisplayMode *)NULL; }
  if(l->isEmpty()) {
    DEBUG_OUT<<"List is empty!\n"; return (ADisplayMode *)NULL;
  }
  ADisplayMode *cur=(ADisplayMode *)NULL;
  ADisplayMode *pick=(ADisplayMode *)NULL;
  long tdepth=0,dist=0;
  long best_tdepth=0,best_dist=0;
  // "Prime the pump..."
  l->reset();  cur=(ADisplayMode *)l->info();  pick=cur;
  best_dist=labs((long)(w-cur->width))+labs((long)(h-cur->height));
  best_tdepth=labs((int)(d-cur->depth));
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<best_dist<<","<<best_tdepth;
  DEBUG_OUT<<" ("<<cur->width<<"x"<<cur->height<<"x"<<cur->depth<<")\n";
  DEBUG_OUT<<"(For starters...)\n";
#endif // DEBUG_VERBOSE
  l->advance();
  // Look at all the others
  for(;!l->atEnd();l->advance()) {
    cur=(ADisplayMode *)l->info();
    dist=labs((long)(w-cur->width))+labs((long)(h-cur->height));
    tdepth=labs((int)(d-cur->depth));
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<dist<<","<<tdepth;
    DEBUG_OUT<<" ("<<cur->width<<"x"<<cur->height<<"x"<<cur->depth<<")\n";
#endif // DEBUG_VERBOSE
    if(tdepth<=best_tdepth) {
      if(dist<=best_dist) {
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"(Hmm...looks better)\n";
#endif // DEBUG_VERBOSE
        best_dist=dist;  best_tdepth=tdepth; pick=cur;
      }
    }
  }
  DEBUG_OUT<<"(Done going through list of displays)\n";
  l->reset();
  ADisplayMode *wnode=(ADisplayMode *)l->info();
  // client passed all zeros, give them first mode...
  if(!w&&!h&&!d&&!np) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Picking first mode.\n";
#endif // DEBUG_VERBOSE
    cur=wnode;  pick=cur;
  }
  // Since the default mode will be the first (or is it last?) in the list
  if(wnode->flags&A_DISPLAYMODE_FLAG_DEFAULT) {
    unsigned long wwidth=wnode->width,wheight=wnode->height,wdepth=wnode->depth;
    // give them the default mode, its "good enough"
    if((wdepth>=d)&&(wheight>=h)&&(wwidth>=w)) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Picking default mode.\n";
#endif // DEBUG_VERBOSE
      cur=wnode;  pick=cur;
    }
  }
#ifdef ASYS_AMIGA
  // Ask, but don't use 3.x's BestModeID...
  // FIXME: Why don't we use it?
  DEBUG_OUT<<"Going to call BestModeID...\n";
  if(aThisApp.version>38) {
    ULONG bestid=BestModeID(BIDTAG_DesiredWidth,w,BIDTAG_DesiredHeight,
     //height,BIDTAG_Depth,depth,BIDTAG_DIPFMustNotHave,DIPF_IS_DUALPF,TAG_END);
      h,BIDTAG_Depth,d,TAG_END);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"BestModeID suggests "<<aThisApp.myHex(bestid);
    DEBUG_OUT<<" (But we don't trust it)\n";
#endif // DEBUG_VERBOSE
  }
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
  // This will one day be used to update the Images when the user
  // changes the monitor's number of colors, etc.
  // Should update pick to fit current display as returned by sync...
  //pick->mode=curMode;  // 2,4,16,256,thousands or millions, etc.
#endif // ASYS_MAC
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"query found mode "<<pick->mode<<": ";
  DEBUG_OUT<<pick->width<<"x"<<pick->height<<"x"<<pick->depth;
  DEBUG_OUT<<" ("<<pick->nPlanes<<")";
  if(pick->flags&A_DISPLAYMODE_FLAG_LESS) DEBUG_OUT<<" (or lower depth)";
  DEBUG_OUT<<"\n";
#endif // DEBUG_VERBOSE
  return pick;
}


bool ADisplay::set(ADisplayMode *tryMode)
{
  bool ret=true;
  if(!tryMode) {
    DEBUG_OUT<<"set NULL tryMode!\n";
  }
  DEBUG_OUT<<"ADisplay::set()...\n";
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
  initDirectDraw(tryMode);
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32
#ifdef ASYS_DOS
  // Because sometimes tryMode IS origMode!!!
  unsigned long tMode=tryMode->mode;
  DEBUG_OUT<<"tMode is "<<tMode<<"\n";
  // Read current mode...
  unsigned long oMode=getCurVideoMode();
  DEBUG_OUT<<"oMode is "<<oMode<<"\n";
  origMode.mode=oMode;
  if(origMode.mode==tMode) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Useless set from "<<aThisApp.myHex(origMode.mode)<<" to "<<aThisApp.myHex(tMode)<<"\n";
#endif // DEBUG_VERBOSE
    return true;  // Already set
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ADisplay::set from "<<aThisApp.myHex(origMode.mode)<<" to "<<aThisApp.myHex(tMode)<<"\n";
  DEBUG_OUT<<"tMode is "<<tMode<<"\n";
  DEBUG_OUT<<"oMode is "<<oMode<<"\n";
#endif // DEBUG_VERBOSE
  hideMouse();
  if(tryMode->hardware==A_VIDEO_HERC) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Setting Hercules Gfx mode...\n";
#endif // DEBUG_VERBOSE
    setHercGfx();
    biossupp=false;  mousesupp=false;
    ret=true;  // Because the bios would ALWAYS FAIL.
  }
  else setVideoMode(tryMode);
  if(theBits) {
    theBits->clear();
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"(cleared screen)\n";
#endif // DEBUG_VERBOSE
  }
  showMouse();
  aThisApp.setMode=true;
#endif // ASYS_DOS
#ifdef ASYS_AMIGA
  wbColors=NULL;
  unsigned long errorCode=0;
  DEBUG_OUT<<"Going to check trymode flags...\n";
  if(tryMode->flags&A_DISPLAYMODE_FLAG_NONE) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"(Using default public screen)\n";
#endif // DEBUG_VERBOSE
    isWorkbench=true;
    if(aThisApp.version>35) {
      TheScreen=LockPubScreen(NULL);  UnlockPubScreen(NULL,TheScreen);
    }
    else {
      TheScreen=(Screen *)OpenWorkBench();
    }
    sync();
    wbColors=new APalette(1<<nPlanes);
    if(!wbColors) { DEBUG_OUT<<"Didn't get palette!\n"; ret=false; }
    else {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Copying pub screen's palette...\n";
#endif // DEBUG_VERBOSE
      for(unsigned int t=0;t<wbColors->getNumColors();t++) {
        // FIXME: Need to use GetRGB32 if we can here...
        unsigned long l=GetRGB4(TheScreen->ViewPort.ColorMap,t);
        unsigned int r,g,b;
        r=(l>>8)&0x0000000f; g=(l>>4)&0x0000000f;
        b=l&0x0000000f;
        wbColors->setColor(t,r<<4,g<<4,b<<4);
      }
    }
  }
  else {
    DEBUG_OUT<<"(Not going to use WorkBench)\n";
    isWorkbench=false;
    unsigned int actualPlanes=tryMode->nPlanes;
    if(aThisApp.version>35) {
      Rectangle clipRect;
      clipRect.MinX=0; clipRect.MinY=0;
      clipRect.MaxX=tryMode->width; clipRect.MaxY=tryMode->height;
      UWORD twidth=width,theight=height;
      if(twidth<tryMode->width) twidth=tryMode->width;
      if(theight<tryMode->height) theight=tryMode->height;
      UWORD pens[]={~0};
      TheScreen=OpenScreenTags(NULL,SA_Pens,(ULONG)pens,
        SA_Depth,actualPlanes,SA_Title,(ULONG)name,SA_Width,twidth,
        SA_Height,theight,SA_SysFont,1,SA_AutoScroll,true,
        SA_DisplayID,tryMode->mode,SA_ErrorCode,(ULONG)(&errorCode),
        SA_DClip,(ULONG)(&clipRect),TAG_DONE);
    }
    else {
      struct NewScreen ns;
      ns.LeftEdge=0; ns.TopEdge=0; ns.Width=tryMode->width; ns.Height=tryMode->height;
      ns.Depth=actualPlanes; ns.DetailPen=1;  ns.BlockPen=2;
      ns.ViewModes=tryMode->mode; ns.Type=CUSTOMSCREEN;
      ns.Font=NULL; ns.Gadgets=NULL; ns.CustomBitMap=NULL;
      TheScreen=OpenScreen(&ns);
    }
  }
  if(!TheScreen) {
    DEBUG_OUT<<"Amiga screen failed to open! (";
    switch(errorCode) {
      case 1:  DEBUG_OUT<<"No Monitor"; break; case 2:  DEBUG_OUT<<"No Chips"; break;
      case 3:  DEBUG_OUT<<"No Memory"; break; case 4:  DEBUG_OUT<<"No Chip Memory"; break;
      case 6:  DEBUG_OUT<<"Bad Mode"; break; case 7:  DEBUG_OUT<<"Too Deep"; break;
      default: DEBUG_OUT<<"Don't know why"; break;
    }
    DEBUG_OUT<<")\n";
    ret=false;
  }
  if(TheScreen) {
    DEBUG_OUT<<"Got TheScreen...\n";
    // NOTE: This isn't right, this looks at tryMode and not what we actually got
    if(tryMode->mode&EXTRAHALFBRITE_KEY) isHBrite=true;
    if(tryMode->mode&HAM_KEY) isHAM=true;
  }
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
  // FIXME:
  // Check if choice and current mode are the same...
  // If not, use Display Manager to change mode if user says yes when we ask.
  // If we changed the mode then mark that its safe for us to muck with the palette...
#endif  // ASYS_MAC
  if(ret) choice=tryMode; else { choice=(ADisplayMode *)NULL; }
  DEBUG_OUT<<"(set is done)\n";
  return ret;
}


bool ADisplay::matchColors()
{
  return matchColors(colors);
}


bool ADisplay::matchColors(APrivateBitmap *im)
{
  if(!im) return false;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ADisplay::matchColors(ABitmap)...\n";
#endif // DEBUG_VERBOSE
  bool ret=im->verifyOkay(this);
  if(!ret) {
    DEBUG_OUT<<"Couldn't verify so can't matchColors either...\n";
    return false;
  }
  return matchColors(im->getColors());
}


bool ADisplay::matchColors(APalette *imcolors)
{
  if(!imcolors) return false;
  if(trueColor) return true;
  if(colors) {
    if(colors->closeEnoughTo(imcolors)) return true;
  }
  if(staticColor) {
    DEBUG_OUT<<"matchColors: static color!\n";
    return false;
  }
  // NOTE: If colors==NULL (True Color) we always match
  // FIXME: We can return false if for some reason we can't match
  if(!colors) { DEBUG_OUT<<"matchColors: no colors!\n";  return false; }
  if(imcolors) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"ADisplay::matchColors(APalette)...\n";
#endif // DEBUG_VERBOSE
    unsigned int t,num=colors->getNumColors();
    if(imcolors->getNumColors()<num) num=imcolors->getNumColors();
    colorLock=true;
    for(t=0;t<num;t++) setColor(t,imcolors->red(t),imcolors->green(t),imcolors->blue(t));
    colorLock=false;
#ifdef ASYS_X11_BASED
    // If visual has dynamic color...
    if(!staticColor) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Dynamic color...\n";
#endif // DEBUG_VERBOSE
      XColor *cell=(XColor *)malloc(colors->getNumColors()*sizeof(XColor));
      if(!cell) {
        DEBUG_OUT<<"Couldn't alloc XColor array!\n";
        colorLock=false;  return false;
      }
      int factor=0;
      for(t=0;t<num;t++) {
        cell[t].flags=DoRed|DoGreen|DoBlue;  cell[t].pixel=t;
        cell[t].red=imcolors->red(t)<<8;
        cell[t].green=imcolors->green(t)<<8;
        cell[t].blue=imcolors->blue(t)<<8;
      }
      XStoreColors(theDisplay,theCMap,cell,colors->getNumColors());
      useCMap=true;
    }
#endif
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
    if(!staticColor) {
      if(ddPtr) {
        unsigned int palFlags=DDPCAPS_1BIT;
        unsigned int closestColors=2;
        if(num>2) { closestColors=4; }
        if(num>4) { closestColors=16; }
        if(num>16) { closestColors=256; }
        if(num>256) {
          DEBUG_OUT.warning("matchColors:DirectDraw with more than 256 indexed colors!\n");
        }
        PALETTEENTRY *cols=(PALETTEENTRY *)malloc(
          closestColors*sizeof(PALETTEENTRY));
        for(t=0;t<closestColors;t++) {
          cols[t].peRed=imcolors->red(t);
          cols[t].peGreen=imcolors->green(t);
          cols[t].peBlue=imcolors->blue(t);
          cols[t].peFlags=PC_RESERVED;
        }
        HRESULT hret=DD_OK;
        if(winPalette) winPalette->SetEntries(0,0,closestColors,cols);
        else DEBUG_OUT<<"matchColors, no winPalette!\n";
        if(surfPtr) hret=surfPtr->SetPalette(winPalette);
        else DEBUG_OUT<<"matchColors, no surfPtr.\n";
        free(cols);
        cols=NULL;
      }
    }
#endif  // USE_DIRECTDRAW
#endif  // ASYS_WIN32
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Colors matched fine.\n";
#endif // DEBUG_VERBOSE
  return true;
}


bool ADisplay::setColor(unsigned int colorNum, unsigned long l)
{
  unsigned int r,g,b;
  r=(unsigned int)((l>>16)&0xff);
  g=(unsigned int)((l>>8)&0xff);
  b=(unsigned int)(l&0xff);
  return setColor(colorNum,r,g,b);
}


bool ADisplay::setColor(unsigned int index,AColor *aColor)
{
  if(!aColor) return false;
  return setColor(index,aColor->red,aColor->green,aColor->blue);
}


// NOTE: This method won't do anything on truecolor displays...
bool ADisplay::setColor(unsigned int colorNum, unsigned int r,unsigned int g, unsigned int b)
{
  bool ret=false;
  if(staticColor) return false;
  // Assume that if we have more than 256 colors, we're truecolor.
  if(colorNum>255) return false;
  if(!colors) return false;
  if(depth>8) return false;
  unsigned int dColors=1<<depth;
#ifdef ASYS_AMIGA
  if(isHAM) if(depth==12) dColors=16; else dColors=64;
#endif
  if(dColors<colorNum) return false;
  if(colorNum<colors->getNumColors()) {
    colors->setColor(colorNum,r,g,b);
  }
#ifdef ASYS_X11_BASED
  if(!colorLock) matchColors(colors);
#endif
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
  if(!colorLock) matchColors(colors);
#endif
#endif
#ifdef ASYS_MAC
  // FIXME: Check that colorNum is within legal range...
  ColorSpec cs;
  cs.value=colorNum; cs.rgb.red=r<<8;  cs.rgb.green=g<<8;  cs.rgb.blue=b<<8;
#ifdef USE_QUARTZ
#else
  SetEntries(colorNum,0,&cs);
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_DOS
  ret=true;
  if(!haveFocus) return ret;
  REGS16 rin,rout;
  if(nPlanes==4) {
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"Set EGA "<<colorNum<<" to "<<r<<","<<g<<","<<b<<" ";
#endif // DEBUG_VERBOSE
    // EGA-style color
    unsigned int colorEGA=colorNum;
    if(aThisApp.videoHardware>A_VIDEO_EGA) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<" (EGA1)";
#endif // DEBUG_VERBOSE
      rout.h.dh=(char)((r>>2)&0x3f); rout.h.ch=(char)((g>>2)&0x3f); rout.h.cl=(char)((b>>2)&0x3f);
#ifdef __SC__
      rout.x.bx=(unsigned short)colorNum;  rout.x.ax=0x1010;
#else
      rout.w.bx=(unsigned short)colorNum;  rout.w.ax=0x1010;
#endif
      int86(0x10,&rout,&rin);
    }
    if(aThisApp.videoHardware==A_VIDEO_EGA) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<" (EGA2)";
#endif // DEBUG_VERBOSE
      colorEGA=((b&0x80)>>7)|((b&0x40)>>1)|((g&0x80)>>6)|((g&0x40)>>2)|
        ((r&0x80)>>5)|((r&0x40)>>3);
    }
    if(aThisApp.videoHardware>A_VIDEO_CGA) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<" (EGA3)";
#endif // DEBUG_VERBOSE
      rout.h.bh=(char)colorEGA; rout.h.bl=(char)colorNum; rout.h.ah=0x10;  rout.h.al=0x00;
      int86(0x10,&rout,&rin);
    }
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"\n";
#endif // DEBUG_VERBOSE
  }
  if(nPlanes==1) {
    // VGA-style color
    // FIXME: Don't assume VGA hardware, use vesa 2.0 if we can here
    if((aThisApp.videoHardware>A_VIDEO_EGA)&&(!aThisApp.monoMode)) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"Set VGA "<<colorNum<<" to "<<r<<","<<g<<","<<b<<"\n";
#endif // DEBUG_VERBOSE
      outportb(0x3c6,0xff); outportb(0x3c8,colorNum); outportb(0x3c9,r>>2);
      outportb(0x3c9,g>>2);  outportb(0x3c9,b>>2);
    }
  }
#endif
#ifdef ASYS_AMIGA
  if(!TheScreen) return false;
  if(aThisApp.version>38) {
    SetRGB32(&(TheScreen->ViewPort),colorNum,r<<24,g<<24,b<<24);
  }
  else {
    SetRGB4(&(TheScreen->ViewPort),colorNum,r>>4,g>>4,b>>4);
  }
  ret=true;
#endif
  return ret;
}


// NOTE: Make sure this function is always fast and can be called
// frequently...see AImage::convert (hint: ADisplay::getHeight calls this)
// FIXME: Make this faster and smaller!
void ADisplay::sync()
{
  // NOTE: Here we check the actual display and update the various
  // properties: width,height,depth, etc.
  // First set some defaults...
  unsigned int paletteSize=0;
  bitmapType=A_BITMAP_NONE;
  avgCharWidth=8;  // arbitrary
  if(!width) width=640;
  if(!height) height=200;
  if(!depth) depth=1;
  if(!nPlanes) nPlanes=1;
  avgCharWidth=8;
  //fullScreenOnly=true; staticColor=true;
  canUseAnyImageFlag=false;  // Actually...can't use ANY image at all!
  canSetAnyColorFlag=false;
#ifdef ASYS_AMIGA
  // FIXME: check screen's viewport for actual mode, etc...
  if(TheScreen) {
    width=TheScreen->Width;
    height=TheScreen->Height;
    x=TheScreen->LeftEdge;
    y=TheScreen->TopEdge;
    nPlanes=TheScreen->RastPort.BitMap->Depth;
    depth=nPlanes;
    isHAM=false;
    if((depth==12)&&(nPlanes==6)) { isHAM=true; /* depth=6; */ }
    if((depth==24)&&(nPlanes==8)) { isHAM=true; /* depth=8; */ }
    if(isHAM) if(nPlanes==6) paletteSize=16; else paletteSize=64;
  }
  else { x=0; y=0; width=0; height=0; depth=0; nPlanes=0; }
  bitmapType=A_BITMAP_PLANES;
  bitsTriplet=4;
  if(aThisApp.videoHardware>=A_VIDEO_AGA) bitsTriplet=8;
  avgCharWidth=8;  // FIXME: Need to get actual value!
#endif // ASYS_AMIGA
#ifdef ASYS_OS2
  HDC hdc;
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"Going to WinOpenWindowDC...\n";
#endif // DEBUG_VERBOSE
  hdc=(HDC)WinOpenWindowDC((HWND)HWND_DESKTOP);
  /*
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to WinQueryWindowDC...\n";
#endif // DEBUG_VERBOSE
  hdc=(HDC)WinQueryWindowDC((HWND)HWND_DESKTOP);
  */
  // FIXME: Check for error here...
  LONG lStart=0;
  LONG lCount=CAPS_DEVICE_FONT_SIM;
  LONG a[CAPS_DEVICE_FONT_SIM];
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"Going to DevQueryCaps...\n";
#endif // DEBUG_VERBOSE
  bool ret=DevQueryCaps(hdc,lStart,lCount,a);
  // FIXME: Check for error here...
  width=(unsigned int)a[CAPS_WIDTH];
  height=(unsigned int)a[CAPS_HEIGHT];
  depth=(unsigned int)a[CAPS_COLOR_BITCOUNT];
  nPlanes=(unsigned int)a[CAPS_COLOR_PLANES];
  bitsTriplet=8;
  if(depth>8) trueColor=true; else trueColor=false;
  if(!trueColor) canSetAnyColorFlag=true;
  canUseAnyImageFlag=true;
  avgCharWidth=8;  // FIXME: Need to get actual value!
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
  nPlanes=1;  width=320;  height=200;  depth=1;  // Bare minimum
#ifdef ASYS_WIN32CE
  //width=GetSystemMetrics(SM_CXSCREEN);
  //height=GetSystemMetrics(SM_CYSCREEN)-GetSystemMetrics(SM_CYMENU);
  //depth=2; // For now...
  HDC hDC=(HDC)NULLHANDLE;
  hDC=CreateDC(L"DISPLAY",(wchar_t *)NULL,(wchar_t *)NULL,(DEVMODE *)NULL);
  if(hDC) {
    nPlanes=GetDeviceCaps(hDC,PLANES);  width=GetDeviceCaps(hDC,HORZRES);
    height=GetDeviceCaps(hDC,VERTRES);  depth=GetDeviceCaps(hDC,BITSPIXEL);
    DeleteDC(hDC);
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"GetDeviceCaps says desktop is "<<width<<"x"<<height<<"\n";
#endif // DEBUG_VERBOSE
  }
  RECT myr;
  if(SystemParametersInfo(SPI_GETWORKAREA,0,&myr,0)) {
    width=(myr.right-myr.left);  height=(myr.bottom-myr.top);
    yOffset=myr.top;  xOffset=myr.left;
    yMargin=0;  xMargin=0;
  }
  if(!trueColor) canSetAnyColorFlag=true;
#else
  HDC hDC=(HDC)NULLHANDLE;
  hDC=CreateDC(ATEXT("DISPLAY"),NULL,NULL,(DEVMODE *)NULL);
  if(hDC) {
    nPlanes=GetDeviceCaps(hDC,PLANES);  width=GetDeviceCaps(hDC,HORZRES);
    height=GetDeviceCaps(hDC,VERTRES);  depth=GetDeviceCaps(hDC,BITSPIXEL);
    DeleteDC(hDC);
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"GetDeviceCaps says desktop is "<<width<<"x"<<height<<"\n";
#endif // DEBUG_VERBOSE
  }
  RECT myr;
#ifdef ASYS_WIN32
#ifndef ASYS_WIN386
  if(GetClientRect(GetDesktopWindow(),&myr)) {
    width=myr.right;  height=myr.bottom;
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"GetClientRect says desktop is "<<width<<"x"<<height<<"\n";
#endif // DEBUG_VERBOSE
  }
#endif // ASYS_WIN386
#endif // ASYS_WIN32
#ifdef USE_DIRECTDRAW
  canSetAnyColorFlag=false;  // FIXME: Is this right?  Only on Truecolor?
#else
  // Get space taken up by Start menu...but not if we're using DirectDraw
#ifdef ASYS_WIN32
#ifndef ASYS_WIN386
  // Does Win16 have this call?
  if(SystemParametersInfo(SPI_GETWORKAREA,0,&myr,0)) {
    width=(myr.right-myr.left);  height=(myr.bottom-myr.top);
    yOffset=myr.top;  xOffset=myr.left;
    yMargin=0;  xMargin=0;
  }
#endif // ASYS_WIN386
#endif // ASYS_WIN32
  if(!trueColor) canSetAnyColorFlag=true;
  canUseAnyImageFlag=true;
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32CE
  if(nPlanes>depth) depth=nPlanes;
  bitsTriplet=8;
  if(depth>8) trueColor=true; else trueColor=false;
  avgCharWidth=8;  // FIXME: Need to get actual value!
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
  nPlanes=1;  // No planar modes
#ifdef REALLY_OLD_MAC
  depth=1;
  width=qd.screenBits.bounds.right-qd.screenBits.bounds.left;
  height=qd.screenBits.bounds.bottom-qd.screenBits.bounds.top;
  staticColor=true;
#else
  CGrafPtr origPort;
  GDHandle origDev;
  width=640;  height=480;
#ifdef USE_QUARTZ
#else
  GetGWorld(&origPort,&origDev);
  if((unsigned long)origDev>256) {
    width=(*origDev)->gdRect.right-(*origDev)->gdRect.left;
    height=(*origDev)->gdRect.bottom-(*origDev)->gdRect.top;
  }
  else {
    DEBUG_OUT<<"GetGWorld returned bogus device!\n";
  }
  curMode=0;
#endif // USE_QUARTZ
  depth=8;
#ifdef USE_QUARTZ
#else
  gfxDevice=GetGDevice();
  if(*gfxDevice) {
    depth=(*(*gfxDevice)->gdPMap)->pixelSize;
    curMode=(*gfxDevice)->gdMode;
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"gfxDevice mode is "<<curMode<<"\n";
    //DEBUG_OUT<<"gfxDevice type is "<<(*gfxDevice)->gdType<<"\n";
    //DEBUG_OUT<<"gfxDevice flags is "<<aThisApp.myHex((*gfxDevice)->gdFlags)<<"\n";
#endif // DEBUG_VERBOSE
  }
  else {
    DEBUG_OUT<<"gfxDevice handle doesn't point to anything!\n";
  }
#endif // USE_QUARTZ
  bitsTriplet=16;
  if(!trueColor) canSetAnyColorFlag=true;
  staticColor=true;  // We don't change the palette unless we set the video mode...
#endif // REALLY_OLD_MAC
  if(depth>8) trueColor=true; else trueColor=false;
  if(depth==1) bitsTriplet=0;
  yOffset=3;
  //yOffset+=GetMBarHeight()*2;
  xOffset=7;
  yMargin=8;  xMargin=7;
  avgCharWidth=8;  // FIXME: Need to get actual value!
#endif // ASYS_MAC
#ifdef ASYS_X11_BASED
  width=DisplayWidth(theDisplay,0);
  height=DisplayHeight(theDisplay,0);
  depth=DefaultDepth(theDisplay,0);
  nPlanes=depth;
  XFlush(theDisplay);
  bitmapType=A_BITMAP_PLANES;
  if(theVis->c_class&0x1) bitsTriplet=theVis->bits_per_rgb;
  else bitsTriplet=0;
  avgCharWidth=6;  // FIXME: a wild guess that usually works...
#endif // ASYS_X11_BASED
#ifdef ASYS_DOS
  // Modes that need special treatment
  if(!choice) { DEBUG_OUT<<"(no choice in sync)\n"; return; }
  interleaved=false;
  if(choice->flags&A_FLAG_INTER) interleaved=true;
  vptr=(long)choice->vMemPtr;
  vsize=choice->vMemSize;
  bytesPerLine=choice->bytesPerLine;
  width=choice->width;
  height=choice->height;
  depth=choice->depth;  nPlanes=choice->nPlanes;
  if(choice->flags&A_FLAG_TEXT) numCols=80;
  else numCols=(width/8);
  // This isn't always right...
  if(choice->flags&A_FLAG_TEXT) numRows=25;
  else numRows=(height/8);
  avgCharWidth=8;
  if(choice->flags&A_FLAG_TEXT) avgCharWidth=1;
  bitsTriplet=choice->bitsTriplet;
  //if(!trueColor) canSetAnyColorFlag=true;
#endif // ASYS_DOS
#ifdef ASYS_PALM
  SWord tw,th;
  WinGetDisplayExtent(&tw,&th);
  width=tw;  height=th;
  depth=1;  nPlanes=1;
  staticColor=true;
  canUseAnyImageFlag=false;
#ifdef __PALMOS_H__
  if(aThisApp.hasThreePointFive) {
    UInt32 tw,th,td;
    Boolean tc;
    Err terr;
    terr=WinScreenMode(winScreenModeGet,&tw,&th,&td,&tc);
    width=tw;  height=th;  depth=td;
  }
#endif // __PALMOS_H__
  avgCharWidth=FntAverageCharWidth();
#endif // ASYS_PALM
#ifdef ASYS_GLUT
  depth=24;  nPlanes=1;
#endif // ASYS_GLUT
  // Calc the pixel type...
  if(bitmapType==A_BITMAP_NONE) {
    if(nPlanes>1) bitmapType=A_BITMAP_PLANES;
    else bitmapType=A_BITMAP_CHUNKY;
    if(depth==1) bitmapType=A_BITMAP_PLANES;
  }
  // Take care of palette
  if(depth<9) {
    if(!paletteSize) paletteSize=1<<depth;
    if(colors) if(paletteSize!=colors->getNumColors())
      { delete colors;  colors=(APalette *)NULL; }
    if(!colors) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"Allocated palette...\n";
#endif // DEBUG_VERBOSE
      colors=new APalette(paletteSize);
    }
  }
  if((depth>8)&&(colors)) { delete colors;  colors=(APalette *)NULL; }
  if(!bitsTriplet) staticColor=true;
  if(depth>8) { staticColor=true; trueColor=true; }
  calcRGBBits();
  //DEBUG_OUT<<"ADisplay::sync calc...\n";
  calc();
  syncCurrentColors();  // Probably shouldn't call this EVERY time...
}


void ADisplay::calcRGBBits()
{
  if(trueColor) {
    switch(depth) {
      case 8: rbits=3;  gbits=3;  bbits=2; break;
      case 15: rbits=5;  gbits=5;  bbits=5; break;
      case 16: rbits=5;  gbits=6;  bbits=5; break;
      default: break;
    }
    // TODO: assumes RGB (not BGR)
    rshift=bbits+gbits;  gshift=bbits;  bshift=0;
    //DEBUG_OUT<<"rbits="<<rbits<<" gbits="<<gbits<<" bbits="<<bbits<<"\n";
    //DEBUG_OUT<<"rshift="<<rshift<<" gshift="<<gshift<<" bshift="<<bshift<<"\n";
    //DEBUG_OUT<<"rmask="<<rmask<<" gmask="<<gmask<<" bmask="<<bmask<<"\n";
  }
}


void ADisplay::syncCurrentColors()
{
  if(colors) {
    unsigned int paletteSize=colors->getNumColors();
#ifdef ASYS_AMIGA
    if(isHBrite) colors->setHBrite();
    // FIXME: read palette colors here...
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef REALLY_OLD_MAC
    // FIXME: I might have black and white reversed here...
    colors->setColor(0,0,0,0);  colors->setColor(1,0xff,0xff,0xff);
#else
    CGrafPtr origPort;
    GDHandle origDev;
#ifdef USE_QUARTZ
#else
    GetGWorld(&origPort,&origDev);
    gfxDevice=GetGDevice();
    // FIXME: check if we can't just read the colors...like on a color
    // screen set to gray (?) and then set palette by hand...
    unsigned int mode=(*gfxDevice)->gdMode,type=(*gfxDevice)->gdType,flags=(*gfxDevice)->gdFlags;
#endif // USE_QUARTZ
    bool readEm=true;
    //if((!type)&&(mode==128)&&(flags&1)) readEm=false;
    if(readEm) {
      RGBColor aColor;
      for(unsigned int t=0;t<paletteSize;t++) {
#ifdef USE_QUARTZ
#else
        Index2Color(t,&aColor);
#endif // USE_QUARTZ
        colors->setColor(t,aColor.red,aColor.green,aColor.blue);
      }
    }
    else {
      for(unsigned int t=0;t<paletteSize;t++) {
        colors->setColor(t,255-t,255-t,255-t);
      }
    }
#endif // REALLY_OLD_MAC
#endif // ASYS_MAC
#ifdef ASYS_DOS
    for(unsigned int t=0;t<paletteSize;t++) {
      // FIXME: read rgb values here...
      // FIXME: set rgb values here...
    }
#endif // ASYS_DOS
#ifdef ASYS_WINDOWS
    for(unsigned int t=0;t<paletteSize;t++) {
      // FIXME: read rgb values here...
      // FIXME: set rgb values here...
    }
#endif // ASYS_WINDOWS
#ifdef ASYS_X11_BASED
    XColor theColor;
    for(unsigned int t=0;t<paletteSize;t++) {
      theColor.pixel=t;
      XQueryColor(theDisplay,
        DefaultColormap(theDisplay,DefaultScreen(theDisplay)),
        &theColor);
      colors->setColor(t,theColor.red,theColor.green,theColor.blue);
    }
#endif // ASYS_X11_BASED
  }
}


UINT ADisplay::calcScaleFactorX(UINT w)
{
  DEBUG_OUT<<"calcScaleFactorX("<<w<<")\n";
  DEBUG_OUT<<"display is "<<width<<","<<height<<"\n";
  UINT prev1=1,prev2=1,prev3=1;
  UINT pick=1;
  UINT scaleFactorX=1;
  for(UINT t=0;t<16;t++) {
    if(width<(w*scaleFactorX)) {
      scaleFactorX=scaleFactorX/2;
    }
    else {
      if(width>(w*scaleFactorX)) {
        scaleFactorX=scaleFactorX*2;
      }
    }
    DEBUG_OUT<<"thinking about using "<<scaleFactorX<<"...\n";
    prev3=prev2;  prev2=prev1;  prev1=scaleFactorX;
    if((prev3==prev1)&&(prev1<=prev2)) pick=prev1;
  }
  scaleFactorX=pick;
  DEBUG_OUT<<"scaleFactorX="<<scaleFactorX<<"("<<w*scaleFactorX<<")\n";
  return scaleFactorX;
}


UINT ADisplay::calcScaleFactorY(UINT h)
{
  DEBUG_OUT<<"calcScaleFactorY("<<h<<")\n";
  DEBUG_OUT<<"display is "<<width<<","<<height<<"\n";
  UINT prev1=1,prev2=1,prev3=1;
  UINT pick=1;
  UINT scaleFactorY=1;
  for(UINT t=0;t<16;t++) {
    if(height<(h*scaleFactorY)) {
      scaleFactorY=scaleFactorY/2;
    }
    else {
      if(height>(h*scaleFactorY)) {
        scaleFactorY=scaleFactorY*2;
      }
    }
    DEBUG_OUT<<"thinking about using "<<scaleFactorY<<"...\n";
    prev3=prev2;  prev2=prev1;  prev1=scaleFactorY;
    if((prev3==prev1)&&(prev1<=prev2)) pick=prev1;
  }
  scaleFactorY=pick;
  DEBUG_OUT<<"scaleFactorY="<<scaleFactorY<<"("<<h*scaleFactorY<<")\n";
  return scaleFactorY;
}


void ADisplay::reallyFocus()
{
#ifdef ASYS_DOS
  set(choice);
  matchColors(colors);
#endif
  focus();
}


void ADisplay::focus()
{
  bool hadFocus=haveFocus;
  unfocus();
  //if(aThisApp.focusDisplay) aThisApp.focusDisplay->unfocus();
  //aThisApp.focusDisplay=this;
  haveFocus=true;
#ifdef ASYS_DOS
  if(!hadFocus) {
    set(choice);
    matchColors(colors);
  }
#endif // ASYS_DOS
#ifdef ASYS_AMIGA
  if(TheScreen) ScreenToFront(TheScreen);
#endif // ASYS_AMIGA
}


// FIXME: doesn't work as we currently have things going...
// instead, you should just focus to the window/display you want...
// unfocusing doesn't do what it should.
void ADisplay::unfocus()
{
  haveFocus=false;
  // The next part is actually a "side effect"
  // See AWindowAbstract::focus()
  /*
  windows.reset();
  while(!windows.atEnd()) {
    AWindowAbstract *w=(AWindowAbstract *)windows.info();
    w->unfocus();
    windows.advance();
  }
  windows.reset();
  */
}


unsigned long ADisplay::fixForeColor(unsigned long c)
{
  unsigned long fc=c;
  if(canSetAnyColorFlag||trueColor) fc=fixRGBColor(c);
  else {
#ifdef ASYS_X11_BASED
    if(fc==0xffffff) fc=1;
#endif
#ifdef ASYS_AMIGA
    if(fc==0xffffff) fc=1;
#endif
    if(colors) fc=fc%colors->getNumColors();
    else fc=fixRGBColor(c);
#ifdef ASYS_DOS
    if(choice->flags&A_FLAG_TEXT) fc=0x7;
#endif
  }
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"fixForeColor: asked for "<<c<<" returned "<<fc<<"\n";
#endif // DEBUG_VERBOSE
  return fc;
}


unsigned long ADisplay::fixBackColor(unsigned long c)
{
  unsigned long fc=c;
  if(canSetAnyColorFlag||trueColor) fc=fixRGBColor(c);
  else {
#ifdef ASYS_X11_BASED
    if(fc==0xffffff) fc=1;
#endif
#ifdef ASYS_AMIGA
    if(fc==0xffffff) fc=1;
#endif
    if(colors) fc=fc%colors->getNumColors();
    else fc=fixBackColor(c);
#ifdef ASYS_DOS
    if(choice->flags&A_FLAG_TEXT) fc=0x32;
#endif
  }
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"fixBackColor: asked for "<<c<<" returned "<<fc<<"\n";
#endif // DEBUG_VERBOSE
  return fc;
}


// NOTE: c is 24 bit RGB
unsigned long ADisplay::fixRGBColor(unsigned long c)
{
  unsigned long fc=c;
  unsigned int red=(int)((fc&0xff0000L)>>16);
  unsigned int green=(int)((fc&0xff00)>>8);
  unsigned int blue=(int)(fc&0xff);
  if(staticColor&&grayscale) {
    fc=(red+green+blue)/3;
        return fc;
  }
  if(trueColor) {
    if((depth==24)||(depth==32)) return fc;
    //DEBUG_OUT<<"1red="<<red<<" green="<<green<<" blue="<<blue<<"\n";
    red=red>>(8-rbits); green=green>>(8-gbits); blue=blue>>(8-bbits);
    fc=((red<<(rshift))&rmask)|((green<<gshift)&gmask)|((blue<<bshift)&bmask);
    //DEBUG_OUT<<"2red="<<red<<" green="<<green<<" blue="<<blue<<"\n";
  }
  return fc;
}


/*
ABitmap *ADisplay::getTheFont(bool tryToLoad)
{
  if(!theFont) {
    char *fontName=ATEXT("font.iff");
    // FIXME: Have should be a prefs kinda thing...
#ifdef ASYS_PC_BASED
    //fontName=ATEXT("d:\\src\\rsrc\\font\\font.iff");
#endif // ASYS_PC_BASED
#ifdef ASYS_UNIX_BASED
    //fontName=ATEXT("~/src/rsrc/font/font.iff");
#endif // ASYS_UNIX_BASED
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Going to load theFont..."<<fontName<<"\n";
#endif // DEBUG_VERBOSE
    AImageDecoder *readFile=(AImageDecoder *)
      ASmartImageDecoder::pickImageDecoder(fontName);
    if(readFile) {
      if(true) {
        delete readFile;
        readFile=(AImageDecoder *)NULL;
        theFont=new ABitmap(fontName);
        if(theFont) {
          if(!theFont->error()) {
#ifdef DEBUG_VERBOSE
            DEBUG_OUT<<"theFont looks good.\n";
#endif // DEBUG_VERBOSE
            theFont->convert(depth,bitmapType,(APalette *)NULL);
            return theFont;
          }
        }
      }
      if(readFile) { delete readFile; readFile=(AImageDecoder *)NULL; }
    }
    // FIXME: Don't use DEBUG_OUT.warning here...it'll cause a race condition
    DEBUG_OUT<<"Couldn't find font...giving up!\n";
#ifdef ALIB_HAVE_EXIT
  //exit(5);
#endif // ALIB_HAVE_EXIT
  return NULL;
  }
  return theFont;
}
*/


APrivateBitmap *ADisplay::getCompatibleBitmap(ADisplayMode *d)
{
  DEBUG_OUT<<"ADisplay::getCompatbileBitmap()...\n";
  bool nPS=false,nBS=false,nI4=false,nTXT=false,nI=false,nFlip=false;
  if(this->getNPlanes()>1) nPS=true;
  if(d) {
    // NOTE: Some of this very dos specific
    if(d->vMemSize>0xffff) nBS=true;
    if(d->flags&128) nI=true;
    if((d->mode==0x07)&&(d->flags&64)) nTXT=true;
    if((d->mode==0x07)&&(d->flags&128)) nI4=true;
  }
#ifdef ASYS_WINDOWS
  nFlip=true;
#endif // ASYS_WINDOWS
  DEBUG_OUT<<"nPS="<<nPS<<" nBS="<<nBS<<" nI4="<<nI4<<" nTXT="<<nTXT<<" nI="<<nI<<" nFlip="<<nFlip<<"\n";
  if(nTXT) return ((APrivateBitmap *)new ABitmapText(this,d));
  if(nI4) return ((APrivateBitmap *)new ABitmapInterFour(this,d));
  if(nI) return ((APrivateBitmap *)new ABitmapInter(this,d));
  if(nBS&&nPS) return ((APrivateBitmap *)new ABitmapBanksPlanes(this,d));
  if(nBS) return ((APrivateBitmap *)new ABitmapBanks(this,d));
  if(nPS) return ((APrivateBitmap *)new ABitmapPlanes(this,d));
  if(nFlip) return ((APrivateBitmap *)new ABitmapFlipped(this,d));
  return ((APrivateBitmap *)new APrivateBitmap(this,d));
}

