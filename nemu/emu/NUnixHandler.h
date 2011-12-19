#ifndef EMU_NUNIXHANDLER_H
#define EMU_NUNIXHANDLER_H


#include <elib/EHandler.h>

#include <portable/nickcpp/AStringHelper.h>


// Standard "tiny" files
#define NUNIX_STDIN   0
#define NUNIX_STDOUT  1
#define NUNIX_STDERR  2


// Path for root of the emulated filesystem...
#define NUNIX_FS_PATH  "/tmp/nemu")


// Types of I/O Emulation
#define N_UNIX_IO_NONE    0x00
#define N_UNIX_IO_GENERIC 0x01
#define N_UNIX_IO_LINUX   0x02
#define N_UNIX_IO_SOLARIS 0x03
#define N_UNIX_IO_DARWIN  0x04


class NUnixHandler : public EHandler {
public:
  // Public Member Functions
  NUnixHandler();
  virtual ~NUnixHandler();
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  virtual bool systemCall(unsigned int num=0);
  virtual void my_reset();
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  virtual void my_reset_postCPU();
  //
  // Private Data Members
};


#endif
