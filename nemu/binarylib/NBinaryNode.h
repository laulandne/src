#ifndef ELIB_NBINARYNODE_H
#define ELIB_NBINARYNODE_H


#include <elib/EMapSymbol.h>


// NBinaryNode types
// Originally based on ELF types...
#define N_BIN_NULL     0
#define N_BIN_PROGBITS 1
#define N_BIN_SYMTAB   2
#define N_BIN_STRTAB   3
#define N_BIN_RELA     4
#define N_BIN_HASH     5
#define N_BIN_DYNAMIC  6
#define N_BIN_NOTE     7
#define N_BIN_NOBITS   8
#define N_BIN_REL      9
#define N_BIN_SHLIB    10
#define N_BIN_DYNSYM   11
#define N_BIN_DEBUG    12


class NBinaryNode : public EMapSymbol {
public:
  NBinaryNode(const char *tname,unsigned int tnum,unsigned int ttype,ULONG taddr,ULONG tsize,ULONG toff, unsigned int tflags,BYTE *tdata=(BYTE *)NULL,ULONG tassoc=0,ULONG tassoc2=0);
  virtual ULONG getOffsetToData() { return offsetToData; }
  virtual unsigned int getPermFlags() { return permFlags; }
  virtual void setAddr(ULONG newAddr) { addr=newAddr; }
  BYTE *getData() { return data; }
protected:
  virtual void init();
  ULONG offsetToData;   // Offset from start of raw data
  unsigned int permFlags; // read, write, exec?
  BYTE *data; // if non-NULL, preloaded data for the node...
};


class NSharedLibrary : public EMapSymbol {
public:
  NSharedLibrary(const char *tname,unsigned int tnum,unsigned int ttype,ULONG taddr,ULONG tsize,ULONG toff, unsigned int tflags,BYTE *tdata=(BYTE *)NULL,ULONG tassoc=0,ULONG tassoc2=0);
protected:
  virtual void init();
};


#endif
