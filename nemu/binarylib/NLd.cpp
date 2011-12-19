
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <binarylib/NBinaryDecoder.h>

#include <binarylib/NAmigaHunkDecoder.h>
#include <binarylib/NAOUTDecoder.h>
#include <binarylib/NBFDDecoder.h>
#include <binarylib/NBFLTDecoder.h>
#include <binarylib/NBundleDecoder.h>
#include <binarylib/NCOFFDecoder.h>
#include <binarylib/NElfDecoder.h>
#include <binarylib/NExeDecoder.h>
#include <binarylib/NJavaClassDecoder.h>
#include <binarylib/NMacAppDecoder.h>
#include <binarylib/NMachDecoder.h>
#include <binarylib/NPalmAppDecoder.h>
#include <binarylib/NSWFDecoder.h>
#include <binarylib/NZMachineDecoder.h>

#include <disklib/DVfs.h>

#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#define LOAD_INTO_MAP 1


////////////////////////////////////////////////////////////////////////////////
//  NLd Class
////////////////////////////////////////////////////////////////////////////////

NLd::NLd(EModule *mod)
{
  init();
  module=mod;
  if(!module) err.set();
  else {
    if(module->err.error()) err.set();
  }
  if(err.error()) {
    DEBUG_OUT<<"NLd got passed a bad EModule!\n";
  }
}


NLd::~NLd()
{
  //init();  // just to clean everything out...
  module=(EModule *)NULL;
}


void NLd::init()
{
  map=(EMemMapAbstract *)NULL;
  vfs=(DVfs *)NULL;
}


/* STATIC */
NBinaryDecoder* NLd::pickDecoder(char *binFile,DVfs *tempdos)
{
  if(!binFile) return (NBinaryDecoder *)NULL;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NLd::pickDecoder...\n";
#endif // DEBUG_VERBOSE
  if(!tempdos) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Creating tempdos...\n";
#endif // DEBUG_VERBOSE
    tempdos=new DVfs();
    if(!tempdos) { std::cerr<<"NLD didn't get tempdos!\n";  exit(5); }
  }
  char *binFile2=binFile;
  FILE *fptr=(FILE *)NULL;
  // open the file
  fptr=(FILE *)tempdos->Fopen(binFile,"rb");
  if(!fptr) fptr=(FILE *)tempdos->Fopen(aDefaultStringHelper.removeQuotes(binFile),"rb");
  if(!fptr) fptr=(FILE *)tempdos->Fopen(aDefaultStringHelper.flipSlashes(binFile),"rb");
  if(!fptr) {
    DEBUG_OUT<<"Unable to open file: '"<<(char *)binFile<<"'\n";
    return (NBinaryDecoder *)NULL;
  }
  unsigned char buffer[128];
  tempdos->Fseek(fptr,0,SEEK_SET);
  tempdos->Fread(buffer,128,1,fptr);      // grab a few bytes
  if(fptr) tempdos->Fclose(fptr);      // close the file
  //
  // Check the various binary executable formats...
  //
  NBinaryDecoder *dec=(NBinaryDecoder *)NULL;
  // I check dec because some files can be legal as more than one format...
  // ...but we just want the first one we recognize.
#ifdef USE_LIBBFD
  if(!dec) {
    if(NBFDDecoder::recognize(buffer,binFile2))
      dec=new NBFDDecoder();
  }
