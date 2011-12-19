
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NElfDecoder.h>
#include <elib/EMemMapAbstract.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  NElfDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NElfDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NElfDecoder looking for signature...\n";
#endif // DEBUG_VERBOSE
  if((buffer[0]==0x7f)&&(buffer[1]=='E')&&(buffer[2]=='L')&&(buffer[3]=='F'))
    return true;
  return false;
}


NElfDecoder::NElfDecoder()
{
  init();
  shouldDemandPage=true;
  subArch=N_LD_SUBARCH_GENERIC;
  // Stuff for the ELF format...
  ftype=0;  fstind=0;
  fphnum=0;  fphsize=0;  fphoff=0;
  fshnum=0;  fshsize=0;  fshoff=0;
  bitness=32;
  offset=0;
}


char *NElfDecoder::whichCPU()
{
  return archString();
}


char *NElfDecoder::whichIO()
{
  return subArchString();
}


bool NElfDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Elf verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  LSB=false;  bitness=32;
  if((binmem[0]==0x7f)&&(binmem[1]=='E')&&(binmem[2]=='L')&&(binmem[3]=='F')) {
    if(binmem[6]!=1) DEBUG_OUT<<"Warning, ELF format may be too new!\n";
    if(binmem[4]==2) bitness=64;
    if(binmem[5]==1) LSB=true;
#ifdef DEBUG_VERBOSE
    if(LSB) DEBUG_OUT<<"Little-endian ";
    else DEBUG_OUT<<"Big-endian ";
#endif // DEBUG_VERBOSE
  }
  else {
    DEBUG_OUT<<"File is not in ELF format!\n";
    return false;
  }
  return true;
}


bool NElfDecoder::decodeNodes()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Elf decodeNodes...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=true;
  ret=readHeader();
  if(ret) { if(fphoff) ret=decode_elf_ph(); }
  if(ret) { if(fshoff) ret=decode_elf_sh(); }
  if(ret) ret=readStringsAndSymbols();
  return ret;
}


bool NElfDecoder::readHeader()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"elf readHeader\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ELF";
  if(bitness==64) DEBUG_OUT<<"64"; else DEBUG_OUT<<"32";
  DEBUG_OUT<<" binary at "<<(ULONG)getImageOffset()<<"\n";
#endif // DEBUG_VERBOSE
  offset=5;
  LSB=false;
  if(map->read8(offset+getImageOffset())==1) LSB=true;
  offset=16;  // Skip rest of magic number, etc.
  ftype=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  arch=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  unsigned int vers=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  if(bitness==64) {
    ld->setEntry(map->read64(offset+getImageOffset(),LSB));  offset+=8;
    fphoff=map->read64(offset+getImageOffset(),LSB);  offset+=8;
    fshoff=map->read64(offset+getImageOffset(),LSB);  offset+=8;
  }
  else {
    ld->setEntry(map->read32(offset+getImageOffset(),LSB));  offset+=4;
    fphoff=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    fshoff=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  }
  unsigned int flags=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  unsigned int hsize=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  fphsize=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  fphnum=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  fshsize=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  fshnum=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  fstind=map->read16(offset+getImageOffset(),LSB);  offset+=2;
  ld->setUnknown(false);
  switch(ftype) {
    case 1:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"REL";
#endif // DEBUG_VERBOSE
      break;
    case 2:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"EXEC";
#endif // DEBUG_VERBOSE
      break;
    case 3:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"DYN";
#endif // DEBUG_VERBOSE
      break;
    case 4:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"CORE";
#endif // DEBUG_VERBOSE
      break;
    case 16:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"REL?";
#endif // DEBUG_VERBOSE
      break;
    default:
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Unknown class (#"<<ftype<<")";
#endif // DEBUG_VERBOSE
      ld->setUnknown(true);
      break;
  }
  ld->setNeedsReloc(false);
  if(ftype==1) ld->setNeedsReloc(true);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<", ";
  DEBUG_OUT<<archString()<<"\n";
#endif // DEBUG_VERBOSE
  if(ld->getUnknown()) {
    DEBUG_OUT<<"ELF file format error!\n";
    //return;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Header, Entry at "<<ld->getEntry()<<"\n";
  DEBUG_OUT<<"phof="<<fphoff<<" phsz="<<fphsize<<" phnm="<<fphnum<<"\n";
  DEBUG_OUT<<"shof="<<fshoff<<" shsz="<<fshsize<<" shnm="<<fshnum<<"\n";
  DEBUG_OUT<<"stind="<<fstind<<"\n";
#endif // DEBUG_VERBOSE
  return true;
}


bool NElfDecoder::decode_elf_ph()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NElfDecoder::decode_elf_ph()\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  offset=fphoff;
  unsigned int phtype,phflags;
  ULONG phaddr,phpaddr;
  ULONG phoff,phsize,phfsize,phalign;
  ULONG lastAddr=0;
  if(fphnum>1000) fphnum=1000;  // Sanity check...
  for(unsigned int t=0;t<fphnum;t++) {
    phtype=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    if(bitness==32) {
      phoff=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      phaddr=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      phpaddr=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      phfsize=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      phsize=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      phflags=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      phalign=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    }
    else {
      phflags=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      phoff=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      phaddr=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      phpaddr=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      phfsize=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      phsize=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      phalign=map->read64(offset+getImageOffset(),LSB);  offset+=8;
    }
    DEBUG_OUT<<"p_node "<<t<<" phtype="<<phtype<<" "<<" phoff="<<phoff;
    DEBUG_OUT<<" phaddr="<<phaddr<<" "<<" phpaddr="<<phpaddr<<" phfsize="<<phfsize;
    DEBUG_OUT<<" phsize="<<phsize<<" "<<" phflags="<<phflags;
    DEBUG_OUT<<"\n";
    if(ld->getNeedsReloc()) {
      phaddr=lastAddr;
      lastAddr+=phsize;
      // Align on 32-bit word (Is this right for 64 bit?)
      if(lastAddr&0x3) lastAddr=(lastAddr&0xfffffffc)+4;
    }
/*
    if(phtype) {
      NBinaryNode *n=new NBinaryNode("node_p"),t,phtype,phaddr,
        phsize,phoff,E_MEM_ALL);
      if(n) ld->getModule()->getNodes()->append(n);
      else std::cerr<<"Didn't get NBinaryNode!\n");
    }
*/
  }
  return true;
}


