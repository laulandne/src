
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define CHECK_C64_MAPPING 1


#include <emu/NCbmHandler.h>
#include <emu/NCbmIO.h>
#include <emu/NCbmVideo.h>
#include <emu/NCbmKernHelper.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/AEvent.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/APalette.h>

#include <elib/EKeys.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>
#include <elib/EMemMapAbstract.h>
#include <elib/EVideoIOTextOnly.h>

#include <disklib/DController65.h>
#include <disklib/DVfs.h>

#include <cpulib/Em6502.h>


#define NCBM_WANT_KERN_TABLE 1
#include <emu/generated/NCbm_kern_tab.h>


#define HIGH_BYTECBM(x)  ((x&0xff00)>>8)
#define LOW_BYTECBM(x)  (x&0xff)


///////////////////////////////////////////////////////////////////////////////
//  NCbmHandler Class
///////////////////////////////////////////////////////////////////////////////

NCbmHandler::NCbmHandler()
{
  DEBUG_OUT<<"NCbmHandler constructor.\n";
  osHelp=NULL;
  init();
}


NCbmHandler::~NCbmHandler()
{
  DEBUG_OUT<<"NCbmHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


void NCbmHandler::init()
{
  EHandler::init();
  unsigned int t;
  c64Mode=false;
  mmuCR=0;
  mmuInited=false;
  for(t=0;t<4;t++) c128pcr[t]=0;
  c128io=true;  c128lo=false;  c128hi=false;
  c128mid=false;
  c65magic=false;
  c128bank=0;
  for(t=0;t<NCBM_MAX_KEY_ROWS;t++) keys[t]=0;
  key_count=0;  key_max=0;
  keyScan=0xff;
  joyKey[0]=0;  joyKey[1]=0;
  kernRom=(ERom *)NULL;  basARom=(ERom *)NULL;  basBRom=(ERom *)NULL;
  editRom=(ERom *)NULL;  charRom=(ERom *)NULL;  monitorRom=(ERom *)NULL;
  magicRom=(ERom *)NULL;  dosRom=(ERom *)NULL;  graphicsRom=(ERom *)NULL;
  ioMem=(EMemAbstract *)NULL;  ramHandle=(EMemAbstract *)NULL;
  mapLock=false;
  if(osHelp) delete osHelp;
  osHelp=new NCbmKernHelper(this);
  timerFactor=1;  timerSpeed=6;
  value0=0xff;  value1=0xff;  loram=true;  hiram=true;  charen=true;
}


/* STATIC */
bool NCbmHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"c64")) return true;
  if(!strcmp(name,"ultimax")) return true;
  if(!strcmp(name,"max")) return true;
  if(!strcmp(name,"sx64")) return true;
  if(!strcmp(name,"pet64")) return true;
  if(!strcmp(name,"pet4064")) return true;
  if(!strcmp(name,"edu4064")) return true;
  if(!strcmp(name,"cbm4064")) return true;
  if(!strcmp(name,"vic30")) return true;
  if(!strcmp(name,"c164")) return true;
  if(!strcmp(name,"c264")) return true;
  if(!strcmp(name,"c364")) return true;
  if(!strcmp(name,"c65")) return true;
  if(!strcmp(name,"c64dx")) return true;
  if(!strcmp(name,"c128")) return true;
  if(!strcmp(name,"c128cr")) return true;
  if(!strcmp(name,"c128d")) return true;
  if(!strcmp(name,"d128")) return true;
  if(!strcmp(name,"plus4")) return true;
  if(!strcmp(name,"c16")) return true;
  if(!strcmp(name,"c116")) return true;
  if(!strcmp(name,"vic10")) return true;
  if(!strcmp(name,"vic20")) return true;
  if(!strcmp(name,"vic")) return true;
  if(!strcmp(name,"vic-20")) return true;
  if(!strcmp(name,"pet")) return true;
  if(!strcmp(name,"pet2001")) return true;
  if(!strcmp(name,"pet3001")) return true;
  if(!strcmp(name,"pet3032")) return true;
  if(!strcmp(name,"pet4016")) return true;
  if(!strcmp(name,"pet4032")) return true;
  if(!strcmp(name,"pet8032")) return true;
  if(!strcmp(name,"pet8096")) return true;
  if(!strcmp(name,"pet8296")) return true;
  if(!strcmp(name,"cbm4032")) return true;
  if(!strcmp(name,"cbm8032")) return true;
  if(!strcmp(name,"cbm8096")) return true;
  if(!strcmp(name,"cbm8296")) return true;
  if(!strcmp(name,"cbm9000")) return true;
  if(!strcmp(name,"sp9000")) return true;
  if(!strcmp(name,"superpet9000")) return true;
  if(!strcmp(name,"superpet")) return true;
  if(!strcmp(name,"p500")) return true;
  if(!strcmp(name,"b500")) return true;
  if(!strcmp(name,"c500")) return true;
  if(!strcmp(name,"c510")) return true;
  if(!strcmp(name,"b128")) return true;
  if(!strcmp(name,"b256")) return true;
  if(!strcmp(name,"bx256")) return true;
  if(!strcmp(name,"b40")) return true;
  if(!strcmp(name,"c600")) return true;
  if(!strcmp(name,"c610")) return true;
  if(!strcmp(name,"c620")) return true;
  if(!strcmp(name,"c630")) return true;
  if(!strcmp(name,"c700")) return true;
  if(!strcmp(name,"c710")) return true;
  if(!strcmp(name,"c720")) return true;
  if(!strcmp(name,"c730")) return true;
  return false;
}


