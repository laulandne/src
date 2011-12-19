#ifndef ELIB_EVIDEOIOABSTRACT_H
#define ELIB_EVIDEOIOABSTRACT_H


#include <elib/EMemIO.h>
#include <elib/EVideoSprite.h>


class AColor;
class EMemAbstract;
class APalette;
class ERom;


// Decode rom copy modes...
#define EVIDEO_NONE      0
#define EVIDEO_INVERSE   1
#define EVIDEO_ROTATE90  2
#define EVIDEO_ROTATE180 4
#define EVIDEO_ROTATE270 6  // 90+180


// NOTE: These are arbitrary
#define A_MAX_NUMBER_SPRITES 256
#define A_MAX_NUMBER_GFX 256


class EVideoIOAbstract : public EMemIO {
public:
  // Public Member Functions
  EVideoIOAbstract(EHandler *parent,unsigned int ncol,unsigned int nrow, unsigned int depthHint=1);
  virtual ~EVideoIOAbstract();
  //
  virtual void setLocation(ULONG ad);
  virtual void redraw();
  virtual void focus();
  virtual void reallyFocus();
  virtual void io_cycle();
  virtual void clearVideo();
  virtual void setScreenChar(unsigned int s,unsigned int c,bool force=false);
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual void vregs();
  virtual void my_vregs();
  virtual bool reallyVideo();
  //
  virtual BYTE read8ColorMem(ULONG address);
  virtual BYTE write8ColorMem(ULONG address,BYTE val);
  virtual EMemAbstract *getColorMem() const { return attrMem; }
  virtual void setColorMemAddr(ULONG addr);
  //
  virtual ULONG getScreenAddr() const { return screenAddr; }
  virtual ULONG getCharAddr() const { return tileAddr; }
  //virtual APrivateBitmap *getBackBuffer() const { return backBuffer; }
  virtual APalette *getColors() const { return colors; }
  virtual unsigned int getFramesPerSec() const { return framesPerSec; }
  //
  virtual void setBorder(unsigned int bwidth,ULONG bclr);
  virtual void setBackground(ULONG val);
  virtual ULONG getBackground() { return backgroundColor; }
  virtual void setDefaultForeground(ULONG val);
  virtual void drawBorder();
  virtual bool readRomPalette(char *fname,unsigned int numColors,unsigned int numBits);
  //
  virtual void setMemSize(unsigned int ncol, unsigned int nrow,bool really=false);
  virtual void setVideoSize(unsigned int ncol, unsigned int nrow);
  virtual void setViewableSize(unsigned int w,unsigned int h);
  //
  virtual void setTextMode(bool really=true);
  virtual void setBitmapMode(bool really=true);
  virtual void setMixedMode(bool really=true);
  //
  virtual void initOffsets();
  virtual void setValid();
  virtual void setOffsets();
  //
  //virtual void setupTileGfx(APrivateBitmap *timage,unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown);
  //virtual void showTileGfx();
  virtual unsigned int getTilesAcross() const { return tilesAcross; }
  virtual unsigned int getTilesDown() const { return tilesDown; }
  //
  virtual void setSpriteActive(unsigned int num,bool val);
  virtual EVideoSprite *getSprite(unsigned int num);
  //virtual void showSpriteGfx();
  virtual void drawSprites();
  //virtual void setupSpriteGfx(APrivateBitmap *timage,unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown);
  virtual void setNumSprites(unsigned int num);
  virtual void setSpriteRegisters(EMemIO *handler, ULONG address);
  //
  virtual unsigned long getRaster();
  virtual unsigned long getRasterFactor() const { return rasterFactor; }
  virtual unsigned long getRasterSpeed() const { return rasterSpeed; }
  virtual unsigned long getVBlankStart() const { return vblankStart/rasterFactor; }
  virtual void setRasterFactor(unsigned long val);
  virtual void setRasterSpeed(unsigned long val);
  virtual bool inVBlank();
  virtual bool passedVBlank();
  virtual bool getIntCaused();
  virtual void setMaxRaster(unsigned long v);
  virtual void setRasterInt(unsigned long v,unsigned int wh);
  virtual void unsetRasterInt();
  virtual void clearInt();
  //
  // Public Data Members
protected:
  // Private Member Functions
  virtual void init();
  void freeVideo();  // Call this in destructors
  //
  //void decodeRomTiles(APrivateBitmap *i, ERom *r, ULONG offset, unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown, unsigned int copyMode=EVIDEO_NONE, unsigned int startRow=0);
  //void decodeMemTiles(APrivateBitmap *i,EMemAbstract *mmap,ULONG addr,unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown, unsigned int copyMode=EVIDEO_NONE, unsigned int startRow=0);
  //void rotateTiles(APrivateBitmap *i, unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown, unsigned int copyMode=EVIDEO_NONE);
  //void flipTiles(APrivateBitmap *i, unsigned int gwidth,unsigned int gheight,unsigned int gacross,unsigned int gdown,bool dx, bool dy, unsigned int copyMode=EVIDEO_NONE);
  //void readTile(APrivateBitmap *i,unsigned char *tile, unsigned int x,unsigned int y,unsigned int gwidth,unsigned int gheight);
  //void writeTile(APrivateBitmap *i,unsigned char *tile, unsigned int x,unsigned int y,unsigned int gwidth,unsigned int gheight);
  //void rotateTile(unsigned char *stile, unsigned char *dtile, unsigned int gwidth,unsigned int gheight, unsigned int copyMode);
  //void flipTile(unsigned char *stile, unsigned char *dtile, bool dx, bool dy,unsigned int gwidth, unsigned int gheight, unsigned int copyMode);
  //
  //void showPalette(AWindow *sw);
  //void pleaseWaitForKey(AWindow *sw);
  //
  void setDisplayList(bool t) { displayList=t; }
  void createBackBuffer();
  virtual int convertToAscii(int c);
  void scaleFactorChanged();
  //
  // Private Data Members
  EMemAbstract *attrMem;
  // NOTE: These may or may not be used and may point into different spaces
  ULONG screenAddr,tileAddr,attrAddr,spriteAddr;
  // NOTE: These are for client's use, they will be automattically freed.
  //APrivateBitmap *tileImage,*spriteImage;
  //APrivateBitmap *backBuffer;
  APalette *colors;  // the emulated palette (if any)
  //AWindow *videow;
  //ADisplay *videod;
  unsigned int tileWidth,tileHeight,imageTilesAcross,imageTilesDown;
  unsigned int spriteWidth,spriteHeight,imageSpritesAcross,imageSpritesDown;
  BYTE *screenRowAddrs, *screenColAddrs;
  BYTE *screenValidAddrs;
  unsigned int roundedSize,ramSize;
  bool redrawLock;
  unsigned long raster,maxRaster;  // In terms of rasterFactor
  bool causeInt, intCaused;
  ULONG whichInt;
  unsigned long rasterInt;   // In terms of rasterFactor
  unsigned long vblankStart; // In terms of rasterFactor
  unsigned long rasterFactor;
  unsigned long rasterSpeed;  // NOT in terms of rasterFactor
  unsigned int framesPerSec;
  EVideoTile tiles[A_MAX_NUMBER_GFX];
  unsigned int numSprites;
  EVideoSprite sprites[A_MAX_NUMBER_SPRITES];
  unsigned int spriteRegisters;
  EMemIO *spriteHandler;
  bool passedRasterInt;
  bool bitmapMode;  // Otherwise text
  bool displayList;  // is one active?
  unsigned int *theDisplayList;  // our format, not atari's or anyone elses...not used right now
  ULONG plane[32];
  bool planeActive[32];
  unsigned int borderSize;
  ULONG borderColor;
  ULONG backgroundColor;
  ULONG defaultForegroundColor;
  unsigned int scaleFactorX,scaleFactorY,oldScaleFactorX,oldScaleFactorY;
  bool scaled;
  bool ntsc; // Only makes sense on archs that can do both ntsc and pal, ignored otherwise
  unsigned int tilesAcross, tilesDown;
  // These are emulated geometry (not including border/overscan)
  unsigned int emuWidth,emuHeight;
  unsigned int emuDepth,emuNPlanes;
  bool trueColor;
};


#endif
