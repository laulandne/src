
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NMacHandler.h>
#include <emu/NMacIO.h>
#include <emu/NTinyHandler.h>
#include <emu/generated/NMac_traps_tab.h>
#include <emu/generated/NMac_globals_tab.h>

#include <cpulib/Em68k.h>
#include <cpulib/EPowerPC.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOVia6522.h>
#include <elib/EVideoIOBitmap.h>
#include <elib/EIOScc8530.h>

#include <disklib/DControllerIWM.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


/*

Rough Mac Plus memory map:

00 0000 - 40 0000  ram (rom shadow at boot)
40 0000 - 50 0000  rom
50 0000 - 58 0000  empty?
58 0000 - 60 0000  scsi
60 0000 - 70 0000  (ram shadow at boot)
70 0000 - 80 0000  empty?
80 0000 - a0 0000  scc read
a0 0000 - c0 0000  scc read
c0 0000 - e0 0000  iwm
e0 0000 - e8 0000  empty?
e8 0000 - f0 0000  via
f0 0000 - ff ffff  empty?

*/


/*

Rough Mac generic 68k 24-bit memory map:

00 0000 - 80 0000  ram
80 0000 - 90 0000  rom
90 0000 - f0 0000  nubus space
f0 0000 - ff ffff  io space

*/


/*

Rough Mac generic 68k 32-bit memory map:

0000 0000 - 4000 0000  ram
4000 0000 - 5000 0000  rom
5000 0000 - 6000 0000  io space
6000 0000 - f000 0000  nubus super space
f000 0000 - ffff ffff  nubus space

*/


////////////////////////////////////////////////////////////////////////////////
//  NMacHandler Class
////////////////////////////////////////////////////////////////////////////////

NMacHandler::NMacHandler()
{
  DEBUG_OUT<<"NMacHandler constructor.\n";
  iPPC=false;
  PCI=false;
  CFM=false;
  sysZone.init();  appZone.init();
  stackMax=0;
  didGlobalSymbols=false;
  romName="mac.rom";
  romSize=0;
}


NMacHandler::~NMacHandler()
{
  DEBUG_OUT<<"NMacHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


unsigned int NMacHandler::name2type(const char *name)
{
  std::cerr<<"NMacHandler name2type not implemented!\n"; exit(5);
}


/* STATIC */
bool NMacHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"mac68k")) return true;
  if(!strcmp(name,"powermac")) return true;
  if(!strcmp(name,"quadra950")) return true;
  if(!strcmp(name,"quadra900")) return true;
  if(!strcmp(name,"quadra800")) return true;
  if(!strcmp(name,"quadra700")) return true;
  if(!strcmp(name,"centris610")) return true;
  if(!strcmp(name,"centris650")) return true;
  if(!strcmp(name,"powerbook540")) return true;
  if(!strcmp(name,"powerbook520")) return true;
  if(!strcmp(name,"mac2fx")) return true;
  if(!strcmp(name,"mac2vx")) return true;
  if(!strcmp(name,"mac2cx")) return true;
  if(!strcmp(name,"mac2si")) return true;
  if(!strcmp(name,"mac2ci")) return true;
  if(!strcmp(name,"mac2x")) return true;
  if(!strcmp(name,"mac2")) return true;
  if(!strcmp(name,"macse30")) return true;
  if(!strcmp(name,"maclc3")) return true;
  if(!strcmp(name,"maclc2")) return true;
  if(!strcmp(name,"maclc")) return true;
  if(!strcmp(name,"macse")) return true;
  if(!strcmp(name,"macclassic")) return true;
  if(!strcmp(name,"macplus")) return true;
  return false;
}


const char *NMacHandler::getName()
{
  if(io_type==N_MAC_IO_PLUS) return "Mac Plus"; else
  if(io_type==N_MAC_IO_GENERIC24) return "Mac (Generic 24-bit mode)"; else
  if(io_type==N_MAC_IO_GENERIC32) return "Mac (Generic 32-bit mode)"; else
  if(io_type==N_MAC_IO_POWERMAC_PCI) return "PowerMac (PCI)"; else
  if(io_type==N_MAC_IO_POWERMAC_NUBUS) return "PowerMac (NuBus)"; else
  if(io_type==N_MAC_IO_MAC68k_NUBUS) return "Mac (NuBus)"; else
  return "Mac (Unknown I/O)";
}


