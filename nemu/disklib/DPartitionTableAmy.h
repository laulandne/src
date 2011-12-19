#ifndef DLIB_DAMYPARTITIONTABLE_H
#define DLIB_DAMYPARTITIONTABLE_H


#include <disklib/DPartitionTable.h>


class DPartitionTableAmy  : public DPartitionTable
{
public:
  DPartitionTableAmy(DVfs *parent);
  DPartitionTableAmy(DVfs *parent,const char *name);
  virtual ~DPartitionTableAmy();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DPartitionTableAmy"; }
  //
protected:
  virtual void init();
};


#endif

