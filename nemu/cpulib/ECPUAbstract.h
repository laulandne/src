#ifndef ELIB_ECPUABSTRACT_H
#define ELIB_ECPUABSTRACT_H




#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/AError.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemAbstract.h>


class EMemMapAbstract;
class EMapSymbol;
class EHandler;


// Useful size encoding...
// NOTE: these are weird because this is what the 68k uses...mostly...
// DO NOT change these since the e68k code relies on this
#define ECPUABSTRACT_SIZE_BYTE 1   /* Low 8 bits */
#define ECPUABSTRACT_SIZE_WORD 3   /* Low 16 bits */
#define ECPUABSTRACT_SIZE_LONG 2   /* Low 32 bits */
#define ECPUABSTRACT_SIZE_OTHER 0  /* Illegal size */


// useful masks
#define ECPUABSTRACT_MASK_BYTE    0x000000ff
#define ECPUABSTRACT_MASK_WORD    0x0000ffff
#define ECPUABSTRACT_MASK_LONG    0xffffffff
#define ECPUABSTRACT_NOTMASK_BYTE 0xffffff00
#define ECPUABSTRACT_NOTMASK_WORD 0xffff0000
#define ECPUABSTRACT_NOTMASK_LONG 0x00000000


// useful for dealing with the sign
#define ECPUABSTRACT_TOP_BYTE     0x00000080
#define ECPUABSTRACT_TOP_WORD     0x00008000
#define ECPUABSTRACT_TOP_LONG     0x80000000
#define ECPUABSTRACT_NOTTOP_BYTE  0x0000007f
#define ECPUABSTRACT_NOTTOP_WORD  0x00007fff
#define ECPUABSTRACT_NOTTOP_LONG  0x7fffffff


// Standard addressing modes...cpus can and will define their own unique ones
#define ECPUABSTRACT_ADDR_NONE 0
#define ECPU_ADDR_NONE 0


// Standard instruction flags...cpus can and will define their own unique ones
#define ECPUABSTRACT_FLAG_NONE 0


// Arbitrary values...
#define ECPUABSTRACT_DEFAULT_CODE  0x00000000
#define ECPUABSTRACT_DEFAULT_STACK 0xfb000000


// Types (Taken from Java, but can be used for types in general)
// Feel free to add stuff, but don't change anything under 16
#define ECPUABSTRACT_TYPE_UTF8 1
#define ECPUABSTRACT_TYPE_INT 3
#define ECPUABSTRACT_TYPE_FLOAT 4
#define ECPUABSTRACT_TYPE_LONG 5
#define ECPUABSTRACT_TYPE_DOUBLE 6
#define ECPUABSTRACT_TYPE_CLASS 7
#define ECPUABSTRACT_TYPE_STRING 8
#define ECPUABSTRACT_TYPE_FIELD 9
#define ECPUABSTRACT_TYPE_METHOD 10
#define ECPUABSTRACT_TYPE_INTERFACE 11
#define ECPUABSTRACT_TYPE_NAMETYPE 12
#define ECPUABSTRACT_TYPE_ARRAY 1000


// These macros are so we don't have to think about the endian mode we're in...
#define read64mem(x) map->read64(x,lsb)
#define write64mem(x,y) map->write64(x,y,lsb)
#define read32mem(x) map->read32(x,lsb)
#define write32mem(x,y) map->write32(x,y,lsb)
#define read16mem(x) map->read16(x,lsb)
#define write16mem(x,y) map->write16(x,y,lsb)
#define read8mem(x) map->read8(x)
#define write8mem(x,y) map->write8(x,y)


struct stackTraceThing {
  ULONG st;
  ULONG pc;
  ULONG addr;
};


