#ifndef CPULIB_ESPARC_H
#define CPULIB_ESPARC_H


#include <cpulib/ECPURisc.h>


// TODO: Fix the way windows size is calculated...
#define NUM_WINDOWS 16
#define NUM_REG (NUM_WINDOWS*16)


#define ESparc_STAT_N 1
#define ESparc_STAT_Z 2
#define ESparc_STAT_V 4
#define ESparc_STAT_C 8
#define ESparc_STAT_ALL 0xff


struct ESparcCondFlags  {
  bool C;
  bool N;
  bool V;
  bool Z;
};


class ESparc : public ECPURisc {
public:
  // Public member functions
  ESparc();
  virtual ~ESparc();
  static bool recognize(const char *name);
  virtual void execute();
  virtual void handle_int(ULONG value);
  virtual void cause_int(ULONG value);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual bool setRegister(const char *name,ULONG value);
  virtual void setReturnReg(ULONG val);
  virtual ULONG readReturnReg();
  virtual void setParamReg(unsigned int num,ULONG val);
  virtual ULONG readParamReg(unsigned int num);
  virtual ULONG readStack();
  virtual void setStack(ULONG val);
  virtual ULONG readFramePtr();
  virtual void setFramePtr(ULONG val);
  virtual ULONG disasm(ULONG loc);
  virtual void reset();
  virtual void wregs();
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
protected:
  // Private member functions
  virtual void init();
  void printS2();
  ULONG getS2();
  //
  void execOp0();
  void execOp1();
  void execOp2();
  void execOp3();
  void execBICC(bool p);
  void execSETHI();
  void execCALL();
  void execADD();
  void execSUB();
  void execSUBCC();
  void execANDCC();
  void execAND();
  void execORCC();
  void execOR();
  void execXOR();
  void execMULX();
  void execTICC();
  void execSAVE();
  void execJMPL();
  void execRESTORE();
  void execRETURN();
  void execLD();
  void execLDX();
  void execLDDF();
  void execLDUB();
  void execLDUH();
  void execLDSB();
  void execLDSH();
  void execLDSW();
  void execST();
  void execSTX();
  void execSTDF();
  void execSTB();
  void execSLL();
  void execSRL();
  void execSRA();
  void disOp0();
  void disOp1();
  void disOp2();
  void disOp3();
  void disBICC(bool p);
  void disSETHI();
  void disCALL();
  void disADD();
  void disSUB();
  void disSUBCC();
  void disAND();
  void disANDCC();
  void disORCC();
  void disOR();
  void disXOR();
  void disMULX();
  void disTICC();
  void disSAVE();
  void disJMPL();
  void disRESTORE();
  void disRETURN();
  void disLD();
  void disLDX();
  void disLDDF();
  void disLDUB();
  void disLDUH();
  void disLDSB();
  void disLDSH();
  void disLDSW();
  void disST();
  void disSTX();
  void disSTDF();
  void disSTB();
  void disSLL();
  void disSRL();
  void disSRA();
  //
  void printReg(unsigned int num);
  void printFReg(unsigned int num,unsigned int prec);
  ULONG readReg(unsigned int n);
  ULONG readFReg(unsigned int n, unsigned int prec);
  void writeReg(unsigned int n, ULONG val);
  void writeFReg(unsigned int n, ULONG val, unsigned int prec);
  void printRegWindow();
  void windowOverflow();
  void windowUnderflow();
  void incCWP();
  void decCWP();
  void wmregs();
  void wfregs();
  // Private data members
  ULONG G[8];
  ULONG R[NUM_REG];
  double F[32];
  bool dirty[NUM_WINDOWS];
  unsigned int CWP;
  unsigned int y;    // Used for multiply step
  unsigned int wim;  // Window invalid mask
  ULONG tbr;  // Trap base reg
  ESparcCondFlags CC;
  unsigned int rs1,rs2,rd,simm13,ibit,imm22,disp30;
};


#endif
