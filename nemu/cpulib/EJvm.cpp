
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <cpulib/EJvm.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <binarylib/NLd.h>
#include <binarylib/NJavaClassDecoder.h>


#include <cpulib/generated/EJvm_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_JVM      0x01


////////////////////////////////////////////////////////////////////////////////
//  EJvm Class
////////////////////////////////////////////////////////////////////////////////

EJvm::EJvm()
{
  DEBUG_OUT<<"EJvm created...\n";
  init();
  type=E_CPU_JVM;
  //w=ww;
  reset();
}


EJvm::~EJvm()
{
  DEBUG_OUT<<"EJvm destroyed.\n";
}


void EJvm::init()
{
  ECPUByteCode::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  lsb=false;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool EJvm::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"jvm")) return true; else
  if(!strcmp(name,"java")) return true; else
  return false;
}


const char *EJvm::getName()
{
  return "Java JVM";
}


void EJvm::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_JVM;
}


void EJvm::reset()
{
  EMapSymbol *s=NULL;
  if(map) s=map->getFirstModule()->lookupSymbolByNameAndType("main",NJAVA_SYM_CODE);
  if(s) {
    ECPUByteCodeObject o;
    // array of command line args...none for now
    o.type=ECPUABSTRACT_TYPE_ARRAY;
    o.len=0;
    o.val=0;
    o.name=0;
    pushObj(&o);
    resetEntry=s->addr;
  }
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffffffff;  // since we start at 0, breakpoint's default
  intLock=false;
  pc=0;  // Jump to zero
  clearStackTrace();
  ECPUAbstract::reset();
}


bool EJvm::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  if(!strcmp(name,"sp"))
  { sp=value; ret=true; }
  //free(name);
  return ret;
}


ULONG EJvm::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


void EJvm::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  DEBUG_OUT<<"SP="<<sp<<"\n";
}


void EJvm::execute()
{
  //if(!lastUsedAddress) loadInitialClass();
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=EJvm_size[opcode];
  cycle=EJvm_time[opcode];
  topcode=opcode;
  unsigned int loc=pc;
#include <cpulib/generated/EJvm_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG EJvm::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=EJVM_ADDR_NONE;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=EJvm_size[topcode];
  mode=EJvm_format[topcode];
  name=EJvm_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<(short)map->read8(loc+t)<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  len+=printAddrMode(loc,topcode,mode);
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"\n";
  return len;
}


unsigned int EJvm::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  switch(mode) {
    case EJVM_ADDR_NONE:
      break;
    case EJVM_ADDR_ONE:
      break;
    case EJVM_ADDR_TWO:
      break;
    case EJVM_ADDR_IMM8:
      DEBUG_OUT<<(short)map->read8(loc+1);
      break;
    case EJVM_ADDR_IMM16:
      DEBUG_OUT<<(short)map->read16b(loc+1);
      break;
    default:
      debugger("Bad addr mode!\n");
      break;
  }
  return 0;
}


void EJvm::exec_nop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented nop opcode");
}


void EJvm::exec_aconst_null(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented aconst_null opcode");
}


void EJvm::exec_iconst_m1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented iconst_m1 opcode");
}


void EJvm::exec_iconst_0(ULONG loc,unsigned int opcode)
{
  iconst_helper(loc,opcode,0);
}


void EJvm::exec_iconst_1(ULONG loc,unsigned int opcode)
{
  iconst_helper(loc,opcode,1);
}


void EJvm::exec_iconst_2(ULONG loc,unsigned int opcode)
{
  iconst_helper(loc,opcode,2);
}


void EJvm::exec_iconst_3(ULONG loc,unsigned int opcode)
{
  iconst_helper(loc,opcode,3);
}


void EJvm::exec_iconst_4(ULONG loc,unsigned int opcode)
{
  iconst_helper(loc,opcode,4);
}


void EJvm::exec_iconst_5(ULONG loc,unsigned int opcode)
{
  iconst_helper(loc,opcode,5);
}


void EJvm::exec_lconst_0(ULONG loc,unsigned int opcode)
{
  lconst_helper(loc,opcode,0);
}


void EJvm::exec_lconst_1(ULONG loc,unsigned int opcode)
{
  lconst_helper(loc,opcode,1);
}


