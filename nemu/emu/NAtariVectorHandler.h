#ifndef EMU_NATARIVECTORHANDLER_H
#define EMU_NATARIVECTORHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_ATARIVEC_IO_NONE        0
#define N_ATARIVEC_IO_LUNARLANDER 1
#define N_ATARIVEC_IO_BATTLEZONE  2
#define N_ATARIVEC_IO_ASTEROIDS   3
#define N_ATARIVEC_IO_REDBARON    4
#define N_ATARIVEC_IO_BLACKWIDOW  5
#define N_ATARIVEC_IO_TEMPEST     6
#define N_ATARIVEC_IO_MAJORHAVOC  7


class NAtariVectorHandler : public EHandler {
public:
  // Public Member Functions
  NAtariVectorHandler();
  virtual ~NAtariVectorHandler();
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
