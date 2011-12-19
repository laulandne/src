
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAppleIO.h>
#include <emu/NAppleHandler.h>
#include <emu/NAppleVideo.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/APrivateBitmap.h>

#include <portable/ndecoder/AImageDecoder.h>
#include <portable/ndecoder/ASmartImageDecoder.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>
#include <elib/EIOVia6522.h>
#include <elib/EIOScc8530.h>

#include <cpulib/Em6502.h>

#include <disklib/DControllerIWM.h>
#include <disklib/DControllerDisk2.h>
#include <disklib/DVfs.h>

#include <portable/nickcpp/AStringHelper.h>


//////////////////////////////////////////////////////////////////////////////
//  NAppleIO Class
//////////////////////////////////////////////////////////////////////////////

NAppleIO::NAppleIO(NAppleHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NAppleIO\n";
  disk2=NULL;
  smartDisk=NULL;
  scc=NULL;
  via1=NULL;
  via2=NULL;
  if(type==N_APPLE_IO_APPLE1) location=0xd000;
  else location=0xc000;
  resize(256);
  disk2=new DControllerDisk2(parent,0);
  if(!disk2) err.set();
  if(type==N_APPLE_IO_APPLE2GS) {
    smartDisk=new DControllerIWM(parent,0);
    if(!smartDisk) err.set();
    scc=new EIOScc8530(parent,0);
    if(!scc) err.set();
  }
  if(type==N_APPLE_IO_APPLE3) {
    via1=new EIOVia6522(parent,1);
    if(!via1) err.set();
    via2=new EIOVia6522(parent,2);
    if(!via2) err.set();
  }
  reset();
}


NAppleIO::~NAppleIO()
{
  if(disk2) delete disk2;
  disk2=NULL;
  if(smartDisk) delete smartDisk;
  smartDisk=NULL;
  if(scc) delete scc;
  scc=NULL;
  if(via1) delete via1;
  via1=NULL;
  if(via2) delete via2;
  via2=NULL;
}


void NAppleIO::reset()
{
  for(unsigned int t=0;t<8;t++) slotHandled[t]=false;
  if(disk2) disk2->reset();
  if(smartDisk) smartDisk->reset();
  if(scc) scc->reset();
  if(via1) via1->reset();
  if(via2) via2->reset();
  // Look for disk controller rom, if found activate slot 6
  ERom *foo=(ERom *)NULL;
  if(handler) {
    foo=(ERom *)handler->getMap()->findArea("disk2.rom");
    if(foo) slotHandled[6]=true;
  }
  s80store=false;  s80col=false;
  sRamrd=false; sRamwrt=false; sAltzp=false;
  sText=false; sHires=false; sMixed=false; sPage2=false;
  srdrom=false; slcbnk2=false; srombank=false; sintcxrom=false;
  newVideoReg=0; speedReg=0;
  sDhires=false; sIoudis=false;
  ann0=0;  ann1=0;  ann2=0;  ann3=0;
}


void NAppleIO::ioregs()
{
  DEBUG_OUT<<"Slots:\n";
  for(unsigned int t=0;t<8;t++) {
    DEBUG_OUT<<t<<" ";
    if(slotHandled[t]) DEBUG_OUT<<"on "; else DEBUG_OUT<<"off";
    DEBUG_OUT<<"  ";
    if(t==3) DEBUG_OUT<<"\n";
  }
  DEBUG_OUT<<"\n";
  if(disk2) disk2->ioregs();
  if(smartDisk) smartDisk->ioregs();
  if(scc) scc->ioregs();
  if(via1) via1->ioregs();
  if(via2) via2->ioregs();
}


void NAppleIO::io_cycle()
{
  if(disk2) disk2->io_cycle();
  if(smartDisk) smartDisk->io_cycle();
  if(scc) scc->io_cycle();
  if(via1) via1->io_cycle();
  if(via2) via2->io_cycle();
}


