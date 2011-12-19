
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define CHECK_MASK 1
#define CHECK_NULL_PAGES 1


#include <elib/EMemMapComplex.h>
#include <elib/EEmptyMem.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EMemIO.h>
#include <elib/EVideoIOAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#ifdef ALIB_HAVE_CTYPE_H
#include <ctype.h>
#endif // ALIB_HAVE_CTYPE_H


///////////////////////////////////////////////////////////////////////////////
//  EMemMapComplex Class
///////////////////////////////////////////////////////////////////////////////

EMemMapComplex::EMemMapComplex(ULONG npages,ULONG psize,ULONG sbits)
{
  DEBUG_OUT<<"EMemMapComplex(pages="<<npages<<",psize="<<psize<<",sbits="<<sbits<<") created.\n";
  // Init all data members
  init();
  constructorHelper(npages,psize,sbits);
}


// This actually does all the work...this is here so subclasses can use it
void EMemMapComplex::constructorHelper(ULONG npages,ULONG psize,ULONG sbits)
{
  name="mem_map";
  letter='*';
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
  sReadFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!sReadFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc sReadFunc!\n"; return;
  }
  sWriteFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!sWriteFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc sWriteFunc!\n"; return;
  }
  //
  vWriteFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!vWriteFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc vWriteFunc!\n"; return;
  }
  //
  origReadFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!origReadFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc origReadFunc!\n"; return;
  }
  origWriteFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!origWriteFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc origWriteFunc!\n"; return;
  }
  //
  saveReadFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!saveReadFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc saveReadFunc!\n"; return;
  }
  saveWriteFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!saveWriteFunc) {
    err.set(); DEBUG_OUT<<"Error! Map couldn't alloc saveWriteFunc!\n"; return;
  }
  //
  ramReadFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!ramReadFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc ramReadFunc!\n"; return;
  }
  ramWriteFunc=(EMemAbstract **)calloc(numPages,sizeof(EMemAbstract *));
  if(!ramWriteFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc ramWriteFunc!\n"; return;
  }
  //
  dontPanic=true;
  unsigned int t;
  for(t=0;t<numPages;t++) {
   setSRead(t,(EMemAbstract *)NULL);
   setSWrite(t,(EMemAbstract *)NULL);
   //setVWrite(t,(EMemAbstract *)NULL);
  }
  empty=new EEmptyMem(memsize);
  if(!empty) {
    std::cerr<<"Couldn't create empty area, bailing out!";
    err.set();
    return;
  }
  //empty->setMap(this);
  empty->setShowInMap(false);
  addArea(empty);  // to keep track of it...
  dontPanic=false;
  for(t=0;t<numPages;t++) {
    setSRead(t,empty);  setSWrite(t,empty);  //setVWrite(t,empty);
  }
  // so there's something there...
  //saveAllRam();
  //saveCurrentMap();
}


void EMemMapComplex::init()
{
  // Init all data members
  pageFaults=false;
  demandPaging=false;
  virtualMemory=false;
  inBusError=false;
  virtualGran=0;  poolStart=0;
  sReadFunc=(EMemAbstract **)NULL;
  sWriteFunc=(EMemAbstract **)NULL;
  //vWriteFunc=(EMemAbstract **)NULL;
  ramReadFunc=(EMemAbstract **)NULL;
  ramWriteFunc=(EMemAbstract **)NULL;
  saveReadFunc=(EMemAbstract **)NULL;
  saveWriteFunc=(EMemAbstract **)NULL;
  origReadFunc=(EMemAbstract **)NULL;
  origWriteFunc=(EMemAbstract **)NULL;
  snap0ReadFunc=(EMemAbstract **)NULL;
  snap0WriteFunc=(EMemAbstract **)NULL;
  snap1ReadFunc=(EMemAbstract **)NULL;
  snap1WriteFunc=(EMemAbstract **)NULL;
}


