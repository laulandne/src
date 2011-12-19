
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#define CHECK_MASK 1
#define CHECK_NULL_PAGES 1


#include <elib/EMemMapSimple.h>
#include <elib/EModule.h>
#include <elib/EEmptyMem.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EMemIO.h>
#include <elib/EHandler.h>
#include <elib/EVideoIOAbstract.h>


#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


///////////////////////////////////////////////////////////////////////////////
//  EMemMapSimple Class
///////////////////////////////////////////////////////////////////////////////


// NOTE: npages MUST be 256, if not, you should be using an EMemMapComplex instead...
EMemMapSimple::EMemMapSimple(ULONG npages,ULONG psize,ULONG sbits)
{
  DEBUG_OUT<<"EMemMapSimple(pages="<<npages<<",psize="<<psize<<",sbits="<<sbits<<") created.\n";
  init();
  name="mem_map";
  letter='*';
  if(npages!=256) {
    DEBUG_OUT<<"Attempted to create an EMemMapSimple with numPages!=256!\n";
    exit(5);
  }
  numPages=npages;
  pageSize=psize;
  addrMask=(numPages*pageSize)-1;
#ifdef ALIB_16BIT_INTS
  // This is so we can squeeze a 64k memmap into a 16 bit int...
  memsize=(unsigned int)addrMask;
#else
  memsize=addrMask+1;
#endif
  pMask=numPages-1;
  if(sbits) addrBits=sbits;
  else {
    addrBits=bytes2Bits(memsize);
  }
  pShift=addrBits-bytes2Bits(numPages);
  DEBUG_OUT<<"(map addrMask:"<<aDefaultStringHelper.myHex(addrMask)<<"  map addrBits:"<<addrBits<<"  ";
  DEBUG_OUT<<"pMask:"<<aDefaultStringHelper.myHex(pMask)<<"  pShift bits:"<<pShift<<")\n";
  DEBUG_OUT<<"(func arrays are "<<(numPages*sizeof(EMemAbstract *))<<" bytes in size)\n";
  //
  empty=new EEmptyMem(memsize);
  if(!empty) {
    std::cerr<<"Couldn't create empty area, bailing out!";
    err.set();
    return;
  }
  //empty->setMap(this);
  empty->setShowInMap(false);
  addArea(empty);  // to keep track of it...
  //
  for(unsigned int t=0;t<numPages;t++) {
    readFunc[t]=empty;
    writeFunc[t]=empty;
    videoFunc[t]=empty;
  }
}


EMemMapSimple::~EMemMapSimple()
{
  err.set();
}


void EMemMapSimple::init()
{
  EMemMapAbstract::init();
  for(unsigned int i=0;i<256;i++) {
    readFunc[i]=(EMemAbstract *)NULL;
    writeFunc[i]=(EMemAbstract *)NULL;
    videoFunc[i]=(EMemAbstract *)NULL;
  }
}


const char *EMemMapSimple::getMapType()
{
  return "EMemMapSimple";
}


char EMemMapSimple::getWPageLetter(unsigned int page)
{
#ifdef CHECK_MASK
  page=page&pMask;
#endif
  if(writeFunc[page]) return writeFunc[page]->getLetter();
  return 'x';
}



char EMemMapSimple::getVPageLetter(unsigned int page)
{
#ifdef CHECK_MASK
  page=page&pMask;
#endif
  if(videoFunc[page]) return videoFunc[page]->getLetter();
  return 'x';
}


char EMemMapSimple::getRPageLetter(unsigned int page)
{
#ifdef CHECK_MASK
  page=page&pMask;
#endif
  if(readFunc[page]) return readFunc[page]->getLetter();
  return 'x';
}


