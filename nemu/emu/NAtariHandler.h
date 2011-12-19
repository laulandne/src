#ifndef EMU_NATARIHANDLER_H
#define EMU_NATARIHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_ATARI_IO_NONE      0x00
#define N_ATARI_IO_ATARI2600 0x01
#define N_ATARI_IO_ATARI7800 0x02
#define N_ATARI_IO_ATARI5200 0x03
#define N_ATARI_IO_ATARI800  0x04
#define N_ATARI_IO_ATARIXL   0x05
#define N_ATARI_IO_ATARIXE   0x06


class NAtariDos;


class NAtariHandler : public EHandler {
friend class NAtariVideo;
friend class NAtariIO;
public:
  // Public Member Functions
  NAtariHandler();
  virtual ~NAtariHandler();
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  virtual void keypress(int c);
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  // Private Data Members
};


#endif