EMemMapComplex::~EMemMapComplex()
{
  if(err.error()) { DEBUG_OUT<<"map has error!  Can't delete!\n"; }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Destroying map... \n";
#endif // DEBUG_VERBOSE
  //
  if(sReadFunc) free(sReadFunc);
  sReadFunc=(EMemAbstract **)NULL;
  if(sWriteFunc) free(sWriteFunc);
  sWriteFunc=(EMemAbstract **)NULL;
  //if(vWriteFunc) free(vWriteFunc);
  //vWriteFunc=(EMemAbstract **)NULL;
  //
  if(ramReadFunc) free(ramReadFunc);
  ramReadFunc=(EMemAbstract **)NULL;
  if(ramWriteFunc) free(ramWriteFunc);
  ramWriteFunc=(EMemAbstract **)NULL;
  //
  if(origReadFunc) free(origReadFunc);
  origReadFunc=(EMemAbstract **)NULL;
  if(origWriteFunc) free(origWriteFunc);
  origWriteFunc=(EMemAbstract **)NULL;
  //
  if(saveReadFunc) free(saveReadFunc);
  saveReadFunc=(EMemAbstract **)NULL;
  if(saveWriteFunc) free(saveWriteFunc);
  saveWriteFunc=(EMemAbstract **)NULL;
  //
  if(snap0ReadFunc) free(snap0ReadFunc);
  snap0ReadFunc=(EMemAbstract **)NULL;
  if(snap0WriteFunc) free(snap0WriteFunc);
  snap0WriteFunc=(EMemAbstract **)NULL;
  if(snap1ReadFunc) free(snap1ReadFunc);
  snap1ReadFunc=(EMemAbstract **)NULL;
  if(snap1WriteFunc) free(snap1WriteFunc);
  snap1WriteFunc=(EMemAbstract **)NULL;
  if(empty) delete empty;
  empty=(EEmptyMem *)NULL;
  //
  modules.kill();
  areas.kill();
  symbols.kill();
  //
  init();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"map destroyed\n";
#endif // DEBUG_VERBOSE
}


