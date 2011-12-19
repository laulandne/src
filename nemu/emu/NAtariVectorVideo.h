#ifndef EMU_NATARIVECTORVIDEO_H
#define EMU_NATARIVECTORVIDEO_H


#include <elib/EVideoIOVector.h>


class NAtariVectorVideo : public EVideoIOVector {
public:
  // Public Member Functions
  NAtariVectorVideo(NAtariVectorHandler *parent,unsigned int ncol,unsigned int nrow);
  virtual ~NAtariVectorVideo();
  // Public Data Members
private:
  // Private Member Functions
  // Private Data Members
};


#endif