// Note: No else on last one...
void NMacHandler::setKind(const char *tname)
{
  io_type=0;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"mac68k")) io_type=N_MAC_IO_MAC68k_NUBUS;
  if(!strcmp(name,"powermac")) io_type=N_MAC_IO_POWERMAC_NUBUS;
  if(!strcmp(name,"quadra950")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"quadra900")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"quadra700")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"quadra650")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"quadra800")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"centris610")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"centris650")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"powerbook540")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"powerbook520")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"mac2fx")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"mac2cx")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"mac2vx")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"mac2si")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"mac2ci")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"mac2x")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"mac2")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"macse30")) io_type=N_MAC_IO_GENERIC32;
  if(!strcmp(name,"se30")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"maclc3")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"maclc2")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"maclc")) io_type=N_MAC_IO_GENERIC24;
  if(!strcmp(name,"macclassic")) io_type=N_MAC_IO_PLUS;
  if(!strcmp(name,"macse")) io_type=N_MAC_IO_PLUS;
  if(!strcmp(name,"macplus")) io_type=N_MAC_IO_PLUS;
  io_init(io_type);
}


void NMacHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_MAC_IO_PLUS:
    case N_MAC_IO_GENERIC24:
    case N_MAC_IO_GENERIC32:
    case N_MAC_IO_MAC68k_NUBUS:
      iPPC=false; PCI=false;
      myIO=new NMacIO(this);
      myVideo=new EVideoIOBitmap(this,20,20);
      break;
    case N_MAC_IO_POWERMAC_NUBUS:
      iPPC=true; PCI=false;
      myIO=new NMacIO(this);
      myVideo=new EVideoIOBitmap(this,20,20);
      break;
    case N_MAC_IO_POWERMAC_PCI:
      iPPC=true; PCI=true;
      myIO=new NMacIO(this);
      myVideo=new EVideoIOBitmap(this,20,20);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_MAC_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  if(!didGlobalSymbols) addGlobalSymbols();
  romOverlay=true;
}


bool NMacHandler::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool done=false;
  if(!strcmp("heap",cmd)) { showHeaps(); done=true; }
  if(!strcmp("heaps",cmd)) { showHeaps(); done=true; }
  if(!strcmp("zone",cmd)) { showZones(); done=true; }
  if(!strcmp("zones",cmd)) { showZones(); done=true; }
  return done;
}


void NMacHandler::showZones()
{
  //if(!w) return;
  //DEBUG_OUT<<"sysZone ";  sysZone.print(w);
  //DEBUG_OUT<<"appZone ";  appZone.print(w);
}


void NMacHandler::showHeaps()
{
  //if(!w) return;
  DEBUG_OUT<<"Heaps not implemented yet!\n";
  //DEBUG_OUT<<"sysHeap ";  sysHeap.print(w);
  //DEBUG_OUT<<"appHeap ";  appHeap.print(w);
}


bool NMacHandler::ioDisHelper(ULONG addr)
{
  bool ret=false;
  bool searching=true;
  unsigned int t=0;
  UINT16 trap=map->read16b(addr);
  while(searching) {
    if(trap==NMac_TrapWord[t]) {
      DEBUG_OUT<<NMac_TrapName[t];
      searching=false;
      ret=true;
    }
    else t++;
    if(!NMac_TrapWord[t]) searching=false;
  }
  return ret;
}


bool NMacHandler::systemCall(unsigned int num)
{
  bool ret=false;
  if(num&0xa000) {
    // its an A-line trap...
    switch(num) {
      case 0xa9f4:  // _ExitToShell
        debugger("ExitToShell");
        break;
      default:
        debugger("Unimplemented Mac A-line toolbox call!");
        break;
    }
    ret=true;
  }
  return ret;
}


void NMacHandler::my_reset()
{
  Em68kHandler::my_reset();
}


void NMacHandler::my_reset_postCPU()
{
  setupMacWorld();
  if(cpu) {
    if(!cpu->getTrapHandler()) {
      NTinyHandler *foo=new NTinyHandler(this);
      if(foo) cpu->setTrapHandler(foo);
    }
  }
}


