
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NBinaryDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <disklib/DVfs.h>
#include <elib/EResourceFile.h>
#include <elib/EMemMapAbstract.h>
#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/AStringHelper.h>


#define N_DEFAULT_IMAGE_OFFSET  0x60000000


////////////////////////////////////////////////////////////////////////////////
//  NBinaryDecoder Class
////////////////////////////////////////////////////////////////////////////////

NBinaryDecoder::NBinaryDecoder()
{
  init();
}


void NBinaryDecoder::init()
{
  ld=(NLd *)NULL;
  dos=(DVfs *)NULL;
  binmem=(BYTE *)NULL;
  map=(EMemMapAbstract *)NULL;
  binsize=0;
  pagesize=0;
  numPages=0;
  arch=N_LD_ARCH_NONE;
  subArch=N_LD_SUBARCH_NONE;
  fname=(char *)NULL;
  realResource=false;
  LSB=false;
  bitness=32;
  dynamic=false;
  shouldDemandPage=false;
  needsReloc=false;
  tname=strdup("~binxx"); // 'cause its write only...
  imageOffset=N_DEFAULT_IMAGE_OFFSET;
}


bool NBinaryDecoder::checkSanity()
{
  if(!binmem) {
    DEBUG_OUT<<"LD Decoder, no binmem!\n";
    err.set();
    return false;
  }
  return checkSanityNoBinmem();
}


bool NBinaryDecoder::checkSanityNoBinmem()
{
  if(err.error()) {
    DEBUG_OUT<<"checkSanity, errorFlag already set!\n";
    return false;
  }
  if(!ld) {
    DEBUG_OUT<<"checkSanity, No LD!\n";
    err.set();
    return false;
  }
  if(!map) {
    DEBUG_OUT<<"checkSanity, no map!!!\n";
    err.set();
    return false;
  }
  if(map->err.error()) {
    DEBUG_OUT<<"checkSanity, map has error!!!\n";
    err.set();
    return false;
  }
  return true;
}


void NBinaryDecoder::setLD(NLd *l)
{
  ld=l;
  if(ld) {
    dos=ld->getVfs();
    map=ld->getMap();
  }
}


bool NBinaryDecoder::openBinary(char *nm)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"\nopenBinary...\n";
  if(!nm) return false;
#endif // DEBUG_VERBOSE
  if(!checkSanityNoBinmem()) return false;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"checkSanityNoBinmem passed.\n";
#endif // DEBUG_VERBOSE
  unsigned int dig=0;
  binmem=(BYTE *)NULL;
  binsize=0;
  pagesize=map->getPageSize();
  numPages=0;
  arch=N_LD_ARCH_NONE;
  ULONG bytesRead=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"About to Fopen...\n";
#endif // DEBUG_VERBOSE
  FILE *fd=(FILE *)dos->Fopen(nm,"rb");
  if(fd) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Fopen was good, going to seek and tell...\n";
#endif // DEBUG_VERBOSE
    dos->Fseek(fd,0,SEEK_END); binsize=dos->Ftell(fd);
    dos->Fseek(fd,0,SEEK_SET);
    // Because a classic 68k binary may have a zero sized data fork...
    if(!binsize) {
      EResourceFile rsrc(nm,dos);
      unsigned int rcount=rsrc.count("CODE");
      if(rcount) {
        realResource=true;
        for(unsigned int t=0;t<rcount;t++) {
          binsize+=rsrc.getSizeFromNum("CODE",t);
        }
      }
    }
    if(binsize) fname=strdup(nm); else { dos->Fclose(fd); fd=(FILE *)NULL; }
  }
  if(fd) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"File is "<<binsize<<" bytes.\n";
