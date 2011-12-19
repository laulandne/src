
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOAcia.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


////////////////////////////////////////////////////////////////////////////////
//  EIOAcia Class
////////////////////////////////////////////////////////////////////////////////

EIOAcia::EIOAcia(EHandler *parent, ULONG address) : EIOChip(parent,address)
{
  DEBUG_OUT<<"EIOAcia created.\n";
  reset();
}


void EIOAcia::ioregs()
{
  //DEBUG_OUT<<"EIOAcia "<<location<<":\n";
  unsigned int t=0;
  for(t=0;t<EIOACIA_MAX_NUM_PORTS;t++) {
    DEBUG_OUT<<"baud "<<t<<" "<<baud[t]<<":\n";
  }
}


void EIOAcia::io_cycle()
{
}


void EIOAcia::reset()
{
  unsigned int t=0;
  for(t=0;t<memsize;t++) mem[t]=0;
  for(t=0;t<EIOACIA_MAX_NUM_PORTS;t++) {
    baud[t]=0;
    serialBufPtr[t]=0;
  }
}


BYTE EIOAcia::write8Reg(unsigned int reg,BYTE value)
{
  BYTE ret=value;
  debugger("EIOAcia::write8Reg called!");
  return ret;
}


BYTE EIOAcia::read8Reg(unsigned int reg)
{
  BYTE value=0;
  debugger("EIOAcia::read8Reg called!");
  return value;
}


void EIOAcia::setBaud(unsigned int pnum,unsigned int b)
{
  baud[pnum]=b;
}


unsigned int EIOAcia::getBaud(unsigned int pnum)
{
  return baud[pnum];
}


void EIOAcia::writeSerialByte(unsigned int pnum,unsigned int b)
{
  serialBufPtr[pnum]++;
  if(serialBufPtr[pnum]==EIOACIA_BUFFER_SIZE) {
    debugger("EIOAcia::writeSerialByte overflow!\n");
    serialBufPtr[pnum]=EIOACIA_BUFFER_SIZE-1;
  }
  serialBuffer[pnum][serialBufPtr[pnum]]=b;
}


unsigned int EIOAcia::readSerialByte(unsigned int pnum)
{
  BYTE val=serialBuffer[pnum][serialBufPtr[pnum]];
  if(serialBufPtr[pnum]==0) {
    debugger("EIOAcia::readSerialByte underflow!\n");
  }
  else serialBufPtr[pnum]--;
  return val;
}
