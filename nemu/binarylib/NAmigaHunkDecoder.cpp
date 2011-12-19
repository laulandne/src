
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <binarylib/NAmigaHunkDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <elib/EMemMapAbstract.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NAmigaHunkDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NAmigaHunkDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NAmigaHunkDecoder looking for signature...\n";
#endif // DEBUG_VERBOSE
  if((buffer[0]==0x00)&&(buffer[1]==0x00)&&(buffer[2]==0x03)&&(buffer[3]==0xf3))
    return true;
  return false;
}


NAmigaHunkDecoder::NAmigaHunkDecoder()
{
  init();
  arch=N_LD_ARCH_M68K;  // Amiga hunks are always 68k
  LSB=false;
}


char *NAmigaHunkDecoder::whichCPU()
{
  return "68020";
}


char *NAmigaHunkDecoder::whichIO()
{
  return "amiga";
}


bool NAmigaHunkDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Amiga verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  if((binmem[0]==0x00)&&(binmem[1]==0x00)&&(binmem[2]==0x03)&&(binmem[3]==0xf3))
  {
    LSB=false;
    shouldDemandPage=true;
    dynamic=false;
    needsReloc=true;
  }
  else {
    DEBUG_OUT<<"File is not in Amiga format!\n";
    return false;
  }
  return true;
}


bool NAmigaHunkDecoder::decodeNodes()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Amiga decodeNodes...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  offset=0;
  numHunks=0;
  nextHunk=0;
  readHeader();
  readNodes();
  return true;
}


bool NAmigaHunkDecoder::readHeader()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"amiga readHeader\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Amiga";
  DEBUG_OUT<<" binary at "<<(ULONG)getImageOffset()<<"\n";
#endif // DEBUG_VERBOSE
  ld->getModule()->setNeedsReloc(true);
  LSB=false;
  offset=4;
  unsigned int nameLen=map->read32(offset+getImageOffset(),LSB);
  offset+=4;
  if(nameLen) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"nameLen="<<nameLen<<"\n";
#endif // DEBUG_VERBOSE
    offset+=4*nameLen;
  }
  numHunks=map->read32(offset+getImageOffset(),LSB);
  offset+=4;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"numHunks="<<numHunks<<" ";
#endif // DEBUG_VERBOSE
  unsigned int F=map->read32(offset+getImageOffset(),LSB);
  offset+=4;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"F="<<F<<" ";
#endif // DEBUG_VERBOSE
  unsigned int L=map->read32(offset+getImageOffset(),LSB);
  offset+=4;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"L="<<L<<"\n";
#endif // DEBUG_VERBOSE
  offset+=(L-F+1)*4;
  return true;
}


