
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


/*

A very braindead implementation of mac resource forks.

NOTE: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


#define PAINFULLY_VERBOSE 1


#include <elib/EResourceFile.h>
#include <elib/EResource.h>

#include <portable/nickcpp/AList.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/ANullStream.h>

#include <disklib/DVfs.h>


#ifdef ALIB_HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif // ALIB_HAVE_SYS_FCNTL_H
#ifdef ALIB_HAVE_FCNTL_H
#include <fcntl.h>
#endif // ALIB_HAVE_FCNTL_H


#ifndef O_BINARY
#define O_BINARY 0
#endif // O_BINARY

#ifndef O_RDONLY
#define O_RDONLY 0
#endif // O_RDONLY


////////////////////////////////////////////////////////////////////////////////
//  EResourceFile Class
////////////////////////////////////////////////////////////////////////////////

EResourceFile::EResourceFile(const char *name,DVfs *dos)
{
  DEBUG_OUT<<"EResourceFile("<<name<<",dos)...\n";
  init();
  if(!name) { DEBUG_OUT<<"ersrc Open failed.\n"; err.set(); return; }
  if(dos) thedos=dos;
  else thedos=new DVfs();
  if(!thedos) { DEBUG_OUT<<"ersrc: no thedos!\n"; err.set(); return; }
  unsigned int binsize=0;
  filename=strdup(name);
  DEBUG_OUT<<"ersrc Attempting to open resource file "<<filename<<"...\n";
#ifdef ERSRC_LOOK_FOR_NORMAL_FILES
  // FIRST we look for a "normal" file...
  fd=(FILE *)thedos->Open(filename,O_RDONLY|O_BINARY|O_RSRC);
  if(fd) {
    binsize=thedos->Lseek(fd,0,SEEK_END);
    thedos->Lseek(fd,0,SEEK_SET);
  }
#endif // ERSRC_LOOK_FOR_NORMAL_FILES
  // Now we look for a resource.frk folder...
  if(!binsize) {
    DEBUG_OUT<<"(ersrc Looking in resource.frk folder...)\n";
    // ...try looking for a "resource.frk" macEntry...
    if(fd) thedos->Close(fd);
    // TODO: filename may be a path! Need to handle this!
    char rffilename[DDISK_MAX_NAMELEN];
    rffilename[0]=0;
    const char *sfilename=thedos->getShortName(filename);
    DEBUG_OUT<<"The short name is "<<sfilename<<"\n";
    AStringHelper::fillInPathFromFullName(rffilename,filename);
    if(rffilename[0]) AStringHelper::fillInString(rffilename,"/");
    DEBUG_OUT<<"Building full path with path "<<rffilename<<"\n";
    AStringHelper::fillInString(rffilename,"resource.frk/");
    DEBUG_OUT<<"Building full path with resource.frk/ "<<rffilename<<"\n";
    AStringHelper::fillInNameFromFullName(rffilename,sfilename);
    DEBUG_OUT<<"Looking for "<<rffilename<<"\n";
    fd=(FILE *)thedos->Open(rffilename,O_RDONLY|O_BINARY);
    if(!fd) {
      // Try a lowercase version...
      for(unsigned int t=0;t<DDISK_MAX_NAMELEN;t++) {
        if((rffilename[t]>='A')&&(rffilename[t]<='Z')) rffilename[t]+='a'-'A';
      }
      DEBUG_OUT<<"Looking for lowercased "<<rffilename<<"...\n";
      fd=(FILE *)thedos->Open(rffilename,O_RDONLY|O_BINARY);
      if(!fd) DEBUG_OUT<<"Didn't find what we were looking for in resource.frk\n";
    }
  }
  if(!fd) {
    // Now look in a Linux HFS style .resource folder
    char rffilename[DDISK_MAX_NAMELEN];
    rffilename[0]=0;
    AStringHelper::fillInPathFromFullName(rffilename,filename);
    if(rffilename[0]) AStringHelper::fillInString(rffilename,"/");
    DEBUG_OUT<<"Building full path with path "<<rffilename<<"\n";
    AStringHelper::fillInString(rffilename,".resource/");
    DEBUG_OUT<<"Building full path with .resource/ "<<rffilename<<"\n";
    AStringHelper::fillInNameFromFullName(rffilename,filename);
    DEBUG_OUT<<"Looking for "<<rffilename<<"\n";
    fd=(FILE *)thedos->Open(rffilename,O_RDONLY|O_BINARY);
  }
  if(!fd) {
    // Now look for an ARDI-style %name resource fork file...
    // TODO: For some reason this doesn't work...
    // TODO: Need to look at the ARDI files and figure out how they store
    // resources
    char rffilename[DDISK_MAX_NAMELEN];
    rffilename[0]=0;
    AStringHelper::fillInPathFromFullName(rffilename,filename);
    if(rffilename[0]) AStringHelper::fillInString(rffilename,"/");
    DEBUG_OUT<<"Building full path with path "<<rffilename<<"\n";
    AStringHelper::fillInString(rffilename,"%");
    DEBUG_OUT<<"Building full path with % "<<rffilename<<"\n";
    AStringHelper::fillInNameFromFullName(rffilename,filename);
    DEBUG_OUT<<"Looking for "<<rffilename<<"\n";
    fd=(FILE *)thedos->Open(rffilename,O_RDONLY|O_BINARY);
  }
  if(!fd) {
    DEBUG_OUT<<"ersrc Open failed.\n";
    err.set(); return;
  }
  if(fd) readMacHeader();
}


void EResourceFile::init()
{
  filename=(char *)NULL;
  fd=(FILE *)NULL;
  offset=0;
  curNum=0;
  thedos=(DVfs *)NULL;
  headerRead=false;
  resourceMap=new AList;
  typeMap=new AList;
  macHeader=new EMacResHeader;
  macMap=new EMacResMap;
  macEnt=new EMacResEntry;
  macRef=new EMacResRef;
}


EResourceFile::~EResourceFile()
{
  if(!err.error()) {
    if(fd) { if(thedos) thedos->Close(fd); }
    //if(filename) free(filename);
    if(resourceMap) delete resourceMap;
    resourceMap=(AList *)NULL;
    if(typeMap) delete resourceMap;
    typeMap=(AList *)NULL;
    if(macHeader) delete macHeader;
    macHeader=(EMacResHeader *)NULL;
    if(macMap) delete macMap;
    macMap=(EMacResMap *)NULL;
    if(macEnt) delete macEnt;
    macEnt=(EMacResEntry *)NULL;
    if(macRef) delete macRef;
    macRef=(EMacResRef *)NULL;
 }
}


// NOTE: sign is tricky...
int EResourceFile::rdLong(long offset)
{
  UINT16 s1,s2;
  s1=rdShort(offset);
  s2=rdShort(offset+2);
  return (s1*65536)+s2;
}


// NOTE: sign is tricky...
short EResourceFile::rdShort(long offset)
{
  unsigned char c1,c2;
  c1=rdChar(offset);
  c2=rdChar(offset+1);
  return (short)((c1*256)+c2);
}


// NOTE: sign is tricky...
char EResourceFile::rdChar(long offset)
{
  if(!fd) return 0;
  if(!thedos) { DEBUG_OUT<<"ersrc: no thedos!\n"; err.set(); return 0; }
  unsigned char c=0;
  thedos->Lseek(fd,offset,SEEK_SET);
  thedos->Read(fd,&c,1);
  return c;
}


unsigned int EResourceFile::rdULong(long offset)
{
  UINT16 s1,s2;
  s1=rdUShort(offset);
  s2=rdUShort(offset+2);
  return (s1*65536)+s2;
}


UINT16 EResourceFile::rdUShort(long offset)
{
  unsigned char c1,c2;
  c1=rdUChar(offset);
  c2=rdUChar(offset+1);
  return (UINT16)((c1*256)+c2);
}


unsigned char EResourceFile::rdUChar(long offset)
{
  if(!fd) return 0;
  if(!thedos) { DEBUG_OUT<<"ersrc: no thedos!\n"; err.set(); return 0; }
  unsigned char c=0;
  thedos->Lseek(fd,offset,SEEK_SET);
  thedos->Read(fd,&c,1);
  return c;
}


unsigned int EResourceFile::count(const char *type)
{
  if(!type) return 0;
  if(!numTypes) return 0;
  if(err.error()) return 0;
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return 0; }
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return 0; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return 0; }
  DEBUG_OUT<<"Looking for type "<<type<<" resources.\n";
  offset=macMap->typeOff+macHeader->mapOff+2;
  unsigned int num=numTypes;
  while(num) {
    readMacEntry();
    if(!strcmp(ttype,type)) {
      DEBUG_OUT<<"Found "<<(int)macEnt->num<<" type "<<type<<" resources.\n";
      return macEnt->num;
    }
    num--;
  }
  return 0;
}


void EResourceFile::printName(long off)
{
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return; }
  offset=macHeader->mapOff+macMap->nameOff+off;
  unsigned int c=rdUChar(offset);  offset++;
  for(unsigned int t=0;t<c;t++) {
    BYTE cc=rdUChar(offset);  offset++;
    DEBUG_OUT<<(char)cc;
  }
  DEBUG_OUT<<"\n";
}


void EResourceFile::printName(char *buf)
{
  if(!buf) return;
  unsigned int c=buf[0];
  for(unsigned int t=0;t<c;t++) {
    BYTE cc=buf[t+1];
    DEBUG_OUT<<(char)cc;
  }
  DEBUG_OUT<<"\n";
}


bool EResourceFile::getRefFromNum(const char *type,unsigned int num)
{
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return false; }
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return false; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return false; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return false; }
  if(!type) return false;
  bool result=true;
  unsigned int n=count(type);
  if(!n) {
    DEBUG_OUT<<"No resources of type "<<type<<".\n";
    return false;
  }
  DEBUG_OUT<<"n is "<<n<<" num is "<<num<<"\n";
  if(num<=n) {
    n--;
    offset=macMap->typeOff+macHeader->mapOff+macEnt->refOff;
    offset+=num*12;  // skip to the one we want...
    macRef->id=rdUShort(offset);  offset+=2;
    macRef->nameOff=rdShort(offset);  offset+=2;
    ULONG temp=rdLong(offset);  offset+=4;
    macRef->attrib=(BYTE)((temp&0xff000000)>>24);
    macRef->dataOff=temp&0xffffff;
  }
  else {
    DEBUG_OUT<<"No such resource with that type and number.\n";
    result=false;
  } if(result) {
    DEBUG_OUT<<"Found macRef for type "<<type<<" number "<<num<<".\n";
    DEBUG_OUT<<"id is "<<(int)macRef->id<<"\n";
    DEBUG_OUT<<"nameOff is "<<(int)macRef->nameOff<<" ";
    if(macRef->nameOff<0) DEBUG_OUT<<"(no name)\n";
    else printName(macRef->nameOff);
    DEBUG_OUT<<"dataOff is "<<(int)macRef->dataOff<<"\n";
  }
  return result;
}


bool EResourceFile::getRefFromID(const char *type,unsigned int id)
{
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return false; }
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return false; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return false; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return false; }
  if(!type) return false;
  bool result=false;;
  unsigned int n=count(type);
  if(!n) {
    DEBUG_OUT<<"No resources of type "<<type<<".\n";
    return false;
  }
  bool looking=true;
  unsigned int counter=0;
  unsigned int tid,tnameOff,temp;
  while(looking) {
    offset=macMap->typeOff+macHeader->mapOff+macEnt->refOff;
    offset+=counter*12;  // skip to the next one...
    tid=rdUShort(offset);  offset+=2;
    tnameOff=rdShort(offset);  offset+=2;
    temp=rdLong(offset);  offset+=4;
    DEBUG_OUT<<"Looking at type "<<type<<" id "<<tid<<"\n";
    if(tid==id) {
      DEBUG_OUT<<"Found the one we were looking for!\n";
      macRef->id=tid;
      macRef->nameOff=tnameOff;
      macRef->attrib=(BYTE)((temp&0xff000000)>>24);
      macRef->dataOff=temp&0xffffff;
      result=true;
    }
    counter++;
    if(counter==n) looking=false;
  }
  if(result) {
    DEBUG_OUT<<"Found macRef for type "<<type<<" id "<<id<<".\n";
    DEBUG_OUT<<"nameOff is "<<(int)macRef->nameOff<<" ";
    if(macRef->nameOff<0) DEBUG_OUT<<"(no name)\n";
    else printName(macRef->nameOff);
    DEBUG_OUT<<"dataOff is "<<(int)macRef->dataOff<<"\n";
  }
  else {
    DEBUG_OUT<<"Didn't find macRef for "<<type<<" id "<<id<<".\n";
  }
  return result;
}


long EResourceFile::getDataOffset()
{
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return 0; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return 0; }
  offset=macHeader->dataOff+macRef->dataOff+4;  // 4 is for size
  return offset;
}


unsigned int EResourceFile::getDataSize()
{
  getDataOffset();
  return rdULong(offset-4);
}


bool EResourceFile::readFromNum(const char *type,unsigned int num,unsigned char *buffer)
{
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return false; }
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return false; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return false; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return false; }
  if(!type) return false;
  if(!numTypes) return false;
  if(!buffer) return false;
  if(!thedos) { DEBUG_OUT<<"ersrc: no thedos!\n"; err.set(); return false; }
  bool result=false;
  unsigned int size=0;
  if(!fd) return 0;
  result=getRefFromNum(type,num);
  if(result) {
    offset=macHeader->dataOff+macRef->dataOff;
    size=rdULong(offset);  offset+=4;
    DEBUG_OUT<<"Found resource type "<<type<<" number "<<num<<" it is ";
    DEBUG_OUT<<size<<" bytes long.\n";
    thedos->Lseek(fd,offset,SEEK_SET);
    thedos->Read(fd,buffer,size);
    curSize=size;
  }
  return result;
}


bool EResourceFile::readFromID(const char *type,unsigned int id,unsigned char *buffer)
{
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return false; }
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return false; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return false; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return false; }
  if(!type) return false;
  if(!numTypes) return false;
  if(!buffer) return false;
  if(!thedos) { DEBUG_OUT<<"ersrc: no thedos!\n"; err.set(); return false; }
  bool result=false;
  unsigned int size=0;
  if(!fd) return 0;
  result=getRefFromID(type,id);
  if(result) {
    offset=macHeader->dataOff+macRef->dataOff;
    size=rdULong(offset);  offset+=4;
    DEBUG_OUT<<"Found resource type "<<type<<" id "<<id<<" it is ";
    DEBUG_OUT<<size<<" bytes long.\n";
    thedos->Lseek(fd,offset,SEEK_SET);
    thedos->Read(fd,buffer,size);
    curSize=size;
  }
  return result;
}


bool EResourceFile::getNameFromNum(const char *type,unsigned int num,unsigned char *buf)
{
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return false; }
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return false; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return false; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return false; }
  if(!type) return false;
  if(!buf) return false;
  bool res=getRefFromNum(type,num);
  if(res) {
    offset=macHeader->mapOff+macMap->nameOff+macRef->nameOff;
    unsigned int c=rdUChar(offset);  offset++;
    unsigned int t;
    for(t=0;t<c;t++) {
      unsigned char cc=rdUChar(offset);  offset++;
      buf[t]=cc;
    }
    buf[t]=0;
  }
  return res;
}


bool EResourceFile::getNameFromID(const char *type,unsigned int id,unsigned char *buf)
{
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return false; }
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return false; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return false; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return false; }
  if(!type) return false;
  if(!buf) return false;
  bool res=getRefFromID(type,id);
  if(res) {
    offset=macHeader->mapOff+macMap->nameOff+macRef->nameOff;
    unsigned int c=rdUChar(offset);  offset++;
    unsigned int t;
    for(t=0;t<c;t++) {
      unsigned char cc=rdUChar(offset);  offset++;
      buf[t]=cc;
    }
    buf[t]=0;
  }
  return res;
}


unsigned int EResourceFile::getSizeFromNum(const char *type,unsigned int num)
{
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return 0; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return 0; }
  if(!type) return 0;
  ULONG size=0;
  bool result=false;
  result=getRefFromNum(type,num);
  if(result) {
    offset=macHeader->dataOff+macRef->dataOff;
    size=rdULong(offset);  offset+=4;
    DEBUG_OUT<<"Found resource type "<<type<<" number "<<num<<" it is ";
    DEBUG_OUT<<size<<" bytes long.\n";
    curSize=size;
    return size;
  }
  return size;
}


unsigned int EResourceFile::getSizeFromID(const char *type,unsigned int id)
{
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return 0; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return 0; }
  if(!type) return 0;
  ULONG size=0;
  bool result=false;
  result=getRefFromID(type,id);
  if(result) {
    offset=macHeader->dataOff+macRef->dataOff;
    size=rdULong(offset);  offset+=4;
    DEBUG_OUT<<"Found resource type "<<type<<" id "<<id<<" it is ";
    DEBUG_OUT<<size<<" bytes long.\n";
    curSize=size;
    return size;
  }
  return size;
}


void EResourceFile::readMacEntry()
{
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return; }
  macEnt->type=rdULong(offset);  offset+=4;
  macEnt->num=rdUShort(offset)+1;  offset+=2;
  macEnt->refOff=rdShort(offset);  offset+=2;
  ttype[4]=0;
  ttype[0]=(char)((macEnt->type)>>24)&0xff;
  ttype[1]=(char)((macEnt->type)>>16)&0xff;
  ttype[2]=(char)((macEnt->type)>>8)&0xff;
  ttype[3]=(char)(macEnt->type)&0xff;
  DEBUG_OUT<<"type="<<ttype<<" ("<<(int)macEnt->type<<") num="<<(int)macEnt->num;
  DEBUG_OUT<<" refOff="<<(int)macEnt->refOff<<"\n";
}


const char *EResourceFile::getCurType()
{
  return ttype;
}


unsigned int EResourceFile::getCurCount()
{
  return macEnt->num;
}


void EResourceFile::readMacHeader()
{
  if(!macMap) { DEBUG_OUT<<"No macMap!!!\n"; return; }
  if(!macEnt) { DEBUG_OUT<<"No macEnt!!!\n"; return; }
  if(!macHeader) { DEBUG_OUT<<"No macHeader!!!\n"; return; }
  if(!macRef) { DEBUG_OUT<<"No macRef!!!\n"; return; }
  if(headerRead) {
    std::cerr<<"Resource Header already read!\n";
    return;
  }
  // Read resource macHeader
  offset=0;
  macHeader->dataOff=rdULong(offset);  offset+=4;
  macHeader->mapOff=rdULong(offset);  offset+=4;
  macHeader->dataLen=rdULong(offset);  offset+=4;
  macHeader->mapLen=rdULong(offset);  offset+=4;
  DEBUG_OUT<<"data: offset="<<macHeader->dataOff<<" len="<<macHeader->dataLen<<"\n";
  DEBUG_OUT<<"map: offset="<<macHeader->mapOff<<" len="<<macHeader->mapLen<<"\n";
  // Read resource map
  offset=macHeader->mapOff;
  rdLong(offset); offset+=4;
  rdLong(offset); offset+=4;
  rdLong(offset); offset+=4;
  rdLong(offset); offset+=4;
  rdLong(offset); offset+=4;
  rdShort(offset); offset+=2;
  macMap->attrib=rdUShort(offset);  offset+=2;
  macMap->typeOff=rdUShort(offset);  offset+=2;
  macMap->nameOff=rdUShort(offset);  offset+=2;
  DEBUG_OUT<<"attrib="<<(int)macMap->attrib<<"\n";
  DEBUG_OUT<<"types: offset="<<(int)macMap->typeOff<<"\n";
  DEBUG_OUT<<"name: offset="<<(int)macMap->nameOff<<"\n";
  // Read in list of resource types...
  DEBUG_OUT<<"Building list of resource types...\n";
  typeMap->reset();
  offset=macMap->typeOff+macHeader->mapOff;
  numTypes=rdUShort(offset)+1; offset+=2;
  if((!macHeader->dataLen)||(!macHeader->mapLen)) numTypes=0;
  unsigned int num=numTypes;
  DEBUG_OUT<<num<<" types of resources in map.\n";
  EResource *r=(EResource *)NULL;
  EResource *t=(EResource *)NULL;
  while(num) {
    //aDefaultStringHelper.yieldToOtherProcesses();
    readMacEntry();
    char *theType=(char *)malloc(5); // 4 letters and null terminator
    strcpy(theType,ttype);
    t=new EResource((char *)NULL,theType,macEnt->num,macEnt->refOff,0);
    typeMap->insert(t);
    num--;
  }
  // Now read in actual resource entries...
  DEBUG_OUT<<"Building resource list...\n";
  typeMap->reset();
  resourceMap->reset();
  bool reading=true;
  while(reading) {
    //aDefaultStringHelper.yieldToOtherProcesses();
    if(typeMap->atEnd()) reading=false;
    else {
      t=(EResource *)typeMap->info();
      char *theType=strdup(t->type);
      for(unsigned int n=0;n<t->num;n++) {
        getRefFromNum(theType,n);
        char *theName=(char *)NULL;  // TODO: not handled yet...
        unsigned int theID=macRef->id;
        unsigned int theOff=macHeader->dataOff+macRef->dataOff+4;
        unsigned int theSize=getDataSize();
        r=new EResource(theName,theType,theID,theOff,theSize);
        resourceMap->insert(r);
      }
      typeMap->advance();
    }
  }
  typeMap->reset();
  resourceMap->reset();
  headerRead=true;
}