/*
void EMemMapComplex::allocMapSaves()
{
  snap0ReadFunc=(EMemAbstract **)calloc(mapsize,sizeof(ERam *));
  if(!snap0ReadFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc snap0ReadFunc!\n"; return;
  }
  snap0WriteFunc=(EMemAbstract **)calloc(mapsize,sizeof(ERam *));
  if(!snap0WriteFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc snap0WriteFunc!\n";
    return;
  }
  snap1ReadFunc=(EMemAbstract **)calloc(mapsize,sizeof(ERam *));
  if(!snap1ReadFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc snap1ReadFunc!\n"; return;
  }
  snap1WriteFunc=(EMemAbstract **)calloc(mapsize,sizeof(ERam *));
  if(!snap1WriteFunc) {
    err.set(); DEBUG_OUT<<"Error!  Map couldn't alloc snap1WriteFunc!\n";
    return;
   }
}


void EMemMapComplex::saveAllRam()
{
  if(!ramReadFunc) {
    DEBUG_OUT<<"Couldn't saveAllRam, no ramReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!ramReadFunc) {
    DEBUG_OUT<<"Couldn't saveAllRam, no ramWriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to ram "<<mapsize<<" pages to snapshot 0...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    ramReadFunc[t]=sReadFunc[t];
    ramWriteFunc[t]=sWriteFunc[t];
  }
}


void EMemMapComplex::setAllRam()
{
  if(!ramReadFunc) {
    DEBUG_OUT<<"Couldn't setAllRam, no ramReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!ramReadFunc) {
    DEBUG_OUT<<"Couldn't setAllRamp, no ramWriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to restore "<<mapsize<<" pages from snapshot 0...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    sReadFunc[t]=ramReadFunc[t];
    sWriteFunc[t]=ramWriteFunc[t];
  }
}


void EMemMapComplex::saveOrigMap()
{
  if(!origReadFunc) {
    DEBUG_OUT<<"Couldn't saveOrigMap, no ramReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!origReadFunc) {
    DEBUG_OUT<<"Couldn't saveOrigMap, no ramWriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to ram "<<mapsize<<" pages to snapshot 0...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    origReadFunc[t]=sReadFunc[t];
    origWriteFunc[t]=sWriteFunc[t];
  }
}


void EMemMapComplex::setOrigMap()
{
  if(!origReadFunc) {
    DEBUG_OUT<<"Couldn't setOrigMap, no ramReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!origReadFunc) {
    DEBUG_OUT<<"Couldn't setOrigMapp, no ramWriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to restore "<<mapsize<<" pages from snapshot 0...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    sReadFunc[t]=origReadFunc[t];
    sWriteFunc[t]=origWriteFunc[t];
  }
}


void EMemMapComplex::saveCurrentMap()
{
  if(!saveReadFunc) {
    DEBUG_OUT<<"Couldn't saveCurrentMap, no saveReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!saveReadFunc) {
    DEBUG_OUT<<"Couldn't saveCurrentMap, no saveWriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to save "<<mapsize<<" pages to snapshot 0...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    saveReadFunc[t]=sReadFunc[t];
    saveWriteFunc[t]=sWriteFunc[t];
  }
}


void EMemMapComplex::restoreSavedMap()
{
  if(!saveReadFunc) {
    DEBUG_OUT<<"Couldn't restoreSavedMap, no saveReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!saveReadFunc) {
    DEBUG_OUT<<"Couldn't restoreSavedMap, no saveWriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to restore "<<mapsize<<" pages from snapshot 0...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    sReadFunc[t]=saveReadFunc[t];
    sWriteFunc[t]=saveWriteFunc[t];
  }
}


void EMemMapComplex::takeSnapshot0()
{
  if(!snap0ReadFunc) {
    DEBUG_OUT<<"Couldn't takeSnapshot0, no snap0ReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!snap0ReadFunc) {
    DEBUG_OUT<<"Couldn't takeSnapshot0, no snap0WriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to save "<<mapsize<<" pages to snapshot 0...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    snap0ReadFunc[t]=sReadFunc[t];
    snap0WriteFunc[t]=sWriteFunc[t];
  }
}


void EMemMapComplex::restoreSnapshot0()
{
  if(!snap0ReadFunc) {
    DEBUG_OUT<<"Couldn't restoreSnapshot0, no snap0ReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!snap0ReadFunc) {
    DEBUG_OUT<<"Couldn't restoreSnapshot0, no snap0WriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to restore "<<mapsize<<" pages from snapshot 0...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    sReadFunc[t]=snap0ReadFunc[t];
    sWriteFunc[t]=snap0WriteFunc[t];
  }
}


void EMemMapComplex::takeSnapshot1()
{
  if(!saveReadFunc) {
    DEBUG_OUT<<"Couldn't takeSnapshot1, no saveReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!saveReadFunc) {
    DEBUG_OUT<<"Couldn't takeSnapshot1, no saveWriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to save "<<mapsize<<" pages to snapshot 1...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    saveReadFunc[t]=sReadFunc[t];
    saveWriteFunc[t]=sWriteFunc[t];
  }
}


void EMemMapComplex::restoreSnapshot1()
{
  if(!saveReadFunc) {
    DEBUG_OUT<<"Couldn't restoreSnapshot1, no saveReadFunc!\n";
    warnAlloc();
    return;
  }
  if(!saveReadFunc) {
    DEBUG_OUT<<"Couldn't restoreSnapshot1, no saveWriteFunc!\n";
    warnAlloc();
    return;
  }
  //DEBUG_OUT<<"Going to restore "<<mapsize<<" pages from snapshot 1...\n";
  for(unsigned int t=0;t<mapsize;t++) {
    sReadFunc[t]=saveReadFunc[t];
    sWriteFunc[t]=saveWriteFunc[t];
  }
}
*/


void EMemMapComplex::setVWrite(ULONG addr,EMemAbstract *foo)
{
#ifdef CHECK_NULL_PAGES
  if(err.error()) return;
  if((!foo)&&(!dontPanic)) {
    DEBUG_OUT<<"ERROR: setVWrite, just set a NULL page at ";
    DEBUG_OUT<<aDefaultStringHelper.myHex(addr)<<"!\n";
    std::cerr<<"setVrite, just set a NULL page!";
    err.set();
  }
#endif
#ifdef CHECK_MASK
  addr=addr&pMask;
#endif
  vWriteFunc[addr]=(ERam *)foo;
}


void EMemMapComplex::setSWrite(ULONG addr,EMemAbstract *foo)
{
#ifdef CHECK_NULL_PAGES
  if(err.error()) return;
  if((!foo)&&(!dontPanic)) {
    DEBUG_OUT<<"ERROR: setSWrite, just set a NULL page at ";
    DEBUG_OUT<<aDefaultStringHelper.myHex(addr)<<"!\n";
    std::cerr<<"setSWrite, just set a NULL page!";
    err.set();
  }
#endif
#ifdef CHECK_MASK
  addr=addr&pMask;
#endif
  sWriteFunc[addr]=(ERam *)foo;
}


