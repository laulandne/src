
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NMachDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>

#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NMachDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NMachDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NMachDecoder looking for signature...\n";
#endif // DEBUG_VERBOSE
  // 32 bit files
  if((buffer[0]==0xfe)&&(buffer[1]==0xed)&&(buffer[2]==0xfa)&&(buffer[3]==0xce))
    return true;
  if((buffer[3]==0xfe)&&(buffer[2]==0xed)&&(buffer[1]==0xfa)&&(buffer[0]==0xce))
    return true;
  // 64 bit files
  if((buffer[0]==0xfe)&&(buffer[1]==0xed)&&(buffer[2]==0xfa)&&(buffer[3]==0xcf))
    return true;
  if((buffer[3]==0xfe)&&(buffer[2]==0xed)&&(buffer[1]==0xfa)&&(buffer[0]==0xcf))
    return true;
  // fat files
  if((buffer[0]==0xca)&&(buffer[1]==0xfe)&&(buffer[2]==0xba)&&(buffer[3]==0xbe)) {
    // Now to check that it isn't a Java classfile
    if((!buffer[8])&&(!buffer[9])) return true;
  }
  if((buffer[3]==0xca)&&(buffer[2]==0xfe)&&(buffer[1]==0xba)&&(buffer[0]==0xbe)) {
    // Now to check that it isn't a Java classfile
    if((!buffer[8])&&(!buffer[9])) return true;
  }
  return false;
}


NMachDecoder::NMachDecoder()
{
  init();
  subArch=N_LD_SUBARCH_MACH;
  guessedEntry=0;
  ncmds=0;
  cmd_size=0;
  filetype=0;
  fatOffset=0;
  curSectNum=0;
}


char *NMachDecoder::whichCPU()
{
  return archString();
}


char *NMachDecoder::whichIO()
{
  return "mach";
}


bool NMachDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Mach verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) { err.set(); return false; }
  bool ret=false;
  ld->reset();
  fatOffset=0;
  if((binmem[0]==0xFE)&&(binmem[1]==0xED)&&(binmem[2]==0xFA)&&(binmem[3]==0xCE))
  {
    ret=true;
    // Probably m68k or PPC...
    arch=N_LD_ARCH_POWERPC;
  }
  if((binmem[3]==0xFE)&&(binmem[2]==0xED)&&(binmem[1]==0xFA)&&(binmem[0]==0xCE))
  {
    LSB=true;
    ret=true;
    // Probably ix86...
    arch=N_LD_ARCH_I386;
  }
  if((binmem[0]==0xFE)&&(binmem[1]==0xED)&&(binmem[2]==0xFA)&&(binmem[3]==0xCF))
  {
    bitness=64;
    ret=true;
    // Probably PPC64...
    arch=N_LD_ARCH_POWERPC;
  }
  if((binmem[3]==0xFE)&&(binmem[2]==0xED)&&(binmem[1]==0xFA)&&(binmem[0]==0xCF))
  {
    LSB=true;  bitness=64;
    ret=true;
    // Probably x86_64...
    arch=N_LD_ARCH_I386;
  }
  if((binmem[0]==0xca)&&(binmem[1]==0xfe)&&(binmem[2]==0xba)&&(binmem[3]==0xbe)) {
  	// Multi-arch, we try to guess native, then the opposite
    // TODO: This only handles two archs, what about others?
    LSB=false;
    fatOffset=findAndSetFatArch(N_LD_ARCH_POWERPC);
    if(!fatOffset) fatOffset=findAndSetFatArch(N_LD_ARCH_I386);
    if(fatOffset) ret=true;
  }
  if((binmem[3]==0xca)&&(binmem[2]==0xfe)&&(binmem[1]==0xba)&&(binmem[0]==0xbe)) {
  	// Multi-arch, we try to guess native, then the opposite
    // TODO: This only handles two archs, what about others?
    LSB=true;
    fatOffset=findAndSetFatArch(N_LD_ARCH_POWERPC);
    if(!fatOffset) fatOffset=findAndSetFatArch(N_LD_ARCH_I386);
    if(fatOffset) ret=true;
  }
  if(!ret) {
    DEBUG_OUT<<"File is not in Mach format!\n";
    return false;
  }
  return ret;
}


