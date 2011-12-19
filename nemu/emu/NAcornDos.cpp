
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAcornDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/ndecoder/AImageDecoder.h>
#include <portable/ndecoder/ASmartImageDecoder.h>

#include <cpulib/Em6502.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>

#include <disklib/DVfs.h>

#include <portable/nickcpp/AStringHelper.h>

/*

Atom OS calls:
       #FFCB      OSSHUT     JMP (SHTVEC)
       #FFCE      OSFIND     JMP (FNDVEC)
       #FFD1      OSBPUT     JMP (BPTVEC)
       #FFD4      OSBGET     JMP (BGTVEC)
       #FFD7      OSSTAR     JMP (STRVEC)
       #FFDA      OSRDAR     JMP (RDRVEC)
       #FFDD      OSSAVE     JMP (SAVVEC)
       #FFED      OSLOAD     JMP (LODVEC)
       #FFE3      OSRDCH     JMP (RDCVEC)
       #FFE6      OSECHO     JSR  OSRDCH
       #FFE9      OSASCI     CMP  @#0D
       #FFEB                 BNE  OSWRCH
       #FFED      OSCRLF     LDA  @#0A
       #FFEF                 JSR  OSWRCH
       #FFF2                 LDA  @#0D
       #FFF4      OSWRCH     JMP (WRCVEC)
       #FFF7      OSCLI      JMP (COMVEC)

Atom OS vectors:
       #0200      NMIVEC     NMI service routine
       #0202      BRKVEC     BRK service routine
       #0204      IRQVEC     IRQ service routine
       #0206      COMVEC     Command line interpreter
       #0208      WRCVEC     Write character
       #020A      RDCVEC     Read character
       #020C      LODVEC     Load file
       #020E      SAVVEC     Save file
       #0210      RDRVEC     Error
       #0212      STRVEC     Error
       #0214      BGTVEC     Get byte from tape
       #0216      BPTVEC     Put hyte to tape
       #0218      FNDVEC     Print message
       #021A      SHTVEC     Dummy

Atom zero page areas:
Hexadecimal:  Decimal:      Function:
 #0            O            Error number
 #1, #2        1, 2         BASIC line number.
 #s - #c       8 - 12       Random number seed
 #10, 011      16, 17       Pointer to BASIC error handler
 #12           18           Text-space pointer
 #00 - 06F     O - 111      BASIC zero-page workspace
 #70 - 07F     112 - 143    Floating-point workspace
 #80 - #AF     144 - 175    Free
 #BO - 0FF     176 - 255    Cassette system workspace
 #FE           254          Character not sent to printer
 #100 - 013F   256 - 319    Input line buffer
 #140 - g17F   320 - 383    Strinq processing & INPUT statement buffer
 #180 - 01FF   344 - 511    Stack
 #200 - 021B   512 - 539    Operating system vectors
 #21C - 023F   540 - 575    Free
 #240 - g3FF   576 - 1023   BASIC workspace
 #3FE, #3FF    1022, 1023   Address of point-plotting routine

*/

//////////////////////////////////////////////////////////////////////////////
//  NAcornDos Class
//////////////////////////////////////////////////////////////////////////////

NAcornDos::NAcornDos(EHandler *parent) : EOSHelper(parent)
{
  init();
  io=parent;
  reset();
}


NAcornDos::~NAcornDos()
{
  // TODO: free kernal entries
}


void NAcornDos::reset()
{
  EOSHelper::reset();
}


bool NAcornDos::systemCall(ULONG loc)
{
  if(!cpu) {
    reset();
    if(!cpu) {
      DEBUG_OUT<<"NAcornDos Couldn't handle the systemCall, no cpu!\n";
      return false;
    }
  }
  if(!dos) {
    reset();
    if(!dos) {
      DEBUG_OUT<<"NAcornDos Couldn't handle the systemCall, no dos!\n";
       return false;
    }
  }
  EMemMapAbstract *map=dos->getMap();
  if(!map) return false;
  bool ret=false;
  EOSEntry *n=findActual(loc);
  unsigned int original=0;
  if(n) original=n->origAddr;
  if(!original) return false;
  DEBUG_OUT<<"sysCall:"<<aDefaultStringHelper.myHex((unsigned short)original)<<"\n";
  bool doIt=false;
  unsigned int t,s,a;
  switch(original) {
    default:
      DEBUG_OUT<<"***unhandled sys call at "<<aDefaultStringHelper.myHex((unsigned short)loc)<<"!!!\n";
      break;
  }
  //if(!ret) {
    //DEBUG_OUT<<"***fall through sys call at "<<aDefaultStringHelper.myHex((unsigned short)loc)<<"!!!\n";
  //}
  //w->setHexDigits(d);
  if(ret) {
    DEBUG_OUT<<"ret was set!\n";
    if(cpu) cpu->rts();
  }
  else {
    DEBUG_OUT<<"(instead "<<loc<<": "<<(short)n->origByte<<")\n";
    if(cpu) {
      DEBUG_OUT<<"Going to jmp!\n";
      cpu->jump(loc);
      //cpu->executeInstead(n->origByte);
      ret=true;
    }
  }
  return ret;
}



unsigned int NAcornDos::load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes)
{
  DEBUG_OUT<<"NAcornDos::load not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NAcornDos::save(const char *filename,ULONG loc,unsigned int size)
{
  DEBUG_OUT<<"NAcornDos::save not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NAcornDos::loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes)
{
  DEBUG_OUT<<"NAcornDos::loadDirectory not implemented!\n";
  exit(5);
  return 0;
}
