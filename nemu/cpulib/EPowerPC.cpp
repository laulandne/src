
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

NOTE: This code doesn't emulate a pipeline of any kind, so there are no
real hazards or stalls.  Delay slots are simulated.  Don't expect it to
act as a real pipelined RISC processor would under all conditions.

*/


#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/EPowerPC.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/EPowerPC_tab.h>
#include <cpulib/generated/EPowerPC_31_tab.h>


////////////////////////////////////////////////////////////////////////////////
//  EPowerPC Class
////////////////////////////////////////////////////////////////////////////////

EPowerPC::EPowerPC()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"EPowerPC created...\n";
#endif // DEBUG_VERBOSE
  init();
  //w=ww;
  reset();
}


EPowerPC::~EPowerPC()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"EPowerPC destroyed.\n";
#endif // DEBUG_VERBOSE
}


void EPowerPC::init()
{
  ECPURisc::init();
  bitness=32;
  branchesAreDelayed=false;
  r0IsZero=false;
  doTrace=false;  doRun=false;
  lsb=false;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool EPowerPC::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"power")) return true; else
  if(!strcmp(name,"power1")) return true; else
  if(!strcmp(name,"power2")) return true; else
  if(!strcmp(name,"power3")) return true; else
  if(!strcmp(name,"power4")) return true; else
  if(!strcmp(name,"rs6000")) return true; else
  if(!strcmp(name,"powerpc")) return true; else
  if(!strcmp(name,"powerpc64")) return true; else
  if(!strcmp(name,"powermac")) return true; else
  if(!strcmp(name,"powermacintosh")) return true; else
  if(!strcmp(name,"ppc")) return true; else
  if(!strcmp(name,"ppc64")) return true; else
  if(!strcmp(name,"401")) return true; else
  if(!strcmp(name,"401ga")) return true; else
  if(!strcmp(name,"403")) return true; else
  if(!strcmp(name,"403ga")) return true; else
  if(!strcmp(name,"505")) return true; else
  if(!strcmp(name,"601")) return true; else
  if(!strcmp(name,"602")) return true; else
  if(!strcmp(name,"603")) return true; else
  if(!strcmp(name,"603e")) return true; else
  if(!strcmp(name,"603ev")) return true; else
  if(!strcmp(name,"604")) return true; else
  if(!strcmp(name,"604e")) return true; else
  if(!strcmp(name,"615")) return true; else
  if(!strcmp(name,"620")) return true; else
  if(!strcmp(name,"801")) return true; else
  if(!strcmp(name,"740")) return true; else
  if(!strcmp(name,"745")) return true; else
  if(!strcmp(name,"750")) return true; else
  if(!strcmp(name,"750fx")) return true; else
  if(!strcmp(name,"750gx")) return true; else
  if(!strcmp(name,"7400")) return true; else
  if(!strcmp(name,"7410")) return true; else
  if(!strcmp(name,"7440")) return true; else
  if(!strcmp(name,"7447")) return true; else
  if(!strcmp(name,"7450")) return true; else
  if(!strcmp(name,"7500")) return true; else
  if(!strcmp(name,"970")) return true; else
  if(!strcmp(name,"970fx")) return true; else
  if(!strcmp(name,"970mp")) return true; else
  if(!strcmp(name,"mpc500")) return true; else
  if(!strcmp(name,"mpc505")) return true; else
  if(!strcmp(name,"mpc601")) return true; else
  if(!strcmp(name,"mpc602")) return true; else
  if(!strcmp(name,"mpc603")) return true; else
  if(!strcmp(name,"mpc603e")) return true; else
  if(!strcmp(name,"mpc603ev")) return true; else
  if(!strcmp(name,"mpc604")) return true; else
  if(!strcmp(name,"mpc604e")) return true; else
  if(!strcmp(name,"mpc620")) return true; else
  if(!strcmp(name,"mpc801")) return true; else
  if(!strcmp(name,"mpc740")) return true; else
  if(!strcmp(name,"mpc750")) return true; else
  if(!strcmp(name,"ibm401ga")) return true; else
  if(!strcmp(name,"ibm403ga")) return true; else
  if(!strcmp(name,"g3")) return true; else
  if(!strcmp(name,"g4")) return true; else
  if(!strcmp(name,"g5")) return true; else
  if(!strcmp(name,"rios")) return true; else
  if(!strcmp(name,"rios1")) return true; else
  if(!strcmp(name,"rios2")) return true; else
  if(!strcmp(name,"mc98000")) return true; else
  return false;
}


const char *EPowerPC::getName()
{
  return "PowerPC";
}


void EPowerPC::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  // For now all treated the same...
  // ...should determine 32 vs 64 bit here someplace.
}


void EPowerPC::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif // DEBUG_EXEC
  unsigned int t;
  pc=0;
  ir=0;
  for(t=0;t<32;t++) { G[t]=0; }
  for(t=0;t<32;t++) { F[t]=0.0; }
  CRz=0;  XERz=0;
  LR=0;  CTR=0;  FPSCRz=0;
  tookBranch=false;
  didOverflow=false;
  branch_address=0;
  op=0;  opName=(char *)NULL;
  top=0;  bop=0;
  opFormat=EPPC_FORMAT_UNKNOWN;
  opSubFormat=0;
  hasFPU=true;
  hasMMU=true;
  //lsb=false; // But we can swtich...
  // TODO: where does a ppc normally go on reset?
  clearStackTrace();
  ECPUAbstract::reset();
  writeReg(13,readReg(1));  // Point "small data area" at stack...
  writeReg(2,readReg(1));  // Point "TOC" at stack...
}


bool EPowerPC::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  if((name[0]=='r')||(name[0]=='R')) {
    unsigned int n=aDefaultStringHelper.parseNumber(name+1);
    writeReg(n,value);
    ret=true;
  } else
  ret=false;
  return ret;
}


// TODO: not sure if 1 is the sp or fp or both!
void EPowerPC::setStack(ULONG val)
{
  writeReg(1,val);
}


// TODO: not sure if 1 is the sp or fp or both!
ULONG EPowerPC::readStack()
{
  return readReg(1);
}


// TODO: not sure if 1 is the sp or fp or both!
void EPowerPC::setFramePtr(ULONG val)
{
  writeReg(1,val);
}


// TODO: not sure if 1 is the sp or fp or both!
ULONG EPowerPC::readFramePtr()
{
  return readReg(1);
}


void EPowerPC::setReturnReg(ULONG val)
{
  writeReg(3,val);
}