const char *NCbmHandler::getName()
{
  if(io_type==N_CBM_IO_C64) return "Commodore 64";
  if(io_type==N_CBM_IO_TED) return "Commodore 264 Series";
  if(io_type==N_CBM_IO_C65) return "Commodore 65 Prototype";
  if(io_type==N_CBM_IO_C128) return "Commodore 128";
  if(io_type==N_CBM_IO_VIC) return "Commodore Vic-20";
  if(io_type==N_CBM_IO_PET) return "Commodore PET Series";
  if(io_type==N_CBM_IO_B) return "Commodore B/P Series";
  return "Unknown Commodore";
}


void NCbmHandler::setKind(const char *tname)
{
  io_type=0;  io_variation=0;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"c64")) io_type=N_CBM_IO_C64;
  if(!strcmp(name,"ultimax")) { io_type=N_CBM_IO_C64; io_variation=1; }
  if(!strcmp(name,"max")) { io_type=N_CBM_IO_C64;  io_variation=1; }
  if(!strcmp(name,"sx64")) io_type=N_CBM_IO_C64;
  if(!strcmp(name,"pet64")) io_type=N_CBM_IO_C64;
  if(!strcmp(name,"vic30")) io_type=N_CBM_IO_C64;
  if(!strcmp(name,"pet4064")) io_type=N_CBM_IO_C64;
  if(!strcmp(name,"edu4064")) io_type=N_CBM_IO_C64;
  if(!strcmp(name,"cbm4064")) io_type=N_CBM_IO_C64;
  if(!strcmp(name,"c164")) io_type=N_CBM_IO_TED;
  if(!strcmp(name,"c264")) io_type=N_CBM_IO_TED;
  if(!strcmp(name,"c364")) io_type=N_CBM_IO_TED;
  if(!strcmp(name,"c65")) io_type=N_CBM_IO_C65;
  if(!strcmp(name,"c64dx")) io_type=N_CBM_IO_C65;
  if(!strcmp(name,"c128")) io_type=N_CBM_IO_C128;
  if(!strcmp(name,"c128d")) io_type=N_CBM_IO_C128;
  if(!strcmp(name,"c128cr")) io_type=N_CBM_IO_C128;
  if(!strcmp(name,"d128")) io_type=N_CBM_IO_C128;
  if(!strcmp(name,"plus4")) io_type=N_CBM_IO_TED;
  if(!strcmp(name,"c16")) { io_type=N_CBM_IO_TED;  io_variation=1; }
  if(!strcmp(name,"c116")) { io_type=N_CBM_IO_TED;  io_variation=1; }
  if(!strcmp(name,"vic10")) io_type=N_CBM_IO_VIC;
  if(!strcmp(name,"vic20")) io_type=N_CBM_IO_VIC;
  if(!strcmp(name,"vic")) io_type=N_CBM_IO_VIC;
  if(!strcmp(name,"vic-20")) io_type=N_CBM_IO_VIC;
  if(!strcmp(name,"pet")) io_type=N_CBM_IO_PET;
  if(!strcmp(name,"pet2001")) io_type=N_CBM_IO_PET;
  if(!strcmp(name,"pet3001")) io_type=N_CBM_IO_PET;
  if(!strcmp(name,"pet3032")) io_type=N_CBM_IO_PET;
  if(!strcmp(name,"pet4016")) io_type=N_CBM_IO_PET;
  if(!strcmp(name,"pet4032")) io_type=N_CBM_IO_PET;
  if(!strcmp(name,"pet8032")) { io_type=N_CBM_IO_PET; io_variation=1; }
  if(!strcmp(name,"pet8096")) { io_type=N_CBM_IO_PET; io_variation=2; }
  if(!strcmp(name,"pet8296")) { io_type=N_CBM_IO_PET; io_variation=2; }
  if(!strcmp(name,"cbm4032")) io_type=N_CBM_IO_PET;
  if(!strcmp(name,"cbm8032")) { io_type=N_CBM_IO_PET; io_variation=1; }
  if(!strcmp(name,"cbm8096")) { io_type=N_CBM_IO_PET; io_variation=1; }
  if(!strcmp(name,"cbm8296")) { io_type=N_CBM_IO_PET; io_variation=1; }
  if(!strcmp(name,"cbm9000")) { io_type=N_CBM_IO_PET; io_variation=3; }
  if(!strcmp(name,"superpet")) { io_type=N_CBM_IO_PET; io_variation=3; }
  if(!strcmp(name,"superpet9000")) { io_type=N_CBM_IO_PET; io_variation=3; }
  if(!strcmp(name,"sp9000")) { io_type=N_CBM_IO_PET; io_variation=3; }
  if(!strcmp(name,"c500")) io_type=N_CBM_IO_B;
  if(!strcmp(name,"c510")) io_type=N_CBM_IO_B;
  if(!strcmp(name,"p500")) io_type=N_CBM_IO_B;
  if(!strcmp(name,"b128")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"b256")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"b500")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"bx256")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"b40")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"c600")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"c610")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"c620")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"c630")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"c700")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"c710")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"c720")) { io_type=N_CBM_IO_B; io_variation=1; }
  if(!strcmp(name,"c730")) { io_type=N_CBM_IO_B; io_variation=1; }
  io_init(io_type);
}


void NCbmHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  //DEBUG_OUT<<"Creating new io's and video...\n";
  //EMemMirror *mirror=(EMemMirror *)NULL;
  io_type=ttype;
  bool doPatch=true;
  //map->saveAllRam();
  APalette *colors=NULL;
  switch(io_type) {
    case N_CBM_IO_C64:
      c64Mode=true;  // We're ALWAYS in c64 mode!
      myVideo=new NCbmVideo(this,40,25);
      if(myVideo) myVideo->setLocation(0x0800);
      myIO=new NCbmIO(this);
      break;
    case N_CBM_IO_C128:
      myVideo=new NCbmVideo(this,40,25);
      if(myVideo) myVideo->setLocation(0x0800);
      myIO=new NCbmIO(this);
      initMMUC128();
      break;
    case N_CBM_IO_C65:
      c64Mode=true;  // Initially in 64 mode...
      myVideo=new NCbmVideo(this,40,25);
      if(myVideo) myVideo->setLocation(0x0800);
      myIO=new NCbmIO(this);
      break;
    case N_CBM_IO_TED:
      myVideo=new NCbmVideo(this,40,25);
      if(myVideo) myVideo->setLocation(0x0c00);
      myIO=new NCbmIO(this);
      initMMUC264();
      break;
    case N_CBM_IO_VIC:
      myVideo=new NCbmVideo(this,22,23);
      // This moves depending on memory expansion
      if(myVideo) myVideo->setLocation(0x1000);
      myIO=new NCbmIO(this);
      break;
    case N_CBM_IO_PET:
      // CBM 2001, 3001, 4032
      if(io_variation==0) myVideo=new NCbmVideo(this,40,25);
      // CBM 8032
      if(io_variation==1) myVideo=new NCbmVideo(this,80,25);
      // CBM 8096 or CBM 8296
      if(io_variation==2) myVideo=new NCbmVideo(this,80,25);
      // SuperPET
      if(io_variation==3) {
        myVideo=new NCbmVideo(this,80,25);
        doPatch=false;
      }
      if(myVideo) myVideo->setLocation(0x8000);
      myIO=new NCbmIO(this);
      break;
    case N_CBM_IO_B:
      // P-variation
      if(io_variation==0) {
        myVideo=new NCbmVideo(this,40,25);
        if(myVideo) myVideo->setLocation(0xfd000);
      }
      // B-variation
      if(io_variation==1) {
        myVideo=new NCbmVideo(this,80,25);
        if(myVideo) myVideo->setLocation(0xfd000);
      }
      myIO=new NCbmIO(this);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_CBM_IO_NONE;
      err.set();
      exit(5);
      break;
  }
  connectParts();
  // Make sure low and high i/o are always there...
  // (Even if video gets temprorarily mapped over it)
  /*
  if(myIOLow||myIOHigh) {
    if(myIOLow) {
      map->map(myIOLow);
      map->write8(1,0);
    }
    if(myIOHigh) map->map(myIOHigh);
    if(myIOLow) {
      myIOLow->reset();
      map->unmap(myIOLow);
    }
    if(myIOHigh) map->unmap(myIOHigh);
  }
  */
  if(!mmuInited) {
    mmuInited=true;
    //map->saveCurrentMap();
  }
  // NOTE: Since we patch here it means we MUST have already loaded
  // (and mapped) the roms...so you set the IO LAST (even after the cpu)
  // in a config file...
  // TODO: This only works for C-64, but will be replaced when we clean up all the mmu code
  write8ZeroOne(1,0xff);
  mapParts();
  // We map in the C-65 kernel so its in the right place to get patched
  if(io_type==N_CBM_IO_C65) {
    EMemAbstract *foo=map->findArea("c65kern.rom");
    if(foo) { map->map(foo); DEBUG_OUT<<"Mapped in c65kern.rom so we can patch it...\n"; }
    else DEBUG_OUT<<"Couldn't find c65kern.rom to map so we could patch it!\n";
  }
  if(doPatch) osHelp->patchAll(NCbm_Kern);
  if(io_type==N_CBM_IO_C65) {
    EMemAbstract *foo=map->findArea("c65kern.rom");
    if(foo) { map->unmap(foo); DEBUG_OUT<<"Unmapped c65kern.rom, done patching.\n"; }
  }
}


