#ifndef EMU_NTRS80HANDLER_H
#define EMU_NTRS80HANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_TRS80_IO_NONE        0x00
#define N_TRS80_IO_MODEL1      0x01
#define N_TRS80_IO_MODEL3      0x02
#define N_TRS80_IO_MODEL4      0x03
#define N_TRS80_IO_COLOURGENIE 0x04


class NTrs80Dos;


class NTrs80Handler : public EHandler {
friend class NTrs80Video;
friend class NTrs80IO;
public:
  // Public Member Functions
  NTrs80Handler();
  virtual ~NTrs80Handler();
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  virtual void keypress(int c);
  //
  virtual void io_out8(ULONG addr,BYTE data);
  virtual BYTE io_in8(ULONG addr);
  //
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  //
  // Private Data Members
};


#endif
