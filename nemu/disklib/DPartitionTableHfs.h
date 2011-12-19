#ifndef DLIB_DHFSPARTITIONTABLE_H
#define DLIB_DHFSPARTITIONTABLE_H


#include <disklib/DPartitionTable.h>


class DPartitionTableHfs  : public DPartitionTable
{
public:
  DPartitionTableHfs(DVfs *parent);
  DPartitionTableHfs(DVfs *parent,const char *name);
  virtual ~DPartitionTableHfs();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DPartitionTableHfs"; }
  //
protected:
  virtual void init();
};


#endif

