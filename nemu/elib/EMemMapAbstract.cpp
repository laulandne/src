
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#define CHECK_MASK 1
#define CHECK_NULL_PAGES 1


#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>


///////////////////////////////////////////////////////////////////////////////
//  EMemMapAbstract Class
///////////////////////////////////////////////////////////////////////////////


EMemMapAbstract::EMemMapAbstract()
{
  DEBUG_OUT<<"EMemMapAbstract() created.\n";
  init();
}


EMemMapAbstract::~EMemMapAbstract()
{
  err.set();
}


void EMemMapAbstract::init()
{
  debug=false;
  numPages=0; pageSize=0; addrMask=0;
  pMask=0; addrBits=0; pShift=0;
  empty=(EEmptyMem *)NULL;
  ioHandler=(EHandler *)NULL;
}


/* static */
ULONG EMemMapAbstract::bits2Bytes(ULONG bits)
{
  ULONG ret=1L<<bits;
  if(bits>31) ret=0xffffffff;
  return ret;
}


/* static */
ULONG EMemMapAbstract::bytes2Bits(ULONG bytes)
{
  ULONG bits=0L;
  ULONG hbytes=1L;
  while(hbytes<bytes) { hbytes*=2L; bits++; }
  return bits;
}


/* static */
ULONG EMemMapAbstract::bits2K(ULONG bits)
{
  ULONG ret=(ULONG)(bits2Bytes(bits)/1024L);
  return ret;
}


EMemAbstract *EMemMapAbstract::findArea(const char *name)
{
  const char *sname=aDefaultStringHelper.toLower(name);
  //DEBUG_OUT<<"Looking for '"<<sname<<"'...\n";
  if(!sname) { DEBUG_OUT<<"NULL name passed to findArea!\n"; return (EMemAbstract *)NULL; }
  areas.reset();
  while(!areas.atEnd()) {
    EMemAbstract *r=(EMemAbstract *)areas.info();
    if(!strcmp(r->getName(),sname)) {
      //DEBUG_OUT<<"Found "<<name<<" that we were looking for.\n";
      areas.reset();
      return r;
    }
    else {
      //DEBUG_OUT<<"Found "<<r->getName()<<", wasn't it...\n";
      areas.advance();
    }
  }
  areas.reset();
  return (EMemAbstract *)NULL;
}


EMemAbstract *EMemMapAbstract::findArea(const char *name,ULONG where)
{
  const char *sname=aDefaultStringHelper.toLower(name);
  if(!sname) { DEBUG_OUT<<"NULL name passed to findArea!\n"; return (EMemAbstract *)NULL; }
  //DEBUG_OUT<<"Looking for '"<<sname<<"',"<<where<<"...\n";
  areas.reset();
  while(!areas.atEnd()) {
    EMemAbstract *r=(EMemAbstract *)areas.info();
    if(!strcmp(r->getName(),sname)) {
      if(r->getLocation()==where) {
        //DEBUG_OUT<<"Found "<<name<<" that we were looking for.\n";
        areas.reset();
        return r;
      }
      else {
        //DEBUG_OUT<<"Found "<<name<<" but location didn't match...\n";
        areas.advance();  // Name matched, location didn't...
      }
    }
    else {
      //DEBUG_OUT<<"Found "<<r->getName()<<", wasn't it...\n";
      areas.advance();
    }
  }
  areas.reset();
  return (EMemAbstract *)NULL;
}


void EMemMapAbstract::addArea(EMemAbstract *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMem passed to addArea!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EMem with error passed to addArea!\n"; return; }
  if(!findArea(foo->getName(),foo->getLocation()))
    areas.insert(foo);
}


void EMemMapAbstract::removeArea(EMemAbstract *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMem passed to removeArea!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EMem with error passed to removeArea!\n"; return; }
  // hunt down and remove from list
  char *huntn=foo->getName();
  //DEBUG_OUT<<"Looking for '"<<huntn<<"'...\n";
  areas.reset();
  while(!areas.atEnd()) {
    EMemAbstract *r=(EMemAbstract *)areas.info();
    if(!strcmp(r->getName(),huntn)) {
      //DEBUG_OUT<<"Found "<<huntn<<", removing.\n";
      areas.remove();
    }
    else {
      //DEBUG_OUT<<"Found "<<r->getName()<<", wasn't it...\n";
      areas.advance();
    }
  }
  areas.reset();
}


void EMemMapAbstract::addModule(EModule *m)
{
  if(!m) return;
  modules.insert(m);
}


