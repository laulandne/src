#ifndef ELIB_ERESOURCEFILE_H
#define ELIB_ERESOURCEFILE_H

/*

TODO: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


#include <stdio.h>


#include <elib/EResObject.h>
#include <elib/EMacResHeader.h>
#include <elib/EMacResMap.h>
#include <elib/EMacResEntry.h>
#include <elib/EMacResRef.h>

#include <portable/nickcpp/AError.h>


class AList;
class DVfs;


class EResourceFile : public EResObject {
public:
  // Public member functions
  EResourceFile(const char *name,DVfs *dos=(DVfs *)NULL);
  virtual ~EResourceFile();
  AList *getMap() { return resourceMap; }
  bool readFromNum(const char *type,unsigned int num,BYTE *buffer);
  bool readFromID(const char *type,unsigned int id,BYTE *buffer);
  unsigned int getSizeFromNum(const char *type,unsigned int num);
  unsigned int getSizeFromID(const char *type,unsigned int num);
  unsigned int count(const char *type);
  //
  AError err;
protected:
  // private member functions
  void init();
  void printName(long off);
  void printName(char *buf);
  int rdLong(long offset);
  short rdShort(long offset);
  char rdChar(long offset);
  unsigned int rdULong(long offset);
  UINT16 rdUShort(long offset);
  unsigned char rdUChar(long offset);
  bool getRefFromNum(const char *type,unsigned int num);
  bool getRefFromID(const char *type,unsigned int id);
  bool getNameFromNum(const char *type,unsigned int num,BYTE *buf);
  bool getNameFromID(const char *type,unsigned int num,BYTE *buf);
  const char *getCurType();
  unsigned int getCurCount();
  ULONG getCurSize() const { return curSize; }
  long getDataOffset();
  unsigned int getDataSize();
  void readMacEntry();
  void readMacHeader();
  // Private data members
  const char *filename;
  FILE *fd;
  AList *resourceMap;
  AList *typeMap;
  long offset;
  EMacResHeader *macHeader;
  EMacResMap *macMap;
  EMacResEntry *macEnt;
  EMacResRef *macRef;
  unsigned int numTypes;
  char ttype[5];
  unsigned int curNum;
  ULONG curSize;
  DVfs *thedos;
  bool headerRead;
};


#endif
