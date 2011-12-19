
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NAOUTDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NAOUTDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NAOUTDecoder::recognize(unsigned char *buffer,char *fname)
{
  // This one will probably catch Apple A/UX...
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NAOUTDecoder looking for Apple A/UX signature...\n";
#endif // DEBUG_VERBOSE
  // TODO: This gives a false positive for COFF files...fix it!
  if((buffer[0]==0x01)&&(buffer[1]==0x50)&&(buffer[2]==0x00)&&(buffer[3]==0x03))
    return true;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NAOUTDecoder looking for generic a.out signature...\n";
#endif // DEBUG_VERBOSE
  if((buffer[0]==0xcc)&&(buffer[1]==0x00)&&(buffer[2]==0x64)&&(buffer[3]==0x00))
    return true;  // an i386 demand a.out with hdr
  if((buffer[1]==0x01)&&(buffer[2]==0x64)&&(buffer[3]==0x00))
    return true;  // an i386 a.out
  if((buffer[0]==0x00)&&(buffer[1]<0x04)&&(buffer[2]==0x00)&&(buffer[3]==0xcc))
    return true;  // a sparc or m68k demand a.out with hdr
  if((buffer[0]==0x00)&&(buffer[1]<0x04)&&(buffer[2]==0x1))
    return true;  // a sparc or m68k a.out
  // NOTE: add all different flavors of AOUTs here
  return false;
}


NAOUTDecoder::NAOUTDecoder()
{
  init();
  subArch=N_LD_SUBARCH_GENERIC;
  shouldDemandPage=true;
}


char *NAOUTDecoder::whichCPU()
{
  return archString();
}


char *NAOUTDecoder::whichIO()
{
  return subArchString();
}


bool NAOUTDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"AOUT verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  LSB=false;
  subArch=N_LD_SUBARCH_NONE;
  // TODO: This gives a false positive for COFF files...fix it!
  if((binmem[0]==0x01)&&(binmem[1]==0x50)&&(binmem[2]==0x00)&&(binmem[3]==0x03))
  {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"AOUT found Apple A/UX.\n";
#endif // DEBUG_VERBOSE
    arch=N_LD_ARCH_M68K;
    subArch=N_LD_SUBARCH_AUX;
    ret=verifyAUX();
  }
  if((binmem[0]==0x00)&&(binmem[1]<0x04)&&(binmem[2]==0x00)&&(binmem[3]==0xcc))
  {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"AOUT found generic unix m68k/sparc with hdr.\n";
#endif // DEBUG_VERBOSE
    // we know its a demand load exe...
    arch=N_LD_ARCH_M68K;
    if(binmem[1]==3) arch=N_LD_ARCH_SPARC;
    subArch=N_LD_SUBARCH_GENERIC;
    ret=verifyAOUT();
  }
  if((binmem[0]==0x00)&&(binmem[1]<0x04)&&(binmem[2]==0x1))
  {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"AOUT found generic unix m68k/sparc.\n";
#endif // DEBUG_VERBOSE
    arch=N_LD_ARCH_M68K;
    if(binmem[1]==3) arch=N_LD_ARCH_SPARC;
    subArch=N_LD_SUBARCH_GENERIC;
    ret=verifyAOUT();
  }
  if((binmem[1]==0x01)&&(binmem[2]==0x64)&&(binmem[3]==0x00))
  {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"AOUT found generic unix i386.\n";
#endif // DEBUG_VERBOSE
    arch=N_LD_ARCH_I386;
    LSB=true;
    subArch=N_LD_SUBARCH_GENERIC;
    ret=verifyAOUT();
  }
  if((binmem[0]==0xcc)&&(binmem[1]==0x00)&&(binmem[2]==0x64)&&(binmem[3]==0x00))
  {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"AOUT found generic unix i386 with hdr.\n";
#endif // DEBUG_VERBOSE
    // we know its a demand load exe...
    arch=N_LD_ARCH_I386;
    LSB=true;
    subArch=N_LD_SUBARCH_GENERIC;
    ret=verifyAOUT();
  }
  // NOTE: add all different flavors of AOUTs here
  if(!ret) {
    DEBUG_OUT<<"File is not in AOUT format!\n";
    return false;
  }
  return ret;
}


bool NAOUTDecoder::verifyAUX()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NAOUTDecoder::verifyAUX not implemented!\n";
#endif // DEBUG_VERBOSE
  return false;
}


bool NAOUTDecoder::verifyAOUT()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NAOUTDecoder::verifyAOUT not implemented!\n";
#endif // DEBUG_VERBOSE
  return false;
}
