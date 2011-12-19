
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NBFDDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>

#include <portable/nickcpp/ANullStream.h>


#ifdef USE_LIBBFD
#include <bfd.h>
#endif // USE_LIBBFD


////////////////////////////////////////////////////////////////////////////////
//  NXO65Decoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
/*
bool NXO65Decoder::recognize(unsigned char *buffer,char *fname)
{
  DEBUG_OUT<<"NXO65Decoder looking for signature...\n";
  if((buffer[3]==0x61)&&(buffer[2]==0x6e)&&(buffer[1]==0x7a)&&(buffer[0]==0x55))
    return true;
  return false;
}


NXO65Decoder::NXO65Decoder()
{
  init();
}


void NXO65Decoder::init()
{
}


char *NXO65Decoder::whichCPU()
{
  return "65816");
}


char *NXO65Decoder::whichIO()
{
  return "apple2gs");
}


bool NXO65Decoder::verifyOkay()
{
  DEBUG_OUT<<"NO65Decoder verify...\n";
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  if((binmem[3]==0x61)&&(binmem[2]==0x6e)&&(binmem[1]==0x7a)&&(binmem[0]==0x55))
    return true;
  return ret;
}


bool NXO65Decoder::readHeader()
{
  offset=4;
  unsigned int version=map->read16l(offset+getImageOffset()); offset+=2;
  unsigned int flags=map->read16l(offset+getImageOffset()); offset+=2;
  optOff=map->read32l(offset+getImageOffset()); offset+=4;
  optSize=map->read32l(offset+getImageOffset()); offset+=4;
  fileOff=map->read32l(offset+getImageOffset()); offset+=4;
  fileSize=map->read32l(offset+getImageOffset()); offset+=4;
  segOff=map->read32l(offset+getImageOffset()); offset+=4;
  segSize=map->read32l(offset+getImageOffset()); offset+=4;
  impOff=map->read32l(offset+getImageOffset()); offset+=4;
  impSize=map->read32l(offset+getImageOffset()); offset+=4;
  expOff=map->read32l(offset+getImageOffset()); offset+=4;
  expSize=map->read32l(offset+getImageOffset()); offset+=4;
  debugOff=map->read32l(offset+getImageOffset()); offset+=4;
  debugSize=map->read32l(offset+getImageOffset()); offset+=4;
  lineOff=map->read32l(offset+getImageOffset()); offset+=4;
  lineSize=map->read32l(offset+getImageOffset()); offset+=4;
  stringOff=map->read32l(offset+getImageOffset()); offset+=4;
  stringSize=map->read32l(offset+getImageOffset()); offset+=4;
  assOff=map->read32l(offset+getImageOffset()); offset+=4;
  assSize=map->read32l(offset+getImageOffset()); offset+=4;
  scopeOff=map->read32l(offset+getImageOffset()); offset+=4;
  scopeSize=map->read32l(offset+getImageOffset()); offset+=4;
  DEBUG_OUT<<"XO65 format version: "<<version<<"\n";
  DEBUG_OUT<<"flags: "<<flags<<"\n";
  DEBUG_OUT<<"Option table: off="<<optOff<<" size="<<optSize<<"\n";
  DEBUG_OUT<<"File table: off="<<fileOff<<" size="<<fileSize<<"\n";
  DEBUG_OUT<<"Debug table: off="<<debugOff<<" size="<<debugSize<<"\n";
  DEBUG_OUT<<"Line table: off="<<lineOff<<" size="<<lineSize<<"\n";
  DEBUG_OUT<<"Assertion table: off="<<assOff<<" size="<<assSize<<"\n";
  DEBUG_OUT<<"Scope table: off="<<scopeOff<<" size="<<scopeSize<<"\n";
  return true;
}


bool NXO65Decoder::decodeNodes()
{
  readHeader();
  handleStrings();
  handleImports();
  handleExports();
  handleSegments();
  return true;
}


void NXO65Decoder::handleStrings()
{
  if(stringSize==1) return;
  if(stringSize==2) return;
  offset=stringOff;
  stringNum=map->read16l(offset+getImageOffset()); offset+=2;
  DEBUG_OUT<<"String table: stringOff="<<stringOff<<" stringSize="<<stringSize<<"\n";
  DEBUG_OUT<<"There are "<<stringNum<<" strings\n";
  AList *strings=map->getFirstModule()->getStrings();
  char s[256];
  unsigned int l;
  for(unsigned int t=1;t<stringNum;t++) {
    unsigned int size=map->read8(offset+getImageOffset()); offset++;
    DEBUG_OUT<<"String "<<t<<" is "<<size<<" bytes: ";
    for(l=0;l<size;l++) s[l]=map->read8(offset+getImageOffset()+l);
    s[size]=0;
    DEBUG_OUT<<s<<"\n";
    EMapSymbol *sym=new EMapSymbol(s,t,offset,size,0,0,0);
    if(strings) strings->append((void *)sym);
    offset+=l;
  }
}


void NXO65Decoder::handleImports()
{
  if(impSize==1) return;
  offset=impOff;
  impNum=map->read8(offset+getImageOffset()); offset+=1;
  DEBUG_OUT<<"Import table: off="<<impOff<<" size="<<impSize<<"\n";
  DEBUG_OUT<<"There are "<<impNum<<" imports\n";
  AList *syms=map->getFirstModule()->getSymbols();
  AList *strings=map->getFirstModule()->getStrings();
  // 5 bytes each
  // type? name? addr?  seg/zero?
  for(unsigned int t=0;t<impNum;t++) {
    unsigned int unknown1=map->read8(offset+getImageOffset()); offset++;
    unsigned int name=map->read8(offset+getImageOffset()); offset++;
    unsigned int addr=map->read16l(offset+getImageOffset()); offset+=2;
    unsigned int unknown2=map->read8(offset+getImageOffset()); offset++;
    char *sname=NULL;
    EMapSymbol *nameString=map->getFirstModule()->lookupStringByNum(name);
    if(nameString) sname=nameString->getName();
    else sname="no_name";
    DEBUG_OUT<<"Import "<<t<<" "<<sname<<" unk1="<<unknown1<<" unk2="<<unknown2<<" addr="<<addr<<"\n";;
    EMapSymbol *sym=new EMapSymbol(sname,t,addr,2,EMAP_SYMBOL_EXTERN,0,0);
    if(syms) syms->append((void *)sym);
  }
}


void NXO65Decoder::handleExports()
{
  if(expSize==1) return;
  offset=expOff;
  expNum=map->read8(offset+getImageOffset()); offset+=1;
  DEBUG_OUT<<"Export table: off="<<expOff<<" size="<<expSize<<"\n";
  DEBUG_OUT<<"There are "<<expNum<<" exports\n";
  DEBUG_OUT<<"(Exports not implemented yet)\n";
}


void NXO65Decoder::handleSegments()
{
  if(segSize==1) return;
  offset=segOff;
  segNum=map->read8(offset+getImageOffset()); offset+=1;
  DEBUG_OUT<<"Segment table: off="<<segOff<<" size="<<segSize<<"\n";
  DEBUG_OUT<<"There are "<<segNum<<" segments\n";
  AList *syms=map->getFirstModule()->getSymbols();
  AList *strings=map->getFirstModule()->getStrings();
  char s[256];
  unsigned int l;
  segNum=2;
  for(unsigned int t=1;t<segNum;t++) {
    ULONG size=map->read32l(offset+getImageOffset()); offset+=4;
    unsigned int name=map->read8(offset+getImageOffset()); offset++;
    ULONG addr=map->read32l(offset+getImageOffset()); offset+=4;
    unsigned int align=map->read8(offset+getImageOffset()); offset++;
    unsigned int asize=map->read8(offset+getImageOffset()); offset++;
    unsigned int fragNum=map->read8(offset+getImageOffset()); offset++;
    char *sname=NULL;
    EMapSymbol *nameString=map->getFirstModule()->lookupStringByNum(name);
    if(nameString) sname=nameString->getName();
    else sname="no_name";
    DEBUG_OUT<<"Segment "<<t<<" "<<sname<<" align="<<align<<" asize="<<asize<<" fragNum="<<fragNum<<" addr="<<addr<<"\n";
    EMapSymbol *sym=new EMapSymbol(sname,t,addr,size,0,0,0);
    if(syms) syms->append((void *)sym);
    unsigned int oldOffset=offset;
    handleFrags(fragNum,offset);
    offset=oldOffset+size;
  }
  //
  ld->reset();
  NBinaryNode *n=new NBinaryNode("node"),0,N_BIN_PROGBITS,0x4000,segSize,0x40000,
    E_MEM_ALL);
  if(n) ld->getModule()->getNodes()->append(n);
  else std::cerr<<"Didn't get NBinaryNode!\n");
}


void NXO65Decoder::handleFrags(unsigned int num,unsigned int offset)
{
  unsigned int size=0;
  for(unsigned int l=0;l<num;l++) {
    unsigned int ftype=map->read8(offset+getImageOffset()); offset++;
    DEBUG_OUT<<"frag #"<<l<<" ";
    switch(ftype) {
      case 0:
        // Literal
        DEBUG_OUT<<"literal";
        break;
      case 8:  case 9:  case 10:  case 11:  case 12:
        // Expression
        size=ftype&3;
        DEBUG_OUT<<"Expression size "<<size;
        break;
      case 16:  case 17: case 18:  case 19: case 20:
        // signed expression
        size=ftype&3;
        DEBUG_OUT<<"Signed expression size "<<size;
        break;
      case 32:
        // fill
        DEBUG_OUT<<"fill";
        break;
      default:
        DEBUG_OUT<<"unknown/illegal";
        break;
    }
    DEBUG_OUT<<"\n";
  }
}
*/

