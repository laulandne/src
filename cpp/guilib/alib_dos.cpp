
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#ifdef ASYS_DOS
#include <alib/adebug.h>
#include <alib/aapp.h>
#include <guilib/aevent.h>
#include <guilib/adisplay.h>
#include <guilib/alib_dos.h>
#endif // ASYS_DOS


/*
#ifdef ASYS_DOS
#ifdef ASYS_DJGPP
#include <sys/nearptr.h>
#endif // ASYS_DJGPP
#ifdef __WATCOM_CPLUSPLUS__
#ifdef ASYS_DOS16
#include <i86.h>
#include <conio.h>
#define REGS16 REGS
#define inportb(x) inp(x)
#define outportb(x,y) outp(x,y)
#endif // ASYS_DOS16
#endif // __WATCOM_CPLUSPLUS__
#endif // ASYS_DOS
*/


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


#ifdef ASYS_DOS
// For now, the same as mapVideoMem...
unsigned char *mapOtherMem(unsigned char *ptr, unsigned long size)
{
  return mapVideoMem(ptr,size);
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
unsigned char *mapVideoMem(unsigned char *ptr, unsigned long size)
{
  unsigned char *ret=ptr;
#ifdef ASYS_DOS32
  ret=NULL;
  // Remember ptr is a REAL address! (not the same as in unmap)
  dBug<<"app mapping physical memory ("<<size<<"@0x"<<((unsigned long)ptr)<<")...\n";
  unsigned long addr=(unsigned long)ptr;
  // correct address from real mode segment/offset to flat here
  // That is 0xa000:0000 is really 0xa0000
  addr=(addr>>12)+(addr&0xffff);
  // size must be 4k minimum...
  if(size<4096) size=4096;  // this shouldn't hurt...
  unsigned char *mptr=NULL;
  long lret= -1;
  bool didIt=false;
#ifdef __SC__
  didIt=true;
  mptr=_x386_map_physical_address((char *)addr,size);
  if(mptr!=(unsigned char *)-1) lret=0;
#endif // __SC__
#ifdef ASYS_DJGPP
  dBug<<"djgpp map...\n";
  didIt=true;
  int nret=__djgpp_nearptr_enable();
  if(nret) {
    lret=0;
    mptr=(unsigned char *)__djgpp_conventional_base+addr;
  }
  else {
    dBug<<"couldn't nearptr_enable!\n";
    return NULL;
  }
#endif // ASYS_DJGPP
#ifdef __EMX__
  didIt=true;
  unsigned int first=addr,last=addr+size;
  mptr=(unsigned char *)_memaccess(first,last,1);
  if(mptr) lret=0;
#endif // __EMX__
#ifdef __WATCOM_CPLUSPLUS__
  didIt=true;
  lret=0;
  // Because 1st meg is mapped flat
  mptr=(unsigned char *)addr;
#endif // __WATCOM_CPLUSPLUS__
  if(!didIt) dBug.fatal("Need to get AApp::mapVideoMem for this compiler!!!\n");
  if(lret== -1) {
    dBug<<"Map failed.\n";
    return NULL;
  }
  else {
    dBug<<"Mapped 0x"<<((unsigned long)ptr)<<" ("<<size<<" bytes)"<<" at 0x"<<((unsigned long)mptr)<<"\n";
  }
  ret=mptr;
#endif // ASYS_DOS32
  return ret;
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
// For now, the same as unmapVideoMem...
void unmapOtherMem(unsigned char *ptr, unsigned long size)
{
  unmapVideoMem(ptr,size);
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
void unmapVideoMem(unsigned char *ptr, unsigned long size)
{
#ifdef ASYS_DOS32
  // Remember ptr is a FLAT address! (not the same as in map)
  dBug<<"app unmapping physical memory: 0x"<<((unsigned long)ptr)<<" ("<<size<<" bytes).\n";
#ifdef __SC__
  // Actually nothing to do for this particular dos extender...
#endif // __SC__
#ifdef ASYS_DJGPP
  __djgpp_nearptr_disable();
#endif // ASYS_DJGPP
#endif // ASYS_DOS32
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
#ifdef USE_JOYSTICK
// FIXME: This will go into an infinite loop if there is no joystick!
// FIXME: fix this
unsigned short dosCenterJoy(unsigned short bit)
{
  unsigned short val=0;
  unsigned short mask=(unsigned short)(1<<bit);
  dBug<<"centerJoy mask is: "<<(int)mask<<" ("<<(int)bit<<")\n";
  cout<<"centerJoy mask is: "<<(int)mask<<" ("<<(int)bit<<")\n";
  // Trigger joystick oneshots
  outportb(0x201,0xff);
  unsigned short c=(unsigned short)inportb(0x201);
  for(unsigned int k=0;(c&mask)!=0;k++) {
    if((c&mask)!=0) val=(unsigned short)k;
    c=(unsigned short)inportb(0x201);
  }
  return val;
}
#endif // USE_JOYSTICK
#endif // ASYS_DOS


#ifdef ASYS_DOS
bool dosMyKbhit()
{
  bool ret=false;
#ifdef ASYS_DOS16
  ret=kbhit();
#endif // ASYS_DOS16
#ifdef ASYS_DOS32
#ifdef __EMX__
  int foo=_read_kbd(0,0,1);
  //dBug<<"_read_kbd returned "<<foo<<"\n";
  if(!foo) {
    // It was an extended key...
    foo=_read_kbd(0,0,1);
    //dBug<<"_read_kbd (ext) returned "<<foo<<"\n";
  }
  if(foo!= -1) ret=true;
#else
  ret=kbhit();
#endif // __EMX__
#endif // ASYS_DOS32
  return ret;
}
#endif // ASYS_DOS



#ifdef ASYS_DOS
unsigned long AEvent::dosHandleExtended(unsigned long c)
{
  unsigned long r=A_KEY_NEWLINE;
  switch(c) {
    case 59: r=A_KEY_F1;  break;
    case 60: r=A_KEY_F2;  break;
    case 61: r=A_KEY_F3;  break;
    case 62: r=A_KEY_F4;  break;
    case 63: r=A_KEY_F5;  break;
    case 64: r=A_KEY_F6;  break;
    case 65: r=A_KEY_F7;  break;
    case 66: r=A_KEY_F8;  break;
    case 67: r=A_KEY_F9;  break;
    case 68: r=A_KEY_F10;  break;
    case 71: r=A_KEY_HOME;  break;
    case 82: r=A_KEY_INSERT;  break;
    case 83: r=A_KEY_DELETE;  break;
    case 72:
      if(aThisApp.joyNotKeys) { number=aThisApp.whichJoy;  r=A_KEY_JOY_UP; }
      else r=A_KEY_UP;
      break;
    case 80:
      if(aThisApp.joyNotKeys) { number=aThisApp.whichJoy;  r=A_KEY_JOY_DOWN; }
      else r=A_KEY_DOWN;
      break;
    case 75:
      if(aThisApp.joyNotKeys) { number=aThisApp.whichJoy;  r=A_KEY_JOY_LEFT; }
      else r=A_KEY_LEFT;
      break;
    case 77:
      if(aThisApp.joyNotKeys) { number=aThisApp.whichJoy;  r=A_KEY_JOY_RIGHT; }
      else r=A_KEY_RIGHT;
      break;
    case 73:  // page up
    case 79:  // end
      if(aThisApp.joyNotKeys) {
        aThisApp.joyNotKeys=false; dBug<<"joystick keys off\n";
      }
      else { aThisApp.joyNotKeys=true; dBug<<"joystick keys on\n"; }
    case 81:  // page down
      if(aThisApp.whichJoy) {
        aThisApp.whichJoy=0; dBug<<"keys are joystick #1\n";
      }
      else { aThisApp.whichJoy=1; dBug<<"keys are joystick #2\n"; }
    default:
      r=A_KEY_NEWLINE;
      dBug<<"Unhandled extended key: "<<c<<"\n";
      break;
  }
  return r;
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
#ifdef USE_JOYSTICK
unsigned long AEvent::dosJoystick(unsigned int joynum)
{
  // This only handles the first fire button
  unsigned long key=0;
  // Trigger joystick oneshots
  outportb(0x201,0xff);
  unsigned short c=inportb(0x201);
  if(joynum) {
    if(c&0x20) key=' ';
    //if(c&0x80) key='\t';
  }
  else {
    if(c&0x10) key=' ';
    //if(c&0x40) key='\t';
  }
  unsigned short jx=0,jy=0;
  unsigned short xyMask=0,xMask=0,yMask=0;
  if(joynum) {
    xyMask=0x0c;  xMask=0x04;  yMask=0x08;
  }
  else {
    xyMask=0x03;  xMask=0x01;  yMask=0x02;
  }
  // Wait for the joy cap's to calm down
  for(unsigned int k=0;(c&xyMask)!=0;k++) {
    if((c&xMask)!=0) jx=k;
    if((c&yMask)!=0) jy=k;
    c=inportb(0x201);
  }
  //cout<<"dosJoystick: "<<jx<<","<<jy<<"\n";
  //dBug<<"dosJoystick: "<<jx<<","<<jy<<"\n";
  if(joynum) {
    ;
  }
  else {
    ;
  }
  if(key) cout<<"joy: "<<(int)key<<"\n";
  return key;
}
#endif // USE_JOYSTICK
#endif // ASYS_DOS


#ifdef ASYS_DOS
void AApp::dos_start()
{
  dBug<<"dos_start()...\n";
  //newlineSize=2;
  // Get DOS version
  REGS16 rin,rout;
  rin.h.ah=0x30;  int86(0x21,&rin,&rout);
  version=rout.h.al;  versionMinor=rout.h.ah;
  dBug<<"DOS version: "<<version<<"."<<versionMinor<<"\n";
  // Get current bios video mode
  rin.h.ah=15;  int86(0x10,&rin,&rout);
  origMode=rout.h.al;
#ifdef DEBUG_VERBOSE
  dBug<<"Video mode: "<<origMode<<"\n";
#endif // DEBUG_VERBOSE
  // If text, get attribute at cursor, we'll clear to that color later
  defaultAttrib=0;
  if(origMode==3) {
    // FIXME: Assume current page is 0
    rin.h.ah=0x08;    rin.h.bh=0x00;
    int86(0x10,&rin,&rout);
    defaultAttrib=rout.h.ah;
  }
  // ADisplay will set this...
  vesaMajor=0;  vesaMinor=0;
#ifdef DEBUG_VERBOSE
  dBug<<"-----------------------\n";
#endif // DEBUG_VERBOSE
  // Get the address that int 0x33 (msmouse interface) points to
  hasMouse=false;
#ifdef DEBUG_VERBOSE
  dBug<<"about to check for mouse...\n";
#endif // DEBUG_VERBOSE
  long *myfp=(long *)NULL;
  unsigned char *ms=(unsigned char *)NULL;
#ifdef ASYS_DOS16
  myfp=(long *)MK_FP(0,0x33*4);
  ms=(unsigned char *)*myfp;
#endif // ASYS_DOS16
#ifdef ASYS_DOS32
  myfp=(long *)mapOtherMem((unsigned char *)myfp);
  ms=mapOtherMem(ms);
#endif // ASYS_DOS32
#ifdef DEBUG_VERBOSE
  dBug<<"Mouse ";
#endif // DEBUG_VERBOSE
  if(ms!=NULL) { if(*ms!=0xcf) hasMouse=true; }
#ifdef DEBUG_VERBOSE
  if(!hasMouse) dBug<<"not ";
  dBug<<"found.\n";
#endif // DEBUG_VERBOSE
#ifdef ASYS_DOS32
  if(ms) unmapOtherMem(ms);
  if(myfp) unmapOtherMem((unsigned char *)myfp);
#endif // ASYS_DOS32
  if(hasMouse) {
    RIN_W_AX=0;
    int86(0x33,&rin,&rout);
  }
  extendedKey=false;
  hercGfx=false;  monoMode=false;  hasHerc=false;
  setMode=false;
  /*
  // Assume there are joysticks and read their centering values
  center1x=AEvent::centerJoy(0); center1y=AEvent::centerJoy(1);
  //center2x=AEvent::centerJoy(2); center2y=AEvent::centerJoy(3);
#ifdef DEBUG_VERBOSE
  dBug<<"Joystick 1 is: "<<center1x<<","<<center1y<<"\n";
  dBug<<"Joystick 2 is: "<<center2x<<","<<center2y<<"\n";
#endif // DEBUG_VERBOSE
  */
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
void ADisplay::setHercText()
{
  REGS16 rin,rout;
  rin.h.ah=0;  rin.h.al=0x07;
  int86(0X10,&rin,&rout);
  outportb(0x3b8,0x20);  // text, display off
  outportb(0x3bf,0x00);
  char nums[]={ 0x61,0x50,0x52,0x0f,0x19,0x06,
    0x19,0x19,0x02,0x0d,0x0b,0x0c };
  for(unsigned int a=0;a<12;a++) {
    outportb(0x3b4,a);  outportb(0x3b5,nums[a]);
  }
  outportb(0x3b8,0x20|0x08);  // text, display on
  // Clear screen here
  memset((void *)0xb0000000L,0,4096);
  aThisApp.hercGfx=false;
}
#endif


#ifdef ASYS_DOS
void ADisplay::setHercGfx()
{
  if(!aThisApp.hercGfx) {
    REGS16 rin,rout;
    rin.h.ah=0;  rin.h.al=0x07;
    int86(0X10,&rin,&rout);
    outportb(0x3b8,0x02);  // gfx, display off
    outportb(0x3bf,0x03);  // enable gfx, page 2...
    char nums[]={ 0x35,0x2d,0x2e,0x07,0x5b,0x02,
      0x57,0x57,0x02,0x03,0x00,0x00 };
    for(unsigned int a=0;a<12;a++) {
      outportb(0x3b4,a);  outportb(0x3b5,nums[a]);
    }
    outportb(0x3b8,0x02|0x08);  // gfx, display on
  }
  // Clear screen here
  memset((void *)0xb0000000L,0,32768L);
  aThisApp.hercGfx=true;
}
#endif


#ifdef ASYS_DOS
bool ADisplay::isTextMode()
{
  bool ret=false;
  //if(choice->flags&A_FLAG_TEXT) ret=true;
  return ret;
}
#endif


#ifdef ASYS_DOS
unsigned int ADisplay::getCurVideoMode()
{
  unsigned int ret=0;
  REGS16 rin,rout;
  // Read current mode...
  if(!aThisApp.vesaMajor) rin.h.ah=15;
  else RIN_W_AX=0x4f03;
  int86(0x10,&rin,&rout);
  if(!aThisApp.vesaMajor) ret=rout.h.al;
  else ret=ROUT_W_BX;
  return ret;
}
#endif


#ifdef ASYS_DOS
bool ADisplay::setVideoMode(ADisplayMode *tryMode)
{
  bool ret=false;
  hideMouse();
  /*
  if(tryMode->hardware==A_VIDEO_HERC) {
#ifdef DEBUG_VERBOSE
    dBug<<"Setting Hercules Gfx mode...\n";
#endif // DEBUG_VERBOSE
    setHercGfx();
    biossupp=false;  mousesupp=false;
    ret=true;  // Because the bios would ALWAYS FAIL.
  }
  else {
    */
    // Set desired mode
    setBiosVideoMode((unsigned int)tryMode->mode);
    // Check to see what we actually got
    unsigned long curid=getCurVideoMode();
    // Probably don't need to treat this quite so seriously
    if(curid!=tryMode->mode) {
#ifdef DEBUG_VERBOSE
      dBug<<"WARNING! Actual BIOS mode set is "<<curid<<"\n";
#endif // DEBUG_VERBOSE
      //ret=false;
    //}
    if(curid>0x14) {
#ifdef DEBUG_VERBOSE
      dBug<<"Setting a VESA mode, going to look up info...\n";
#endif // DEBUG_VERBOSE
      // FIXME: handle different read/write windows
      // FIXME: handle two windows and different locations...
      char p[256];
      char *ptr=(char *)&p[0];
      AVesaModeInfo *vi=(AVesaModeInfo *)ptr;
      bool bret=vesaGetModeInfo(tryMode->mode,vi);
      // FIXME: Check for error here!
      //wsize=vi->size;
      //wgran=vi->gran;
      mousesupp=true; // Don't assume, check vesa...
      biossupp=true;  // Don't assume, check vesa...
    }
    aThisApp.hercGfx=false;
  }
  // FIXME:Here search through dmList looking at mode and set
  // choice (or tryMode?) when we find what we got
  if((nPlanes==4)&&(!aThisApp.monoMode)) {
    // FIXME: Don't do this unless we need to (don't assume vga!)
#ifdef DEBUG_VERBOSE
    dBug<<"Setting up a 4 planer mode's registers...\n";
#endif // DEBUG_VERBOSE
    outportb(0x3c4,2);  outportb(0x3c5,0x0f);  // Write to all planes
    outportb(0x3ce,5);  outportb(0x3cf,0);  // Write mode 0
    outportb(0x3ce,1);  outportb(0x3cf,0);  // Disable fill read mode
    outportb(0x3ce,3);  outportb(0x3cf,0);  // No func, no rotate
  }
  if(theBits) {
    //theBits->clear();
  }
  showMouse();
  ret=true;
  return ret;
}
#endif


#ifdef ASYS_DOS
bool ADisplay::setBiosVideoMode(unsigned int mode)
{
  bool ret=true;
  REGS16 rin,rout;
  if(mode<0x14) { rin.h.ah=0;  rin.h.al=(char)mode; }
  else { RIN_W_AX=0x4f02;  RIN_W_BX=(unsigned short)mode; }
  int86(0X10,&rin,&rout);
  return ret;
}
#endif


#ifdef ASYS_DOS
bool ADisplay::hideMouse()
{
  bool ret=true;
  REGS16 rin,rout;
  // Hide mouse pointer
  if((aThisApp.hasMouse)&&(mousesupp)) {
    RIN_W_AX=2;
    int86(0x33,&rin,&rout);
  }
  return ret;
}
#endif


#ifdef ASYS_DOS
bool ADisplay::showMouse()
{
  bool ret=true;
  REGS16 rin,rout;
  // Show mouse pointer
  if((aThisApp.hasMouse)&&(mousesupp)) {
    RIN_W_AX=1;
    int86(0x33,&rin,&rout);
  }
  return ret;
}
#endif


// FIXME: Something's wrong with this somewhere...
#ifdef ASYS_DOS
bool ADisplay::vesaGetInfo(AVesaInfo *i)
{
#ifdef DEBUG_VERBOSE
  //dBug<<"Checking for vesa bios...\n";
#endif // DEBUG_VERBOSE
  bool ret=false;
  REGS16 rin,rout;
  SREGS srin;
  ROUT_W_AX=0;
  RIN_W_AX=0x4f00;
#ifdef ASYS_DOS16
  srin.es=FP_SEG(i);
  rin.x.di=FP_OFF(i);
  int86x(0x10,&rin,&rout,&srin);
#else
/*
#ifdef DEBUG_VERBOSE
  dBug<<"protected mode pointer is at "<<aThisApp.myHex((long)i)<<"\n";
#endif // DEBUG_VERBOSE
  unsigned char *src=aThisApp.map((unsigned char *)0xa0000000,0xffff);
  srin.es=0xa000;
  rin.x.di=0x0000;
  int86x_real(0x10,&rin,&rout,&srin);
  memcpy(i,src,sizeof(AVesaInfo));
  aThisApp.unmap(src,0xffff);
#ifdef DEBUG_VERBOSE
  dBug<<"rout.x.ax is "<<aThisApp.myHex(rout.x.ax)<<"\n";
#endif // DEBUG_VERBOSE
*/
#endif // ASYS_DOS16
  if(ROUT_W_AX==0x004f) ret=true;
#ifdef DEBUG_VERBOSE
  //if(ret) dBug<<"Vesa detected.\n";
#endif // DEBUG_VERBOSE
  return ret;
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
bool ADisplay::vesaGetModeInfo(unsigned long mode, AVesaModeInfo *i)
{
#ifdef DEBUG_VERBOSE
  //dBug<<"Checking for vesa mode...\n";
#endif // DEBUG_VERBOSE
  bool ret=false;
  REGS16 rin,rout;
  SREGS srin;
  ROUT_W_AX=0;
  RIN_W_AX=0x4f01;
  RIN_W_CX=(unsigned short)mode;
#ifdef ASYS_DOS16
  srin.es=FP_SEG(i);
  rin.x.di=FP_OFF(i);
  int86x(0x10,&rin,&rout,&srin);
#else
/*
#ifdef DEBUG_VERBOSE
  dBug<<"protected mode pointer is at "<<aThisApp.myHex((long)i)<<"\n";
#endif // DEBUG_VERBOSE
  unsigned char *src=aThisApp.map((unsigned char *)0xa0000000,0xffff);
  srin.es=0xa000;
  rin.x.di=0x0000;
  int86x_real(0x10,&rin,&rout,&srin);
  memcpy(i,src,sizeof(AVesaModeInfo));
  aThisApp.unmap(src,0xffff);
#ifdef DEBUG_VERBOSE
  dBug<<"rout.x.ax is "<<aThisApp.myHex(rout.x.ax)<<"\n";
#endif // DEBUG_VERBOSE
*/
#endif // ASYS_DOS16
  if(ROUT_W_AX==0x004f) ret=true;
#ifdef DEBUG_VERBOSE
  //if(ret) dBug<<"Vesa detected.\n";
#endif // DEBUG_VERBOSE
  return ret;
}
#endif // ASYS_DOS


#ifdef ASYS_DOS16
void *ADisplay::getFunc()
{
  void *ret=(void *)NULL;
  if(aThisApp.vesaMajor) {
    AVesaModeInfo i;
    vesaGetModeInfo(getBiosMode(),&i);
    ret=(void *)i.winfunc;
  }
  return ret;
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
unsigned int ADisplay::getEquipStatus()
{
  unsigned int ret=0;
  REGS16 rin,rout;
  // Read equipment status to see what bios thinks of the video cards
  int86(0x11,&rin,&rout);
  ret=(ROUT_W_AX&0x30)>>4;
  return ret;
}
#endif // ASYS_DOS


#ifdef ASYS_DOS
void ADisplay::setPaletteRegs()
{
  // FIXME: set all VGA palette regs to what colors has...
}
#endif // ASYS_DOS

