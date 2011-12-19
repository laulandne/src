#ifndef ELIB_EMEMIO_H
#define ELIB_EMEMIO_H


#include "ERam.h"


class EHandler;


class EMemIO : public ERam {
public:
  // Public Member Functions
  EMemIO(EHandler *parent);
  virtual ~EMemIO();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  virtual void io_cycle();
  virtual void reset();
  virtual void ioregs();
  virtual ULONG getActualSize() const { return actualSize; }
  virtual void resize(unsigned int s);
  virtual void resize(unsigned int actual,unsigned int mapSize);
  void debugger(const char *str=(const char *)NULL);
  bool getDebug() { return debug; }
  void setDebug(bool t) { debug=t; }
  // Public Data Members
protected:
  // Private Member Functions
  virtual void init();
  // Private Data Members
  bool debug;
  ULONG actualSize;
  EHandler *handler;
  BYTE type;
  BYTE temp;
};


#endif
