
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NMacAppDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <elib/EMemMapAbstract.h>
#include <elib/EResource.h>
#include <elib/EResourceFile.h>
#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NMacAppDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NMacAppDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NMacAppDecoder looking for resources in file...\n";
#endif // DEBUG_VERBOSE
  EResourceFile *rsrc=new EResourceFile(fname);
  bool resForkGood=false;
  bool code=false,cfrg=false,peff=false;
  if(rsrc) { if(!rsrc->err.error()) resForkGood=true; }
  if(!resForkGood) {
    if((buffer[0]=='J')&&(buffer[1]=='o')&&(buffer[2]=='y')&&(buffer[3]=='!')) {
      if((buffer[4]=='p')&&(buffer[5]=='e')&&(buffer[6]=='f')&&(buffer[7]=='f')) {
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"(Looks like a PEF file missing its resource fork)\n";
#endif // DEBUG_VERBOSE
        peff=true;
        return true;
      }
    }
    if(rsrc) delete rsrc;
    return false;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NMacAppDecoder got a good file...\n";
#endif // DEBUG_VERBOSE
  if(rsrc->count("CODE")) code=true;
  if(rsrc->count("cfrg")) cfrg=true;
  delete rsrc;
  rsrc=(EResourceFile *)NULL;
#ifdef DEBUG_VERBOSE
  if(code) DEBUG_OUT<<"(Found"; else DEBUG_OUT<<"(Didn't find";
  DEBUG_OUT<<" a CODE resource)\n";
  if(cfrg) DEBUG_OUT<<"(Found"; else DEBUG_OUT<<"(Didn't find";
  DEBUG_OUT<<" a cfrg resource)\n";
  if(peff) DEBUG_OUT<<"(Found"; else DEBUG_OUT<<"(Didn't find";
  DEBUG_OUT<<" a PEF file)\n";
#endif // DEBUG_VERBOSE
  bool good=false;
  if(code) good=true;
  if(cfrg) good=true;
  if(peff) good=true;
  return good;
}


NMacAppDecoder::NMacAppDecoder()
{
  init();
  arch=N_LD_ARCH_M68K;  // for starters
  ersrc=(EResourceFile *)NULL;
  hasSIZE=false;
  hasCFRG=false;
  hasPEF=false;
}

char *NMacAppDecoder::whichCPU()
{
  return archString();
}


char *NMacAppDecoder::whichIO()
{
  if(arch==N_LD_ARCH_M68K) return "mac68k";
  else return "powermac";
}


bool NMacAppDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"macApp verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  LSB=false;
  needsReloc=true;
  badResFork=false;
  ersrc=new EResourceFile(fname);
  if(!ersrc) badResFork=true;
  else {
    if(ersrc->err.error()) { delete ersrc; badResFork=true; }
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NMacAppDecoder got a good file...\n";
#endif // DEBUG_VERBOSE
  arch=N_LD_ARCH_M68K;  // by default...
  bool good=false;
  bool foundCODE=false;
  if(!badResFork) {
    if(ersrc->count("cfrg")) { good=true; hasCFRG=true; }
    if(ersrc->count("SIZE")) { hasSIZE=true; }
    if(ersrc->count("CODE")) {
      good=true; foundCODE=true;
      if(!hasCFRG) arch=N_LD_ARCH_M68K;
    }
  }
  if((binmem[0]=='J')&&(binmem[1]=='o')&&(binmem[2]=='y')&&(binmem[3]=='!')) {
    if((binmem[4]=='p')&&(binmem[5]=='e')&&(binmem[6]=='f')&&(binmem[7]=='f')) {
        DEBUG_OUT<<"(Looks like a PEF file missing its resource fork)\n";
        hasPEF=true;
        good=true;
    }
  }
  if((good)&&(!foundCODE)) {
    // NOTE: Not necessarily true...what about cfm68k?
    arch=N_LD_ARCH_POWERPC;
  }
  return good;
}


// NOTE: any addresses we specify for the nodes are ignored because
// NLD::loadNodes will reloc all of them since we specify that we needReloc!
bool NMacAppDecoder::decodeNodes()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"(macApp decodeNodes)\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  ld->reset();
  if(badResFork) { return decodePEFHeader(0,0); }
  // Load all resources...
  AList *res=ersrc->getMap();
  unsigned int rsize=0,roffset=0,t=0;
  unsigned int curloc=64*1024;
  if(binsize) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"There were "<<aDefaultStringHelper.myHex((unsigned long)binsize);
    DEBUG_OUT<<" bytes in the data fork, skip past them\n";
