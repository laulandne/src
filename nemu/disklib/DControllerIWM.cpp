
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DControllerIWM.h>

#include <cpulib/ECPUAbstract.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  DControllerIWM Class
////////////////////////////////////////////////////////////////////////////////

DControllerIWM::DControllerIWM(EHandler *parent,ULONG address) : DControllerAbstract(parent,address)
{
  DEBUG_OUT<<"DControllerIWM::DControllerIWM()\n";
  reset();
}


DControllerIWM::~DControllerIWM()
{
}


void DControllerIWM::ioregs()
{
  DEBUG_OUT<<"DControllerIWM "<<location<<":\n";
  DEBUG_OUT<<"ca0 "<<ca0<<":\n";
  DEBUG_OUT<<"ca1 "<<ca1<<":\n";
  DEBUG_OUT<<"ca2 "<<ca2<<":\n";
  DEBUG_OUT<<"lstrb "<<lstrb<<":\n";
  DEBUG_OUT<<"enable "<<enable<<":\n";
  DEBUG_OUT<<"select "<<select<<":\n";
  DEBUG_OUT<<"q6 "<<q6<<":\n";
  DEBUG_OUT<<"q7 "<<q7<<":\n";
  DControllerAbstract::ioregs();
}


void DControllerIWM::io_cycle()
{
  DControllerAbstract::io_cycle();
}


void DControllerIWM::reset()
{
  DControllerAbstract::reset();
  ca0=false;
  ca1=false;
  ca2=false;
  lstrb=false;
  enable=false;
  select=false;
  q6=false;
  q7=false;
}


BYTE DControllerIWM::write8Reg(unsigned int reg,BYTE data)
{
  BYTE ret=data;
  strobe8Reg(reg);
  return ret;
}


BYTE DControllerIWM::read8Reg(unsigned int reg)
{
  BYTE ret=0;
  strobe8Reg(reg);
  return ret;
}


void DControllerIWM::strobe8Reg(unsigned int reg)
{
  reg&=0xf;
  switch(reg) {
    case 0: ca0=false; break;
    case 1: ca0=true; break;
    case 2: ca1=false; break;
    case 3: ca1=true; break;
    case 4: ca2=false; break;
    case 5: ca2=true; break;
    case 6: lstrb=false; break;
    case 7: lstrb=true; break;
    case 8: enable=false; break;
    case 9: enable=true; break;
    case 10: select=false; break;
    case 11: select=true; break;
    case 12: q6=false; break;
    case 13: q6=true; break;
    case 14: q7=false; break;
    case 15: q7=true; break;
  }
}
