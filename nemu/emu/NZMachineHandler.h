#ifndef EMU_NZMACHINEHANDLER_H
#define EMU_NZMACHINEHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_ZMACHINE_IO_NONE      0x00


class NZMachineHandler : public EHandler {
public:
  // Public Member Functions
  NZMachineHandler();
  virtual ~NZMachineHandler();
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  // Private Data Members
};


#endif
