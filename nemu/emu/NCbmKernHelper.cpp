
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NCbmHandler.h>
#include <emu/NCbmIO.h>
#include <emu/NCbmVideo.h>
#include <emu/NCbmKernHelper.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/AEvent.h>

#include <portable/nickcpp/AStringHelper.h>

#include <elib/EKeys.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>
#include <elib/EMemMapAbstract.h>

#include <disklib/DController65.h>
#include <disklib/DVfs.h>

#include <cpulib/Em6502.h>


#define NCBM_WANT_KERN_TABLE 1
#include <emu/generated/NCbm_kern_tab.h>


#define HIGH_BYTECBM(x)  ((x&0xff00)>>8)
#define LOW_BYTECBM(x)  (x&0xff)


////////////////////////////////////////////////////////////////////////////////
//  NCbmKernHelper Class
////////////////////////////////////////////////////////////////////////////////

NCbmKernHelper::NCbmKernHelper(NCbmHandler *parent) : EOSHelper(parent)
{
  init();
  io=parent;
  reset();
}


NCbmKernHelper::~NCbmKernHelper()
{
  // TODO: free kernal entries
}


void NCbmKernHelper::reset()
{
  EOSHelper::reset();
  for(unsigned int t=0;t<NCBM_NUM_CHANNELS;t++) {
    chanDev[t]=0;
    chanState[t]=NCBMDOS_NONE;
    chanUse[t]=NCBMDOS_NONE;
    chanSec[t]=0;
  }
  curDev=0;  curChan=0;  curSecond=0;
}


// TODO: Interp/ignore prefixed drive numbers or things like ,s
void NCbmKernHelper::doLoad()
{
  if(!cpu) { std::cerr<<"NCbmKernHelper::doLoad No CPU!\n"; return; }
  if(!dos) { std::cerr<<"NCbmKernHelper::doLoad No DOS!\n"; return; }
  char fname[256];
  unsigned int t;
  for(t=0;t<nameLen;t++) fname[t]=map->read8(nameAddr+t);
  fname[t]=0;
  DEBUG_OUT<<"Load "<<fname<<" from device "<<curDev<<","<<curSecond<<"\n";
  unsigned int diskNum=0;
  switch(curDev) {
    case 1: diskNum=0; break;  // "Default device num"
    case 8: diskNum=0; break;  // Floppy Image #1
    case 9: diskNum=1; break;  // Floppy Image #2
    case 10: diskNum=2; break;  // Floppy Image #3
    case 11: diskNum=3; break;  // Floppy Image #4
    default:
      std::cerr<<"Bad device number "<<curDev<<"!\n";
      cpu->setRegister("p",0x81);    cpu->setRegister("a",4);
      return;
      break;
  }
  dos->setDiskNum(diskNum);
  unsigned int start=0;
  if(curSecond==0) start=p3*256+p2;
  else start=dos->getStartAddress(fname);
  unsigned int readSize=0;
  if(fname[0]=='$') readSize=loadDirectory(fname,start,0xffff-start);
  else readSize=load(fname,start,2,0xffff-start);
  if(readSize) {
    cpu->setRegister("p",0);
    cpu->setRegister("y",(start+readSize+2)/256);
    cpu->setRegister("x",(start+readSize+2)&0xff);
  }
  else {
    std::cerr<<"NCbmKernHelper couldn't load "<<fname<<".\n";
    cpu->setRegister("p",0x81);
    cpu->setRegister("a",4);
  }
}


