#ifndef EMU_NMOTO684XVIDEO_H
#define EMU_NMOTO684XVIDEO_H


#include <elib/EVideoIOMultiMode.h>


class EVideoIOMoto684x : public EVideoIOMultiMode {
public:
  // Public Member Functions
  EVideoIOMoto684x(EHandler *parent,unsigned int ncol,unsigned int nrow);
  virtual ~EVideoIOMoto684x();
  // Public Data Members
private:
  // Private Member Functions
  // Private Data Members
};


#endif