ULONG EPowerPC::readReturnReg()
{
  return readReg(3);
}


ULONG EPowerPC::readParamReg(unsigned int num)
{
  return readReg(num+3);
}


void EPowerPC::setParamReg(unsigned int num,ULONG val)
{
  writeReg(num+3,val);
}


bool EPowerPC::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool ret=false;
  if(!strcmp(cmd,"rm")) { wmregs(); ret=true; }
  if(!strcmp(cmd,"rf")) { wfregs(); ret=true; }
  return ret;
}


void EPowerPC::wmregs()
{
}


void EPowerPC::wfregs()
{
  if(!hasFPU) return;
  DEBUG_OUT<<"FPSCRz="<<FPSCRz<<"\n";
  unsigned int t;
  for(t=0;t<32;t++) {
    DEBUG_OUT<<"f";
    if(t<10) DEBUG_OUT<<"0";
    DEBUG_OUT<<t<<"="<<F[t]<<" ";
    if(!((t+1)%3)) { DEBUG_OUT<<"\n"; }
  }
  DEBUG_OUT<<"\n";
}


void EPowerPC::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<" ";
  if(lsb) DEBUG_OUT<<"(Little)"; else DEBUG_OUT<<"(Big)";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"LR="<<LR<<" CTR="<<CTR<<"\n";
  for(unsigned int r=0;r<8;r++) {
    printReg(r*4);
    DEBUG_OUT<<"=";
    for(unsigned int c=0;c<4;c++)  DEBUG_OUT<<G[r*4+c]<<" ";
    DEBUG_OUT<<"\n";
  }
  DEBUG_OUT<<"CRz="<<CRz<<" XERz="<<XERz<<"\n";
}


void EPowerPC::handle_int(ULONG value)
{
  // Need to handle these...
}


void EPowerPC::cause_int(ULONG value)
{
  // Need to handle these...
}


ULONG EPowerPC::disasm(ULONG loc)
{
  bool old=doTrace;
  doTrace=true;
  unsigned int oldPC=pc;
  pc=loc;
  ir=read32mem(pc);
  lastSymbol=map->findAnyExactSymbol(pc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."<<pc<<" ";
  DEBUG_OUT<<ir<<"  ";
  op=ir;
  top=(op>>24)&0xff;
  top=(top>>2)&0x3f;
  opName=EPowerPC_name[top];
  opFormat=EPowerPC_format[top];
  opSubFormat=EPowerPC_subformat[top];
  decodeAndCalc();
  if(opFormat==EPPC_FORMAT_UNKNOWN) dis_unimplemented(opName);
  else disHelper(opName,opFormat,opSubFormat);
  doTrace=old;
  pc=oldPC;
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return 4;
}


void EPowerPC::execute(void)
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  doRun=true;
  ir=read32mem(pc);
  op=ir;
  tookBranch=false;
  top=(op>>24)&0xff;
  top=(top>>2)&0x3f;
  opName=EPowerPC_name[top];
  opFormat=EPowerPC_format[top];
  opSubFormat=EPowerPC_subformat[top];
  int topcode=top;
  decodeAndCalc();
#include <cpulib/generated/EPowerPC_exec.h>
  if(tookBranch) { pc=branch_address; } else { pc+=4; }
  if(int_pending) handle_int(int_pending);
}


ULONG EPowerPC::readReg(unsigned int n)
{
  unsigned int rn=n&0x1f;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tRead "<<G[rn]<<" from ";
  //w->dec();
  DEBUG_OUT<<"r"<<rn<<"\n";
  //w->hex();
#endif
  return G[rn];
}


ULONG EPowerPC::readReg0(unsigned int n)
{
  unsigned int rn=n&0x1f;
  unsigned int rval=G[rn];
  if(!rn) rval=0;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tRead0 "<<rval<<" from ";
  //w->dec();
  DEBUG_OUT<<"r"<<rn<<"\n";
  //w->hex();
#endif
  return rval;
}


void EPowerPC::writeReg(unsigned int n, ULONG val)
{
  unsigned int rn=n&0x1f;
  G[rn]=val;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tWrite "<<val;
  //w->dec();
  DEBUG_OUT<<" to r"<<rn<<"\n";
  //w->hex();
#endif
}


void EPowerPC::writeReg0(unsigned int n, ULONG val)
{
  unsigned int rn=n&0x1f;
  if(!rn) return;
  G[rn]=val;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tWrite0 "<<val;
  //w->dec();
  DEBUG_OUT<<" to r"<<rn<<"\n";
  //w->hex();
#endif
}


void EPowerPC::printReg(unsigned int num)
{
  unsigned int rn=num&0x1f;
  switch(rn) {
    case 0:  DEBUG_OUT<<"r0"; break;
    case 1:  DEBUG_OUT<<"r1"; break;
    case 2:  DEBUG_OUT<<"r2"; break;
    case 3:  DEBUG_OUT<<"r3"; break;
    case 4:  DEBUG_OUT<<"r4"; break;
    case 5:  DEBUG_OUT<<"r5"; break;
    case 6:  DEBUG_OUT<<"r6"; break;
    case 7:  DEBUG_OUT<<"r7"; break;
    case 8:  DEBUG_OUT<<"r8"; break;
    case 9:  DEBUG_OUT<<"r9"; break;
    case 10:  DEBUG_OUT<<"r10"; break;
    case 11:  DEBUG_OUT<<"r11"; break;
    case 12:  DEBUG_OUT<<"r12"; break;
    case 13:  DEBUG_OUT<<"r13"; break;
    case 14:  DEBUG_OUT<<"r14"; break;
    case 15:  DEBUG_OUT<<"r15"; break;
    case 16:  DEBUG_OUT<<"r16"; break;
    case 17:  DEBUG_OUT<<"r17"; break;
    case 18:  DEBUG_OUT<<"r18"; break;
    case 19:  DEBUG_OUT<<"r19"; break;
    case 20:  DEBUG_OUT<<"r20"; break;
    case 21:  DEBUG_OUT<<"r21"; break;
    case 22:  DEBUG_OUT<<"r22"; break;
    case 23:  DEBUG_OUT<<"r23"; break;
    case 24:  DEBUG_OUT<<"r24"; break;
    case 25:  DEBUG_OUT<<"r25"; break;
    case 26:  DEBUG_OUT<<"r26"; break;
    case 27:  DEBUG_OUT<<"r27"; break;
    case 28:  DEBUG_OUT<<"r28"; break;
    case 29:  DEBUG_OUT<<"r29"; break;
    case 30:  DEBUG_OUT<<"r30"; break;
    case 31:  DEBUG_OUT<<"r31"; break;
    default: DEBUG_OUT<<"???"; break;
  }
  //w->hex();
}


