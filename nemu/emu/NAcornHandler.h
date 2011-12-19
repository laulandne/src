#ifndef EMU_NACORNHANDLER_H
#define EMU_NACORNHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_ACORN_IO_NONE 0
#define N_ACORN_IO_ATOM 1
#define N_ACORN_IO_BBC  2


class NAcornHandler : public EHandler {
friend class NAcornIO;
public:
  // Public Member Functions
  NAcornHandler();
  virtual ~NAcornHandler();
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  // Private Data Members
};


#endif
