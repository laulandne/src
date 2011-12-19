#ifndef NICKCPP_ACONVERTINGBITLINE_H
#define NICKCPP_ACONVERTINGBITLINE_H


#include <portable/nickcpp/ABitmap.h>


class APalette;
class AColor;
class APrivateBitmap;


// (We're using a static array of this size, but will change to a
//   dynamically allocated array in the future)
#define NUM_PLANES	4*8


class AConvertingBitLine : public ABitmap {
public:
  friend class ABitmap;
  friend class APrivateBitmap;
  AConvertingBitLine(unsigned int w, unsigned int d, unsigned int p=1);
  AConvertingBitLine(ABitmap *source);
  virtual ~AConvertingBitLine();
  //
  void planes2Chunky8(AConvertingBitLine *source);
  void planes2Chunky24(AConvertingBitLine *source);
  void chunky2Chunky8(AConvertingBitLine *source);
  void chunky2Chunky24(AConvertingBitLine *source);
  void eightBits2Planes(AConvertingBitLine *source);
  void eightBits2Chunky(AConvertingBitLine *source);
  void twentyFourBits2Planes(AConvertingBitLine *source);
  void twentyFourBits2Chunky(AConvertingBitLine *source);
  void twentyFourBitsChunky2Planes(AConvertingBitLine *source);
  void twentyFour2Chunky8(AConvertingBitLine *source); // Chunky or planar...
  void ham2Chunky24(AConvertingBitLine *source);
  void eightBits2Chunky24(AConvertingBitLine *source);
  //
  void remapChunky8(APalette *givenColors);
  void scaleLine(AConvertingBitLine *source,int xFactor);
  void flipLine();
  void convertChunky8();
  void convertChunky24();
  void debugDump();
  //
private:
  //
  void init();
  void resetHistogram();
  void cumulativeHistogram(AConvertingBitLine *source);
  //
  unsigned int originalBits;
  bool warned;
  AConvertingBitLine *temp24;
};


#endif