void EJvm::exec_fconst_0(ULONG loc,unsigned int opcode)
{
  fconst_helper(loc,opcode,0);
}


void EJvm::exec_fconst_1(ULONG loc,unsigned int opcode)
{
  fconst_helper(loc,opcode,1);
}


void EJvm::exec_fconst_2(ULONG loc,unsigned int opcode)
{
  fconst_helper(loc,opcode,2);
}


void EJvm::exec_dconst_0(ULONG loc,unsigned int opcode)
{
  dconst_helper(loc,opcode,0);
}


void EJvm::exec_dconst_1(ULONG loc,unsigned int opcode)
{
  dconst_helper(loc,opcode,1);
}


void EJvm::exec_bipush(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bipush opcode");
}


void EJvm::exec_sipush(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sipush opcode");
}


void EJvm::exec_ldc(ULONG loc,unsigned int opcode)
{
  unsigned int num=0;
  num=map->read8(loc+1);
  EMapSymbol *sym=map->getFirstModule()->lookupStringByNum(num);
  if(!sym) {
    debugger("ldc didn't find needed constant!\n");
    return;
  }
  DEBUG_OUT<<"ldc num="<<num<<" type="<<sym->type<<"\n";
  if(sym->type==ECPUABSTRACT_TYPE_STRING) {
    ECPUByteCodeObject o;
    o.type=0;
    o.val=sym->val;
    o.name=0;
    o.len=0;
    pushObj(&o);
    DEBUG_OUT<<"ldc["<<o.val<<"] ";
    s[0]=0;
    getNameString(s,o.val); // value?
    DEBUG_OUT<<s<<"\n";
  }
  else {
    debugger("Bad ldc constant type!");
  }
}


void EJvm::exec_ldc_w(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc_w opcode");
}


void EJvm::exec_ldc2_w(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc2_w opcode");
}


void EJvm::exec_iload(ULONG loc,unsigned int opcode)
{
  unsigned int num=0;
  // TODO: get num, 8 or 16 bits?
  iload_helper(loc,opcode,num);
}


void EJvm::exec_lload(ULONG loc,unsigned int opcode)
{
  unsigned int num=0;
  // TODO: get num, 8 or 16 bits?
  lload_helper(loc,opcode,num);
}


void EJvm::exec_fload(ULONG loc,unsigned int opcode)
{
  unsigned int num=0;
  // TODO: get num, 8 or 16 bits?
  fload_helper(loc,opcode,num);
}


void EJvm::exec_dload(ULONG loc,unsigned int opcode)
{
  unsigned int num=0;
  // TODO: get num, 8 or 16 bits?
  dload_helper(loc,opcode,num);
}


void EJvm::exec_aload(ULONG loc,unsigned int opcode)
{
  unsigned int num=0;
  // TODO: get num
  aload_helper(loc,opcode,num);
}


void EJvm::exec_iload_0(ULONG loc,unsigned int opcode)
{
  iload_helper(loc,opcode,0);
}


void EJvm::exec_iload_1(ULONG loc,unsigned int opcode)
{
  iload_helper(loc,opcode,1);
}


void EJvm::exec_iload_2(ULONG loc,unsigned int opcode)
{
  iload_helper(loc,opcode,2);
}


void EJvm::exec_iload_3(ULONG loc,unsigned int opcode)
{
  iload_helper(loc,opcode,3);
}


void EJvm::exec_lload_0(ULONG loc,unsigned int opcode)
{
  lload_helper(loc,opcode,0);
}


void EJvm::exec_lload_1(ULONG loc,unsigned int opcode)
{
  lload_helper(loc,opcode,1);
}


void EJvm::exec_lload_2(ULONG loc,unsigned int opcode)
{
  lload_helper(loc,opcode,2);
}


void EJvm::exec_lload_3(ULONG loc,unsigned int opcode)
{
  lload_helper(loc,opcode,3);
}


void EJvm::exec_fload_0(ULONG loc,unsigned int opcode)
{
  fload_helper(loc,opcode,0);
}


void EJvm::exec_fload_1(ULONG loc,unsigned int opcode)
{
  fload_helper(loc,opcode,1);
}


void EJvm::exec_fload_2(ULONG loc,unsigned int opcode)
{
  fload_helper(loc,opcode,2);
}


void EJvm::exec_fload_3(ULONG loc,unsigned int opcode)
{
  fload_helper(loc,opcode,3);
}