BYTE NAppleIO::write8(ULONG addr, BYTE value)
{
  switch(addr) {
    case 0xc000:
      s80store=false;
      // TODO: Map/video magic goes here...
      break;
    case 0xc001:
      s80store=true;
      // TODO: Map/video magic goes here...
      break;
    case 0xc002:
      sRamrd=false;
      // TODO: Map/video magic goes here...
      break;
    case 0xc003:
      sRamrd=true;
      // TODO: Map/video magic goes here...
      break;
    case 0xc004:
      sRamwrt=false;
      // TODO: Map/video magic goes here...
      break;
    case 0xc005:
      sRamwrt=true;;
      // TODO: Map/video magic goes here...
      break;
    case 0xc006:
      std::cerr<<"turn on cx rom not implemented!\n";
      exit(5);
      break;
    case 0xc007:
      std::cerr<<"turn off cx rom not implemented!\n";
      exit(5);
      break;
    case 0xc008:
      sAltzp=false;
      // TODO: Map/video magic goes here...
      break;
    case 0xc009:
      sAltzp=true;
      // TODO: Map/video magic goes here...
      break;
    case 0xc00a:
      sRamwrt=true;;
      // TODO: Map/video magic goes here...
      // NOTE: Should this also be turn on c3 rom?  (See c00b)
      break;
    case 0xc00b:
      std::cerr<<"turn off c3 rom not implemented!\n";
      exit(5);
      break;
    case 0xc00c:
      s80col=false;
      // TODO: Map/video magic goes here...
      break;
    case 0xc00d:
      s80col=true;
      // TODO: Map/video magic goes here...
      break;
    case 0xc010:
      strobe(addr,value);
      break;
    case 0xc011: case 0xc012: case 0xc013: case 0xc014: case 0xc015:
    case 0xc016: case 0xc017: case 0xc018: case 0xc019: case 0xc01a:
    case 0xc01b: case 0xc01c: case 0xc01d: case 0xc01e: case 0xc01f:
      // Read only registers
      break;
    case 0xc022:
      if(type==N_APPLE_IO_APPLE2GS) {
        BYTE v=(BYTE)((value&0xf)>>4);
        handler->getVideo()->setBackground(value&0xf);
      }
      break;
    case 0xc029:
      setNewVideoReg(value);
      break;
    case 0xc030:
      strobe(addr,value);
      break;
    case 0xc036:
      setSpeedReg(value);
      break;
    case 0xc040:
      strobe(addr,value);
      break;
    case 0xc050: case 0xc051: case 0xc052: case 0xc053:
    case 0xc054: case 0xc055:
    case 0xc056: case 0xc057:
    case 0xc058: case 0xc059: case 0xc05a: case 0xc05b: case 0xc05c: case 0xc05d:
    case 0xc05e: case 0xc05f:
      strobe(addr,value);
      break;
    case 0xc068:
      setStateReg(value);
      break;
    case 0xc070:
      strobe(addr,value);
      break;
    case 0xc073:
      std::cerr<<"banks sel >128k not implemented!\n";
      exit(5);
      break;
    case 0xc078:
      std::cerr<<"turn on iou access not implemented!\n";
      exit(5);
      break;
    case 0xc079:
      std::cerr<<"turn off iou access not implemented!\n";
      exit(5);
      break;
    case 0xc07e:
      std::cerr<<"turn on ioudis not implemented!\n";
      exit(5);
      break;
    case 0xc07f:
      std::cerr<<"turn off ioudis not implemented!\n";
      exit(5);
      break;
    case 0xc080: case 0xc081: case 0xc082: case 0xc083:
    case 0xc084: case 0xc085: case 0xc086: case 0xc087:
    case 0xc088: case 0xc089: case 0xc08a: case 0xc08b:
    case 0xc08c: case 0xc08d: case 0xc08e: case 0xc08f:
      strobe(addr,0);
      value=0xff;  // arbitrary
      break;
    //case 0xd010:
      // Apple1 keyboard data
      // (does writing this do anything?)
      //break;
    //case 0xd011:
      // Apple1 keyboard control (high bit says key ready)
      // (does writing this do anything?)
      //break;
    //case 0xd012:
      // Apple1 video char out
      // TODO: write char out
      //break;
    default:
      unsigned int adr=addr&0xfff0;
      switch(adr) {
        case 0xc080: write8_slot(0,addr,value); break;
        case 0xc090: write8_slot(1,addr,value); break;
        case 0xc0a0: write8_slot(2,addr,value); break;
        case 0xc0b0: write8_slot(3,addr,value); break;
        case 0xc0c0: write8_slot(4,addr,value); break;
        case 0xc0d0: write8_slot(5,addr,value); break;
        case 0xc0e0: write8_slot(6,addr,value); break;
        case 0xc0f0: write8_slot(7,addr,value); break;
        default:
          // Not a known register or a slot
          DEBUG_OUT<<"io write of "<<aDefaultStringHelper.myHex2(value)<<" at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
          exit(5);
          break;
      }
      break;
  }
  return value;
}


