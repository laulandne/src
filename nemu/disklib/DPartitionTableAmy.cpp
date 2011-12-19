
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DPartitionTableAmy.h>
#include <disklib/DDiskImageAbstract.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  DPartitionTableAmy Class
////////////////////////////////////////////////////////////////////////////////

DPartitionTableAmy::DPartitionTableAmy(DVfs *parent) : DPartitionTable(parent)
{
  DEBUG_OUT<<"DPartitionTableAmy::DPartitionTableAmy()\n";
  init();
  dos=parent;
}


DPartitionTableAmy::DPartitionTableAmy(DVfs *parent,const char *fname) : DPartitionTable(parent,fname)
{
  DEBUG_OUT<<"DPartitionTableAmy::DPartitionTableAmy()\n";
  init();
  dos=parent;
}


DPartitionTableAmy::~DPartitionTableAmy()
{
}


void DPartitionTableAmy::init()
{
  DPartitionTable::init();
}


/* STATIC */
unsigned int DPartitionTableAmy::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  //DEBUG_OUT<<"DPartitionTableAmy::recognize not implemented!\n";
  return ret;
}
