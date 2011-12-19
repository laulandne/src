#ifndef ELIB_EIOHANDLER_H
#define ELIB_EIOHANDLER_H


#include <portable/nickcpp/AClock.h>
#include <portable/nickcpp/AError.h>

#include <elib/EMemAbstract.h>


class EVideoIOAbstract;
class ESoundIOAbstract;
class DVfs;
class ECPUAbstract;
class EMemIO;
class EMemMapAbstract;
class EOSHelper;


class EHandler
{
public:
  // Public member functions
  // The next four are the bare minimum needed
  // for a new system
  EHandler();
  virtual ~EHandler();
  //
  // NOTE: derived classes need to provide at least these three funcitons
  static bool recognize(const char *name) { return false; }
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  // NOTE: derived classes can optionally provide these...
  static char *suggestCpu() { return NULL; }
  static unsigned int suggestNumAddrBits() { return 0; }
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  // These may all be inherited if you don't
  // need any special handling
  virtual void my_io_cycle();
  virtual void my_ioregs();
  virtual void my_reset();
  virtual void my_reset_postCPU();
  virtual void keypress(int c);
  virtual bool systemCall(unsigned int num=0);
  virtual bool ioDisHelper(ULONG addr);
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
  virtual void setupVfs();
  virtual void xia_write8(ULONG addr, BYTE b);
  virtual BYTE xia_read8(ULONG addr);
  virtual void io_out8(ULONG addr,BYTE data);
  virtual BYTE io_in8(ULONG addr);
  virtual void redraw();
  virtual void io_cycle();  // NOTE: don't overload this, overload my_io_cycle instead
  virtual void ioregs();  // NOTE: don't overload this, overload my_ioregs instead
  virtual void vregs();
  virtual void reset();  // NOTE: don't overload this, overload my_reset instead
  virtual void reset_postCPU();  // NOTE: don't overload this, overload my_reset_postCPU instead
  virtual void focusVideo();
  virtual void setMap(EMemMapAbstract *m) { map=m; }
  virtual void setCPU(ECPUAbstract *c) { cpu=c; }
  virtual void setKeypress(char key) { io_keypress=key; }
  void mapParts();
  void unmapParts();
  void connectParts();
  void debugger(const char *str=(const char *)NULL);
  // The rest of these are accessors
  BYTE getType() const { return io_type; }
  BYTE getVariation() const { return io_variation; }
  ECPUAbstract *getCPU() const { return cpu; }
  EVideoIOAbstract *getVideo() const { return myVideo; }
  ESoundIOAbstract *getSound() const { return mySound; }
  EMemMapAbstract *getMap() const { return map; }
  EMemIO *getIO() const { return myIO; }
  EOSHelper *getOSHelper() const { return osHelp; }
  DVfs *getVfs() const { return vfs; }
  unsigned int getKeypress() const { return io_keypress; }
  unsigned int getTimerSpeed() const { return timerSpeed; }
  unsigned int getTimerFactor() const { return timerFactor; }
  void setTimerFactor(unsigned int v) { timerFactor=v; }
  void setTimerSpeed(unsigned int v) { timerSpeed=v; }
  bool getDebug() { return debug; }
  void setDebug(bool t) { debug=t; }
  // Public Data Members
  AError err;
  bool io_debug, io_stop;
protected:
  // Private member functions
  virtual void io_init(BYTE type);
  virtual void init();
  virtual void freeIO();
  // Private data members
  EMemMapAbstract *map;
  EOSHelper *osHelp;
  ECPUAbstract *cpu;
  AClock myTimer;
  EVideoIOAbstract *myVideo;
  ESoundIOAbstract *mySound;
  EMemIO *myIO;
  BYTE io_type;
  BYTE io_variation;  // sub-type, usually 0
  unsigned int io_keypress;
  unsigned int timerSpeed;
  unsigned int timerFactor;
  DVfs *vfs;
  unsigned int resetAddr,resetStack;  // These are suggestions only...
  bool debug;
  bool hasSound;
};


#endif
