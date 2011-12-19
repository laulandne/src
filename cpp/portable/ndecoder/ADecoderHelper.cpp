
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "ADecoderHelper.h"


////////////////////////////////////////////////////////////////////////////////
//  ADecoderHelper Class
////////////////////////////////////////////////////////////////////////////////

ADecoderHelper::ADecoderHelper(const char *fname) : AImageDecoder(fname)
{
  init();
}


void ADecoderHelper::init()
{
  for(unsigned int t=0;t<1024;t++) tBuffer[t]=0;
}


/* STATIC */
bool ADecoderHelper::recognizeRIFF(uint8_t *buf)
{
  if(!strncmp((char *)buf,"RIFF",4)) {
    char type[5];
    memcpy(type,&buf[8],4);
    type[4]=0;
    DEBUG_OUT<<"RIFF file type: "<<type<<"\n";
    // FIXME: Do some other stuff here...
    ////DEBUG_OUT.warning("AWAVDecoder::recognize not fully implemented yet!\n");
    if(!strncmp(type,"sfbk",4)) {
      // "sfbk" is E-Mu sound font/bank
      return true;
    }
    if(!strncmp(type,"WAVE",4)) {
      // "WAVE" is a windows wav file...
      return true;
    }
    if(!strncmp(type,"AVI ",4)) {
      // "AVI " is a windows avi file...
      return true;
    }
  }
  return false;
}

/* STATIC */
bool ADecoderHelper::recognizeAIFF(uint8_t *buf)
{
  if(!strncmp((char *)buf,"FORM",4)) {
    char type[5];
    memcpy(type,&buf[8],4);
    type[4]=0;
    DEBUG_OUT<<"IFF file type: "<<type<<"\n";
    if(!strncmp((char *)(buf+8),"AIFF",4)) {
      // good enough
      return true;
    }
  }
  return false;
}



/* STATIC */
bool ADecoderHelper::recognizeMidi(uint8_t *buf)
{
  if(!strncmp((char *)buf,"MThd",4)) return true;
  return false;
}


/* STATIC */
bool ADecoderHelper::recognizeMPEG(uint8_t *buf)
{
  // TODO: recognize various kinds of mpegs...
  if((buf[0]==(uint8_t)0xFF)&&(buf[1]==(uint8_t)0xFB)&&(buf[2]==(uint8_t)0x90)) return true;  // mp3 without tags
  if((buf[0]==(uint8_t)0x00)&&(buf[1]==(uint8_t)0x00)&&(buf[2]==(uint8_t)0x01)&&(buf[3]==(uint8_t)0xBA)&&(buf[4]==(uint8_t)0x21)&&(buf[5]==(uint8_t)0x00)) return true;  // mp2
  if(!strncmp("ID3",(char *)buf,3)) return true;  // mp3 with id3 tags
  if(!strncmp("moov",(char *)(buf+4),4)) return true;  // QuickTime
  if(!strncmp("M4A ",(char *)(buf+8),4)) return true;  // m4a
  if(!strncmp("M4V ",(char *)(buf+8),4)) return true;  // m4v
  return false;
}


void ADecoderHelper::headerAIFF()
{
  // FIXME: ADecoderHelper::headerAIFF needs to be implemented
  ////DEBUG_OUT.warning("ADecoderHelper::headerAIFF not implemented!\n";
}


void ADecoderHelper::headerWAV()
{
  // FIXME: ADecoderHelper::headerWAV needs to be implemented
  ////DEBUG_OUT.warning("ADecoderHelper::headerWAV not implemented!\n";
}



void ADecoderHelper::headerMidi()
{
  // FIXME: ADecoderHelper::headerMidi needs to be implemented
  //DEBUG_OUT.warning("ADecoderHelper::headerMidi not implemented!\n";
}


void ADecoderHelper::headerMPEG()
{
  // FIXME: ADecoderHelper::headerMPEG needs to be implemented
  //DEBUG_OUT.warning("ADecoderHelper::headerMPEG not implemented!\n";
  width=320;  height=200;
  depth=24;  nPlanes=1;
  calc();
}

