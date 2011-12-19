#ifndef DLIB_DPARTITIONTABLE_H
#define DLIB_DPARTITIONTABLE_H


#include <disklib/DDiskAbstract.h>


class DPartitionTable  : public DDiskAbstract
{
public:
  DPartitionTable(DVfs *parent);
  DPartitionTable(DVfs *parent,const char *name);
  virtual ~DPartitionTable();
  //static unsigned int recognize(BYTE *tbuf);
  virtual const char *which() { return "DPartitionTable"; }
  virtual void selectPartition(char *name);
  virtual void selectPartition(unsigned int n);
protected:
  virtual void init();
  virtual void readDirectory();
  virtual void readPartitions();
  //
  DDiskAbstract *curPart;
  DDiskAbstract *partitions[10];
};


#endif

