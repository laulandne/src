
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DPartitionTableHfs.h>
#include <disklib/DDiskImageAbstract.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  DPartitionTableHfs Class
////////////////////////////////////////////////////////////////////////////////

DPartitionTableHfs::DPartitionTableHfs(DVfs *parent) : DPartitionTable(parent)
{
  DEBUG_OUT<<"DPartitionTableHfs::DPartitionTableHfs()\n";
  init();
  dos=parent;
  constructorHelper();
}


DPartitionTableHfs::DPartitionTableHfs(DVfs *parent,const char *fname) : DPartitionTable(parent,fname)
{
  DEBUG_OUT<<"DPartitionTableHfs::DPartitionTableHfs()\n";
  init();
  dos=parent;
  constructorHelper();
}



DPartitionTableHfs::~DPartitionTableHfs()
{
}


void DPartitionTableHfs::init()
{
  DPartitionTable::init();
}


/* STATIC */
unsigned int DPartitionTableHfs::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  //DEBUG_OUT<<"DPartitionTableHfs::recognize not implemented!\n";
  return ret;
}
