
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/AConvertingBitLine.h>


#include "AMultiImageDecoder.h"
#include "ADecoderHelper.h"


////////////////////////////////////////////////////////////////////////////////
//  AMultiImageDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMultiImageDecoder::AMultiImageDecoder(const char *fname) : AImageDecoder(fname)
{
  init();
  theImage=(ABitmap *)NULL;
  bitLine=(AConvertingBitLine *)NULL;
  compressed=true;
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
  /*if(!error()) if (!(*/initLineBuffer();//)) errorFlag = true;
  /*if(!error())*/ color();
}


/* STATIC */
bool AMultiImageDecoder::recognize(uint8_t *buffer,char *fname)
{
  if(ADecoderHelper::recognizeGIF(buffer)) return true;
  if(ADecoderHelper::recognizePNG(buffer)) return true;
  if(ADecoderHelper::recognizeJPEG(buffer)) return true;
  if(!strncmp("P6",(char *)buffer,2)) return true;
  if(!strncmp("P5",(char *)buffer,2)) return true;
  // FIXME: Check for other formats PNM/PBM/PPM supports...(TGA at least)
#ifdef ASYS_DARWIN
  // SIPS supports jpg, tiff, png, gif, jp2, pict, gmp, qtif, psd, sgi, and targa
  // TODO: How to call command in static method?
#endif // ASYS_DARWIN
  return false;
}


void AMultiImageDecoder::header()
{
  //if(errorFlag) return;
  bool didIt=false;
  uint8_t buffer[128];
  char cmdbuf[1024];
  tname[0]=0;
  fseek(fd,0,SEEK_SET);
  fread(buffer,128,1,fd);      // grab a few bytes
  if(!strncmp("P6",(char *)buffer,2)) { readPNM(filename); return; }  // already pnm
  if(!strncmp("P5",(char *)buffer,2)) { readPNM(filename); return; }  // already pnm
  if(!didIt) {
    // Yes, I know this is not the most secure way to do this...
    uint8_t num=0;
#ifdef ALIB_HAVE_GETPID
    num=getpid();
#endif // ALIB_HAVE_GETPID
    sprintf(tname,"/tmp/APNMDecoderTempFile-%d.pnm",num);
    DEBUG_OUT<<"Running command to decode file to "<<tname<<"...\n";
    if(ADecoderHelper::recognizeGIF(buffer)) {
      // File is a GIF, run giftoppm
      //sprintf(cmdbuf,"giftoppm <%s >%s",filename,tname);
      sprintf(cmdbuf,"giftopnm <%s >%s",filename,tname);
      DEBUG_OUT<<"cmdbuf is "<<cmdbuf<<"\n";
      if(!system(cmdbuf)) didIt=true;
      else //DEBUG_OUT.warning("giftopnm command failed!\n")
      ;
    }
/*
    if(ADecoderHelper::recognizePNG(buffer)) {
      // File is a PNG, run ?
      sprintf(cmdbuf,"djpeg -pnm >%s %s ",tname,filename);
      DEBUG_OUT<<"cmdbuf is "<<cmdbuf<<"\n";
      if(!system(cmdbuf)) didIt=true;
      else //DEBUG_OUT.warning("? command failed!\n");
    }
*/
    if(ADecoderHelper::recognizeJPEG(buffer)) {
      // File is a JPEG, run djpeg
      sprintf(cmdbuf,"djpeg -pnm >%s %s ",tname,filename);
      DEBUG_OUT<<"cmdbuf is "<<cmdbuf<<"\n";
      if(!system(cmdbuf)) didIt=true;
      else //DEBUG_OUT.warning("djpeg command failed!\n")
      ;
    }
    if(!didIt) {
      sprintf(cmdbuf,"anytopnm >%s %s ",tname,filename);
      DEBUG_OUT<<"cmdbuf is "<<cmdbuf<<"\n";
      if(!system(cmdbuf)) didIt=true;
      else //DEBUG_OUT.warning("anytopnm command failed!\n")
      ;
    }
  }
  if(!didIt) {
    // If we got here we didn't REALLY recognize the file type...
    DEBUG_OUT<<"AMultiImageDecoder::header didn't recognize filetype!\n";
    //errorFlag=true;
    return;
  }
  readPNM(tname);
}


void AMultiImageDecoder::color()
{
  //if(errorFlag) return;
  if(!theImage) return;
  colors=theImage->getColors();
}


uint8_t *AMultiImageDecoder::readLine(unsigned int lineNum,unsigned int numPlane)
{
  //if(errorFlag) return (unsigned char *)NULL;
  if(!lineBuffer) return NULL;
  if(!theImage) return NULL;
  if(!bitLine) return NULL;
  if(lineNum>height) {
    DEBUG_OUT<<"line ref out of bounds!\n";  return (uint8_t *)NULL;
  }
  if(numPlane>depth) {
    DEBUG_OUT<<"plane ref out of bounds!\n";  return (uint8_t *)NULL;
  }
  if(!fd) return (uint8_t *)NULL;
  if(bitLine) {
    theImage->copyLineTo(bitLine,lineNum);
    uint8_t *src=bitLine->getPlane(0);
    if(src) {
      for(unsigned int t=0;t<bytesPerLine;t++) lineBuffer[t]=*(src+t);
    }
  }
  return lineBuffer;
}