void NCbmHandler::handleVic3MapC65(BYTE d030Byte)
{
  bool problem=false;
  DEBUG_OUT<<"Vic3 d030 set to "<<aDefaultStringHelper.myHex((unsigned char)d030Byte)<<"\n";
  // TODO: what are the other roms the vic chip manages?
  if(d030Byte&4) ((NCbmIO *)myIO)->colorRamExtend=true;
  else ((NCbmIO *)myIO)->colorRamExtend=false;
  if(d030Byte&2) ((NCbmIO *)myIO)->paletteMode=true;
  else ((NCbmIO *)myIO)->paletteMode=false;
  ERom *foo=(ERom *)NULL;
  if(d030Byte&0x80) { DEBUG_OUT<<"ROM@e000\n";  problem=true; }
  if(d030Byte&0x10) { DEBUG_OUT<<"ROM@a000\n";  problem=true; }
  if(d030Byte&0x08) { DEBUG_OUT<<"ROM@8000\n";  problem=true; }
  if(d030Byte&0x20) {
    if(!c65magic) {
      DEBUG_OUT<<"MAGIC@c000 (magic) swapped in\n";
      foo=(ERom *)map->findArea("magic.rom");
      if(foo) map->map(foo);
      c65magic=true;
    }
  }
  else {
    if(c65magic) {
      DEBUG_OUT<<"MAGIC@c000 (magic) swapped out\n";
      foo=(ERom *)map->findArea("magic.rom");
      if(foo) map->unmap(foo);
      c65magic=false;
    }
  }
  if(problem) getCPU()->debugger("Vic-3 0xd030 problem!");
}


void NCbmHandler::handleMMUC128(BYTE cr)
{
  DEBUG_OUT<<"CR set to "<<(short)cr<<"\n";
  ERom *foo=(ERom *)NULL;
  unsigned int b=(cr&0xc0)>>6;
  DEBUG_OUT<<"Ram bank "<<b<<" selected.\n";
  //map->setMem(b);
  if(cr&0x02) {
    if(c128lo) {
      DEBUG_OUT<<"LOROM@4000 (bas128a) swapped out\n";
      foo=(ERom *)map->findArea("bas128a.rom");
      if(foo) map->unmap(foo);
      c128lo=false;
    }
  }
  else {
    if(!c128lo) {
      DEBUG_OUT<<"LOROM@4000 (bas128a) swapped in\n";
      foo=(ERom *)map->findArea("bas128a.rom");
      if(foo) map->map(foo);
      c128lo=true;
    }
  }
  if(cr&0x0c) {
    if(c128mid) {
      DEBUG_OUT<<"MIDROM@8000 (bas128b) swapped out\n";
      foo=(ERom *)map->findArea("bas128b.rom");
      if(foo) map->unmap(foo);
      c128mid=false;
    }
  }
  else {
    if(!c128mid) {
      DEBUG_OUT<<"MIDROM@8000 (bas128b) swapped in\n";
      foo=(ERom *)map->findArea("bas128b.rom");
      if(foo) map->map(foo);
      c128mid=true;
    }
  }
  if(cr&0x30) {
    if(c128hi) {
      DEBUG_OUT<<"HIROM@C000 (kern, editor, char64) swapped out\n";
      foo=(ERom *)map->findArea("kern128.rom");
      if(foo) map->unmap(foo);
      foo=(ERom *)map->findArea("editor.rom");
      if(foo) map->unmap(foo);
      foo=(ERom *)map->findArea("char.rom");
      if(foo) map->unmap(foo);
      if(c128io) {
        foo=(ERom *)map->findArea("io.d000");
        if(foo) {
          map->unmap((EMemAbstract *)foo);
          map->map((EMemAbstract *)foo);
        }
      }
      c128hi=false;
    }
  }
  else {
    if(!c128hi) {
      DEBUG_OUT<<"HIROM@C000 (kern, editor, char64) swapped in\n";
      foo=(ERom *)map->findArea("kern128.rom");
      if(foo) map->map(foo);
      foo=(ERom *)map->findArea("editor.rom");
      if(foo) map->map(foo);
      foo=(ERom *)map->findArea("char.rom");
      if(foo) map->map(foo);
      if(c128io) {
        foo=(ERom *)map->findArea("char.rom");
        if(foo) map->unmap(foo);
        foo=(ERom *)map->findArea("io.d000");
        if(foo) {
          map->unmap((EMemAbstract *)foo);
          map->map((EMemAbstract *)foo);
        }
      }
      c128hi=true;
    }
  }
  if(cr&0x01) {
    if(c128io) {
      DEBUG_OUT<<"IO@d000 swapped out\n";
      foo=(ERom *)map->findArea("io.d000");
      if(foo) map->unmap((EMemAbstract *)foo);
      c128io=false;
      if(c128hi) {
        foo=(ERom *)map->findArea("char.rom");
        if(foo) map->map(foo);
      }
    }
  }
  else {
    if(!c128io) {
      DEBUG_OUT<<"IO@d000 swapped in\n";
      if(c128hi) {
        foo=(ERom *)map->findArea("char.rom");
        map->unmap(foo);
      }
      foo=(ERom *)map->findArea("io.d000");
      map->map((EMemAbstract *)foo);  c128io=true;
    }
  }
  mmuCR=cr;
}


