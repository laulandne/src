#ifndef CPULIB_EX86_H
#define CPULIB_EX86_H


#include <cpulib/ECPUCisc.h>
#include <cpulib/ECPUOperand.h>


// Instruction, operand and other internal formats
#define Ex86_NONE 0
#define Ex86_SPECIAL 1
#define Ex86_AX 2
#define Ex86_REG 3
#define Ex86_SEG 4
#define Ex86_INDREG 5
#define Ex86_DISP 6
#define Ex86_IMM 7
#define Ex86_BRANCH 8
#define Ex86_MODRM 9
#define Ex86_MODRM8 10
#define Ex86_MODREG 11
#define Ex86_MODREG8 12


// register/data sizes
// see elib/ECPUAbstract for why I choose these numbers...
#define Ex86_SIZE_BAD 0
// High 8 bits of low 16 bits
#define Ex86_SIZE_H   4
// Low 8 bits (same as ECPUABSTRACT_SIZE_BYTE)
#define Ex86_SIZE_L   1
#define Ex86_SIZE_BYTE   1
// Low 16 bits (same as ECPUABSTRACT_SIZE_SHORT)
#define Ex86_SIZE_SHORT   3
// All 32 bits (same as ECPUABSTRACT_SIZE_LONG)
#define Ex86_SIZE_LONG  2
// a 64-bit "quadword"
#define Ex86_SIZE_QUAD  5


// more useful masks (see ECPUAbstract.h for more)
#define Ex86_MASK_HIGHBYTE    0x0000ff00
#define Ex86_NOTMASK_HIGHBYTE 0xffff00ff


// flag bits
#define Ex86_FLAG_C   1
#define Ex86_FLAG_X1  (1<<1)
#define Ex86_FLAG_P   (1<<2)
#define Ex86_FLAG_X3  (1<<3)
#define Ex86_FLAG_A   (1<<4)
#define Ex86_FLAG_X5  (1<<5)
#define Ex86_FLAG_Z   (1<<6)
// NOTE: the S flag is renamed to N to match other cpu's
#define Ex86_FLAG_N   (1<<7)
#define Ex86_FLAG_T   (1<<8)
#define Ex86_FLAG_I   (1<<9)
#define Ex86_FLAG_D   (1<<10)
// NOTE: the O flag is renamed to V to match other cpu's
#define Ex86_FLAG_V   (1<<11)
#define Ex86_FLAG_IO0 (1<<12)
#define Ex86_FLAG_IO1 (1<<13)
#define Ex86_FLAG_NT  (1<<14)
#define Ex86_FLAG_X15 (1<<15)
#define Ex86_FLAG_R   (1<<16)
#define Ex86_FLAG_VM  (1<<17)
#define Ex86_FLAG_AC  (1<<18)
#define Ex86_FLAG_VIF (1<<19)
#define Ex86_FLAG_VIP (1<<20)
#define Ex86_FLAG_ID  (1<<21)
#define Ex86_FLAG_ALL 0xffffffff


// Values for overrode, which keeps track of instruction prefixes in effect
#define Ex86_PREFIX_ADDRSIZE 1
#define Ex86_PREFIX_DATASIZE 2
#define Ex86_PREFIX_SEG 4
// ...many others...


// Ex86_seg flags values (these are bits that can be combined)
#define Ex86_SEG_FLAG_INVALID 0
#define Ex86_SEG_FLAG_VALID   1
#define Ex86_SEG_FLAG_16BIT   0
#define Ex86_SEG_FLAG_32BIT   2


// register nums (8 bit)
#define Ex86_REGB_AL  0
#define Ex86_REGB_CL  1
#define Ex86_REGB_DL  2
#define Ex86_REGB_BL  3
#define Ex86_REGB_AH  4
#define Ex86_REGB_CH  5
#define Ex86_REGB_DH  6
#define Ex86_REGB_BH  7


// register nums (16 bit)
#define Ex86_REGW_AX  0
#define Ex86_REGW_CX  1
#define Ex86_REGW_DX  2
#define Ex86_REGW_BX  3
#define Ex86_REGW_SP  4
#define Ex86_REGW_BP  5
#define Ex86_REGW_SI  6
#define Ex86_REGW_DI  7


// register nums (32 bit)
#define Ex86_REGD_EAX  0
#define Ex86_REGD_ECX  1
#define Ex86_REGD_EDX  2
#define Ex86_REGD_EBX  3
#define Ex86_REGD_ESP  4
#define Ex86_REGD_EBP  5
#define Ex86_REGD_ESI  6
#define Ex86_REGD_EDI  7


// other register nums
#define Ex86_REG_EIP   8