class ECPUAbstract {
public:
  // Public member functions
  ECPUAbstract();
  virtual ~ECPUAbstract();
  virtual const char *getName();
  virtual ULONG getPC() { return 0; }
  virtual void setKind(const char *name);
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual void setReturnReg(ULONG val);
  virtual ULONG readReturnReg();
  virtual void setParamReg(unsigned int num,ULONG val);
  virtual ULONG readParamReg(unsigned int num);
  virtual ULONG readStack();
  virtual void setStack(ULONG location);
  virtual void setFramePtr(ULONG location);
  virtual ULONG readFramePtr();
  virtual void setMachineSpecificRegister(unsigned int n,unsigned int val);
  virtual unsigned int readMachineSpecificRegister(unsigned int n);
  //
  virtual void jump(ULONG location);
  virtual void jsr(ULONG location);
  virtual void rts();
  virtual void pushAddr(ULONG location);
  virtual ULONG pullAddr();
  //
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
  virtual void changeMap(EMemMapAbstract *mmap);
  virtual void setMap(EMemMapAbstract *m);
  virtual void execute();
  virtual void executeInstead(ULONG opcode);
  virtual ULONG disasm(ULONG loc);
  virtual ULONG disasmPC() { return disasm(0); }
  //
  void exec_unknown(ULONG loc,unsigned int op=0) { exec_unknown(NULL,loc,op); }
  virtual void exec_unknown(const char *s,unsigned int loc=0,unsigned int op=0);
  void exec_unimplemented(ULONG loc,unsigned int op=0) { exec_unimplemented(NULL,loc,op); }
  virtual void exec_unimplemented(const char *s,unsigned int loc=0,unsigned int op=0);
  void exec_illegal(ULONG loc=0,unsigned int op=0) { exec_illegal(NULL,loc,op); }
  virtual void exec_illegal(const char *s,unsigned int loc=0,unsigned int op=0);
  void dis_unimplemented(ULONG loc,unsigned int op=0) { dis_unimplemented(NULL,loc,op); }
  virtual void dis_unimplemented(const char *str,unsigned int loc=0,unsigned int op=0);
  void dis_illegal(unsigned int loc=0,unsigned int op=0) { dis_illegal(NULL,loc,op); }
  virtual void dis_illegal(const char *s=NULL,unsigned int loc=0,unsigned int op=0);
  virtual void exec_invalid(const char *s);
  virtual void dis_invalid(const char *s);
  //
  virtual void nmi();  // generic non-maskable int
  virtual void reset();
  virtual void wregs();
  // Request an int, it is either done now or put in pending
  virtual void cause_int(ULONG value);
  // Actually handle the event
  virtual void handle_int(ULONG value);
  virtual void ints();
  virtual void showStackTrace();
  virtual void setFlagsNZ(ULONG res,unsigned int size);
  virtual void setFlagsNZvc(ULONG res, unsigned int size);
  virtual void setFlagsVC(ULONG val1, ULONG val2, ULONG res, unsigned int size);
  virtual void setCFlag(ULONG val);
  virtual void setVFlag(ULONG val);
  virtual void setZFlag(ULONG val);
  virtual void setNFlag(ULONG val);
  virtual void setBitness(unsigned int val);
  virtual unsigned int getBitness() { return bitness; }
  virtual void my_yield();
  virtual ULONG my_fork();
  virtual ULONG my_exec(const char *name);  // TODO: no args here...
  //
  void setResetEntry(ULONG addr);
  void setResetStack(ULONG addr);
  unsigned long getCycles() const { return cycles; }
  void setLSB(bool t) { lsb=t; } // Its up to the cpu to use this or not
  void setTrace(bool t) { trace=t; }
  void setRunning(bool r) { running=r; }
  void setBreakpoint(ULONG addr) { breakpoint=addr; }
  void setIO(EHandler *i);
  EHandler *getIO() const { return io; }
  void setTrapHandler(EHandler *handler);
  EHandler *getTrapHandler() const { return trapHandler; }
  bool getTrace() const { return trace; }
  bool getRunning() const { return running; }
  ULONG getBreakpoint() const { return breakpoint; }
  void resetCycles() { cycles=0; }
  ULONG getCyclesPerSec() const { return cyclesPerSec; }
  void setIntLock(bool t) { intLock=t; }
  void setFPU(const char *n=(char *)NULL);
  void debugger(const char *str="Error\n");
  void startTracingStack();
  void endTracingStack();
  bool getDebug() { return debug; }
  void setDebug(bool t) { debug=t; }
  //
  AError err;
protected:
  // Private member functions
  virtual void init();
  void clearStackTrace();
  void stackTracePush(ULONG addr);
  void stackTracePop(ULONG addr);
  long sign_ext(ULONG num, ULONG numbits);
  ULONG unsign_ext(ULONG num, ULONG numbits);
  unsigned int popCount(BYTE q);
  unsigned int power2(unsigned int v);
  void printHex(ULONG val,unsigned int digits);
  //ULONG getMaskSize(ULONG size);
  //ULONG getTopBitSize(ULONG size);
  // Private data members
  ULONG type;
  ECPUAbstract *next;
  EMemMapAbstract *map;
  EHandler *io;
  EHandler *trapHandler;
  EMapSymbol *lastSymbol;
  ULONG breakpoint;
  ULONG int_pending;  // If non-zero, an int is pending
  ULONG op_len,op,val,opcode,cycle,ea;  // Feel free to use these or not as you see fit...
  BYTE topcode;  // Feel free to use these or not as you see fit...
  bool running;
  bool trace;
  bool doBreak;
  bool doTrace,doRun;
  bool intLock;
  ULONG cyclesPerSec;
  ULONG cycles;
  //
  bool hasResetEntry,hasResetStack;
  ULONG resetEntry,resetStack;
  //
  bool hasFPU;
  bool hasMMU;
  AList *stackTrace;
  ULONG lastStackPtr;
  bool traceStack;
  bool lsb;  // if true we're little-endian, if false then big
  unsigned int bitness;  // 8 for 8 bit, 16 for 16 bit, etc...
  bool debug;
  bool canReassemble;
};


#endif
