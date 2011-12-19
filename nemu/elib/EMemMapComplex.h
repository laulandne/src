#ifndef ELIB_EMEMMAPCOMPLEX_H
#define ELIB_EMEMMAPCOMPLEX_H


#include <portable/nickcpp/AList.h>


#include "EMemMapAbstract.h"


class ERom;
class EMemIO;
class EEmptyMem;
class EVideoIOAbstract;
class EMapSymbol;
class EModule;


class EMemMapComplex : public EMemMapAbstract {
public:
  // Public Member Functions
  EMemMapComplex(ULONG npages,ULONG psize,ULONG sbits);
  virtual ~EMemMapComplex();
  //
  virtual BYTE read8(ULONG addr);
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE reallyWrite8(ULONG addr,BYTE data);
  //
  virtual bool testMapped(ULONG addr,ULONG sz);
  virtual bool testMappedWritable(ULONG addr,ULONG sz);
  virtual char getWPageLetter(unsigned int page);
  virtual char getRPageLetter(unsigned int page);
  virtual char getVPageLetter(unsigned int page);
  virtual void map(EMemAbstract *em);
  virtual void map(ERom *em);
  virtual void map(EMemIO *em);
  virtual void mapVideo(EVideoIOAbstract *v);
  virtual void unmap(EMemAbstract *em);
  //
  virtual const char *getMapType();
  //
  virtual void setPageFaults(bool f);
  virtual bool getPageFaults() { return pageFaults; }
  virtual void setDemandPaging(bool f);
  virtual bool getDemandPaging() { return demandPaging; }
  virtual void setVirtualMemory(bool f);
  virtual bool getVirtualMemory() { return virtualMemory; }
  virtual void setVirtualGranularity(unsigned int gran) { virtualGran=gran; }
  virtual bool busError(EMemAbstract *m,ULONG maddr,unsigned int mdata,unsigned int msize,unsigned int maccess);
  virtual void resetBusError() { inBusError=false; }
  //
protected:
  // Private Member Functions
  virtual void init();
  void constructorHelper(ULONG npages,ULONG psize,ULONG sbits);
  virtual void setVWrite(ULONG page,EMemAbstract *foo);
  virtual void setSWrite(ULONG page,EMemAbstract *foo);
  virtual void setSRead(ULONG page,EMemAbstract *foo);
  virtual void mapHelper(EMemAbstract *em);
  virtual bool mapNewRam(ULONG maddr);
  virtual void warnVideo();
  virtual void warnAlloc();
  // Private Data Members
  ULONG virtualGran;
  ULONG poolStart;
  EMemAbstract **sReadFunc;
  EMemAbstract **sWriteFunc;
  EMemAbstract **vWriteFunc;
  // Should be set to map with all ram and only ram
  EMemAbstract **ramReadFunc;
  EMemAbstract **ramWriteFunc;
  // These next two are used to save when mapping
  EMemAbstract **origReadFunc;
  EMemAbstract **origWriteFunc;
  // May be used for scratch...WILL be overwritten
  EMemAbstract **saveReadFunc;
  EMemAbstract **saveWriteFunc;
  // May be used for any purpose
  EMemAbstract **snap0ReadFunc;
  EMemAbstract **snap0WriteFunc;
  // May be used for any purpose
  EMemAbstract **snap1ReadFunc;
  EMemAbstract **snap1WriteFunc;
  bool pageFaults;
  bool demandPaging;
  bool virtualMemory;
  bool dontPanic;
  bool inBusError;
};


#endif