BYTE NAppleIO::read8(ULONG addr)
{
  BYTE val=mem[addr&0xfff];
  switch(addr) {
    case 0xc000:
      if(handler->getKeypress()) {
        val=(BYTE)handler->getKeypress();
        //if(val==A_KEY_SHIFT_KEY) val=0;
        //if(val==A_KEY_CTRL_KEY) val=0;
        if(val) val|=0x80;
        //DEBUG_OUT<<"keypress: "<<(short)val<<"\n";
        mem[0]=val;  // 0xc000
        if(type==N_APPLE_IO_APPLE3) handler->setKeypress(0);
      }
      break;
    case 0xc010:
      strobe(addr,0);
      val=0xff;  // arbitrary
      break;
    case 0xc011:
      std::cerr<<"read status of dx bank not implemented!\n";
      exit(5);
      break;
    case 0xc012:
      std::cerr<<"read status of dx ram/rom not implemented!\n";
      exit(5);
      break;
    case 0xc013:
      if(sRamrd) val=0xff; else val=0;
      break;
    case 0xc014:
      if(sRamwrt) val=0xff; else val=0;
      break;
    case 0xc015:
      std::cerr<<"read status of cx rom not implemented!\n";
      exit(5);
      break;
    case 0xc016:
      if(sAltzp) val=0xff; else val=0;
      break;
    case 0xc017:
      std::cerr<<"read status of c3 rom not implemented!\n";
      exit(5);
      break;
    case 0xc018:
      if(s80store) val=0xff; else val=0;
      break;
    case 0xc019:
      std::cerr<<"read status of vblank not implemented!\n";
      exit(5);
      break;
    case 0xc01a:
      if(sText) val=0xff; else val=0;
      break;
    case 0xc01b:
      if(sMixed) val=0xff; else val=0;
      break;
    case 0xc01c:
      if(sPage2) val=0xff; else val=0;
      break;
    case 0xc01d:
      if(sHires) val=0xff; else val=0;
      break;
    case 0xc01e:
      std::cerr<<"read status of primary/art charset not implemented!\n";
      exit(5);
      break;
    case 0xc01f:
      if(s80col) val=0xff; else val=0;
      break;
    case 0xc020:
      strobe(addr,0);
      val=0xff;  // arbitrary
      break;
    case 0xc029:
      val=readNewVideoReg();
      break;
    case 0xc030:
      strobe(addr,0);
      val=0xff;  // arbitrary
      break;
    case 0xc036:
      val=readSpeedReg();
      break;
    case 0xc050: case 0xc051: case 0xc052: case 0xc053:
    case 0xc054: case 0xc055:
    case 0xc056: case 0xc057:
    case 0xc058: case 0xc059: case 0xc05a: case 0xc05b: case 0xc05c: case 0xc05d:
    case 0xc05e: case 0xc05f:
      strobe(addr,0);
      val=0xff;  // arbitrary
      break;
    case 0xc060:
      std::cerr<<"read tape not implemented!\n";
      exit(5);
      break;
    case 0xc061:
      std::cerr<<"read status of open apple key not implemented!\n";
      exit(5);
      break;
    case 0xc062:
      std::cerr<<"read status of close apple key not implemented!\n";
      exit(5);
      break;
    case 0xc063:
      std::cerr<<"read status of shift key not implemented!\n";
      exit(5);
      break;
    case 0xc064:
      std::cerr<<"read status of analog 0 not implemented!\n";
      exit(5);
      break;
    case 0xc065:
      std::cerr<<"read status of analog 1 not implemented!\n";
      exit(5);
      break;
    case 0xc066:
      std::cerr<<"read status of analog 2 not implemented!\n";
      exit(5);
      break;
    case 0xc067:
      std::cerr<<"read status of analog 3 not implemented!\n";
      exit(5);
      break;
    case 0xc068:
      val=readStateReg();
      break;
    case 0xc070:
      strobe(addr,0);
      val=0xff;  // arbitrary
      break;
    case 0xc07e:
      std::cerr<<"read status of ioudis not implemented!\n";
      exit(5);
      break;
    case 0xc07f:
      std::cerr<<"read status of dhires not implemented!\n";
      exit(5);
      break;
    case 0xc080: case 0xc081: case 0xc082: case 0xc083:
    case 0xc084: case 0xc085: case 0xc086: case 0xc087:
    case 0xc088: case 0xc089: case 0xc08a: case 0xc08b:
    case 0xc08c: case 0xc08d: case 0xc08e: case 0xc08f:
      strobe(addr,0);
      val=0xff;  // arbitrary
      break;
      /*
    case 0xd010:
      // Apple1 keyboard data
      if(handler->getKeypress()) {
        val=(BYTE)handler->getKeypress();
        if(val) val|=0x80;
        //DEBUG_OUT<<"keypress: "<<(short)val<<"\n";
      }
      break;
    case 0xd011:
      // Apple1 keyboard control (high bit says key ready)
      if(handler->getKeypress()) val=0xff; else val=0;
      break;
    case 0xd012:
      // Apple1 video char out
      // Does reading this return anything interesting?
      break;
      */
    default:
      unsigned int adr=addr&0xfff0;
      switch(adr) {
        case 0xc080: val=read8_slot(0,addr);  break;
        case 0xc090: val=read8_slot(1,addr);  break;
        case 0xc0a0: val=read8_slot(2,addr);  break;
        case 0xc0b0: val=read8_slot(3,addr);  break;
        case 0xc0c0: val=read8_slot(4,addr);  break;
        case 0xc0d0: val=read8_slot(5,addr);  break;
        case 0xc0e0: val=read8_slot(6,addr);  break;
        case 0xc0f0: val=read8_slot(7,addr);  break;
        default:
          // Not a known register or a slot
          DEBUG_OUT<<"io read of "<<(short)val<<" at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
          exit(5);
          break;
      }
      break;
  }
  return val;
}


