
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NZMachineDecoder.h>
#include <elib/EMemMapAbstract.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NZMachineDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NZMachineDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NZMachineDecoder looking for signature...\n";
#endif // DEBUG_VERBOSE
  if((buffer[0]>2)&&(buffer[0]<9)) {
    // Looks like a version number...keep checking
    unsigned int highBase=(buffer[4]*256)+buffer[5];
    unsigned int initialPC=(buffer[6]*256)+buffer[7];
    unsigned int dictLoc=(buffer[8]*256)+buffer[9];
    unsigned int objLoc=(buffer[0xa]*256)+buffer[0xb];
    unsigned int globalLoc=(buffer[0xc]*256)+buffer[0xd];
    unsigned int staticBase=(buffer[0xe]*256)+buffer[0xf];
    unsigned int abvLoc=(buffer[0x18]*256)+buffer[0x19];
    // These are assumptions and a valid z-machine could break them
    if((abvLoc<objLoc)&&(objLoc<globalLoc)&&(globalLoc<staticBase)&&(staticBase<dictLoc)&&(dictLoc<highBase)&&(highBase<initialPC))
    return true;
  }
  return false;
}


NZMachineDecoder::NZMachineDecoder()
{
  init();
}


char *NZMachineDecoder::whichCPU()
{
  return "zmachine";
}


char *NZMachineDecoder::whichIO()
{
  return "zmachine";
}


bool NZMachineDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ZMachine verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  highBase=map->read16b(4+getImageOffset());
  initialPC=map->read16b(6+getImageOffset());
  dictLoc=map->read16b(8+getImageOffset());
  objLoc=map->read16b(0x0a+getImageOffset());
  globalLoc=map->read16b(0x0c+getImageOffset());
  staticBase=map->read16b(0x0e + getImageOffset()); // extra spaces due to compiler bug...it thinks its e+whatever notation
  abvLoc=map->read16b(0x18+getImageOffset());
  DEBUG_OUT<<"highBase="<<highBase<<"\n";
  DEBUG_OUT<<"initialPC="<<initialPC<<"\n";
  DEBUG_OUT<<"dictLoc="<<dictLoc<<"\n";
  DEBUG_OUT<<"objLoc="<<objLoc<<"\n";
  DEBUG_OUT<<"globalLoc="<<globalLoc<<"\n";
  DEBUG_OUT<<"staticBase="<<staticBase<<"\n";
  DEBUG_OUT<<"abvLoc="<<abvLoc<<"\n";
  if((abvLoc<objLoc)&&(objLoc<globalLoc)&&(globalLoc<staticBase)&&(staticBase<dictLoc)&&(dictLoc<highBase)&&(highBase<initialPC)) ret=true;
  if(!ret) {
    DEBUG_OUT<<"File is not in ZMachine format!\n";
    return false;
  }
  return ret;
}


bool NZMachineDecoder::decodeNodes()
{
  ld->setEntry(initialPC);
  NBinaryDecoder::decodeNodes();
  return true;
}

