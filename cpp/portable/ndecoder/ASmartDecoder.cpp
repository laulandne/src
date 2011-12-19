
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>



#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>

#include "ASmartDecoder.h"

#include "ASmartImageDecoder.h"
#include "ASmartMovieDecoder.h"
//#include "ASmartSongDecoder.h"
//#include "ASmartSoundDecoder.h"
//#include "ASmartMeshDecoder.h"
//#include "ASmartSceneDecoder.h"


#ifdef ASYS_WINDOWS
#include <io.h>
#endif // ASYS_WINDOWS


//static char tempStringBuf[256];


////////////////////////////////////////////////////////////////////////////////
//  ASmartDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
ADecoder *ASmartDecoder::pickBestDecoder(const char *theFilename)
{
  DEBUG_OUT<<"ASmartDecoder::pickBestDecoder...\n";
  ADecoder *dec=NULL;
  //
  //dec=ASmartSongDecoder::pickSongDecoder(theFilename);
  //if(dec) return dec;
  //dec=ASmartSoundDecoder::pickSoundDecoder(theFilename);
  //if(dec) return dec;
  dec=ASmartImageDecoder::pickImageDecoder(theFilename);
  if(dec) return dec;
  dec=ASmartMovieDecoder::pickMovieDecoder(theFilename);
  if(dec) return dec;
  //dec=ASmartMeshDecoder::pickMeshDecoder(theFilename);
  //if(dec) return dec;
  //dec=ASmartSceneDecoder::pickSceneDecoder(theFilename);
  //if(dec) return dec;
  //
  std::cerr<<"pickBestDecoder couldn't recognize that file.\n";
  return NULL;
}


/* STATIC */
bool ASmartDecoder::getBuffer(const char *theFilename,uint8_t *buffer)
{
  if(!theFilename) {
    std::cerr<<"getBuffer passed NULL filename!\n";
    return false;
  }
  if(!strlen(theFilename)) {
    std::cerr<<"getBuffer passed 0 sized filename!\n";
    return false;
  }
  FILE *fptr=(FILE *)NULL;
  AStringHelper shelp;
  DEBUG_OUT<<"Looking for '"<<theFilename<<"'\n";
  char pathName[1024];
  if(aDefaultStringHelper.getPathFromFullName(pathName,theFilename)) {
    // If there's a prefixed path to the config file, we'll go to that directory
    DEBUG_OUT<<"path is "<<pathName<<"\n";
#ifdef _MSC_VER
    // TODO: what?
    //_chdir(pathName);
#else
    chdir(pathName);
#endif // _MSC_VER
  }
  char fileName[1024];
  if(aDefaultStringHelper.getNameFromFullName(fileName,theFilename)) {
    DEBUG_OUT<<"file is "<<fileName<<"\n";
    theFilename=fileName;
  }
  // open the file
  fptr=fopen(theFilename,"rb");
  if(!fptr) fptr=fopen(shelp.removeQuotes(theFilename),"rb");
  if(!fptr) fptr=fopen(shelp.flipSlashes(theFilename),"rb");
  if(!fptr) {
    std::cerr<<"getBuffer couldn't open file: '"<<(char *)theFilename<<"'!\n";
    return false;
  }
  //bzero(buffer,128);
  for(int t=0;t<128;t++) buffer[t]=0;
  fseek(fptr,0,SEEK_SET);
  fread(buffer,128,1,fptr);      // grab a few bytes
  if(fptr) fclose(fptr);      // close the file
  return true;
}


