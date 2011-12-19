#ifndef EMU_NCBMVIDEO_H
#define EMU_NCBMVIDEO_H


#include <elib/EVideoIOMultiMode.h>


class NCbmVideo : public EVideoIOMultiMode {
public:
  // Public Member Functions
  NCbmVideo(NCbmHandler *parent,unsigned int ncol,unsigned int nrow);
  virtual ~NCbmVideo();
  //
  virtual BYTE write8(ULONG addr,BYTE value);
  //
  virtual void setScreenChar(unsigned int s,unsigned int c,bool force=false);
  virtual void setBitmapMode(bool on);
  //
  void setScreenCharSprites(unsigned int saddr,unsigned int caddr,bool force=false);
  void setVicDoubleHeightMode(bool on);
  //
  // Public Data Members
protected:
  // Private Member Functions
  virtual int convertToAscii(int c);
  virtual void my_vregs();
  // Private Data Members
  bool multicolorMode;
};


#endif

