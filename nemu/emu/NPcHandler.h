#ifndef EMU_NPCHANDLER_H
#define EMU_NPCHANDLER_H


#include <elib/EHandler.h>

#include <portable/nickcpp/AStringHelper.h>


// Standard "tiny" files
#define NPC_STDIN   0
#define NPC_STDOUT  1
#define NPC_STDERR  2


// Path for root of the emulated filesystem...
#define NPC_FS_PATH  "/tmp/nemu")


// Types of I/O Emulation
#define N_PC_IO_NONE  0x00
#define N_PC_IO_GENERIC 0x01


// Location in memory that EXE image is loaded if needed
#define N_PC_EXE_OFF  0x100000


class NPcHandler : public EHandler {
public:
  // Public Member Functions
  NPcHandler();
  virtual ~NPcHandler();
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  virtual bool systemCall(unsigned int num=0);
  virtual void my_reset();
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
  //
  virtual void io_out8(ULONG addr,BYTE data);
  virtual BYTE io_in8(ULONG addr);
  //
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  virtual void my_reset_postCPU();
  //
  // Private Data Members
};


#endif
