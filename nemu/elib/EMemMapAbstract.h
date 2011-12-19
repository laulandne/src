#ifndef ELIB_EMEMMAPABSTRACT_H
#define ELIB_EMEMMAPABSTRACT_H


#include "EMemAbstract.h"

#include <portable/nickcpp/AList.h>


class EMapSymbol;
class EModule;
class ERom;
class EMemIO;
class EVideoIOAbstract;
class EEmptyMem;
class EHandler;


class EMemMapAbstract : public EMemAbstract {
public:
  // Public member functions
  EMemMapAbstract();
  virtual ~EMemMapAbstract();
  //
  virtual BYTE read8(ULONG addr);
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE reallyWrite8(ULONG addr,BYTE data);
  //
  virtual void touch(ULONG addr);
  virtual unsigned int writePage(ULONG addr,BYTE *data,unsigned int size);
  //
  static ULONG bytes2Bits(ULONG bytes);
  static ULONG bits2Bytes(ULONG bits);
  static ULONG bits2K(ULONG bits);
  //
  virtual AList *getSymbols() { return &symbols; }  // Does NOT include symbols in modules!
  virtual EMapSymbol *findAnyExactSymbol(ULONG addr);  // This searches modules and outside of them
  virtual EMapSymbol *findExactNonModSym(ULONG addr);  // This does not search modules
  virtual EMapSymbol *findAnyNearestPrevSymbol(ULONG where);  // This searches modules and outside of them
  virtual EMapSymbol *lookupSymbolByName(const char *str);  // This does not search modules
  virtual EMapSymbol *lookupNextSymbolByName(const char *str);  // This does not search modules
  //
  virtual EModule *findNearestPrevModule(ULONG where);
  virtual AList *getModules() { return &modules; }
  virtual EModule *getFirstModule();
  virtual EModule *findModule(const char *name);
  virtual void addModule(EModule *m);
  //
  virtual AList *getAreas() { return &areas; }
  virtual EMemAbstract *findArea(const char *name);  // Find first area with that name...
  virtual EMemAbstract *findArea(const char *name,ULONG where);  // Find only area with name and loc
  virtual void addArea(EMemAbstract *foo);
  virtual void removeArea(EMemAbstract *foo);
  //
  virtual bool testMapped(ULONG addr,ULONG sz) { return false; }
  virtual bool testMappedWritable(ULONG addr,ULONG sz) { return false; }
  //
  virtual char getWPageLetter(unsigned int page) { return '?'; };
  virtual char getRPageLetter(unsigned int page) { return '?'; };
  virtual char getVPageLetter(unsigned int page) { return '?'; };
  //
  virtual void map(EMemAbstract *em) {};
  virtual void map(ERom *em) {};
  virtual void map(EMemIO *em) {};
  virtual void mapVideo(EVideoIOAbstract *v) {};
  virtual void unmap(EMemAbstract *em) {};
  //
  virtual void setIOHandler(EHandler *ioh) { ioHandler=ioh; }
  virtual EHandler *getIOHandler() { return ioHandler; }
  //
  virtual const char *getMapType();
  //
  virtual ULONG getAddrBits() const { return addrBits; }
  virtual ULONG getNumPages() const { return numPages; }
  virtual ULONG getAddrMask() const { return addrMask; }
  virtual ULONG getPageSize() const { return pageSize; }
  virtual ULONG getPMask() const { return pMask; }
  virtual ULONG getPShift() const { return pShift; }
  //
  virtual void setPageFaults(bool f) {};
  virtual bool getPageFaults() { return false; }
  virtual void setDemandPaging(bool f) {};
  virtual bool getDemandPaging() { return false; }
  virtual void setVirtualMemory(bool f) {};
  virtual bool getVirtualMemory() { return false; }
  virtual void setVirtualGranularity(unsigned int gran) {};
  virtual bool busError(EMemAbstract *m,ULONG maddr,unsigned int mdata,unsigned int msize,unsigned int maccess) { return false; };
  virtual void resetBusError() {};
  //
  virtual bool getDebug() { return debug; }
  virtual void setDebug(bool t) { debug=t; }
  //
  // Public data members
protected:
  // Private member functions
  virtual void init();
  // Private data members
  EEmptyMem *empty;
  EHandler *ioHandler;
  AList modules;
  AList areas;
  AList symbols; // This is for symbols outside of any module
  bool debug;
  ULONG numPages,pageSize,addrMask;  // NOTE: Don't count on any of these to be valid...
  ULONG pMask,addrBits,pShift;  // NOTE: Don't count on any of these to be valid...
};


#endif
