
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NCOFFDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NCOFFDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NCOFFDecoder::recognize(unsigned char *buffer,char *fname)
{
  return false;
}


NCOFFDecoder::NCOFFDecoder()
{
  init();
  subArch=N_LD_SUBARCH_GENERIC;
  shouldDemandPage=true;
}


char *NCOFFDecoder::whichCPU()
{
  return archString();
}


char *NCOFFDecoder::whichIO()
{
  return subArchString();
}


bool NCOFFDecoder::verifyOkay()
{
  DEBUG_OUT<<"COFF verify...\n";
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  LSB=false;
  subArch=N_LD_SUBARCH_NONE;
  // TODO:
  if(!ret) {
    DEBUG_OUT<<"File is not in COFF format!\n";
    return false;
  }
  return ret;
}