double EPowerPC::readFloatReg(unsigned int n)
{
  unsigned int rn=n&0x1f;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tRead "<<F[rn]<<" from ";
  //w->dec();
  DEBUG_OUT<<"f"<<rn<<"\n";
  //w->hex();
#endif
  return F[rn];
}


void EPowerPC::writeFloatReg(unsigned int n, double val)
{
  unsigned int rn=n&0x1f;
  F[rn]=val;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tWrite "<<val;
  DEBUG_OUT<<" to f"<<rn<<"\n";
#endif
}


void EPowerPC::printFloatReg(unsigned int num)
{
  unsigned int rn=num&0x1f;
  DEBUG_OUT<<"f";
  DEBUG_OUT<<rn;
}


void EPowerPC::printAddr(unsigned int val)
{
  EMapSymbol *e=map->findAnyExactSymbol(val);
  if(e) DEBUG_OUT<<e->getName();
  else {
    DEBUG_OUT<<"$"<<val;
  }
}


unsigned int EPowerPC::readCR()
{
  debugger("readCR not implemented!\n");
  return CRz;
}


void EPowerPC::writeCR(unsigned int val)
{
  debugger("writeCR not implemented!\n");
  CRz=val;
}


unsigned int EPowerPC::readXER()
{
  return XERz;
}


void EPowerPC::writeXER(unsigned int val)
{
  XERz=val;
}


void EPowerPC::setCRbit(unsigned int bit)
{
#ifdef CHECK_SAFE
  if(bit>31) debugger("Bad bit in setCRbit!\n");
#endif
  bit=31-bit;  // because IBM numbered the bits backwards
  unsigned int theBit=1<<bit;
  //DEBUG_OUT<<"theBit is "<<theBit<<"\n";
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tsetCRbit "<<bit<<"\n";
#endif
  CRz|=theBit;
}


void EPowerPC::clrCRbit(unsigned int bit)
{
#ifdef CHECK_SAFE
  if(bit>31) debugger("Bad bit in clrCRbit!\n");
#endif
  bit=31-bit;  // because IBM numbered the bits backwards
  unsigned int theBit=1<<bit;
  //DEBUG_OUT<<"theBit is "<<theBit<<"\n";
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tclrCRbit "<<bit<<"\n";
#endif
  CRz&=~theBit;
}


bool EPowerPC::getCRbit(unsigned int bit)
{
#ifdef CHECK_SAFE
  if(bit>31) debugger("Bad bit in getCRbit!\n");
#endif
  bit=31-bit;  // because IBM numbered the bits backwards
  unsigned int theBit=1<<bit;
  //DEBUG_OUT<<"theBit is "<<theBit<<"\n";
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tgetCRbit "<<bit<<" (="<<(CRz&theBit)<<")\n";
#endif
  return CRz&theBit;
}


void EPowerPC::setXERbit(unsigned int bit)
{
#ifdef CHECK_SAFE
  if(bit>31) debugger("Bad bit in setXERbit!\n");
#endif
  bit=31-bit;  // because IBM numbered the bits backwards
  unsigned int theBit=1<<bit;
  //DEBUG_OUT<<"theBit is "<<theBit<<"\n";
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tsetXERbit "<<bit<<"\n";
#endif
  XERz|=theBit;
}


void EPowerPC::clrXERbit(unsigned int bit)
{
#ifdef CHECK_SAFE
  if(bit>31) debugger("Bad bit in clrXERbit!\n");
#endif
  bit=31-bit;  // because IBM numbered the bits backwards
  unsigned int theBit=1<<bit;
  //DEBUG_OUT<<"theBit is "<<theBit<<"\n";
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tclrXERbit "<<bit<<"\n";
#endif
  XERz&=~theBit;
}


bool EPowerPC::getXERbit(unsigned int bit)
{
#ifdef CHECK_SAFE
  if(bit>31) debugger("Bad bit in getXERbit!\n");
#endif
  bit=31-bit;  // because IBM numbered the bits backwards
  unsigned int theBit=1<<bit;
  //DEBUG_OUT<<"theBit is "<<theBit<<"\n";
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tgetXERbit "<<bit<<" (="<<(XERz&theBit)<<")\n";
#endif
  return XERz&theBit;
}


// NOTE: be sure to sign extend a and b to 32 bits before calling!
void EPowerPC::setCR_unsigned(unsigned int field,unsigned int a,unsigned int b)
{
#ifdef CHECK_SAFE
  if(field>7) debugger("Bad field in setCR_unsigned(field,a,b)!\n");
#endif // CHECK_SAFE
  //field=7-field;
  if(a==b) setCRbit(EPPC_CR_EQ+(4*field));
  else clrCRbit(EPPC_CR_EQ+(4*field));
  if(a>b) setCRbit(EPPC_CR_GT+(4*field));
  else clrCRbit(EPPC_CR_GT+(4*field));
  if(a<b) setCRbit(EPPC_CR_LT+(4*field));
  else clrCRbit(EPPC_CR_LT+(4*field));
  if(getXERbit(EPPC_XER_SO)) setCRbit(EPPC_CR_SO+(4*field));
  else clrCRbit(EPPC_CR_SO+(4*field));
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tsetCR_unsign("<<field<<","<<a<<","<<b<<")="<<CRz<<"\n";
#endif // DEBUG_EXEC
}


// NOTE: be sure to sign extend a and b to 32 bits before calling!
void EPowerPC::setCR_signed(unsigned int field,int a,int b)
{
#ifdef CHECK_SAFE
  if(field>7) debugger("Bad field in setCR_signed(field,a,b)!\n");
#endif // CHECK_SAFE
  //field=7-field;
  if(a==b) setCRbit(EPPC_CR_EQ+(4*field));
  else clrCRbit(EPPC_CR_EQ+(4*field));
  if(a>b) setCRbit(EPPC_CR_GT+(4*field));
  else clrCRbit(EPPC_CR_GT+(4*field));
  if(a<b) setCRbit(EPPC_CR_LT+(4*field));
  else clrCRbit(EPPC_CR_LT+(4*field));
  if(getXERbit(EPPC_XER_SO)) setCRbit(EPPC_CR_SO+(4*field));
  else clrCRbit(EPPC_CR_SO+(4*field));
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tsetCR_sign("<<field<<","<<a<<","<<b<<")="<<CRz<<"\n";
#endif // DEBUG_EXEC
}


