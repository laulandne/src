
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/AList.h>

#include <binarylib/NJavaClassDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>

#include <cpulib/ECPUAbstract.h>


////////////////////////////////////////////////////////////////////////////////
//  NJavaClassDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool NJavaClassDecoder::recognize(unsigned char *buffer,char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NJavaClassDecoder looking for signature...\n";
#endif // DEBUG_VERBOSE
  if((buffer[0]==0xca)&&(buffer[1]==0xfe)&&(buffer[2]==0xba)&&(buffer[3]==0xbe)) {
    // Next to check that it isn't a mutli-arch Mach-O file.
    if((buffer[8])||(buffer[9])) return true;
  }
  /*
  if((buffer[2]=='P')&&(buffer[3]=='K')) {
    // may be a ZIP file, look inside for java classes...
  }
  */
  return false;
}


NJavaClassDecoder::NJavaClassDecoder()
{
  init();
  theOffset=0;
  numConstants=0;  numInterfaces=0;  numFields=0;
  numMethods=0;  numAttributes=0;
  lastConstant=0;
  theOffset=0;
  lastUsedAddress=0;
  thisClass=0;  superClass=0;  accessFlags=0;
}


NJavaClassDecoder::~NJavaClassDecoder()
{
}


char *NJavaClassDecoder::whichCPU()
{
  return "java";
}


char *NJavaClassDecoder::whichIO()
{
  return "java";
}


bool NJavaClassDecoder::verifyOkay()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"JavaClass verify...\n";
#endif // DEBUG_VERBOSE
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  if((binmem[0]==0xca)&&(binmem[1]==0xfe)&&(binmem[2]==0xba)&&(binmem[3]==0xbe))
  {
    arch=N_LD_ARCH_JAVA;
    ret=true;
  }
  if(!ret) {
    DEBUG_OUT<<"File is not in JavaClass format!\n";
    return false;
  }
  return ret;
}


bool NJavaClassDecoder::decodeNodes()
{
  DEBUG_OUT<<" binary at "<<(ULONG)getImageOffset()<<"\n";
  theOffset=getImageOffset();
  unsigned int signature=map->read32b(theOffset);  theOffset+=4;
  DEBUG_OUT<<"signature="<<signature;
  unsigned int minorVersion=map->read16b(theOffset);  theOffset+=2;
  unsigned int majorVersion=map->read16b(theOffset);  theOffset+=2;
  DEBUG_OUT<<"  majorVersion="<<majorVersion<<"  minorVersion="<<minorVersion<<"\n";
  numConstants=map->read16b(theOffset)-1;  theOffset+=2;
  if(!readConstants()) return false;
  accessFlags=map->read16b(theOffset);  theOffset+=2;
  // 0x001=public 0x010=final 0x020=super 0x200=interface 0x400=abstract
  DEBUG_OUT<<"accessFlags="<<accessFlags;
  thisClass=map->read16b(theOffset);  theOffset+=2;
  superClass=map->read16b(theOffset);  theOffset+=2;
  getNameString(s,constants[thisClass].misc);
  DEBUG_OUT<<"  thisClass="<<s;
  getNameString(s,constants[superClass].misc);
  DEBUG_OUT<<"  superClass="<<s<<"\n";
  numInterfaces=map->read16b(theOffset);  theOffset+=2;
  if(!readInterfaces()) return false;
  numFields=map->read16b(theOffset);  theOffset+=2;
  if(!readFields()) return false;
  numMethods=map->read16b(theOffset);  theOffset+=2;
  if(!readMethods()) return false;
  numAttributes=map->read16b(theOffset);  theOffset+=2;
  if(!readAttributes()) return false;
  if(!checkSanity()) return false;
  ld->reset();
  NBinaryNode *n=new NBinaryNode("node",0,N_BIN_PROGBITS,0,binsize,0,E_MEM_ALL);
  if(n) ld->getModule()->getNodes()->append(n);
  else { std::cerr<<"Didn't get NBinaryNode!\n"; exit(5); }
  return true;
}


