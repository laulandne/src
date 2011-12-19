
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NExeDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <elib/EMemMapAbstract.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NExeDecoder Class
////////////////////////////////////////////////////////////////////////////////

// Types of EXE files
#define N_LD_EXE_MZ  0
#define N_LD_EXE_NE  1
#define N_LD_EXE_LE  2
#define N_LD_EXE_PE  3
#define N_LD_EXE_LX  4
#define N_LD_EXE_OBJ 5


/* STATIC */
bool NExeDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NExeDecoder looking for MZ signature...\n";
#endif // DEBUG_VERBOSE
  if((buffer[0]=='M')&&(buffer[1]=='Z')) return true;
  // These next ones are guesses from what I've seen
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NExeDecoder looking for OBJ bytes...\n";
#endif // DEBUG_VERBOSE
  if((buffer[0]==0x80)&&(buffer[2]==0x00)) {
    //DEBUG_OUT<<"Found the 0x80 and 0x00, looking at bytes at 1 and 3\n";
    //if((buffer[1]<0x20)&&(buffer[3]==0x20)) return true;
    return true;
  }
  return false;
}


NExeDecoder::NExeDecoder()
{
  init();
  offset=0;  // To secondary header (if any)
  clr=false;
  os=0;
  imageBase=0;
  guessedEntry=0;
}


char *NExeDecoder::whichCPU()
{
  if(clr) return "dotnet";
  return archString();
}


char *NExeDecoder::whichIO()
{
  if(clr) return "windows";
  switch(os) {
    case N_LD_EXE_MZ: return "msdos"; break; // Plain old MZ
    case N_LD_EXE_NE: return "windows"; break; // NE, 16 bit
    case N_LD_EXE_LE: return "win95"; break; // LE, 32 bit VxD
    case N_LD_EXE_PE: return "winnt"; break; // PE, 32 bit
    case N_LD_EXE_LX: return "os2"; break; // LX, 32 bit
    case N_LD_EXE_OBJ: return "msdos"; break; // some intel .obj
    default: return "dos"; break;
  }
  return "dos";
}


bool NExeDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"EXE verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  LSB=true;  // So far all OS's that use EXE's are little-endian...
  arch=N_LD_ARCH_I386;  // assume 386 for starters
  os=N_LD_EXE_MZ;  // assume plain DOS for now
  if((binmem[0]==0x80)&&(binmem[2]==0x00)) {
    //if((binmem[1]<0x20)&&(binmem[3]<0x20)) os=N_LD_EXE_OBJ;
    os=N_LD_EXE_OBJ;
  }
  offset=binmem[0x3c]+(binmem[0x3d]*256);
  if((binmem[offset]=='N')&&(binmem[offset+1]=='E')) os=N_LD_EXE_NE;
  if((binmem[offset]=='L')&&(binmem[offset+1]=='E')) os=N_LD_EXE_LE;
  if((binmem[offset]=='P')&&(binmem[offset+1]=='E')) os=N_LD_EXE_PE;
  if((binmem[offset]=='L')&&(binmem[offset+1]=='X')) os=N_LD_EXE_LX;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"os is "<<os<<", guessing "<<whichIO()<<"\n";
#endif // DEBUG_VERBOSE
  return true;
}


bool NExeDecoder::decodeNodes()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NExeDecoder::decodeNodes...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  switch(os) {
    case N_LD_EXE_MZ: return decodeNodesMZ(); break;
    case N_LD_EXE_NE: return decodeNodesNE(); break;
    case N_LD_EXE_LE: return decodeNodesLE(); break;
    case N_LD_EXE_PE: return decodeNodesPE(); break;
    case N_LD_EXE_LX: return decodeNodesLX(); break;
    case N_LD_EXE_OBJ: return decodeNodesObj(); break;
    default:
      std::cerr<<"NExeDecoder doesn't know that format!\n";  exit(5);
      break;
  }
  if(clr) handleCLR();
  return false;
}


