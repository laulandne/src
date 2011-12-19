
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <binarylib/NBFLTDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NBFLTDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NBFLTDecoder::recognize(unsigned char *buffer,char *fname)
{
  if((buffer[0]=='b')&&(buffer[1]=='F')&&(buffer[2]=='L')&&(buffer[3]=='T')) return true;
  return false;
}


NBFLTDecoder::NBFLTDecoder()
{
  init();
  shouldDemandPage=false;
}


char *NBFLTDecoder::whichCPU()
{
  return archString();
}


char *NBFLTDecoder::whichIO()
{
  return subArchString();
}


bool NBFLTDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NBFLTDecoder verifyOkay...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  ret=true;
  return ret;
}


bool NBFLTDecoder::decodeNodes()
{
  DEBUG_OUT<<"NBFLTDecoder decodeNodes...\n";
  readHeader();
  NBinaryNode *n=new NBinaryNode("node",0,N_BIN_PROGBITS,0,binsize,0, E_MEM_ALL);
  if(n) ld->getModule()->getNodes()->append(n);
  else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
  return true;
}


void NBFLTDecoder::readHeader()
{
  DEBUG_OUT<<"NBFLTDecoder readHeader...\n";
  // These are assumptions for now...
  arch=N_LD_ARCH_ARM;
  subArch=N_LD_SUBARCH_LINUX;
  LSB=true;
  //
  offset=4;
  unsigned long rev=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long entry=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long data_start=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long data_end=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long bss_end=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long stack_size=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long reloc_size=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long reloc_count=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long flags=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long build_date=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long filler0=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long filler1=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long filler2=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long filler3=map->read32b(offset+getImageOffset()); offset+=4;
  unsigned long filler4=map->read32b(offset+getImageOffset()); offset+=4;
  DEBUG_OUT<<"rev="<<rev<<"\n";
  // rev should be 4
  DEBUG_OUT<<"entry="<<aDefaultStringHelper.myHex(entry)<<"\n";
  DEBUG_OUT<<"data_start="<<aDefaultStringHelper.myHex(data_start)<<"\n";
  DEBUG_OUT<<"data_end="<<aDefaultStringHelper.myHex(data_end)<<"\n";
  // assumed data_end thru bss_end is bss seg
  DEBUG_OUT<<"bss_end="<<aDefaultStringHelper.myHex(bss_end)<<"\n";
  DEBUG_OUT<<"stack_size="<<aDefaultStringHelper.myHex(stack_size)<<"\n";
  DEBUG_OUT<<"reloc_size="<<aDefaultStringHelper.myHex(reloc_size)<<"\n";
  DEBUG_OUT<<"reloc_count="<<aDefaultStringHelper.myHex(reloc_count)<<"\n";
  DEBUG_OUT<<"flags="<<flags<<" ";
  // flags 1=ram 2=gotpic 4=gzip 8=gzdata 10=ktrace
  if(flags&0x1) DEBUG_OUT<<"ram ";
  if(flags&0x2) DEBUG_OUT<<"got_pic ";
  if(flags&0x4) DEBUG_OUT<<"gzip ";
  if(flags&0x8) DEBUG_OUT<<"gzdata ";
  if(flags&0x10) DEBUG_OUT<<"ktrace ";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"build_date="<<aDefaultStringHelper.myHex(build_date)<<"\n";
}

