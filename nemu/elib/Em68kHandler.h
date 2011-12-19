#ifndef ELIB_EM68KHANDLER_H
#define ELIB_EM68KHANDLER_H


#include "EHandler.h"


class ERom;
class ERam;


class Em68kHandler : public EHandler {
public:
  // Public Member Functions
  Em68kHandler();
  virtual ~Em68kHandler();
  //
  //static unsigned int name2type(const char *name);
  //static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  virtual void my_reset();
  virtual void my_reset_postCPU();
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  // Public Data Members
  //
protected:
  //
  // Private Member Functions
  virtual void io_init(BYTE type);
  //
  // Private Data Members
  ERam *theRam;
  char *romName;
  ERom *theRom;
  unsigned int romSize;
  bool romOverlay;
  //
};


#endif
