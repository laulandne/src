#ifndef CPULIB_EM6509_H
#define CPULIB_EM6509_H


/*
// The cpu used in the commodore b-seies computers
class Em6509: public Em6502
{
public:
  // Public member functions
  Em6509();
  virtual ~Em6509();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void reset();
  virtual void wregs();
  virtual void setMachineSpecificRegister(unsigned int n,unsigned int val);
  virtual unsigned int readMachineSpecificRegister(unsigned int n);
  virtual void executeInstead(ULONG opcode);
  virtual void execute();
  virtual void handle_int(ULONG value);
  virtual ULONG disasm(ULONG loc);
  // Public data members
protected:
  // Private member functions
  virtual void handle_adc(unsigned int aa);
  virtual void handle_sbc(unsigned int aa);
  virtual UINT16 handle_adc_d(unsigned int aa, unsigned int bb);
  virtual UINT16 handle_sbc_d(unsigned int aa, unsigned int bb);
  virtual bool execMain();
  // NOTE: We've got our own stack handing funcs because of the wider address space
  virtual void stack_push(BYTE val);
  virtual BYTE stack_pop();
  virtual unsigned int readv(ULONG addr,unsigned int size);
  virtual void writev(ULONG addr,unsigned int val,unsigned int size);
  void adj_op_len(bool n);
  // Private data members
  UINT16 dp;   // Not actually used on this chip
  unsigned int dbr;  // Not actually used on this chip
  unsigned int pbr;  // program offset (in bytes)
  unsigned int indexedOffset;  // used only for lda and sta indexed
  bool m16Bit,x16Bit;  // Not actually used on this chip
  unsigned int mSizeMask,xSizeMask;  // Not actually used on this chip
  unsigned int mHighBit,xHighBit;  // Not actually used on this chip
  unsigned int mHalfBit,xHalfBit;  // Not actually used on this chip
  unsigned int dpMask;
};
*/


#endif