void EJvm::exec_dload_0(ULONG loc,unsigned int opcode)
{
  dload_helper(loc,opcode,0);
}


void EJvm::exec_dload_1(ULONG loc,unsigned int opcode)
{
  dload_helper(loc,opcode,1);
}


void EJvm::exec_dload_2(ULONG loc,unsigned int opcode)
{
  dload_helper(loc,opcode,2);
}


void EJvm::exec_dload_3(ULONG loc,unsigned int opcode)
{
  dload_helper(loc,opcode,3);
}


void EJvm::exec_aload_0(ULONG loc,unsigned int opcode)
{
  aload_helper(loc,opcode,0);
}


void EJvm::exec_aload_1(ULONG loc,unsigned int opcode)
{
  aload_helper(loc,opcode,1);
}


void EJvm::exec_aload_2(ULONG loc,unsigned int opcode)
{
  aload_helper(loc,opcode,2);
}


void EJvm::exec_aload_3(ULONG loc,unsigned int opcode)
{
  aload_helper(loc,opcode,3);
}


void EJvm::exec_iaload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented iaload opcode");
}


void EJvm::exec_laload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented laload opcode");
}


void EJvm::exec_faload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented faload opcode");
}


void EJvm::exec_daload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented daload opcode");
}


void EJvm::exec_aaload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented aaload opcode");
}


void EJvm::exec_baload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented baload opcode");
}


void EJvm::exec_caload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented caload opcode");
}


void EJvm::exec_saload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented saload opcode");
}


void EJvm::exec_istore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented istore opcode");
}


void EJvm::exec_lstore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lstore opcode");
}


void EJvm::exec_fstore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fstore opcode");
}


void EJvm::exec_dstore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dstore opcode");
}


void EJvm::exec_astore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astore opcode");
}


void EJvm::exec_istore_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented istore_0 opcode");
}


void EJvm::exec_istore_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented istore_1 opcode");
}


void EJvm::exec_istore_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented istore_2 opcode");
}


void EJvm::exec_istore_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented istore_3 opcode");
}


void EJvm::exec_lstore_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lstore_0 opcode");
}


void EJvm::exec_lstore_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lstore_1 opcode");
}


void EJvm::exec_lstore_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lstore_2 opcode");
}


void EJvm::exec_lstore_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lstore_3 opcode");
}


void EJvm::exec_fstore_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fstore_0 opcode");
}


void EJvm::exec_fstore_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fstore_1 opcode");
}


void EJvm::exec_fstore_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fstore_2 opcode");
}


void EJvm::exec_fstore_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fstore_3 opcode");
}


void EJvm::exec_dstore_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dstore_0 opcode");
}


void EJvm::exec_dstore_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dstore_1 opcode");
}


void EJvm::exec_dstore_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dstore_2 opcode");
}


void EJvm::exec_dstore_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dstore_3 opcode");
}


void EJvm::exec_astore_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astore_0 opcode");
}


void EJvm::exec_astore_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astore_1 opcode");
}


void EJvm::exec_astore_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astore_2 opcode");
}


void EJvm::exec_astore_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astore_3 opcode");
}


void EJvm::exec_iastore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented iastore opcode");
}


void EJvm::exec_lastore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lastore opcode");
}


void EJvm::exec_fastore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fastore opcode");
}


void EJvm::exec_dastore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dastore opcode");
}


void EJvm::exec_aastore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented aastore opcode");
}


void EJvm::exec_bastore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bastore opcode");
}


void EJvm::exec_castore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented castore opcode");
}


void EJvm::exec_sastore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sastore opcode");
}


void EJvm::exec_pop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pop opcode");
}


void EJvm::exec_pop2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pop2 opcode");
}


void EJvm::exec_dup(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup opcode");
}


void EJvm::exec_dup_x1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup_x1 opcode");
}


void EJvm::exec_dup_x2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup_x2 opcode");
}


void EJvm::exec_dup2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup2 opcode");
}


void EJvm::exec_dup2_x1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup2_x1 opcode");
}


void EJvm::exec_dup2_x2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup2_x2 opcode");
}


void EJvm::exec_swap(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented swap opcode");
}


void EJvm::exec_iadd(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented iadd opcode");
}