bool NExeDecoder::decodeNodesMZ()
{
  // Plain old MS-DOS executable
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Decoding an MZ .exe file...\n";
#endif // DEBUG_VERBOSE
  offset=0;
  NExe_MZ_Header mz;
  mz.mzMagic=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.lastPageSize=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.totalPages=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.relocItems=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.parasHeader=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.minExtraPara=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.maxExtraPara=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.initalSS=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.initalSP=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.checksum=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.initalIP=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.initalCS=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.relocOffset=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  mz.overlayNum=map->read16(offset+getImageOffset(),LSB);  offset+=2;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"mzMagic="<<mz.mzMagic<<"\n";
  DEBUG_OUT<<"lastPageSize="<<mz.lastPageSize<<"\n";
  DEBUG_OUT<<"totalPages="<<mz.totalPages<<"\n";
  DEBUG_OUT<<"relocItems="<<mz.relocItems<<"\n";
  DEBUG_OUT<<"parasHeader="<<mz.parasHeader<<"\n";
  DEBUG_OUT<<"minExtraPara="<<mz.minExtraPara<<"\n";
  DEBUG_OUT<<"maxExtraPara="<<mz.maxExtraPara<<"\n";
  DEBUG_OUT<<"initalSS="<<mz.initalSS<<"\n";
  DEBUG_OUT<<"initalSP="<<mz.initalSP<<"\n";
  DEBUG_OUT<<"checksum="<<mz.checksum<<"\n";
  DEBUG_OUT<<"initalIP="<<mz.initalIP<<"\n";
  DEBUG_OUT<<"initalCS="<<mz.initalCS<<"\n";
  DEBUG_OUT<<"relocOffset="<<mz.relocOffset<<"\n";
  DEBUG_OUT<<"overlayNum="<<mz.overlayNum<<"\n";
#endif // DEBUG_VERBOSE
  NBinaryDecoder::decodeNodes();
  return true;
}


bool NExeDecoder::decodeNodesObj()
{
  // Intel object file...
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Decoding an OMF .obj file...\n";
#endif // DEBUG_VERBOSE
  ULONG offset=0,noffset=0,t=0;
  ULONG type,size,ntype;
  char *name=NULL;
  bool reading=true;
  while(reading) {
    noffset=offset;
    type=map->read8(offset+getImageOffset());  offset+=1;
    size=map->read16(offset+getImageOffset(),LSB)+3;  offset+=2;
    switch(type) {
      case 0x00:
        reading=false;
        break;
      case 0x80:
        DEBUG_OUT<<"Node "<<t<<": 0x80 THEADR size="<<size<<"\n";
        name="THEADER";
        ntype=N_BIN_NOTE;
        break;
      case 0x8a:
        DEBUG_OUT<<"Node "<<t<<": 0x8a MODEND size="<<size<<"\n";
        name="MODEND";
        ntype=N_BIN_NOTE;
        break;
      case 0x8c:
        DEBUG_OUT<<"Node "<<t<<": 0x8c EXTDEF size="<<size<<"\n";
        name="EXTDEF";
        ntype=N_BIN_NOTE;
        break;
      case 0x90:
        DEBUG_OUT<<"Node "<<t<<": 0x90 PUBDEF size="<<size<<"\n";
        name="PUBDEF";
        ntype=N_BIN_NOTE;
        break;
      case 0x96:
        DEBUG_OUT<<"Node "<<t<<": 0x96 LNAMES size="<<size<<"\n";
        name="LNAMES";
        ntype=N_BIN_NOTE;
        break;
      case 0x98:
        DEBUG_OUT<<"Node "<<t<<": 0x98 SEGDEF size="<<size<<"\n";
        name="SEGDEF";
        ntype=N_BIN_NOTE;
        break;
      case 0x9c:
        DEBUG_OUT<<"Node "<<t<<": 0x9c FIXUPP size="<<size<<"\n";
        name="FIXUPP";
        ntype=N_BIN_NOTE;
        break;
      case 0xa0:
        DEBUG_OUT<<"Node "<<t<<": 0xa0 LEDATA size="<<size<<"\n";
        name="LEDATA";
        ntype=N_BIN_NOTE;
        break;
      case 0xa2:
        DEBUG_OUT<<"Node "<<t<<": 0xa2 LIDATA size="<<size<<"\n";
        name="LIDATA";
        ntype=N_BIN_NOTE;
        break;
      default:
        DEBUG_OUT<<"Node "<<t<<": type="<<type<<" size="<<size<<"\n";
        name="Unknown";
        ntype=N_BIN_NOTE;
        break;
    }
    if(reading) {
      NBinaryNode *n=new NBinaryNode(name,t,ntype,noffset,size,noffset,E_MEM_ALL);
      if(n) ld->getModule()->getNodes()->append(n);
      else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
      t++;
      offset=noffset+size;
    }
  }
  return true;
}