// NOTE: Nasty little hacks to get around Endian issues...
unsigned int NMachDecoder::findAndSetFatArch(unsigned int desiredArch)
{
  DEBUG_OUT<<"findAndSetFatArch(desiredArch=="<<desiredArch<<")\n";
  unsigned int ret=0;
  unsigned int n=binmem[4];
  if(!n) n=binmem[7];
  DEBUG_OUT<<"Fat file has "<<n<<" archs in it.\n";
  for(unsigned int t=0;t<n;t++) {
    // struct is: type subtype offset size align, 20 bytes
    unsigned int skip=(t*20)+8;
    unsigned int type=binmem[skip+0];
    if(!type) type=binmem[skip+3];
    unsigned int a=translateMcharch(type);
    ULONG off1=binmem[skip+8];
    if(!off1) off1=binmem[skip+11];
    ULONG off2=binmem[skip+9];
    if(!off2) off2=binmem[skip+10];
    ULONG off=(off2*256)+off1;
    unsigned int size=binmem[skip+12];
    if(!size) size=binmem[skip+15];
    DEBUG_OUT<<"Arch #"<<t<<" arch="<<a<<" type="<<type<<" off="<<off<<" size="<<size<<"\n";
    if(desiredArch==a) {
      DEBUG_OUT<<"Found desired arch type "<<desiredArch<<"!\n";
      ret=off;
      if(desiredArch==N_LD_ARCH_I386) LSB=true; else LSB=false; // kludge
    }
  }
  return ret;
}


bool NMachDecoder::decodeNodes()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Mach decodeNodes...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) {
    err.set();
    return false;
  }
  if(!readHeader()) return false;
  curSectNum=1;
  if(!readAllMachNodes()) return false;
  return true;
}


bool NMachDecoder::readHeader()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"readMachHeader...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) {
    err.set();
    return false;
  }
  if(fatOffset) offset=fatOffset;
  else offset=0;
  DEBUG_OUT<<"Offset to header is "<<offset<<"\n";
  unsigned int magic,cpu,cpu_sub,flags;
  magic=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  cpu=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  if(cpu&0x01000000) bitness=64; else bitness=32;
  cpu&=0xffffff;
  cpu_sub=map->read32(offset+getImageOffset(),LSB);  offset+=4;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"MACH";
  if(bitness==64) DEBUG_OUT<<"64"; else DEBUG_OUT<<"32";
  DEBUG_OUT<<" binary at "<<(ULONG)getImageOffset()<<"\n";
#endif // DEBUG_VERBOSE
  filetype=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  ncmds=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  cmd_size=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  flags=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  if(bitness==64) offset+=4;  // padding for 64-bit format
  arch=translateMcharch(cpu);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"cpu is "<<archString()<<"\n";
  //DEBUG_OUT<<"cpu_sub is "<<cpu_sub<<"\n";
  DEBUG_OUT<<"filetype is ";
#endif // DEBUG_VERBOSE
  switch(filetype) {
    case 1:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"obj file";
#endif // DEBUG_VERBOSE
      break;
    case 2:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"executable file";
#endif // DEBUG_VERBOSE
      break;
    case 3:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"fixed vm shared library file";
#endif // DEBUG_VERBOSE
      break;
    case 4:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"core file";
#endif // DEBUG_VERBOSE
      break;
    case 5:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"preload executable file";
#endif // DEBUG_VERBOSE
      break;
    case 6:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"dyn shared lib file";
#endif // DEBUG_VERBOSE
      break;
    case 7:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"dyn ld file";
#endif // DEBUG_VERBOSE
      break;
    case 8:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"dyn bundle file";
#endif // DEBUG_VERBOSE
      break;
    case 9:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"dyn shared lib stub file";
#endif // DEBUG_VERBOSE
      break;
	default:
      DEBUG_OUT<<"unknown MACH format ("<<filetype<<")\n";
      err.set();
      return false;
      break;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"ncmds is "<<ncmds<<"\n";
  DEBUG_OUT<<"cmd_size is "<<cmd_size<<"\n";
  DEBUG_OUT<<"flags is "<<flags;
#endif // DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
  if(flags&0x01) DEBUG_OUT<<" (noUndef'd)";
  if(flags&0x02) DEBUG_OUT<<" (incLink)";
  if(flags&0x04) DEBUG_OUT<<" (dynLink)";
  if(flags&0x08) DEBUG_OUT<<" (bindAtLoad)";
  if(flags&0x10) DEBUG_OUT<<" (prebound)";
  if(flags&0x20) DEBUG_OUT<<" (split)";
  if(flags&0x40) DEBUG_OUT<<" (lazyInit)";
  if(flags&0x80) DEBUG_OUT<<" (twolevel)";
  if(flags&0x100) DEBUG_OUT<<" (forceFlat)";
  if(flags&0x200) DEBUG_OUT<<" (noMultiDefs)";
  if(flags&0x400) DEBUG_OUT<<" (noFixPrebind)";
  if(flags&0x800) DEBUG_OUT<<" (prebindable)";
  if(flags&0x1000) DEBUG_OUT<<" (allModsBound)";
  if(flags&0x2000) DEBUG_OUT<<" (safeSubs)";
  if(flags&0x4000) DEBUG_OUT<<" (unprebound)";
  if(flags&0x8000) DEBUG_OUT<<" (weakExts)";
  if(flags&0x10000) DEBUG_OUT<<" (weakSyms)";
  if(flags&0x20000) DEBUG_OUT<<" (stackExe)";
  DEBUG_OUT<<"\n";
#endif // DEBUG_VERBOSE
  ld->setUnknown(false);
  return true;
}