#endif // DEBUG_VERBOSE
    binmem=(BYTE *)malloc(binsize);
    if(!binmem) {
      DEBUG_OUT<<"Couldn't allocate temp bin area!\n";
      dos->Fclose(fd);
      return false;
    }
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Loading file "<<nm<<"\n";
#endif // DEBUG_VERBOSE
    unsigned int t=0;
    BYTE *c=(BYTE *)malloc(pagesize);
    if(c) {
      bool ww=true;
      while(ww) {
        //DEBUG_OUT<<numPages<<": Reading page at offset "<<t<<"...\n";
        bytesRead=dos->Fread(c,1,pagesize,fd);
        if(bytesRead!=pagesize) ww=false;
        memcpy(binmem+t,c,bytesRead);
        t+=bytesRead;  numPages++;
      }
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<t<<" bytes ("<<numPages<<" pages) total.\n";
#endif // DEBUG_VERBOSE
      binsize=t;
      free(c);
      c=(BYTE *)NULL;
    }
    else {
      DEBUG_OUT<<"Couldn't get temp memory!\n";
    }
  }
  if(!fd) {
    DEBUG_OUT<<"Couldn't load "<<nm<<".\n";
  }
  if(fd) {
    dos->Fclose(fd);
  }
  // Make a copy at getImageOffset()
  if(binmem&&binsize) {
    // Let map know what's coming...
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"Going to pVM("<<(ULONG)getImageOffset()<<","<<binsize<<")...\n";
#endif // DEBUG_VERBOSE
    //map->preallocVirtMem(getImageOffset(),binsize);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Got it, going to copy up there...\n";
#endif // DEBUG_VERBOSE
    /*
    for(unsigned int t=0;t<binsize;t++) {
      map->write8((unsigned int)(t+getImageOffset()),binmem[t]);
    }
    */
    // NOTE: we copy full pages, going past the end a bit...
    ULONG doffset=getImageOffset(),soffset=0;
    ULONG left=binsize;
    ULONG wsize=pagesize;
    for(unsigned int n=0;n<numPages;n++) {
      //DEBUG_OUT<<"Copying page "<<n<<"\n";
      if(left<pagesize) wsize=left;
      map->writePage(doffset,binmem+soffset,wsize);
      if(map->err.error()) {
        DEBUG_OUT<<"Map has error...bailing!\n";
        //map->resetError();
        return false;
      }
      doffset+=pagesize;  soffset+=pagesize;
      left-=pagesize;
    }
  }
  return true;
}



bool NBinaryDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"(Default verify)\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  // Hey, anything's alright with us!
  return true;
}


bool NBinaryDecoder::decodeNodes()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"(Default decodeNodes)\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  ld->reset();
  NBinaryNode *n=new NBinaryNode("node",0,N_BIN_PROGBITS,0,binsize,0,E_MEM_ALL);
  if(n) ld->getModule()->getNodes()->append(n);
  else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
  return true;
}


char *NBinaryDecoder::archString()
{
  char *ret=(char *)NULL;
  // TODO: all lowercase due to my_strlwr crashing...
  // TODO: ...when strings are in readonly blocks
  switch(arch) {
    case N_LD_ARCH_NONE: return "none"; break;
    case N_LD_ARCH_M32: return "m32"; break;
    case N_LD_ARCH_SPARC: return "sparc"; break;
    case N_LD_ARCH_I386: return "i386"; break;
    case N_LD_ARCH_M68K: return "m68k"; break;
    case N_LD_ARCH_M88K: return "m88k"; break;
    case N_LD_ARCH_I486: return "i486"; break;
    case N_LD_ARCH_I860: return "i860"; break;
    case N_LD_ARCH_MIPS:
    case N_LD_ARCH_MIPSEL:
      if(LSB) return "mipsel";
      else return "mips";
      break;
    case N_LD_ARCH_AMDAHL: return "amdahl"; break;
    case N_LD_ARCH_RS6000: return "rs6000"; break;
    case N_LD_ARCH_PARISC: return "parisc"; break;
    case N_LD_ARCH_NCUBE: return "ncube"; break;
    case N_LD_ARCH_VPP500: return "vpp500"; break;
    case N_LD_ARCH_SPARC32PLUS: return "sparc32plus"; break;
    case N_LD_ARCH_I960: return "i960"; break;
    case N_LD_ARCH_POWERPC: return "powerpc"; break;
    case N_LD_ARCH_POWERPC64: return "powerpc64"; break;
    case N_LD_ARCH_ARM: return "arm"; break;
    case N_LD_ARCH_THUMB: return "thumb"; break;
    case N_LD_ARCH_SH: return "sh"; break;
    case N_LD_ARCH_SPARC64: return "sparc64"; break;
    case N_LD_ARCH_ALPHA: return "alpha"; break;
    case N_LD_ARCH_JAVA: return "java"; break;
    case N_LD_ARCH_68HC12: return "68hc12"; break;
    case N_LD_ARCH_68HC16: return "68hc16"; break;
    case N_LD_ARCH_68HC11: return "68hc11"; break;
    case N_LD_ARCH_68HC08: return "68hc08"; break;
    case N_LD_ARCH_68HC05: return "68hc05"; break;
    case N_LD_ARCH_IA64: return "ia64"; break;
    case N_LD_ARCH_X86_64: return "x86_64"; break;
    case N_LD_ARCH_H8_300: return "h8_300"; break;
    case N_LD_ARCH_H8_300H: return "h8_300h"; break;
    case N_LD_ARCH_H8S: return "h8s"; break;
    case N_LD_ARCH_H8_500: return "h8_500"; break;
    default:
      DEBUG_OUT<<"NLD::archString unknown was "<<arch<<"\n";
      return "unknown";
      break;
  }
  return ret;
}