bool NJavaClassDecoder::readConstants()
{
  DEBUG_OUT<<"readConstants: num="<<numConstants<<"\n";
  if(!numConstants) return true;
  unsigned int t=1;
  lastConstant=1;
  bool reading=true;
  while(reading) {
    theOffset+=getConstantAtAddr(&(constants[lastConstant]),theOffset);
    t++;  lastConstant++;
    if(t>numConstants) reading=false;
  }
  t=1;
  reading=true;
  while(reading) {
    s[0]=0;
    DEBUG_OUT<<"Constant #"<<t;
	DEBUG_OUT<<"  addr="<<constants[t].addr<<" ("<<(constants[t].addr-getImageOffset())<<")";
	DEBUG_OUT<<" len="<<constants[t].size;
    DEBUG_OUT<<" val="<<constants[t].val<<" ("<<(constants[t].val-getImageOffset())<<")";
	DEBUG_OUT<<" val2="<<constants[t].val2<<" ";
    DEBUG_OUT<<" name="<<constants[t].getName()<<" ";
    switch(constants[t].type) {
      case ECPUABSTRACT_TYPE_UTF8:
        getStringAtAddr(s,constants[t].size,constants[t].val);
        DEBUG_OUT<<"UTF8 "<<s;
        break;
      case ECPUABSTRACT_TYPE_INT:
        DEBUG_OUT<<"INT";
        break;
      case ECPUABSTRACT_TYPE_FLOAT:
        DEBUG_OUT<<"FLOAT";
        break;
      case ECPUABSTRACT_TYPE_LONG:
        DEBUG_OUT<<"LONG";
        break;
      case ECPUABSTRACT_TYPE_DOUBLE:
        DEBUG_OUT<<"DOUBLE";
        break;
      case ECPUABSTRACT_TYPE_CLASS:
        getNameString(s,constants[t].misc);
        DEBUG_OUT<<"CLASS "<<s;
        break;
      case ECPUABSTRACT_TYPE_STRING:
        DEBUG_OUT<<"STRING ";
        break;
      case ECPUABSTRACT_TYPE_FIELD:
        getNameString(s,constants[constants[t].val].misc);
        DEBUG_OUT<<"FIELD "<<s;
        getNameString(s,constants[constants[t].val2].misc);
        DEBUG_OUT<<" "<<s;
        break;
      case ECPUABSTRACT_TYPE_METHOD:
        getNameString(s,constants[constants[t].val].misc);
        DEBUG_OUT<<"METHOD "<<s;
        getNameString(s,constants[constants[t].val2].misc);
        DEBUG_OUT<<" "<<s;
        break;
      case ECPUABSTRACT_TYPE_INTERFACE:
        getNameString(s,constants[constants[t].val2].misc);
        DEBUG_OUT<<"INTERFACE "<<s;
        getNameString(s,constants[constants[t].val].misc);
        DEBUG_OUT<<" "<<s;
        break;
      case ECPUABSTRACT_TYPE_NAMETYPE:
        getNameString(s,constants[t].misc);
        DEBUG_OUT<<"NAMETYPE "<<s;
        getNameString(s,constants[t].val);
        DEBUG_OUT<<" "<<s;
        break;
      default:
        DEBUG_OUT<<"Unknown";
        break;
    }
    DEBUG_OUT<<"\n";
    EMapSymbol *sym=new EMapSymbol(s,t,constants[t].addr-getImageOffset(),constants[t].size,constants[t].type,constants[t].val,constants[t].val2);
    AList *strings=map->getFirstModule()->getStrings();
    if(strings) strings->append((void *)sym);
    t++;
    if(t>numConstants) reading=false;
  }
  return true;
}


bool NJavaClassDecoder::readInterfaces()
{
  DEBUG_OUT<<"readInterfaces: num="<<numInterfaces<<"\n";
  if(!numInterfaces) return true;
  DEBUG_OUT<<"readInterfaces not implemented!\n";
  return false;
}


bool NJavaClassDecoder::readFields()
{
  DEBUG_OUT<<"readFields: num="<<numFields<<"\n";
  if(!numFields) return true;
  DEBUG_OUT<<"readFields not implemented!\n";
  return false;
}