void EJvm::exec_ladd(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ladd opcode");
}


void EJvm::exec_fadd(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fadd opcode");
}


void EJvm::exec_dadd(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dadd opcode");
}


void EJvm::exec_isub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented isub opcode");
}


void EJvm::exec_lsub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lsub opcode");
}


void EJvm::exec_fsub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fsub opcode");
}


void EJvm::exec_dsub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dsub opcode");
}


void EJvm::exec_imul(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented imul opcode");
}


void EJvm::exec_lmul(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lmul opcode");
}


void EJvm::exec_fmul(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fmul opcode");
}


void EJvm::exec_dmul(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dmul opcode");
}


void EJvm::exec_idiv(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented idiv opcode");
}


void EJvm::exec_ldiv(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldiv opcode");
}


void EJvm::exec_fdiv(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fdiv opcode");
}


void EJvm::exec_ddiv(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ddiv opcode");
}


void EJvm::exec_irem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented irem opcode");
}


void EJvm::exec_lrem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lrem opcode");
}


void EJvm::exec_frem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented frem opcode");
}


void EJvm::exec_drem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented drem opcode");
}


void EJvm::exec_ineg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ineg opcode");
}


void EJvm::exec_lneg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lneg opcode");
}


void EJvm::exec_fneg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fneg opcode");
}


void EJvm::exec_dneg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dneg opcode");
}


void EJvm::exec_ishl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ishl opcode");
}


void EJvm::exec_lshl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lshl opcode");
}


void EJvm::exec_ishr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ishr opcode");
}


void EJvm::exec_lshr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lshr opcode");
}


void EJvm::exec_iushr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented iushr opcode");
}


void EJvm::exec_lushr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lushr opcode");
}


void EJvm::exec_iand(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented iand opcode");
}


void EJvm::exec_land(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented land opcode");
}


void EJvm::exec_ior(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ior opcode");
}


void EJvm::exec_lor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lor opcode");
}


void EJvm::exec_ixor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ixor opcode");
}


void EJvm::exec_lxor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lxor opcode");
}


void EJvm::exec_iinc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented iinc opcode");
}


void EJvm::exec_i2l(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented i2l opcode");
}


void EJvm::exec_i2f(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented i2f opcode");
}


void EJvm::exec_i2d(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented i2d opcode");
}


void EJvm::exec_l2i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented l2i opcode");
}


void EJvm::exec_l2f(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented l2f opcode");
}


void EJvm::exec_l2d(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented l2d opcode");
}


void EJvm::exec_f2i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented f2i opcode");
}


void EJvm::exec_f2l(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented f2l opcode");
}


void EJvm::exec_f2d(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented f2d opcode");
}


void EJvm::exec_d2i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented d2i opcode");
}


void EJvm::exec_d2l(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented d2l opcode");
}


void EJvm::exec_d2f(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented d2f opcode");
}


void EJvm::exec_i2b(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented i2b opcode");
}


void EJvm::exec_i2c(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented i2c opcode");
}


void EJvm::exec_i2s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented i2s opcode");
}


void EJvm::exec_lcmp(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lcmp opcode");
}


void EJvm::exec_fcmpl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fcmpl opcode");
}


void EJvm::exec_fcmpg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented fcmpg opcode");
}


void EJvm::exec_dcmpl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dcmpl opcode");
}


void EJvm::exec_dcmpg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dcmpg opcode");
}


void EJvm::exec_ifeq(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifeq opcode");
}


void EJvm::exec_ifne(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifne opcode");
}


void EJvm::exec_iflt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented iflt opcode");
}


void EJvm::exec_ifge(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifge opcode");
}


void EJvm::exec_ifgt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifgt opcode");
}


void EJvm::exec_ifle(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifle opcode");
}


void EJvm::exec_if_icmpeq(ULONG loc,unsigned int opcode)
{
  ECPUByteCodeObject *o=popObj();
  // TODO: check type of o, case if necessary(?)
  unsigned int val1=o->val;
  o=popObj();
  // TODO: check type of o, case if necessary(?)
  unsigned int val2=o->val;
  // TODO: compare val1 and val2
  // If equal, jump to offset given (8 or 16 bit?)
  debugger("Unimplemented if_icmpeq opcode");
}