bool NElfDecoder::decode_elf_sh()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NElfDecoder::decode_elf_sh()\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  if(ld->getUnknown()) return false;
  offset=fshoff;
  unsigned int shname,shtype,shlink,shinfo;
  ULONG shaddr,shoff,shflags,shsize,shalign,shentsize;
  ULONG lastAddr=0;
  bool reloc=false;
  if(fshnum>1000) fshnum=1000;  // Sanity check...
  for(unsigned int t=0;t<fshnum;t++) {
    reloc=false;
    ULONG toffset=offset;
    shname=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    shtype=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    if(bitness==32) {
      shflags=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      shaddr=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      shoff=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      shsize=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    }
    else {
      shflags=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      shaddr=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      shoff=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      shsize=map->read64(offset+getImageOffset(),LSB);  offset+=8;
    }
    DEBUG_OUT<<"(offset is "<<offset<<")\n";
    shlink=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    shinfo=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    if(bitness==32) {
      shalign=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      shentsize=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    }
    else {
      shalign=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      shentsize=map->read64(offset+getImageOffset(),LSB);  offset+=8;
    }
    DEBUG_OUT<<"s_node "<<t<<" shname="<<shname<<" shoff="<<shoff<<" shaddr="<<shaddr;
    DEBUG_OUT<<" shsize="<<shsize<<" shlink="<<shlink<<" "<<"shinfo="<<shinfo;
    DEBUG_OUT<<" shtype="<<shtype<<" shflags="<<shflags<<" "<<"shentsize="<<shentsize;
    DEBUG_OUT<<"\n";
    if(ld->getNeedsReloc()) {
      reloc=true;
      shaddr=lastAddr;
      lastAddr+=shsize;
      // Align on 32-bit word (Is this right for 64 bit?)
      if(lastAddr&0x3) lastAddr=(lastAddr&0xfffffffc)+4;
    }
    unsigned int perm=0;
    if(shflags&1) perm|=E_MEM_WRITE;
    if(shflags&2) perm|=E_MEM_READ;
    if(shflags&4) perm|=E_MEM_EXEC;
    if(shtype) {
      NBinaryNode *n=new NBinaryNode("node_s",t,shtype,shaddr,shsize,shoff,perm,(BYTE *)NULL,toffset);
      if(n) ld->getModule()->getNodes()->append(n);
      else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
    }
  }
  return true;
}


bool NElfDecoder::readStringsAndSymbols()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NElfDecoder::readStringsAndSymbols()\n";
#endif // DEBUG_VERBOSE
  NBinaryNode *nodeNames=ld->findNode(fstind);
  if(!nodeNames) {
    DEBUG_OUT<<"findNode(fstind) didn't work!\n";
    err.set();
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Found node "<<fstind<<" with strings, going to fix mod's names now...\n";
#endif // DEBUG_VERBOSE
  ld->getModule()->getNodes()->reset();
  NBinaryNode *n=(NBinaryNode *)NULL;
  bool working=true;
  unsigned int shname;
  while(working) {
    n=(NBinaryNode *)ld->getModule()->getNodes()->info();
    if(n) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Node #"<<n->num<<" ";
#endif // DEBUG_VERBOSE
      shname=map->read32(n->val+getImageOffset(),LSB);
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"shname="<<shname<<" ";
#endif // DEBUG_VERBOSE
      char *tname=getStringAtOffset(shname,nodeNames);
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"getStringAtOffset was fine\n";
#endif // DEBUG_VERBOSE
      if(tname) {
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"name is "<<tname<<"\n";
#endif // DEBUG_VERBOSE
        if(tname) n->setName(strdup(tname));
      }
      switch(n->type) {
        case N_BIN_SYMTAB:
        case N_BIN_DYNSYM:
        case N_BIN_DYNAMIC:
          // shinfo is string table index
          handleSymbols(n);
          break;
        case N_BIN_REL:
        case N_BIN_RELA:
        case N_BIN_HASH:
          // shinfo is symbol table index
          break;
        default:
          break;
      }
    }
    ld->getModule()->getNodes()->advance();
    if(ld->getModule()->getNodes()->atEnd()) working=false;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"readStringsAndSymbols done.\n";
#endif // DEBUG_VERBOSE
  return true;
}


