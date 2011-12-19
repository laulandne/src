#ifndef ALIB_AFRAMEABSTRACT_H
#define ALIB_AFRAMEABSTRACT_H


#include "APoint.h"
#include "ARect.h"
#include "AError.h"


enum ABitmapType {
  A_BITMAP_NONE=0,    // a NULL bitmap
  A_BITMAP_PLANES=1,  // pixels are deep
  A_BITMAP_CHUNKY=2,  // packed pixels
  A_BITMAP_HYBRID=3   // planes of packed pixels (NOTE: modes are bits)
                      // NOTE: nobody really uses hybrid mode!
};


enum ABitmapColorOrder {
  A_BITMAP_RGB=0,
  A_BITMAP_BGR=1
};


class AFrameAbstract
{
public:
  AFrameAbstract();
  virtual ~AFrameAbstract();
  bool isTrueColor() const { return trueColor; }
  bool boundsValid();
  virtual unsigned int getWidth() const { return width; }
  virtual unsigned int getHeight() const { return height; }
  unsigned int getDepth() const { return depth; }
  unsigned int getNPlanes() const { return nPlanes; }
  unsigned int getBytesPerLine() const { return bytesPerLine; }
  unsigned int getMinBlit() const { return minBlit; }
  unsigned int getMaxBlit() const { return maxBlit; }
  unsigned int getX() const { return x; }
  unsigned int getY() const { return y; }
  //
  AError err;
protected:
  virtual void init();
  // Calculate the "min/maxBlit" fraction...pixel/byte ratio nand
  // figure the type based on depth and nPlanes
  void calc();
  unsigned int calcBPL(unsigned int unit=0);
  // NOTE: These won't all be used by derived classes...
  unsigned int depth,nPlanes;
  unsigned int bytesPerLine;
  // The pixel/byte ratio: minBlit pixels per byte, maxBlit bytes per pixel
  unsigned int minBlit,maxBlit;
  ABitmapType bitmapType;
  ABitmapColorOrder colorOrder;
  // Are the pixels in some compressed mode for video hardware use?
  bool compressed;
  // Are our pixels in rgb instead of indexed and thus we don't have a palette?
  bool trueColor;
  unsigned long planeSize;
  // These are used to see if we're one of the "optimized" style bitmaps...
  bool eightBits,twentyFourBits;
  // Are our rows interleaved for video hardware use?
  bool interleaved;
  bool modified;  // TRUE if ALib modified the pixels behind your back
  int x,y;
  unsigned int width,height;
};


#endif // ALIB_AFRAMEABSTRACT_H