bool NCbmKernHelper::systemCall(ULONG loc)
{
  if(!cpu) {
    reset();
    if(!cpu) { std::cerr<<"NCBMKH Couldn't handle the systemCall, no cpu!\n"; return false; }
  }
  if(!dos) {
    reset();
    if(!dos) { std::cerr<<"NCBMKH Couldn't handle the systemCall, no dos!\n"; return false; }
  }
  DEBUG_OUT<<"NCbmKernHelper::systemCall("<<loc<<")...\n";
  unsigned int t=0;
  unsigned int c[3];
  for(t=0;t<3;t++) c[t]=0;
  bool ret=false;
  p1=cpu->readRegister("a");
  p2=cpu->readRegister("x");
  p3=cpu->readRegister("y");
  unsigned int original=0;
  EOSEntry *n=findActual(loc);
  if(n) original=n->origAddr;
  else {
    std::cerr<<"Couldn't findActual  ("<<aDefaultStringHelper.myHex((unsigned short)loc)<<"), must be a real BRK...\n";
    io->debugger("");
    return true;
  }
  //unsigned int d=w->getHexDigits();
  //w->setHexDigits(2);
  //DEBUG_OUT<<"sysCall: "<<n->name<<" "<<aDefaultStringHelper.myHex((unsigned short)original)<<" ";
  //DEBUG_OUT<<aDefaultStringHelper.myHex((unsigned short)loc)<<"\n";
  switch(original) {
    case NCBM_KERNEL_CHKIN:
      inFile=p2;
      if(chanState[inFile]==NCBMDOS_OPEN) {
        //DEBUG_OUT<<"CHKIN "<<x<<"\n";
        ret=true;
      }
      break;
    case NCBM_KERNEL_CHKOUT:
      outFile=p2;
      if(chanState[outFile]==NCBMDOS_OPEN) {
        //DEBUG_OUT<<"CHKOUT "<<x<<"\n";
        ret=true;
      }
      break;
    case NCBM_KERNEL_CHRIN:
      if(chanState[inFile]==NCBMDOS_OPEN) {
        p1=rbuf[rbufPtr];
        rbufPtr++;
        if(rbufPtr==1024) rbufPtr--;
        //DEBUG_OUT<<"CHRIN channel "<<inFile<<": "<<(char)a<<" ("<<(int)a<<","<<rbufPtr<<")\n";
        ret=true;
      }
      break;
    case NCBM_KERNEL_CHROUT:
      if(chanState[outFile]==NCBMDOS_OPEN) {
        //DEBUG_OUT<<"CHROUT channel "<<outFile<<": "<<(char)a<<" ("<<(unsigned int)a<<")\n";
        wbuf[wbufPtr]=p1;  wbufPtr++;
        if(wbufPtr==1024) wbufPtr--;
        if((chanUse[outFile]==NCBMDOS_COMMAND)&&(p1==13)) doDOSCommand();
        ret=true;
      }
      break;
    case NCBM_KERNEL_CLALL:
      //DEBUG_OUT<<"CLALL\n";
      curChan=0;  curDev=0;  curSecond=0;
      for(t=0;t<NCBM_NUM_CHANNELS;t++) {
        chanDev[t]=0;
        chanState[t]=NCBMDOS_CLOSED;
        chanUse[t]=NCBMDOS_NONE;
        chanSec[t]=0;
      }
      wbufPtr=0;  rbufPtr=0;
      break;
    case NCBM_KERNEL_CLRCHN:
      //DEBUG_OUT<<"CLRCHN\n";
      inFile=0;
      outFile=0;
      wbufPtr=0;  rbufPtr=0;
      break;
    case NCBM_KERNEL_CLOSE:
      if(chanState[p1]==NCBMDOS_OPEN) {
        //DEBUG_OUT<<"CLOSE "<<a<<"\n";
        wbufPtr=0;  rbufPtr=0;
        ret=true;
      }
      curChan=p1;
      chanDev[curChan]=0;
      chanState[curChan]=NCBMDOS_CLOSED;
      chanUse[curChan]=NCBMDOS_NONE;
      chanSec[curChan]=0;
      break;
    case NCBM_KERNEL_LOAD:  // LOAD ()
      // a=0 if load, else a=1 if verify.
      // x,y may have load addr
      doLoad();
      ret=true;
      break;
    case NCBM_KERNEL_OPEN:
      if(curDev==8) {
        //DEBUG_OUT<<"OPEN "<<curChan<<","<<curDev<<","<<curSecond<<",";
        //for(t=0;t<nameLen;t++) DEBUG_OUT<<(char)map->read8(nameAddr+t);
        //DEBUG_OUT<<"\n";
        chanState[curChan]=NCBMDOS_OPEN;
        chanUse[curChan]=NCBMDOS_UNKNOWN;
        if(chanSec[curChan]==15) chanUse[curChan]=NCBMDOS_COMMAND;
        if(map->read8(nameAddr)=='#') chanUse[curChan]=NCBMDOS_DIRECT;
        ret=true;
      }
      break;
    case NCBM_KERNEL_READST:
      if(curDev==8) {
        //DEBUG_OUT<<"READST\n";
        p1=0;
        ret=true;
      }
      break;
    case NCBM_KERNEL_SETLFS:  // SETLFS ()
      curChan=p1;
      curDev=p2;
      curSecond=p3;
      chanDev[curChan]=curDev;
      chanState[curChan]=NCBMDOS_UNKNOWN;
      chanSec[curChan]=curSecond;
      //ret=true;
      break;
    case NCBM_KERNEL_SETNAM:  // SETNAM ()
      nameLen=p1;
      nameAddr=p3*256+p2;
      //ret=true;
      break;
    default:
      DEBUG_OUT<<"***unhandled sys call at "<<loc<<"!!!\n";
      break;
  }
  if(!ret) {
    DEBUG_OUT<<"***fall through sys call at "<<loc<<"!!!\n";
  }
  if(ret) {
    map->write8(0x90,0);  // TODO: is this status for all CBM's?!?
    cpu->setRegister("a",p1);
    if(cpu) cpu->rts();
  }
  else {
    DEBUG_OUT<<"(instead "<<loc<<": "<<(int)n->origByte<<")\n";
    if(cpu) {
      cpu->jump(loc);
      cpu->executeInstead(n->origByte);
      ret=true;
    }
    else std::cerr<<"No cpu!\n";
  }
  return ret;
}


