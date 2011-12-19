#ifndef EMU_NATARIARCADEVIDEO_H
#define EMU_NATARIARCADEVIDEO_H


#include <elib/EVideoIOMultiMode.h>


class NAtariArcadeVideo : public EVideoIOMultiMode {
public:
  // Public Member Functions
  NAtariArcadeVideo(NAtariArcadeHandler *parent,unsigned int ncol,unsigned int nrow);
  virtual ~NAtariArcadeVideo();
  virtual void setValid();
  virtual void setOffsets();
  // Public Data Members
private:
  // Private Member Functions
  // Private Data Members
};


#endif

