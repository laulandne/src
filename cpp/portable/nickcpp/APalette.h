#ifndef ALIB_APALETTE_H
#define ALIB_APALETTE_H


#include "AColor.h"


// Useful triplet breaking macros...
// These are for getting 8 bit colors out of a 24 bit triplet
#define getRGBRed24(x)   ((unsigned int)((x>>16)&0xff))
#define getRGBGreen24(x) ((unsigned int)((x>>8)&0xff))
#define getRGBBlue24(x)  ((unsigned int)(x&0xff))
// These are for getting 4 bit colors out of a 12 bit triplet
#define getRGBRed12(x)   ((unsigned int)((x>>8)&0xf))
#define getRGBGreen12(x) ((unsigned int)((x>>4)&0xf))
#define getRGBBlue12(x)  ((unsigned int)(x&0xf))
// These are for getting 3 bit colors out of a 9 bit triplet
#define getRGBRed9(x)    ((unsigned int)((x>>6)&0x7))
#define getRGBGreen9(x)  ((unsigned int)((x>>3)&0x7))
#define getRGBBlue9(x)   ((unsigned int)(x&0x7))
// Build 24 bit triplet from 8 bit colors
#define makeRGB24(x,y,z) ((z&0xff)+((y&0xff)<<8)+((x&0xff)<<16))
// Build 12 bit triplet from 4 bit colors
#define makeRGB12(x,y,z) ((z&0xf)+((y&0xf)<<4)+((x&0xf)<<8))
// Build 9 bit triplet from 3 bit colors
#define makeRGB9(x,y,z) ((z&0x7)+((y&0x7)<<3)+((x&0x7)<<6))


class APalette {
public:
  APalette(unsigned int num,unsigned int bitsNum=0);
  APalette(APalette *orig); // Make an exact duplicate
  virtual ~APalette();
  unsigned int getNumColors() const { return numColors; }
  void setColor(unsigned int index,unsigned int red, unsigned int green,unsigned int blue);
  void setColor(unsigned int index,unsigned long triplet);
  void setColor(unsigned int index,AColor *aColor);
  void reverse();
  void resize(unsigned int newSize);
  unsigned long findBestColor(unsigned long triplet);
  unsigned long findBestColor(unsigned int red, unsigned int green,unsigned int blue);
  bool isEmpty();
  bool isReadOnly() const { return readOnly; }
  void setReadOnly(bool val) { readOnly=val; }
  bool sameAs(APalette *otherPalette);
  bool closeEnoughTo(APalette *otherPalette);
  void touch();
  bool histogramComputed() const { return histComputed; }
  // This need's original to have a computed histogram...
  // It will take another palette and remap as many colors as it can
  void remap(APalette *original);
  void finishHistogram();
  void resetHistogram();
  void createSimpleMap(APalette *org);
  void createEmptyMapAndHistogram();
  void copy(APalette *p);
  int *getMap() const { return map; }
  int readMap(unsigned int i) const { return map[i]; }
  void writeMap(unsigned int i, int val) { map[i]=val; }
  void printMap();
  long *getScore() const { return score; }
  long readScore(unsigned int i) const { return score[i]; }
  void writeScore(unsigned int i, long val) { score[i]=val; }
  unsigned int red(unsigned int i) const { return r[i]; }
  unsigned int green(unsigned int i) const { return g[i]; }
  unsigned int blue(unsigned int i) const { return b[i]; }
  unsigned long getColor(unsigned int i);
  void setHBrite() { hBrite=true; }
  bool isHBrite() const { return hBrite; }
  void setBitsHint(unsigned int bh) { bitsHint=bh; }
  unsigned long findLightest();
  unsigned long findDarkest();
  unsigned int getSigBits() { return significantBits; }
  unsigned int getMapSigBits() { return mapSigBits; }
  void setToDefaultGrayscale();
  void setToDefaultRGB();
private:
  void init();
  void freeParts();
  void allocParts();
  void makeMap();
  void makeInverseMap();
  void setFirstAndLastColors();
  unsigned int bits2Num(unsigned int bits);
  //void printColor(unsigned int t);
  unsigned int *r;
  unsigned int *g;
  unsigned int *b;
  bool *used;
  long *score;
  int *map;
  bool hBrite;
  bool readOnly;
  unsigned int numColors;
  // # of bits total that are significant, must be divisible by 3
  unsigned int significantBits;
  // # of bits total that are significant in map, must be divisible by 3
  unsigned int mapSigBits;
  // # bits per triplet...usually 1/3 of mapSigBits...
  unsigned int bitsHint;
  unsigned int bitsMask;
  // Has the palette been changed since it was created?
  bool changed;
  // Is the background black (not used yet)
  bool blackBackground;
  // Does this palette have a computed histogram for an image?
  bool histComputed;
  bool tooBig;
  // Is the palette the default RGB palette?
  bool defaultRGB;
};



#endif // ALIB_APALETTE.H