bool NMachDecoder::readAllMachNodes()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"readAllMachNodes...\n";
#endif // DEBUG_VERBOSE
  if(ncmds>1000) ncmds=1000;  // Sanity check...
  if(!checkSanity()) {
    err.set();
    return false;
  }
  bool reading=true;
  while(reading) {
    if(!readMachNode()) return false;
    ncmds--;
    if(!ncmds) reading=false;
  }
  return true;
}


bool NMachDecoder::readMachNode()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"(NMachDecoder::readMachNode)\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) {
    err.set();
    return false;
  }
  unsigned int oldOffset=offset,cmd,size;
  cmd=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  size=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  unsigned int ctype=N_BIN_DEBUG,caddr=oldOffset,csize=size,coff=oldOffset;  // Should fatOffset be in here someplace?
  NBinaryNode *n=(NBinaryNode *)NULL;
  cmd&=0x7fffffff;  // strip off "LC_REQ_DYLD" flag if any
  char *cname="Node";
  switch(cmd) {
    case 0x01:  // LC_SEGMENT
      // common
      ctype=N_BIN_PROGBITS;
      n=handleSegment(coff,32);
      break;
    case 0x02:  // LC_SYMTAB
      // common
      //ctype=N_BIN_SYMTAB;
      //n=new NBinaryNode(".symtab"),0,ctype,caddr,csize,coff,E_MEM_ALL);
      handleSymtab(coff);
      break;
    case 0x03:  // LC_SYMSEG
      ctype=N_BIN_SYMTAB;
      n=new NBinaryNode(".symseg",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x04:  // LC_THREAD
      //n=new NBinaryNode(".thread"),0,ctype,caddr,csize,coff,E_MEM_ALL);
      handleThreadCommand(coff,false);
      break;
    case 0x05:  // LC_UNIXTHREAD
      // common
      //n=new NBinaryNode(".unixthread"),0,ctype,caddr,csize,coff,E_MEM_ALL);
      handleThreadCommand(coff,true);
      break;
    case 0x06:  // LC_LOADFVMLIB
      n=new NBinaryNode(".loadfvmlib",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x07:  // LC_IDFVMLIB
      n=new NBinaryNode(".idfvmlib",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x08:  // LC_IDENT
      n=new NBinaryNode(".ident",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x09:  // LC_FVMFILE
      n=new NBinaryNode(".fvmfile",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x0a:  // LC_PREPAGE
      n=new NBinaryNode(".prepage",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x0b:  // LC_DYSYMTAB
      // common
      //ctype=N_BIN_SYMTAB;
      //n=new NBinaryNode(".dysymtab"),0,ctype,caddr,csize,coff,E_MEM_ALL);
      handleDysymtab(coff);
      break;
    case 0x0c:  // LC_LOAD_DYLIB
      // common
      //n=new NBinaryNode(".load_dylib"),0,ctype,caddr,csize,coff,E_MEM_ALL);
      DEBUG_OUT<<"Found .load_dynlib: ";
      debugLcStr(caddr);
      DEBUG_OUT<<"\n";
      break;
    case 0x0d:  // LC_ID_DYLIB
      n=new NBinaryNode(".id_dylib",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x0e:  // LC_LOAD_DYLINKER
      // common
      //n=new NBinaryNode(".load_dynlinker"),0,ctype,caddr,csize,coff,E_MEM_ALL);
      DEBUG_OUT<<"Found .load_dynld: ";
      debugLcStr(caddr);
      DEBUG_OUT<<"\n";
      break;
    case 0x0f:  // LC_ID_DYLINKER
      //n=new NBinaryNode(".id_dylinker"),0,ctype,caddr,csize,coff,E_MEM_ALL);
      DEBUG_OUT<<"Found .dynld_ident: ";
      debugLcStr(caddr);
      DEBUG_OUT<<"\n";
      break;
    case 0x10:  // LC_PREBOUND_DYLIB
      n=new NBinaryNode(".prebound_dylib",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x11:  // LC_ROUTINES
      n=new NBinaryNode(".routines",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x12:  // LC_SUB_FRAMEWORK
      n=new NBinaryNode(".sub_framework",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x13:  // LC_SUB_UMBRELLA
      n=new NBinaryNode(".sub_umbrella",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x14:  // LC_SUB_CLIENT
      n=new NBinaryNode(".sub_client",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x15:  // LC_SUB_LIBRARY
      n=new NBinaryNode(".sub_library",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x16: // LC_TWOLEVEL_HINTS
      // common
      n=new NBinaryNode(".twolevel_hints",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x17:  // LC_PREBIND_CKSUM
      n=new NBinaryNode(".prebind_cksum",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x18:  // LC_LOAD_WEAK_DYLIB
      n=new NBinaryNode(".load_weak_dylib",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x19: // LC_SEGMENT_64
      ctype=N_BIN_PROGBITS;
      n=handleSegment(coff,64);
      break;
    case 0x1a:  // LC_ROUTINES_64
      n=new NBinaryNode(".routines_64",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
    case 0x1b:  // LC_UUID
      n=new NBinaryNode(".uuid",0,ctype,caddr,csize,coff,E_MEM_ALL);
      break;
	default:
      DEBUG_OUT<<"unknown load command ("<<cmd<<")";
      break;
  }
  if(!n) {
    //n=new NBinaryNode(cname,0,ctype,caddr,csize,coff,E_MEM_ALL);
  }
#ifdef DEBUG_VERBOSE
  if(n) DEBUG_OUT<<n->getName()<<" ("<<size<<" bytes)\n";
#endif // DEBUG_VERBOSE
  if(n) ld->getModule()->getNodes()->append(n);
  //else std::cerr<<"Didn't get NBinaryNode!\n");
  offset=oldOffset+size;
  return true;
}


NBinaryNode *NMachDecoder::handleSegment(ULONG off,unsigned int bits)
{
  DEBUG_OUT<<"handleSegment(off=="<<off<<")\n";
  unsigned int cmd=map->read32(off+getImageOffset(),LSB);  off+=4;
  unsigned int cmdsize=map->read32(off+getImageOffset(),LSB);  off+=4;
  char segname[17];
  segname[16]=0;
  for(unsigned int t=0;t<16;t++) {
    segname[t]=map->read8(off+getImageOffset()+t);
  }
  off+=16;
  ULONG vmaddr,vmsize,fileoff,filesize;
  if(bitness==64) {
    vmaddr=map->read64(off+getImageOffset(),LSB);  off+=8;
    vmsize=map->read64(off+getImageOffset(),LSB);  off+=8;
    fileoff=map->read64(off+getImageOffset(),LSB);  off+=8;
    filesize=map->read64(off+getImageOffset(),LSB);  off+=8;
  }
  else {
    vmaddr=map->read32(off+getImageOffset(),LSB);  off+=4;
    vmsize=map->read32(off+getImageOffset(),LSB);  off+=4;
    fileoff=map->read32(off+getImageOffset(),LSB);  off+=4;
    filesize=map->read32(off+getImageOffset(),LSB);  off+=4;
  }
  unsigned int maxprot=map->read32(off+getImageOffset(),LSB);  off+=4;
  unsigned int initprot=map->read32(off+getImageOffset(),LSB);  off+=4;
  unsigned int nsects=map->read32(off+getImageOffset(),LSB);  off+=4;
  unsigned int flags=map->read32(off+getImageOffset(),LSB);  off+=4;
  DEBUG_OUT<<"cmdsize="<<cmdsize<<" segname="<<segname<<" vmaddr="<<vmaddr;
  DEBUG_OUT<<" vmsize="<<vmsize<<" fileoff="<<fileoff<<" filesize="<<filesize;
  DEBUG_OUT<<" maxprot="<<maxprot<<" initprot="<<initprot<< " nsects="<<nsects;
  DEBUG_OUT<<" flags="<<flags<<" ";
  if(flags&0x01) DEBUG_OUT<<"(highvm) ";
  if(flags&0x02) DEBUG_OUT<<"(fvmlib) ";
  if(flags&0x04) DEBUG_OUT<<"(noreloc) ";
  if(flags&0x08) DEBUG_OUT<<"(protected) ";
  DEBUG_OUT<<"\n";
  NBinaryNode *n=new NBinaryNode(segname,0,N_BIN_PROGBITS,vmaddr,vmsize,fileoff,E_MEM_ALL);
  if(nsects) {
    for(unsigned int t=0;t<nsects;t++) {
      unsigned int newoff=handleSections(off,t);
      off=newoff;
    }
  }
  return n;
}


void NMachDecoder::debugLcStr(ULONG off)
{
  unsigned int len=map->read32(off+getImageOffset()+4,LSB);
  unsigned int ptr=map->read32(off+getImageOffset()+8,LSB);
  unsigned int t=off+ptr;
  char c=map->read8(getImageOffset()+t);
  while(c) {
#ifdef DEBUG_VERBOSE
	DEBUG_OUT<<c;
#endif // DEBUG_VERBOSE
    t++;
    c=map->read8(getImageOffset()+t);
  }
}


unsigned int NMachDecoder::handleSections(ULONG off,unsigned int nsect)
{
  DEBUG_OUT<<"handleSections(off=="<<off<<",nsect=="<<nsect<<")\n";
  unsigned int t;
  char sectname[17];
  sectname[16]=0;
  for(t=0;t<16;t++) {
    sectname[t]=map->read8(off+getImageOffset()+t);
  }
  off+=16;
  char segname[17];
  segname[16]=0;
  for(t=0;t<16;t++) {
    segname[t]=map->read8(off+getImageOffset()+t);
  }
  off+=16;
  ULONG addr,size;
  if(bitness==64) {
    addr=map->read64(off+getImageOffset(),LSB);  off+=8;
    size=map->read64(off+getImageOffset(),LSB);  off+=8;
  }
  else {
    addr=map->read32(off+getImageOffset(),LSB);  off+=4;
    size=map->read32(off+getImageOffset(),LSB);  off+=4;
  }
  ULONG offset=map->read32(off+getImageOffset(),LSB);  off+=4;
  unsigned int align=map->read32(off+getImageOffset(),LSB);  off+=4;
  unsigned int reloff=map->read32(off+getImageOffset(),LSB);  off+=4;
  unsigned int nreloc=map->read32(off+getImageOffset(),LSB);  off+=4;
  unsigned int flags=map->read32(off+getImageOffset(),LSB);  off+=4;
  off+=8;
  if(bitness==64) off+=4;  // padding
  DEBUG_OUT<<"curSectNum="<<curSectNum;
  DEBUG_OUT<<" sectname="<<sectname<<" segname="<<segname<<" addr="<<addr<<" size="<<size;
  DEBUG_OUT<<" offset="<<offset<<" align="<<align<<" reloff="<<reloff<<" nreloc="<<nreloc;
  DEBUG_OUT<<" flags="<<flags;
  unsigned int stype=flags&0xff;
  unsigned int sattrib=flags&0xffffff00;
  DEBUG_OUT<<" stype="<<stype<<" ";
  switch(stype) {
    case 0x0:
      DEBUG_OUT<<"(regular)";
      break;
    case 0x1:
      DEBUG_OUT<<"(zerofill)";
      break;
    case 0x2:
      DEBUG_OUT<<"(cstring_literals)";
      break;
    case 0x3:
      DEBUG_OUT<<"(4byte_literals)";
      break;
    case 0x4:
      DEBUG_OUT<<"(8byte_literals)";
      break;
    case 0x5:
      DEBUG_OUT<<"(literal_pointers)";
      break;
    case 0x6:
      DEBUG_OUT<<"(non_lazy_symbol_pointers)";
      break;
    case 0x7:
      DEBUG_OUT<<"(lazy_symbol_pointers)";
      break;
    case 0x8:
      DEBUG_OUT<<"(symbol_stubs)";
      break;
    case 0x9:
      DEBUG_OUT<<"(mod_init_func_pointers)";
      break;
    case 0xa:
      DEBUG_OUT<<"(mod_term_func_pointers)";
      break;
    case 0xb:
      DEBUG_OUT<<"(coalesced)";
      break;
    case 0xc:
      DEBUG_OUT<<"(gb_zerofill)";
      break;
    case 0xd:
      DEBUG_OUT<<"(interposing)";
      break;
    case 0xe:
      DEBUG_OUT<<"(16byte_literals)";
      break;
    default:
      DEBUG_OUT<<"(unknown)";
      break;
  }
  DEBUG_OUT<<" sattrib="<<sattrib<<" ";
  if(sattrib&0x80000000) DEBUG_OUT<<"(pure_instructions) ";
  if(sattrib&0x40000000) DEBUG_OUT<<"(no_toc) ";
  if(sattrib&0x20000000) DEBUG_OUT<<"(strip_static_syms) ";
  if(sattrib&0x10000000) DEBUG_OUT<<"(no_dead_strip) ";
  if(sattrib&0x08000000) DEBUG_OUT<<"(live_support) ";
  if(sattrib&0x04000000) DEBUG_OUT<<"(self_mod_code) ";
  if(sattrib&0x00000400) DEBUG_OUT<<"(some_instructions) ";
  if(sattrib&0x00000200) DEBUG_OUT<<"(ext_reloc) ";
  if(sattrib&0x00000100) DEBUG_OUT<<"(loc_reloc) ";
  DEBUG_OUT<<"\n";
  if(!strcmp(sectname,"__text")) { if(!guessedEntry) { ld->setEntry(addr); guessedEntry=addr; } }
  if(!strcmp(sectname,"__cstring")) handleStringTable(offset,size);
  NBinaryNode *n=new NBinaryNode(sectname,curSectNum,N_BIN_PROGBITS,addr,size,offset,E_MEM_ALL);
  if(n) ld->getModule()->getNodes()->append(n);
  else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
  curSectNum++;
  return off;
}


unsigned int NMachDecoder::translateMcharch(unsigned int a)
{
  unsigned int ret=0;
  switch(a) {
    case 6:
      ret=N_LD_ARCH_M68K;
      break;
    case 7:
      ret=N_LD_ARCH_I386;
      break;
    case 12:
      ret=N_LD_ARCH_ARM;
      break;
    case 10: case 18:
      ret=N_LD_ARCH_POWERPC;
      break;
    default:
      DEBUG_OUT<<"translateMcharch unknown arch "<<a<<"!\n";
      err.set();
      break;
  }
  return ret;
}


// TODO: We get the entry, how can we communicate the rest of the saved state to nemu?!?
bool NMachDecoder::handleThreadCommand(ULONG off,bool isUnix)
{
  DEBUG_OUT<<"handleThread("<<off<<")\n";
  if(isUnix) DEBUG_OUT<<"This thread is a Unix thread.\n";
  unsigned int cmd=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int len=map->read32(off+getImageOffset(),LSB); off+=4;
  DEBUG_OUT<<"len is "<<len<<"\n";
  unsigned int remaining=len-8;
  bool working=true;
  while(working) {
    unsigned int flavor=map->read32(off+getImageOffset(),LSB); off+=4;
    unsigned int count=map->read32(off+getImageOffset(),LSB); off+=4;
    count=count*4;
    remaining-=8;
    DEBUG_OUT<<"flavor="<<flavor<<" count="<<count<<"\n";
    DEBUG_OUT<<"(remaining is "<<remaining<<")\n";
    DEBUG_OUT<<archString();
    bool done=false;
    switch(flavor) {
      case 1:
        DEBUG_OUT<<" 32-bit thread\n";
        if(arch==N_LD_ARCH_POWERPC) {
          unsigned int srr0=map->read32(off+getImageOffset(),LSB); off+=4;
          unsigned int srr1=map->read32(off+getImageOffset(),LSB); off+=4;
          unsigned int regs[32];
          for(unsigned int t=0;t<32;t++) {
            regs[t]=map->read32(off+getImageOffset(),LSB); off+=4;
          }
          unsigned int cr=map->read32(off+getImageOffset(),LSB); off+=4;
          unsigned int xer=map->read32(off+getImageOffset(),LSB); off+=4;
          unsigned int lr=map->read32(off+getImageOffset(),LSB); off+=4;
          unsigned int ctr=map->read32(off+getImageOffset(),LSB); off+=4;
          unsigned int mq=map->read32(off+getImageOffset(),LSB); off+=4;
          unsigned int vrsave=map->read32(off+getImageOffset(),LSB); off+=4;
          if(!guessedEntry) { ld->setEntry(srr0); guessedEntry=srr0; }
          DEBUG_OUT<<"Setting entry to "<<srr0<<"\n";
          done=true;
        }
        if(arch==N_LD_ARCH_I386) {
          ULONG eax=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG ebx=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG ecx=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG edx=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG edi=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG esi=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG ebp=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG esp=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG ss=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG eflags=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG eip=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG cs=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG ds=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG es=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG fs=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG gs=map->read64(off+getImageOffset(),LSB); off+=8;
          if(!guessedEntry) { ld->setEntry(eip); guessedEntry=eip; }
          DEBUG_OUT<<"Setting entry to "<<eip<<"\n";
          done=true;
        }
        if(arch==N_LD_ARCH_M68K) {
          // TODO
        }
        if(arch==N_LD_ARCH_ARM) {
          // TODO
        }
        break;
      case 5:
        DEBUG_OUT<<" 64-bit thread\n";
        if(arch==N_LD_ARCH_POWERPC) {
          ULONG srr0=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG srr1=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG regs[32];
          for(unsigned int t=0;t<32;t++) {
            regs[t]=map->read64(off+getImageOffset(),LSB); off+=8;
          }
          ULONG cr=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG xer=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG lr=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG ctr=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG vrsave=map->read64(off+getImageOffset(),LSB); off+=8;
          if(!guessedEntry) { ld->setEntry(srr0); guessedEntry=srr0; }
          DEBUG_OUT<<"Setting entry to "<<srr0<<"\n";
          done=true;
        }
        if(arch==N_LD_ARCH_I386) {
          ULONG rax=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rbx=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rcx=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rdx=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rdi=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rsi=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rbp=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rsp=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG r8=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG r9=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG r10=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG r11=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG r12=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG r13=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG r14=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG r15=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rip=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG rflags=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG cs=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG fs=map->read64(off+getImageOffset(),LSB); off+=8;
          ULONG gs=map->read64(off+getImageOffset(),LSB); off+=8;
          if(!guessedEntry) { ld->setEntry(rip); guessedEntry=rip; }
          DEBUG_OUT<<"Setting entry to "<<rip<<"\n";
          done=true;
        }
        break;
      default:
        DEBUG_OUT<<" unknown thread\n";
        break;
    }
    if(!done) { std::cerr<<"We didn't grok the thread state, entry may be wrong!"; exit(5); }
    if(!(remaining-count)) working=false;
  }
  return true;
}


bool NMachDecoder::handleSymtab(ULONG off)
{
  DEBUG_OUT<<"handleSymtab("<<off<<")\n";
  unsigned int cmd=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int size=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int symoff=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nsyms=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int stroff=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int strsize=map->read32(off+getImageOffset(),LSB); off+=4;
  DEBUG_OUT<<"symoff="<<symoff<<" nsyms="<<nsyms<<"\n";
  DEBUG_OUT<<"stroff="<<stroff<<" strsize="<<strsize<<"\n";
  AList *syms=map->getFirstModule()->getSymbols();
  for(unsigned int t=0;t<nsyms;t++) {
    unsigned int n_strx=map->read32(symoff+getImageOffset(),LSB); symoff+=4;
    unsigned int n_type=map->read8(symoff+getImageOffset()); symoff+=1;
    unsigned int n_sect=map->read8(symoff+getImageOffset()); symoff+=1;
    unsigned int n_desc=map->read16(symoff+getImageOffset(),LSB); symoff+=2;
    unsigned int n_value=map->read32(symoff+getImageOffset(),LSB); symoff+=4;
    //
    char *name=NULL;
    EMapSymbol *s=NULL;
    if(n_type&0xe0) {
      DEBUG_OUT<<"N_STAB ";
      switch(n_type) {
        case 0x20:
          // global var
          DEBUG_OUT<<"N_GSYM ";
          s=new EMapSymbol(getStringAtOffset(stroff+n_strx),t,n_value,4,EMAP_SYMBOL_NORMAL);
          break;
        case 0x22: DEBUG_OUT<<"N_FNAME "; break;
        case 0x24:
          // function
          DEBUG_OUT<<"N_FUN ";
          name=getStringAtOffset(stroff+n_strx);
          if(strlen(name)) {
            name[strlen(name)-7]=0;
            s=new EMapSymbol(name,t,n_value,4,EMAP_SYMBOL_NORMAL);
            if(!strcmp(name,"_start")) ld->setEntry(n_value);  // TODO: kludge to find entry
          }
          break;
        case 0x26: DEBUG_OUT<<"N_STSYM "; break;
        case 0x28: DEBUG_OUT<<"N_LCSYM "; break;
        case 0x2e: DEBUG_OUT<<"N_BNSYM "; break;
        case 0x30: DEBUG_OUT<<"N_PC "; break;
        case 0x3c: DEBUG_OUT<<"N_OPT "; break;
        case 0x40: DEBUG_OUT<<"N_RSYM "; break;
        case 0x44: DEBUG_OUT<<"N_SLINE "; break;
        case 0x4e: DEBUG_OUT<<"N_ENSYM"; break;
        case 0x60: DEBUG_OUT<<"N_SSYM "; break;
        case 0x64: DEBUG_OUT<<"N_SO "; break;
        case 0x66: DEBUG_OUT<<"N_OSO "; break;
        case 0x80: DEBUG_OUT<<"N_LSYM "; break;
        case 0x82: DEBUG_OUT<<"N_BINCL "; break;
        case 0x84: DEBUG_OUT<<"N_SOL "; break;
        case 0x86: DEBUG_OUT<<"N_PARAMS "; break;
        case 0x88: DEBUG_OUT<<"N_VERSION "; break;
        case 0x8a: DEBUG_OUT<<"N_OLEVEL "; break;
        case 0xa0: DEBUG_OUT<<"N_PSYM "; break;
        case 0xa2: DEBUG_OUT<<"N_EINCL "; break;
        case 0xa4: DEBUG_OUT<<"N_ENTRY "; break;
        case 0xe0: DEBUG_OUT<<"N_LBRAC "; break;
        case 0xe2: DEBUG_OUT<<"N_EXCL "; break;
        case 0xe4: DEBUG_OUT<<"N_RBRAC "; break;
        case 0xe8: DEBUG_OUT<<"N_ECOML "; break;
        case 0xfe: DEBUG_OUT<<"N_LENG "; break;
        default:
          DEBUG_OUT<<"UNKNOWN ";
          break;
      }
    }
    else {
      if(n_type&0x10) DEBUG_OUT<<"N_PEXT ";
      if(n_type&0x01) DEBUG_OUT<<"N_EXT ";
      // types
      if((n_type&0xe)==0x00) DEBUG_OUT<<"N_UNDF ";
      if((n_type&0xe)==0x02) DEBUG_OUT<<"N_ABS ";
      if((n_type&0xe)==0x0e) DEBUG_OUT<<"N_SECT ";
      if((n_type&0xe)==0x0c) DEBUG_OUT<<"N_PBUD ";
      if((n_type&0xe)==0x0a) DEBUG_OUT<<"N_INDR ";
      //
      if((n_desc&0xf)==0x0) DEBUG_OUT<<"undefined_nonlazy ";
      if((n_desc&0xf)==0x1) DEBUG_OUT<<"undefined_lazy ";
      if((n_desc&0xf)==0x2) DEBUG_OUT<<"defined ";
      if((n_desc&0xf)==0x3) DEBUG_OUT<<"private_defined ";
      if((n_desc&0xf)==0x4) DEBUG_OUT<<"private_undefined_nonlazy ";
      if((n_desc&0xf)==0x5) DEBUG_OUT<<"private_undefined_lazy ";
      //
      if(n_desc&0x10) DEBUG_OUT<<"ref'd_dyn ";
      if(n_desc&0x20) DEBUG_OUT<<"no_dead_strip ";
      if(n_desc&0x40) DEBUG_OUT<<"weak_ref ";
      if(n_desc&0x80) DEBUG_OUT<<"weak_def ";
    }
    if(n_strx) DEBUG_OUT<<getStringAtOffset(stroff+n_strx)<<" ";
    DEBUG_OUT<<" #"<<t<<": n_strx="<<n_strx<<" n_type="<<n_type<<" n_sect="<<n_sect<<" n_desc="<<n_desc<<" n_value="<<n_value<<"\n";
    if((s)&&(syms)) syms->insert(s);
  }
  return true;
}


bool NMachDecoder::handleDysymtab(ULONG off)
{
  DEBUG_OUT<<"handleDysymtab("<<off<<")\n";
  unsigned int cmd=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int size=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int ilocalsym=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nlocalsym=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int iextdefsym=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nextdefsym=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int iundefsym=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nundefsym=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int tocoff=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int ntoc=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int modtaboff=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nmodtab=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int extrefsymoff=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nextrefsyms=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int indirectsymoff=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nindirectsyms=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int extreloff=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nextrel=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int locreloff=map->read32(off+getImageOffset(),LSB); off+=4;
  unsigned int nlocrel=map->read32(off+getImageOffset(),LSB); off+=4;
  DEBUG_OUT<<"ilocalsym="<<ilocalsym<<" nlocalsym="<<nlocalsym<<"\n";
  DEBUG_OUT<<"iextdefsym="<<iextdefsym<<" nextdefsym="<<nextdefsym<<"\n";
  DEBUG_OUT<<"iundefsym="<<iundefsym<<" nundefsym="<<nundefsym<<"\n";
  DEBUG_OUT<<"tocoff="<<tocoff<<" ntoc="<<ntoc<<"\n";
  DEBUG_OUT<<"modtaboff="<<modtaboff<<" nmodtab="<<nmodtab<<"\n";
  DEBUG_OUT<<"extrefsymoff="<<extrefsymoff<<" nextrefsyms="<<nextrefsyms<<"\n";
  DEBUG_OUT<<"indirectsymoff="<<indirectsymoff<<" nindirectsyms="<<nindirectsyms<<"\n";
  DEBUG_OUT<<"extreloff="<<extreloff<<" nextrel="<<nextrel<<"\n";
  DEBUG_OUT<<"locreloff="<<locreloff<<" nlocrel="<<nlocrel<<"\n";
  // TODO: Actually process strings/symbols/etc
  return true;
}


