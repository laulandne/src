#ifndef EMU_NORICHANDLER_H
#define EMU_NORICHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_ORIC_IO_NONE     0x00
#define N_ORIC_IO_STANDARD 0x01


class NOricDos;


class NOricHandler : public EHandler {
friend class NOricIO;
public:
  // Public Member Functions
  NOricHandler();
  virtual ~NOricHandler();
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  // Private Data Members
};


#endif