#endif // USE_LIBBFD
  if(!dec) {
    if(NElfDecoder::recognize(buffer,binFile2))  dec=new NElfDecoder();
  }
  if(!dec) {
    if(NExeDecoder::recognize(buffer,binFile2))  dec=new NExeDecoder();
  }
  if(!dec) {
    if(NCOFFDecoder::recognize(buffer,binFile2))  dec=new NCOFFDecoder();
  }
  if(!dec) {
    if(NAOUTDecoder::recognize(buffer,binFile2))  dec=new NAOUTDecoder();
  }
  if(!dec) {
    if(NBFLTDecoder::recognize(buffer,binFile2))  dec=new NBFLTDecoder();
  }
  if(!dec) {
    if(NAmigaHunkDecoder::recognize(buffer,binFile2))
      dec=new NAmigaHunkDecoder();
  }
  if(!dec) {
    if(NMachDecoder::recognize(buffer,binFile2))  dec=new NMachDecoder();
  }
  if(!dec) {
    if(NJavaClassDecoder::recognize(buffer,binFile2))
      dec=new NJavaClassDecoder();
  }
  if(!dec) {
    if(NBundleDecoder::recognize(buffer,binFile2))
      dec=new NBundleDecoder();
  }
  if(!dec) {
    if(NZMachineDecoder::recognize(buffer,binFile2))
      dec=new NZMachineDecoder();
  }
  if(!dec) {
    if(NSWFDecoder::recognize(buffer,binFile2))
      dec=new NSWFDecoder();
  }
  /*
  if(!dec) {
    if(NXO65Decoder::recognize(buffer,binFile2))
      dec=new NXO65Decoder();
  }
  */
  if(!dec) {
    // Try this one last as it may be less picky...
    if(NMacAppDecoder::recognize(buffer,binFile2))
      dec=new NMacAppDecoder();
  }
  //
  // finally give up!
  //
  return dec;
}


void NLd::nm(char *nm)
{
  if(module->getUnknown()) {
    DEBUG_OUT<<"LD, no file loaded!\n";
    return;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Entry at "<<module->getEntry()<<"\n";
#endif // DEBUG_VERBOSE
  module->getNodes()->reset();
  if(module->getNodes()->atEnd()) return;
  NBinaryNode *n=(NBinaryNode *)NULL;
  bool working=true;
  while(working) {
    if(module->getNodes()->atEnd()) working=false;
    else {
      n=(NBinaryNode *)module->getNodes()->info();
      //n->print();
      module->getNodes()->advance();
    }
  }
}


// TODO: I'm still a little iffy on this one...
void NLd::reset()
{
  NBinaryNode *n=(NBinaryNode *)NULL;
  module->getNodes()->reset();
  if(module->getNodes()->atEnd()) return;  // It was already empty
  bool working=true;
  while(working) {
    n=(NBinaryNode *)module->getNodes()->info();
    //DEBUG_OUT<<"Just removed node at "<<(unsigned int)n<<"\n";
    if(n) delete n; else working=false;
    module->getNodes()->remove();
    module->getNodes()->advance();
  }
  module->setUnknown(true);
}


bool NLd::ld(char *nm,ULONG addr)
{
  // TODO: need to implement this later...relocating code...
  return ld(nm);
}


bool NLd::ld(char *nm)
{
  if(!nm) return false;
  bool ret=false;
  /*
  if(!dos) {
    *streamOut<<"LD doesn't have a dos!\n";
    return;
  }
  */
  module->setUnknown(true);
  NBinaryDecoder *dec=pickDecoder(nm);
  if(!dec) {
    DEBUG_OUT<<"Couldn't recognize file type!\n";
    return false;
  }
  dec->setLD(this);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<dec->which();
#endif // DEBUG_VERBOSE
  dec->openBinary(nm);
  if(dec->err.error()) err.set();
  if(dec->verifyOkay()) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"File passed format's verify.\n";
#endif // DEBUG_VERBOSE
    dec->decodeNodes();
    if(dec->err.error()) err.set();
    module->setCPUType(dec->whichCPU());
    module->setIOType(dec->whichIO());
    module->setLSB(dec->getLSB());
    module->setDynamic(dec->getDynamic());
    module->setBitness(dec->getBitness());
    module->setShouldDemandPage(dec->getShouldDemandPage());
    module->setNeedsReloc(dec->getNeedsReloc());
    module->setUnknown(false);
    ret=loadNodes(dec->getImageOffset());
  }
  else {
    DEBUG_OUT<<"NLD:ld File format error!\n";
    err.set();
    ret=false;
  }
  return ret;
}


