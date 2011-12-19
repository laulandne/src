
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


//#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/EDotNet.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/EDotNet_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_DOTNET   0x01


////////////////////////////////////////////////////////////////////////////////
//  EDotNet Class
////////////////////////////////////////////////////////////////////////////////

EDotNet::EDotNet()
{
  DEBUG_OUT<<"eDotNet created...\n";
  init();
  type=E_CPU_DOTNET;
  //w=ww;
  reset();
}


EDotNet::~EDotNet()
{
  DEBUG_OUT<<"eDotNet destroyed.\n";
}


void EDotNet::init()
{
  ECPUByteCode::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  lsb=true;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool EDotNet::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"dotnet")) return true; else
  if(!strcmp(name,"clr")) return true; else
  if(!strcmp(name,"cil")) return true; else
  if(!strcmp(name,".net")) return true; else
  return false;
}


const char *EDotNet::getName()
{
  return "DotNet";
}


void EDotNet::setKind(const char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_DOTNET;
}


void EDotNet::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffffffff;  // since we start at 0, breakpoint's default
  intLock=false;
  pc=0;  // Jump to zero
  clearStackTrace();
  ECPUAbstract::reset();
}


bool EDotNet::setRegister(const char *tname,ULONG value)
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


ULONG EDotNet::readRegister(const char *name)
{
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


void EDotNet::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  DEBUG_OUT<<"SP="<<sp<<"\n";
}


void EDotNet::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=EDotNet_size[opcode];
  cycle=EDotNet_time[opcode];
  topcode=opcode;
  unsigned int loc=pc;
#include <cpulib/generated/EDotNet_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG EDotNet::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=EDOTNET_NONE;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=EDotNet_size[topcode];
  mode=EDotNet_format[topcode];
  name=EDotNet_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<(short)map->read8(loc+t)<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  len+=printAddrMode(loc,topcode,mode);
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return len;
}


unsigned int EDotNet::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  return 0;
}


void EDotNet::exec_nop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented nop opcode");
}


void EDotNet::exec_break(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented break opcode");
}


void EDotNet::exec_ldarg_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldarg.0 opcode");
}


void EDotNet::exec_ldarg_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldarg.1 opcode");
}


void EDotNet::exec_ldarg_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldarg.2 opcode");
}


void EDotNet::exec_ldarg_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldarg.3 opcode");
}


void EDotNet::exec_ldloc_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldloc.0 opcode");
}


void EDotNet::exec_ldloc_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldloc.1 opcode");
}


void EDotNet::exec_ldloc_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldloc.2 opcode");
}


void EDotNet::exec_ldloc_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldloc.3 opcode");
}


void EDotNet::exec_stloc_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stloc.0 opcode");
}


void EDotNet::exec_stloc_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stloc.1 opcode");
}


void EDotNet::exec_stloc_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stloc.2 opcode");
}


void EDotNet::exec_stloc_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stloc.3 opcode");
}


void EDotNet::exec_ldarg_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldarg.s opcode");
}


void EDotNet::exec_ldarga_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldarga.s opcode");
}


void EDotNet::exec_starg_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented starg.s opcode");
}


void EDotNet::exec_ldloc_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldloc.s opcode");
}


void EDotNet::exec_ldloca_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldloca.s opcode");
}


void EDotNet::exec_stloc_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stloc.s opcode");
}


void EDotNet::exec_ldnull(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldnull opcode");
}


void EDotNet::exec_ldc_i4_m1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.m1 opcode");
}


void EDotNet::exec_ldc_i4_0(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.0 opcode");
}


void EDotNet::exec_ldc_i4_1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.1 opcode");
}


void EDotNet::exec_ldc_i4_2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.2 opcode");
}


void EDotNet::exec_ldc_i4_3(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.3 opcode");
}


void EDotNet::exec_ldc_i4_4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.4 opcode");
}


void EDotNet::exec_ldc_i4_5(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.5 opcode");
}


void EDotNet::exec_ldc_i4_6(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.6 opcode");
}


void EDotNet::exec_ldc_i4_7(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.7 opcode");
}


void EDotNet::exec_ldc_i4_8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.8 opcode");
}


void EDotNet::exec_ldc_i4_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4.s opcode");
}


void EDotNet::exec_ldc_i4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i4 opcode");
}


void EDotNet::exec_ldc_i8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.i8 opcode");
}


void EDotNet::exec_ldc_r4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.r4 opcode");
}


void EDotNet::exec_ldc_r8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldc.r8 opcode");
}


void EDotNet::exec_dup(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup opcode");
}


void EDotNet::exec_pop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pop opcode");
}


void EDotNet::exec_jmp(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jmp opcode");
}


void EDotNet::exec_call(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented call opcode");
}


void EDotNet::exec_calli(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented calli opcode");
}


void EDotNet::exec_ret(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ret opcode");
}


void EDotNet::exec_br_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented br.s opcode");
}


void EDotNet::exec_brfalse_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented brfalse.s opcode");
}