void EMemMapSimple::map(EMemAbstract *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMem passed to map!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EMem with error passed to map!\n"; err.set(); return; }
  if(foo->reallyVideo()) { DEBUG_OUT<<"EVideo passed to map!\n"; err.set(); return; }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  if(!foo->isMapped()) mapHelper(foo);
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  unsigned int apage=(addr>>pShift);
  for(unsigned int t=0;t<pages;t++) {
    readFunc[apage+t]=foo;
    writeFunc[apage+t]=foo;
  }
  if(!foo->isMapped()) addArea(foo);
  foo->setMapped();
  //DEBUG_OUT<<"(done mapping)\n";
}


void EMemMapSimple::map(ERom *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL ERom passed to map!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"ERom with error passed to map!\n"; return; }
  if(foo->reallyVideo()) { DEBUG_OUT<<"EVideo passed to map!\n"; err.set(); return; }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  if(!foo->isMapped()) mapHelper(foo);
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  unsigned int apage=(addr>>pShift);
  for(unsigned int t=0;t<pages;t++) {
    readFunc[apage+t]=foo;
    // NOTE: ROMS are only mapped as readable, not writable
  }
  if(!foo->isMapped()) addArea(foo);
  foo->setMapped();
  //DEBUG_OUT<<"(done mapping)\n";
}


void EMemMapSimple::map(EMemIO *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMemIO passed to map!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EMemIO with error passed to map!\n"; return; }
  if(foo->reallyVideo()) { DEBUG_OUT<<"EVideo passed to map!\n"; err.set(); return; }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  if(!foo->isMapped()) mapHelper(foo);
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  unsigned int apage=(addr>>pShift);
  for(unsigned int t=0;t<pages;t++) {
    readFunc[apage+t]=foo;
    writeFunc[apage+t]=foo;
  }
  if(!foo->isMapped()) addArea(foo);
  foo->setMapped();
  //DEBUG_OUT<<"(done mapping)\n";
}


void EMemMapSimple::mapVideo(EVideoIOAbstract *foo)
{
  DEBUG_OUT<<"EMemMapSimple::mapVideo...\n";
  if(!foo) { DEBUG_OUT<<"NULL EVideoIOAbstract passed to map!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EVideoIOAbstract with error passed to map!\n"; return; }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  if(!foo->isMapped()) mapHelper(foo);
  DEBUG_OUT<<"len is "<<len<<"\n";
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  DEBUG_OUT<<"pages is "<<pages<<"\n";
  unsigned int apage=(addr>>pShift);
  DEBUG_OUT<<"apage is "<<apage<<"\n";
  unsigned int t=0;
  for(t=0;t<numPages;t++) {
    videoFunc[t]=empty;
  }
  for(t=0;t<pages;t++) {
    videoFunc[apage+t]=foo;
  }
  if(!foo->isMapped()) addArea(foo);
  //foo->setMapped();
  //DEBUG_OUT<<"(done mapping)\n";
}


// NOTE: Remember to give your areas unique names or they
// won't be removed from the list correctly...
void EMemMapSimple::mapHelper(EMemAbstract *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMem passed to mapHelper!\n"; return; }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  //DEBUG_OUT<<"Going to map "<<foo->getName()<<" at "<<addr;
  //DEBUG_OUT<<" for "<<len<<" bytes.\n";
  addr=addr&addrMask;
  unsigned int leftover=len%pageSize;
  if(leftover) {
    //DEBUG_OUT<<"Area's size isn't a multiple of pagesize!\n";
    unsigned int waspages=len/pageSize;
    unsigned int newsize=(waspages+1)*pageSize;
    //DEBUG_OUT<<"It will be resized to "<<newsize<<".\n";
    if(foo->getSize()!=foo->getActualSize()) {
      // Area takes up a lot of space, but is actually smaller
      EMemIO *fooio=(EMemIO *)foo;
      fooio->resize(fooio->getActualSize(),newsize);
    }
    else foo->resize(newsize);
  }
}