BYTE NCbmHandler::getCRC128(unsigned int num)
{
  if(!num) return mmuCR;
  else return c128pcr[num-1];
}


void NCbmHandler::setCRC128(unsigned int num,BYTE value)
{
  if(!num) handleMMUC128(value);
  else c128pcr[num-1]=value;
}


void NCbmHandler::setCommonC128(unsigned int value)
{
  bool c=false;
  if(value&0x0c) c=true;
  //map->setCommon(c);
}


void NCbmHandler::handleMMUC264(BYTE cr)
{
  // TODO: This is nowhere near finished...I think...
  mmuCR=cr;
  if(cr) {
    // both roms mapped in...
    //map->restoreSnapshot1();
  }
  else {
    // all ram, no roms
    //map->restoreSnapshot0();
  }
}


void NCbmHandler::initMMUC264()
{
  //map->allocMapSaves();
  kernRom=(ERom *)map->findArea("kern264.rom");
  if(!kernRom) DEBUG_OUT<<"Couldn't find area named kern264.rom!\n";
  basARom=(ERom *)map->findArea("basic264.rom");
  if(!basARom) DEBUG_OUT<<"Couldn't find area named basic264.rom!\n";
  ramHandle=(EMemAbstract *)map->findArea("ram_area");
  if(!ramHandle) DEBUG_OUT<<"Couldn't find area named ram_area!\n";
  if(io_variation==1) {
    // c-16...
  }
  //map->takeSnapshot0();
  map->map(kernRom);
  map->map(basARom);
  //map->takeSnapshot1();
  mmuInited=true;
}


void NCbmHandler::initMMUC128()
{
  kernRom=(ERom *)map->findArea("kern128.rom");
  if(!kernRom) DEBUG_OUT<<"Couldn't find area named kern128.rom!\n";
  basARom=(ERom *)map->findArea("bas128a.rom");
  if(!basARom) DEBUG_OUT<<"Couldn't find area named bas128a.rom!\n";
  basBRom=(ERom *)map->findArea("bas128b.rom");
  if(!basBRom) DEBUG_OUT<<"Couldn't find area named bas128b.rom!\n";
  editRom=(ERom *)map->findArea("editor.rom");
  if(!editRom) DEBUG_OUT<<"Couldn't find area named editor.rom!\n";
  charRom=(ERom *)map->findArea("char.rom");
  if(!charRom) DEBUG_OUT<<"Couldn't find area named char.rom!\n";
  ioMem=(EMemAbstract *)map->findArea("io.d000");
  if(!ioMem) DEBUG_OUT<<"Couldn't find area named io.d000!\n";
  mmuInited=true;
  handleMMUC128(0);  // Set initial on-reset map
}


