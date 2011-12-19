
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include "nemu.h"

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/AWindowCharMapped.h>
//#include <guilib/ADialogMsg.h>

#include <portable/nickcpp/AClock.h>
#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/AStringHelper.h>

#include <cpulib/ECPUAbstract.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EResourceFile.h>
#include <elib/EResource.h>
#include <elib/EVideoIOAbstract.h>
#include <elib/EHandler.h>
#include <elib/ERom.h>
#include <elib/ERam.h>

#include <disklib/DVfs.h>

#include <binarylib/NLd.h>


static char *errMsgNoMap="Can't, no Map!";
static char *errMsgNoVfs="Can't, no VFS!";
static char *errMsgCmdErr="Command error!";
static char *errMsgMissParam="Missing parameter!";


////////////////////////////////////////////////////////////////////////////////
//  NCmd Class
////////////////////////////////////////////////////////////////////////////////

NCmd::NCmd()
{
  //std::cerr<<"NCmd constructor.\n";
  //
  init();
}


void NCmd::init()
{
  debug_pointer=0;
  debug_buffer[0]=0;
  last_debug_buffer[0]=0;
  last_debug_pointer=0;
  debug_args=0;
  for(unsigned int t=0;t<10;t++) debug_arg[t]=(char *)NULL;
  debug_loc=0;
  // All these are for parsing EMU's info files...
  inComment=false;
  inBracket=false;
  cmdError=false;
  foundIt=false;
  loadingName=(char *)NULL;
  curLoadAddr=0;
  sourcing=false;
  aDefaultStringHelper.setHexMode();
  outputCount=0;
  tempmem=(unsigned char *)NULL;
  tempsize=0;
  defaultNumLines=12;
};


NCmd::~NCmd()
{
  std::cerr<<"NCmd destructor.\n";
  if(!err.error()) {
    if(tempmem) tempfree();
    tempmem=(unsigned char *)NULL;
  }
}


void NCmd::loadcolorrom()
{
  if(!debug_arg[0]) { DEBUG_OUT<<"missing param!\n"; return; }
  if(!debug_arg[1]) { DEBUG_OUT<<"missing param!\n"; return; }
  if(!debug_arg[2]) { DEBUG_OUT<<"missing param!\n"; return; }
  unsigned int ncolors=evaluate(debug_arg[1]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  unsigned int nbits=evaluate(debug_arg[2]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if(emu.myvideo) emu.myvideo->readRomPalette(debug_arg[0],ncolors,nbits);
}


void NCmd::printSymbol(EMapSymbol *s)
{
  if(!s) { DEBUG_OUT<<"no symbol!\n"; return; }
  DEBUG_OUT<<"\""<<s->getName()<<"\"@"<<s->addr;
  DEBUG_OUT<<" n="<<s->num<<" s="<<s->size<<" t="<<s->type<<"\n";
  switch(s->type) {
    case EMAP_SYMBOL_FUNC:
      DEBUG_OUT<<s->getDemangledName()<<" is a function.\n";
      break;
    case EMAP_SYMBOL_INT:
      DEBUG_OUT<<s->getDemangledName()<<" is an integer.\n";
      break;
    case EMAP_SYMBOL_POINTER:
      DEBUG_OUT<<s->getDemangledName()<<" is an pointer.\n";
      break;
    default: break;
  }
}


// TODO: Change to be specific to node
void NCmd::printNode(NBinaryNode *n)
{
  if(!n) { DEBUG_OUT<<"no node!\n"; return; }
  DEBUG_OUT<<"\""<<n->getName()<<"\" addr="<<n->addr;
  DEBUG_OUT<<" num="<<n->num<<" size="<<n->size<<" type="<<n->type<<"\n";
}


// TODO: Change to be specific to module
void NCmd::printModule(EModule *m)
{
  if(!m) { DEBUG_OUT<<"no module!\n"; return; }
  DEBUG_OUT<<"\""<<m->getName()<<"\" addr="<<m->addr;
  DEBUG_OUT<<" num="<<m->num<<" size="<<m->size<<" type="<<m->type<<"\n";
}


void NCmd::symbols()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  AList *syms=emu.getMap()->getSymbols();
  symbols_list(syms);
  EModule *mod=emu.getMap()->getFirstModule();
  if(!mod) return;
  syms=mod->getSymbols();
  symbols_list(syms);
}


void NCmd::symbols_list(AList *syms)
{
  if(!syms) { DEBUG_OUT<<"No symbols!\n"; return; }
  //if(w->getUseMore()) w->startMore();
  DEBUG_OUT<<"Symbols:\n";
  syms->reset();
  if(syms->atEnd()) return;
  EMapSymbol *s=(EMapSymbol *)NULL;
  bool working=true;
  while(working) {
    if(syms->atEnd()) working=false;
    else {
      s=(EMapSymbol *)syms->info();
      printSymbol(s);
      syms->advance();
    }
  }
  //if(w->getUseMore()) w->endMore();
}


void NCmd::strings()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  EModule *mod=emu.getMap()->getFirstModule();
  if(!mod) return;
  AList *strs=mod->getStrings();
  if(!strs) { DEBUG_OUT<<"No strings!\n"; return; }
  //if(w->getUseMore()) w->startMore();
  DEBUG_OUT<<"Strings:\n";
  strs->reset();
  if(strs->atEnd()) return;
  EMapSymbol *s=(EMapSymbol *)NULL;
  bool working=true;
  while(working) {
    if(strs->atEnd()) working=false;
    else {
      s=(EMapSymbol *)strs->info();
      printSymbol(s);
      strs->advance();
    }
  }
  //if(w->getUseMore()) w->endMore();
}


void NCmd::nodes()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  EModule *mod=emu.getMap()->getFirstModule();
  if(!mod) return;
  AList *nodes=mod->getNodes();
  if(!nodes) { DEBUG_OUT<<"No nodes!\n"; return; }
  //if(w->getUseMore()) w->startMore();
  DEBUG_OUT<<"nodes:\n";
  nodes->reset();
  if(nodes->atEnd()) return;
  EMapSymbol *s=(EMapSymbol *)NULL;
  bool working=true;
  while(working) {
    if(nodes->atEnd()) working=false;
    else {
      s=(EMapSymbol *)nodes->info();
      printNode((NBinaryNode *)s);
      nodes->advance();
    }
  }
  //if(w->getUseMore()) w->endMore();
}


void NCmd::modules()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  AList *mods=emu.getMap()->getModules();
  if(!mods) { DEBUG_OUT<<"No modules!\n"; return; }
  //if(w->getUseMore()) w->startMore();
  DEBUG_OUT<<"modules:\n";
  mods->reset();
  if(mods->atEnd()) return;
  EModule *m=(EModule *)NULL;
  bool working=true;
  while(working) {
    if(mods->atEnd()) working=false;
    else {
      m=(EModule *)mods->info();
      printModule(m);
      mods->advance();
    }
  }
  //if(w->getUseMore()) w->endMore();
}


void NCmd::symbol()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<"Please supply an address.\n"; return; }
  ULONG addr=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  EModule *mod=emu.getMap()->getFirstModule();
  if(!mod) return;
  EMapSymbol *s=mod->findNearestSymbol(addr);
  if(s) printSymbol(s);
  else DEBUG_OUT<<"No symbol near that address!\n";
}


void NCmd::node()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<"Please supply an address.\n"; return; }
  ULONG addr=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  EModule *mod=emu.getMap()->getFirstModule();
  if(!mod) return;
  EMapSymbol *s=mod->findNearestPrevNode(addr);
  if(s) printNode((NBinaryNode *)s);
  else DEBUG_OUT<<"No node near that address!\n";
}


void NCmd::module()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<"Please supply an address.\n"; return; }
  ULONG addr=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  EModule *m=emu.getMap()->findNearestPrevModule(addr);
  if(m) printModule(m);
  else DEBUG_OUT<<"No module near that address!\n";
}


void NCmd::print()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  unsigned int num=0;
  bool lookupWithNum=false;
  char *theArg=debug_arg[0];
  char *theNumArg=debug_arg[1];
  if(theNumArg) { lookupWithNum=true; num=evaluate(theNumArg); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  EMapSymbol *sym=(EMapSymbol *)NULL;
  DEBUG_OUT<<"(looking for symbol named "<<theArg<<")\n";
  if(lookupWithNum) DEBUG_OUT<<"(symbol num "<<num<<")\n";
  EModule *mod=emu.getMap()->getFirstModule();
  if(mod) {
    DEBUG_OUT<<"(Got mod)\n";
    if(!lookupWithNum) sym=mod->lookupSymbolByName(theArg);
    else sym=mod->lookupSymbolByNameAndNum(theArg,num);
    if(!sym) {
      DEBUG_OUT<<"(Going to try uppercase)\n";
      char *theArgLC=aDefaultStringHelper.toUpper(theArg);
      if(!lookupWithNum) sym=mod->lookupSymbolByName(theArgLC);
      else sym=mod->lookupSymbolByNameAndNum(theArgLC,num);
    }
  }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if(sym) printSymbol(sym);
  else DEBUG_OUT<<evaluate(theArg)<<"\n";
}


void NCmd::tmem()
{
  DEBUG_OUT<<"tmem Uniplemented!\n";
}


void NCmd::more()
{
  //bool oldMore=w->getUseMore();
  //w->setUseMore(true);
  //w->startMore();
  cat();
  //w->endMore();
  //w->setUseMore(oldMore);
}


void NCmd::cat()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  void *fin=vfs->Fopen(debug_arg[0],"r");
  if(fin) {
    bool reading=true;
    char c;
    while(reading) {
      int num=vfs->Fread((void *)&c,1,1,fin);
      if(num!=1) reading=false;
      else DEBUG_OUT<<c;
    }
    vfs->Fclose(fin);
  }
  else {
    DEBUG_OUT<<"No such file as '"<<debug_arg[0]<<"'!\n";
  }
}


void NCmd::rm()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  if(!vfs->Remove(debug_arg[0])) {
    DEBUG_OUT<<"Couldn't rm '"<<debug_arg[0]<<"'!\n";
  }
}


void NCmd::mv()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  if(!vfs->Rename(debug_arg[0],debug_arg[1]))  {
    DEBUG_OUT<<"Couldn't mv '"<<debug_arg[0]<<"'!\n";
  }
}


void NCmd::cp()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  if(!vfs->CopyFile(debug_arg[0],debug_arg[1])) {
    DEBUG_OUT<<"Couldn't cp '"<<debug_arg[0]<<"'!\n";
  }
}


