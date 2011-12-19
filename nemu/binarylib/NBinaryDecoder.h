#ifndef BINARYLIB_NBINARYDECODER_H
#define BINARYLIB_NBINARYDECODER_H




#include <portable/nickcpp/AError.h>

#include <elib/EMemMapAbstract.h>


class NLd;
class NBinaryNode;
class DVfs;
class AList;


// TODO: NOT a child of ADecoder as you might suppose...
class NBinaryDecoder {
public:
  NBinaryDecoder();
  void setLD(NLd *l);
  virtual char *whichCPU() { return (char *)"Unknown"; }
  virtual char *whichIO() { return (char *)"Unknown"; }
  virtual bool verifyOkay();
  virtual bool decodeNodes();
  virtual ULONG getImageOffset();
  bool openBinary(char *nm);
  virtual char *which() { return (char *)"NBinaryDecoder"; }
  unsigned int getBitness() const { return bitness; }
  bool getLSB() const { return LSB; }
  bool getDynamic() const { return dynamic; }
  bool getShouldDemandPage() const { return shouldDemandPage; }
  bool getNeedsReloc() const { return needsReloc; }
  //
  AError err;
protected:
  virtual bool checkSanity();
  virtual bool checkSanityNoBinmem();
  char *archString();
  char *subArchString();
  char *getStringAtOffset(ULONG start,NBinaryNode *strtab=NULL);
  virtual void init();
  void readGnuStab();
  unsigned int handleStringTable(ULONG addr,ULONG size,AList *stdb=NULL);
  //
  NLd *ld;
  EMemMapAbstract *map;
  DVfs *dos;
  BYTE *binmem;
  unsigned int binsize;
  unsigned int pagesize;
  unsigned int numPages;
  unsigned int arch;  // CPU
  unsigned int subArch;  // sometimes a specific flavor of OS, etc
  char *fname;
  bool realResource;  // a hack because we have to help with code resources
  bool LSB;
  unsigned int bitness;
  bool dynamic;
  bool shouldDemandPage;
  bool needsReloc;
  char *tname;
  ULONG offset;
  ULONG imageOffset;
};


#endif
