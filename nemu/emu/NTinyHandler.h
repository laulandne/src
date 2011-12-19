#ifndef EMU_NTINYHANDLER_H
#define EMU_NTINYHANDLER_H


#include <portable/nickcpp/AStringHelper.h>

#include <elib/EHandler.h>

#include <emu/generated/NTiny_syscall_tab.h>


// Standard "tiny" files
#define NTINY_STDIN   0
#define NTINY_STDOUT  1
#define NTINY_STDERR  2


// Path for root of the emulated filesystem...
#define NTINY_FS_PATH  "/tmp/nemu"


// Types of I/O Emulation
#define N_TINY_IO_NONE   0x00
#define N_TINY_IO_TINY   0x01


class NTinyHandler : public EHandler {
public:
  // Public Member Functions
  NTinyHandler(EHandler *parent);
  virtual ~NTinyHandler();
  //
  virtual bool systemCall(unsigned int num=0);
  virtual void reset();
  //
  // Public Data Members
  //
private:
  //
  // Private Member Functions
  virtual void init();
  //
  void readMemFromAddr(long p1,long size);
  void readStringFromAddr(long p1);
  void writeMemToAddr(long p1,long size);
  void readFilenameFromAddr(long p1);
  long translateFileHandleTo(long p1);
  long translateFileHandleFrom(long p1);
  void writeStringToAddr(long p1);
  long limitToBufferSize(long suggest);
  long builtinSysCall(unsigned int p0,long p1,long p2, long p3, long p4, long p5);
  //
  long tinyInit();
  long tinyExit(long p1);
  long tinyDebugger(long p1,long p2);
  long tinyDebugString(long p1,long p2);
  long tinyOpen(long p1,long p2);
  long tinyClose(long p1);
  long tinyRead(long p1,long p2,long p3);
  long tinyWrite(long p1,long p2,long p3);
  long tinySeek(long p1,long p2,long p3);
  long tinyTell(long p1);
  long tinyFlush(long p1);
  long tinyMalloc(long p1);
  long tinyFree(long p1);
  long tinyInt2String(long p1,long p2);
  long tinyString2Int(long p1);
  long tinyUmul(long p1,long p2);
  long tinySmul(long p1,long p2);
  long tinyUdiv(long p1,long p2);
  long tinySdiv(long p1,long p2);
  //
  // Private Data Members
  char tempBuffer[ATEMPSTRINGBUF_SIZE];
  EHandler *io;
  unsigned int heapTop,heapBottom,heapCur;
  //
};


#endif