bool NJavaClassDecoder::readMethods()
{
  DEBUG_OUT<<"readMethods: num="<<numMethods<<"\n";
  if(!numMethods) return true;
  unsigned int t=1;
  bool reading=true;
  NJavaCode c;
  NJavaMethod m;
  NJavaAttribute a;
  while(reading) {
    s[0]=0;
    methods.append((void *)theOffset);
    getMethodAtAddr(&m,theOffset);
    DEBUG_OUT<<"Method #"<<t<<" addr="<<m.addr<<" flags="<<m.flags;
    getNameString(s,m.desc);
    DEBUG_OUT<<" desc="<<m.desc<<" "<<s;
    DEBUG_OUT<<" num="<<m.num;
    getNameString(s,m.name);
    DEBUG_OUT<<" name="<<m.name<<" "<<s;
    DEBUG_OUT<<"\n";
    theOffset+=8;
    for(unsigned int i=0;i<m.num;i++) {
      getAttributeAtAddr(&a,theOffset);
      s[0]=0;
      getNameString(s,a.name);
      if(!strcmp(s,"Code")) {
        getCodeAtAddr(&c,theOffset);
        DEBUG_OUT<<"  Code #"<<i;
        DEBUG_OUT<<" addr="<<c.addr;
        DEBUG_OUT<<" name="<<c.name;
        DEBUG_OUT<<" len="<<c.len;
        DEBUG_OUT<<" maxStack="<<c.maxStack;
        DEBUG_OUT<<" maxLocals="<<c.maxLocals;
        DEBUG_OUT<<" codeLen="<<c.codeLen;
        DEBUG_OUT<<" codeAddr="<<c.codeAddr;
        DEBUG_OUT<<" exceptionLen="<<c.exceptionLen;
        DEBUG_OUT<<" exceptionAddr="<<c.exceptionAddr;
        DEBUG_OUT<<" numAttrib="<<c.numAttrib;
        DEBUG_OUT<<" attribAddr="<<c.attribAddr;
        DEBUG_OUT<<"\n";
        getNameString(s,m.name);
        EMapSymbol *sym=new EMapSymbol(s,t,c.codeAddr-getImageOffset(),c.codeLen,NJAVA_SYM_CODE);
        AList *symbols=map->getFirstModule()->getSymbols();
        if(symbols) symbols->append((void *)sym);
      }
      else {
        getAttributeAtAddr(&a,theOffset);
        DEBUG_OUT<<"  Attribute #"<<i<<" addr="<<a.addr<<" len="<<a.len;
        DEBUG_OUT<<" data="<<a.data;
        DEBUG_OUT<<" name="<<a.name<<" "<<s;
        DEBUG_OUT<<"\n";
        getNameString(s,a.name);
        EMapSymbol *sym=new EMapSymbol(s,t,a.addr-getImageOffset(),a.len,NJAVA_SYM_METHOD_ATTRIBUTE);
        AList *symbols=map->getFirstModule()->getSymbols();
        if(symbols) symbols->append((void *)sym);
      }
      theOffset+=6;
      theOffset+=a.len;
    }
    t++;
    if(t>numMethods) reading=false;
  }
  return true;
}


bool NJavaClassDecoder::readAttributes()
{
  DEBUG_OUT<<"readAttributes: num="<<numAttributes<<"\n";
  if(!numAttributes) return true;
  unsigned int t=1;
  NJavaAttribute a;
  bool reading=true;
  while(reading) {
    attributes.append((void *)theOffset);
    getAttributeAtAddr(&a,theOffset);
    DEBUG_OUT<<"Attribute #"<<t<<" addr="<<a.addr<<" len="<<a.len;
    DEBUG_OUT<<" data="<<a.data;
    getNameString(s,a.name);
    DEBUG_OUT<<" name="<<a.name<<" "<<s;
    DEBUG_OUT<<"\n";
    getNameString(s,a.name);
    EMapSymbol *sym=new EMapSymbol(s,t,a.addr-getImageOffset(),a.len,NJAVA_SYM_ATTRIBUTE);
    AList *symbols=map->getFirstModule()->getSymbols();
    if(symbols) symbols->append((void *)sym);
    theOffset+=6;
    theOffset+=a.len;
    t++;
    if(t>numAttributes) reading=false;
  }
  return true;
}


unsigned int NJavaClassDecoder::getConstantAtAddr(EMapSymbol *c,ULONG addr)
{
  if(!c) return 0;
  c->addr=addr;
  c->type=map->read8(addr);  addr+=1;
  c->setName(NULL); c->misc=0;  c->val=0;  c->val2=0;  c->size=0;
  unsigned int total=0;
  switch(c->type) {
    case ECPUABSTRACT_TYPE_UTF8:
      c->size=map->read16b(addr);  addr+=2;
      c->val=addr; addr+=c->size;
      break;
    case ECPUABSTRACT_TYPE_INT:
      c->val=map->read16b(addr);  addr+=4;
      break;
    case ECPUABSTRACT_TYPE_FLOAT:
      c->val=map->read16b(addr);  addr+=4;
      break;
    case ECPUABSTRACT_TYPE_LONG:
      c->val=map->read16b(addr);  addr+=4;
      c->val2=map->read16b(addr);  addr+=4;
      break;
    case ECPUABSTRACT_TYPE_DOUBLE:
      c->val=map->read16b(addr);  addr+=4;
      c->val2=map->read16b(addr);  addr+=4;
      break;
    case ECPUABSTRACT_TYPE_CLASS:
      c->misc=map->read16b(addr);  addr+=2;
      break;
    case ECPUABSTRACT_TYPE_STRING:
      c->val=map->read16b(addr);  addr+=2;
      break;
    case ECPUABSTRACT_TYPE_FIELD:
      c->val=map->read16b(addr);  addr+=2;
      c->val2=map->read16b(addr);  addr+=2;
      break;
    case ECPUABSTRACT_TYPE_METHOD:
      c->val=map->read16b(addr);  addr+=2;
      c->val2=map->read16b(addr);  addr+=2;
      break;
    case ECPUABSTRACT_TYPE_INTERFACE:
      c->val=map->read16b(addr);  addr+=2;
      c->val2=map->read16b(addr);  addr+=2;
      break;
    case ECPUABSTRACT_TYPE_NAMETYPE:
      c->misc=map->read16b(addr);  addr+=2;
      c->val=map->read16b(addr);  addr+=2;
      break;
    default:
      DEBUG_OUT<<"Bad constant type!\n";
      return 0;
      break;
  }
  total=addr-c->addr;
  return total;
}


