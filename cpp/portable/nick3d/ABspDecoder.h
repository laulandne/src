#ifndef ALIB_ABSPDECODER_H
#define ALIB_ABSPDECODER_H


#include "AMapDecoder.h"


// BSP game map
class ABspDecoder : public AMapDecoder
{
public:
  //
  ABspDecoder(const char *fname);
  static bool recognize(uint8_t *str);
  //
protected:
  //
  void init();
  void readObjects();
  //
  char the_format;
  //
};


#endif // ALIB_ABSPDECODER_H