BYTE NAppleIO::strobe(ULONG addr,BYTE val)
{
  switch(addr) {
    case 0xc010:
      val=(BYTE)handler->getKeypress();
      mem[0]=val;  // 0xc000
      handler->setKeypress(0);
      break;
    case 0xc030:
      val=(BYTE)(mem[addr-location]+1);
      if(val==0x80) {
        //DEBUG_OUT<<"Beep!\n";
        //aDefaultStringHelper.systemBeep();
        val=0;
      }
      mem[addr-location]=val;
      break;
    case 0xc050:
      ((NAppleVideo *)handler->getVideo())->setGfx(true);
      sText=true;
      break;
    case 0xc051:
      ((NAppleVideo *)handler->getVideo())->setGfx(false);
      ((NAppleVideo *)handler->getVideo())->setSplitScreen(false);
      sText=false;
      break;
    case 0xc052:
      ((NAppleVideo *)handler->getVideo())->setSplitScreen(false);
      sMixed=false;
      break;
    case 0xc053:
      ((NAppleVideo *)handler->getVideo())->setSplitScreen(true);
      sMixed=true;
      break;
    case 0xc054:
      // NOTE: Doesn't handle 80col/aux mem yet...
      ((NAppleVideo *)handler->getVideo())->setPage2(false);
      sPage2=false;
      break;
    case 0xc055:
      // NOTE: Doesn't handle 80col/aux mem yet...
      ((NAppleVideo *)handler->getVideo())->setPage2(true);
      sPage2=true;
      break;
    case 0xc056:
      // NOTE: Doesn't handle 80col/aux mem yet...
      ((NAppleVideo *)handler->getVideo())->setGfxHigh(false);
      sHires=false;
      break;
    case 0xc057:
      // NOTE: Doesn't handle 80col/aux mem yet...
      ((NAppleVideo *)handler->getVideo())->setGfxHigh(true);
      sHires=true;
      break;
    case 0xc058:
      ann0=0;
      break;
    case 0xc059:
      ann0=1;
      break;
    case 0xc05a:
      ann1=0;
      break;
    case 0xc05b:
      ann1=1;
      break;
    case 0xc05c:
      ann2=0;
      break;
    case 0xc05d:
      ann2=1;
      break;
    case 0xc05e:
      if(sIoudis) sDhires=true; else ann3=0;
      break;
    case 0xc05f:
      if(sIoudis) sDhires=false; else ann3=1;
      break;
    case 0xc080:
      std::cerr<<"strobe read ram 2 no write not implemented!\n";
      exit(5);
      break;
    case 0xc081:
      std::cerr<<"strobe read rom write ram 2 not implemented!\n";
      exit(5);
      break;
    case 0xc082:
      std::cerr<<"strobe read rom no write not implemented!\n";
      exit(5);
      break;
    case 0xc083:
      std::cerr<<"strobe read write ram 2 not implemented!\n";
      exit(5);
      break;
    case 0xc084:
      std::cerr<<"strobe read ram 2 no write not implemented!\n";
      exit(5);
      break;
    case 0xc085:
      std::cerr<<"strobe read rom write ram 2 not implemented!\n";
      exit(5);
      break;
    case 0xc086:
      std::cerr<<"strobe read rom no write not implemented!\n";
      exit(5);
      break;
    case 0xc087:
      std::cerr<<"strobe read write ram 2 not implemented!\n";
      exit(5);
      break;
    case 0xc088:
      std::cerr<<"strobe read ram 1 no write not implemented!\n";
      exit(5);
      break;
    case 0xc08a:
      std::cerr<<"strobe read rom no write not implemented!\n";
      exit(5);
      break;
    case 0xc08b:
      std::cerr<<"strobe read write ram 1 not implemented!\n";
      exit(5);
      break;
    case 0xc08c:
      std::cerr<<"strobe read ram 1 no write not implemented!\n";
      exit(5);
      break;
    case 0xc08d:
      std::cerr<<"strobe read rom write ram 1 not implemented!\n";
      exit(5);
      break;
    case 0xc08e:
      std::cerr<<"strobe read rom no write not implemented!\n";
      exit(5);
      break;
    case 0xc08f:
      std::cerr<<"strobe read write ram 1 not implemented!\n";
      exit(5);
      break;
    default:
      DEBUG_OUT<<"io strobe at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
      exit(5);
      break;
  }
  return val;
}