void NCbmKernHelper::doDOSCommand()
{
  unsigned int t=0,tt=0;
  unsigned int c[3];
  for(t=0;t<3;t++) c[t]=0;
  unsigned int tk=0,sc=0,cn=0;
  BYTE *tbuf=(BYTE *)NULL;
  wbuf[wbufPtr]=0;
  DEBUG_OUT<<"CMD: ";
  for(t=0;t<wbufPtr;t++) DEBUG_OUT<<(char)wbuf[t];
  DEBUG_OUT<<"\n";
  // TODO: interp at this point
  switch(wbuf[0]) {
    case 'I':
      // init drive, buffers, etc...
      break;
    case 'U':
      // "user" commands...
      switch(wbuf[1]) {
      case '1':
        // sector read...
        for(t=0;t<3;t++) c[t]=0;
        tt=0;
        for(t=0;t<wbufPtr;t++) {
          if(wbuf[t]==',') { wbuf[t]=0; c[tt]=t; tt++; }
        }
        aDefaultStringHelper.setDecMode();
        cn=aDefaultStringHelper.parseNumber((char *)(wbuf+c[0]+1));
        tk=aDefaultStringHelper.parseNumber((char *)(wbuf+c[1]+1));
        sc=aDefaultStringHelper.parseNumber((char *)(wbuf+c[2]+1));
        aDefaultStringHelper.setHexMode();
        dos->setDiskNum(chanDev[outFile]-8);
        dos->readTrackSector(tk,sc);
        DEBUG_OUT<<"U1:Read sector "<<tk<<","<<sc<<"\n";
        tbuf=dos->getBuffer();
        if(tbuf) {
          for(t=0;t<256;t++) {
            rbuf[t]=tbuf[t];
            //DEBUG_OUT<<(int)rbuf[t]<<",";
            //if(!(t&0xf)) DEBUG_OUT<<"\n";
          }
          //DEBUG_OUT<<"\n";
        }
        else std::cerr<<"nKernHelper U1, DOS didn't give us a buffer!!!!!\n";
        rbufPtr=0;
        wbufPtr=0;
        break;
      default:
        break;
      }
      break;
    default:
      break;
  }
  wbufPtr=0;
}