#endif // DEBUG_VERBOSE
    curloc+=binsize;
  }
  NBinaryNode *cfrg0=(NBinaryNode *)NULL;
  NBinaryNode *n=(NBinaryNode *)NULL;
  EResource *r=(EResource *)NULL;
  bool reading=true;
  while(reading) {
    //aDefaultStringHelper.yieldToOtherProcesses();
    if(res->atEnd()) reading=false;
    else {
      r=(EResource *)res->info();
      rsize=r->size;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Creating resource node: "<<rsize<<"@";
      DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)curloc)<<"\n";
#endif // DEBUG_VERBOSE
      n=new NBinaryNode(r->type,r->num,N_BIN_PROGBITS,curloc,rsize,curloc,E_MEM_READ|E_MEM_EXEC);
      if(n) {
        ld->getModule()->getNodes()->append(n);
        BYTE *buf=(BYTE *)malloc(rsize);
        if(buf) {
          ersrc->readFromID(r->type,r->num,buf);
          // Let map know what's coming...
          //map->preallocVirtMem(getImageOffset()+curloc,rsize);
          for(unsigned int a=0;a<rsize;a++) {
            map->write8(getImageOffset()+curloc+a,buf[a]);
          }
          free(buf);
        }
        // if it's cfrg #0 remember it for the next step...
        if((!strcmp(r->type,"cfrg"))&&!r->num) cfrg0=n;
      }
      else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
      curloc+=rsize;
    }
    res->advance();
  }
  // Now, if we had a cfrg #0 resource load any fragments it specifies...
  // TODO: Also if hasPEF is set, read them from the data fork as we can
  if(cfrg0) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"We've got a cfrg 0, so let's load the frags!\n";
#endif // DEBUG_VERBOSE
    curloc=64*1024;
    unsigned int srcloc=0;
    unsigned int foffset=getImageOffset()+cfrg0->addr+28;
    unsigned int numFrags=map->read32b(foffset);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"numFrags is "<<numFrags<<"\n";
#endif // DEBUG_VERBOSE
    foffset+=4;
    char isa[5];
    isa[4]=0;
    char theName[256];
    unsigned int libAlis=0,where=0,fragOffset=0,fragLen=0,thisSize=0,theNameLen=0;
    unsigned int toffset=0;
    unsigned int fragNum=0;
    for(t=0;t<numFrags;t++) {
      toffset=foffset;
      isa[0]=map->read8(foffset);  foffset++;
      isa[1]=map->read8(foffset);  foffset++;
      isa[2]=map->read8(foffset);  foffset++;
      isa[3]=map->read8(foffset);  foffset++;
      foffset+=4;  // skip update flag
      foffset+=4;  // skip cur version
      foffset+=4;  // skip oldest def version
      foffset+=4;  // skip stack size
      libAlis=map->read16b(foffset);  foffset+=2;
      foffset++;  // skip frag type
      where=map->read8(foffset);  foffset++;
      fragOffset=map->read32b(foffset);  foffset+=4;
      fragLen=map->read32b(foffset);  foffset+=4;
      if(!fragLen) {
        // 0 specifies the entire data fork...
        fragLen=binsize;  // we already know this
      }
      foffset+=8;  // skip reserved
      thisSize=map->read16b(foffset);  foffset+=2;
      theNameLen=map->read8(foffset);  foffset++;
      unsigned int i=0;
      for(i=0;i<256;i++) theName[i]=0;
      for(i=0;i<theNameLen;i++) theName[i]=map->read8(foffset+i);
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"frag: isa="<<isa<<" off="<<fragOffset<<" len="<<fragLen<<" name="<<theName<<"\n";
#endif // DEBUG_VERBOSE
      handlePEF(theName,fragNum,fragOffset,fragLen,curloc,srcloc);
      foffset=toffset+thisSize;
      curloc+=fragLen; srcloc+=fragLen;
      fragNum++;
    }
  }
  return true;
}


void NMacAppDecoder::handlePEF(char *name,unsigned int num,ULONG off,unsigned int len,
  ULONG loc,unsigned int src)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Creating frag node: "<<len<<"@";
  DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)loc)<<"\n";
#endif // DEBUG_VERBOSE
  decodePEFHeader(off,len);
  NBinaryNode *n=new NBinaryNode(name,num,N_BIN_PROGBITS,loc,len,
    src,E_MEM_READ|E_MEM_EXEC);
  if(n) {
    ld->getModule()->getNodes()->append(n);
  }
}


// NOTE: len can be zero if we don't have a clue
bool NMacAppDecoder::decodePEFHeader(ULONG off,ULONG len)
{
  DEBUG_OUT<<"decodePEFHeader not implemented!";
  return false;
}