void NCmd::ls()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  vfs->ls(debug_arg[0]);
}


void NCmd::cd()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  bool ret=vfs->Chdir(debug_arg[0]);
  char cwd[256];
  vfs->Getcwd(cwd,256);
  DEBUG_OUT<<cwd<<"\n";
  if(!ret) DEBUG_OUT<<"Error: Couldn't change directory!\n";
}


void NCmd::rb()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  unsigned int blk=0,addr=0;
  if(debug_arg[0]) blk=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if(debug_arg[1]) addr=evaluate(debug_arg[1]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  vfs->readBlock(blk);
  if(vfs->getDiskError()) { DEBUG_OUT<<"A disk error occured!\n"; return;  }
  unsigned int bsize=vfs->getBlockSize();
  BYTE *buf=vfs->getBuffer();
  if(buf) {
    for(unsigned int t=0;t<bsize;t++) emu.getMap()->write8(addr+t,buf[t]);
  }
  else { DEBUG_OUT<<"Couldn't get disk buffer!\n"; return;  }
  DEBUG_OUT<<"Read block "<<blk<<" to address "<<addr<<"\n";
}


void NCmd::ld()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap;  return;  }
  if(!debug_arg[0]) { DEBUG_OUT<<"Need to specify a name!\n"; return; }
  emu.ld->ld(debug_arg[0]);
  if(!emu.ld->getUnknown()) {
    if(emu.getCPU()) {
      emu.getCPU()->setResetEntry(emu.ld->getEntry());
      emu.getCPU()->jump(emu.ld->getEntry());
    }
  }
}


void NCmd::nm()
{
  //if(w->getUseMore()) w->startMore();
  emu.ld->nm(debug_arg[0]);
  //if(w->getUseMore()) w->endMore();
}


void NCmd::mount()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) {  DEBUG_OUT<<errMsgNoVfs;  return;  }
  if(!debug_arg[0]) {
    // NOTE: With no parameter act like Unix and show all mounted disks...
    disks();
    return;
  }
  unsigned int num=0;
  if(debug_arg[1]) num=evaluate(debug_arg[1]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  vfs->mount(debug_arg[0],num);
}


void NCmd::unmount()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  unsigned int num=0;
  if(debug_arg[0]) num=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  vfs->unmount(num);
}


void NCmd::disks()
{
  //if(!emu.myio) {
    //DEBUG_OUT<<"No IO!\n";
    //return;
  //}
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  vfs->disks();
}


// Change default drive #
void NCmd::disk()
{
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<"Missing param!\n"; return; }
  unsigned int num=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  vfs->setDiskNum(num);
}


void NCmd::poke()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<"Missing param!\n"; return; }
  if(!debug_arg[1]) { DEBUG_OUT<<"Missing param!\n"; return; }
  ULONG addr=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  BYTE val=evaluate(debug_arg[1]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  emu.getMap()->write8(addr,val);
  DEBUG_OUT<<"Set "<<addr<<" to "<<(short)val<<"\n";
}


void NCmd::rpoke()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<"Missing param!\n"; return; }
  if(!debug_arg[1]) { DEBUG_OUT<<"Missing param!\n"; return; }
  ULONG addr=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  BYTE val=evaluate(debug_arg[1]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  emu.getMap()->reallyWrite8(addr,val);
  DEBUG_OUT<<"Set "<<addr<<" to "<<(short)val<<"\n";
}


void NCmd::peek()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<"Missing param!\n"; return; }
  ULONG addr=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  BYTE val=emu.getMap()->read8(addr);
  DEBUG_OUT<<addr<<" is "<<(short)val<<"\n";
}


void NCmd::d()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!emu.getCPU()) return;
  unsigned int t=debug_loc;
  unsigned int lastoplen=0,lastloc=debug_loc;
  bool temp;
  unsigned int start=debug_loc,end;
  if(debug_arg[0]) { start=evaluate(debug_arg[0]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  end=start+(defaultNumLines*2);
  if(debug_arg[1]) { end=evaluate(debug_arg[1]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if(end<start) end=emu.getNumBytes();
  //DEBUG_OUT<<"Disassembling from "<<start<<" to ~"<<end;  emu.print_mem_type(start);
  //DEBUG_OUT<<"\n";
  temp=emu.getCPU()->getTrace();
  emu.getCPU()->setTrace(true);
  unsigned int adr;
  //w->endMore();
  //if(w->getUseMore()) w->startMore();
  //emu.getMap()->printNearestPrevSymbolIfAny(start);
  for(t=(unsigned int)start;t<(unsigned int)end;) {
    adr=t&emu.getAddrMask();
    emu.getMap()->touch(adr);
    lastoplen=emu.getCPU()->disasm(adr);
    if(!lastoplen) {
      // Something went wrong, cpu wasn't able to disasm...
      t=end*2;  // Break out of the for loop...
    }
    t+=lastoplen;
  }
  lastloc=t;
  emu.getCPU()->setTrace(temp);
  debug_loc=lastloc;
  // Wipe out last d'd location
  last_debug_pointer=1;
  last_debug_buffer[0]='d';
  last_debug_buffer[1]=0;
  //if(w->getUseMore()) w->endMore();
}


void NCmd::list()
{
  DEBUG_OUT<<"List command not Implemented yet!"<<"\n";
}


void NCmd::jsr()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!emu.getCPU()) return;
  unsigned int val;
  emu.getCPU()->setRunning(true);
  if(debug_arg[0]) {
    val=evaluate(debug_arg[0]);
    if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
    emu.getCPU()->jsr(val);
  }
  emu.startTime=emu.myClock->read();
  emu.ignore=true;
}


void NCmd::g()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!emu.getCPU()) return;
  unsigned int val;
  if(emu.myvideo) {
    emu.myvideo->reallyFocus();
    emu.myvideo->redraw();
  }
  emu.getCPU()->setDebug(false);
  if(emu.myio) emu.myio->setDebug(false);
  if(emu.getMap()) emu.getMap()->setDebug(false);
  emu.getCPU()->setRunning(true);
  if(debug_arg[0]) {
    val=evaluate(debug_arg[0]);
    if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
    emu.getCPU()->jump(val);
  }
  emu.startTime=emu.myClock->read();
  emu.ignore=true;
  emu.debugASAP=false;
}


void NCmd::reset()
{
  emu.clearRam();
  debug_loc=0;
  if(emu.myio) emu.myio->reset();
  if(emu.getCPU()) {
    emu.getCPU()->reset();
    debug_loc=emu.getCPU()->getPC();
  }
  if(emu.myio) emu.myio->reset_postCPU();
}


void NCmd::m_row(unsigned int t)
{
  if(!emu.getMap()) return;
  emu.getMap()->touch(t);
  BYTE b[8];
  unsigned char c[2];
  unsigned int r;
  //unsigned int dig=w->getHexDigits();
  DEBUG_OUT<<":";
  DEBUG_OUT<<(ULONG)t<<" ";
  //w->setHexDigits(2);
  for(r=0;r<8;r++) {
    b[r]=emu.getMap()->read8(t+r);
    if(b[r]<0x10) DEBUG_OUT<<"0";
    DEBUG_OUT<<(int)b[r];
    /*if(r&1)*/ DEBUG_OUT<<" ";
  }
  DEBUG_OUT<<" ";
  c[1]=0;
  for(r=0;r<8;r++) {
    c[0]=b[r];
    c[0]=convertToPrintable(c[0]);
    DEBUG_OUT<<(char *)c;
  }
  DEBUG_OUT<<"\n";
  //w->setHexDigits(dig);
}


void NCmd::tm_row(unsigned int t)
{
  if((t+8)>tempsize) {
    DEBUG_OUT<<"Can't tm_row "<<(t+8)<<", past end!\n";
    return;
  }
  BYTE b[8];
  unsigned char c[2];
  unsigned int r;
  ////unsigned int dig=w->getHexDigits();
  DEBUG_OUT<<":";
  DEBUG_OUT<<(ULONG)t<<" ";
  ////w->setHexDigits(2);
  for(r=0;r<8;r++) {
    b[r]=tempmem[t+r];
    if(b[r]<0x10) DEBUG_OUT<<"0";
    DEBUG_OUT<<(int)b[r];
    if(r&1) DEBUG_OUT<<" ";
  }
  DEBUG_OUT<<" ";
  c[1]=0;
  for(r=0;r<8;r++) {
    c[0]=b[r];
    c[0]=convertToPrintable(c[0]);
    DEBUG_OUT<<(char *)c;
  }
  DEBUG_OUT<<"\n";
  ////w->setHexDigits(dig);
}