bool NExeDecoder::decodeNodesNE()
{
  // Windows or OS/2 16-bit executable
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Decoding an NE .exe file...\n";
#endif // DEBUG_VERBOSE
  // TODO: need to implement...
  NBinaryDecoder::decodeNodes();
  return true;
}


bool NExeDecoder::decodeNodesPE()
{
  // Windows 32-bit portable executable...
  // rough format:
  //   header
  //   object table
  //   image pages
  //   import info
  //   export info
  //   fixup info
  //   resource info
  //   debug info
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Decoding a PE .exe file...\n";
#endif // DEBUG_VERBOSE
  offset=binmem[0x3c]+(binmem[0x3d]*256);
  USHORT peMagic=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  USHORT isUnknown=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  // COFF header...
  NCOFF_Header coff;
  coff.machine=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  coff.numSections=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  coff.dateStamp=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  coff.symbolsPtr=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  coff.numSym=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  coff.sizeOption=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  coff.charac=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  // ...end of COFF header
  USHORT optionMagic=map->read16(offset+getImageOffset(),LSB);  offset+=2;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"peMagic="<<peMagic<<"\n";
  DEBUG_OUT<<"unknown="<<isUnknown<<"\n";
  DEBUG_OUT<<"(COFF Header starts)\n";
  subArch=0;  arch=0;
  // TODO: Is the endian for the various mips correct?
  // TODO: Should set subArch and LSB and bitness to values using this field
  DEBUG_OUT<<"machine="<<coff.machine<<" ";
  switch(coff.machine) {
    case 0x014c: DEBUG_OUT<<"(i386)"; arch=N_LD_ARCH_I386; break;
    case 0x0160: DEBUG_OUT<<"(mipseb r3000)"; arch=N_LD_ARCH_MIPS; break;
    case 0x0162: DEBUG_OUT<<"(mipsel r3000)"; arch=N_LD_ARCH_MIPSEL; break;
    case 0x0166: DEBUG_OUT<<"(mipsel r4000)"; arch=N_LD_ARCH_MIPSEL; break;
    case 0x0168: DEBUG_OUT<<"(mipsel r10000)"; arch=N_LD_ARCH_MIPSEL; break;
    case 0x0169: DEBUG_OUT<<"(mipsel win ce)"; arch=N_LD_ARCH_MIPSEL; break;
    case 0x0266: DEBUG_OUT<<"(mips16)"; arch=N_LD_ARCH_MIPS; break;
    case 0x0366: DEBUG_OUT<<"(mips with fpu)"; arch=N_LD_ARCH_MIPS; break;
    case 0x0466: DEBUG_OUT<<"(mips16 with fpu)"; arch=N_LD_ARCH_MIPS; break;
    case 0x0184: DEBUG_OUT<<"(alpha)"; arch=N_LD_ARCH_ALPHA; break;
    case 0x0284: DEBUG_OUT<<"(alpha64)"; arch=N_LD_ARCH_ALPHA; break;
    case 0x01f0: DEBUG_OUT<<"(powerpc)"; arch=N_LD_ARCH_POWERPC; break;
    case 0x01a2: DEBUG_OUT<<"(sh3)"; arch=N_LD_ARCH_SH; break;
    case 0x01a4: DEBUG_OUT<<"(sh3e)"; arch=N_LD_ARCH_SH; break;
    case 0x01a6: DEBUG_OUT<<"(sh4)"; arch=N_LD_ARCH_SH; break;
    case 0x0200: DEBUG_OUT<<"(ia64)"; arch=N_LD_ARCH_IA64; break;
    case 0x01c0: DEBUG_OUT<<"(arm)"; arch=N_LD_ARCH_ARM; break;
    case 0x01c2: DEBUG_OUT<<"(thumb)"; arch=N_LD_ARCH_THUMB; break;
    default: DEBUG_OUT<<"(Unknown)"; break;
  }
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"numSections="<<coff.numSections<<"\n";
  DEBUG_OUT<<"dateStamp="<<coff.dateStamp<<"\n";
  DEBUG_OUT<<"symbolsPtr="<<coff.symbolsPtr<<"\n";
  DEBUG_OUT<<"numSym="<<coff.numSym<<"\n";
  DEBUG_OUT<<"sizeOption="<<coff.sizeOption<<"\n";
  DEBUG_OUT<<"charac="<<coff.charac<<" (";
  if(coff.charac&0x1) DEBUG_OUT<<"reloc_strip ";
  if(coff.charac&0x2) DEBUG_OUT<<"executable ";
  if(coff.charac&0x4) DEBUG_OUT<<"linenum_strip ";
  if(coff.charac&0x8) DEBUG_OUT<<"localsyms_strip ";
  if(coff.charac&0x10) DEBUG_OUT<<"aggressive_ws ";
  if(coff.charac&0x20) DEBUG_OUT<<"large_address ";
  if(coff.charac&0x80) DEBUG_OUT<<"little_endian ";
  if(coff.charac&0x100) DEBUG_OUT<<"32_bit ";
  if(coff.charac&0x200) DEBUG_OUT<<"debug_strip ";
  if(coff.charac&0x400) DEBUG_OUT<<"debug_strip ";
  if(coff.charac&0x800) DEBUG_OUT<<"run_from_swap ";
  if(coff.charac&0x1000) DEBUG_OUT<<"system_file ";
  if(coff.charac&0x2000) DEBUG_OUT<<"dll_file ";
  if(coff.charac&0x4000) DEBUG_OUT<<"uni_only ";
  if(coff.charac&0x8000) DEBUG_OUT<<"big_endian ";
  DEBUG_OUT<<")\n";
  DEBUG_OUT<<"(End of COFF header)\n";
  DEBUG_OUT<<"optionMagic="<<optionMagic<<"\n";
#endif // DEBUG_VERBOSE
  bool optionOkay=false;
  if(optionMagic==0x010b) {
    // 32 bit PE
    optionOkay=true; bitness=32;
  }
  if(optionMagic==0x0107) {
    // 32 bit ROM PE
    optionOkay=true; bitness=32;
  }
  if(optionMagic==0x020b) {
    // 64 bit PE
    optionOkay=true; bitness=64;
  }
  imageBase=0;
  NExe_PE_Optional opt;
  if(optionOkay) {
    // PE "optional" header (obj files won't have this)
    opt.linkerVer=map->read8(offset+getImageOffset());  offset+=1;
    opt.linkerRev=map->read8(offset+getImageOffset());  offset+=1;
    opt.sizeCode=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.sizeData=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.sizeBSS=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.addrEntry=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.baseCode=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.baseData=0;
    if(bitness==32) {
      opt.baseData=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    }
    if(bitness==32) {
      imageBase=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    }
    else {
      imageBase=map->read64(offset+getImageOffset(),LSB);  offset+=8;
    }
    opt.sectionAlign=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.fileAlign=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.osVer=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    opt.osRev=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    opt.imVer=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    opt.imRev=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    opt.subVer=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    opt.subRev=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    opt.reserved1=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.sizeImage=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.sizeHeaders=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.checksum=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.subsystem=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    opt.dllChars=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    opt.sizeStackRes=0;  opt.sizeStackCom=0;
    opt.sizeHeapRes=0;  opt.sizeHeapCom=0;
    if(bitness==32) {
      opt.sizeStackRes=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      opt.sizeStackCom=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      opt.sizeHeapRes=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      opt.sizeHeapCom=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    }
    else {
      opt.sizeStackRes=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      opt.sizeStackCom=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      opt.sizeHeapRes=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      opt.sizeHeapCom=map->read64(offset+getImageOffset(),LSB);  offset+=8;
    }
    opt.loaderFlags=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    opt.sizeDataDir=map->read32(offset+getImageOffset(),LSB);  offset+=4;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"(Start of optional header)\n";
    DEBUG_OUT<<"linker version="<<opt.linkerVer<<"."<<opt.linkerRev<<"\n";
    DEBUG_OUT<<"sizeCode="<<opt.sizeCode<<"\n";
    DEBUG_OUT<<"sizeData="<<opt.sizeData<<"\n";
    DEBUG_OUT<<"sizeBSS="<<opt.sizeBSS<<"\n";
    DEBUG_OUT<<"addrEntry="<<opt.addrEntry<<"\n";
    DEBUG_OUT<<"baseCode="<<opt.baseCode<<"\n";
    DEBUG_OUT<<"baseData="<<opt.baseData<<"\n";
    DEBUG_OUT<<"imageBase="<<imageBase<<"\n";
    DEBUG_OUT<<"sectionAlign="<<opt.sectionAlign<<"\n";
    DEBUG_OUT<<"fileAlign="<<opt.fileAlign<<"\n";
    DEBUG_OUT<<"os version="<<opt.osVer<<"."<<opt.osRev<<"\n";
    DEBUG_OUT<<"image version="<<opt.imVer<<"."<<opt.imRev<<"\n";
    DEBUG_OUT<<"sub version="<<opt.subVer<<"."<<opt.subRev<<"\n";
    DEBUG_OUT<<"reserved1="<<opt.reserved1<<"\n";
    DEBUG_OUT<<"sizeImage="<<opt.sizeImage<<"\n";
    DEBUG_OUT<<"sizeHeaders="<<opt.sizeHeaders<<"\n";
    DEBUG_OUT<<"checksum="<<opt.checksum<<"\n";
    DEBUG_OUT<<"subsystem="<<opt.subsystem<<" (";
    switch(opt.subsystem) {
      case 1: DEBUG_OUT<<"native"; break;
      case 2: DEBUG_OUT<<"windows gui"; break;
      case 3: DEBUG_OUT<<"windows cui"; break;
      case 5: DEBUG_OUT<<"os/2"; break;
      case 7: DEBUG_OUT<<"posix"; break;
      case 8: DEBUG_OUT<<"native9x"; break;
      case 9: DEBUG_OUT<<"win ce"; break;
      case 0xa: case 0xb: case 0xc:
        DEBUG_OUT<<"efi"; break;
      default: DEBUG_OUT<<"unknown"; break;
    }
    DEBUG_OUT<<")\n";
    DEBUG_OUT<<"dllChars="<<opt.dllChars<<"\n";
    DEBUG_OUT<<"sizeStackRes="<<opt.sizeStackRes<<"\n";
    DEBUG_OUT<<"sizeStackCom="<<opt.sizeStackCom<<"\n";
    DEBUG_OUT<<"sizeHeapRes="<<opt.sizeHeapRes<<"\n";
    DEBUG_OUT<<"sizeHeapCom="<<opt.sizeHeapCom<<"\n";
    DEBUG_OUT<<"loaderFlags="<<opt.loaderFlags<<"\n";
    DEBUG_OUT<<"sizeDataDir="<<opt.sizeDataDir<<"\n";
    DEBUG_OUT<<"(End of optional header)\n";
#endif // DEBUG_VERBOSE
    DEBUG_OUT<<"The data directory (not showing zero sized entries)\n";
    for(unsigned int t=0;t<opt.sizeDataDir;t++) {
      ULONG ddAddr=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      ULONG ddSize=map->read32(offset+getImageOffset(),LSB);  offset+=4;
#ifdef DEBUG_VERBOSE
      if(ddAddr&&ddSize) {
	      DEBUG_OUT<<t<<" ";
		  switch(t) {
		    case 0:
		      DEBUG_OUT<<"exports";
          // TODO: handle these...
		      break;
		    case 1:
		      DEBUG_OUT<<"imports";
          // TODO: handle these...
		      break;
		    case 2:
		      DEBUG_OUT<<"resources";
          // TODO: handle these...
		      break;
		    case 3:
		      DEBUG_OUT<<"exceptions";
		      break;
		    case 4:
		      DEBUG_OUT<<"security";
		      break;
		    case 5:
		      DEBUG_OUT<<"fixups";
          // TODO: handle these...
		      break;
		    case 6:
		      DEBUG_OUT<<"debug";
          // TODO: handle these...
		      break;
		    case 7:
		      DEBUG_OUT<<"image description";
		      break;
		    case 8:
		      DEBUG_OUT<<"machine specific";
		      break;
		    case 9:
		      DEBUG_OUT<<"thread local storage";
		      break;
		    case 10:
		      DEBUG_OUT<<"load config";
		      break;
		    case 11:
		      DEBUG_OUT<<"bound imports";
		      break;
		    case 12:
		      DEBUG_OUT<<"import addresses";
		      break;
		    case 13:
		      DEBUG_OUT<<"delay import descriptor";
		      break;
		    case 14:
		      DEBUG_OUT<<"common language runtime header";
          clr=true;
          // TODO: handle these...
		      break;
		    case 15:
		      DEBUG_OUT<<"reserved";
		      break;
		    default:
		      DEBUG_OUT<<"unknown";
		      break;
		  }
		  DEBUG_OUT<<" : ddAddr="<<ddAddr<<" ddSize="<<ddSize<<"\n";
		 }
     DEBUG_OUT<<"(End of data directory)\n";
#endif // DEBUG_VERBOSE
	  }
  }
  else {
    DEBUG_OUT<<"PE header optional magic is not valid!\n";
  }
  if(coff.numSections) handleSections(coff.numSections);
  if(!guessedEntry) { ld->setEntry(opt.addrEntry+imageBase);  guessedEntry=opt.addrEntry+imageBase; }
  return true;
}


