
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#define CHECK_SIZE 1


#include <elib/EMemAbstract.h>

#include <portable/nickcpp/ANullStream.h>


///////////////////////////////////////////////////////////////////////////////
//  EMemAbstract Class
///////////////////////////////////////////////////////////////////////////////


EMemAbstract::EMemAbstract()
{
  init();
}


EMemAbstract::~EMemAbstract()
{
  err.set();
}


void EMemAbstract::init()
{
  memsize=0;
  location=0;
  letter='?';
  name=(char *)NULL;
  mapped=false;
  busErrorFlag=false;
  wantBusErrors=false;
  showInMap=true;
}


void EMemAbstract::resize(ULONG newsize)
{
  memsize=newsize;
}


void EMemAbstract::clear(ULONG addr,ULONG size)
{
  if(err.error()) { DEBUG_OUT<<"EMem with error!\n"; return; }
  for(unsigned int t=addr;t<addr+size;t++) write8(t,0);
}


void EMemAbstract::copy(BYTE *data,ULONG addr,ULONG psize)
{
  if(err.error()) { DEBUG_OUT<<"EMem with error!\n"; return; }
  if(!data) { DEBUG_OUT<<"bad pointer passed to copy!\n";  return; }
  unsigned int tsize=psize;
  if(tsize>memsize) tsize=memsize;
  for(unsigned int t=0;t<tsize;t++) write8(addr+t,data[t]);
}


void EMemAbstract::copy(EMemAbstract *src,ULONG addr)
{
  if(err.error()) { DEBUG_OUT<<"EMem with error!\n"; return; }
  if(!src) { DEBUG_OUT<<"bad src passed to copy!\n";  return; }
  if(src->err.error()) { DEBUG_OUT<<"src error passed to copy!\n";  return; }
  unsigned int tsize=src->getSize();
  if(tsize>memsize) tsize=memsize;
  for(unsigned int t=0;t<tsize;t++) {
    write8(addr+t,src->read8(t));
  }
}


// Don't call this on anything that's currently mapped!!!
void EMemAbstract::setLocation(ULONG loc)
{
  if(mapped) return;
  location=loc;
}


/* STATIC */
void *EMemAbstract::acalloc(unsigned int num,unsigned int size)
{
  void *foo=NULL;
  if(size&&num) {
    //DEBUG_OUT<<"Going to ::calloc(num="<<num<<",size="<<size<<")\n";
    foo=(void *)::calloc(num,size);
    //DEBUG_OUT<<"Got back from the ::calloc\n";
    if(!foo) DEBUG_OUT<<"acalloc("<<num<<","<<size<<") failed!\n";
  }
  else DEBUG_OUT<<"acalloc passed 0 parameter!\n";
  return foo;
}


void EMemAbstract::setMapped()
{
  mapped=true;
}


void EMemAbstract::setUnmapped()
{
  mapped=false;
}


void EMemAbstract::setName(char *n)
{
  //if(name) free(name);
  name=strdup(n);
}


bool EMemAbstract::reallyAMap()
{
  return false;
}


bool EMemAbstract::reallyVideo()
{
  return false;
}


BYTE EMemAbstract::reallyWrite8(ULONG addr,BYTE data)
{
  DEBUG_OUT<<"EMemAbstract::readllyWrite8("<<addr<<","<<(int)data<<")...\n";
  return write8(addr,data);
}


unsigned int EMemAbstract::read16l(ULONG addr)
{
  BYTE low=read8(addr);
  BYTE high=read8(addr+1);
  return low+(high<<8);
}


unsigned int EMemAbstract::read16b(ULONG addr)
{
  BYTE high=read8(addr);
  BYTE low=read8(addr+1);
  return low+(high<<8);
}


unsigned int EMemAbstract::write16l(ULONG addr, unsigned int data)
{
  BYTE low=(BYTE)(data&0xff);
  BYTE high=(BYTE)((data>>8)&0xff);
  write8(addr,low);
  write8(addr+1,high);
  return data;
}


unsigned int EMemAbstract::write16b(ULONG addr, unsigned int data)
{
  BYTE low=(BYTE)(data&0xff);
  BYTE high=(BYTE)((data>>8)&0xff);
  write8(addr,high);
  write8(addr+1,low);
  return data;
}


unsigned int EMemAbstract::read32l(ULONG addr)
{
  unsigned int low=read16l(addr);
  unsigned int high=read16l(addr+2);
  return low+(high<<16);
}


unsigned int EMemAbstract::read32b(ULONG addr)
{
  unsigned int high=read16b(addr);
  unsigned int low=read16b(addr+2);
  return low+(high<<16);
}


unsigned int EMemAbstract::write32l(ULONG addr, unsigned int data)
{
  unsigned int low=(unsigned int)(data&0xffff);
  unsigned int high=(unsigned int)((data>>16)&0xffff);
  write16l(addr,low);
  write16l(addr+2,high);
  return data;
}


unsigned int EMemAbstract::write32b(ULONG addr, unsigned int data)
{
  unsigned int low=(unsigned int)(data&0xffff);
  unsigned int high=(unsigned int)((data>>16)&0xffff);
  write16b(addr,high);
  write16b(addr+2,low);
  return data;
}


ULONG EMemAbstract::read64l(ULONG addr)
{
  ULONG low=read32l(addr);
  ULONG high=read32l(addr+4);
#ifdef __LP64__
  return low+(high<<32);
#else
  return low;
#endif // __LP64__
}


ULONG EMemAbstract::read64b(ULONG addr)
{
  ULONG high=read32b(addr);
  ULONG low=read32b(addr+4);
#ifdef __LP64__
  return low+(high<<32);
#else
  return low;
#endif // __LP64__
}


ULONG EMemAbstract::write64l(ULONG addr,ULONG data)
{
  ULONG low=(ULONG)(data&0xffffffffL);
#ifdef __LP64__
  ULONG high=(ULONG)((data>>32)&0xffffffffL);
#else
  ULONG high=0;
#endif // __LP64__
  write32b(addr,low);
  write32b(addr+4,high);
  return data;
}


ULONG EMemAbstract::write64b(ULONG addr,ULONG data)
{
  ULONG low=(ULONG)(data&0xffffffffL);
#ifdef __LP64__
  ULONG high=(ULONG)((data>>32)&0xffffffffL);
#else
  ULONG high=0;
#endif // __LP64__
  write32b(addr,high);
  write32b(addr+4,low);
  return data;
}

