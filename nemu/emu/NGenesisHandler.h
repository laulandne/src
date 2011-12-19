#ifndef EMU_NGENESISHANDLER_H
#define EMU_NGENESISHANDLER_H


#include <elib/Em68kHandler.h>


// Types of I/O Emulation
#define N_GENESIS_IO_NONE    0x00
#define N_GENESIS_IO_GENESIS 0x01


class NGenesisHandler : public Em68kHandler {
public:
  // Public Member Functions
  NGenesisHandler();
  virtual ~NGenesisHandler();
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
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
private:
  //
  // Private Member Functions
  virtual void io_init(BYTE type);
  //
  // Private Data Members
  //
};


#endif