void EJvm::exec_if_icmpne(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented if_icmpne opcode");
}


void EJvm::exec_if_icmplt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented if_icmplt opcode");
}


void EJvm::exec_if_icmpge(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented if_icmpge opcode");
}


void EJvm::exec_if_icmpgt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented if_icmpgt opcode");
}


void EJvm::exec_if_icmple(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented if_icmple opcode");
}


void EJvm::exec_if_acmpeq(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented if_acmpeq opcode");
}


void EJvm::exec_ifacmpne(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifacmpne opcode");
}


void EJvm::exec_goto(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented goto opcode");
}


void EJvm::exec_jsr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jsr opcode");
}


void EJvm::exec_ret(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ret opcode");
}


void EJvm::exec_tableswitch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented tableswitch opcode");
}


void EJvm::exec_lookupswitch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lookupswitch opcode");
}


void EJvm::exec_ireturn(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ireturn opcode");
}


void EJvm::exec_lreturn(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lreturn opcode");
}


void EJvm::exec_freturn(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented freturn opcode");
}


void EJvm::exec_dreturn(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dreturn opcode");
}


void EJvm::exec_areturn(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented areturn opcode");
}


void EJvm::exec_return(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented return opcode");
}


void EJvm::exec_getstatic(ULONG loc,unsigned int opcode)
{
  unsigned int num=0;
  num=map->read16b(loc+1);
  EMapSymbol *sym=map->getFirstModule()->lookupStringByNum(num);
  if(!sym) {
    debugger("exec_getstatic didn't find needed constant!\n");
    return;
  }
  DEBUG_OUT<<"getstatic num="<<num<<" type="<<sym->type<<"\n";
  if(sym->type==ECPUABSTRACT_TYPE_FIELD) {
    ECPUByteCodeObject o;
    o.type=sym->val;
    o.name=sym->val2;
	EMapSymbol *val=map->getFirstModule()->lookupStringByNum(sym->val2);
    o.val=val->val;
    o.len=0;
    pushObj(&o);
    DEBUG_OUT<<"getstatic["<<o.type<<","<<o.name<<"] ";
	DEBUG_OUT<<sym->getName()<<" ";   // field name?
    s[0]=0;
    getNameString(s,o.type); // class?
    DEBUG_OUT<<s<<" ";
    s[0]=0;
    getNameString(s,o.name); // type?
    DEBUG_OUT<<s<<"\n";
  }
  else {
    debugger("Bad getstatic constant type!");
  }
}


void EJvm::exec_putstatic(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented putstatic opcode");
}


void EJvm::exec_getfield(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getfield opcode");
}


void EJvm::exec_putfield(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented putfield opcode");
}


void EJvm::invoke_helper(ULONG loc,unsigned int opcode)
{
  unsigned int type=0,num=0,name=0;
  ULONG addr=0;
  num=map->read16b(loc+1);
  EMapSymbol *sym=map->getFirstModule()->lookupStringByNum(num);
  if(!sym) {
    debugger("invoke_helper didn't find needed constant!\n");
    return;
  }
  DEBUG_OUT<<"invoke_helper num="<<num<<" type="<<sym->type<<"\n";
  if(sym->type==ECPUABSTRACT_TYPE_METHOD) {
    type=sym->val;
    name=sym->val2;
    unsigned int numArgs=2;  // TODO: read method sig for this...
    ECPUByteCodeObject *args[10];
    unsigned int t=0;
    for(t=0;t<10;t++) args[t]=NULL;
    for(t=0;t<numArgs;t++) {
      args[t]=popObj();
    }
    DEBUG_OUT<<"invoke_helper #"<<num<<" type="<<type<<" name="<<name<<" "<<"numArgs is "<<numArgs<<"\n";
    DEBUG_OUT<<"invoke_helper #"<<num<<" type="<<type<<" name="<<name<<" "<<"numArgs is "<<numArgs<<"\n";
    DEBUG_OUT<<"invoke_helper["<<type<<","<<name<<"] "<<sym->getName()<<" ";  // meth name?
    DEBUG_OUT<<"invoke_helper["<<type<<","<<name<<"] "<<sym->getName()<<" ";  // meth name?
    s[0]=0;
    getNameString(s,type);
    DEBUG_OUT<<s<<" ";  // class?
    DEBUG_OUT<<s<<" ";  // class?
    s[0]=0;
    getNameString(s,name);
    DEBUG_OUT<<s<<"\n";  // signature?
    DEBUG_OUT<<s<<"\n";  // signature?
    EMapSymbol *sym2=map->getFirstModule()->lookupSymbolByName(s);
    if(sym2) addr=sym2->addr;
    if(addr) {
      jump(addr);
    }
    else {
      debugger("Couldn't locate method to jump to!");
    }
  }
  else {
    debugger("Bad invoke_helper constant type!");
  }
}