void NCbmHandler::keypress(int c)
{
  //if((e->code&0xff)==E_KEY_NMI) { if(getCPU()) getCPU()->nmi(); return; }
  //DEBUG_OUT<<c<<"\n";
  // These next two lines are initialized with nonsense for no reason
  BYTE row0=0,row1=1,row2=2,row3=3,row4=4,row5=5,row6=6,row7=7;
  BYTE col1=2,col2=3,col3=4,col4=5,col5=6,col6=7,col7=8,col8=0;
  // Remember, columns are one off...rows aren't!
  if(io_type==N_CBM_IO_B) {
    // These are wrong...
    row0=1; row1=2; row2=3; row3=8; row4=5; row5=6; row6=7; row7=4;
    col8=0; col1=7; col2=1; col3=2; col4=3; col5=4; col6=5; col7=6;
  }
  if(io_type==N_CBM_IO_VIC) {
    row0=0; row1=1; row2=2; row3=3; row4=4; row5=5; row6=6; row7=7;
    col8=8; col1=1; col2=2; col3=3; col4=4; col5=5; col6=6; col7=7;
  }
  if(io_type==N_CBM_IO_TED) {
    row0=1; row1=2; row2=3; row3=8; row4=5; row5=6; row6=7; row7=4;
    col8=0; col1=7; col2=1; col3=2; col4=3; col5=4; col6=5; col7=6;
  }
  if(io_type==N_CBM_IO_C64) {
    row0=1; row1=2; row2=3; row3=8; row4=5; row5=6; row6=7; row7=4;
    col8=0; col1=7; col2=1; col3=2; col4=3; col5=4; col6=5; col7=6;
  }
  if(io_type==N_CBM_IO_C65) {
    row0=1; row1=2; row2=3; row3=8; row4=5; row5=6; row6=7; row7=4;
    col8=0; col1=7; col2=1; col3=2; col4=3; col5=4; col6=5; col7=6;
  }
  if(io_type==N_CBM_IO_C128) {
    row0=1; row1=2; row2=3; row3=8; row4=5; row5=6; row6=7; row7=4;
    col8=0; col1=7; col2=1; col3=2; col4=3; col5=4; col6=5; col7=6;
  }
  io_keypress=1;
  BYTE col_value=0,row_value=0;
  bool rshift=false,lshift=false;
  joyKey[0]=0;  joyKey[1]=0;
  for(BYTE t=0;t<NCBM_MAX_KEY_ROWS;t++) keys[t]=0;
  key_max=8;
  switch(c) {
    case '1': row_value=row0;  col_value=col1; break;
    case '2': row_value=row7;  col_value=col1; break;
    case '3': row_value=row0;  col_value=col2; break;
    case '4': row_value=row7;  col_value=col2; break;
    case '5': row_value=row0;  col_value=col3; break;
    case '6': row_value=row7;  col_value=col3; break;
    case '7': row_value=row0;  col_value=col4; break;
    case '8': row_value=row7;  col_value=col4; break;
    case '9': row_value=row0;  col_value=col5; break;
    case '0': row_value=row7;  col_value=col5; break;
    case '-': row_value=row7;  col_value=col6; break;
    case '=': row_value=row5;  col_value=col7; break;
    case 'q': row_value=row6;  col_value=col1; break;
    case 'w': row_value=row1;  col_value=col2; break;
    case 'e': row_value=row6;  col_value=col2; break;
    case 'r': row_value=row1;  col_value=col3; break;
    case 't': row_value=row6;  col_value=col3; break;
    case 'y': row_value=row1;  col_value=col4; break;
    case 'u': row_value=row6;  col_value=col4; break;
    case 'i': row_value=row1;  col_value=col5; break;
    case 'o': row_value=row6;  col_value=col5; break;
    case 'p': row_value=row1;  col_value=col6; break;
    case 'a': row_value=row2;  col_value=col2; break;
    case 's': row_value=row5;  col_value=col2; break;
    case 'd': row_value=row2;  col_value=col3; break;
    case 'f': row_value=row5;  col_value=col3; break;
    case 'g': row_value=row2;  col_value=col4; break;
    case 'h': row_value=row5;  col_value=col4; break;
    case 'j': row_value=row2;  col_value=col5; break;
    case 'k': row_value=row5;  col_value=col5; break;
    case 'l': row_value=row2;  col_value=col6; break;
    case ';': row_value=row2;  col_value=col7; break;
    case 'z': row_value=row4;  col_value=col2; break;
    case 'x': row_value=row3;  col_value=col3; break;
    case 'c': row_value=row4;  col_value=col3; break;
    case 'v': row_value=row3;  col_value=col4; break;
    case 'b': row_value=row4;  col_value=col4; break;
    case 'n': row_value=row3;  col_value=col5; break;
    case 'm': row_value=row4;  col_value=col5; break;
    case ',': row_value=row3;  col_value=col6; break;
    case '.': row_value=row4;  col_value=col6; break;
    case '/': row_value=row3;  col_value=col7; break;
    //case A_KEY_BACKSPACE: row_value=row0;  col_value=col8; break;
    //case A_KEY_NEWLINE: row_value=row1;  col_value=col8; break;
    case ' ': row_value=row4;  col_value=col1; break;
    case '!': row_value=row0;  col_value=col1; rshift=true; break;
    case '@': row_value=row6;  col_value=col6; break;
    case '#': row_value=row0;  col_value=col2; lshift=true; break;
    case '$': row_value=row7;  col_value=col2; lshift=true; break;
    case '%': row_value=row0;  col_value=col3; rshift=true; break;
    case '&': row_value=row7;  col_value=col3; rshift=true; break;
    case '*': row_value=row1;  col_value=col7; break;
    case ')': row_value=row0;  col_value=col5; rshift=true; break;
    case '(': row_value=row7;  col_value=col4; rshift=true; break;
    case '+': row_value=row0;  col_value=col6; break;
    case 'Q': row_value=row6;  col_value=col1; rshift=true; break;
    case 'W': row_value=row1;  col_value=col2; rshift=true; break;
    case 'E': row_value=row6;  col_value=col2; rshift=true; break;
    case 'R': row_value=row1;  col_value=col3; rshift=true; break;
    case 'T': row_value=row6;  col_value=col3; rshift=true; break;
    case 'Y': row_value=row1;  col_value=col4; rshift=true; break;
    case 'U': row_value=row6;  col_value=col4; rshift=true; break;
    case 'I': row_value=row1;  col_value=col5; rshift=true; break;
    case 'O': row_value=row6;  col_value=col5; rshift=true; break;
    case 'P': row_value=row1;  col_value=col6; rshift=true; break;
    case 'A': row_value=row2;  col_value=col2; rshift=true; break;
    case 'S': row_value=row5;  col_value=col2; rshift=true; break;
    case 'D': row_value=row2;  col_value=col3; rshift=true; break;
    case 'F': row_value=row5;  col_value=col3; rshift=true; break;
    case 'G': row_value=row2;  col_value=col4; rshift=true; break;
    case 'H': row_value=row5;  col_value=col4; rshift=true; break;
    case 'J': row_value=row2;  col_value=col5; rshift=true; break;
    case 'K': row_value=row5;  col_value=col5; rshift=true; break;
    case 'L': row_value=row2;  col_value=col6; rshift=true; break;
    case ':': row_value=row5;  col_value=col6; break;
    case 'Z': row_value=row4;  col_value=col2; lshift=true; break;
    case 'X': row_value=row3;  col_value=col3; rshift=true; break;
    case 'C': row_value=row4;  col_value=col3; lshift=true; break;
    case 'V': row_value=row3;  col_value=col4; rshift=true; break;
    case 'B': row_value=row4;  col_value=col4; lshift=true; break;
    case 'N': row_value=row3;  col_value=col5; rshift=true; break;
    case 'M': row_value=row4;  col_value=col5; lshift=true; break;
    case '<': row_value=row3;  col_value=col6; lshift=true; break;
    case '>': row_value=row4;  col_value=col6; rshift=true; break;
    case '?': row_value=row3;  col_value=col7; rshift=true; lshift=true; break;
    case '[': row_value=row5;  col_value=col6; lshift=true; break;
    case ']': row_value=row2;  col_value=col7; rshift=true; break;
    case '\'': row_value=row0;  col_value=col4; lshift=true; break;
    case '`': row_value=row1;  col_value=col1; break;
    case '~': row_value=row1;  col_value=col1; rshift=true; break;
    case '"': row_value=row7;  col_value=col1; rshift=true; break;
    //case '\\': row_value=row6;  col_value=col7; break;
    case '^': row_value=row6;  col_value=col7; break;
    case '|': row_value=row6;  col_value=col7; lshift=true; break;
/*
    case A_KEY_F1: row_value=row4;  col_value=col8; break;
    case A_KEY_F2: row_value=row4;  col_value=col8; rshift=true; break;
    case A_KEY_F3: row_value=row5;  col_value=col8; break;
    case A_KEY_F4: row_value=row5;  col_value=col8; rshift=true; break;
    case A_KEY_F5: row_value=row6;  col_value=col8; break;
    case A_KEY_F6: row_value=row6;  col_value=col8; rshift=true; break;
    case A_KEY_F7: row_value=row7;  col_value=col8; break;
    case A_KEY_F8: row_value=row7;  col_value=col8; rshift=true; break;
    case A_KEY_RIGHT: row_value=row2;  col_value=col8; break;
    case A_KEY_DOWN: row_value=row3;  col_value=col8; break;
    case A_KEY_LEFT: row_value=row2;  col_value=col8; rshift=true; break;
    case A_KEY_UP: row_value=row3;  col_value=col8; lshift=true; break;
    case A_KEY_TAB: row_value=row3;  col_value=col1; break;
*/
    /*
    case A_KEY_JOY_UP:
    case A_KEY_JOY_DOWN:
    case A_KEY_JOY_LEFT:
    case A_KEY_JOY_RIGHT:
    case A_KEY_JOY_FIRE:
      joyKey[(e->number)&1]=e->code;
      //key_max=8;
      break;
    */
    default: key_max=0; break;
  }
  //DEBUG_OUT<<"keypress: "<<(short)row_value<<","<<(short)col_value<<"\n";
  switch(io_type) {
    case N_CBM_IO_VIC:
      if(rshift) keys[row3]=col2;
      if(lshift) keys[row4]=col7;
      if(row_value||col_value) keys[row_value]=col_value;
      break;
    case N_CBM_IO_TED:
      // Take care of how c264 differs from c64
      switch(c) {
/*
        case A_KEY_UP: row_value=row7;  col_value=col6; break;
        case A_KEY_DOWN: row_value=row0;  col_value=col6; break;
        case A_KEY_LEFT: row_value=row0;  col_value=col7; break;
        case A_KEY_RIGHT: row_value=row7;  col_value=col7; break;
*/
        case '*': row_value=row1;  col_value=col7; break;
        case '-': row_value=row6;  col_value=col6; break;
        case '+': row_value=row6;  col_value=col7; break;
        case '@': row_value=row3;  col_value=col8; break;
        default: break;
      }
      if(lshift) keys[col2]=row3;
      if(rshift) keys[col2]=row3;
      if(row_value||col_value) keys[col_value]=row_value;
      break;
    case N_CBM_IO_PET:
      if(rshift) keys[col2]=row3;
      if(lshift) keys[col7]=row4;
      if(row_value||col_value) keys[col_value]=row_value;
      break;
    case N_CBM_IO_C65:
    case N_CBM_IO_C128:
    case N_CBM_IO_B:
    case N_CBM_IO_C64:
      if(rshift) keys[col2]=row3;
      if(lshift) keys[col7]=row4;
      if(row_value||col_value) keys[col_value]=row_value;
      break;
    default: break;
  }
  key_count=key_max;
}


