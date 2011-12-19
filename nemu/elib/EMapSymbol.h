#ifndef ELIB_EMAPSYMBOL_H
#define ELIB_EMAPSYMBOL_H


#include <portable/nickcpp/AGenericNode.h>

#include <elib/EMemAbstract.h>


// Values for the type field of EMapSymbol
#define EMAP_SYMBOL_NORMAL    0
#define EMAP_SYMBOL_BINNODE   1
#define EMAP_SYMBOL_ENTRY     2
#define EMAP_SYMBOL_VECTOR    3
#define EMAP_SYMBOL_DEBUG     4
#define EMAP_SYMBOL_EXTERN    5
#define EMAP_SYMBOL_RELOC     6
#define EMAP_SYMBOL_FUNC      16
#define EMAP_SYMBOL_INT       17
#define EMAP_SYMBOL_POINTER   18
#define EMAP_SYMBOL_FILENAME  19


// Obviously a work in progress...
class EMapSymbol : public AGenericNode {
public:
  // Public Member Functions
  EMapSymbol();
  EMapSymbol(const char *tname,unsigned int tnum,ULONG taddr,ULONG tsize,unsigned int ttype,ULONG tassoc=0,ULONG tassoc2=0);
  virtual const char *getName();
  virtual const char *getDemangledName();
  virtual void setName(const char *n);
  virtual bool compareName(const char *n);
  // Public data members
//protected:
  virtual void init();
  unsigned int num;   // may be zero, otherwise symbol count
  ULONG addr;  // address where symbol points (after reloc)
  ULONG size;  // size of what symbol points to
  unsigned int type;  // symbol type, or if not strictly a symbol, arbitrary num representing type
  ULONG val; // may be number of node symbol is associated with or any other val
  ULONG val2; // may be number of node symbol is associated with or any other val
  ULONG orig;  // original (pre-reloc) value of symbol
  ULONG misc;  // can be used for any misc value or not used at all
protected:
  const char *name;  // may be NULL
  char *demangled;  // may also be NULL, in fact, only non-NULL if name is mangled
};


#endif
