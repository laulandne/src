#ifndef EMU_NAPPLEHANDLER_H
#define EMU_NAPPLEHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_APPLE_IO_NONE      0x00
#define N_APPLE_IO_APPLE1    0x01
#define N_APPLE_IO_APPLE2    0x02
#define N_APPLE_IO_APPLE3    0x03
#define N_APPLE_IO_APPLE2E   0x04
#define N_APPLE_IO_APPLE2GS  0x05


class NAppleDos;


class NAppleHandler : public EHandler {
friend class NAppleVideo;
friend class NAppleIO;
public:
  // Public Member Functions
  NAppleHandler();
  virtual ~NAppleHandler();
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
