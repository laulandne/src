#ifndef EMU_NATARIVIDEO_H
#define EMU_NATARIVIDEO_H


#include <elib/EVideoIOMultiMode.h>


class NAtariVideo : public EVideoIOMultiMode {
public:
  // Public Member Functions
  NAtariVideo(NAtariHandler *parent,unsigned int ncol,unsigned int nrow);
  virtual ~NAtariVideo();
  // Public Data Members
private:
  // Private Member Functions
  // Private Data Members
};


#endif