// NOTE: if you set needsReloc, all your nodes will be loaded willy-nilly
// around the map, be sure this is okay with you before you ask for it!
bool NLd::loadNodes(ULONG imageOffset)
{
  if(err.error()) return false;
  if(!map) {
    DEBUG_OUT<<"loadNodes, no map!!!\n";
    return false;
  }
  //if(unknown) {
    //*streamOut<<"loadNodes, unknown!\n";
    //return false;
  //}
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"loadNodes("<<imageOffset<<")...\n";
#endif // DEBUG_VERBOSE
  if(module->getNeedsReloc()) module->setHighestAddressUsed((64*1024)*1);  // start at the 64k mark
  bool relocNode=false;
  bool anyReloc=false;
  unsigned int t=0;
  NBinaryNode *n=(NBinaryNode *)NULL;
  module->getNodes()->reset();
  bool working=true;
  // Add a symbol for the entry address...
  EMapSymbol *sym=new EMapSymbol("Start",0,module->getEntry(),0,EMAP_SYMBOL_ENTRY);
  AList *symbols=map->getFirstModule()->getSymbols();
  if(symbols) symbols->append((void *)sym);
  // Take care of all the binary nodes...
  while(working) {
    //aDefaultStringHelper.yieldToOtherProcesses();
    if(module->getNodes()->atEnd()) working=false;
    else {
      n=(NBinaryNode *)module->getNodes()->info();
      DEBUG_OUT<<"Node "<<n->num<<" "<<n->getName()<<" @ "<<n->addr<<" for "<<n->size<<" bytes.\n";
      relocNode=false;
      if(module->getNeedsReloc()) {
        relocNode=true;
      }
      if(!(n->addr)) {
        // Node asked for address 0, relocate if already used...
        if(module->getAlreadyUsedZero()) { relocNode=true; }
        module->setAlreadyUsedZero(true);
      }
      if(relocNode) {
        DEBUG_OUT<<"(Going to reloc this node)\n";
        n->setAddr(module->getHighestAddressUsed());
        anyReloc=true;
      }
      if((n->addr+n->size)>module->getHighestAddressUsed())
        module->setHighestAddressUsed(n->addr+n->size);
      // print some info...
      //n->print(w);
      // copy it if needed
      if(n->size) {
        // Let map know what's coming...
        //map->preallocVirtMem(n->addr,n->size);
        if(n->getData()) {
          // Copy the data that was preloaded to final addr
          BYTE *tdata=n->getData();
          for(ULONG q=0;q<n->size;q++)
            map->write8(n->addr+q,tdata[q]);
        }
        else {
          // Copy from N_LD_IMAGE_OFFSET to final addr
          for(ULONG q=0;q<n->size;q++)
            map->write8(n->addr+q,
              map->read8(imageOffset+n->getOffsetToData()+q));
        }
      }
      else DEBUG_OUT<<"Node has zero size!\n";
      /*
      // Now add a symbol for this node...
      sym=new EMapSymbol(n->name,n->num,n->addr,
        n->size,EMAP_SYMBOL_BINNODE,t);
      AList *symbols=map->getFirstModule()->getSymbols();
      if(symbols) symbols->append((void *)sym);
      // next!
      */
      module->getNodes()->advance();
      t++;
    }
  }
  // NOTE: have to fix symbols and relocs AFTER we've loaded all the nodes...
  if(anyReloc) {
    DEBUG_OUT<<"Done loading nodes...going to do reloc...\n";
    module->getNodes()->reset();
    working=true;
    while(working) {
      if(module->getNodes()->atEnd()) working=false;
      else {
        n=(NBinaryNode *)module->getNodes()->info();
        doReloc(n);
        // next!
        module->getNodes()->advance();
      }
    }
  }
  if(!module->getEntry()) {
    DEBUG_OUT<<"Setting entry to beginning of first PROGBITS node.\n";
    // Probably didn't want entry at 0...find first PROGBITS and set it there
    n=findNodeByType(N_BIN_PROGBITS);
    if(n) module->setEntry(n->addr);
  }
  return true;
}


bool NLd::doReloc(NBinaryNode *n)
{
  if(err.error()) return false;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"doReloc requested for node "<<n->num<<" ("<<n->getName()<<").\n";
#endif // DEBUG_VERBOSE
  // first fix actual reloc entries...
  module->getReloc()->reset();
  if(module->getReloc()->atEnd()) {
    DEBUG_OUT<<"No reloc entries for module!\n";
    return true;
  }
  bool reading=true;
  while(reading) {
    EMapSymbol *s=(EMapSymbol *)module->getReloc()->info();
    if(module->getReloc()->atEnd()) reading=false;
    else {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Looking at reloc assoc with "<<s->val<<"\n";
#endif // DEBUG_VERBOSE
      if(s->val==n->num) {
        // this reloc is in this node
        ULONG addr=s->addr+n->addr;
        ULONG dest=findNode(s->val2)->addr;
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"reloc: ";
        DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)s->addr)<<" ";
        DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)n->addr)<<" ";
        DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned long)dest)<<" ";
        DEBUG_OUT<<"\n";