void NCmd::m()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  unsigned int n,t,start=debug_loc,end;
  if(debug_arg[0]) { start=evaluate(debug_arg[0]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  end=start+(defaultNumLines*8);
  if(debug_arg[1]) { end=evaluate(debug_arg[1]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  //DEBUG_OUT<<"Displaying from "<<start<<" to "<<end;  emu.print_mem_type(start);  DEBUG_OUT<<"\n";
  n=end-start;
  //w->endMore();
  //if(w->getUseMore()) w->startMore();
  //emu.getMap()->printNearestPrevSymbolIfAny(start);
  unsigned int adr;
  for(t=0;t<n;t+=8) {
    adr=(start+t)&emu.getAddrMask();
    m_row(adr);
  }
  debug_loc=end;
  // Wipe out last m'd location
  last_debug_pointer=1;
  last_debug_buffer[0]='m';
  last_debug_buffer[1]=0;
  debug_pointer=0;
  //if(w->getUseMore()) w->endMore();
}


void NCmd::tm()
{
  unsigned int n,t,start=debug_loc,end;
  if(debug_arg[0]) { start=evaluate(debug_arg[0]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  end=start+(defaultNumLines*8);
  if(debug_arg[1]) { end=evaluate(debug_arg[1]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  //DEBUG_OUT<<"Displaying from "<<start<<" to "<<end;  emu.print_mem_type(start);  DEBUG_OUT<<"\n";
  n=end-start;
  //w->endMore();
  //if(w->getUseMore()) w->startMore();
  unsigned int adr;
  for(t=0;t<n;t+=8) {
    adr=(start+t);
    tm_row(adr);
  }
  debug_loc=end;
  // Wipe out last m'd location
  last_debug_pointer=2;
  last_debug_buffer[0]='t';
  last_debug_buffer[1]='m';
  last_debug_buffer[2]=0;
  debug_pointer=0;
  //if(w->getUseMore()) w->endMore();
}


void NCmd::fill()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(!debug_arg[1]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(!debug_arg[2]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  unsigned int t,start=debug_loc,end;
  BYTE dat;
  start=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  end=evaluate(debug_arg[1]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  dat=evaluate(debug_arg[2]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  for(t=start;t<end;t++) emu.getMap()->reallyWrite8(t,dat);
}


void NCmd::cpu()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  unsigned int n=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  emu.setCPU(n);
}


void NCmd::bank()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  unsigned int n=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  emu.setMap(n);
}


/*
void NCmd::mirror()
{
  if(!debug_arg[0]) { DEBUG_OUT<<"Missing param!\n"; return; }
  if(!debug_arg[1]) { DEBUG_OUT<<"Missing param!\n"; return; }
  if(!debug_arg[2]) { DEBUG_OUT<<"Missing param!\n"; return; }
  if(!nemu) return;
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  unsigned int start=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  unsigned int size=evaluate(debug_arg[1]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  ULONG loc=evaluate(debug_arg[2]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  unsigned int end=start+size;
  DEBUG_OUT<<"Mirroring locations "<<start<<"-"<<end<<" at "<<loc<<"\n";
  // NOTE: Map must destroy this as we leave it dangling...
  EMemMirror *mm=new EMemMirror(emu.getMap(),start,size,loc);
  if(mm) {
    if(mm->error()) {
      DEBUG_OUT<<"Error creating mirror!\n";
    }
    else {
      emu.getMap()->map(mm);
    }
  }
  else DEBUG_OUT<<"Couldn't create mirror!\n";
  if(emu.getMap()->error()) emu.quitASAP=true;
}
*/


void NCmd::map()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  char *searchName=debug_arg[0];
  EMemAbstract *area=emu.getMap()->findArea(searchName);
  if(!area) {
    DEBUG_OUT<<"map: Couldn't find "<<searchName<<"\n";
    return;
  }
  emu.getMap()->map(area);
  if(emu.getMap()->err.error()) emu.quitASAP=true;
}


void NCmd::unmap()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  char *searchName=debug_arg[0];
  EMemAbstract *area=emu.getMap()->findArea(searchName);
  if(!area) {
    DEBUG_OUT<<"Couldn't find "<<searchName<<"\n";
    return;
  }
  emu.getMap()->unmap(area);
  if(emu.getMap()->err.error()) emu.quitASAP=true;
}


void NCmd::step()
{
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  if(emu.getCPU()) {
    bool old_trace=emu.getCPU()->getTrace();
    bool old_run=emu.getCPU()->getRunning();
    emu.getCPU()->setTrace(true);
    emu.getCPU()->setRunning(false);
    emu.getCPU()->execute();
    emu.getCPU()->setTrace(old_trace);
    emu.getCPU()->setRunning(old_run);
  }
}


void NCmd::showVars()
{
  for(unsigned int i=0;i<4;i++) {
    if(emu.mycpu[i])
    DEBUG_OUT<<"cpu"<<i<<"=     "<<emu.mycpu[i]->getName()<<"\n";
  }
  DEBUG_OUT<<"chips=    ";
  if(emu.myio) DEBUG_OUT<<emu.myio->getName(); else DEBUG_OUT<<"None";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"addrbits= "<<aDefaultStringHelper.myDec(emu.getAddrBits());
  DEBUG_OUT<<" ("<<(unsigned int)emu.getAddrMask()<<")\n";
  DEBUG_OUT<<"pagesize= "<<aDefaultStringHelper.myDec(emu.getPageSize())<<"\n";
  DEBUG_OUT<<"load off= "<<emu.loadOffset;
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"iostop=   ";
  if(emu.myio) { if(emu.myio->io_stop)  DEBUG_OUT<<"on ";  else DEBUG_OUT<<"off"; }
  else DEBUG_OUT<<"n/a";
  DEBUG_OUT<<" : Stop on I/O access\n";
  DEBUG_OUT<<"io=       ";
  if(emu.myio) { if(emu.myio->io_debug)  DEBUG_OUT<<"on ";  else DEBUG_OUT<<"off"; }
  else DEBUG_OUT<<"n/a";
  DEBUG_OUT<<" : Debug I/O access\n";
  DEBUG_OUT<<"output=   ";
  //if(w->getDebugOutput()) DEBUG_OUT<<"on "; else DEBUG_OUT<<"off";
  //DEBUG_OUT<<" : Output debug info\n";
  //DEBUG_OUT<<"window=   ";
  //if(w->getTextOutput()) DEBUG_OUT<<"on "; else DEBUG_OUT<<"off";
  //DEBUG_OUT<<" : Output to window\n";
  DEBUG_OUT<<"trace=    ";
  if(emu.getCPU()) { if(emu.getCPU()->getTrace())  DEBUG_OUT<<"on" ;  else DEBUG_OUT<<"off"; }
  else DEBUG_OUT<<"n/a";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"break=    ";
  if(emu.getCPU()) DEBUG_OUT<<emu.getCPU()->getBreakpoint(); else DEBUG_OUT<<"n/a";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"op_ticks= "<<emu.op_ticks<<"\n";
  DEBUG_OUT<<"ticks=    "<<emu.ticks<<"\n";
  if(emu.myio) {
    DEBUG_OUT<<"tfactor=  "<<emu.myio->getTimerFactor()<<"\n";
    DEBUG_OUT<<"tspeed=   "<<emu.myio->getTimerSpeed()<<"\n";
  }
  if(emu.myvideo) {
    DEBUG_OUT<<"rfactor=  "<<emu.myvideo->getRasterFactor()<<"\n";
    DEBUG_OUT<<"rspeed=   "<<emu.myvideo->getRasterSpeed()<<"\n";
  }
}


void NCmd::set()
{
  //w->endMore();
  //if(w->getUseMore()) w->startMore();
  bool db=false;
  unsigned int val,ea;
  if(!debug_arg[0]) {
    showVars();
    return;
  }
  // ELSE there was at least one arg...
  if(!debug_arg[1]) debug_arg[2]="0";
  val=evaluate(debug_arg[1],false);
  bool tmpErrror=cmdError;
  cmdError=false;
  if(!strcmp(debug_arg[0],"io")) {
    if(emu.myio) {
      emu.myio->io_debug=!emu.myio->io_debug;
      if(emu.myio->io_debug) outputCount++; else outputCount--;
    }
    checkOutputCount();
  } else
  //if(!strcmp(debug_arg[0],"output")))
  //{ w->setDebugOutput(!w->getDebugOutput()); } else
  //if(!strcmp(debug_arg[0],"window")))
  //{ w->setTextOutput(!w->getTextOutput());  } else
  if(!strcmp(debug_arg[0],"trace")) {
    if(emu.getCPU()) {
      emu.getCPU()->setTrace(!emu.getCPU()->getTrace());
      if(emu.getCPU()->getTrace()) outputCount++; else outputCount--;
      checkOutputCount();
    }
  } else
  if(!strcmp(debug_arg[0],"fstop"))
  { emu.unknown_stop=!emu.unknown_stop; } else
  if(!strcmp(debug_arg[0],"iostop"))
  { if(emu.myio) emu.myio->io_stop=!emu.myio->io_stop; } else
  /*
  if(!strcmp(debug_arg[0],"more"))) {
    bool useMore=w->getUseMore();
    if(useMore) DEBUG_OUT<<"Use 'more'\n";
    else DEBUG_OUT<<"Don't use 'more'\n";
    w->setUseMore(!useMore);
  } else
  */
  if(!strcmp(debug_arg[0],"rfactor"))
  { if(emu.myio) emu.myvideo->setRasterFactor(val); } else
  if(!strcmp(debug_arg[0],"rspeed"))
  { if(emu.myio) emu.myvideo->setRasterSpeed(val); } else
  if(!strcmp(debug_arg[0],"tfactor"))
  { if(emu.myio) emu.myio->setTimerFactor(val); } else
  if(!strcmp(debug_arg[0],"tspeed"))
  { if(emu.myio) emu.myio->setTimerSpeed(val); } else
  if(!strcmp(debug_arg[0],"break"))
  { if(emu.getCPU()) emu.getCPU()->setBreakpoint(val); } else
  if(!strcmp(debug_arg[0],"ticks"))
  { emu.ticks=val; } else
  if(!strcmp(debug_arg[0],"cpu")) {
    emu.handleNewCPU(debug_arg[1]);
  } else
  if(!strcmp(debug_arg[0],"fpu")) {
    emu.getCPU()->setFPU(debug_arg[1]);
  } else
  if(!strcmp(debug_arg[0],"addr"))
  { emu.loadOffset=val; } else
  if(!strcmp(debug_arg[0],"addrbits"))
  {
    // shift all args down one and call the command version
    debug_arg[0]=debug_arg[1];
    debug_arg[1]=debug_arg[2];
    debug_arg[2]=debug_arg[3];
    addrbits();
  } else
  if(!strcmp(debug_arg[0],"pagesize"))
  { emu.setPageSize(val); } else
  if(!strcmp(debug_arg[0],"chips")) {
    emu.handleNewIO(debug_arg[1]);
  } else
  if(debug_arg[0][0]=='$') {
    sscanf((char *)(debug_arg[0]+1),"%x",&ea);
    emu.getMap()->write8((unsigned int)ea,(BYTE)val);
  } else
  if(debug_arg[0][0]=='#') {
    sscanf((char *)(debug_arg[0]+1),"%d",&ea);
    emu.getMap()->write8((unsigned int)ea,(BYTE)val);
  } else
  if(emu.getCPU()) {
    if(emu.getCPU()->setRegister(debug_arg[0],val)) { db=true; }
    else DEBUG_OUT<<debug_arg[0]<<" is not valid.\n";
  }
  else DEBUG_OUT<<debug_arg[0]<<" is not valid.\n";
  //if(w->getUseMore()) w->endMore();
  if(db) emu.debugger((char *)NULL);
}


void NCmd::s()
{
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  BYTE *ptr=(BYTE *)NULL;
  ULONG address=0,start=0,end=0xffff;
  unsigned int err=0,num;
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(debug_arg[1]) { start=evaluate(debug_arg[1]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if(debug_arg[2]) { end=evaluate(debug_arg[2]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  num=end-start+1;
  //if((start+num)>0xffff) num=0xffff-start;
  FILE *fd=(FILE *)NULL;
  DVfs *vfs=emu.getVfs();
  if(emu.useVfs) {
    if(!vfs) {
      DEBUG_OUT<<errMsgNoVfs;
      return;
    }
    fd=(FILE *)vfs->Fopen(debug_arg[0],"wb");
  }
  else fd=fopen(debug_arg[0],"wb");
  if(!fd) {
    DEBUG_OUT<<"Couldn't open file for writing!\n";
    return;
  }
  ptr=(BYTE *)&address;
  ptr[0]=start&0xff;  ptr[1]=(start>>8)&0xff;
  DEBUG_OUT<<"Saving...\n";
  // NOTE: This only really does 2 bytes for loadOffset...
  if(emu.loadOffset) {
    if(emu.useVfs) err=vfs->Fwrite((BYTE *)&address,2,1,fd);
    else err=fwrite((BYTE *)&address,2,1,fd);
  }
  for(unsigned int t=start;t<start+num;t++) {
    unsigned char c;
    c=emu.getMap()->read8(t);
    if(emu.useVfs) err+=vfs->Fwrite((BYTE *)&c,1,1,fd);
    else err+=fwrite((BYTE *)&c,1,1,fd);
  }
  //if(emu.core_save) num+=8;
  //if(emu.load_addr) num+=2;
  if(err!=num) { DEBUG_OUT<<"File write error ("<<err<<"!="<<num<<").\n"; }
  if(fd) {
    if(emu.useVfs) vfs->Fclose(fd); else fclose(fd);
    fd=(FILE *)NULL;
  }
}


unsigned int NCmd::parseNumberOrSymbol(char *s,bool errIfNotFound)
{
  if(!s) return 0;
  unsigned int val=0;
  EMapSymbol *sym=NULL;
  EMemMapAbstract *map=emu.getMap();
  EModule *mod=NULL;
  if(map) mod=map->getFirstModule();
  bool pointer=false;
  if(s[0]=='*') { pointer=true; s=&s[1]; }
  if(mod) sym=mod->lookupSymbolByName(s);
  if(!sym) { if(mod) sym=mod->lookupStringByName(s); }
  if(!sym) { if(mod) sym=mod->lookupNodeByName(s); }
  if(!sym) { if(map) sym=map->findModule(s); }
  if(!sym) { if(map) sym=map->lookupSymbolByName(s); }
  if(!sym) {
    char *slc=aDefaultStringHelper.toUpper(s);
    if(mod) sym=mod->lookupSymbolByName(slc);
    if(!sym) { if(mod) sym=mod->lookupStringByName(slc); }
    if(!sym) { if(mod) sym=mod->lookupNodeByName(slc); }
    if(!sym) { if(map) sym=map->findModule(slc); }
    if(!sym) { if(map) sym=map->lookupSymbolByName(slc); }
  }
  //
  // If we get here, it is not a symbol, but must be a number etc.
  if(!sym) {
    bool number=true;
    for(unsigned int t=0;t<strlen(s);t++) {
      switch(s[t]) {
        case '#':  case '$':
        case '0':  case '1':  case '2':  case '3':
        case '4':  case '5':  case '6':  case '7':
        case '8':  case '9':
        case 'a':  case 'b': case 'c':  case 'd':  case 'e':  case 'f':
        case 'A':  case 'B': case 'C':  case 'D':  case 'E':  case 'F':
          break;
        default:
          //DEBUG_OUT<<"Must not be a number, found "<<(int)(s[t])<<" "<<s[t]<<"\n";
          number=false;
          break;
      }
    }
    if(number) {
      aDefaultStringHelper.setDecMode(false);
      val=aDefaultStringHelper.parseNumber(s);
      if(pointer) val=map->read32b(val);  // FIXME: Assumes big endian
    }
    else {
      if(errIfNotFound) {
        DEBUG_OUT<<"No such number or symbol as "<<s<<"!\n";
        cmdError=true;
      }
    }
  }
  else {
    if(pointer) val=map->read32b(sym->addr);  // FIXME: Assumes big endian
    else val=sym->addr;
  }
  return val;
}


unsigned int NCmd::evaluate(char *s,bool errIfNotFound)
{
  if(!s) return 0;
  // TODO: eventually this would evaluate simple expressions...
  // TODO: print an error if its non numeric!
  unsigned int ret=0;
  ret=parseNumberOrSymbol(s,errIfNotFound);
  //DEBUG_OUT<<"NCmd::evaluate("<<s<<")=="<<ret<<"\n";
  return ret;
}


void NCmd::l()
{
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  unsigned int start=0;
  unsigned int err;
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(debug_arg[1]) { start=evaluate(debug_arg[1]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  FILE *fd=(FILE *)NULL;
  DVfs *vfs=emu.getVfs();
  if(emu.useVfs) {
    if(vfs) fd=(FILE *)vfs->Fopen(debug_arg[0],"rb");
  }
  else fd=fopen(debug_arg[0],"rb");
  if(fd) {
    // NOTE: This only really handles 2 byte cbm style offsets...
    if(emu.loadOffset) {
      ULONG address;
      BYTE *ptr=(BYTE *)&address;
      if(emu.useVfs) err=vfs->Fread((BYTE *)&address,2,1,fd);
      else err=fread((BYTE *)&address,2,1,fd);
      if(!start) start=ptr[0]+(ptr[1]*256);
    }
    DEBUG_OUT<<"Loading "<<debug_arg[0]<<" at "<<start<<"...";
    unsigned int t=start;
    BYTE c;
    bool ww=true;
    while(ww) {
      if(emu.useVfs) err=vfs->Fread(&c,1,1,fd);
      else err=fread(&c,1,1,fd);
      if(err!=1) ww=false;
      else { emu.getMap()->write8(t,c); t++; }
    }
    DEBUG_OUT<<t<<"\n";
  }
  else {
    DEBUG_OUT<<"Couldn't load "<<debug_arg[0]<<".\n";
    emu.setError();
  }
  if(fd) {
    if(emu.useVfs) vfs->Fclose(fd); else fclose(fd);
    fd=(FILE *)NULL;
  }
}


void NCmd::romload()
{
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  unsigned int start=0;
  //unsigned int err;
  /*
  if(!debug_arg[0]) {
    AFileRequester *rw=new AFileRequester();
    debug_arg[0]=rw->show("Choose ROM to load:"));
    delete rw;  rw=(AFileRequester *)NULL;
  }
  */
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(!debug_arg[1]) {
    DEBUG_OUT<<"need to specify an address for romload!\n";
    return;
  }
  if(debug_arg[1]) { start=evaluate(debug_arg[1]); }
  DEBUG_OUT<<"romload "<<debug_arg[0]<<" @ "<<start<<"\n";
  char *mapname=debug_arg[0];
  if(debug_arg[2]) mapname=debug_arg[2];
  DEBUG_OUT<<"mapname of the rom is "<<mapname<<"\n";
  ERom *rom=new ERom(start,debug_arg[0],emu.loadOffset,mapname);
  if(rom) {
    if(!rom->err.error()) {
      emu.roms->reset();
      while(!emu.roms->atEnd()) emu.roms->advance();
      emu.roms->append(rom);
      emu.getMap()->addArea(rom);
      curLoadAddr=start+rom->getSize();
      if(emu.getMap()) {
        EMapSymbol *sym=new EMapSymbol(rom->getName(),0,rom->getLocation(),rom->getSize(),EMAP_SYMBOL_BINNODE);
        emu.getMap()->getSymbols()->append((void *)sym);
      }
      return;
    }
  }
  DEBUG_OUT<<"Couldn't romload '"<<debug_arg[0]<<"'.\n";
  emu.setError();
  exit(5);
}


void NCmd::source()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  AClock c;
  unsigned int startClock=c.read(),endClock=0;
  sourcing=true;
  bool temp=false;
  bool sourced=false;
  if(emu.getCPU()) {
    emu.getCPU()->getRunning();
    emu.getCPU()->setRunning(true);
  }
  DEBUG_OUT<<"\n";
  char *fname=debug_arg[0];
  DEBUG_OUT<<"Reading from '"<<fname<<"'...\n";
  if(!emu.isItATextFile(fname)) {
    DEBUG_OUT<<"("<<fname<<" is not a text file!)\n";
    return;
  }
  FILE *f=(FILE *)NULL;
  DVfs *vfs=emu.getVfs();
  if(emu.useVfs) {
    if(vfs) f=(FILE *)vfs->Fopen(fname,"r");
    else DEBUG_OUT<<"should useVfs, but no vfs!\n";
  }
  else f=fopen(fname,"r");
  if(!f) f=fopen(aDefaultStringHelper.removeQuotes(fname),"r");
  if(!f) f=fopen(aDefaultStringHelper.flipSlashes(fname),"r");
  if(f) {
#ifdef ALIB_HAVE_CHDIR
    char pathName[1024];
    if(aDefaultStringHelper.getPathFromFullName(pathName,fname)) {
      // If there's a prefixed path to the config file, we'll go to that directory
      DEBUG_OUT<<"path is "<<pathName<<"\n";
      chdir(pathName);
    }
#endif // ALIB_HAVE_CHDIR
    bool ww=true;
    sourced=true;
    while(ww) {
      debug_pointer=0;
      unsigned int foo;
      unsigned char c=0;
      while(c!='\n') {
        if(emu.useVfs) foo=vfs->Fread(&c,1,1,f); else foo=fread(&c,1,1,f);
        if(foo!=1) { debug_buffer[debug_pointer]=0;  c='\n'; }
        else {
          debug_buffer[debug_pointer]=c;
          debug_pointer++;
          if(debug_pointer==NEMU_DEBUGBUF_SIZE) debug_pointer--;
        }
      }
      unsigned int tpointer=debug_pointer;
      if(tpointer) tpointer-=1;
      debug_buffer[tpointer]=0;
      if(foo!=1) ww=false;
      //DEBUG_OUT<<"Source: '"<<debug_buffer<<"'\n";
      if(ww) {
        debug_interp();
      }
      if(cmdError) ww=false;
    }
    //DEBUG_OUT<<"source done.\n";
  }
  else {
    //DEBUG_OUT<<"Couldn't open file.\n";
    emu.setError();
  }
  if(f) {
    if(emu.useVfs) vfs->Fclose(f); else fclose(f);
    f=(FILE *)NULL;
  }
  sourcing=false;
  last_debug_pointer=0;
  last_debug_buffer[0]=0;
  debug_pointer=0;
  if(sourced) {
    if(emu.getCPU()) emu.getCPU()->setRunning(temp);
    emu.getReady();
    endClock=c.read();
    //DEBUG_OUT<<"Sourcing took "<<(endClock-startClock)<<" clocks.\n\n";
    //emu.debugger((char *)NULL);
    //DEBUG_OUT<<">";
  }
  //w->focus();
}


void NCmd::romlist()
{
  DEBUG_OUT<<"Loaded roms:\n";
  emu.roms->reset();
  int n=0;
  while(!emu.roms->atEnd()) {
    ERom *r=(ERom *)emu.roms->info();
    DEBUG_OUT<<n<<" ";
    DEBUG_OUT<<r->getName()<<" "<<r->getLocation()<<" "<<r->getSize()<<" bytes\n";
    emu.roms->advance();
    n++;
  }
  emu.roms->reset();
}


void NCmd::addrbits()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  // other args are optional
  DEBUG_OUT<<"addrbits: "<<debug_arg[0]<<"\n";
  unsigned int val=evaluate(debug_arg[0]);
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  DEBUG_OUT<<"val is "<<val<<"\n";
  emu.setAddrBits(val,debug_arg[1]);
}


void NCmd::areas()
{
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  //if(!w) return;
  //if(w->getUseMore()) w->startMore();
  DEBUG_OUT<<"Areas: (*=mapped)\n";
  emu.getMap()->getAreas()->reset();
  while(!emu.getMap()->getAreas()->atEnd()) {
    EMemAbstract *r=(EMemAbstract *)emu.getMap()->getAreas()->info();
    if(!r->getShowInMap()) DEBUG_OUT<<"("; else DEBUG_OUT<<" ";
    if(r->isMapped()) DEBUG_OUT<<"*"; else DEBUG_OUT<<"-";
    DEBUG_OUT<<" "<<r->getSize()<<"@"<<r->getLocation();
    DEBUG_OUT<<" "<<r->getName();
    if(!r->getShowInMap()) DEBUG_OUT<<")";
    DEBUG_OUT<<"\n";
    emu.getMap()->getAreas()->advance();
  }
  emu.getMap()->getAreas()->reset();
  //if(w->getUseMore()) w->endMore();
}


void NCmd::help()
{
  //if(w->getUseMore()) w->startMore();
  DEBUG_OUT<<"NEMU debugger help:\n";
  DEBUG_OUT<<"clear    : Clear window\n";
  DEBUG_OUT<<"exit,x,q : Quit NEMU\n";
  DEBUG_OUT<<"source   : Read command file\n";
  DEBUG_OUT<<"help,h,? : This help list\n";
  DEBUG_OUT<<"redraw   : Redraw video\n";
  DEBUG_OUT<<"r,regs   : Register dump\n";
  DEBUG_OUT<<"ior      : Internal IO dump\n";
  DEBUG_OUT<<"vr       : Internal Video dump\n";
  DEBUG_OUT<<"mirror   : Mirror area in map\n";
  DEBUG_OUT<<"showg    : Show gfx\n";
  DEBUG_OUT<<"shows    : Show sprites\n";
  DEBUG_OUT<<"ints     : Show cpu's ints\n";
  DEBUG_OUT<<"ram      : Mark area as RAM\n";
  DEBUG_OUT<<"mem,memr : Display read map\n";
  DEBUG_OUT<<"memw     : Display write map\n";
  DEBUG_OUT<<"memv     : Display video map\n";
  DEBUG_OUT<<"romlist  : Display ROM list\n";
  DEBUG_OUT<<"romload  : Load ROM image\n";
  DEBUG_OUT<<"unused   : Mark area unused\n";
  DEBUG_OUT<<"m,hd     : Hex/Ascii dump\n";
  DEBUG_OUT<<"d        : Disassemble memory\n";
  DEBUG_OUT<<"s        : Save to disk\n";
  DEBUG_OUT<<"l        : Load from disk\n";
  DEBUG_OUT<<"fill     : Fill memory\n";
  DEBUG_OUT<<"set      : List or set vars\n";
  DEBUG_OUT<<"reset,rs : Reset emulator\n";
  DEBUG_OUT<<"rsg      : Reset and go\n";
  DEBUG_OUT<<"st,step  : Step over\n";
  DEBUG_OUT<<"stg      : Step over and go\n";
  DEBUG_OUT<<"g        : Start running\n";
  DEBUG_OUT<<"tron     : Tracing on\n";
  DEBUG_OUT<<"troff    : Tracing off\n";
  DEBUG_OUT<<"tempload : Load into temp\n";
  DEBUG_OUT<<"romtemp  : Load ROM from temp\n";
  DEBUG_OUT<<"tm       : Hex/Ascii temp\n";
  DEBUG_OUT<<"tempfree : Free temp\n";
  DEBUG_OUT<<"areas    : List all map areas\n";
  DEBUG_OUT<<"poke     : Set memory\n";
  DEBUG_OUT<<"rpoke    : Really set memory\n";
  DEBUG_OUT<<"peek     : Read memory\n";
  //if(w->getUseMore()) w->endMore();
}


void NCmd::memHeader()
{
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  //w->setHexDigits(emu.getSigDigits());
  //if(w->getUseMore()) w->startMore();
  DEBUG_OUT<<emu.getMap()->getName()<<" ";
  //w->dec();
  DEBUG_OUT<<emu.getNumPages()<<" pages of "<<emu.getPageSize()<<" bytes ";
  //w->hex();
  DEBUG_OUT<<"\n";
  bool foo=false;
  if(emu.getMap()->getDemandPaging()) { DEBUG_OUT<<"(Demand on) "; foo=true; }
  if(emu.getMap()->getVirtualMemory()) { DEBUG_OUT<<"(VM on) "; foo=true; }
  if(emu.getMap()->getPageFaults()) { DEBUG_OUT<<"(Faults on) "; foo=true; }
  if(foo) DEBUG_OUT<<"\n";
}


void NCmd::memr()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  //if(emu.getMap()->error()) return;
  //if(w->getUseMore()) w->startMore();
  ////unsigned int dig=w->getHexDigits();
  memHeader();
  // x is rows, y is columns, should change this...
  unsigned int numRows=emu.getNumPages()/16;
  unsigned int x,y;
  unsigned int inc=1;
  //if(emu.getNumPages()>256) { inc=16; numRows=numRows/256; }
  char tc[2];
  tc[1]=0;
  unsigned int counter=0;
  bool flag=false;
  for(y=0;y<16;y++) prevMemLine[y]=0;
  for(x=0;x<numRows;x++) {
    unsigned int t=x*emu.getPageSize()*16*inc;
    counter=0;
    for(y=0;y<16;y++) {
      curMemLine[y]=emu.getMap()->getRPageLetter(y*inc+x*16*inc);
      if(curMemLine[y]==prevMemLine[y]) counter++;
    }
    if(x==(numRows-1)) counter=0;  // so we always print the last row
    if(counter==16) {
      if(!flag) {
        DEBUG_OUT<<"             ....      \n";
        flag=true;
      }
    }
    else {
      DEBUG_OUT<<(UINT16)t;    DEBUG_OUT<<" : ";
      for(y=0;y<16;y++) {
        tc[0]=curMemLine[y];
        DEBUG_OUT<<tc;
        prevMemLine[y]=curMemLine[y];
        flag=false;
      }
      DEBUG_OUT<<"\n";
    }
  }
  //areas();
  //if(w->getUseMore()) w->endMore();
  ////w->setHexDigits(dig);
}


void NCmd::memw()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  //if(emu.getMap()->error()) return;
  //if(w->getUseMore()) w->startMore();
  ////unsigned int dig=w->getHexDigits();
  memHeader();
  // x is rows, y is columns, should change this...
  unsigned int numRows=emu.getNumPages()/16;
  unsigned int x,y;
  unsigned int inc=1;
  //if(emu.getNumPages()>256) { inc=16; numRows=numRows/256; }
  char tc[2];
  tc[1]=0;
  unsigned int counter=0;
  bool flag=false;
  for(y=0;y<16;y++) prevMemLine[y]=0;
  for(x=0;x<numRows;x++) {
    unsigned int t=x*emu.getPageSize()*16*inc;
    counter=0;
    for(y=0;y<16;y++) {
      curMemLine[y]=emu.getMap()->getWPageLetter(y*inc+x*16*inc);
      if(curMemLine[y]==prevMemLine[y]) counter++;
    }
    if(x==(numRows-1)) counter=0;  // so we always print the last row
    if(counter==16) {
      if(!flag) {
        DEBUG_OUT<<"             ....      \n";
        flag=true;
      }
    }
    else {
      DEBUG_OUT<<(UINT16)t;    DEBUG_OUT<<" : ";
      for(y=0;y<16;y++) {
        tc[0]=curMemLine[y];
        DEBUG_OUT<<tc;
        prevMemLine[y]=curMemLine[y];
        flag=false;
      }
      DEBUG_OUT<<"\n";
    }
  }
  //areas();
  //if(w->getUseMore()) w->endMore();
  ////w->setHexDigits(dig);
}


void NCmd::memv()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  //if(emu.getMap()->error()) return;
  //if(w->getUseMore()) w->startMore();
  ////unsigned int dig=w->getHexDigits();
  memHeader();
  // x is rows, y is columns, should change this...
  unsigned int numRows=emu.getNumPages()/16;
  unsigned int x,y;
  unsigned int inc=1;
  //if(emu.getNumPages()>256) { inc=16; numRows=numRows/256; }
  char tc[2];
  tc[1]=0;
  unsigned int counter=0;
  bool flag=false;
  for(y=0;y<16;y++) prevMemLine[y]=0;
  for(x=0;x<numRows;x++) {
    unsigned int t=x*emu.getPageSize()*16*inc;
    counter=0;
    for(y=0;y<16;y++) {
      curMemLine[y]=emu.getMap()->getVPageLetter(y*inc+x*16*inc);
      if(curMemLine[y]==prevMemLine[y]) counter++;
    }
    if(x==(numRows-1)) counter=0;  // so we always print the last row
    if(counter==16) {
      if(!flag) {
        DEBUG_OUT<<"             ....      \n";
        flag=true;
      }
    }
    else {
      DEBUG_OUT<<(UINT16)t;    DEBUG_OUT<<" : ";
      for(y=0;y<16;y++) {
        tc[0]=curMemLine[y];
        DEBUG_OUT<<tc;
        prevMemLine[y]=curMemLine[y];
        flag=false;
      }
      DEBUG_OUT<<"\n";
    }
  }
  //areas();
  //if(w->getUseMore()) w->endMore();
  ////w->setHexDigits(dig);
}


void NCmd::ram(bool needToMap)
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap;return;}
  if(emu.getMap()->err.error()) { DEBUG_OUT<<errMsgNoMap;return;}
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  ULONG addr=0,len=0;
  if(!debug_arg[1]) {
    // One arg version, addr is assumed to be 0
    len=evaluate(debug_arg[0]);  // In Kbytes
    return;
  }
  else {
    // Two are version, with addr and size
    addr=evaluate(debug_arg[0]);
    if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
    len=evaluate(debug_arg[1]);  // In Kbytes
    if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  }
  if(!len) return;
  if((len*1024)>emu.getNumBytes()) {
    DEBUG_OUT<<"NOTE: RAM is larger than memory map...going to try bigger map.\n";
    unsigned int target=20;
    if(len>1024) target=24;
    if(len>(16*1024)) target=32;
    emu.setAddrBits(target);
  }
#ifdef ALIB_16BIT_INTS
  // If we've got 16 bit ints, keep ram less than 64k in size...
  if(len>63) len=63;
#endif
  ERam *r=new ERam(len*1024);
  if(!r) {
    DEBUG_OUT<<"Couldn't alloc ram!\n";
    return;
  }
  if(r->err.error()) {
    DEBUG_OUT<<"Couldn't alloc ram!\n";
    delete r;
    return;
  }
  r->setLocation(addr);
  // set optional name if passed...
  if(debug_arg[2]) r->setName(debug_arg[2]);
  DEBUG_OUT<<"Mapping ram (";
  //w->dec();
  DEBUG_OUT<<r->getSize()<<" at ";
  //w->hex();
  DEBUG_OUT<<r->getLocation()<<").\n";
  if(needToMap) emu.getMap()->map(r);
  if(emu.getMap()->err.error()) emu.quitASAP=true;
}


void NCmd::romfromrom()
{
}


void NCmd::romtemp()
{
  if(!emu.getMap()) {
    DEBUG_OUT<<errMsgNoMap;
    return;
  }
  // TODO: Check number of args here...
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  unsigned int start=curLoadAddr,tstart=0,size=0;
  if(debug_arg[1]) { start=evaluate(debug_arg[1]); }
  else { start=curLoadAddr; }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if(debug_arg[2]) { tstart=evaluate(debug_arg[2]); }
  else { tstart=0; }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if(debug_arg[3]) { size=evaluate(debug_arg[3]); }
  else { size=tempsize-tstart; }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if((!tempmem)||(!tempsize)) {
    DEBUG_OUT<<"Can't romtemp "<<debug_arg[0]<<", no tempmem.\n";
    return;
  }
  BYTE *tmem=(BYTE *)calloc(64,1024);
  if(!tmem) {
    DEBUG_OUT<<"Couldn't allocate 64k temp buffer!\n";
    return;
  }
  DEBUG_OUT<<"romtemp "<<debug_arg[0]<<" "<<tstart<<"@"<<start<<" ";
  DEBUG_OUT<<size<<"\n";
  BYTE c=0;
  for(unsigned int t=0;t<size;t++) {
    if((tstart+t)<tempsize) c=tempmem[tstart+t];
    if(t<65535) tmem[t]=c;
  }
  ERom *rom=new ERom((char *)debug_arg[0],(unsigned int)start,(BYTE *)tmem,(unsigned int)size);
  if(rom) {
    if(!rom->err.error()) {
      emu.roms->reset();
      while(!emu.roms->atEnd()) emu.roms->advance();
      emu.roms->append(rom);
      emu.getMap()->addArea(rom);
    }
  }
  curLoadAddr=start+size;
  if(tmem) free(tmem);
}


void NCmd::tempfree()
{
  if(tempmem) free(tempmem);
  tempsize=0;
  tempmem=(unsigned char *)NULL;
}


void NCmd::tempload()
{
  unsigned int err;
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  //FILE *fd=(FILE *)vfs->Fopen(debug_arg[0],"rb"));
  FILE *fd=(FILE *)fopen(debug_arg[0],"rb");
  if(fd) {
    if(tempmem) tempfree();
    vfs->Fseek(fd,0,SEEK_END);
    tempsize=(unsigned int)vfs->Ftell(fd);
    vfs->Fseek(fd, 0, SEEK_SET);
    tempmem=(unsigned char *)malloc(tempsize);
    DEBUG_OUT<<"File is "<<tempsize<<" bytes.\n";
    if(!tempmem) {
      DEBUG_OUT<<"Couldn't allocate temp area!\n";
      vfs->Fclose(fd);
      tempsize=0;
      return;
    }
    DEBUG_OUT<<"Loading from "<<debug_arg[0]<<" to temp...";
    // NOTE: This only handles 2 byte offsets...
    if(emu.loadOffset) {
      ULONG address;
      BYTE *ptr=(BYTE *)&address;
      err=vfs->Fread((BYTE *)&address,2,1,fd);
      //if(!start) start=ptr[0]+(ptr[1]*256);
    }
    unsigned int t=0;
    BYTE c;
    bool ww=true;
    while(ww) {
      err=vfs->Fread(&c,1,1,fd);
      if(err!=1) ww=false;
      else { tempmem[t]=c;  t++; }
    }
    DEBUG_OUT<<t<<"\n";
  }
  else {
    DEBUG_OUT<<"Couldn't load "<<debug_arg[0]<<".\n";
    emu.setError();
    //ASimpleMsgWindow mw(debug_arg[0],"Error: couldn't tempload!"));
  }
  if(fd) vfs->Fclose(fd);
}


void NCmd::srec()
{
  unsigned int err;
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  //FILE *fd=(FILE *)vfs->Fopen(debug_arg[0],"rb"));
  FILE *fd=(FILE *)fopen(debug_arg[0],"rb");
  if(fd) {
    DEBUG_OUT<<"Loading srec from "<<debug_arg[0]<<"...\n";
    char c,stype;
    unsigned int icount,iaddr,ichecksum;
    char tbuf[10];
    bool reading=true;
    bool okay=false;
    unsigned int t=0;
    while(reading) {
      okay=false;
      fread(&c,1,1,fd);
      if(c=='S') {
        fread(&stype,1,1,fd);
        fread(&tbuf,2,1,fd);  tbuf[2]=0;
        sscanf(tbuf,"%x",&icount);
        unsigned int nab=4;
        if((stype=='2')||(stype=='8')) nab=6;
        if((stype=='3')||(stype=='7')) nab=8;
        fread(&tbuf,nab,1,fd);  tbuf[nab]=0;
        sscanf(tbuf,"%x",&iaddr);
        icount-=(nab/2)+1;
        DEBUG_OUT<<"icount="<<icount<<" iaddr=$"<<aDefaultStringHelper.myHex4(iaddr)<<" stype='"<<stype<<"'\n";
        char tbyte=0;
        switch(stype) {
          case '0': case '1': case '2': case '3':
            for(t=0;t<icount;t++) {
              fread(&tbuf,2,1,fd);  tbuf[2]=0;
              sscanf(tbuf,"%x",&tbyte);
              emu.getMap()->write8(iaddr,tbyte);
              DEBUG_OUT<<aDefaultStringHelper.myHex2(tbyte);
              iaddr++;
            }
            fread(&tbuf,2,1,fd);  tbuf[2]=0;
            sscanf(tbuf,"%x",&ichecksum);
            DEBUG_OUT<<" chksum:"<<aDefaultStringHelper.myHex2(ichecksum)<<"\n";
            break;
          case '7': case '8': case '9':
            for(t=0;t<icount;t++) {
              fread(&tbuf,2,1,fd);  tbuf[2]=0;
              sscanf(tbuf,"%x",&tbyte);
              //emu.getMap()->write8(iaddr,tbyte);
              DEBUG_OUT<<aDefaultStringHelper.myHex2(tbyte);
              iaddr++;
            }
            fread(&tbuf,2,1,fd);  tbuf[2]=0;
            sscanf(tbuf,"%x",&ichecksum);
            DEBUG_OUT<<" chksum:"<<aDefaultStringHelper.myHex2(ichecksum)<<"\n";
            break;
          default:
            std::cerr<<"bad record type in srec!\n";
            reading=false;
            break;
        }
        okay=true;
      }
      if(c=='\n') okay=true;
      if(!okay) { std::cerr<<"srec error, got '"<<c<<"' ("<<(int)c<<"), expecting 'S'!\n"; exit(5); }
      if(feof(fd)) reading=false;
    }
  }
  else {
    DEBUG_OUT<<"Couldn't load "<<debug_arg[0]<<".\n";
    emu.setError();
  }
  if(fd) vfs->Fclose(fd);
}


void NCmd::ihex()
{
  unsigned int err;
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  DVfs *vfs=emu.getVfs();
  if(!vfs) { DEBUG_OUT<<errMsgNoVfs; return; }
  //FILE *fd=(FILE *)vfs->Fopen(debug_arg[0],"rb"));
  FILE *fd=(FILE *)fopen(debug_arg[0],"rb");
  if(fd) {
    DEBUG_OUT<<"Loading ihex from "<<debug_arg[0]<<"...\n";
    char c;
    unsigned int icount,iaddr,itype,ichecksum;
    char tbuf[10];
    bool reading=true;
    bool okay=false;
    unsigned int t=0;
    while(reading) {
      okay=false;
      fread(&c,1,1,fd);
      if(c==':') {
        fread(&tbuf,2,1,fd);  tbuf[2]=0;
        sscanf(tbuf,"%x",&icount);
        fread(&tbuf,4,1,fd);  tbuf[4]=0;
        sscanf(tbuf,"%x",&iaddr);
        fread(&tbuf,2,1,fd);  tbuf[2]=0;
        sscanf(tbuf,"%x",&itype);
        DEBUG_OUT<<"icount="<<icount<<" iaddr=$"<<aDefaultStringHelper.myHex4(iaddr)<<" itype="<<itype<<"\n";
        char tbyte=0;
        switch(itype) {
          case 0x00:
            for(t=0;t<icount;t++) {
              fread(&tbuf,2,1,fd);  tbuf[2]=0;
              sscanf(tbuf,"%x",&tbyte);
              emu.getMap()->write8(iaddr,tbyte);
              DEBUG_OUT<<aDefaultStringHelper.myHex2(tbyte);
              iaddr++;
            }
            fread(&tbuf,2,1,fd);  tbuf[2]=0;
            sscanf(tbuf,"%x",&ichecksum);
            DEBUG_OUT<<" chksum:"<<aDefaultStringHelper.myHex2(ichecksum)<<"\n";
            break;
          case 0x01:
            DEBUG_OUT<<"ihex eof record fount.\n";
            reading=false;
            break;
          default:
            std::cerr<<"bad record type in ihex!\n";
            reading=false;
            break;
        }
        okay=true;
      }
      if(c=='\n') okay=true;
      if(!okay) { std::cerr<<"ihex error, got '"<<c<<"' ("<<(int)c<<"), expecting ':'!\n"; exit(5); }
      if(feof(fd)) reading=false;
    }
  }
  else {
    DEBUG_OUT<<"Couldn't load "<<debug_arg[0]<<".\n";
    emu.setError();
  }
  if(fd) vfs->Fclose(fd);
}


#ifndef ALIB_16BIT_INTS
void NCmd::rsrcdir()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  EResourceFile rf(debug_arg[0]);
  if(rf.err.error()) {
    DEBUG_OUT<<"Couldn't open resource file "<<debug_arg[0]<<"\n";
    emu.setError();
    return;
  }
  AList *res=rf.getMap();
  bool reading=true;
  while(reading) {
    if(res->atEnd()) reading=false;
    else {
      EResource *r=(EResource *)res->info();
      DEBUG_OUT<<r->type<<" "<<r->num<<" ";
      DEBUG_OUT<<r->offset<<" ";
      DEBUG_OUT<<r->size<<" ";
      if(r->name) DEBUG_OUT<<r->name;
      DEBUG_OUT<<"\n";
      res->advance();
    }
  }
}
#endif


#ifndef ALIB_16BIT_INTS
void NCmd::rsrcload()
{
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  EResourceFile r(debug_arg[0]);
  if(r.err.error()) {
    DEBUG_OUT<<"Couldn't open resource file "<<debug_arg[0]<<"\n";
    emu.setError();
    return;
  }
  // TODO: Always pad debug_arg[1] with spaces to 4 characters
  if(!r.count(debug_arg[1])) {
    DEBUG_OUT<<"No resources of type "<<debug_arg[1]<<" in "<<debug_arg[0]<<"\n";
    emu.setError();
    return;
  }
  // TODO: have debug_arg[2] be resource ID (or num?)
  unsigned int num=0;
  // TODO: Is this big enough for the largest resource?
  if(tempmem) tempfree();
  BYTE *buf=(BYTE *)calloc(64,1024);
  if(!r.readFromNum(debug_arg[1],num,buf)) {
    DEBUG_OUT<<"Error reading that resource...\n";
    emu.setError();
    return;
  }
  memcpy(tempmem,buf,r.getSizeFromNum(debug_arg[1],num));
  // TODO: Transfer to temp memory or something...
  free(buf);
}
#endif


void NCmd::unused()
{
  if(!emu.getMap()) { DEBUG_OUT<<errMsgNoMap; return; }
  unsigned int start=0,len=0;
  if(!debug_arg[0]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(!debug_arg[1]) { DEBUG_OUT<<errMsgMissParam; return; }
  if(debug_arg[0]) { start=evaluate(debug_arg[0]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  if(debug_arg[1]) { len=evaluate(debug_arg[1]); }
  if(cmdError) { DEBUG_OUT<<errMsgCmdErr; return; }
  //emu.getMap()->markUnusedArea(start,len);
  warnCmd();
}


void NCmd::warnCmd()
{
  //ASimpleMsgWindow mw(debug_cmd,"Warning: that command should not be used!"));
}


char NCmd::convertToPrintable(char c)
{
  char r=c;
  if(!r) r='.';
  if(r&0x80) r&=0x7f;
  if(r==0x7f) r='.';
  if(r<32) r+='a'-1;
  return r;
}


/*
char *NCmd::mangleName(char *s)
{
  if(!s) return NULL;
  bool flag=false;
  unsigned int count=0;
  for(unsigned int t=0;t<strlen(s);t++) {
    if(s[t]==':') {
      count++;
      if(count==2) flag=true;
    }
    else count=0;
  }
  if(flag) {
    // should mangle name here...
    DEBUG_OUT<<"mangleName: "<<s<<"\n";
  }
  return s;
}
*/


/*
char *NCmd::unmangleName(char *s)
{
  if(!s) return NULL;
  return s;
}
*/


void NCmd::debug_parse()
{
  unsigned int t;
  AStringHelper s;
  s.parseString(debug_buffer);
  if(!s.getArgc()) { debug_args=0; debug_cmd=(char *)NULL; return; }
  debug_args=s.getArgc()-1;
  debug_cmd=strdup(s.getArgv(0));
  for(t=0;t<10;t++) debug_arg[t]=(char *)NULL;
  for(t=1;t<10;t++) {
    if(s.getArgv(t)) debug_arg[t-1]=strdup(s.getArgv(t));
    else debug_arg[t-1]=(char *)NULL;
    //if(debug_arg[t-1]) std::cerr<<t<<": '"<<debug_arg[t-1]<<"'\n";
  }
  debug_arg[9]=(char *)NULL;
}


void NCmd::debug_interp()
{
  unsigned int t;
  bool foo=false,done=false;
  if(emu.getCPU()) { if(!emu.getCPU()->getRunning()) foo=true; } else foo=true;
  if(foo) {
    if(!sourcing) DEBUG_OUT<<"\n";
    // If command was all blanks, just repeat last one...
    if(!debug_pointer) {
      if(last_debug_pointer) {
        for(t=0;t<last_debug_pointer;t++)  debug_buffer[t]=last_debug_buffer[t];
        debug_pointer=last_debug_pointer;
      }
      else { /*if(!sourcing) DEBUG_OUT<<">";*/ }
    }
  }
  if(!debug_pointer) { DEBUG_OUT<<"No cmd!]n"; return; }
  debug_parse();
  if(!debug_cmd) {
    //DEBUG_OUT<<">";
    debug_pointer=0;
    return;
  }
  char *tdebug_cmd=aDefaultStringHelper.toLower(debug_cmd);
  cmdError=false;
  if(inComment) {
    // The following commands are only valid within comments...
    //if(!strcmp(tdebug_cmd,"*/")) inComment=false;
  }
  else {
    //std::cerr<<"Going to try do_cmd...\n";
    done=do_cmd(tdebug_cmd);
    if(!done) {
      if(emu.myio) {
        //std::cerr<<"Going to try io's interp...\n";
        done=emu.myio->interp(tdebug_cmd,debug_arg[0],debug_arg[1],debug_arg[2]);
        if(emu.getCPU()) debug_loc=emu.getCPU()->getPC();
      }
    }
    if(!done) {
      if(emu.getCPU()) {
        //std::cerr<<"Going to try cpu's interp...\n";
        done=emu.getCPU()->interp(tdebug_cmd,debug_arg[0],debug_arg[1],debug_arg[2]);
        debug_loc=emu.getCPU()->getPC();
      }
    }
    if(!done) {
      //std::cerr<<"Going to try handleFile...\n";
      done=emu.handleFile(tdebug_cmd);
    }
    if(!done) {
      DEBUG_OUT<<"Unknown command '"<<tdebug_cmd<<"'!\n";
      cmdError=true;
    }
  }
  //DEBUG_OUT<<"debug_loc="<<debug_loc<<"\n";
  if(emu.getCPU()) { if(!emu.getCPU()->getRunning()) { /*if(!sourcing) DEBUG_OUT<<">";*/ } }
  else { /*if(!sourcing) DEBUG_OUT<<">";*/ }
  for(t=0;t<debug_pointer;t++)  last_debug_buffer[t]=debug_buffer[t];
  last_debug_pointer=debug_pointer;
  debug_pointer=0;
}


bool NCmd::do_cmd(char *tdebug_cmd)
{
  bool done=false;
  //if(!strcmp(tdebug_cmd,"clear")) { aDefaultAWindowOutputStream->getWindow()->clear();  aDefaultAWindowOutputStream->getWindow()->moveTo(0,0); done=true; }
  //if(!strcmp(tdebug_cmd,"cls")) { aDefaultAWindowOutputStream->getWindow()->clear();  aDefaultAWindowOutputStream->getWindow()->moveTo(0,0); done=true; }
  if(!strcmp(tdebug_cmd,"x")) { emu.quit(); done=true; }
  if(!strcmp(tdebug_cmd,"exit")) { emu.quit(); done=true; }
  if(!strcmp(tdebug_cmd,"zz")) { emu.quit(); done=true; }
  if(!strcmp(tdebug_cmd,"ZZ")) { emu.quit(); done=true; }
  if(!strcmp(tdebug_cmd,"es")) { emu.quit(); done=true; }
  if(!strcmp(tdebug_cmd,"q")) { emu.quit(); done=true; }
  if(!strcmp(tdebug_cmd,"quit")) { emu.quit(); done=true; }
  if(!strcmp(tdebug_cmd,"r")) { emu.debugASAP=false; emu.debugger((char *)NULL); done=true; }
  if(!strcmp(tdebug_cmd,"regs")) { emu.debugASAP=false; emu.debugger((char *)NULL); done=true; }
  if(!strcmp(tdebug_cmd,"m")) { m(); done=true; }
  if(!strcmp(tdebug_cmd,"hd")) { m(); done=true; }
  if(!strcmp(tdebug_cmd,"s")) { s(); done=true; }
  if(!strcmp(tdebug_cmd,"l")) { l(); done=true; }
  if(!strcmp(tdebug_cmd,"ver")) { emu.ver(); done=true; }
  if(!strcmp(tdebug_cmd,"tempfree")) { tempfree(); done=true; }
  if(!strcmp(tdebug_cmd,"tempload")) { tempload(); done=true; }
  if(!strcmp(tdebug_cmd,"romtemp")) { romtemp(); done=true; }
  if(!strcmp(tdebug_cmd,"romfromrom")) { romfromrom(); done=true; }
  if(!strcmp(tdebug_cmd,"ram")) { ram(true); done=true; }
  if(!strcmp(tdebug_cmd,"ram_nomap")) { ram(false); done=true; }
  if(!strcmp(tdebug_cmd,"tm")) { tm(); done=true; }
  if(!strcmp(tdebug_cmd,"ls")) { ls(); done=true; }
  if(!strcmp(tdebug_cmd,"ll")) { ls(); done=true; }
  if(!strcmp(tdebug_cmd,"dir")) { ls(); done=true; }
  if(!strcmp(tdebug_cmd,"rb")) { rb(); done=true; }
  if(!strcmp(tdebug_cmd,"readblock")) { rb(); done=true; }
  if(!strcmp(tdebug_cmd,"rblock")) { rb(); done=true; }
  if(!strcmp(tdebug_cmd,"readblk")) { rb(); done=true; }
  if(!strcmp(tdebug_cmd,"rdblk")) { rb(); done=true; }
  if(!strcmp(tdebug_cmd,"rblk")) { rb(); done=true; }
  if(!strcmp(tdebug_cmd,"dir")) { ls(); done=true; }
  if(!strcmp(tdebug_cmd,"catalog")) { ls(); done=true; }
  if(!strcmp(tdebug_cmd,"cd")) { cd(); done=true; }
  if(!strcmp(tdebug_cmd,"chdir")) { cd(); done=true; }
  if(!strcmp(tdebug_cmd,"more")) { more(); done=true; }
  if(!strcmp(tdebug_cmd,"cat")) { cat(); done=true; }
  if(!strcmp(tdebug_cmd,"type")) { cat(); done=true; }
  if(!strcmp(tdebug_cmd,"rm")) { rm(); done=true; }
  if(!strcmp(tdebug_cmd,"del")) { rm(); done=true; }
  if(!strcmp(tdebug_cmd,"delete")) { rm(); done=true; }
  if(!strcmp(tdebug_cmd,"mv")) { mv(); done=true; }
  if(!strcmp(tdebug_cmd,"ren")) { mv(); done=true; }
  if(!strcmp(tdebug_cmd,"cp")) { cp(); done=true; }
  if(!strcmp(tdebug_cmd,"copy")) { cp(); done=true; }
  if(!strcmp(tdebug_cmd,"crash")) {
    char *a=(char *)NULL;
    *a='a';
    done=true;
  }
  if(!strcmp(tdebug_cmd,"ior")) {
    if(emu.myio) emu.myio->ioregs();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"io")) {
    if(emu.myio) emu.myio->ioregs();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"ints")) {
    if(emu.getCPU()) emu.getCPU()->ints();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"int")) {
    if(emu.getCPU()) emu.getCPU()->ints();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"irq")) {
    if(emu.getCPU()) emu.getCPU()->ints();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"irqs")) {
    if(emu.getCPU()) emu.getCPU()->ints();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"vr")) {
    if(emu.myio) emu.myio->vregs();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"video")) {
    if(emu.myio) emu.myio->vregs();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"ld")) { ld(); done=true; }
  if(!strcmp(tdebug_cmd,"nm")) { nm(); done=true; }
