
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NPalmAppDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <elib/EMemMapAbstract.h>
#include <elib/EResource.h>
#include <elib/EResourceFile.h>
#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NPalmAppDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NPalmAppDecoder::recognize(unsigned char *buffer,char *fname)
{
  DEBUG_OUT<<"NPalmAppDecoder looking for resources in file...\n";
  EResourceFile *rsrc=new EResourceFile(fname);
  if(!rsrc) return false;
  if(rsrc->err.error()) { delete rsrc; return false; }
  DEBUG_OUT<<"NPalmAppDecoder got a good resource file...\n";
  bool code=false,cfrg=false;
  if(rsrc->count("CODE")) code=true;
  if(rsrc->count("cfrg")) cfrg=true;
  delete rsrc;
  rsrc=(EResourceFile *)NULL;
  if(code) DEBUG_OUT<<"(Found"; else DEBUG_OUT<<"(Didn't find";
  DEBUG_OUT<<" a CODE resource)\n";
  if(cfrg) DEBUG_OUT<<"(Found"; else DEBUG_OUT<<"(Didn't find";
  DEBUG_OUT<<" a cfrg resource)\n";
  bool good=false;
  if(code) good=true;
  if(cfrg) good=true;
  return good;
}


NPalmAppDecoder::NPalmAppDecoder()
{
  init();
  arch=N_LD_ARCH_M68K;  // for starters
  ersrc=(EResourceFile *)NULL;
}


char *NPalmAppDecoder::whichCPU()
{
  return archString();
}


char *NPalmAppDecoder::whichIO()
{
  return "palm";
}


bool NPalmAppDecoder::verifyOkay()
{
  DEBUG_OUT<<"PalmApp verify...\n";
  if(!checkSanity()) return false;
  LSB=false;
  needsReloc=true;
  ersrc=new EResourceFile(fname);
  if(!ersrc) return false;
  if(ersrc->err.error()) { delete ersrc; return false; }
  DEBUG_OUT<<"NPalmAppDecoder got a good resource file...\n";
  arch=N_LD_ARCH_M68K;  // by default...
  bool good=false;
  bool foundCODE=false;
  if(ersrc->count("CODE")) {
    good=true; foundCODE=true;
    arch=N_LD_ARCH_M68K;
  }
  return good;
}


// NOTE: any addresses we specify for the nodes are ignored because
// NLD::loadNodes will reloc all of them since we specify that we needReloc!
bool NPalmAppDecoder::decodeNodes()
{
  DEBUG_OUT<<"(PalmApp decodeNodes)\n";
  if(!checkSanity()) return false;
  ld->reset();
  // Load all resources...
  AList *res=ersrc->getMap();
  unsigned int rsize=0,roffset=0,t=0;
  unsigned int curloc=64*1024;
  if(binsize) {
    DEBUG_OUT<<"There were "<<aDefaultStringHelper.myHex((unsigned long)binsize);
    DEBUG_OUT<<" bytes in the data fork, skip past them\n";
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
      DEBUG_OUT<<"Creating resource node: "<<rsize<<"@";
      DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)curloc)<<"\n";
      n=new NBinaryNode(r->type,r->num,N_BIN_PROGBITS,curloc,rsize,
        curloc,E_MEM_READ|E_MEM_EXEC);
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
  return true;
}