void EDotNet::exec_brtrue_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented brtrue.s opcode");
}


void EDotNet::exec_beq_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented beq.s opcode");
}


void EDotNet::exec_bge_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bge.s opcode");
}


void EDotNet::exec_bgt_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bgt.s opcode");
}


void EDotNet::exec_ble_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ble.s opcode");
}


void EDotNet::exec_blt_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented blt.s opcode");
}


void EDotNet::exec_bne_un_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bne.un.s opcode");
}


void EDotNet::exec_bge_un_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bge.un.s opcode");
}


void EDotNet::exec_bgt_un_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bgt.un.s opcode");
}


void EDotNet::exec_ble_un_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ble.un.s opcode");
}


void EDotNet::exec_blt_un_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented blt.un.s opcode");
}


void EDotNet::exec_br(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented br opcode");
}


void EDotNet::exec_brfalse(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented brfalse opcode");
}


void EDotNet::exec_brtrue(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented brtrue opcode");
}


void EDotNet::exec_beq(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented beq opcode");
}


void EDotNet::exec_bge(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bge opcode");
}


void EDotNet::exec_bgt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bgt opcode");
}


void EDotNet::exec_ble(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ble opcode");
}


void EDotNet::exec_blt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented blt opcode");
}


void EDotNet::exec_bne_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bne.un opcode");
}


void EDotNet::exec_bge_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bge.un opcode");
}


void EDotNet::exec_bgt_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bgt.un opcode");
}


void EDotNet::exec_ble_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ble.un opcode");
}


void EDotNet::exec_blt_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented blt.un opcode");
}


void EDotNet::exec_switch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented switch opcode");
}


void EDotNet::exec_ldind_i1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.i1 opcode");
}


void EDotNet::exec_ldind_u1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.u1 opcode");
}


void EDotNet::exec_ldind_i2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.i2 opcode");
}


void EDotNet::exec_ldind_u2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.u2 opcode");
}


void EDotNet::exec_ldind_i4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.i4 opcode");
}


void EDotNet::exec_ldind_u4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.u4 opcode");
}


void EDotNet::exec_ldind_i8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.i8 opcode");
}


void EDotNet::exec_ldind_i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.i opcode");
}


void EDotNet::exec_ldind_r4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.r4 opcode");
}


void EDotNet::exec_ldind_r8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.r8 opcode");
}


void EDotNet::exec_ldind_ref(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldind.ref opcode");
}


void EDotNet::exec_stind_ref(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stind.ref opcode");
}


void EDotNet::exec_stind_i1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stind.i1 opcode");
}


void EDotNet::exec_stind_i2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stind.i2 opcode");
}


void EDotNet::exec_stind_i4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stind.i4 opcode");
}


void EDotNet::exec_stind_i8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stind.i8 opcode");
}


void EDotNet::exec_stind_r4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stind.r4 opcode");
}


void EDotNet::exec_stind_r8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stind.r8 opcode");
}


void EDotNet::exec_add(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented add opcode");
}


void EDotNet::exec_sub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sub opcode");
}


void EDotNet::exec_mul(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mul opcode");
}


void EDotNet::exec_div(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented div opcode");
}


void EDotNet::exec_div_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented div.un opcode");
}


void EDotNet::exec_rem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented rem opcode");
}


void EDotNet::exec_rem_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented rem.un opcode");
}


void EDotNet::exec_and(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented and opcode");
}


void EDotNet::exec_or(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented or opcode");
}


void EDotNet::exec_xor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented xor opcode");
}


void EDotNet::exec_shl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented shl opcode");
}


void EDotNet::exec_shr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented shr opcode");
}


void EDotNet::exec_shr_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented shr.un opcode");
}


void EDotNet::exec_neg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented neg opcode");
}


void EDotNet::exec_not(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented not opcode");
}


void EDotNet::exec_conv_i1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.i1 opcode");
}


void EDotNet::exec_conv_i2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.i2 opcode");
}


void EDotNet::exec_conv_i4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.i4 opcode");
}


void EDotNet::exec_conv_i8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.i8 opcode");
}


void EDotNet::exec_conv_r4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.r4 opcode");
}


void EDotNet::exec_conv_r8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.r8 opcode");
}


void EDotNet::exec_conv_u4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.u4 opcode");
}


void EDotNet::exec_conv_u8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.u8 opcode");
}


void EDotNet::exec_callvirt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented callvirt opcode");
}


void EDotNet::exec_cpobj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented cpobj opcode");
}


void EDotNet::exec_ldobj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldobj opcode");
}


void EDotNet::exec_ldstr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldstr opcode");
}


void EDotNet::exec_newobj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented newobj opcode");
}


void EDotNet::exec_castclass(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented castclass opcode");
}


void EDotNet::exec_isinst(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented isinst opcode");
}


void EDotNet::exec_conv_r_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.r.un opcode");
}


void EDotNet::exec_unbox(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented unbox opcode");
}


void EDotNet::exec_throw(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented throw opcode");
}