// NOTE: You should only call this once the CPU is set up also...
void NMacHandler::setupMacWorld()
{
  DEBUG_OUT<<"setupMacWorld()\n";
  // set up a Mac-style runtime environment
  unsigned int bottom=0,top=0;
  unsigned int flags=0;
  unsigned int preferedMem=0,minMem=0;
  unsigned int theEntry=0;
  // look for a SIZE resource and set the prefered memory if we've got it
  EModule *mod=map->getFirstModule();
  EMapSymbol *size=NULL;
  if(mod) size=mod->lookupNodeByName("SIZE");
  if(size) {
    flags=map->read16b(size->addr);
    preferedMem=map->read32b(size->addr+2);
    minMem=map->read32b(size->addr+6);
  }
  // look for a cfrg #0, if we have it we're in cfm mode
  EMapSymbol *cfrg=NULL;
  if(mod) cfrg=mod->lookupNodeByNameAndNum("cfrg",0);
  if(cfrg) {
    CFM=true;
    // TODO: handle all PEF nodes and set bottom and top accordingly...
  }
  else {
    if(iPPC) std::cerr<<"Couldn't find PPC code frags!\n";
  }
  // if we're not in CFM mode, we're in 68k classic mode, find all CODE
  // and handle the jumptable
  EMapSymbol *jumpTable=NULL;
  if(mod) jumpTable=mod->lookupNodeByNameAndNum("CODE",0);
  if(!CFM) {
    EMapSymbol *sym=NULL;
    if(mod) sym=mod->lookupNodeByName("CODE");
    bool working=true;
    while(working)
    {
      if(sym) {
        DEBUG_OUT<<"Found CODE "<<sym->num<<": "<<sym->size;
        DEBUG_OUT<<"@"<<sym->addr<<"\n";
        if(!bottom) bottom=sym->addr;
        top=sym->addr+sym->size;
      }
      else working=false;
      if(mod) sym=mod->lookupNextNodeByName("CODE");
    }
    if(jumpTable) {
      // TODO: set up jumptable and get theEntry
    }
    else {
      std::cerr<<"Couldn't find m68k jumpTable!\n";
      return;
    }
  }
  DEBUG_OUT<<"CODE from "<<bottom<<" to "<<top<<"\n";
  DEBUG_OUT<<"flags: "<<flags<<"\n";
  DEBUG_OUT<<"minMem: "<<minMem<<"\n";
  DEBUG_OUT<<"preferedMem: "<<preferedMem<<"\n";
  DEBUG_OUT<<"theEntry: "<<theEntry<<"\n";
  sysZone.loc=0x1400;  // may be wrong
  sysZone.len=bottom-sysZone.loc;
  appZone.loc=top;
  appZone.len=bottom+preferedMem-appZone.loc;
  stackMax=appZone.getMax();
  unsigned int bottomWorld=appZone.getMax();
  if(!CFM) {
    // relocate jump table to top of application partition
    bottomWorld-=jumpTable->size;
    for(unsigned int t=0;t<jumpTable->size;t++)
      map->write8(bottomWorld+t,map->read8(jumpTable->addr+t));
    // compute size of globals and a5 world...
    // TODO: where can I get info on the globals?
    unsigned int a5Size=32*1024;  // arbitrary for now
    stackMax+=jumpTable->size-a5Size;
  }
  // set up some of the low mem globals
  map->write32b(0x2a6,sysZone.loc);  // SysZone
  map->write32b(0xaaa,appZone.loc);  // ApplZone
  map->write32b(0x908,stackMax);  // CurStackBase
  map->write32b(0x10c,appZone.getMax());  // BufPtr
  map->write32b(0x108,appZone.getMax()+(64*1024));  // MemTop
  if(cpu) {
    DEBUG_OUT<<"Setting cpu registers...\n";
    cpu->setResetStack(stackMax);
    if(iPPC) {
      // PPC to m68k regs for mac
      // gpr1=stack
      // gpr2=TOC
      // gpr3=func return val
      // gpr3-10=params
      // gpr12=transition vector
      // gpr13-31=local vars
      // ????=m68k context
    }
    else {
      cpu->setRegister("a6",stackMax);
      cpu->setRegister("a5",bottomWorld-32);
    }
    if(theEntry) cpu->setRegister("pc",theEntry);
  }
  map->write32b(0x904,bottomWorld-32);  // CurrentA5
}


void NMacHandler::addGlobalSymbols()
{
  if(didGlobalSymbols) return;
  didGlobalSymbols=true;
  EModule *mod=map->getFirstModule();
  if(!mod) return;
  mod->getSymbols()->reset();
  bool working=true;
  EMapSymbol *s=(EMapSymbol *)NULL;
  unsigned int t=0;
  while(working) {
    if(NMac_GlobalName[t]) {
      s=new EMapSymbol(strdup(NMac_GlobalName[t]),t,NMac_GlobalAddress[t],
        NMac_GlobalSize[t],EMAP_SYMBOL_NORMAL);
      if(s) mod->getSymbols()->insert(s);
      t++;
    }
    else working=false;
  }
}


bool NMacHandler::read8ret(ULONG addr,BYTE *ret)
{
  if(Em68kHandler::read8ret(addr,ret)) return true;
  unsigned int apage=(addr>>16);
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}


bool NMacHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  if(Em68kHandler::write8ret(addr,val,ret)) return true;
  unsigned int apage=(addr>>16);
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}