unsigned int NJavaClassDecoder::getInterfaceAtAddr(NJavaInterface *i,ULONG addr)
{
  if(!i) return 0;
  i->addr=addr;
  return 0;
}


unsigned int NJavaClassDecoder::getFieldAtAddr(NJavaField *f,ULONG addr)
{
  if(!f) return 0;
  f->addr=addr;
  return 0;
}


unsigned int NJavaClassDecoder::getMethodAtAddr(NJavaMethod *m,ULONG addr)
{
  if(!m) return 0;
  unsigned int total=0;
  m->addr=addr;
  m->flags=map->read16b(addr);  addr+=2;
  m->name=map->read16b(addr);  addr+=2;
  // 0x001=public 0x002=private 0x005=protected 0x008=static 0x010=final
  // 0x020=synchronized 0x100=native 0x400=abstract 0x800=strict
  m->desc=map->read16b(addr);  addr+=2;
  m->num=map->read16b(addr);  addr+=2;
  m->attributes=addr;
  total=addr-m->addr;  // NOTE: Not right, doesn't include attributes
  return total;
}


unsigned int NJavaClassDecoder::getAttributeAtAddr(NJavaAttribute *a,ULONG addr)
{
  if(!a) return 0;
  unsigned int total=0;
  a->addr=addr;
  a->name=map->read16b(addr);  addr+=2;
  a->len=map->read32b(addr);  addr+=4;
  a->data=addr;
  total=addr-a->addr+a->len;
  return total;
}


unsigned int NJavaClassDecoder::getCodeAtAddr(NJavaCode *c,ULONG addr)
{
  if(!c) return 0;
  unsigned int total=0;
  c->addr=addr;
  c->name=map->read16b(addr); addr+=2;
  c->len=map->read32b(addr); addr+=4;
  c->maxStack=map->read16b(addr); addr+=2;
  c->maxLocals=map->read16b(addr); addr+=2;
  c->codeLen=map->read32b(addr); addr+=4;
  c->codeAddr=addr;  addr+=c->codeLen;
  c->exceptionLen=map->read16b(addr); addr+=2;
  c->exceptionAddr=addr;  addr+=c->exceptionLen;
  c->numAttrib=map->read16b(addr); addr+=2;
  c->attribAddr=addr;
  total=addr-c->addr;  // NOTE: Not correct, doesn't include attribs
  return total;
}


void NJavaClassDecoder::getStringAtAddr(char *s,unsigned int len,ULONG addr)
{
  if(!s) return;
  unsigned int i=0;
  for(i=0;i<len;i++) s[i]=map->read8(addr+i);
  s[i]=0;
}


unsigned int NJavaClassDecoder::getUTF8Const(char *s,ULONG index)
{
  if(!s) return 0;
  if(!index) { s[0]=0;  return 0; } //there is no const 0
  s[0]=0;
  unsigned int t=1;
  bool reading=true;
  while(reading) {
    if(t==index) {
      reading=false;
      getStringAtAddr(s,constants[t].size,constants[t].val);
      return constants[t].size;
    }
    else {
      t++;
      if(t>numConstants) reading=false;
    }
  }
  return 0;
}


unsigned int NJavaClassDecoder::getNameString(char *s,ULONG index)
{
  if(!s) return 0;
  if(!index) { s[0]=0;  return 0; } //there is no const 0
  s[0]=0;
  unsigned int t=1;
  bool reading=true;
  while(reading) {
    if(t==index) {
      reading=false;
      getStringAtAddr(s,constants[t].size,constants[t].val);
      return constants[t].size;
    }
    else {
      t++;
      if(t>numConstants) reading=false;
    }
  }
  return 0;
}