// segment nums
#define Ex86_SEG_ES 0
#define Ex86_SEG_CS 1
#define Ex86_SEG_SS 2
#define Ex86_SEG_DS 3
#define Ex86_SEG_FS 4
#define Ex86_SEG_GS 5


// mod/rm modes (32-bit ones)
#define Ex86_MODRM_INDREG        0
#define Ex86_MODRM_INDREG_DISP8  1
#define Ex86_MODRM_INDREG_DISP   2
#define Ex86_MODRM_REGREG        3


class Ex86_seg {
public:
  Ex86_seg();
  void reset();
  void print();
  unsigned int getVal() { return val; }
  unsigned int getBase() { return base; }
  unsigned int getLimit() { return limit; }
  unsigned int getFlags() { return flags; }
  unsigned int getSize();
  void setBase(unsigned int n);
  void setVal(unsigned int n);
protected:
  unsigned int val;
  unsigned int base;
  unsigned int limit;
  unsigned int flags;
};


class Ex86 : public ECPUCisc {
public:
  // Public member functions
  Ex86();
  virtual ~Ex86();
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
  virtual void setFlagsNZ(ULONG val,unsigned int size);
  virtual void setFlagsNZvc(ULONG val,unsigned int size);
  virtual void setFlagsVC(ULONG val1,ULONG val2, ULONG val3,unsigned int size);
  virtual void setCFlag(ULONG val);
  virtual void setVFlag(ULONG val);
  virtual void setZFlag(ULONG val);
  virtual void setNFlag(ULONG val);
  virtual void setBitness(unsigned int val);
protected:
  // Private member functions
  void init();
  unsigned int getMaskSizeX86(unsigned int size);
  unsigned int getTopBitSizeX86(unsigned int size);
  void printSeg(unsigned int num);
  void printReg(unsigned int num,unsigned int size=Ex86_SIZE_LONG);
  char *getRegString(unsigned int num,unsigned int size=Ex86_SIZE_LONG);
  ULONG readReg(unsigned int n,unsigned int size=Ex86_SIZE_LONG);
  void writeReg(unsigned int n,ULONG val,unsigned int size=Ex86_SIZE_LONG);
  ULONG readMem(ULONG ea,unsigned int size=Ex86_SIZE_LONG);
  void writeMem(ULONG ea,ULONG val,unsigned int size=Ex86_SIZE_LONG);
  unsigned int getSegPart(unsigned int vaddr);
  unsigned int getOffPart(unsigned int vaddr);
  void load_seg(Ex86_seg& seg,ULONG addr);
  unsigned int calcSegVal(Ex86_seg& seg);
  unsigned int calcFlat(Ex86_seg& seg,ULONG addr);
  void go_real();
  void go_protected();
  void go_v86();
  void wmregs();
  void wfregs();
  void decode();
  void decodeOperand(unsigned int format,ECPUOperand *op);
  void decodeModrm(ECPUOperand *op);
  void decodeModrm16(ECPUOperand *op);
  void decodeModreg(ECPUOperand *op);
  void decodeSpecial();
  char *getSpecialOpName(unsigned int op,unsigned int reg);
  void execSpecialOp(unsigned int loc,unsigned int op,unsigned int reg);
  char *getModeName(unsigned int format);
  void disOperand(ECPUOperand *op);
  ULONG getOpDisp(ECPUOperand *op);
  ULONG getOpImm(ECPUOperand *op);
  ULONG readOperand(ECPUOperand *op);
  void writeOperand(ECPUOperand *op,ULONG val);
  unsigned int getImm(ULONG addr,unsigned int size);
  void push(unsigned int val,unsigned int reg,unsigned int size);
  unsigned int pull(unsigned int reg,unsigned int size);
  void setFlagBits(unsigned int mask);
  void resetFlagBits(unsigned int mask);
  unsigned int getFlagBits(unsigned int mask);
  void setPFlag(ULONG val);
  void setAFlag(ULONG val);
  void setFlagsP(ULONG val,unsigned int size);
  //
  void dis_special(ULONG loc,unsigned int opcode);
  void exec_special(ULONG loc,unsigned int opcode);
  void exec_plusseg(ULONG loc,unsigned int opcode);
  void exec_es(ULONG loc,unsigned int opcode);
  void exec_cs(ULONG loc,unsigned int opcode);
  void exec_ss(ULONG loc,unsigned int opcode);
  void exec_ds(ULONG loc,unsigned int opcode);
  void exec_fs(ULONG loc,unsigned int opcode);
  void exec_gs(ULONG loc,unsigned int opcode);
  void exec_op_size(ULONG loc,unsigned int opcode);
  void exec_addr_size(ULONG loc,unsigned int opcode);
  void exec_lar(ULONG loc,unsigned int opcode);
  void exec_lsl(ULONG loc,unsigned int opcode);
  void exec_clts(ULONG loc,unsigned int opcode);
  void exec_invd(ULONG loc,unsigned int opcode);
  void exec_wbinvd(ULONG loc,unsigned int opcode);
  void exec_setc(ULONG loc,unsigned int opcode);
  void exec_setnc(ULONG loc,unsigned int opcode);
  void exec_setz(ULONG loc,unsigned int opcode);
  void exec_setnz(ULONG loc,unsigned int opcode);
  void exec_setbe(ULONG loc,unsigned int opcode);
  void exec_setnbe(ULONG loc,unsigned int opcode);
  void exec_sets(ULONG loc,unsigned int opcode);
  void exec_setns(ULONG loc,unsigned int opcode);
  void exec_setpe(ULONG loc,unsigned int opcode);
  void exec_setpo(ULONG loc,unsigned int opcode);
  void exec_setnge(ULONG loc,unsigned int opcode);
  void exec_setge(ULONG loc,unsigned int opcode);
  void exec_setng(ULONG loc,unsigned int opcode);
  void exec_setg(ULONG loc,unsigned int opcode);
  void exec_bt(ULONG loc,unsigned int opcode);
  void exec_bts(ULONG loc,unsigned int opcode);
  void exec_btr(ULONG loc,unsigned int opcode);
  void exec_btc(ULONG loc,unsigned int opcode);
  void exec_bsf(ULONG loc,unsigned int opcode);
  void exec_bsr(ULONG loc,unsigned int opcode);
  void exec_shld(ULONG loc,unsigned int opcode);
  void exec_shrd(ULONG loc,unsigned int opcode);
  void exec_cmpxchg(ULONG loc,unsigned int opcode);
  void exec_lss(ULONG loc,unsigned int opcode);
  void exec_lfs(ULONG loc,unsigned int opcode);
  void exec_lgs(ULONG loc,unsigned int opcode);
  void exec_movzx(ULONG loc,unsigned int opcode);
  void exec_movsx(ULONG loc,unsigned int opcode);
  void exec_xadd(ULONG loc,unsigned int opcode);
  void exec_bswap(ULONG loc,unsigned int opcode);
  void exec_add(ULONG loc,unsigned int opcode);
  void exec_and(ULONG loc,unsigned int opcode);
  void exec_push(ULONG loc,unsigned int opcode);
  void exec_pop(ULONG loc,unsigned int opcode);
  void exec_or(ULONG loc,unsigned int opcode);
  void exec_adc(ULONG loc,unsigned int opcode);
  void exec_sbb(ULONG loc,unsigned int opcode);
  void exec_daa(ULONG loc,unsigned int opcode);
  void exec_sub(ULONG loc,unsigned int opcode);
  void exec_das(ULONG loc,unsigned int opcode);
  void exec_xor(ULONG loc,unsigned int opcode);
  void exec_aaa(ULONG loc,unsigned int opcode);
  void exec_cmp(ULONG loc,unsigned int opcode);
  void exec_aas(ULONG loc,unsigned int opcode);
  void exec_inc(ULONG loc,unsigned int opcode);
  void exec_dec(ULONG loc,unsigned int opcode);
  void exec_pusha(ULONG loc,unsigned int opcode);
  void exec_popa(ULONG loc,unsigned int opcode);
  void exec_bound(ULONG loc,unsigned int opcode);
  void exec_arpl(ULONG loc,unsigned int opcode);
  void exec_imul(ULONG loc,unsigned int opcode);
  void exec_insb(ULONG loc,unsigned int opcode);
  void exec_insw(ULONG loc,unsigned int opcode);
  void exec_outsb(ULONG loc,unsigned int opcode);
  void exec_outsw(ULONG loc,unsigned int opcode);
  void exec_jo(ULONG loc,unsigned int opcode);
  void exec_jno(ULONG loc,unsigned int opcode);
  void exec_jc(ULONG loc,unsigned int opcode);
  void exec_jnc(ULONG loc,unsigned int opcode);
  void exec_jz(ULONG loc,unsigned int opcode);
  void exec_jnz(ULONG loc,unsigned int opcode);
  void exec_jbe(ULONG loc,unsigned int opcode);
  void exec_jnbe(ULONG loc,unsigned int opcode);
  void exec_js(ULONG loc,unsigned int opcode);
  void exec_jns(ULONG loc,unsigned int opcode);
  void exec_jpe(ULONG loc,unsigned int opcode);
  void exec_jpo(ULONG loc,unsigned int opcode);
  void exec_jng(ULONG loc,unsigned int opcode);
  void exec_jge(ULONG loc,unsigned int opcode);
  void exec_jnge(ULONG loc,unsigned int opcode);
  void exec_jg(ULONG loc,unsigned int opcode);
  void exec_test(ULONG loc,unsigned int opcode);
  void exec_xchg(ULONG loc,unsigned int opcode);
  void exec_mov(ULONG loc,unsigned int opcode);
  void exec_lea(ULONG loc,unsigned int opcode);
  void exec_nop(ULONG loc,unsigned int opcode);
  void exec_cbw(ULONG loc,unsigned int opcode);
  void exec_cwd(ULONG loc,unsigned int opcode);
  void exec_call(ULONG loc,unsigned int opcode);
  void exec_wait(ULONG loc,unsigned int opcode);
  void exec_pushf(ULONG loc,unsigned int opcode);
  void exec_popf(ULONG loc,unsigned int opcode);
  void exec_lahf(ULONG loc,unsigned int opcode);
  void exec_sahf(ULONG loc,unsigned int opcode);
  void exec_movsb(ULONG loc,unsigned int opcode);
  void exec_movsw(ULONG loc,unsigned int opcode);
  void exec_cmpsb(ULONG loc,unsigned int opcode);
  void exec_cmpsw(ULONG loc,unsigned int opcode);
  void exec_stosb(ULONG loc,unsigned int opcode);
  void exec_stosw(ULONG loc,unsigned int opcode);
  void exec_lodsb(ULONG loc,unsigned int opcode);
  void exec_lodsw(ULONG loc,unsigned int opcode);
  void exec_scasb(ULONG loc,unsigned int opcode);
  void exec_scasw(ULONG loc,unsigned int opcode);
  void exec_ret(ULONG loc,unsigned int opcode);
  void exec_les(ULONG loc,unsigned int opcode);
  void exec_lds(ULONG loc,unsigned int opcode);
  void exec_enter(ULONG loc,unsigned int opcode);
  void exec_leave(ULONG loc,unsigned int opcode);
  void exec_in(ULONG loc,unsigned int opcode);
  void exec_int(ULONG loc,unsigned int opcode);
  void exec_into(ULONG loc,unsigned int opcode);
  void exec_iret(ULONG loc,unsigned int opcode);
  void exec_xlat(ULONG loc,unsigned int opcode);
  void exec_loopnz(ULONG loc,unsigned int opcode);
  void exec_loopz(ULONG loc,unsigned int opcode);
  void exec_loop(ULONG loc,unsigned int opcode);
  void exec_jcxz(ULONG loc,unsigned int opcode);
  void exec_out(ULONG loc,unsigned int opcode);
  void exec_jmp(ULONG loc,unsigned int opcode);
  void exec_lock(ULONG loc,unsigned int opcode);
  void exec_repnz(ULONG loc,unsigned int opcode);
  void exec_repz(ULONG loc,unsigned int opcode);
  void exec_hlt(ULONG loc,unsigned int opcode);
  void exec_cmc(ULONG loc,unsigned int opcode);
  void exec_clc(ULONG loc,unsigned int opcode);
  void exec_stc(ULONG loc,unsigned int opcode);
  void exec_cli(ULONG loc,unsigned int opcode);
  void exec_sti(ULONG loc,unsigned int opcode);
  void exec_cld(ULONG loc,unsigned int opcode);
  void exec_std(ULONG loc,unsigned int opcode);
  // Private data members
  ULONG eax,ebx,ecx,edx;  // i.e. regs 0-3
  ULONG esi,edi;  // i.e. regs 4 and 5
  ULONG ebp;  // i.e. reg 7 (esp is 6)
  unsigned int eflags;  // i.e. status
  Ex86_seg cs,ds,es,ss,fs,gs;
  // these are used as the "current seg we're using"
  Ex86_seg *curCodeSeg;
  Ex86_seg *curDataSeg;
  unsigned int curAddrSize,curDataSize;  // Ex86_SIZE_WORD vs Ex86_SIZE_LONG, etc, can be overridden
  unsigned int curNumBytes;  // 2 or 4 or maybe 8
  unsigned int cpl;
  double fp[8];
  UINT16 fpsw,fpcw,fptw;
  ULONG cr[5];
  ULONG dr[8];
  ULONG gdtr,idtr,ldtr,tr;
  bool protMode;
  bool v86;
  const char *opName;
  unsigned int opLen,opFormat,opSubFormat,opFlags;
  unsigned int topcode;
  bool sibPresent,modrmPresent;
  ULONG sib,modrm,imm,disp;
  unsigned int preBytes,immBytes,dispBytes;
  // op2 is typically the dest
  ECPUOperand op1,op2;
};


#endif