bool NAmigaHunkDecoder::readNodes()
{
  unsigned int hunkType=0;
  unsigned int hunkSize=0;
  bool reading=true;
  unsigned int hunkCount=0;
  ULONG loc=0;
  NBinaryNode *n=(NBinaryNode *)NULL;
  nextHunk=0;
  while(reading) {
    hunkType=map->read32(offset+getImageOffset(),LSB);
    offset+=4;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<hunkType<<" ";
#endif // DEBUG_VERBOSE
    switch(hunkType) {
      case 0x3e7:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_unit\n";
#endif // DEBUG_VERBOSE
        hunkSize=map->read32(offset+getImageOffset(),LSB); offset+=4;
        hunkSize&=0x3fffffff;  // strip any "ATOM" bits if any...
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"nameLen="<<hunkSize<<"\n";
#endif // DEBUG_VERBOSE
        offset+=hunkSize*4;
        break;
      case 0x3e8:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_name\n";
#endif // DEBUG_VERBOSE
        hunkSize=map->read32(offset+getImageOffset(),LSB); offset+=4;
        hunkSize&=0x3fffffff;  // strip any "ATOM" bits if any...
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"nameLen="<<hunkSize<<"\n";
#endif // DEBUG_VERBOSE
        offset+=hunkSize*4;
        break;
      case 0x3e9:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_code\n";
#endif // DEBUG_VERBOSE
        hunkSize=map->read32(offset+getImageOffset(),LSB); offset+=4;
        hunkSize&=0x3fffffff;  // strip any "ATOM" bits if any...
        loc=offset;
        n=new NBinaryNode("code",hunkCount,N_BIN_PROGBITS,loc,hunkSize*4,offset,E_MEM_ALL);
        if(n) ld->getModule()->getNodes()->append(n);
        else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
        offset+=hunkSize*4;
        hunkCount++; nextHunk++;
        break;
      case 0x3ea:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_data\n";
#endif // DEBUG_VERBOSE
        hunkSize=map->read32(offset+getImageOffset(),LSB); offset+=4;
        hunkSize&=0x3fffffff;  // strip any "ATOM" bits if any...
        loc=offset;
        n=new NBinaryNode("data",hunkCount,N_BIN_PROGBITS,loc,hunkSize*4,offset,E_MEM_READ|E_MEM_WRITE);
        if(n) ld->getModule()->getNodes()->append(n);
        else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
        offset+=hunkSize*4;
        hunkCount++; nextHunk++;
        break;
      case 0x3eb:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_bss\n";
#endif // DEBUG_VERBOSE
        hunkSize=map->read32(offset+getImageOffset(),LSB); offset+=4;
        hunkSize&=0x3fffffff;  // strip any "ATOM" bits if any...
        loc=offset;
        n=new NBinaryNode("bss",hunkCount,N_BIN_NOBITS,loc,hunkSize*4,0,E_MEM_READ|E_MEM_WRITE);
        if(n) ld->getModule()->getNodes()->append(n);
        else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
        hunkCount++; nextHunk++;
        break;
      case 0x3ec:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_reloc32\n";
#endif // DEBUG_VERBOSE
        hunkSize=1;  // not really, but it can't be zero
        readReloc(32);
        break;
      case 0x3ed:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_reloc16\n";
#endif // DEBUG_VERBOSE
        hunkSize=1;  // not really, but it can't be zero
        readReloc(16);
        break;
      case 0x3ee:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_reloc8\n";
#endif // DEBUG_VERBOSE
        hunkSize=1;  // not really, but it can't be zero
        readReloc(8);
        break;
      case 0x3ef:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_ext\n";
#endif // DEBUG_VERBOSE
        hunkSize=1;  // not really, but it can't be zero
        readSymbols(true);
        break;
      case 0x3f0:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_symbol\n";
#endif // DEBUG_VERBOSE
        hunkSize=1;  // not really, but it can't be zero
        readSymbols(false);
        break;
      case 0x3f1:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_debug\n";
#endif // DEBUG_VERBOSE
        hunkSize=map->read32(offset+getImageOffset(),LSB); offset+=4;
        hunkSize&=0x3fffffff;  // strip any "ATOM" bits if any...
        loc=offset;
        n=new NBinaryNode("debug",0,N_BIN_NOBITS,loc,hunkSize*4,0,E_MEM_READ|E_MEM_WRITE);
        if(n) ld->getModule()->getNodes()->append(n);
        else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
        offset+=hunkSize*4;
        break;
      case 0x3f2:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_end\n";
#endif // DEBUG_VERBOSE
        hunkSize=1;  // not really, but it can't be zero
        numHunks--;
        break;
      case 0x3f6:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"hunk_break\n";
#endif // DEBUG_VERBOSE
        hunkSize=1;  // not really, but it can't be zero
        break;
      default:
        DEBUG_OUT<<"unknown hunk type...bailing!\n";
        reading=false;
        break;
    }
    if(!numHunks) reading=false;
  }
  return true;
}