BYTE NAppleIO::read8_slot(unsigned int num,unsigned int addr)
{
  BYTE ret=0;
  if(slotHandled[num]) {
    switch(num) {
      case 6:  // 5.25 in disk controller
        if(disk2) ret=disk2->read8Reg(addr-0xc600);
        break;
      default:
        DEBUG_OUT<<"Read of slot $"<<num<<" at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
        exit(5);
        break;
    }
  }
  else {
    DEBUG_OUT<<"Read of slot $"<<num<<" at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
    exit(5);
  }
  return ret;
}


BYTE NAppleIO::write8_slot(unsigned int num,unsigned int addr,BYTE val)
{
  BYTE ret=0;
  if(slotHandled[num]) {
    switch(num) {
      case 6:  // 5.25 in disk controller
        if(disk2) disk2->write8Reg(addr-0xc600,val);
        break;
      default:
        DEBUG_OUT<<"write slot $"<<num<<" "<<(short)val<<" to "<<aDefaultStringHelper.myHex4(addr)<<"\n";
        exit(5);
        break;
    }
  }
  else {
    DEBUG_OUT<<"write slot $"<<num<<" "<<(short)val<<" to "<<aDefaultStringHelper.myHex4(addr)<<"\n";
    exit(5);
  }
  return ret;
}


BYTE NAppleIO::strobe8_slot(unsigned int num,unsigned int addr,BYTE val)
{
  BYTE ret=0;
  if(slotHandled[num]) {
    switch(num) {
      case 6:  // 5.25 in disk controller
        if(disk2) disk2->write8Reg(addr-0xc600,val);
        break;
      default:
        DEBUG_OUT<<"strobe slot $"<<num<<" "<<(short)val<<" to "<<aDefaultStringHelper.myHex4(addr)<<"\n";
        exit(5);
        break;
    }
  }
  else {
    DEBUG_OUT<<"strobe slot $"<<num<<" "<<(short)val<<" to "<<aDefaultStringHelper.myHex4(addr)<<"\n";
    exit(5);
  }
  return ret;
}


BYTE NAppleIO::read8apple3(ULONG addr)
{
  BYTE page=(BYTE)((addr>>8)&0xff);
  BYTE value=mem[addr-location];  // TODO: some range checking...
  switch(addr) {
    // TODO: obviously a lot more here...
    default:
      DEBUG_OUT<<"io read8apple3 at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
      exit(5);
      break;
  }
  return value;
}


BYTE NAppleIO::write8apple3(ULONG addr, BYTE value)
{
  BYTE page=(BYTE)((addr>>8)&0xff);
  switch(addr) {
    // TODO: obviously a lot more here...
    default:
      DEBUG_OUT<<"io write8apple3 of "<<(short)value<<" at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
      exit(5);
      break;
  }
  return value;
}