void EJvm::exec_invokevirtual(ULONG loc,unsigned int opcode)
{
  invoke_helper(loc,opcode);
}


void EJvm::exec_invokespecial(ULONG loc,unsigned int opcode)
{
  invoke_helper(loc,opcode);
}


void EJvm::exec_invokestatic(ULONG loc,unsigned int opcode)
{
  invoke_helper(loc,opcode);
}


void EJvm::exec_invokeinterface(ULONG loc,unsigned int opcode)
{
  invoke_helper(loc,opcode);
}


void EJvm::exec_new(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented new opcode");
}


void EJvm::exec_newarray(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented newarray opcode");
}


void EJvm::exec_anewarray(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented anewarray opcode");
}


void EJvm::exec_arraylength(ULONG loc,unsigned int opcode)
{
  ECPUByteCodeObject *o=popObj();
  // TODO: Check type of o, must be array
  unsigned int len=o->len;
  ECPUByteCodeObject theLen;
  theLen.type=ECPUABSTRACT_TYPE_INT;
  theLen.val=len;
  pushObj(&theLen);
}


void EJvm::exec_athrow(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented athrow opcode");
}


void EJvm::exec_checkcast(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented checkcast opcode");
}


void EJvm::exec_instanceof(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented instanceof opcode");
}


void EJvm::exec_monitorenter(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented monitorenter opcode");
}


void EJvm::exec_monitorexit(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented monitorexit opcode");
}


void EJvm::exec_wide(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented wide opcode");
}


void EJvm::exec_multinewarray(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented multinewarray opcode");
}


void EJvm::exec_ifnull(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifnull opcode");
}


void EJvm::exec_ifnonnull(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifnonnull opcode");
}


void EJvm::exec_goto_w(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented goto_w opcode");
}


void EJvm::exec_jsr_w(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jsr_w opcode");
}


void EJvm::exec_breakpoint(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented breakpoint opcode");
}


void EJvm::exec_ldc_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc_quick opcode");
}


void EJvm::exec_ldc_w_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc_w_quick opcode");
}


void EJvm::exec_ldc2_w_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc2_w_quick opcode");
}


void EJvm::exec_getfield_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getfield_quick opcode");
}


void EJvm::exec_putfield_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented putfield_quick opcode");
}


void EJvm::exec_getfield2_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getfield2_quick opcode");
}


void EJvm::exec_putfield2_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented putfield2_quick opcode");
}


void EJvm::exec_getstatic_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getstatic_quick opcode");
}


void EJvm::exec_putstatic_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented putstatic_quick opcode");
}


void EJvm::exec_getstatic2_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getstatic2_quick opcode");
}


void EJvm::exec_putstatic2_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented putstatic2_quick opcode");
}


void EJvm::exec_invokevirtual_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented invokevirtual_quick opcode");
}


void EJvm::exec_invokenonvirtual_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented invokenonvirtual_quick opcode");
}


void EJvm::exec_invokesuper_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented invokesuper_quick opcode");
}


void EJvm::exec_invokestatic_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented invokestatic_quick opcode");
}


void EJvm::exec_invokeinterface_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented invokeinterface_quick opcode");
}


void EJvm::exec_invokevirtualobject_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented invokevirtualobject_quick opcode");
}


void EJvm::exec_new_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented new_quick opcode");
}


void EJvm::exec_anewarray_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented anewarray_quick opcode");
}


void EJvm::exec_multinewarray_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented multinewarray_quick opcode");
}


void EJvm::exec_checkcast_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented checkcast_quick opcode");
}


void EJvm::exec_instanceof_quick(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented instanceof_quick opcode");
}


