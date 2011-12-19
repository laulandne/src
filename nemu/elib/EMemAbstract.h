#ifndef ELIB_EMEMABSTRACT_H
#define ELIB_EMEMABSTRACT_H


#include <stdlib.h>
#include <string.h>


#include <portable/nickcpp/AError.h>


// Basic types
typedef char SBYTE;
typedef unsigned char BYTE;
typedef unsigned long ULONG;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef short SINT16;
typedef unsigned short UINT16;
typedef long SINT32;
typedef unsigned long UINT32;


// access sizes
#define E_MEM_BYTE  1
#define E_MEM_SHORT 2
#define E_MEM_LONG  4


// access types
#define E_MEM_READ  1
#define E_MEM_WRITE 2
#define E_MEM_EXEC  4
#define E_MEM_ALL   (E_MEM_READ|E_MEM_WRITE|E_MEM_EXEC)


class EMemAbstract {
public:
  // Public member functions
  EMemAbstract();
  virtual ~EMemAbstract();
  //
  static void *acalloc(unsigned int num,unsigned int size);
  //
  virtual BYTE read8(ULONG addr)=0;
  virtual BYTE write8(ULONG addr,BYTE data)=0;
  //
  virtual BYTE reallyWrite8(ULONG addr,BYTE data);
  virtual unsigned int read16l(ULONG addr);
  virtual unsigned int read16b(ULONG addr);
  virtual unsigned int read16(ULONG addr,bool lsb) { if(lsb) return read16l(addr); else return read16b(addr); }
  virtual unsigned int write16l(ULONG addr,unsigned int data);
  virtual unsigned int write16b(ULONG addr,unsigned int data);
  virtual unsigned int write16(ULONG addr,unsigned int data,bool lsb) { if(lsb) return write16l(addr,data); else return write16b(addr,data); }
  virtual unsigned int read24l(ULONG addr) { return ((read32l(addr))>>8)&0xffffffL; }
  virtual unsigned int read24b(ULONG addr) { return ((read32b(addr))>>8)&0xffffffL; }
  virtual unsigned int read32l(ULONG addr);
  virtual unsigned int read32b(ULONG addr);
  virtual unsigned int read32(ULONG addr,bool lsb) { if(lsb) return read32l(addr); else return read32b(addr); }
  virtual unsigned int write32l(ULONG addr,unsigned int data);
  virtual unsigned int write32b(ULONG addr,unsigned int data);
  virtual unsigned int write32(ULONG addr,unsigned int data,bool lsb) { if(lsb) return write32l(addr,data); else return write32b(addr,data); }
  virtual ULONG read64l(ULONG addr);
  virtual ULONG read64b(ULONG addr);
  virtual ULONG read64(ULONG addr,bool lsb) { if(lsb) return read64l(addr); else return read64b(addr); }
  virtual ULONG write64l(ULONG addr,ULONG data);
  virtual ULONG write64b(ULONG addr,ULONG data);
  virtual ULONG write64(ULONG addr,ULONG data,bool lsb) { if(lsb) return write64l(addr,data); else return write64b(addr,data); }
  //
  virtual void resize(ULONG newsize);
  virtual void resize(ULONG actual,ULONG mapSize) { resize(mapSize); }
  virtual char getLetter() const { return letter; }
  virtual bool reallyAMap();
  virtual bool reallyVideo();
  virtual ULONG getActualSize() const { return memsize; }
  virtual ULONG getSize() const { return memsize; }
  virtual ULONG getLocation() const { return location; }
  virtual char *getName() const { return name; }
  virtual void setLocation(ULONG loc);
  virtual void setLetter(char l) { letter=l; }
  virtual void setName(char *n);
  virtual void clear(ULONG addr,ULONG size);
  virtual void copy(BYTE *data,ULONG addr,ULONG size);
  virtual void copy(EMemAbstract *src,ULONG addr);
  virtual bool isMapped() const { return mapped; }
  virtual void setMapped();
  virtual void setUnmapped();
  virtual void clearBusErrorFlag() { busErrorFlag=false; }
  virtual bool getBusErrorFlag() const { return busErrorFlag; }
  virtual void setWantBusErrors(bool t) { wantBusErrors=t; }
  virtual bool getWantBusErrors() const { return wantBusErrors; }
  virtual bool getShowInMap() const { return showInMap; }
  virtual void setShowInMap(bool t) { showInMap=t; }
  // Public data members
  AError err;
protected:
  // Private member functions
  virtual void init();
  // Private data members
  ULONG memsize;
  ULONG location;
  char letter;
  char *name;
  bool mapped;
  bool busErrorFlag;
  bool wantBusErrors;
  bool showInMap;
};


#endif
