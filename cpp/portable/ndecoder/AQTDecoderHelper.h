#ifndef DECODERLIB_AQTDECODERHELPER_H
#define DECODERLIB_AQTDECODERHELPER_H


#ifdef ASYS_MAC
//#include <guilib/alib_carbon.h>
#endif


#include "ADecoderHelper.h"


#ifdef ALIB_NO_PCH
#undef true
#endif // ALIB_NO_PCH


#ifdef USE_QUICKTIME
#ifdef USE_UMBRELLA_HEADERS
#include <QuickTime/Movies.h>
#else
#include <Movies.h>
#endif // USE_UMBRELLA_HEADERS
#endif // USE_QUICKTIME


class AQTImageDecoder;


// QuickTime decoder
class AQTDecoderHelper : public ADecoderHelper
{
public:
  AQTDecoderHelper(const char *fname);
  virtual ~AQTDecoderHelper();
  //
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
  //
  void setFile(const char *fname,FILE *theFd);
  void startQuickTime();
  void stopQuickTime();
  void initParts();
  void freeParts();
  virtual void header();
  virtual void color();
protected:
  //
#ifdef USE_QUICKTIME
  void createGWorld();
  void openMovie(char *fname);
  void debugErrorIfAny();
#endif
  //
#ifdef USE_QUICKTIME
  OSErr err;
  CGrafPtr origPort;
  GDHandle origDev;
  GWorldPtr theWorld;
  PixMapHandle thePM;
  Rect theRect;
#if !__LP64__
  Movie theMoov;
  MovieController theController;
#endif
  short movieResFile;
#ifdef ASYS_WINDOWS
  HINSTANCE qtLib;
#endif
#endif // USE_QUICKTIME
  bool gotQuickTime;
  unsigned int fDepth;  // depth we'll use for QuickTime...
};


#endif // DECODERLIB_AQTDECODERHELPER_H