bool AMultiImageDecoder::readPNM(const char *ttname)
{
  DEBUG_OUT<<"readPNM, ttname is "<<ttname<<"\n";
  //if(errorFlag) return false;
  FILE *f=fopen(ttname,"rb");
  if(f) {
    // Check out start of tmp file...
    uint8_t buffer[128];
    fseek(f,0,SEEK_SET);
    fread(buffer,3,1,f);  // NOTE: we read the return too
    unsigned int bufp=0;
    bool isPNM=false;
    unsigned factor=0;
    if(!strncmp("P6",(char *)buffer,2)) { isPNM=true; factor=3; }
    if(!strncmp("P5",(char *)buffer,2)) { isPNM=true; factor=1; }
    if(isPNM) {
      DEBUG_OUT<<"Looks like a valid PNM file...\n";
      bool oldDecMode=false;
      // Read width first
      bufp=0;
      bool reading=true;
      while(reading) {
        fread(buffer+bufp,1,1,f);
        if((buffer[bufp]==0x0a)||(buffer[bufp]==' ')) {
          reading=false;
          buffer[bufp]=0;
        }
        else bufp++;
        if(bufp==128) reading=false;
      }
      oldDecMode=aDefaultStringHelper.getDecMode();
      aDefaultStringHelper.setDecMode();
      width=aDefaultStringHelper.parseNumber((char *)buffer);
      // Then read height
      bufp=0;
      reading=true;
      while(reading) {
        fread(buffer+bufp,1,1,f);
        if((buffer[bufp]==0x0a)||(buffer[bufp]==' ')) {
          reading=false;
          buffer[bufp]=0;
        }
        else bufp++;
        if(bufp==128) reading=false;
      }
      height=aDefaultStringHelper.parseNumber((char *)buffer);
      aDefaultStringHelper.setDecMode(oldDecMode);
      // Now the third field (is it levels of rgb?)
      bufp=0;
      reading=true;
      while(reading) {
        fread(buffer+bufp,1,1,f);
        if((buffer[bufp]==0x0a)||(buffer[bufp]==' ')) {
          reading=false;
          buffer[bufp]=0;
        }
        else bufp++;
        if(bufp==128) reading=false;
      }
      unsigned int mystery=aDefaultStringHelper.parseNumber((char *)buffer);
      DEBUG_OUT<<"The mystery number is "<<mystery<<"\n";
      aDefaultStringHelper.setDecMode(oldDecMode);
      // We've got the size of the image, now the hard work...
      DEBUG_OUT<<"Looks like the image is "<<width<<"x"<<height<<"\n";
      depth=8;  // Lie for now...
      nPlanes=1;
      calc();
      bytesPerLine=calcBPL(2); // round up width to 32-bit boundary
      depth=8*factor;  // actual depth
      width=bytesPerLine;
      calc();
      DEBUG_OUT<<"Maybe image should be "<<width<<"x"<<height<<"\n";
      DEBUG_OUT<<"Guessing the bytesPerLine should be "<<bytesPerLine<<"\n";
      //needsRGBReorder=true;
      theImage=new ABitmap(width,height,depth,nPlanes);
      if(theImage) {
        bitLine=theImage->getLine(0);
        if(bitLine) {
          uint8_t *lineBuffer=(uint8_t *)malloc(bytesPerLine);
          if(lineBuffer) {
            for(unsigned int ty=0;ty<height;ty++) {
              fread(lineBuffer,bytesPerLine,1,f);
              theImage->writeLine(ty,lineBuffer,bytesPerLine,0);
            }
            free(lineBuffer);
            lineBuffer=NULL;
          }
          else {
            DEBUG_OUT<<"readPNM Couldn't create the tmp lineBuffer!\n";
            //errorFlag=true;
          }
        }
        else {
          DEBUG_OUT<<"readPNM Couldn't create the tmp bitLine!\n";
          //errorFlag=true;
        }
      }
      else {
        DEBUG_OUT<<"readPNM Couldn't create the tmp image!\n";
        //errorFlag=true;
      }
    }
    else {
      DEBUG_OUT<<"readPNM, didn't understand file format!\n";
      //errorFlag=true;
    }
    fclose(f);
    if(tname) {
      if(strlen(tname)) {
        DEBUG_OUT<<"going to delete temp file\n";
        unlink(tname);  // delete the tmp PNM file...
      }
    }
  }
  else {
    DEBUG_OUT<<"readPNM Couldn't open "<<ttname<<"!\n";
    //errorFlag=true;
  }
  return true;
}

