#ifndef ELIB_EMEMMAPSIMPLE_H
#define ELIB_EMEMMAPSIMPLE_H


#include "EMemMapAbstract.h"


class EMemMapSimple : public EMemMapAbstract {
public:
  // Public member functions
  EMemMapSimple(ULONG npages,ULONG psize,ULONG sbits);
  virtual ~EMemMapSimple();
  //
  virtual BYTE read8(ULONG addr);
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE reallyWrite8(ULONG addr,BYTE data);
  //
  virtual char getWPageLetter(unsigned int page);
  virtual char getRPageLetter(unsigned int page);
  virtual char getVPageLetter(unsigned int page);
  //
  virtual void map(EMemAbstract *em);
  virtual void map(ERom *em);
  virtual void map(EMemIO *em);
  virtual void mapVideo(EVideoIOAbstract *v);
  virtual void unmap(EMemAbstract *em);
  //
  virtual const char *getMapType();
  //
  // Public data members
protected:
  // Private member functions
  virtual void init();
  virtual void mapHelper(EMemAbstract *em);
  // Private data members
  EMemAbstract *readFunc[256];
  EMemAbstract *writeFunc[256];
  EMemAbstract *videoFunc[256];
};


#endif