#ifndef ALIB_16BIT_INTS
  if(!strcmp(tdebug_cmd,"rdir")) { rsrcdir(); done=true; }
  if(!strcmp(tdebug_cmd,"rload")) { rsrcload(); done=true; }
#endif
  if(!strcmp(tdebug_cmd,"print")) { print(); done=true; }
  if(!strcmp(tdebug_cmd,"strings")) { strings(); done=true; }
  if(!strcmp(tdebug_cmd,"symbols")) { symbols(); done=true; }
  if(!strcmp(tdebug_cmd,"syms")) { symbols(); done=true; }
  if(!strcmp(tdebug_cmd,"nodes")) { nodes(); done=true; }
  if(!strcmp(tdebug_cmd,"symbol")) { symbol(); done=true; }
  if(!strcmp(tdebug_cmd,"modules")) { modules(); done=true; }
  if(!strcmp(tdebug_cmd,"mods")) { modules(); done=true; }
  if(!strcmp(tdebug_cmd,"mod")) { module(); done=true; }
  if(!strcmp(tdebug_cmd,"module")) { module(); done=true; }
  if(!strcmp(tdebug_cmd,"sym")) { symbol(); done=true; }
  if(!strcmp(tdebug_cmd,"node")) { node(); done=true; }
  if(!strcmp(tdebug_cmd,"insert")) { mount(); done=true; }
  if(!strcmp(tdebug_cmd,"mount")) { mount(); done=true; }
  if(!strcmp(tdebug_cmd,"eject")) { unmount(); done=true; }
  if(!strcmp(tdebug_cmd,"unmount")) { unmount(); done=true; }
  if(!strcmp(tdebug_cmd,"umount")) { unmount(); done=true; }
  if(!strcmp(tdebug_cmd,"disks")) { disks(); done=true; }
  if(!strcmp(tdebug_cmd,"df")) { disks(); done=true; }
  if(!strcmp(tdebug_cmd,"disk")) { disk(); done=true; }
  if(!strcmp(tdebug_cmd,"drives")) { disks(); done=true; }
  if(!strcmp(tdebug_cmd,"drive")) { disk(); done=true; }
  if(!strcmp(tdebug_cmd,"loadcolorrom")) { loadcolorrom(); done=true; }
  if(!strcmp(tdebug_cmd,"romload")) { romload(); done=true; }
  if(!strcmp(tdebug_cmd,"romlist")) { romlist(); done=true; }
  //if(!strcmp(tdebug_cmd,"shows")) {
    //if(emu.myvideo) emu.myvideo->showSpriteGfx(); done=true;
  //}
  //if(!strcmp(tdebug_cmd,"showg")) {
    //if(emu.myvideo) emu.myvideo->showTileGfx(); done=true;
  //}
  if(!strcmp(tdebug_cmd,"poke")) { poke(); done=true; }
  if(!strcmp(tdebug_cmd,"rpoke")) { rpoke(); done=true; }
  if(!strcmp(tdebug_cmd,"peek")) { peek(); done=true; }
  if(!strcmp(tdebug_cmd,"unused")) { unused(); done=true; }
  if(!strcmp(tdebug_cmd,"areas")) { areas(); done=true; }
  if(!strcmp(tdebug_cmd,"set")) { set(); done=true; }
  if(!strcmp(tdebug_cmd,"d")) { d(); done=true; }
  if(!strcmp(tdebug_cmd,"g")) { g(); done=true; }
  if(!strcmp(tdebug_cmd,"jsr")) { jsr(); done=true; }
  if(!strcmp(tdebug_cmd,"list")) { list(); done=true; }
  if(!strcmp(tdebug_cmd,"reset")) { reset(); done=true; }
  if(!strcmp(tdebug_cmd,"rs")) { reset(); done=true; }
  if(!strcmp(tdebug_cmd,"rsg")) { reset(); g(); done=true; }
  if(!strcmp(tdebug_cmd,"bt")) {
    if(emu.getCPU()) emu.getCPU()->showStackTrace();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"sc")) {
    if(emu.getCPU()) emu.getCPU()->showStackTrace();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"stack")) {
    if(emu.getCPU()) emu.getCPU()->showStackTrace();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"stron")) {
    if(emu.getCPU()) emu.getCPU()->startTracingStack();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"stroff")) {
    if(emu.getCPU()) emu.getCPU()->endTracingStack();
    done=true;
  }
  if(!strcmp(tdebug_cmd,"tron")) {
    if(emu.getCPU()) {
      if(emu.getCPU()->getTrace()) {
        if(outputCount<10) outputCount++;
      }
      emu.getCPU()->setTrace(true);
      checkOutputCount();
    }
    done=true;
  }
  if(!strcmp(tdebug_cmd,"troff")) {
    if(emu.getCPU()) {
      if(emu.getCPU()->getTrace()) {
        if(outputCount) outputCount--;
      }
      emu.getCPU()->setTrace(false);
      checkOutputCount();
    }
    done=true;
  }
  /*
  if(!strcmp(tdebug_cmd,"output")) {
    w->setDebugOutput(!w->getDebugOutput());
    done=true;
  }
  */
  if(!strcmp(tdebug_cmd,"step")) { step(); done=true; }
  if(!strcmp(tdebug_cmd,"redraw")) {
    //if(w) {
      //w->redraw();
    //}
    if(emu.myio) {
      emu.myio->redraw();
    }
    done=true;
  }
  if(!strcmp(tdebug_cmd,"memmap")) { memr(); done=true; }
  if(!strcmp(tdebug_cmd,"mem")) { memr(); done=true; }
  if(!strcmp(tdebug_cmd,"memr")) { memr(); done=true; }
  if(!strcmp(tdebug_cmd,"rmem")) { memr(); done=true; }
  if(!strcmp(tdebug_cmd,"memw")) { memw(); done=true; }
  if(!strcmp(tdebug_cmd,"wmem")) { memw(); done=true; }
  if(!strcmp(tdebug_cmd,"memv")) { memv(); done=true; }
  if(!strcmp(tdebug_cmd,"vmem")) { memv(); done=true; }
  if(!strcmp(tdebug_cmd,"memt")) { tmem(); done=true; }
  if(!strcmp(tdebug_cmd,"tmem")) { tmem(); done=true; }
  //if(!strcmp(tdebug_cmd,"mirror")) { mirror(); done=true; }
  if(!strcmp(tdebug_cmd,"st")) { step(); done=true; }
  if(!strcmp(tdebug_cmd,"stg")) { step(); g(); done=true; }
  if(!strcmp(tdebug_cmd,"source")) { source(); done=true; }
  if(!strcmp(tdebug_cmd,"map")) { map(); done=true; }
  if(!strcmp(tdebug_cmd,"unmap")) { unmap(); done=true; }
  if(!strcmp(tdebug_cmd,"h")) { help(); done=true; }
  if(!strcmp(tdebug_cmd,"help")) { help(); done=true; }
  if(!strcmp(tdebug_cmd,"fill")) { fill(); done=true; }
  if(!strcmp(tdebug_cmd,"cpu")) { cpu(); done=true; }
  if(!strcmp(tdebug_cmd,"bank")) { bank(); done=true; }
  //if(!strcmp(tdebug_cmd,"resmap")) { emu.getMap()->resetError(); done=true; }
  if(!strcmp(tdebug_cmd,"ihex")) { ihex(); done=true; }
  if(!strcmp(tdebug_cmd,"srec")) { srec(); done=true; }
  if(!strcmp(tdebug_cmd,"addrbits")) { addrbits(); done=true; }
  if(tdebug_cmd[0]=='#') { done=true; } // comment
  if(tdebug_cmd[0]==';') { done=true; } // comment
  if((tdebug_cmd[0]==47)&&(tdebug_cmd[1]==47)) { done=true; } // comment
  if(tdebug_cmd[0]==63) { help(); done=true; }
  /*
  if(!strcmp(tdebug_cmd,"f")) {
    for(t=1;t<10;t++) debug_arg[t]=(char *)NULL;
    ARequestWindow *rw=new ARequestWindow;
    tdebug_cmd=strdup(rw->askFile("Choose a file"));
    delete rw;  rw=NULL;
  }
  */
  return done;
}


void NCmd::checkOutputCount()
{
  if(outputCount>0) emu.setCheckOften();
  else {
    emu.setCheckRarely();
    outputCount=0;
  }
}

