#ifndef ALIB_A3DDECODER_H
#define ALIB_A3DDECODER_H


#include <portable/ndecoder/ADecoder.h>

#include <portable/nickcpp/AList.h>


extern unsigned long ARend386Colors16[];
extern unsigned long ARend386Colors256base[];


class A3DDecoder : public ADecoder
{
public:
  A3DDecoder(const char *fname);
  virtual ~A3DDecoder();
  //
  void lookupRend386Color(unsigned int surf, float *r, float *g, float *b);
  unsigned int handleFace(AList *faces, int offset, int voff, unsigned long surf);
  unsigned int buildArrayFromList(void **theArray,AList *theList);
  void **allocArrayBigEnoughForList(AList *theList);
  //
  virtual bool isMap() { return false; }
  //
protected:
};


#endif // ALIB_A3DDECODER_H

