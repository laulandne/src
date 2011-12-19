#ifndef ELIB_EMODULE_H
#define ELIB_EMODULE_H


#include <elib/EMemMapAbstract.h>

#include <binarylib/NBinaryNode.h>

#include <portable/nickcpp/AError.h>
#include <portable/nickcpp/AList.h>


class EModule : public EMapSymbol {
public:
  EModule(char *tname="No name");
  virtual ~EModule();
  virtual AList *getNodes() { return &nodes; }
  virtual AList *getSymbols() { return &symbols; }
  virtual AList *getStrings() { return &strings; }
  AList *getReloc() { return &reloc; }
  char *whichCPUType() const { return cpuType; }
  char *whichIOType() const { return ioType; }
  void setCPUType(char *t) { cpuType=t; }
  void setIOType(char *t) { ioType=t; }
  ULONG getEntry() const { return entry; }
  void setEntry(unsigned int e) { entry=e; }
  bool getUnknown() const { return isUnknown; }
  void setUnknown(bool u) { isUnknown=u; }
  bool getLSB() const { return LSB; }
  void setLSB(bool l) { LSB=l; }
  bool getNeedsReloc() const { return needsReloc; }
  void setNeedsReloc(bool r) { needsReloc=r; }
  unsigned int getBitness() const { return bitness; }
  void setBitness(unsigned int b) { bitness=b; }
  bool getShouldDemandPage() const { return shouldDemandPage; }
  void setShouldDemandPage(bool s) { shouldDemandPage=s; }
  bool getDynamic() const { return dynamic; }
  void setDynamic(bool d) { dynamic=d; }
  ULONG getHighestAddressUsed() const { return highestAddressUsed; }
  void setHighestAddressUsed(ULONG t);
  bool getAlreadyUsedZero() const { return alreadyUsedZero; }
  void setAlreadyUsedZero(bool d) { alreadyUsedZero=d; }
  //
  EMapSymbol *lookupStringByNum(unsigned int num);
  EMapSymbol *lookupStringByName(char *s);
  //
  EMapSymbol *findNearestPrevNode(ULONG addr);
  EMapSymbol *lookupNodeByName(char *s);
  EMapSymbol *lookupNodeByNameAndNum(char *s,unsigned int num);
  EMapSymbol *lookupNextNodeByName(char *s);
  //
  EMapSymbol *findExactSymbol(ULONG addr);
  EMapSymbol *findNearestSymbol(ULONG addr);
  EMapSymbol *findNearestPrevSymbol(ULONG addr);
  EMapSymbol *findNearestSymbolOfType(ULONG addr,unsigned int type);
  EMapSymbol *findNearestPrevSymbolOfType(ULONG addr,unsigned int type);
  EMapSymbol *lookupSymbolByName(char *s);
  EMapSymbol *lookupClosestSymbolByName(char *s);
  EMapSymbol *lookupNextSymbolByName(char *s);
  EMapSymbol *lookupNextClosestSymbolByName(char *s);
  EMapSymbol *lookupSymbolByNameAndNum(char *s,unsigned int num);
  EMapSymbol *lookupSymbolByNameAndType(char *s,unsigned int t);
  EMapSymbol *lookupSymbolByNameAndNumAndType(char *s,unsigned int num,unsigned int t);
  EMapSymbol *lookupSymbolByNumAndType(unsigned int num,unsigned int t);
  unsigned int countSymbolWithName(char *s);
  unsigned int countSymbolWithType(unsigned int t);
  //
  void calcAddrAndSizeFromNodes();
  //
  AError err;
protected:
  virtual void init();
  // Private Data Members
  bool alreadyUsedZero;
  ULONG highestAddressUsed;
  ULONG entry;  // Where to jump at start
  bool needsReloc;
  bool isUnknown;  // we don't know what the module is
  bool LSB;
  unsigned int bitness;
  bool dynamic;
  bool shouldDemandPage;
  char *cpuType;
  char *ioType;
  AList reloc;
  AList nodes;
  AList symbols;
  AList strings;
};


#endif
