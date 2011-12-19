
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


/*

TODO: look at nemu2.cpp and fix it so you can include different disk
formats as you choose and it'll check them just by the define'd symbol
from the include file...so you don't have to move the check in Mount()
by hand...

*/


#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EResourceFile.h>
#include <elib/EMemMapAbstract.h>

#include <disklib/DVfs.h>
#include <disklib/DPartitionTable.h>
#include <disklib/DDiskImageFile.h>
#include <disklib/DDiskImageMem.h>
#include <disklib/DNativeFilesystem.h>


#ifdef ALIB_HAVE_UNISTD_H
//#include <unistd.h>
#endif // ALIB_HAVE_UNISTD_H

#ifdef ALIB_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef ALIB_HAVE_STAT_H
#include <stat.h>
#endif

#ifdef ALIB_HAVE_DIRENT_H
//#include <dirent.h>
#endif // ALIB_HAVE_DIRENT_H

#ifdef ALIB_HAVE_DIRECT_H
#include <direct.h>
#endif // ALIB_HAVE_DIRECT_H

#ifdef ALIB_HAVE_SYS_DIR_H
//#include <sys/dir.h>
#endif // ALIB_HAVE_SYS_DIR_H


#ifdef ASYS_MAC
#ifndef __GNUC__
#define MAC_USE_MOREFILES 1
#include <IterateDirectory.h>
#endif // __GNUC__
#endif // ASYS_MAC


////////////////////////////////////////////////////////////////////////////////
//  DPartitionTable Class
////////////////////////////////////////////////////////////////////////////////

DPartitionTable::DPartitionTable(DVfs *parent)
{
  DEBUG_OUT<<"DPartitionTable::DPartitionTable()\n";
}


DPartitionTable::DPartitionTable(DVfs *parent,const char *name)
{
  DEBUG_OUT<<"DPartitionTable::DPartitionTable()\n";
}


DPartitionTable::~DPartitionTable()
{
}


void DPartitionTable::init()
{
  curPart=NULL;
  for(unsigned int t=0;t<10;t++) partitions[t]=NULL;
}


void DPartitionTable::readDirectory()
{
  std::cerr<<"DPartitionTable::readDirectory not implemented!\n";
  diskError=true;
}


void DPartitionTable::readPartitions()
{
  std::cerr<<"DPartitionTable::readPartitions not implemented!\n";
  diskError=true;
}


void DPartitionTable::selectPartition(char *name)
{
  std::cerr<<"DPartitionTable::selectPartition not implemented!\n";
  diskError=true;
}


void DPartitionTable::selectPartition(unsigned int n)
{
  std::cerr<<"DPartitionTable::selectPartition not implemented!\n";
  diskError=true;
}