// NOTE: Assumes 256 pages of 256 bytes each
bool NCbmHandler::read8ret(ULONG addr,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    case 0:
      if(addr==0) { *ret=read8ZeroOne(addr); return true; }
      if(addr==1) { *ret=read8ZeroOne(addr); return true; }
      return false;
      break;
    default:
      return false;
      break;
  }
  return false;
}


// NOTE: Assumes 256 pages of 256 bytes each
bool NCbmHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    case 0:
      if(addr==0) { *ret=write8ZeroOne(addr,val); return true; }
      if(addr==1) { *ret=write8ZeroOne(addr,val); return true; }
      return false;
      break;
    default:
      return false;
      break;
  }
  return false;
}


BYTE NCbmHandler::read8ZeroOne(ULONG address)
{
  BYTE page=(address>>8)&0xff;
  BYTE adr=address&0xff;
  BYTE value=0;
  switch(adr) {
    case 0x0:
      value=value0;
      break;
    case 0x1:
      value=value1;
      switch(io_type) {
       case N_CBM_IO_C65:
       case N_CBM_IO_C64:
          value=0x20;  // TODO: Where did I get this from?
          if(loram) value|=1;
          if(hiram) value|=2;
          if(charen) value|=4;
          break;
        case N_CBM_IO_B:
          break;
        case N_CBM_IO_TED:
          break;
        default:
          //DEBUG_OUT<<"I/O read  at "<<address<<"\n";
          break;
      }
      break;
    default:
      break;
  }
  return value;
}


