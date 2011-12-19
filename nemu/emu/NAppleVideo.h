#ifndef EMU_NAPPLEVIDEO_H
#define EMU_NAPPLEVIDEO_H


#include <elib/EVideoIOMultiMode.h>


class NAppleVideo : public EVideoIOMultiMode {
public:
  // Public Member Functions
  NAppleVideo(NAppleHandler *parent,unsigned int ncol,unsigned int nrow);
  virtual ~NAppleVideo();
  //
  virtual void reset();
  virtual void setValid();
  virtual void setOffsets();
  virtual void my_vregs();
  //
  virtual BYTE write8(ULONG addr,BYTE value);
  //
  void setSplitScreen(bool f);
  void setGfx(bool f);
  void setGfxHigh(bool f);
  void setPage2(bool f);
  void setupVideoMode();
  //
  // Public Data Members
protected:
  // Private Member Functions
  virtual int convertToAscii(int c);
  // Private Data Members
  bool sMixed,sText,sHires,s80col,sPage2;
  bool newVideo;
};


#endif
