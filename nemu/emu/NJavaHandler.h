#ifndef EMU_NJAVAHANDLER_H
#define EMU_NJAVAHANDLER_H


#include <elib/EHandler.h>


// Standard "tiny" files
#define NJAVA_STDIN   0
#define NJAVA_STDOUT  1
#define NJAVA_STDERR  2


// Path for root of the emulated filesystem...
#define NJAVA_FS_PATH  "/tmp/nemu")


// Types of I/O Emulation
#define N_JAVA_IO_NONE    0x00
#define N_JAVA_IO_GENERIC 0x01


class NJavaHandler : public EHandler {
public:
  // Public Member Functions
  NJavaHandler();
  virtual ~NJavaHandler();
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
  // these read the classfile (actually parse it into objects)
  void loadInitialClass();
};


#endif
