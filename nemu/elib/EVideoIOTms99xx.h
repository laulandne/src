#ifndef EMU_NTMS99XXVIDEO_H
#define EMU_NTMS99XXVIDEO_H


#include <elib/EVideoIOMultiMode.h>


class EVideoIOTms99xx : public EVideoIOMultiMode {
public:
  // Public Member Functions
  EVideoIOTms99xx(EHandler *parent,unsigned int ncol,unsigned int nrow);
  virtual ~EVideoIOTms99xx();
  // Public Data Members
private:
  // Private Member Functions
  // Private Data Members
};


#endif
