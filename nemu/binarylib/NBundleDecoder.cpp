
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NBundleDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NBundleDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NBundleDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NBundleDecoder looking for signature...\n";
#endif // DEBUG_VERBOSE
  /*if((buffer[0]==0xca)&&(buffer[1]==0xfe)&&(buffer[2]==0xba)&&(buffer[3]==0xbe))
    return true;
  */
  return false;
}


NBundleDecoder::NBundleDecoder()
{
  init();
}


char *NBundleDecoder::whichCPU()
{
  return "unknown";
}


char *NBundleDecoder::whichIO()
{
  return "unix";
}


bool NBundleDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NBundleDecoder verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  /*
  if((binmem[0]==0xca)&&(binmem[1]==0xfe)&&(binmem[2]==0xba)&&(binmem[3]==0xbe))
  {
    arch=N_LD_ARCH_JAVA;
    ret=true;
  }
  */
  if(!ret) {
    DEBUG_OUT<<"File is not in NBundleDecoder format!\n";
    return false;
  }
  return ret;
}