char *NBinaryDecoder::subArchString()
{
  char *ret=(char *)NULL;
  // TODO: all lowercase due to my_strlwr crashing...
  // TODO: ...when strings are in readonly blocks
  switch(subArch) {
    case N_LD_SUBARCH_NONE: return "none"; break;
    case N_LD_SUBARCH_GENERIC: return "unix"; break;
    case N_LD_SUBARCH_LINUX: return "linux"; break;
    case N_LD_SUBARCH_MACH: return "mach"; break;
    case N_LD_SUBARCH_SCO: return "sco"; break;
    case N_LD_SUBARCH_AIX: return "aix"; break;
    case N_LD_SUBARCH_AUX: return "aux"; break;
    case N_LD_SUBARCH_IRIX: return "irix"; break;
    case N_LD_SUBARCH_HPUX: return "hpux"; break;
    case N_LD_SUBARCH_OSF1: return "osf1"; break;
    case N_LD_SUBARCH_BSD: return "bsd"; break;
    case N_LD_SUBARCH_SUNOS: return "sunos"; break;
    case N_LD_SUBARCH_SOLARIS: return "solaris"; break;
    case N_LD_SUBARCH_GNU: return "gnu"; break;
    default: return "unknown"; break;
  }
  return ret;
}


char *NBinaryDecoder::getStringAtOffset(ULONG start,NBinaryNode *strtab)
{
  //if(!strtab) return NULL;
  static char tbuf[100];
  start+=getImageOffset();
  if(strtab) start+=strtab->getOffsetToData();
  char c=map->read8(start);
  unsigned int t=0;
  while(c) {
    t++;
    c=map->read8(start+t);
    if(t==256) c=0;  // Arbitrarily cut off at 255 chars...
  }
  ULONG end=start+t;
  ULONG len=end-start;
  for(t=0;t<len;t++) tbuf[t]=map->read8(start+t);
  tbuf[len]=0;
  //DEBUG_OUT<<start<<" "<<tbuf<<"\n";
  return tbuf;
}


void NBinaryDecoder::readGnuStab()
{
}


// TODO: There's still something messed up in here someplace...
unsigned int NBinaryDecoder::handleStringTable(ULONG addr,ULONG size,AList *stdb)
{
  DEBUG_OUT<<"handleStringTable("<<addr<<","<<size<<")\n";
  ULONG start;
  unsigned int t=0;
  char s[256];
  if(!stdb) stdb=map->getFirstModule()->getStrings();
  if(!stdb) return 0;
  bool reading=true;
  start=addr;
  ULONG o=0,c=0;
  ULONG last=0;
  while(reading) {
    s[o]=map->read8(getImageOffset()+start+o);
    o++;  c++;
    if(!s[o-1]) {
      if(o>1) {
        DEBUG_OUT<<"string @"<<last<<" #"<<t<<" "<<strlen(s)<<" "<<s<<"\n";
        EMapSymbol *sym=new EMapSymbol(s,t,start,strlen(s),0);
        stdb->append(sym);
        last=c;
        t++;
        c+=strlen(s);
      }
      start+=o;
      o=0;
    }
    if(c>=size) reading=false;
  }
  DEBUG_OUT<<"There were a total of "<<t<<" strings in the table.\n";
  return t;
}


ULONG NBinaryDecoder::getImageOffset()
{
  return imageOffset;
}