void EMemMapComplex::setSRead(ULONG addr,EMemAbstract *foo)
{
#ifdef CHECK_NULL_PAGES
  if(err.error()) return;
  if((!foo)&&(!dontPanic)) {
    DEBUG_OUT<<"ERROR: setSRead, just set a NULL page at ";
    DEBUG_OUT<<aDefaultStringHelper.myHex(addr)<<"!\n";
    std::cerr<<"setSRead, just set a NULL page!";
    err.set();
  }
#endif
#ifdef CHECK_MASK
  addr=addr&pMask;
#endif
  sReadFunc[addr]=(ERam *)foo;
}


char EMemMapComplex::getVPageLetter(unsigned int page)
{
#ifdef CHECK_MASK
  page=page&pMask;
#endif
  if(vWriteFunc[page]) return vWriteFunc[page]->getLetter();
  return 'x';
}


char EMemMapComplex::getWPageLetter(unsigned int page)
{
#ifdef CHECK_MASK
  page=page&pMask;
#endif
  if(sWriteFunc[page]) return sWriteFunc[page]->getLetter();
  return 'x';
}


char EMemMapComplex::getRPageLetter(unsigned int page)
{
#ifdef CHECK_MASK
  page=page&pMask;
#endif
  if(sReadFunc[page]) return sReadFunc[page]->getLetter();
  return 'x';
}


void EMemMapComplex::map(EMemAbstract *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMem passed to map!\n"; return; }
  if(foo->err.error()) {
    DEBUG_OUT<<"EMem with error passed to map!\n"; err.set(); return;
  }
  if(foo->reallyVideo()) {
    warnVideo(); DEBUG_OUT<<"EVideo passed to map!\n"; err.set(); return;
  }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  if(!foo->isMapped()) mapHelper(foo);
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  ULONG loc=addr/pageSize;
  if(!foo->isMapped()) {
    for(unsigned int t=0;t<pages;t++) {
      origReadFunc[loc+t]=sReadFunc[loc+t];
      origWriteFunc[loc+t]=sWriteFunc[loc+t];
    }
  }
  for(unsigned int t=0;t<pages;t++) {
    setSRead(loc+t,foo);
    setSWrite(loc+t,foo);
  }
  if(!foo->isMapped()) addArea(foo);
  foo->setMapped();
  //DEBUG_OUT<<"(done mapping)\n";
}


void EMemMapComplex::map(ERom *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL ERom passed to map!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"ERom with error passed to map!\n"; return; }
  if(foo->reallyVideo()) {
    warnVideo(); DEBUG_OUT<<"EVideo passed to map!\n"; err.set(); return;
  }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  if(!foo->isMapped()) mapHelper(foo);
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  ULONG loc=addr/pageSize;
  if(!foo->isMapped()) {
    for(unsigned int t=0;t<pages;t++) {
      origReadFunc[loc+t]=sReadFunc[loc+t];
      // NOTE: ROMS are only mapped as readable, not writable
      // ...so we leave the SWrite set to whatever it was...
    }
  }
  for(unsigned int t=0;t<pages;t++) {
    setSRead(loc+t,foo);
    // NOTE: ROMS are only mapped as readable, not writable
    // ...so we leave the SWrite set to whatever it was...
  }
  if(!foo->isMapped()) addArea(foo);
  foo->setMapped();
  //DEBUG_OUT<<"(done mapping)\n";
}


void EMemMapComplex::map(EMemIO *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMemIO passed to map!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EMemIO with error passed to map!\n"; return; }
  if(foo->reallyVideo()) {
    warnVideo(); DEBUG_OUT<<"EVideo passed to map!\n"; err.set(); return;
  }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  if(!foo->isMapped()) mapHelper(foo);
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  ULONG loc=addr/pageSize;
  if(!foo->isMapped()) {
    for(unsigned int t=0;t<pages;t++) {
      origReadFunc[loc+t]=sReadFunc[loc+t];
      origWriteFunc[loc+t]=sWriteFunc[loc+t];
    }
  }
  for(unsigned int t=0;t<pages;t++) {
    setSRead(loc+t,foo);
    setSWrite(loc+t,foo);
  }
  if(!foo->isMapped()) addArea(foo);
  foo->setMapped();
  //DEBUG_OUT<<"(done mapping)\n";
}


