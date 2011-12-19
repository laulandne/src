
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <cpulib/ECPUOperand.h>

#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ECPUOperand Class
////////////////////////////////////////////////////////////////////////////////

ECPUOperand::ECPUOperand()
{
  init();
}


void ECPUOperand::init()
{
  mode=0;  len=0;
  size=0;  val=0;  ea=0;  reg=0;
}