// NOTE: be sure to sign extend a and b to 32 bits before calling!
void EPowerPC::setXER_unsigned(unsigned int a,unsigned int b,unsigned int r)
{
  unsigned int a31=a&ECPUABSTRACT_TOP_LONG;
  unsigned int b31=b&ECPUABSTRACT_TOP_LONG;
  unsigned int r31=r&ECPUABSTRACT_TOP_LONG;
  if((a31&&(!b31)&&(!r31))||((!a31)&&b31&&r31)) didOverflow=true;
  if(didOverflow) setXERbit(EPPC_XER_OV);
  else clrXERbit(EPPC_XER_OV);
  if(didOverflow) setXERbit(EPPC_XER_SO);
  // TODO: SO is NOT cleared here!
  // TODO: set EPPC_XER_CA here
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tsetXER_unsign("<<a<<","<<b<<","<<r<<")="<<XERz<<"\n";
#endif // DEBUG_EXEC
}


// NOTE: be sure to sign extend a and b to 32 bits before calling!
void EPowerPC::setXER_signed(int a,int b,int r)
{
  unsigned int a31=a&ECPUABSTRACT_TOP_LONG;
  unsigned int b31=b&ECPUABSTRACT_TOP_LONG;
  unsigned int r31=r&ECPUABSTRACT_TOP_LONG;
  if((a31&&(!b31)&&(!r31))||((!a31)&&b31&&r31)) didOverflow=true;
  if(didOverflow) setXERbit(EPPC_XER_OV);
  else clrXERbit(EPPC_XER_OV);
  if(didOverflow) setXERbit(EPPC_XER_SO);
  // TODO: SO is NOT cleared here!
  // TODO: set EPPC_XER_CA here
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tsetXER_sign("<<a<<","<<b<<","<<r<<")="<<XERz<<"\n";
#endif // DEBUG_EXEC
}


void EPowerPC::setCR_float(unsigned int field,double a,double b,double r)
{
#ifdef CHECK_SAFE
  if(field>7) debugger("Bad field in setCR_float(field,a,b,r)!\n");
#endif // CHECK_SAFE
  //field=7-field;
  if(a==b) setCRbit(EPPC_CR_EQ+(4*field));
  else clrCRbit(EPPC_CR_EQ+(4*field));
  if(a>b) setCRbit(EPPC_CR_GT+(4*field));
  else clrCRbit(EPPC_CR_GT+(4*field));
  if(a<b) setCRbit(EPPC_CR_LT+(4*field));
  else clrCRbit(EPPC_CR_LT+(4*field));
  // TODO: set EPPC_CR_FU here
  // TODO: set fields from FPSCRz here
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tsetCR_float("<<field<<","<<a<<","<<b<<")="<<CRz<<"\n";
#endif // DEBUG_EXEC
}


// NOTE: this does most of the disasm'ing for the X... instructions
void EPowerPC::disVarious()
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(1);
  bop=(op>>1)&0x3ff;
  switch(top) {
    case 19:
      // All of these are format XL
      opFormat=EPPC_FORMAT_XL;
      bb=(op>>11)&0x1f;
      ba=(op>>16)&0x1f;
      bt=(op>>21)&0x1f;
      lk=op&1;
      switch(bop) {
        case 16: DEBUG_OUT<<"bclr"; if(lk) DEBUG_OUT<<"l"; break; // NOTE: think "rts"
        case 33: DEBUG_OUT<<"crnor"; break;
        case 129: DEBUG_OUT<<"crandc"; break;
        case 193: DEBUG_OUT<<"crxor"; break;
        case 225: DEBUG_OUT<<"crnand"; break;
        case 257: DEBUG_OUT<<"crand"; break;
        case 289: DEBUG_OUT<<"creqv"; break;
        case 417: DEBUG_OUT<<"crorc"; break;
        case 449: DEBUG_OUT<<"cror"; break;
        default:
          //w->dec();
          DEBUG_OUT<<"??? top=19 bop="<<bop;
          debugger();
          //w->hex();
          break;
      }
      switch(bop) {
        case 16: // bclr
          // NOTE: think "rts" when you see bclr
          DEBUG_OUT<<" bo"<<bt<<","<<"bi"<<ba;
          break;
        default:
          DEBUG_OUT<<" cr"<<bt<<","<<"cr"<<ba<<","<<"cr"<<bb;
          break;
      }
      break;
    case 31:
      opFormat=EPPC_FORMAT_UNKNOWN;
      rc=op&1;
      DEBUG_OUT<<EPowerPC_31_name[bop];
      opFormat=EPowerPC_31_format[bop];
      switch(bop) {
        case 0: // cmp
          opFormat=EPPC_FORMAT_D;
          break;
        case 32: // cmpl
          opFormat=EPPC_FORMAT_D;
          break;
        case 266:
          /* DEBUG_OUT<<"add"; */ opFormat=EPPC_FORMAT_XO; oe=(op>>10)&1;
          if(oe) DEBUG_OUT<<"o";
          if(rc) DEBUG_OUT<<".";
          break;
        case 339: /* DEBUG_OUT<<"mfspr"; */ opFormat=EPPC_FORMAT_XFX; break;
        case 444: /* DEBUG_OUT<<"or"; */ opFormat=EPPC_FORMAT_X; if(rc) DEBUG_OUT<<"."; break;
        case 467: /* DEBUG_OUT<<"mtspr"; */ opFormat=EPPC_FORMAT_XFXr; break;
        case 922: /* DEBUG_OUT<<"extsh"; */ opFormat=EPPC_FORMAT_X2; if(rc) DEBUG_OUT<<"."; break;
        case 954: /* DEBUG_OUT<<"extsb"; */ opFormat=EPPC_FORMAT_X2; if(rc) DEBUG_OUT<<"."; break;
        case 986: /* DEBUG_OUT<<"extsw"; */ opFormat=EPPC_FORMAT_X2; if(rc) DEBUG_OUT<<"."; break;
        default:
          //*debugOut->dec();
          DEBUG_OUT<<"??? top=31 bop="<<bop;
          debugger();
          //w->hex();
          break;
      }
      DEBUG_OUT<<" ";
      bb=(op>>11)&0x1f;
      ba=(op>>16)&0x1f;
      bt=(op>>21)&0x1f;
      switch(opFormat) {
        case EPPC_FORMAT_X:
          printReg(ba);  DEBUG_OUT<<",";  printReg(bt);  DEBUG_OUT<<",";  printReg(bb);
          break;
        case EPPC_FORMAT_X2:
          printReg(ba);  DEBUG_OUT<<",";  printReg(bt);
          break;
        case EPPC_FORMAT_XO:
          printReg(bt);  DEBUG_OUT<<",";  printReg(ba);  DEBUG_OUT<<",";  printReg(bb);
          break;
        case EPPC_FORMAT_XFX:
        case EPPC_FORMAT_XFXr:
          spr=(bb<<5)+ba;
          if(opFormat==EPPC_FORMAT_XFX) { printReg(bt); DEBUG_OUT<<","; }
          switch(spr) {
            case 1: DEBUG_OUT<<"xer"; break;
            case 8: DEBUG_OUT<<"lr"; break;
            case 9: DEBUG_OUT<<"ctr"; break;
            default: DEBUG_OUT<<"spr#"<<spr; break;
          }
          if(opFormat==EPPC_FORMAT_XFXr) { DEBUG_OUT<<","; printReg(bt); }
          break;
        default: DEBUG_OUT<<"EPPC_FORMAT_???"; break;
      }
      break;
    default:
      dis_unimplemented(opName);
      break;
  }
  //w->setHexDigits(dig);
}