BYTE NCbmHandler::write8ZeroOne(ULONG address, BYTE value)
{
  EMemAbstract *foo=(EMemAbstract *)NULL;
  BYTE page=(address>>8)&0xff;
  BYTE adr=address&0xff;
  BYTE loramHiram=0;
  switch(adr) {
    case 0x0:  // DDR in 6510 and friends, exec bank in 6509
      value0=value;
      switch(io_type) {
       case N_CBM_IO_B:
          // TODO: set exec bank
          DEBUG_OUT<<"ExecBank set to "<<aDefaultStringHelper.myHex((unsigned char)value)<<"\n";
          if(cpu) { cpu->setMachineSpecificRegister(0,value); }
          break;
        default:
          break;
      }
      break;
    case 0x1:
      value1=value;
      switch(io_type) {
        case N_CBM_IO_B:
          // TODO: set indirect bank
          DEBUG_OUT<<"IndirBank set to "<<aDefaultStringHelper.myHex((unsigned char)value)<<"\n";
          if(cpu) { cpu->setMachineSpecificRegister(1,value); }
          break;
        case N_CBM_IO_C65:
          if(!getC64Mode()) {
            DEBUG_OUT<<"Write of "<<aDefaultStringHelper.myHex((unsigned char)value)<<" to 0x0001\n";
          }
          // Fall through to C64 version...
          // break;
       case N_CBM_IO_C64:
          if(value&0x01) loram=true; else loram=false;
          if(value&0x02) hiram=true; else hiram=false;
          if(value&0x04) charen=true; else charen=false;
          loramHiram=hiram&0x01;
          if(loram) loramHiram|=0x02;
          switch(loramHiram) {
            case 0x00:
              // both loram and hiram off
              // No basic...
              foo=map->findArea("basic64.rom");
              if(foo) map->unmap(foo);
              // No kern...
              foo=map->findArea("kern64.rom");
              if(foo) map->unmap(foo);
              // No IO...
              foo=map->findArea("io.d000");
              if(foo) map->unmap(foo);
              // No char...
              foo=map->findArea("char.rom");
              if(foo) map->unmap(foo);
              // Yes ram...
              ramHandle=map->findArea("ram");
              map->map(ramHandle);
              break;
            case 0x01:
              // loram off, hiram on
              // No basic...
              foo=map->findArea("basic64.rom");
              if(foo) map->unmap(foo);
              // Yes ram...
              ramHandle=map->findArea("ram");
              map->map(ramHandle);
              // Yes kern...
              foo=map->findArea("kern64.rom");
              if(foo) map->map(foo);
              // Yes either io or char
              if(charen) foo=map->findArea("io.d000");
              else foo=map->findArea("char.rom");
              if(foo) map->map(foo);
              break;
            case 0x02:
              // loram on, hiram off
              // No basic...
              foo=map->findArea("basic64.rom");
              if(foo) map->unmap(foo);
              // No kern...
              foo=map->findArea("kern64.rom");
              if(foo) map->unmap(foo);
              // Yes ram...
              ramHandle=map->findArea("ram");
              map->map(ramHandle);
              // Yes either io or char
              if(charen) foo=map->findArea("io.d000");
              else foo=map->findArea("char.rom");
              if(foo) map->map(foo);
              break;
            case 0x03:
              // both loram and hiram on
              // Yes basic...
              foo=map->findArea("basic64.rom");
              if(foo) map->map(foo);
              // Yes kern...
              foo=map->findArea("kern64.rom");
              if(foo) map->map(foo);
              // Yes either io or char
              if(charen) foo=map->findArea("io.d000");
              else foo=map->findArea("char.rom");
              if(foo) map->map(foo);
              break;
            default:
              cpu->debugger("bad value for loramHiram!\n");
              break;
          }
          break;
        case N_CBM_IO_TED:
          break;
        default:
          //DEBUG_OUT<<"Write of "<<aDefaultStringHelper.myHex((unsigned char)value)<<" to 0x0001\n";
          break;
      }
      break;
    default:
      break;
  }
  return value;
}