void EDotNet::exec_ldfld(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldfld opcode");
}


void EDotNet::exec_ldflda(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldflda opcode");
}


void EDotNet::exec_stfld(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stfld opcode");
}


void EDotNet::exec_ldsfld(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldsfld opcode");
}


void EDotNet::exec_ldsflda(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldsflda opcode");
}


void EDotNet::exec_stsfld(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stsfld opcode");
}


void EDotNet::exec_stobj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stobj opcode");
}


void EDotNet::exec_conv_ovf_i1_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i1.un opcode");
}


void EDotNet::exec_conv_ovf_i2_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i2.un opcode");
}


void EDotNet::exec_conv_ovf_i4_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i4.un opcode");
}


void EDotNet::exec_conv_ovf_i8_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i8.un opcode");
}


void EDotNet::exec_conv_ovf_u1_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u1.un opcode");
}


void EDotNet::exec_conv_ovf_u2_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u2.un opcode");
}


void EDotNet::exec_conv_ovf_u4_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u4.un opcode");
}


void EDotNet::exec_conv_ovf_u8_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u8.un opcode");
}


void EDotNet::exec_conv_ovf_i_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i.un opcode");
}


void EDotNet::exec_conv_ovf_u_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u.un opcode");
}


void EDotNet::exec_box(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented box opcode");
}


void EDotNet::exec_newarr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented newarr opcode");
}


void EDotNet::exec_ldlen(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldlen opcode");
}


void EDotNet::exec_ldelema(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelema opcode");
}


void EDotNet::exec_ldelem_i1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.i1 opcode");
}


void EDotNet::exec_ldelem_u1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.u1 opcode");
}


void EDotNet::exec_ldelem_i2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.i2 opcode");
}


void EDotNet::exec_ldelem_u2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.u2 opcode");
}


void EDotNet::exec_ldelem_i4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.i4 opcode");
}


void EDotNet::exec_ldelem_u4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.u4 opcode");
}


void EDotNet::exec_ldelem_i8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.i8 opcode");
}


void EDotNet::exec_ldelem_i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.i opcode");
}


void EDotNet::exec_ldelem_r4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.r4 opcode");
}


void EDotNet::exec_ldelem_r8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.r8 opcode");
}


void EDotNet::exec_ldelem_ref(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldelem.ref opcode");
}


void EDotNet::exec_stelem_i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stelem.i opcode");
}


void EDotNet::exec_stelem_i1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stelem.i1 opcode");
}


void EDotNet::exec_stelem_i2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stelem.i2 opcode");
}


void EDotNet::exec_stelem_i4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stelem.i4 opcode");
}


void EDotNet::exec_stelem_i8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stelem.i8 opcode");
}


void EDotNet::exec_stelem_r4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stelem.r4 opcode");
}


void EDotNet::exec_stelem_r8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stelem.r8 opcode");
}


void EDotNet::exec_stelem_ref(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stelem.ref opcode");
}


void EDotNet::exec_conv_ovf_i1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i1 opcode");
}


void EDotNet::exec_conv_ovf_u1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u1 opcode");
}


void EDotNet::exec_conv_ovf_i2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i2 opcode");
}


void EDotNet::exec_conv_ovf_u2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u2 opcode");
}


void EDotNet::exec_conv_ovf_i4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i4 opcode");
}


void EDotNet::exec_conv_ovf_u4(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u4 opcode");
}


void EDotNet::exec_conv_ovf_i8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i8 opcode");
}


void EDotNet::exec_conv_ovf_u8(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u8 opcode");
}


void EDotNet::exec_refanyval(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented refanyval opcode");
}


void EDotNet::exec_ckfinite(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ckfinite opcode");
}


void EDotNet::exec_mkrefany(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mkrefany opcode");
}


void EDotNet::exec_ldtoken(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ldtoken opcode");
}


void EDotNet::exec_conv_u2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.u2 opcode");
}


void EDotNet::exec_conv_u1(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.u1 opcode");
}


void EDotNet::exec_conv_i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.i opcode");
}


void EDotNet::exec_conv_ovf_i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.i opcode");
}


void EDotNet::exec_conv_ovf_u(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.ovf.u opcode");
}


void EDotNet::exec_add_ovf(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented add.ovf opcode");
}


void EDotNet::exec_add_ovf_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented add.ovf.un opcode");
}


void EDotNet::exec_mul_ovf(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mul.ovf opcode");
}


void EDotNet::exec_mul_ovf_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mul.ovf.un opcode");
}


void EDotNet::exec_sub_ovf(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sub.ovf opcode");
}


void EDotNet::exec_sub_ovf_un(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sub.ovf.un opcode");
}


void EDotNet::exec_endfinally(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented endfinally opcode");
}


void EDotNet::exec_leave(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented leave opcode");
}


void EDotNet::exec_leave_s(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented leave.s opcode");
}


void EDotNet::exec_stind_i(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stind.i opcode");
}


void EDotNet::exec_conv_u(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented conv.u opcode");
}


void EDotNet::exec_special(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented special opcode");
}