void EMemMapComplex::mapVideo(EVideoIOAbstract *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EVideoIOAbstract passed to map!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EVideoIOAbstract with error passed to map!\n"; return; }
  ULONG addr=foo->getLocation(),len=foo->getSize();
  if(!foo->isMapped()) mapHelper(foo);
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  ULONG loc=addr/pageSize;
  if(!foo->isMapped()) {
    for(unsigned int t=0;t<pages;t++) {
      vWriteFunc[loc+t]=sWriteFunc[loc+t];
    }
  }
  for(unsigned int t=0;t<pages;t++) {
    setSRead(loc+t,foo);
    setVWrite(loc+t,foo);
  }
  if(!foo->isMapped()) addArea(foo);
  //foo->setMapped();
  //DEBUG_OUT<<"(done mapping)\n";
}


// NOTE: Remember to give your areas unique names or they
// won't be removed from the list correctly...
void EMemMapComplex::mapHelper(EMemAbstract *foo)
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
void EMemMapComplex::unmap(EMemAbstract *foo)
{
  if(!foo) { DEBUG_OUT<<"NULL EMem passed to unmap!\n"; return; }
  if(foo->err.error()) { DEBUG_OUT<<"EMem with error passed to unmap!\n"; return; }
  if(foo->reallyVideo()) {
    warnVideo(); DEBUG_OUT<<"EVideo passed to unmap!\n"; err.set(); return;
  }
  /*
  if(!foo->isMapped()) {
    // DEBUG_OUT<<"No need to unmap..."<<foo->getName()<<" not mapped!\n";
    return;
  }
  */
  ULONG addr=foo->getLocation(),len=foo->getSize();
  //DEBUG_OUT<<"Going to unmap "<<foo->getName()<<" at ";
  //DEBUG_OUT<<addr<<" for "<<len<<" bytes.\n";
  addr=addr&addrMask;
  unsigned int pages=len/pageSize;
  if((pages*pageSize)<len) pages++;
  ULONG loc=addr/pageSize;
  //DEBUG_OUT<<"(Located at page "<<loc<<" for "<<pages<<" pages)\n";
  for(unsigned int t=0;t<pages;t++) {
    setSRead(loc+t,origReadFunc[loc+t]);
    setSWrite(loc+t,origWriteFunc[loc+t]);
  }
  foo->setUnmapped();
  //DEBUG_OUT<<"(done unmapping)\n";
}


BYTE EMemMapComplex::read8(ULONG addr)
{
#ifdef CHECK_MASK
  addr=addr&addrMask;
  unsigned int apage=(addr>>pShift)&pMask;
#else
  unsigned int apage=(addr>>pShift);
#endif
#ifdef CHECK_NULL_PAGES
  if(!sReadFunc[apage]) { return 0; }
#endif // CHECK_NULL_PAGES
  return sReadFunc[apage]->read8(addr);
}


BYTE EMemMapComplex::write8(ULONG addr,BYTE data)
{
#ifdef CHECK_MASK
  addr=addr&addrMask;
  unsigned int apage=(addr>>pShift)&pMask;
#else
  unsigned int apage=(addr>>pShift);
#endif
#ifdef CHECK_NULL_PAGES
  if(!sWriteFunc[apage]) { return 0; }
#endif // CHECK_NULL_PAGES
  sWriteFunc[apage]->write8(addr,data);
  if(vWriteFunc[apage]) vWriteFunc[apage]->write8(addr,data);
  return data;
}


// NOTE: This calls write on readFunc pages!  Used to patch roms among other things...
BYTE EMemMapComplex::reallyWrite8(ULONG addr,BYTE data)
{
  DEBUG_OUT<<"EMemMapComplex::readllyWrite8("<<addr<<","<<(int)data<<")...\n";
#ifdef CHECK_MASK
  addr=addr&addrMask;
  unsigned int apage=(addr>>pShift)&pMask;
#else
  unsigned int apage=(addr>>pShift);
#endif
#ifdef CHECK_NULL_PAGES
  if(!sReadFunc[apage]) { return 0; }
  if(!sWriteFunc[apage]) { return 0; }
#endif // CHECK_NULL_PAGES
  sWriteFunc[apage]->write8(addr,data);
  sReadFunc[apage]->reallyWrite8(addr,data);
  if(vWriteFunc[apage]) vWriteFunc[apage]->write8(addr,data);
  return data;
}


