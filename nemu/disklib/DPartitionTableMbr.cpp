
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DPartitionTableMbr.h>
#include <disklib/DDiskImageAbstract.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  DPartitionTableMbr Class
////////////////////////////////////////////////////////////////////////////////

DPartitionTableMbr::DPartitionTableMbr(DVfs *parent) : DPartitionTable(parent)
{
  DEBUG_OUT<<"DPartitionTableMbr::DPartitionTableMbr()\n";
  init();
  dos=parent;
  constructorHelper();
}


DPartitionTableMbr::DPartitionTableMbr(DVfs *parent,const char *fname) : DPartitionTable(parent,fname)
{
  DEBUG_OUT<<"DPartitionTableMbr::DPartitionTableMbr()\n";
  init();
  dos=parent;
  constructorHelper();
}


DPartitionTableMbr::~DPartitionTableMbr()
{
}


void DPartitionTableMbr::init()
{
  DPartitionTable::init();
}


/* STATIC */
unsigned int DPartitionTableMbr::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  //DEBUG_OUT<<"DPartitionTableMbr::recognize not implemented!\n";
  return ret;
}
