
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAppleDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>

#include <cpulib/Em6502.h>

#include <disklib/DControllerIWM.h>
#include <disklib/DControllerDisk2.h>
#include <disklib/DVfs.h>

#include <portable/nickcpp/AStringHelper.h>

/*
Notes:

When dos is booting...
0200-03ff: nibble buffers
0800-08ff: first stage boot
1b00-1d00: dos reloc code
1d00-3fff: dos
9600-9cf8: three buffers of 595 (253) bytes (dos himem points here)
9d00-bfff: relocated dos

reconnect dos:
48k: 9dbf
32k: 5dbf
16k: 1dbf

I/O block example
c0a i/o block indicator (01)
c0b slot number times 6 (60)
c0c drive number (01)
c0d expected vol number (00)
c0e track num (12)
c0f sector num (06)
c10 low addr of dev char table (20)
c11 high addr of dev char table (0c)
c12 low addr of data buf (00)
c13 high addr of data buf (20)
c14 unused (00)
c15 unused (00)
c16 command code (02)
c17 error code (00)
c18 actual vol number (00)
c19 previous slot num (60)
c1a previous drive num (01)

dev char table example
c20 dev type code (00)
c21 num phases per track (01)
c22 time count (ef)
c23 time count (d8)
(These are the only valid vals for disk drives)

I/O block
01 1BTYPE
02 1BSLOT
03 IBDRVN
04 IBVOL (0=any)
05 IBTRK
06 IBSECT
07-08 IBBUFP
0b-0c unused
0d IBCMD (0=start and position head, 1=read, 2=write, 4=format)
0e IBSTAT (0=none, 10=write protected, 20=vol num didn't match, 40=drive error)
0f IBSMOD
10 IOBPSN
11 IOBPDN

3d9 RWTS
a=high addr of I/O block
y=low addr of I/O block

03f2-03f3: addr to handle reset for autostart rom
03f4: cold-start byte
03fe-03ff: add to handle nmi

dos loading...
track 0, sectors 0-9 go to $3600-$3f00 or $b600-$bf00 (reloc)
track 0, sectors a-f go to $1b00-$2000 or (skipping a and b) $9d00-$a000 (reloc)
track 1, sectors 0-f go to $2100-$3000 or $a100-$b000 (reloc)
track 2, sectors 0-4 go to $3100-$3500 or $b100-$b500 (reloc)

*/


unsigned int NAppleSectorSkew[] = {
  0,7,14,6,13,5,12,4,11,3,10,2,9,1,8,15
};


//////////////////////////////////////////////////////////////////////////////
//  NAppleDos Class
//////////////////////////////////////////////////////////////////////////////

NAppleDos::NAppleDos(EHandler *parent) : EOSHelper(parent)
{
  init();
  io=parent;
  reset();
}


NAppleDos::~NAppleDos()
{
  // TODO: free kernal entries
}


void NAppleDos::reset()
{
  EOSHelper::reset();
  curSlot=0;  curDrive=0;  curVolume=0;
  clearBuffer();
  // TODO: There is a jump at 0x3d0...it points to a dos entry...
  // could put a brk to where it points.
  // Also, could put brk's at 0x3e3 and 0x3d9 to create fake dos...
  // or, later, patch it as we should...
}


bool NAppleDos::systemCall(ULONG loc)
{
  if(!cpu) {
    reset();
    if(!cpu) {
      std::cerr<<"NAppleDos Couldn't handle the systemCall, no cpu!\n";
      return false;
    }
  }
  if(!dos) {
    reset();
    if(!dos) {
       std::cerr<<"NAppleDos Couldn't handle the systemCall, no dos!\n";
       return false;
    }
  }
  EMemMapAbstract *map=dos->getMap();
  if(!map) return false;
  unsigned int i=0;
  BYTE *tbuf=(BYTE *)NULL;
  unsigned int c[3];
  for(i=0;i<3;i++) c[i]=0;
  bool ret=false;
  a=cpu->readRegister("a");
  x=cpu->readRegister("x");
  y=cpu->readRegister("y");
  EOSEntry *n=findActual(loc);
  unsigned int original=0;
  if(n) original=n->origAddr;
  if(!original) return false;
  //unsigned int d=w->getHexDigits();
  //w->setHexDigits(2);
  DEBUG_OUT<<"sysCall:"<<aDefaultStringHelper.myHex((unsigned short)original)<<" "<<aDefaultStringHelper.myHex((unsigned short)loc)<<"\n";
  bool doIt=false;
  unsigned int t,s,a;
  switch(original) {
    case 0xc65c:
      // This is BTRDSEC, the boot sector reader
      t=map->read8(0x41);
      s=map->read8(0x3d);
      a=map->read16l(0x26);
      readSector(t,s,a);
      loc=0xc6f8;
      n->origByte=0x4c;
      map->write8(0x3d,s+1);
      map->write16l(0x26,a+256);
      break;
    default:
      DEBUG_OUT<<"***unhandled sys call at "<<aDefaultStringHelper.myHex((unsigned short)loc)<<"!!!\n";
      break;
  }
  //if(!ret) {
    //DEBUG_OUT<<"***fall through sys call at "<<aDefaultStringHelper.myHex((unsigned short)loc)<<"!!!\n";
  //}
  //w->setHexDigits(d);
  if(ret) {
    DEBUG_OUT<<"ret was set...\n";
    if(cpu) cpu->rts();
  }
  else {
    DEBUG_OUT<<"(instead "<<loc<<": "<<(short)n->origByte<<")\n";
    if(cpu) {
      DEBUG_OUT<<"Going to jmp...\n";
      cpu->jump(loc);
      //cpu->executeInstead(n->origByte);
      ret=true;
    }
  }
  return ret;
}


void NAppleDos::readTrack(unsigned int tnum)
{
}


void NAppleDos::decodeTrack()
{
  // legal values are 0x96-0xff
}


void NAppleDos::clearBuffer()
{
  for(unsigned int t=0;t<N_APPLE_TRACKBUF_SIZE;t++) trackBuf[t]=0;
}


void NAppleDos::readSector(unsigned int t,unsigned int s,ULONG addr)
{
  unsigned int ss=NAppleSectorSkew[s];
  DEBUG_OUT<<"readSector: "<<t<<","<<ss<<" ("<<s<<" pre-skew) at "<<addr<<"\n";
  if(!dos) { std::cerr<<"No DOS!\n";  return; }
  dos->readTrackSector(t,ss);
  BYTE *buf=dos->getBuffer();
  for(unsigned int i=0;i<256;i++) map->write8(a+i,buf[i]);
  //map->setDebug(true);
}


unsigned int NAppleDos::load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes)
{
  std::cerr<<"NAppleDos::load not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NAppleDos::save(const char *filename,ULONG loc,unsigned int size)
{
  std::cerr<<"NAppleDos::save not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NAppleDos::loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes)
{
  std::cerr<<"NAppleDos::loadDirectory not implemented!\n";
  exit(5);
  return 0;
}
