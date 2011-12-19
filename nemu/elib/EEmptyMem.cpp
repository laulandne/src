
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <elib/EEmptyMem.h>
#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/ANullStream.h>


///////////////////////////////////////////////////////////////////////////////
//  EEmptyMem Class
///////////////////////////////////////////////////////////////////////////////

EEmptyMem::EEmptyMem(unsigned int tsize)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"EEmptyMem("<<tsize<<")...\n";
#endif // DEBUG_VERBOSE
  //init();
  memsize=tsize;
  letter='X';
  name="empty_area";
  busErrorFlag=false;
}


BYTE EEmptyMem::write8(ULONG addr,BYTE data)
{
  //DEBUG_OUT<<"#";
  busErrorFlag=true;
  bool ret=false;
  /*
  if((memMap)&&(wantBusErrors)) {
    ret=memMap->busError(this,addr,data,E_MEM_BYTE,E_MEM_WRITE);
    if(ret) memMap->write8(addr,data);
    memMap->resetBusError();
  }
  */
  return 0;
}


BYTE EEmptyMem::reallyWrite8(ULONG addr,BYTE data)
{
  //DEBUG_OUT<<"#";
  busErrorFlag=true;
  bool ret=false;
  /*
  if((memMap)&&(wantBusErrors)) {
    ret=memMap->busError(this,addr,data,E_MEM_BYTE,E_MEM_WRITE);
    if(ret) memMap->reallyWrite8(addr,data);
    memMap->resetBusError();
  }
  */
  return (addr>>8)&0xff;
}


BYTE EEmptyMem::read8(ULONG addr)
{
  //DEBUG_OUT<<"r";
  busErrorFlag=true;
  bool ret=false;
  BYTE rval=(addr>>8)&0xff;
  /*
  if((memMap)&&(wantBusErrors)) {
    ret=memMap->busError(this,addr,0,E_MEM_BYTE,E_MEM_READ);
    if(ret) rval=memMap->read8(addr);
    memMap->resetBusError();
  }
  */
  return rval;
}
