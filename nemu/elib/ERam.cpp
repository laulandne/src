
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#define CHECK_SIZE 1


#include <elib/ERam.h>

#include <portable/nickcpp/ANullStream.h>


///////////////////////////////////////////////////////////////////////////////
//  ERam Class
///////////////////////////////////////////////////////////////////////////////


ERam::ERam(unsigned int tsize)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ERam("<<tsize<<")...\n";
#endif // DEBUG_VERBOSE
  init();
  memsize=tsize;
  mem=NULL;
  allocSize=0;
  if(memsize) {
    unsigned int num=tsize/1024;
    if((num*1024)<tsize) num++;
    DEBUG_OUT<<"ERam going to acalloc "<<num<<"k...\n";
    mem=(BYTE *)acalloc(num,1024);
    allocSize=1024*num;
    if(!mem) { err.set();  DEBUG_OUT<<"ERam(x) acalloc("<<allocSize<<") failed!\n"; }
  }
}


ERam::ERam(ULONG addr, unsigned int tsize)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ERam("<<addr<<","<<tsize<<")...\n";
#endif // DEBUG_VERBOSE
  init();
  memsize=tsize;
  mem=NULL;
  allocSize=0;
  if(memsize) {
    unsigned int num=tsize/1024;
    if((num*1024)<tsize) num++;
    DEBUG_OUT<<"ERam going to acalloc "<<num<<"k...\n";
    mem=(BYTE *)acalloc(num,1024);
    allocSize=1024*num;
    if(!mem) { err.set();  DEBUG_OUT<<"ERam(x,y) acalloc("<<allocSize<<") failed!\n"; }
  }
  location=addr;
}


ERam::ERam(ULONG addr, unsigned int numblks, unsigned int blksize)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ERam("<<addr<<","<<numblks<<","<<blksize<<")...\n";
#endif // DEBUG_VERBOSE
  init();
  DEBUG_OUT<<"ERam going to acalloc "<<numblks<<" blocks of "<<blksize<<"...\n";
  mem=(BYTE *)acalloc(numblks,blksize);
  if(!mem) { err.set();  DEBUG_OUT<<"ERam(x,y,z) acalloc("<<allocSize<<") failed!\n"; }
  allocSize=blksize*numblks;
  location=addr;
  ULONG tsize=numblks*blksize;
  unsigned int ttsize=(unsigned int)tsize;
  if(ttsize!=tsize) {
    DEBUG_OUT<<"NOTE: 16-bit int overflow...\n";
    ttsize=65535;
    allocSize=ttsize;
  }
  memsize=ttsize;
}


ERam::~ERam()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Destroying ERam\n";
#endif // DEBUG_VERBOSE
  if(err.error()) { DEBUG_OUT<<"ERam with error!\n"; return; }
  freeMem();
  err.set();
}


void ERam::init()
{
  EMemAbstract::init();
  letter='r';
  name="ram";
  allocSize=0;
  mem=(BYTE *)NULL;
}


// Remember the size we alloc'd is in allocSize if we need it.
void ERam::freeMem()
{
  if(err.error()) { DEBUG_OUT<<"EMem with error!\n"; return; }
  if(mem) free(mem);
  mem=NULL;
}


void ERam::resize(ULONG newsize)
{
  reallyResize(newsize);
}


// TODO: Never call this on a mapped area unless you
// unmap it first!
void ERam::reallyResize(ULONG newsize)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Resize EMem ("<<memsize<<") to "<<newsize<<"...\n";
  //DEBUG_OUT<<"(Contents will be lost)\n";
#endif // DEBUG_VERBOSE
  if(mapped) DEBUG_OUT<<"YIKES!  We're mapped!  Procede with caution!\n";
  if(!newsize) { err.set(); DEBUG_OUT<<"zero sized mem!\n"; return; }
  if(newsize) {
    unsigned int num=newsize/1024;
    if((num*1024)<newsize) num++;
    BYTE *mem2=(BYTE *)acalloc(num,1024);
    allocSize=1024*num;
    if(!mem2) {
      err.set();  DEBUG_OUT<<"EMem reallyResize acalloc("<<allocSize<<") failed!\n";
      return;
    }
    if(memsize) {
      unsigned int s=memsize;
      if(s>newsize) s=newsize;
      // TODO: Could make this much more efficient...or use remalloc or whatever
      for(unsigned int q=0;q<s;q++) mem2[q]=mem[q];
    }
    memsize=newsize;
    if(mem) free(mem);
    mem=mem2;
  }
}


BYTE ERam::read8(ULONG addr)
{
  //DEBUG_OUT<<"r";
  if(!mem) return 0;
#ifdef CHECK_SIZE
  unsigned int ad=addr-location;
  if(ad>memsize) return 0;
  return mem[ad];
#else
  return mem[addr-location];
#endif
}


BYTE ERam::write8(ULONG addr,BYTE data)
{
  if(!mem) return 0;
#ifdef CHECK_SIZE
  unsigned int ad=addr-location;
  if(ad>memsize) {
    DEBUG_OUT<<"Attempt to write past end of "<<name<<" (";
    DEBUG_OUT<<memsize<<") at "<<ad<<".\n";
    return 0;
  }
  mem[ad]=data;
#else
  mem[addr-location]=data;
#endif
  return data;
}