void EJvm::exec_invokevirtual_quick_w(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented invokevirtual_quick_w opcode");
}


void EJvm::exec_getfield_quick_w(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getfield_quick_w opcode");
}


void EJvm::exec_putfield_quick_w(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented putfield_quick_w opcode");
}


void EJvm::exec_impdep1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented impdep1 opcode");
}


void EJvm::exec_impdep2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented impdep2 opcode");
}


void EJvm::aload_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  DEBUG_OUT<<"aload #"<<num<<"\n";
  debugger("Unimplemented aload_helper");
}


void EJvm::iload_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  DEBUG_OUT<<"iload #"<<num<<"\n";
  debugger("Unimplemented iload_helper");
}


void EJvm::lload_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  DEBUG_OUT<<"lload #"<<num<<"\n";
  debugger("Unimplemented lload_helper");
}


void EJvm::fload_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  DEBUG_OUT<<"fload #"<<num<<"\n";
  debugger("Unimplemented fload_helper");
}


void EJvm::dload_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  DEBUG_OUT<<"dload #"<<num<<"\n";
  debugger("Unimplemented dload_helper");
}


void EJvm::iconst_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  ECPUByteCodeObject o;
  o.type=ECPUABSTRACT_TYPE_INT;
  o.val=num;
  o.name=0;
  o.len=0;
  pushObj(&o);
  DEBUG_OUT<<"iconst #"<<o.val<<"\n";
}


void EJvm::lconst_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  ECPUByteCodeObject o;
  o.type=ECPUABSTRACT_TYPE_LONG;
  o.val=num;
  o.name=0;
  o.len=0;
  pushObj(&o);
  DEBUG_OUT<<"lconst #"<<o.val<<"\n";
}


// TODO: obviously this should be float not int
void EJvm::fconst_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  ECPUByteCodeObject o;
  o.type=ECPUABSTRACT_TYPE_FLOAT;
  o.val=num;
  o.name=0;
  o.len=0;
  pushObj(&o);
  DEBUG_OUT<<"fconst #"<<o.val<<"\n";
}


// TODO: obviously this should be float not int
void EJvm::dconst_helper(ULONG loc, unsigned int opcode, unsigned int num)
{
  ECPUByteCodeObject o;
  o.type=ECPUABSTRACT_TYPE_DOUBLE;
  o.val=num;
  o.name=0;
  o.len=0;
  pushObj(&o);
  DEBUG_OUT<<"dconst #"<<o.val<<"\n";
}


// copied from NJavaClassDecoder...for now
void EJvm::getStringAtAddr(char *s,unsigned int len,ULONG addr)
{
  if(!s) return;
  unsigned int i=0;
  for(i=0;i<len;i++) s[i]=map->read8(addr+i);
  s[i]=0;
}


unsigned int EJvm::getNameString(char *s,unsigned int index)
{
  if(!s) return 0;
  if(!index) { s[0]=0;  return 0; } //there is no const 0
  s[0]=0;
  EMapSymbol *sym=map->getFirstModule()->lookupStringByNum(index);
  if(sym) {
    strcpy(s,sym->getName());
    return strlen(s);
  }
  return 0;
}


unsigned int EJvm::getNameStringVal(char *s,unsigned int index)
{
  if(!s) return 0;
  if(!index) { s[0]=0;  return 0; } //there is no const 0
  s[0]=0;
  EMapSymbol *sym=map->getFirstModule()->lookupStringByNum(index);
  if(sym) {
    return getNameString(s,sym->val);
  }
  return 0;
}


unsigned int EJvm::getNameStringVal2(char *s,unsigned int index)
{
  if(!s) return 0;
  if(!index) { s[0]=0;  return 0; } //there is no const 0
  s[0]=0;
  EMapSymbol *sym=map->getFirstModule()->lookupStringByNum(index);
  if(sym) {
    return getNameString(s,sym->val2);
  }
  return 0;
}


unsigned int EJvm::getNameStringMisc(char *s,unsigned int index)
{
  if(!s) return 0;
  if(!index) { s[0]=0;  return 0; } //there is no const 0
  s[0]=0;
  EMapSymbol *sym=map->getFirstModule()->lookupStringByNum(index);
  if(sym) {
    return getNameString(s,sym->misc);
  }
  return 0;
}

