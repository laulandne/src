#ifndef EMU_NCPMHANDLER_H
#define EMU_NCPMHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_CPM_IO_NONE     0x00
#define N_CPM_IO_STANDARD 0x01
#define N_CPM_IO_AMSTRAD  0x02
#define N_CPM_IO_MICROBEE 0x03
#define N_CPM_IO_SAMCOUPE 0x04
#define N_CPM_IO_SPECTRUM 0x05
#define N_CPM_IO_KAYPRO   0x06
#define N_CPM_IO_OSBORNE  0x07
#define N_CPM_IO_MSX      0x08


class NCpmDos;


class NCpmHandler : public EHandler {
friend class NCpmIO;
public:
  // Public Member Functions
  NCpmHandler();
  virtual ~NCpmHandler();
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  virtual void io_out8(ULONG addr,BYTE data);
  virtual BYTE io_in8(ULONG addr);
  //
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  // Private Data Members
};


#endif
