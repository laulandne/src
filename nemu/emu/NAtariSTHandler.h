#ifndef EMU_NATARISTHANDLER_H
#define EMU_NATARISTHANDLER_H


#include <elib/Em68kHandler.h>


// Types of I/O Emulation
#define N_ATARIST_IO_NONE   0x00
#define N_ATARIST_IO_ST     0x01
#define N_ATARIST_IO_TT     0x02
#define N_ATARIST_IO_FALCON 0x03


class NAtariSTHandler : public Em68kHandler {
public:
  // Public Member Functions
  NAtariSTHandler();
  virtual ~NAtariSTHandler();
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
