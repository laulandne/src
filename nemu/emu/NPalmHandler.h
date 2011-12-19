#ifndef EMU_NPALMHANDLER_H
#define EMU_NPALMHANDLER_H


#include <elib/Em68kHandler.h>


// Standard "tiny" files
#define NPALM_STDIN   0
#define NPALM_STDOUT  1
#define NPALM_STDERR  2


// Path for root of the emulated filesystem...
#define NPALM_FS_PATH  "/tmp/nemu")


// Types of I/O Emulation
#define N_PALM_IO_NONE    0x00
#define N_PALM_IO_GENERIC 0x01


class NPalmHandler : public Em68kHandler {
public:
  // Public Member Functions
  NPalmHandler();
  virtual ~NPalmHandler();
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  virtual bool systemCall(unsigned int num=0);
  virtual void my_reset();
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
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