bool NAmigaHunkDecoder::readSymbols(bool external)
{
  AList *syms=map->getFirstModule()->getSymbols();
  EMapSymbol *s=(EMapSymbol *)NULL;
  bool readingEntries=true;
  unsigned int n,num,type,ttype,t,val;
  char tname[100];
  while(readingEntries) {
    n=map->read32(offset+getImageOffset(),LSB); offset+=4;
    if(n) {
      type=(n>>24)&0xff; n&=0xffffff;
      //DEBUG_OUT<<"symbol type "<<type<<" : ";
      for(t=0;t<n*4;t++) {
        tname[t]=map->read8(offset+getImageOffset()+t);
        //DEBUG_OUT<<tname[t];
      }
      tname[n*4]=0;
      //DEBUG_OUT<<"\n";
      offset+=n*4;  // skip name
      switch(type) {
        case 0: // symbol
          val=map->read32(offset+getImageOffset(),LSB); offset+=4;
          if(external) ttype=EMAP_SYMBOL_EXTERN;
          else ttype=EMAP_SYMBOL_NORMAL;
          s=new EMapSymbol(tname,0,val,0,ttype,nextHunk-1);
          if(syms) syms->insert(s);
          break;
        case 1: // def
        case 2: // abs
        case 3: // res
          val=map->read32(offset+getImageOffset(),LSB); offset+=4;
          if(external) ttype=EMAP_SYMBOL_EXTERN;
          else ttype=EMAP_SYMBOL_NORMAL;
          s=new EMapSymbol(tname,0,val,0,ttype,nextHunk-1);
          if(syms) syms->insert(s);
          break;
        case 129: // 32-bit ref
        case 131: // 16-bit ref
        case 132: // 8-bit ref
          num=map->read32(offset+getImageOffset(),LSB); offset+=4;
          val=map->read32(offset+getImageOffset(),LSB); offset+=4;
          if(external) ttype=EMAP_SYMBOL_EXTERN;
          else ttype=EMAP_SYMBOL_RELOC;
          s=new EMapSymbol(tname,num,val,0,ttype,nextHunk-1);
          if(syms) syms->insert(s);
          offset+=num*4; // skip refs
          break;
        case 130: // common ref
          // first size of common block
          num=map->read32(offset+getImageOffset(),LSB); offset+=4;
          // then num of refs in it
          val=map->read32(offset+getImageOffset(),LSB); offset+=4;
          if(external) ttype=EMAP_SYMBOL_EXTERN;
          else ttype=EMAP_SYMBOL_NORMAL;
          s=new EMapSymbol(tname,0,val,num,ttype,nextHunk-1);
          if(syms) syms->insert(s);
          offset+=num*4; // skip refs
          break;
        default:
          DEBUG_OUT<<"unknown symbol type...bailing!\n";
          readingEntries=false;
          break;
      }
    }
    else readingEntries=false;
  }
  return true;
}


bool NAmigaHunkDecoder::readReloc(unsigned int size)
{
  AList *reloc=ld->getReloc();
  if(!reloc) { DEBUG_OUT<<"readReloc couldn't get ld's reloc!\n"; }
  bool readingEntries=true;
  unsigned int n,num,r;
  while(readingEntries) {
    n=map->read32(offset+getImageOffset(),LSB); offset+=4;
    if(n) {
      num=map->read32(offset+getImageOffset(),LSB); offset+=4;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"hunk "<<nextHunk-1<<" has "<<n<<" relocs to hunk "<<num<<"...\n";
#endif // DEBUG_VERBOSE
      for(unsigned int t=0;t<n;t++) {
        r=map->read32(offset+getImageOffset(),LSB); offset+=4;
        //DEBUG_OUT<<t<<" : "<<r<<"\n";
        if(reloc) {
          EMapSymbol *s=new EMapSymbol("reloc",0,r,size,EMAP_SYMBOL_RELOC,nextHunk-1,num);
          reloc->append(s);
        }
      }
    }
    else readingEntries=false;
  }
  return true;
}