// TODO: Actually use the passed filename
unsigned int NCbmKernHelper::loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes)
{
  if(!filename) { std::cerr<<"NCbmKernHelper::loadDirectory No filename!\n"; return 0; }
  unsigned int t=0,origLoc=loc,nextLoc=0,locHolder=0;;
  char *types=(char *)NULL;
  DEBUG_OUT<<"loadDirectory "<<filename<<" at "<<loc<<"\n";
  EMemMapAbstract *map=dos->getMap();
  // Write dir header
  nextLoc=loc+30;
  map->write8(loc,LOW_BYTECBM(nextLoc)); loc++;  //link
  map->write8(loc,HIGH_BYTECBM(nextLoc)); loc++;
  map->write8(loc,2); loc++;  // line #
  map->write8(loc,0); loc++;
  /*
  map->write8(loc,32); loc++;  // spaces
  map->write8(loc,32); loc++;
  map->write8(loc,32); loc++;
  */
  map->write8(loc,18); loc++;  // rvs
  map->write8(loc,34); loc++;  // quote
  locHolder=loc;
  for(t=0;t<16;t++) { map->write8(loc,32); loc++; }
  loc=locHolder;
  unsigned int blocksFree=0;  // Not the real value
  const char *diskID=dos->getDiskID();
  // Note the skipped byte between the id and dos ver...
  char id1=32,id2=32,dos1=32,dos2=32;
  // because we can't have it be a zero or basic will get confused
  if(diskID[0]) id1=diskID[0];
  if(diskID[1]) id2=diskID[1];
  if(diskID[3]) dos1=diskID[3];
  if(diskID[4]) dos2=diskID[4];
  const char *diskName=dos->getDiskName();
  for(t=0;t<strlen(diskName);t++) {
    if(diskName[t]) map->write8(loc,ascii2petscii(diskName[t]));
    loc++;
  }
  loc=origLoc+22;
  map->write8(loc,34); loc++;  // quote
  map->write8(loc,32); loc++;  // space
  map->write8(loc,id1); loc++;
  map->write8(loc,id2); loc++;
  map->write8(loc,32); loc++;
  map->write8(loc,dos1); loc++;
  map->write8(loc,dos2); loc++;
  map->write8(loc,0); loc++;  // end of line
  AList *theDir=dos->getTheDir();
  theDir->reset();
  DDirEntry *aFile=(DDirEntry *)NULL;
  aFile=(DDirEntry *)theDir->info();
  while(aFile) {
    origLoc=loc;
    nextLoc=loc+32;
    map->write8(loc,LOW_BYTECBM(nextLoc)); loc++;  //link
    map->write8(loc,HIGH_BYTECBM(nextLoc)); loc++;
    map->write8(loc,aFile->sizeBlocks); loc++;  // line #
    map->write8(loc,0); loc++;
    if(aFile->sizeBlocks<10) { map->write8(loc,32); loc++; } // space
    if(aFile->sizeBlocks<100) { map->write8(loc,32); loc++; } // space
    map->write8(loc,34); loc++;  // quote
    locHolder=loc;
    for(t=0;t<22;t++) { map->write8(loc,32); loc++; }
    loc=locHolder;
    DEBUG_OUT<<"file: "<<aFile->name<<"...\n";
    for(t=0;t<strlen(aFile->name);t++) {
      map->write8(loc,ascii2petscii(aFile->name[t])); loc++;
    }
    map->write8(loc,34); loc++;  // quote
    loc=origLoc+27;
    if(aFile->sizeBlocks<10) loc++;
    if(aFile->sizeBlocks<100) loc++;
    switch(aFile->type) {
      case DDISK_TYPE_NONE:  types="PRG";  break;
      case DDISK_TYPE_BINARY:  types="PRG";  break;
      case DDISK_TYPE_BASIC:  types="PRG";  break;
      case DDISK_TYPE_TEXT:  types="SEQ";  break;
      case DDISK_TYPE_DIR:  types="DIR";  break;
      default: types="???"; break;
    }
    for(t=0;t<3;t++) { map->write8(loc,types[t]); loc++; }
    map->write8(loc,32); loc++;
    map->write8(loc,0); loc++;
    theDir->advance();
    aFile=(DDirEntry *)theDir->info();
  }
  origLoc=loc;
  nextLoc=loc+30;
  map->write8(loc,LOW_BYTECBM(nextLoc)); loc++;  //link
  map->write8(loc,HIGH_BYTECBM(nextLoc)); loc++;
  map->write8(loc,LOW_BYTECBM(blocksFree)); loc++;  // line #
  map->write8(loc,HIGH_BYTECBM(blocksFree)); loc++;
  locHolder=loc;
  for(t=0;t<29;t++) { map->write8(loc,32); loc++; }
  loc=locHolder;
  types="BLOCKS FREE.";
  for(t=0;t<strlen(types);t++) { map->write8(loc,types[t]); loc++; }
  loc=origLoc+29;
  map->write8(loc,0); loc++;
  map->write8(loc,0); loc++;  // link (end)
  map->write8(loc,0); loc++;
  DEBUG_OUT<<"loadDirectory done.\n";
  return (loc-origLoc);
}