void EPowerPC::disHelper(const char *oname,unsigned int oformat,unsigned int osubformat)
{
  // we have these local so we don't screw with the global ones...
  unsigned int d,ra,rb,rs,rt,li,bf,me,mb,rc,lk,bi,bo,bd,aa;
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(1);
  switch(oformat) {
    case EPPC_FORMAT_D:
      d=sign_ext(op&0xffff,16);
      ra=(op>>16)&0x1f;
      rt=(op>>21)&0x1f;  // sometimes its really rs
      li=rt&1; // actually just "l"
      bf=(rt>>2)&0x3;
      DEBUG_OUT<<oname<<" ";
      switch(osubformat) {
        case EPPC_SUBFORMAT_CMPI:  // like cmpi
          DEBUG_OUT<<"cr"<<bf; DEBUG_OUT<<","; printReg(ra); DEBUG_OUT<<",#$"<<d;
          break;
        case EPPC_SUBFORMAT_LDST:  // stores and loads
          printReg(rt); DEBUG_OUT<<",#$"<<d<<"("; printReg(ra); DEBUG_OUT<<")";
          break;
        default:  // everything else
          printReg(rt); DEBUG_OUT<<","; printReg(ra); DEBUG_OUT<<",#$"<<d;
          break;
      }
      break;
    case EPPC_FORMAT_B:  // Conditional branches
      bd=sign_ext(op&0xfffc,16);
      lk=op&0x01;
      aa=(op>>1)&0x01;
      bi=(op>>16)&0x1f;
      bo=(op>>21)&0x1f;
      DEBUG_OUT<<oname;
      if(lk) DEBUG_OUT<<"l";
      if(aa) DEBUG_OUT<<"a";
      DEBUG_OUT<<" "<<bo<<","<<bi<<",";
      printAddr(bd);
      DEBUG_OUT<<"  ; ";
      ea=pc+bd;
      printAddr(ea);
      break;
    case EPPC_FORMAT_I:  // Non-cond branches?
      li=sign_ext(op&0x3fffffc,26);
      lk=op&0x01;
      aa=(op>>1)&0x01;
      DEBUG_OUT<<oname;
      if(lk) DEBUG_OUT<<"l";
      if(aa) DEBUG_OUT<<"a";
      DEBUG_OUT<<" ";
      printAddr(li);
      DEBUG_OUT<<"  ; ";
      ea=pc+li;
      printAddr(ea);
      break;
    case EPPC_FORMAT_M:
      rs=(op>>21)&0x1f;
      rb=(op>>11)&0x1f;
      ra=(op>>16)&0x1f;
      mb=(op>>6)&0x1f;
      me=(op>>1)&0x1f;
      rc=op&0x01;
      DEBUG_OUT<<oname;
      if(rc) DEBUG_OUT<<".";
      DEBUG_OUT<<" ";
      printReg(ra); DEBUG_OUT<<","; printReg(rs); DEBUG_OUT<<",";
      if(osubformat==EPPC_SUBFORMAT_IMMED) DEBUG_OUT<<rb;
      else printReg(rb);
      DEBUG_OUT<<","<<mb<<"-"<<me;
      break;
    case EPPC_FORMAT_SC:
      DEBUG_OUT<<oname;
      break;
    case EPPC_FORMAT_VARIOUS:
      disVarious();
      break;
    default:
      dis_unimplemented(oname);
      break;
  }
  //w->setHexDigits(dig);
}