/* new video reg at c029
bit 7 super hires vs apple2 modes
bit 6 linear map vs apple2 map
bit 5 color or mono double hires
bits 4-1 reserved
bit 0 enable bank latch
*/
void NAppleIO::setNewVideoReg(BYTE val)
{
  if(type!=N_APPLE_IO_APPLE2GS) { DEBUG_OUT<<"setNewVideoReg on non-gs...\n"; return; }
  newVideoReg=val;
  DEBUG_OUT<<"NewVideoReg set: ";
  if(val&0x80) DEBUG_OUT<<"superhires "; else DEBUG_OUT<<"apple2 ";
  if(val&0x40) DEBUG_OUT<<"linear "; else DEBUG_OUT<<"ap2map ";
  if(val&0x20) DEBUG_OUT<<"color "; else DEBUG_OUT<<"mono ";
  if(val&0x01) DEBUG_OUT<<"banklatch "; else DEBUG_OUT<<"banknotlatched ";
  DEBUG_OUT<<"\n";
}


BYTE NAppleIO::readNewVideoReg()
{
  if(type!=N_APPLE_IO_APPLE2GS) { DEBUG_OUT<<"readNewVideoReg on non-gs...\n"; return 0; }
  return newVideoReg;
}


/* speed reg at c036
bit 7 cpu speed
bit 6 power on status
bit 5 reserved
bit 4 shadow all banks or just 00/01
bit 0-3 disk2 drive motors
*/
void NAppleIO::setSpeedReg(BYTE val)
{
  if(type!=N_APPLE_IO_APPLE2GS) { DEBUG_OUT<<"setSpeedReg on non-gs...\n"; return; }
  speedReg=val;
  DEBUG_OUT<<"SpeedReg set: ";
  if(val&0x80) DEBUG_OUT<<"cpufast "; else DEBUG_OUT<<"cpuslow ";
  if(val&0x40) DEBUG_OUT<<"poweron "; else DEBUG_OUT<<"poweroff ";
  if(val&0x10) DEBUG_OUT<<"shadowall "; else DEBUG_OUT<<"shadow0001only ";
  DEBUG_OUT<<" disks ";
  if(val&0x08) DEBUG_OUT<<"1 "; else DEBUG_OUT<<"0 ";
  if(val&0x04) DEBUG_OUT<<"1 "; else DEBUG_OUT<<"0 ";
  if(val&0x02) DEBUG_OUT<<"1 "; else DEBUG_OUT<<"0 ";
  if(val&0x01) DEBUG_OUT<<"1 "; else DEBUG_OUT<<"0 ";
  DEBUG_OUT<<"\n";
}


BYTE NAppleIO::readSpeedReg()
{
  if(type!=N_APPLE_IO_APPLE2GS) { DEBUG_OUT<<"readSpeedReg on non-gs...\n"; return 0; }
  return speedReg;
}


/* state reg at c068
bit 7 altzp
bit 6 page2
bit 5 ramrd
bit 4 ramwrt
bit 3 rdrom
bit 2 lcbnk2
bit 1 rombank
bit 0 intcxrom
*/
void NAppleIO::setStateReg(BYTE val)
{
  if(type!=N_APPLE_IO_APPLE2GS) { DEBUG_OUT<<"setStateReg on non-gs...\n"; return; }
  // TODO: when values are changed, do map magic!
  if(val&0x80) sAltzp=true; else sAltzp=false;
  if(val&0x40) sPage2=true; else sPage2=false;
  if(val&0x20) sRamrd=true; else sRamrd=false;
  if(val&0x10) sRamwrt=true; else sRamwrt=false;
  if(val&0x08) srdrom=true; else srdrom=false;
  if(val&0x04) slcbnk2=true; else slcbnk2=false;
  if(val&0x02) srombank=true; else srombank=false;
  if(val&0x01) sintcxrom=true; else sintcxrom=false;
}


BYTE NAppleIO::readStateReg()
{
  if(type!=N_APPLE_IO_APPLE2GS) { DEBUG_OUT<<"readStateReg on non-gs...\n"; return 0; }
  BYTE val=0;
  if(sAltzp) val|=0x80;
  if(sPage2) val|=0x40;
  if(sRamrd) val|=0x20;
  if(sRamwrt) val|=0x10;
  if(srdrom) val|=0x08;
  if(slcbnk2) val|=0x04;
  if(srombank) val|=0x02;
  if(sintcxrom) val|=0x01;
  return val;
}

