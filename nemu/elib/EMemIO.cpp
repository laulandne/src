
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemIO.h>
#include <elib/EHandler.h>
#include <elib/EMemMapAbstract.h>


////////////////////////////////////////////////////////////////////////////////
//  EMemIO Class
////////////////////////////////////////////////////////////////////////////////


EMemIO::EMemIO(EHandler *parent) : ERam(0)
{
  DEBUG_OUT<<"(Creating EMemIO)\n";
  init();
  handler=parent;
  type=handler->getType();
  // other inits
  letter='i';
  name=strdup("io.area");
  actualSize=0;
  reset();
}


EMemIO::~EMemIO()
{
  DEBUG_OUT<<"EMemIO destroyed.\n";
  if(err.error()) { DEBUG_OUT<<"EMemIO with error!\n"; return; }
  if(mapped) handler->getMap()->unmap(this);
  freeMem();
  init();
  //err.set();
}


void EMemIO::init()
{
  // Init all data members
  handler=(EHandler *)NULL;
  type=0;
  temp=0;
  letter='i';
  name=(char *)"io";
  location=0;
  mapped=false;
  debug=false;
}


void EMemIO::ioregs()
{
}


void EMemIO::reset()
{
}


void EMemIO::io_cycle()
{
}


BYTE EMemIO::read8(ULONG address)
{
  //BYTE page=(address>>8)&0xff;
  //if(io->io_debug) *(io->getWindow())<<"I/O read  at "<<address<<"\n";
  return mem[address-location];
}


BYTE EMemIO::write8(ULONG address, BYTE value)
{
  //BYTE page=(address>>8)&0xff;
  //BYTE val=value;
  //if(io->io_debug) *(io->getWindow())<<"I/O write at "<<address<<" of "<<(BYTE)value<<"\n";
  mem[address-location]=value;
  return value;
}


void EMemIO::resize(unsigned int s)
{
  unsigned int ss=s;
  if(!ss) { DEBUG_OUT<<"Can't set ss to 0!";  return; }
  bool wasMap=mapped;
  //DEBUG_OUT<<"Going to unmap if needed.\n";
  if(wasMap) handler->getMap()->unmap(this);
  //DEBUG_OUT<<"Going to saferresize.\n";
  reallyResize(ss);
  //DEBUG_OUT<<"Going to map if needed.\n";
  if(wasMap) handler->getMap()->map(this);
  //DEBUG_OUT<<"saferresize done.\n";
}


void EMemIO::resize(unsigned int actual, unsigned int mapSize)
{
  if(!actual) { DEBUG_OUT<<"Can't set actualSize to 0!\n";  return; }
  if(!mapSize) { DEBUG_OUT<<"Can't set mapSize to 0!\n";  return; }
  actualSize=actual;
  resize(actual);
  memsize=mapSize;  // ...Careful!  This is really lying...
}


void EMemIO::debugger(const char *str)
{
  debug=true;
  if(handler) handler->debugger(str);
}

