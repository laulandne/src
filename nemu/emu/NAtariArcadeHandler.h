#ifndef EMU_NATARIARCADEHANDLER_H
#define EMU_NATARIARCADEHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_ATARIARCADE_IO_NONE      0
#define N_ATARIARCADE_IO_MISSLECMD 1
#define N_ATARIARCADE_IO_CENTIPEDE 2
#define N_ATARIARCADE_IO_MILIPEDE  3
#define N_ATARIARCADE_IO_TETRIS    4
#define N_ATARIARCADE_IO_CCASTLES  5
#define N_ATARIARCADE_IO_WARLORDS  6


class NAtariArcadeHandler : public EHandler {
friend class NAtariArcadeVideo;
friend class NAtariArcadeIO;
public:
  // Public Member Functions
  NAtariArcadeHandler();
  virtual ~NAtariArcadeHandler();
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