#endif // DEBUG_VERBOSE
        s->addr=addr;
        // TODO: actually use reloc's size field!
	if(module->getBitness()==32) {
          ULONG val=map->read32(addr,module->getLSB());
          map->write32(addr,val+dest,module->getLSB());
	}
	else {
          ULONG val=map->read64(addr,module->getLSB());
          map->write64(addr,val+dest,module->getLSB());
	}
      }
      module->getReloc()->advance();
    }
  }
  // then fix any symbols that refer to this node...
  AList *symbols=map->getFirstModule()->getSymbols();
  if(symbols) {
    symbols->reset();
    reading=true;
  }
  else reading=false;
  while(reading) {
    EMapSymbol *s=(EMapSymbol *)map->getFirstModule()->getSymbols()->info();
    if(map->getFirstModule()->getSymbols()->atEnd()) reading=false;
    else {
      DEBUG_OUT<<"Looking at sym assoc with "<<s->val<<"\n";
      if(s->val==n->num) {
        // this symbol is in this node
        // NOTE: don't do binnodes, since they already have the right addr...
        if(s->type!=EMAP_SYMBOL_BINNODE) {
          ULONG addr=s->addr+n->addr;
#ifdef DEBUG_VERBOSE
          DEBUG_OUT<<"reloc sym: "<<aDefaultStringHelper.myHex((unsigned long)addr)<<" "<<s->getName()<<"\n";
#endif // DEBUG_VERBOSE
          s->addr=addr;
        }
      }
      map->getFirstModule()->getSymbols()->advance();
    }
  }
  return true;
}


NBinaryNode *NLd::findNodeByType(unsigned int type)
{
  //DEBUG_OUT<<"Looking for node "<<num<<"\n";
  bool worked=module->getNodes()->placeBookmark();
  if(!worked) {
    DEBUG_OUT<<"findNode couldn't since placeBookmark failed!\n";
    return (NBinaryNode *)NULL;
  }
  module->getNodes()->reset();
  NBinaryNode *ret=(NBinaryNode *)NULL;
  bool reading=true;
  while(reading) {
    if(module->getNodes()->atEnd()) reading=false;
    else {
      NBinaryNode *n=(NBinaryNode *)module->getNodes()->info();
      //DEBUG_OUT<<"Looking at node "<<n->num<<"\n";
      if(n->type==type) {
        //DEBUG_OUT<<"Found it!\n";
        ret=n;
        reading=false;
      }
      else module->getNodes()->advance();
    }
  }
  module->getNodes()->gotoBookmark();
  return ret;
}


NBinaryNode *NLd::findNode(unsigned int num)
{
  //DEBUG_OUT<<"Looking for node "<<num<<"\n";
  bool worked=module->getNodes()->placeBookmark();
  if(!worked) {
    DEBUG_OUT<<"findNode couldn't since placeBookmark failed!\n";
    return (NBinaryNode *)NULL;
  }
  module->getNodes()->reset();
  NBinaryNode *ret=(NBinaryNode *)NULL;
  bool reading=true;
  while(reading) {
    if(module->getNodes()->atEnd()) reading=false;
    else {
      NBinaryNode *n=(NBinaryNode *)module->getNodes()->info();
      //DEBUG_OUT<<"Looking at node "<<n->num<<"\n";
      if(n->num==num) {
        //DEBUG_OUT<<"Found it!\n";
        ret=n;
        reading=false;
      }
      else module->getNodes()->advance();
    }
  }
  module->getNodes()->gotoBookmark();
  return ret;
}


NBinaryNode *NLd::findNodeForAddress(ULONG addr)
{
  // TODO:
  return (NBinaryNode *)NULL;
}



