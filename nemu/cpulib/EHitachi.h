#ifndef CPULIB_EHITACHI_H
#define CPULIB_EHITACHI_H


#include <cpulib/ECPU8Bit.h>


// Address modes
#define EHITACHI_NONE 0
#define EHITACHI_SPECIAL 1
#define EHITACHI_ILLEGAL 2


// Status bits
#define EHITACHI_STAT_C  1
#define EHITACHI_STAT_V  2
#define EHITACHI_STAT_Z  4
#define EHITACHI_STAT_N  8
#define EHITACHI_STAT_U1  16
#define EHITACHI_STAT_H  32
#define EHITACHI_STAT_U2  64
#define EHITACHI_STAT_I  128
#define EHITACHI_STAT_ALL 0xff


#ifndef A_BIG_ENDIAN
struct EHitachiStatusBits  {
  volatile unsigned int c:1;
  volatile unsigned int v:1;
  volatile unsigned int z:1;
  volatile unsigned int n:1;
  volatile unsigned int u1:1;
  volatile unsigned int h:1;
  volatile unsigned int u2:1;
  volatile unsigned int i:1;
};
#else
struct EHitachiStatusBits  {
  volatile unsigned int i:1;
  volatile unsigned int u2:1;
  volatile unsigned int h:1;
  volatile unsigned int u1:1;
  volatile unsigned int n:1;
  volatile unsigned int z:1;
  volatile unsigned int v:1;
  volatile unsigned int c:1;
};
#endif


class EHitachi : public ECPU8Bit
{
public:
  // Public member functions
  EHitachi();
  virtual ~EHitachi();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void execute();
  virtual void handle_int(ULONG value);
  virtual void cause_int(ULONG value);
  virtual void ints();
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual ULONG readStack();
  virtual void setStack(ULONG location);
  virtual ULONG disasm(ULONG loc);
  virtual unsigned int printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode);
  virtual void reset();
  virtual void wregs();
  // Public data members
protected:
  // Private member functions
  virtual void init();
  // Private data members
  unsigned int R[8];
  volatile BYTE ccr; // status bits
  volatile EHitachiStatusBits *sbits;
};


#endif // CPULIB_HITACHI_H
