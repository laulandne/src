#ifndef EMU_NFAMICOMHANDLER_H
#define EMU_NFAMICOMHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_FAMICOM_IO_NONE    0x00
#define N_FAMICOM_IO_FAMICOM 0x01


class ERam;


class NFamicomHandler : public EHandler {
public:
  // Public Member Functions
  NFamicomHandler();
  virtual ~NFamicomHandler();
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
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
  void nescart(const char *a1,const char *a2,const char *a3);
  //
  // Private Data Members
  ERam *theRam;
  //
};


#endif
