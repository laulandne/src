
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/Em68kHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em68k.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>


////////////////////////////////////////////////////////////////////////////////
//  Em68kHandler Class
////////////////////////////////////////////////////////////////////////////////

Em68kHandler::Em68kHandler()
{
  DEBUG_OUT<<"Em68kHandler constructor.\n";
  romName="none";
  romSize=0;
  romOverlay=true;
  theRom=NULL;
  theRam=NULL;
}


Em68kHandler::~Em68kHandler()
{
  DEBUG_OUT<<"Em68kHandler destructor.\n";
}


const char *Em68kHandler::getName()
{
  std::cerr<<"Em68kHandler getName called!\n"; exit(5);
  return NULL;
}


void Em68kHandler::setKind(const char *tname)
{
  std::cerr<<"Em68kHandler setKind called!\n"; exit(5);
}


void Em68kHandler::io_init(BYTE ttype)
{
  std::cerr<<"Em68kHandler io_init called!\n"; exit(5);
}


void Em68kHandler::my_reset()
{
  if(map) {
    if(!theRom) { theRom=(ERom *)map->findArea(romName); }
    if(theRom) {
      romSize=theRom->getSize();
      DEBUG_OUT<<"Found rom "<<romName<<", size is "<<romSize<<"\n";
    }
    else {
      std::cerr<<"Unable to find rom "<<romName<<"!\n";
      exit(5);
    }
  }
  romOverlay=true;
  if(cpu) {
    cpu->setResetStack(resetStack);
    cpu->setStack(resetStack);
    cpu->setFramePtr(resetStack);
    cpu->setResetEntry(resetAddr);
    cpu->jump(resetAddr);
  }
}


void Em68kHandler::my_reset_postCPU()
{
  std::cerr<<"Em68kHandler my_reset_postCPU called!\n"; exit(5);
}


bool Em68kHandler::read8ret(ULONG addr,BYTE *ret)
{
  //DEBUG_OUT<<"Em68kHandler::read8ret: apage="<<apage<<" a="<<a<<"...\n";
  if(romOverlay) {
    if(theRom) {
      if(addr<romSize) { *ret=theRom->read8(addr); return true; }
    }
    else {  std::cerr<<"No rom "<<romName<<"!\n"; exit(5); }
  }
  return false;
}


bool Em68kHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  //DEBUG_OUT<<"Em68kHandler::read8ret: apage="<<apage<<" a="<<a<<"...\n";
  if(romOverlay) {
    if(theRom) {
      if(addr<romSize) { *ret=theRom->write8(addr,val); return true; }
    }
    else {  std::cerr<<"No rom "<<romName<<"!\n"; exit(5); }
  }
  return false;
}