bool NExeDecoder::decodeNodesLE()
{
  // Windows 32-bit VxD executable...
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Decoding an LE .exe file...\n";
#endif // DEBUG_VERBOSE
  // TODO: need to implement...
  NBinaryDecoder::decodeNodes();
  return true;
}


bool NExeDecoder::decodeNodesLX()
{
  // OS/2 32-bit executable
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Decoding an LX .exe file...\n";
#endif // DEBUG_VERBOSE
  // TODO: need to implement...
  NBinaryDecoder::decodeNodes();
  return true;
}


bool NExeDecoder::handleSections(unsigned int numSections)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"handleSections("<<numSections<<")\n";
#endif // DEBUG_VERBOSE
  for(unsigned int t=0;t<numSections;t++) handleSection(t);
  return true;
}


bool NExeDecoder::handleSection(unsigned int num)
{
  NExe_PE_Section sect;
  sect.sectName[8]=0;
  for(unsigned int t=0;t<8;t++) {
    sect.sectName[t]=map->read8(offset+getImageOffset()); offset++;
  }
  sect.vSize=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  sect.rva=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  sect.pSize=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  sect.pOff=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  sect.rOff=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  sect.lOff=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  sect.rNum=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  sect.lNum=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  sect.flags=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  sect.rva+=imageBase;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Section"<<" name="<<sect.sectName<<" vSize="<<sect.vSize<<" rva="<<sect.rva<<" pSize="<<sect.pSize<<" pOff="<<sect.pOff<<" rOff="<<sect.rOff<<" lOff="<<sect.lOff<<" rNum="<<sect.rNum<<" lNum="<<sect.lNum<<" flags="<<sect.flags<<" (";
  if(sect.flags&0x00000001) DEBUG_OUT<<"tls_scaled ";
  if(sect.flags&0x00000020) DEBUG_OUT<<"code ";
  if(sect.flags&0x00000040) DEBUG_OUT<<"initialized_data ";
  if(sect.flags&0x00000080) DEBUG_OUT<<"uninitialized_data ";
  if(sect.flags&0x00004000) DEBUG_OUT<<"no_defer_spec ";
  if(sect.flags&0x01000000) DEBUG_OUT<<"ext_reloc ";
  if(sect.flags&0x02000000) DEBUG_OUT<<"discardable ";
  if(sect.flags&0x04000000) DEBUG_OUT<<"not_cachable ";
  if(sect.flags&0x08000000) DEBUG_OUT<<"not_pagable ";
  if(sect.flags&0x10000000) DEBUG_OUT<<"shared ";
  if(sect.flags&0x20000000) DEBUG_OUT<<"executable ";
  if(sect.flags&0x40000000) DEBUG_OUT<<"readable ";
  if(sect.flags&0x80000000) DEBUG_OUT<<"writable ";
  DEBUG_OUT<<")\n";
#endif // DEBUG_VERBOSE
  if(!strcmp(sect.sectName,".stabstr")) handleStringTable(sect.pOff,sect.vSize);
  NBinaryNode *n=new NBinaryNode(sect.sectName,num,N_BIN_PROGBITS,sect.rva,sect.vSize,sect.pOff,E_MEM_ALL);
  if(n) ld->getModule()->getNodes()->append(n);
  else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
  return true;
}


// NOTE: This is called after we've decoded the regular PE header
void NExeDecoder::handleCLR()
{
  // dot net clr executable
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Found a dotnet clr header...\n";
#endif // DEBUG_VERBOSE
  // TODO: need to implement...
}