// TODO: foo's location MUST be where it was mapped before
// or this will probably trash the map!
// NOTE: Don't unmap if you didn't map
// (at least once, but you can map more than once)
// This should be caught, but currently confuses the "orig" stuff...
void EMemMapSimple::unmap(EMemAbstract *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMem passed to unmap!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EMem with error passed to unmap!\n"; return; }
  if(foo->reallyVideo()) { DEBUG_OUT<<"EVideo passed to unmap!\n"; err.set(); return; }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  //DEBUG_OUT<<"Going to unmap "<<foo->getName()<<" at ";
  //DEBUG_OUT<<addr<<" for "<<len<<" bytes.\n";
  addr=addr&addrMask;
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  unsigned int apage=(addr>>pShift);
  //DEBUG_OUT<<"(Located at page "<<loc<<" for "<<pages<<" pages)\n";
  for(unsigned int t=0;t<pages;t++) {
    readFunc[apage+t]=empty;
    writeFunc[apage+t]=empty;
  }
  foo->setUnmapped();
  //DEBUG_OUT<<"(done unmapping)\n";
}



/*
void EMemMapSimple::clearVideoMap()
{
  for(unsigned int t=0;t<numPages;t++) setVWrite(t,(EMemAbstract *)NULL);
}
*/


/*
void EMemMapSimple::setVideoMap(EVideoIOAbstract *em)
{
  if(!em) { DEBUG_OUT<<"NULL EVideoIO passed to setVideoMap!\n"; return; }
  if(em->err.error()) {
    DEBUG_OUT<<"EVideoIO with error passed to setVideoMap!\n"; err.set();
    return;
  }
  ULONG addr=em->getLocation(),len=em->getSize();
  mapHelper(em);
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  ULONG loc=addr/pageSize;
  for(unsigned int t=0;t<pages;t++) setVWrite(loc+t,em);
}
*/


BYTE EMemMapSimple::read8(ULONG addr)
{
#ifdef CHECK_MASK
  addr=addr&addrMask;
  unsigned int apage=(addr>>pShift)&pMask;
#else
  unsigned int apage=(addr>>pShift);
#endif
#ifdef CHECK_NULL_PAGES
  if(!readFunc[apage]) { return 0; }
#endif // CHECK_NULL_PAGES
  BYTE ret=0;
  if(ioHandler) { if(!ioHandler->read8ret(addr,&ret)) { ret=readFunc[apage]->read8(addr); } }
  else { ret=readFunc[apage]->read8(addr); }
  return ret;
}


BYTE EMemMapSimple::write8(ULONG addr,BYTE data)
{
#ifdef CHECK_MASK
  addr=addr&addrMask;
  unsigned int apage=(addr>>pShift)&pMask;
#else
  unsigned int apage=(addr>>pShift);
#endif
#ifdef CHECK_NULL_PAGES
  if(!writeFunc[apage]) { return 0; }
#endif // CHECK_NULL_PAGES
  BYTE ret=0;
  videoFunc[apage]->write8(addr,data);
  if(ioHandler) { if(!ioHandler->write8ret(addr,data,&ret)) { ret=writeFunc[apage]->write8(addr,data); } }
  else { ret=writeFunc[apage]->write8(addr,data); }
  return ret;
}


// NOTE: This calls reallyWrite8 on readFunc pages!  Used to patch roms among other things...
BYTE EMemMapSimple::reallyWrite8(ULONG addr,BYTE data)
{
  DEBUG_OUT<<"EMemMapSimple::reallyWrite8("<<addr<<","<<(int)data<<")...\n";
#ifdef CHECK_MASK
  addr=addr&addrMask;
  unsigned int apage=(addr>>pShift)&pMask;
#else
  unsigned int apage=(addr>>pShift);
#endif
#ifdef CHECK_NULL_PAGES
  if(!readFunc[apage]) { return 0; }
#endif // CHECK_NULL_PAGES
  readFunc[apage]->reallyWrite8(addr,data);
  return write8(addr,data);
}