bool EMemMapComplex::testMapped(ULONG addr,ULONG sz)
{
  bool ret=true;
  unsigned int pages=sz/pageSize;
  if((pages*pageSize)<sz) pages++;
  unsigned int start=addr/pageSize;
  for(unsigned int t=start;t<(start+pages);t++) { if(getRPageLetter(t)=='X') ret=false; }
  return ret;
}


bool EMemMapComplex::testMappedWritable(ULONG addr,ULONG sz)
{
  bool ret=true;
  unsigned int pages=sz/pageSize;
  if((pages*pageSize)<sz) pages++;
  unsigned int start=addr/pageSize;
  for(unsigned int t=start;t<(start+pages);t++) { if(getWPageLetter(t)=='X') ret=false; }
  return ret;
}


void EMemMapComplex::setPageFaults(bool f)
{
  // If this is true busError on access of unallocated space in the map
  pageFaults=f;
  if(empty) empty->setWantBusErrors(f);
}


void EMemMapComplex::setDemandPaging(bool f)
{
  // If this is true we will demand load binaries
  demandPaging=f;
  pageFaults=f;
  if(empty) empty->setWantBusErrors(f);
}


// TODO: There's a bug if you have virtual memory with the video map
// (Call destroyVideoMap if you can as a workaround)
void EMemMapComplex::setVirtualMemory(bool f)
{
  // If this is true we'll alloc ram when we're asked to access
  // an unused part of the map
  virtualMemory=f;
  pageFaults=f;
  if(empty) empty->setWantBusErrors(f);
}


// returns true if should try to reaccess address...
bool EMemMapComplex::busError(EMemAbstract *m,ULONG maddr,unsigned int mdata,unsigned int msize,unsigned int maccess)
{
  bool ret=false;
  if(inBusError) {
    inBusError=false;
    DEBUG_OUT<<"Double fault in bus error!\n";
    err.set();
    return false;
  }
  inBusError=true;
  if(virtualMemory) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Going to try to map new ram there...\n";
#endif // DEBUG_VERBOSE
    ret=mapNewRam(maddr);
    if(ret) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Tell them to try again.\n";
#endif // DEBUG_VERBOSE
      return true;
    }
  }
  else {
    DEBUG_OUT<<m->getName()<<" : bus error at "<<aDefaultStringHelper.myHex((unsigned long)maddr)<<"...\n";
  }
  if(true) {
  //if(pageFaults) {
    debug=true;
    return false;
  }
  return false;
}


// TODO: does this ever get called when virtual memory isn't on?
// Do we want to add it to the map's area list or not?
bool EMemMapComplex::mapNewRam(ULONG maddr)
{
  if(maddr) maddr=(maddr/pageSize)*pageSize;  // put on a page boundery
  //if(w) *w<<"Placing one page of ram at "<<maddr<<"\n";
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Placing one page of ram at "<<aDefaultStringHelper.myHex((unsigned long)maddr)<<"\n";
#endif // DEBUG_VERBOSE
  ERam *r=new ERam(1,pageSize);
  if(!r) {
    DEBUG_OUT<<"map::mNR ERam new failed...\n";
    return false;
  }
  if(r->err.error()) {
    DEBUG_OUT<<"map::mNR ERam error...\n";
    delete r;
    return false;
  }
  r->setLocation(maddr);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Mapping new ram page ("<<r->getSize()<<" at ";
  DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)r->getLocation())<<").\n";
#endif // DEBUG_VERBOSE
  //r->setShowInMap(false);
  map(r);
  return true;
}


/*
void EMemMapComplex::setVideoMap(EVideoIOAbstract *em)
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


/*
void EMemMapComplex::clearVideoMap()
{
  for(unsigned int t=0;t<numPages;t++) setVWrite(t,(EMemAbstract *)NULL);
}
*/


void EMemMapComplex::warnVideo()
{
  std::cerr<<"Warning: EVideoIO passed to map/unmap!";
}


void EMemMapComplex::warnAlloc()
{
  std::cerr<<"Warning: map saves not alloc'd!";
}


const char *EMemMapComplex::getMapType()
{
  return "EMemMapComplex";
}


