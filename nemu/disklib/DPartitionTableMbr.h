#ifndef DLIB_DPARTITIONTABLEMBR_H
#define DLIB_DPARTITIONTABLEMBR_H


#include <disklib/DPartitionTable.h>


class DPartitionTableMbr  : public DPartitionTable
{
public:
  DPartitionTableMbr(DVfs *parent);
  DPartitionTableMbr(DVfs *parent,const char *name);
  virtual ~DPartitionTableMbr();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DPartitionTableMbr"; }
  //
protected:
  virtual void init();
};


#endif

