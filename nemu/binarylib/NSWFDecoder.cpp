
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NSWFDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NZMachineDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NSWFDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NSWFDecoder looking for signature...\n";
#endif // DEBUG_VERBOSE
  if((buffer[0]=='F')&&(buffer[1]=='W')&&(buffer[2]=='S')&&(buffer[3]<10))
  {
    return true;
  }
  // Compressed, but we don't know the method, so fail it...
  //if((buffer[0]=='C')&&(buffer[1]=='W')&&(buffer[2]=='S')&&(buffer[3]<10))
  //{
  //  return true;
  //}
  return false;
}


NSWFDecoder::NSWFDecoder()
{
  init();
}


char *NSWFDecoder::whichCPU()
{
  return "swf";
}


char *NSWFDecoder::whichIO()
{
  return "swf";
}


bool NSWFDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NSWFDecoder verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  if((binmem[0]=='F')&&(binmem[1]=='W')&&(binmem[2]=='S')&&(binmem[3]<10))
  {
    ret=true;
  }
  //if((binmem[0]=='C')&&(binmem[1]=='W')&&(binmem[2]=='S')&&(binmem[3]<10))
  //{
  //  ret=true;
  //}
  if(!ret) {
    DEBUG_OUT<<"File is not in NSWFDecoder format!\n";
    return false;
  }
  return ret;
}


bool NSWFDecoder::decodeNodes()
{
  offset=3;
  unsigned int ver=map->read8(offset+getImageOffset());  offset++;
  unsigned int len=map->read32l(offset+getImageOffset());  offset+=4;
  unsigned int bits=map->read8(offset+getImageOffset());  offset++;
  bits=5+4*((bits&0xf8)>>3);
  unsigned int bytes=(bits+7)/8;
  unsigned int w=map->read16b(offset+getImageOffset());  offset+=2;
  unsigned int w1=map->read16b(offset+getImageOffset());  offset+=2;
  unsigned int h=map->read16b(offset+getImageOffset());  offset+=2;
  unsigned int h1=map->read16b(offset+getImageOffset());  offset+=2;
  unsigned int rate1=map->read8(offset+getImageOffset());  offset+=1;
  unsigned int rate2=map->read8(offset+getImageOffset());  offset+=1;
  unsigned int count=map->read16l(offset+getImageOffset());  offset+=2;
  DEBUG_OUT<<"ver= "<<ver<<"\n";
  DEBUG_OUT<<"len= "<<len<<"\n";
  //DEBUG_OUT<<"bits= "<<bits<<"\n";
  //DEBUG_OUT<<"bytes= "<<bytes<<"\n";
  //DEBUG_OUT<<"w= "<<w<<"\n";
  //DEBUG_OUT<<"w1= "<<w1<<"\n";
  //DEBUG_OUT<<"h= "<<h<<"\n";
  //DEBUG_OUT<<"h1= "<<h1<<"\n";
  DEBUG_OUT<<"rate= "<<rate1<<"."<<rate2<<"\n";
  DEBUG_OUT<<"count= "<<count<<"\n";
  //
  NBinaryDecoder::decodeNodes();
  return true;
}

