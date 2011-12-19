
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <elib/EOSHelper.h>
#include <elib/EOSEntry.h>
#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  EOSHelper Class
////////////////////////////////////////////////////////////////////////////////


EOSHelper::EOSHelper(EHandler *parent)
{
  init();
  io=parent;
  reset();
}


void EOSHelper::init()
{
  io=(EHandler *)NULL;
  map=(EMemMapAbstract *)NULL;
  cpu=(ECPUAbstract *)NULL;
  dos=(DVfs *)NULL;
  osCalls=new AList;
}


EOSHelper::~EOSHelper()
{
  // TODO: free kernal entries
  delete osCalls;
  osCalls=(AList *)NULL;
}


bool EOSHelper::systemCall(ULONG loc)
{
  // We didn't handle it...
  return false;
}


// TODO: This only works with 6502 cpu's due to the hard coded jumps!
// TODO: This is also hard coded to the commodore kern table format!
void EOSHelper::patchAll(ULONG *nDos)
{
  reset();
  DEBUG_OUT<<"EOSHelper attempting to patch kernel...\n";
  if(!map) {
    DEBUG_OUT<<"No map...can't patch!\n";
    return;
  }
  EOSEntry *entry=(EOSEntry *)NULL;
  unsigned int t=0;
  DEBUG_OUT<<"EOSHelper entries:\n";
  bool doPatch=false;
  bool reallyPatch=false;
  bool working=true;
  while(working) {
    if(nDos[t*EOSENTRY_PATCH_SIZE]) {
      entry=new EOSEntry;
      if(entry) {
        entry->name=(char *)nDos[t*EOSENTRY_PATCH_SIZE];
        DEBUG_OUT<<"Looking at: "<<entry->name<<"...\n";
        entry->origAddr=(unsigned int)nDos[t*EOSENTRY_PATCH_SIZE+1];
        doPatch=nDos[t*EOSENTRY_PATCH_SIZE+2]==0 ? false : true;
        entry->actualAddr=0;
        entry->call=0;
        if(map) entry->call=map->read8(entry->origAddr);
        reallyPatch=false;
        switch(entry->call) {
          case 76:  // a direct JMP
            DEBUG_OUT<<"(a direct JMP)\n";
            if(doPatch) reallyPatch=true;
            if(map) entry->actualAddr=map->read16l(entry->origAddr+1);
            break;
          case 108: // an indirect JMP
            DEBUG_OUT<<"(an indirect JMP...hmm...)\n";
            if(doPatch) reallyPatch=true;
            // Baaaahhh! Won't work!
            // The pointer isn't set up until the kernel boots!
            //if(map) temp=map->read16l(entry->origAddr+1);
            //if(map) entry->actualAddr=map->read16l(temp);
            entry->actualAddr=entry->origAddr;
            break;
          default:
            // Somthing else...
            DEBUG_OUT<<"(a "<<(unsigned int)entry->call<<", not a JMP)\n";
            if(doPatch) reallyPatch=true;
            entry->actualAddr=entry->origAddr;
            break;
        }
        entry->origByte=0;
        if(map) {
          entry->origByte=map->read8(entry->actualAddr);
          DEBUG_OUT<<"(Saved origByte "<<aDefaultStringHelper.myHex(entry->origByte)<<")\n";
        }
        if(osCalls) osCalls->append((void *)entry);
        entry->present=false;
        if(reallyPatch) {
          entry->present=true;
          if(map) map->reallyWrite8(entry->actualAddr,0);
          DEBUG_OUT<<"Just patched it.\n";
        }
        if(map) {
          EMapSymbol *sym=new EMapSymbol(entry->name,entry->origAddr,entry->actualAddr,0,EMAP_SYMBOL_NORMAL);
          map->getSymbols()->append((void *)sym);
        }
        if(entry->present) {
          if(reallyPatch) DEBUG_OUT<<"*";
          DEBUG_OUT<<entry->name<<","<<aDefaultStringHelper.myHex((unsigned long)entry->origAddr)<<",";
          DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)entry->actualAddr)<<",";
          DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)entry->call);
          DEBUG_OUT<<"\n";
        }
      }
      t++;
    }
    else working=false;
  }
  osCalls->reset();
}


void EOSHelper::reset()
{
  map=(EMemMapAbstract *)NULL;
  if(io) map=io->getMap();
  cpu=(ECPUAbstract *)NULL;
  if(io) cpu=io->getCPU();
  dos=(DVfs *)NULL;
  if(io) dos=io->getVfs();
  nameLen=0;  nameAddr=0;
  outFile=0;  inFile=0;
  wbufPtr=0; rbufPtr=0;
}


EOSEntry *EOSHelper::findOriginal(ULONG loc)
{
  //DEBUG_OUT<<"findOriginal looking for actual "<<loc<<"\n";
  EOSEntry *n=(EOSEntry *)NULL;
  osCalls->reset();
  bool working=true;
  while(working) {
    n=(EOSEntry *)osCalls->info();
    if(n) {
      //DEBUG_OUT<<n->origAddr<<", ";
      if(n->origAddr==loc) return n;
    }
    else working=false;
    if(osCalls->atEnd()) working=false;
    if(working) osCalls->advance();
  }
  return (EOSEntry *)NULL;
}


EOSEntry *EOSHelper::findActual(ULONG loc)
{
  //DEBUG_OUT<<"findActual looking for actual "<<loc<<"\n";
  EOSEntry *n=(EOSEntry *)NULL;
  osCalls->reset();
  bool working=true;
  while(working) {
    n=(EOSEntry *)osCalls->info();
    if(n) {
      //DEBUG_OUT<<n->actualAddr<<", ";
      if(n->actualAddr==loc) return n;
    }
    else working=false;
    if(osCalls->atEnd()) working=false;
    if(working) osCalls->advance();
  }
  return (EOSEntry *)NULL;
}


unsigned int EOSHelper::load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes)
{
  DEBUG_OUT<<"EOSHelper::load not implemented!\n";
  exit(5);
  return 0;
}


unsigned int EOSHelper::save(const char *filename,ULONG loc,unsigned int size)
{
  DEBUG_OUT<<"EOSHelper::save not implemented!\n";
  exit(5);
  return 0;
}


unsigned int EOSHelper::loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes)
{
  DEBUG_OUT<<"EOSHelper::loadDirectory not implemented!\n";
  exit(5);
  return 0;
}