EModule *EMemMapAbstract::findNearestPrevModule(ULONG addr)
{
  modules.reset();
  EModule *ret=(EModule *)modules.info();
  // TODO: Obviously only works for 32 bits...
  unsigned int dist=0xffffffffL;
  unsigned int newdist=0xffffffffL;
  if(!ret) return (EModule *)NULL;
  bool reading=true;
  while(reading) {
    if(modules.atEnd()) reading=false;
    else {
      EModule *s=(EModule *)modules.info();
      newdist=(unsigned int)(addr-s->addr);
      //DEBUG_OUT<<"findNearestSymbol: newdist is "<<newdist<<"\n";
      if((newdist<dist)&&(!(s->addr>addr)))  {
        //DEBUG_OUT<<"findNearestSymbol: we like that better...\n";
        ret=s;
        dist=newdist;
      }
      modules.advance();
    }
  }
  return ret;
}


EModule *EMemMapAbstract::findModule(const char *name)
{
  modules.reset();
  bool reading=true;
  while(reading) {
    if(modules.atEnd()) reading=false;
    else {
      EModule *s=(EModule *)modules.info();
	  if(!s->compareName(name)) return s;
      else modules.advance();
    }
  }
  return NULL;
}


EModule *EMemMapAbstract::getFirstModule()
{
  modules.reset();
  return (EModule *)modules.info();
}


EMapSymbol *EMemMapAbstract::findAnyExactSymbol(ULONG addr)
{
  // First check modules
  modules.reset();
  EModule *mod=(EModule *)modules.info();
  if(!mod) return NULL;
  EMapSymbol *ret=NULL;
  bool reading=true;
  while(reading) {
    mod=(EModule *)modules.info();
    if(mod) ret=mod->findExactSymbol(addr);
    else reading=false;
    if(!ret) reading=false;
    else modules.advance();
  }
  // Now check symbols outside of modules, if we still haven't found it...
  if(!ret) ret=findExactNonModSym(addr);
  return ret;
}


// NOTE: This is for symbols outside of any module
EMapSymbol *EMemMapAbstract::findExactNonModSym(ULONG addr)
{
  symbols.reset();
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      if(s->addr==addr) {
        return s;
      }
      symbols.advance();
    }
  }
  return (EMapSymbol *)NULL;
}


EMapSymbol *EMemMapAbstract::findAnyNearestPrevSymbol(ULONG addr)
{
  EMapSymbol *ret=NULL;
  ret=findAnyExactSymbol(addr);
  if(ret) return ret;
  // First check modules
  modules.reset();
  EModule *mod=(EModule *)modules.info();
  if(!mod) return NULL;
  bool reading=true;
  while(reading) {
    mod=(EModule *)modules.info();
    if(mod) ret=mod->findNearestPrevSymbol(addr);
    else reading=false;
    if(!ret) reading=false;
    else modules.advance();
  }
  return ret;
}


EMapSymbol *EMemMapAbstract::lookupNextSymbolByName(const char *str)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  if(!str) return ret;
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      //DEBUG_OUT<<"Comparing "<<str<<" and "<<s->getName()<<"...\n";
      if(!s->compareName(str)) {
        //DEBUG_OUT<<"Got it!\n";
        ret=s; reading=false;
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EMemMapAbstract::lookupSymbolByName(const char *str)
{
  symbols.reset();
  return lookupNextSymbolByName(str);
}


const char *EMemMapAbstract::getMapType()
{
  return "EMemMapAbstract";
}


unsigned int EMemMapAbstract::writePage(ULONG addr,BYTE *data,unsigned int size)
{
  for(unsigned int t=0;t<size;t++) {
    write8(addr+t,data[t]);
    if(err.error()) return t;
  }
  return size;
}


void EMemMapAbstract::touch(ULONG addr)
{
  BYTE data=read8(addr);
  write8(addr,data);
}


BYTE EMemMapAbstract::read8(ULONG addr)
{
  DEBUG_OUT<<"EMemMapAbstract::read8 called!\n";
  exit(5);
  return 0;
}


BYTE EMemMapAbstract::write8(ULONG addr,BYTE data)
{
  DEBUG_OUT<<"EMemMapAbstract::write8 called!\n";
  exit(5);
  return 0;
}


BYTE EMemMapAbstract::reallyWrite8(ULONG addr,BYTE data)
{
  DEBUG_OUT<<"EMemMapAbstract::readllyWrite8("<<addr<<","<<(int)data<<")...\n";
  return write8(addr,data);
}
