#ifndef EMU_NSWFHANDLER_H
#define EMU_NSWFHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_SWF_IO_NONE      0x00


class NSwfHandler : public EHandler {
public:
  // Public Member Functions
  NSwfHandler();
  virtual ~NSwfHandler();
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