unsigned int NCbmKernHelper::load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes)
{
  if(!filename) { std::cerr<<"NCbmKernHelper::load No filename!\n"; return 0; }
  char fname[256];
  for(unsigned int t=0;t<strlen(filename);t++) fname[t]=ascii2petscii(filename[t]);
  fname[strlen(filename)]=0;
  DEBUG_OUT<<"Attempting to load "<<fname<<" at "<<loc<<"\n";
  // NOTE: Fopen, etc are OUR versions, not the standard C ones...
  void *fd=dos->Fopen(fname,"rb");
  if(!fd) {
    std::cerr<<"Load Can't open file "<<fname<<"!\n";
    return 0;
  }
  ULONG tfsize=dos->getFileSize(fname);
  if(!tfsize) {
    std::cerr<<"Got a size of zero for "<<fname<<", probably a broken fseek...\n";
    return 0;
  }
  //DEBUG_OUT<<"curSeek is "<<dos->Ftell(fd)<<", should be 0\n";
  DEBUG_OUT<<"offset is "<<off<<"...\n";
  //for(unsigned int t=0;t<off;t++) fread(&c,1,1,fd);
  dos->Fseek(fd,off,SEEK_SET);
  //DEBUG_OUT<<"curSeek is "<<dos->Ftell(fd)<<", should be "<<off<<"\n";
  unsigned int rsize=tfsize-off;
  if(rsize>maxBytes) rsize=maxBytes;
  dos->readMany(fd,loc,rsize);
  //DEBUG_OUT<<"curSeek is "<<dos->Ftell(fd)<<", should be "<<(off+rsize)<<"\n";
  dos->Fclose(fd);
  return rsize;
}


unsigned int NCbmKernHelper::save(const char *filename,ULONG loc,unsigned int size)
{
  std::cerr<<"NCbmKernHelper::save not implemented!\n";
  exit(5);
  return 0;
}


char NCbmKernHelper::ascii2petscii(char c)
{
  char val=c;
  if((val<('Z'+1))&&(val>('A'-1))) val-='A'-'a';
  else {
    if((val<('z'+1))&&(val>('a'-1))) val+='A'-'a';
  }
  return val;
}