// NOTE: Using PPC bit/field numbering (opposite of usual!)
void EPowerPC::decodeAndCalc()
{
  unsigned int raval=0;
  switch(opFormat) {
    case EPPC_FORMAT_VARIOUS:
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tdecodeAndCalc format VARIOUS\n";
#endif
      break;
    case EPPC_FORMAT_XO:  // TODO: Is this right?
    case EPPC_FORMAT_XS:  // TODO: Is this right?
    case EPPC_FORMAT_XFX:  // TODO: Is this right?
    case EPPC_FORMAT_XFXr:  // TODO: Is this right?
    case EPPC_FORMAT_X:
    case EPPC_FORMAT_X2:
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tdecodeAndCalc format X*\n";
#endif
      ra=(op>>16)&0x1f;
      rb=(op>>11)&0x1f;
      rd=(op>>21)&0x1f;
      rt=rd;
      li=rd&1; // actually just "l"
      bf=(rt>>2)&0x7;
      break;
    case EPPC_FORMAT_XL:
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tdecodeAndCalc format XL\n";
#endif
      bb=(op>>11)&0x1f;
      ba=(op>>16)&0x1f;
      bt=(op>>21)&0x1f;
      lk=op&1;
      break;
    case EPPC_FORMAT_D:
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tdecodeAndCalc format D\n";
#endif
      imm=op&0xffff;  // signed displacement
      ra=(op>>16)&0x1f;
      rd=(op>>21)&0x1f;
      rt=rd;
      li=rd&1; // actually just "l"
      bf=(rt>>2)&0x7;
      // TODO: where does this zero flag get set?
      raval=readReg(ra);
      switch(opSubFormat) {
        case EPPC_SUBFORMAT_CMPI:  // like cmpi
          ea=(int)raval-sign_ext(imm,16);
          break;
        case EPPC_SUBFORMAT_LDST:  // stores and loads
          ea=(int)raval+sign_ext(imm,16);  // rt,#imm,(ra)
          break;
        default:  // everything else
          ea=(int)raval+sign_ext(imm,16);  // rt,ra,#imm
          break;
        break;
      }
      break;
    case EPPC_FORMAT_B:  // Conditional branches
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tdecodeAndCalc format B\n";
#endif
      bd=op&0xfffc;
      lk=op&0x01;
      aa=(op>>1)&0x01;
      bi=(op>>16)&0x1f;
      bo=(op>>21)&0x1f;
      if(aa) ea=sign_ext(bd,16); else ea=sign_ext(bd,16)+pc;
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tsign_ext(bd,16) is "<<sign_ext(bd,16)<<"\n";
      DEBUG_OUT<<"\tea is "<<ea<<"\n";
#endif
      break;
    case EPPC_FORMAT_I:  // Non-cond branches
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tdecodeAndCalc format I\n";
#endif
      li=op&0x3fffffc;
      lk=op&0x01;
      aa=(op>>1)&0x01;
      if(aa) ea=sign_ext(li,26); else ea=sign_ext(li,26)+pc;
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tsign_ext(li,26) is "<<sign_ext(li,26)<<"\n";
      DEBUG_OUT<<"\tea is "<<ea<<"\n";
#endif
      break;
    case EPPC_FORMAT_SC:
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tdecodeAndCalc format SC\n";
#endif
      ea=0;
      break;
    case EPPC_FORMAT_M:
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\tdecodeAndCalc format M\n";
#endif
      rs=(op>>21)&0x1f;
      ra=(op>>16)&0x1f;
      rb=(op>>11)&0x1f;  // can also be "sh" for immediates...
      mb=(op>>6)&0x1f;
      me=(op>>1)&0x1f;
      rc=op&0x1;
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"\trs="<<rs<<" ra="<<ra<<" rb="<<rb<<"\n";
      DEBUG_OUT<<"\tfield is from "<<mb<<" to "<<me<<"\n";
      if(rc) DEBUG_OUT<<"\trc is on\n";
#endif
      break;
    default:
      DEBUG_OUT<<"bad format is "<<opFormat<<" sub is "<<opSubFormat<<"\n";
      debugger("decodeAndCalc, bad format!");
      return;
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tcalc'd ea is "<<ea<<"\n";
#endif
}


void EPowerPC::exec_various()
{
  int topcode=0;
  bop=(op>>1)&0x3ff;
  switch(top) {
    case 19:
      // All of these are format XL
      opFormat=EPPC_FORMAT_XL;
      decodeAndCalc();
      switch(bop) {
        // NOTE: think "rts" when you see bclr
        case 16: exec_bclr(); break;
        case 33: exec_crnor(); break;
        case 129: exec_crandc(); break;
        case 193: exec_crxor(); break;
        case 225: exec_crnand(); break;
        case 257: exec_crand(); break;
        case 289: exec_creqv(); break;
        case 417: exec_crorc(); break;
        case 449: exec_cror(); break;
        default:
          //w->dec();
          DEBUG_OUT<<"exec_various():bop="<<bop<<"?\n"; //w->hex();
          exec_unimplemented("top=19");
          break;
      }
      break;
    case 31:
      opName=EPowerPC_31_name[bop];
      opFormat=EPowerPC_31_format[bop];
      opSubFormat=EPowerPC_31_subformat[bop];
      decodeAndCalc();
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"exec_various():bop=31 opFormat="<<opFormat<<" opSubFormat="<<opSubFormat<<"\n";
#endif
      // TODO: subsume the following decoding into decodeAndCalc...
      rc=op&1;
      bb=(op>>11)&0x1f;
      ba=(op>>16)&0x1f;
      bt=(op>>21)&0x1f;
      oe=(op>>10)&1;
      spr=(bb<<5)+ba;
      spr=(bb<<5)+ba;
      topcode=bop;
#include <cpulib/generated/EPowerPC_31_exec.h>
      break;
    default:
      //w->dec();
      DEBUG_OUT<<"exec_various():top="<<top<<"?\n"; //w->hex();
      exec_unimplemented("exec_various");
      break;
  }
}


void EPowerPC::exec_abs()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_addme()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_addze()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_adde()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_addc()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_addic()
{
  imm=op&0xffff;  // signed immediate
  int b=sign_ext(imm,16);
  rd=(op>>21)&0x1f;
  ra=(op>>16)&0x1f;
  int a=readReg(ra); // NOTE: not 0
  val=a+b;
  writeReg(rd,val);
  // TODO: should these be signed?
  setXER_unsigned(a,b,val);
}


void EPowerPC::exec_addic_()
{
  imm=op&0xffff;  // signed immediate
  int b=sign_ext(imm,16);
  rd=(op>>21)&0x1f;
  ra=(op>>16)&0x1f;
  int a=readReg(ra); // NOTE: not 0
  val=a+b;
  writeReg(rd,val);
  // TODO: should these be signed?
  setCR_unsigned(0,a,b);
  setXER_unsigned(a,b,val);
}


// Format D, subformat 0 (default)
void EPowerPC::exec_addi()
{
  imm=op&0xffff;  // signed immediate
  int b=sign_ext(imm,16);
  rd=(op>>21)&0x1f;
  ra=(op>>16)&0x1f;
  int a=readReg0(ra); // NOTE: 0
  val=a+b;
  writeReg(rd,val);
  // TODO: should these be signed?
  setXER_unsigned(a,b,val);
}


// Format D, subformat 0 (default) ???
void EPowerPC::exec_addis()
{
  imm=sign_ext(op&0xffff,16);
  int b=imm<<16; // signed immediate (shifted)
  rd=(op>>21)&0x1f;
  ra=(op>>16)&0x1f;
  int a=readReg0(ra); // NOTE: 0
  val=a+b;
  writeReg(rd,val);
  // TODO: should these be signed?
  setXER_unsigned(a,b,val);
}


// Format XO
void EPowerPC::exec_add()
{
  int a=readReg(ba); // NOTE: not 0
  int b=readReg(bb); // NOTE: not 0
  val=readReg(ba)+readReg(bb);
  writeReg(bt,val);
  if(rc) exec_unimplemented("add rc=1");
  if(oe) exec_unimplemented("add oe=1");
  // TODO: should these be signed?
  setXER_unsigned(a,b,val);
}


void EPowerPC::exec_and()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_andc()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_andi_()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_andis_()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_bc()
{
  // TODO: make sure these bits are correct...
  bool bo3=bo&0x2,bo2=bo&0x4,bo1=bo&0x8,bo0=bo&0x10;
  bool bit=getCRbit(bi);
  // TODO: Check that all this bo logic is correct!
  if(!bo2) CTR--;
  bool ctrOK=false;
  if(bo3) { if(!CTR) ctrOK=true; } else { if(CTR) ctrOK=true; }
  if(bo2) ctrOK=true;
  bool condOK=false;
  if(bo1) { if(bit) condOK=true; } else { if(!bit) condOK=true; }
  if(bo0) condOK=true;
  branch_address=ea;
  if(ctrOK&&condOK) tookBranch=true;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tbc ctrOK="<<ctrOK<<" condOK="<<condOK<<"\n";
#endif
  if(lk) LR=pc+4;
  if(tookBranch) willJump(branch_address);
}


// Format I
void EPowerPC::exec_b()
{
  branch_address=ea;
  tookBranch=true;
  if(lk) LR=pc+4;
  if(tookBranch) willJump(branch_address);
}


// Format XL
// NOTE: think "rts" when you see bclr
void EPowerPC::exec_bclr()
{
  bo=bt;  bi=ba;
  // TODO: make sure these bits are correct...
  bool bo3=bo&0x2,bo2=bo&0x4,bo1=bo&0x8,bo0=bo&0x10;
  bool bit=getCRbit(bi);
  // TODO: Check that all this bo logic is correct!
  if(!bo2) CTR--;
  bool ctrOK=false;
  if(bo3) { if(!CTR) ctrOK=true; } else { if(CTR) ctrOK=true; }
  if(bo2) ctrOK=true;
  bool condOK=false;
  if(bo1) { if(bit) condOK=true; } else { if(!bit) condOK=true; }
  if(bo0) condOK=true;
  branch_address=LR;
  if(ctrOK&&condOK) tookBranch=true;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tbclr ctrOK="<<ctrOK<<" condOK="<<condOK<<"\n";
#endif
  if(lk) LR=pc+4;
  if(tookBranch) willJump(branch_address);
}


void EPowerPC::exec_clf()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_cli()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_clcs()
{
  exec_unimplemented(opName);
}


// Format X, op31
void EPowerPC::exec_cmp()
{
  if(li) exec_unimplemented("cmp with L set on 32bit PPC!");
  // TODO: should this be a readReg0?
  int a=readReg(ra);
  int b=readReg(rb);
  setCR_signed(bf,a,b);
}


// Format X, op31
void EPowerPC::exec_cmpl()
{
  if(li) exec_unimplemented("cmpl with L set on 32bit PPC!");
  // TODO: should this be a readReg0?
  unsigned int a=readReg(ra);
  unsigned int b=readReg(rb);
  setCR_unsigned(bf,a,b);
}


// Format D, subformat 1 (compares)
void EPowerPC::exec_cmpi()
{
  if(li) exec_unimplemented("cmpi with L set on 32bit PPC!");
  // TODO: should this be a readReg0?
  int a=readReg(ra);
  int b=sign_ext(imm,16);
  setCR_signed(bf,a,b);
}


// Format D, subformat 1 (compares)
void EPowerPC::exec_cmpli()
{
  if(li) exec_unimplemented("cmpli with L set on 32bit PPC!");
  // TODO: should this be a readReg0?
  unsigned int a=readReg(ra);
  unsigned int b=sign_ext(imm,16);
  setCR_unsigned(bf,a,b);
}


// Format XL
void EPowerPC::exec_crnor()
{
  exec_unimplemented("crnor");
}


// Format XL
void EPowerPC::exec_crandc()
{
  exec_unimplemented("crandc");
}


// Format XL
void EPowerPC::exec_crxor()
{
  // bt, ba ,bb
  bool cba=getCRbit(ba);
  bool cbb=getCRbit(bb);
  bool cond=(cba&&cbb)||((!cba)&&(!cbb));
  if(cond) clrCRbit(bt); else setCRbit(bt);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tcrxor cond was "<<cond<<"\n";
#endif
}


// Format XL
void EPowerPC::exec_crnand()
{
  exec_unimplemented("crnand");
}


// Format XL
void EPowerPC::exec_crand()
{
  exec_unimplemented("crand");
}


// Format XL
void EPowerPC::exec_creqv()
{
  exec_unimplemented("creqv");
}


// Format XL
void EPowerPC::exec_crorc()
{
  exec_unimplemented("crorc");
}


// Format XL
void EPowerPC::exec_cror()
{
  exec_unimplemented("cror");
}


void EPowerPC::exec_cntlzd()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_cntlzw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_dcbst()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_dcbf()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_dcbtst()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_dcbt()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_dcbi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_dclst()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_dcbz()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_div()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_divs()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_divdu()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_divwu()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_divd()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_divw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_doz()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_dozi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_eciwx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_ecowx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_eieio()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_eqv()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_extsh()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_extsb()
{
  unsigned int val=readReg(ra)&0xff;
  if(val&0x80) val=val|0xffffff;
  writeReg(rt,val);
}


void EPowerPC::exec_extsw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_isync()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_icbi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lwarx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_ldx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lwzx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_ldux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lwzux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_ldarx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lbzx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lbzux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lscbx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lhzx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lhzux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lwax()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lhax()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lhbrx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lfqx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lfqux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lwaux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lhaux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lswx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lwbrx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lfsx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lfsux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lswi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lfdx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_lfdux()
{
  exec_unimplemented(opName);
}


// Load word and zero
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lwz()
{
  val=read32mem(ea);
  writeReg(rd,val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tread "<<val<<" from "<<ea<<"\n";
#endif // DEBUG_EXEC
}


// Load word and zero with update
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lwzu()
{
  if(!ra) debugger("Bad RA=0 in lwzu\n");
  if(ra==rt) debugger("Bad RA==RT in lwzu\n");
  val=read32mem(ea);
  if(ra) writeReg(ra,ea);
  writeReg(rd,val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tread "<<val<<" from "<<ea<<"\n";
#endif // DEBUG_EXEC
}


// Load byte and zero
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lbz()
{
  val=read8mem(ea);
  writeReg(rd,val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tread "<<val<<" from "<<ea<<"\n";
#endif // DEBUG_EXEC
}


// Load byte and zero with update
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lbzu()
{
  if(!ra) debugger("Bad RA=0 in lbzu\n");
  exec_unimplemented(opName);
}


// Load half word and zero
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lhz()
{
  exec_unimplemented(opName);
}


// Load half word and zero with update
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lhzu()
{
  if(!ra) debugger("Bad RA=0 in lhzu\n");
  exec_unimplemented(opName);
}


// Load half word algebraic
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lha()
{
  exec_unimplemented(opName);
}


// Load half word algebraic with update
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lhau()
{
  if(!ra) debugger("Bad RA=0 in lhau\n");
  exec_unimplemented(opName);
}


// Load multiple word
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lmw()
{
  int n=32-rt;
  for(unsigned int t=0;t<n;t++) {
    unsigned int val=read32mem(ea+(t*4));
    writeReg(t,val);
  }
}


// Load floating point single precision
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lfs()
{
  exec_unimplemented(opName);
}


// Load floating point single precision with update
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lfsu()
{
  decodeAndCalc();
  if(!ra) debugger("Bad RA=0 in lfsu\n");
  exec_unimplemented(opName);
}


// Load floating point single precision
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lfd()
{
  exec_unimplemented(opName);
}


// Load floating point double precision with update
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_lfdu()
{
  if(!ra) debugger("Bad RA=0 in lfdu\n");
  exec_unimplemented(opName);
}


void EPowerPC::exec_maskg()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_maskir()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mcrxr()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mfsrin()
{
  exec_unimplemented(opName);
}


// Format XFX
void EPowerPC::exec_mfspr()
{
  switch(spr) {
    case 1: val=readXER(); break;
    case 8: val=LR; break;
    case 9: val=CTR; break;
    default: exec_unimplemented("bad spr# in mfspr!\n"); break;
  }
  writeReg(bt,val);
}


void EPowerPC::exec_mftb()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mfsri()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mfsr()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mfcr()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mfmsr()
{
  exec_unimplemented(opName);
}


// Format XFXr
void EPowerPC::exec_mtspr()
{
  val=readReg(bt);
  switch(spr) {
    case 1: writeXER(val); break;
    case 8: LR=val; break;
    case 9: CTR=val; break;
    default: exec_unimplemented("bad spr# in mfspr!\n"); break;
  }
}


void EPowerPC::exec_mtsrin()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mtcrf()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mtmsr()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mtsr()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mulli()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mulld()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mullw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mulhdu()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mulhwu()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mulhd()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mulhw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_mul()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_neg()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_nor()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_nand()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_nabs()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_orc()
{
  exec_unimplemented(opName);
}


// Format D
void EPowerPC::exec_ori()
{
  val=readReg(rs);
  val|=(imm&0xffff); // NOTE: not sign extended
  writeReg(ra,val);
}


void EPowerPC::exec_oris()
{
  exec_unimplemented(opName);
}


// Format X
void EPowerPC::exec_or()
{
  // TODO: check readReg vs readReg0
  val=readReg(bt)|readReg(bb);
  writeReg(ba,val);
}


void EPowerPC::exec_rfi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_rlmi()
{
  exec_unimplemented(opName);
}


// Format B
void EPowerPC::exec_rlwimi()
{
// NOTE: rb is actually sh
  exec_unimplemented(opName);
}


// Format M, subformat IMMED
void EPowerPC::exec_rlwinm()
{
// NOTE: rb is actually sh
  val=readReg(rs)<<rb;
  me=31-me;
  mb=31-mb;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\ttranslated me="<<me<<" mb="<<mb<<"\n";
#endif
  if(mb>me) {
    unsigned int temp=me; me=mb; mb=temp;
#ifdef DEBUG_EXEC
    DEBUG_OUT<<"\tFlipped the bits for the mask.\n";
#endif
  }
  unsigned int mask=0;
  for(unsigned int t=mb;t<=me;t++) mask|=(1<<t);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tval="<<val<<" mask="<<mask<<"\n";
#endif
  writeReg(ra,val&mask);
}


void EPowerPC::exec_rlwnm()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sc()
{
  // TODO: get val to pass syscall someway...
  bool ret=false;
  if(io) ret=io->systemCall(0xf);
  if(!ret) {
    DEBUG_OUT<<"SC: system call\n";
    debugger();
  }
}


void EPowerPC::exec_sllq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sleq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_slw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sld()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_slq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sle()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sliq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_slliq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_slbie()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_slbia()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sradi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_srw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sraq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_srea()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sraiq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_srd()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_srq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sre()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sriq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_srlq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sreq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_srliq()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sraw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_srad()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_srawi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stwx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stdux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stwux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_subfze()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stdcx_()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stbx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stbux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sthx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sthux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stswx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stwbrx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stfax()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_rrib()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stfsux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_rac()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stswi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stfdx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stfdux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sthbrx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stfqx()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stfqux()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_stfiwx()
{
  exec_unimplemented(opName);
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_stw()
{
  val=readReg(rd);
  write32mem(ea,val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\twrite "<<val<<" to "<<ea<<"\n";
#endif // DEBUG_EXEC
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_stwu()
{
  if(!ra) debugger("Bad RA=0 in stwu\n");
  //if(ra==rt) debugger("Bad RA==RT in stwu\n");
  val=readReg(rd);
  write32mem(ea,val);
  if(ra) writeReg(ra,ea);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\twrite "<<val<<" to "<<ea<<"\n";
#endif // DEBUG_EXEC
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_stb()
{
  val=readReg(rd)&0xff;
  write8mem(ea,val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\twrite "<<val<<" to "<<ea<<"\n";
#endif // DEBUG_EXEC
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_stbu()
{
  if(!ra) debugger("Bad RA=0 in stbu\n");
  exec_unimplemented(opName);
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_sth()
{
  exec_unimplemented(opName);
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_sthu()
{
  if(!ra) debugger("Bad RA=0 in sthu\n");
  exec_unimplemented(opName);
}


// store multiple word
// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_stmw()
{
  int n=32-rt;
  for(unsigned int t=0;t<n;t++) {
    unsigned int val=readReg(t);
    write32mem(ea+(t*4),val);
  }
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_stfs()
{
  exec_unimplemented(opName);
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_stfsu()
{
  if(!ra) debugger("Bad RA=0 in stfsu\n");
  exec_unimplemented(opName);
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_std()
{
  exec_unimplemented(opName);
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_stfdu()
{
  if(!ra) debugger("Bad RA=0 in stfdu\n");
  exec_unimplemented(opName);
}


// Format D, subformat 2 (loads and stores)
void EPowerPC::exec_sfd()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_subfc()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_subf()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_subfe()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_subfne()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_subfic()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_sync()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_tw()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_td()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_tlbie()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_tlbia()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_tlbsync()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_tdi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_twi()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_xor()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_xori()
{
  exec_unimplemented(opName);
}


void EPowerPC::exec_xoris()
{
  exec_unimplemented(opName);
}


