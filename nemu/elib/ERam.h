#ifndef ELIB_ERAM_H
#define ELIB_ERAM_H


#include "EMemAbstract.h"


class EMemMapAbstract;


class ERam : public EMemAbstract {
public:
  // Public member functions
  ERam(unsigned int size);
  ERam(ULONG address, unsigned int size);
  ERam(ULONG address, unsigned int numblks, unsigned int blksize);
  virtual ~ERam();
  //
  virtual BYTE read8(ULONG addr);
  virtual BYTE write8(ULONG addr,BYTE data);
  //
  virtual void resize(ULONG newsize);
  // Public data members
protected:
  // Private member functions
  virtual void init();
  virtual void freeMem();  // Call in destructor
  virtual void reallyResize(ULONG newsize);
  // Private data members
  ULONG allocSize;  // In case its different than size...
  BYTE *mem;
 };


#endif
