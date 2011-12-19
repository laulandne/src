#ifndef EMU_NCOCOHANDLER_H
#define EMU_NCOCOHANDLER_H


#include <elib/EHandler.h>


// Types of I/O Emulation
#define N_COCO_IO_NONE     0x00
#define N_COCO_IO_STANDARD 0x01


class NCocoDos;


class NCocoHandler : public EHandler {
friend class NCocoIO;
public:
  // Public Member Functions
  NCocoHandler();
  virtual ~NCocoHandler();
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  // Public Data Members
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  // Private Data Members
};


#endif