bool NElfDecoder::handleSymbols(NBinaryNode *n)
{
  if(!n) {
    DEBUG_OUT<<"handleSymbols...NULL node!\n";
    err.set();
    return false;
  }
  unsigned int nsyms=0;
  if(bitness==32) nsyms=n->size/16;
  else nsyms=n->size/24;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"handleSymbols for node "<<n->num<<" has "<<nsyms<<" symbols...\n";
#endif // DEBUG_VERBOSE
  if(bitness==32) offset=n->val+24;
  else offset=n->val+40;
  DEBUG_OUT<<"(offset is "<<offset<<")\n";
  unsigned int shlink=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  unsigned int shinfo=map->read32(offset+getImageOffset(),LSB);  offset+=4;
  DEBUG_OUT<<"shlink="<<shlink<<" shinfo="<<shinfo<<"\n";
  if(!shlink) {
    DEBUG_OUT<<"shlink was zero, probably not right, giving up...\n";
    return false;
  }
  NBinaryNode *strings=ld->findNode(shlink);
  if(strings) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Associated strings in node "<<strings->num<<"...\n";
#endif // DEBUG_VERBOSE
  }
  else DEBUG_OUT<<"Couldn't find strings for those symbols!\n";
  bool working=true;
  unsigned int stname,stinfo,stother,stind,stbind,sttype;
  ULONG stvalue,stsize;
  offset=n->getOffsetToData();
  EMapSymbol *s=(EMapSymbol *)NULL;
  AList *syms=map->getFirstModule()->getSymbols();
  for(unsigned int t=0;t<nsyms;t++) {
    stname=map->read32(offset+getImageOffset(),LSB);  offset+=4;
    if(bitness==32) {
      stvalue=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      stsize=map->read32(offset+getImageOffset(),LSB);  offset+=4;
      stinfo=map->read8(offset+getImageOffset());  offset+=1;
      stother=map->read8(offset+getImageOffset());  offset+=1;
      stind=map->read16(offset+getImageOffset(),LSB);  offset+=2;
    }
    else {
      stinfo=map->read8(offset+getImageOffset());  offset+=1;
      stother=map->read8(offset+getImageOffset());  offset+=1;
      stind=map->read16(offset+getImageOffset(),LSB);  offset+=2;
      stvalue=map->read64(offset+getImageOffset(),LSB);  offset+=8;
      stsize=map->read64(offset+getImageOffset(),LSB);  offset+=8;
    }
    stbind=(stinfo>>4)&0xf;
    sttype=stinfo&0xf;
    DEBUG_OUT<<"#"<<t<<" stname="<<stname<<" stvalue="<<stvalue<<" stsize="<<stsize<<" stbind="<<stbind<<" stind="<<stind<<" ";
    unsigned int symType=EMAP_SYMBOL_NORMAL;
    switch(sttype) {
      case 0: // No type
        DEBUG_OUT<<"sttype is No Type.\n";
        break;
      case 1: // object
        DEBUG_OUT<<"sttype is object.\n";
        symType=EMAP_SYMBOL_INT;
        break;
      case 2: // func
        DEBUG_OUT<<"sttype is func.\n";
        symType=EMAP_SYMBOL_FUNC;
        break;
      case 3: // section
        DEBUG_OUT<<"sttype is section.\n";
        break;
      case 4: // file
        DEBUG_OUT<<"sttype is file.\n";
        symType=EMAP_SYMBOL_FILENAME;
        break;
      default:
        DEBUG_OUT<<"sttype is default.\n";
        break;
    }
    if(stname) {
      DEBUG_OUT<<"New symbol...";
      DEBUG_OUT<<""<<getStringAtOffset(stname,strings)<<"";
      s=new EMapSymbol(getStringAtOffset(stname,strings),t,stvalue,stsize,symType);
      DEBUG_OUT<<"...adding to map...";
      if(syms) syms->insert(s);
      DEBUG_OUT<<"done.\n";
    }
  }
  return true;
}


