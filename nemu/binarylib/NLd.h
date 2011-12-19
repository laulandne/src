#ifndef BINARYLIB_NLD_H
#define BINARYLIB_NLD_H


//#include <alib/abaseclass.h>
#include <portable/nickcpp/ANullStream.h>
#include <elib/EModule.h>

#include <portable/nickcpp/AError.h>


class AList;
class NBinaryDecoder;
class NBinaryNode;
class EMemMap;
class DVfs;
class AWindowOutputStream;


// cpu arch codes (same as EM_ values in elf.h from linux)
#define N_LD_ARCH_NONE 0
#define N_LD_ARCH_M32 1
#define N_LD_ARCH_SPARC 2
#define N_LD_ARCH_I386 3
#define N_LD_ARCH_M68K 4
#define N_LD_ARCH_M88K 5
#define N_LD_ARCH_I486 6
#define N_LD_ARCH_I860 7
#define N_LD_ARCH_MIPS 8
#define N_LD_ARCH_AMDAHL 9
#define N_LD_ARCH_MIPSEL 10
#define N_LD_ARCH_RS6000 11
#define N_LD_ARCH_UNKNOWN12 12
#define N_LD_ARCH_UNKNOWN13 13
#define N_LD_ARCH_UNKNOWN14 14
#define N_LD_ARCH_PARISC 15
#define N_LD_ARCH_NCUBE 16
#define N_LD_ARCH_VPP500 17
#define N_LD_ARCH_SPARC32PLUS 18
#define N_LD_ARCH_I960 19
#define N_LD_ARCH_POWERPC 20
#define N_LD_ARCH_POWERPC64 21
#define N_LD_ARCH_S390 22
#define N_LD_ARCH_ARM 40
#define N_LD_ARCH_SH 42
#define N_LD_ARCH_SPARC64 43
#define N_LD_ARCH_H8_300 46
#define N_LD_ARCH_H8_300H 47
#define N_LD_ARCH_H8S 48
#define N_LD_ARCH_H8_500 49
#define N_LD_ARCH_IA64 50
#define N_LD_ARCH_68HC12 53
#define N_LD_ARCH_X86_64 62
#define N_LD_ARCH_68HC16 69
#define N_LD_ARCH_68HC11 70
#define N_LD_ARCH_68HC08 71
#define N_LD_ARCH_68HC05 72
#define N_LD_ARCH_ALPHA 0x9026
#define N_LD_ARCH_JAVA 0xcafe
// I made the following up...
#define N_LD_ARCH_THUMB 0xff00



// "subArch" codes (i.e. which flavor of os)
// (These numbers are completely arbitrary)
#define N_LD_SUBARCH_NONE    0
#define N_LD_SUBARCH_GENERIC 1
#define N_LD_SUBARCH_LINUX   2
#define N_LD_SUBARCH_MACH    3
#define N_LD_SUBARCH_SCO     4
#define N_LD_SUBARCH_AIX     5
#define N_LD_SUBARCH_AUX     6
#define N_LD_SUBARCH_IRIX    7
#define N_LD_SUBARCH_HPUX    8
#define N_LD_SUBARCH_OSF1    9
#define N_LD_SUBARCH_BSD     10
#define N_LD_SUBARCH_SUNOS   11
#define N_LD_SUBARCH_SOLARIS 12
#define N_LD_SUBARCH_GNU     13


class NLd {
public:
  // Public Member Functions
  NLd(EModule *mod);
  virtual ~NLd();
  bool recognize(char *fname);  // check file, but don't decode
  static NBinaryDecoder* pickDecoder(char *binFile,DVfs *tempdos=(DVfs *)NULL);
  bool ld(char *nm);
  bool ld(char *nm,ULONG addr);
  void nm(char *nm=(char *)NULL);
  void reset();
  void setMap(EMemMapAbstract *m) { map=m; }
  void setVfs(DVfs *d) { vfs=d; }
  EMemMapAbstract *getMap() const { return map; }
  DVfs *getVfs() const { return vfs; }
  EModule *getModule() { return module; }
  NBinaryNode *findNodeForAddress(ULONG addr);
  NBinaryNode *findNode(unsigned int num);
  NBinaryNode *findNodeByType(unsigned int num);
  //
  ULONG getEntry() const { return module->getEntry(); }
  void setEntry(ULONG e) { module->setEntry(e); }
  bool getUnknown() const { return module->getUnknown(); }
  void setUnknown(bool u) { module->setUnknown(u); }
  bool getLSB() const { return module->getLSB(); }
  bool getNeedsReloc() const { return module->getNeedsReloc(); }
  void setNeedsReloc(bool r) { module->setNeedsReloc(r); }
  unsigned int getBitness() const { return module->getBitness(); }
  bool getShouldDemandPage() const { return module->getShouldDemandPage(); }
  bool getDynamic() const { return module->getDynamic(); }
  AList *getReloc() const { return module->getReloc(); }
  // Public Data Members
  AError err;
protected:
  // Private Member Functions
  virtual void init();
  bool loadNodes(ULONG imageOffset);
  bool doReloc(NBinaryNode *n);
  EModule *module;
  EMemMapAbstract *map;
  DVfs *vfs;
};


#endif
