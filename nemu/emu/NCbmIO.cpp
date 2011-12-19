
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


//#define CHECK_C64_MAPPING 1


#include <emu/NCbmHandler.h>
#include <emu/NCbmIO.h>
#include <emu/NCbmVideo.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/AEvent.h>
//#include <guilib/APrivateBitmap.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EKeys.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>
#include <elib/EMemMapAbstract.h>
#include <elib/EIOPia.h>
#include <elib/EIOPia6520.h>
#include <elib/EIOCia6526.h>
#include <elib/EIOVia6522.h>
#include <elib/EIOTia6525.h>
#include <elib/ESoundIOSid.h>
#include <elib/EIOAcia6551.h>

#include <disklib/DController65.h>
#include <disklib/DVfs.h>

#include <cpulib/Em6502.h>


#define NCBM_WANT_KERN_TABLE 1
#include <emu/generated/NCbm_kern_tab.h>


#define HIGH_BYTECBM(x)  ((x&0xff00)>>8)
#define LOW_BYTECBM(x)  (x&0xff)


///////////////////////////////////////////////////////////////////////////////
//  NCbmIO Class
///////////////////////////////////////////////////////////////////////////////

NCbmIO::NCbmIO(NCbmHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NCbmIO\n";
  irq2en=0;  irq1en=0;
  ta_int=0; tb_int=0;
  state1a=0;  state1b=0;  state2a=0;  state2b=0;
  tlatch1a=0;  tlatch1b=0;  tlatch2a=0;  tlatch2b=0;
  timer1a=0;  timer1b=0;  timer2a=0;  timer2b=0;
  free1a=false;  free1b=false;  free2a=false;  free2b=false;
  run1a=false;  run1b=false;  run2a=false;  run2b=false;
  IFR1=0;  IFR2=0;
  xia1=NULL;  xia2=NULL;
  tpia1=NULL;  tpia2=NULL;
  //sid1=NULL;  sid2=NULL;
  vic3ModeKey=0x00;
  vic3ModeFlag=false;
  dma0=0;  dma1=0;  dma2=0;
  vic3Red=(BYTE *)NULL;  vic3Green=(BYTE *)NULL;  vic3Blue=(BYTE *)NULL;
  vdc=(ERam *)NULL;  vdcMem=(ERam *)NULL;
  vdcReg=0;  vdcAddr=0;
  fdc65=NULL;
  acia=NULL;
  colorRamExtend=false;
  paletteMode=false;
  RAST_IRQ=false;
  raster_check=0;
  nscreen_addr=0;  nchar_addr=0;  ncolor_addr=0;
  switch(type) {
    case N_CBM_IO_C64:
      name=strdup("io.d000");
      location=0xd000;
      resize(4096);
      ta_int=0x01; tb_int=0x02;
      IFR1=0xdc0d;  IFR2=0xdd0d;
      xia1=new EIOCia6526(parent,0);
      xia2=new EIOCia6526(parent,0);
      //sid1=new ESoundIOSid(parent,0);
      break;
    case N_CBM_IO_B:
      name=strdup("io.fd800");
      location=0xfd800;
      resize(2048);
      ta_int=0x01; tb_int=0x02;
      IFR1=0xfdb0d;  IFR2=0xfdc0d;
      vdc=new ERam(64);
      vdcMem=new ERam(16384);
      // We add these to the map (but never map them) to keep track of them...
      handler->getMap()->addArea(vdc);
      handler->getMap()->addArea(vdcMem);
      xia1=new EIOCia6526(parent,0);
      tpia1=new EIOTia6525(parent,0);
      tpia2=new EIOTia6525(parent,0);
      acia=new EIOAcia6551(parent,0);
      //sid1=new ESoundIOSid(parent,0);
      break;
    case N_CBM_IO_C128:
      name=strdup("io.d000");
      location=0xd000;
      resize(4096);
      vdc=new ERam(64);
      vdcMem=new ERam(16384);
      // We add these to the map (but never map them) to keep track of them...
      handler->getMap()->addArea(vdc);
      handler->getMap()->addArea(vdcMem);
      ta_int=0x01; tb_int=0x02;
      IFR1=0xdc0d;  IFR2=0xdd0d;
      xia1=new EIOCia6526(parent,0);
      xia2=new EIOCia6526(parent,0);
      //sid1=new ESoundIOSid(parent,0);
      break;
    case N_CBM_IO_C65:
      name=strdup("io.d000");
      location=0xd000;
      resize(4096);
      // TODO: test for failure of the following...
      vic3Red=(BYTE *)calloc(256,1);
      vic3Green=(BYTE *)calloc(256,1);
      vic3Blue=(BYTE *)calloc(256,1);
      fdc65=new DController65(parent,0);
      ta_int=0x01; tb_int=0x02;
      IFR1=0xdc0d;  IFR2=0xdd0d;
      xia1=new EIOCia6526(parent,0);
      xia2=new EIOCia6526(parent,0);
      //sid1=new ESoundIOSid(parent,0);
      //sid2=new ESoundIOSid(parent,0);
      break;
    case N_CBM_IO_VIC:
      name=strdup("io.9000");
      location=0x9000;
      resize(4096);
      ta_int=0x40; tb_int=0x20;
      IFR1=0x911d;  IFR2=0x912d;
      xia1=new EIOVia6522(parent,0);
      xia2=new EIOVia6522(parent,0);
      break;
    case N_CBM_IO_TED:
      //name=strdup("io.9000"));
      //location=0x9000;
      //resize(4096);
      //ta_int=0x40; tb_int=0x20;
      //IFR1=0x911d;  IFR2=0x912d;
      acia=new EIOAcia6551(parent,0);
      break;
    case N_CBM_IO_PET:
      name=strdup("io.e800");
      location=0xe800;
      resize(256);
      //ta_int=0x40; tb_int=0x20;
      //IFR1=0x911d;  IFR2=0x912d;
      xia1=new EIOVia6522(parent,0);
      tpia1=new EIOPia6520(parent,0);
      tpia2=new EIOPia6520(parent,0);
      break;
    default:
      debugger("Unknown IO type!");
      err.set();
      exit(5);
      break;
  }
  //reset();
}


NCbmIO::~NCbmIO()
{
  if(err.error()) { DEBUG_OUT<<"EMemIO with error!\n"; return; }
  //if(mapped) handler->getMap()->unmap(this);
  //if(!error()) freeMem();
  if(vdc) delete vdc;
  vdc=(ERam *)NULL;
  if(vdcMem) delete vdcMem;
  vdcMem=(ERam *)NULL;
  if(vic3Red) free(vic3Red);
  vic3Red=(BYTE *)NULL;
  if(vic3Green) free(vic3Green);
  vic3Green=(BYTE *)NULL;
  if(vic3Blue) free(vic3Blue);
  vic3Blue=(BYTE *)NULL;
  if(fdc65) delete fdc65;
  fdc65=NULL;
  if(xia1) delete xia1;
  xia1=NULL;
  if(xia2) delete xia2;
  xia2=NULL;
  //if(sid1) delete sid1;
  //sid1=NULL;
  //if(sid2) delete sid2;
  //sid2=NULL;
  if(tpia1) delete tpia1;
  tpia1=NULL;
  if(tpia2) delete tpia2;
  tpia2=NULL;
  if(acia) delete acia;
  acia=NULL;
  //err.set();
}


void NCbmIO::ioregs()
{
  //if(win) {
    DEBUG_OUT<<"IRQs: ";
    DEBUG_OUT<<"irq1en="<<(short)irq1en<<"  irq2en="<<(short)irq2en<<"\n";
    DEBUG_OUT<<"Timers:\n";
    DEBUG_OUT<<"Latches="<<tlatch1a<<","<<tlatch1b<<",";
    DEBUG_OUT<<tlatch2a<<","<<tlatch2b<<"\n";
    DEBUG_OUT<<"Values="<<timer1a<<","<<timer1b<<",";
    DEBUG_OUT<<timer2a<<","<<timer2b<<"\n";
    DEBUG_OUT<<"Continuous="<<free1a<<","<<free1b<<",";
    DEBUG_OUT<<free2a<<","<<free2b<<"\n";
    DEBUG_OUT<<"Running="<<run1a<<","<<run1b<<",";
    DEBUG_OUT<<run2a<<","<<run2b<<"\n";
    DEBUG_OUT<<"key_count="<<(short)((NCbmHandler *)handler)->key_count<<"  key_max=";
    DEBUG_OUT<<(short)((NCbmHandler *)handler)->key_max<<"\n";
    DEBUG_OUT<<"RAST_IRQ="<<RAST_IRQ<<"\n";
    unsigned int t,x;
    if(fdc65) fdc65->ioregs();
    if(xia1) xia1->ioregs();
    if(xia2) xia2->ioregs();
    if(tpia1) tpia1->ioregs();
    if(tpia2) tpia2->ioregs();
    if(acia) acia->ioregs();
    //if(sid1) sid1->ioregs();
    //if(sid2) sid2->ioregs();
    switch(type) {
      case N_CBM_IO_C65:
        if(vic3ModeFlag) DEBUG_OUT<<"Vic3 in native mode.\n";
        else DEBUG_OUT<<"Vic3 in Vic2 mode.\n";
        if(((NCbmHandler *)handler)->getC64Mode()) DEBUG_OUT<<"System in C64 Mode\n";
        break;
      case N_CBM_IO_C128:
        if(((NCbmHandler *)handler)->getC64Mode()) DEBUG_OUT<<"System in C64 Mode\n";
        DEBUG_OUT<<"VDC: (reg:"<<(short)vdcReg<<", addr="<<vdcAddr<<")\n";
        for(t=0;t<5;t++) {
          DEBUG_OUT<<(t*8)<<": ";
          for(x=0;x<8;x++) if(vdc) DEBUG_OUT<<(short)vdc->read8(t*8+x)<<" ";
          DEBUG_OUT<<"\n";
        }
        break;
      case N_CBM_IO_B:
        DEBUG_OUT<<"VDC: (reg:"<<(short)vdcReg<<", addr="<<vdcAddr<<")\n";
        for(t=0;t<5;t++) {
          DEBUG_OUT<<(t*8)<<": ";
          for(x=0;x<8;x++) if(vdc) DEBUG_OUT<<(short)vdc->read8(t*8+x)<<" ";
          DEBUG_OUT<<"\n";
        }
        break;
      default:
        break;
    }
  //}
}


void NCbmIO::reset()
{
  unsigned int t;
  ((NCbmHandler *)handler)->key_count=0;  ((NCbmHandler *)handler)->key_max=0;
  for(t=0;t<NCBM_MAX_KEY_ROWS;t++) ((NCbmHandler *)handler)->keys[t]=0;
  irq2en=0;  irq1en=0;
  run1a=false;  run1b=false;  run2a=false;  run2b=false;
  state1a=0;  state1b=0;  state2a=0;  state2b=0;
  tlatch1a=0;  tlatch1b=0;  tlatch2a=0;  tlatch2b=0;
  free1a=false;  free1b=false;  free2a=false;  free2b=false;
  timer1a=0;  timer1b=0;  timer2a=0;  timer2b=0;
  vic3ModeKey=0x00;
  vic3ModeFlag=false;
  colorRamExtend=false;
  dma0=0;  dma1=0;  dma2=0;
  paletteMode=false;
  if(fdc65) fdc65->reset();
  if(xia1) xia1->reset();
  if(xia2) xia2->reset();
  if(tpia1) tpia1->reset();
  if(tpia2) tpia2->reset();
  if(acia) acia->reset();
  //if(sid1) sid1->reset();
  //if(sid2) sid2->reset();
  switch(type) {
    case N_CBM_IO_C128:
      for(t=0;t<64;t++) if(vdc) vdc->write8(t,0);
      break;
    case N_CBM_IO_B:
      for(t=0;t<64;t++) if(vdc) vdc->write8(t,0);
      //write8(location+0,0xff);  write8(location+1,0xff);
      break;
    case N_CBM_IO_C65:
      for(t=0;t<0xff;t++) c65_df[t]=0;
      //write8(location+0,0xff);  write8(location+1,0xff);
      break;
    case N_CBM_IO_C64:
      //write8(location+0,0xff);  write8(location+1,0xff);
      break;
    default:
      break;
  }
  //for(t=0;t<memsize;t++) write8(location+t,0);
  if(type==N_CBM_IO_B) {
    // PET-style hardwired raster irq...
    RAST_IRQ=true;
    if(handler->getVideo()) {
      handler->getVideo()->setRasterFactor(10);
      handler->getVideo()->setRasterSpeed(1);
      handler->getVideo()->setRasterInt(raster_check,E6502_INT_IRQ);
    }
  }
}


void NCbmIO::io_cycle()
{
  //DEBUG_OUT<<"~";
  if(fdc65) fdc65->io_cycle();
  if(xia1) xia1->io_cycle();
  if(xia2) xia2->io_cycle();
  if(tpia1) tpia1->io_cycle();
  if(tpia2) tpia2->io_cycle();
  if(acia) acia->io_cycle();
  //if(sid1) sid1->io_cycle();
  //if(sid2) sid2->io_cycle();
  // Processing
  unsigned int speed=handler->getTimerSpeed();
  if(run1a) {
    if(timer1a) timer1a-=speed;
    if(timer1a<0) timer1a=0;
    if(!timer1a) {
      //DEBUG_OUT<<"%";
      timer1a=0;
      if(irq1en&ta_int) {
        if(handler->getCPU()) handler->getCPU()->cause_int(E6502_INT_IRQ);
        if(IFR1) {
          temp=read8(IFR1);
          temp=temp|0x80;
          temp=temp|ta_int;
          write8(IFR1,temp);
        }
      }
      if(free1a) timer1a=tlatch1a;
      else run1a=false;
    }
  }
  if(run1b) {
    if(timer1b) timer1b-=speed;
    if(timer1b<0) timer1b=0;
    if(!timer1b) {
      //DEBUG_OUT<<"%";
      timer1b=0;
      if(irq1en&tb_int) {
        if(handler->getCPU()) handler->getCPU()->cause_int(E6502_INT_IRQ);
        if(IFR1) {
          temp=read8(IFR1);
          temp=temp|0x80;
          temp=temp|tb_int;
          write8(IFR1,temp);
        }
      }
      if(free1b) timer1b=tlatch1b;
      else run1b=false;
    }
  }
  if(run2a) {
    if(timer2a) timer2a-=speed;
    if(timer2a<0) timer2a=0;
    if(!timer2a) {
      //DEBUG_OUT<<"%";
      timer2a=0;
      if(irq2en&ta_int) {
        if(handler->getCPU()) handler->getCPU()->cause_int(E6502_INT_IRQ);
        if(IFR2) {
          temp=read8(IFR2);
          temp=temp|0x80;
          temp=temp|ta_int;
          write8(IFR2,temp);
        }
      }
      if(free2a) timer2a=tlatch2a;
      else run2a=false;
    }
  }
  if(run2b) {
    if(timer2b) timer2b-=speed;
    if(timer2b<0) timer2b=0;
    if(!timer2b) {
      //DEBUG_OUT<<"%";
      timer2b=0;
      if(irq2en&tb_int) {
        if(handler->getCPU()) handler->getCPU()->cause_int(E6502_INT_IRQ);
        if(IFR2) {
          temp=read8(IFR2);
          temp=temp|0x80;
          temp=temp|tb_int;
          write8(IFR2,temp);
        }
      }
      if(free2b) timer2b=tlatch2b;
      else run2b=false;
    }
  }
  // Post processing
  if(type==N_CBM_IO_VIC) {
    move_timer(0x9114,timer1a);
    move_timer(0x9116,timer1a);
    move_timer(0x9118,timer1b);
    move_timer(0x9124,timer2a);
    move_timer(0x9126,timer2a);
    move_timer(0x9128,timer2b);
  }
  if(type==N_CBM_IO_B) {
    move_timer(0xfdc04,timer1a);
    move_timer(0xfdc06,timer2a);
  }
  if(type==N_CBM_IO_C64) {
    move_timer(0xdc04,timer1a);
    move_timer(0xdc06,timer1b);
    move_timer(0xdd04,timer2a);
    move_timer(0xdd06,timer2b);
  }
  if(type==N_CBM_IO_C65) {
    move_timer(0xdc04,timer1a);
    move_timer(0xdc06,timer1b);
    move_timer(0xdd04,timer2a);
    move_timer(0xdd06,timer2b);
  }
  if(type==N_CBM_IO_C128) {
    move_timer(0xdc04,timer1a);
    move_timer(0xdc06,timer1b);
    move_timer(0xdd04,timer2a);
    move_timer(0xdd06,timer2b);
  }
}


void NCbmIO::move_timer(ULONG address, UINT16 value)
{
  mem[address-location]=value&0x00ff;
  mem[address-location+1]=(value&0xff00)>>8;
}


BYTE NCbmIO::write8(ULONG address,BYTE value)
{
  BYTE page=(address>>8)&0xff;
  BYTE nib2=address&0x00f0;
  switch(type) {
    case N_CBM_IO_C64:
      switch(page) {
        case 0xd0: case 0xd1: case 0xd2: case 0xd3:
          value=write8_vic2(address,value); break;
        case 0xd4: case 0xd5: case 0xd6: case 0xd7:
          value=write8_sid(address,value); break;
        case 0xd8: case 0xd9: case 0xda: case 0xdb:
          // color ram
          if(handler->getVideo()) value=handler->getVideo()->write8ColorMem(address,value);
          break;
        case 0xdc: case 0xdd:
          value=write8_cias(address,value); break;
        default:
          DEBUG_OUT<<"io write of "<<aDefaultStringHelper.myHex2(value)<<" at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
   case N_CBM_IO_C128:
      switch(page) {
        case 0xd0: case 0xd1: case 0xd2: case 0xd3:
          value=write8_vic2(address,value); break;
        case 0xd4:
          value=write8_sid(address,value); break;
        case 0xd5:
          value=write8_c128mmu(address,value); break;
        case 0xd6:
          value=write8_c128vdc(address,value); break;
        case 0xd8: case 0xd9: case 0xda: case 0xdb:
          // color ram
          if(handler->getVideo()) value=handler->getVideo()->write8ColorMem(address,value);
          break;
        case 0xdc: case 0xdd:
          value=write8_cias(address,value); break;
        default:
          DEBUG_OUT<<"io write of "<<aDefaultStringHelper.myHex2(value)<<" at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    case N_CBM_IO_C65:
      switch(page) {
        case 0xd0:
          switch(nib2) {
            case 0x80:  case 0x90:
              if(vic3ModeFlag) value=fdc65->write8(address&0xf,value);
              else value=write8_vic2(address,value);
              break;
            case 0xa0:  case 0xb0:  case 0xc0:
            case 0xd0:  case 0xe0:  case 0xf0:
              if(vic3ModeFlag) value=write8_c65rec(address,value);
              else value=write8_vic2(address,value);
              break;
            default:
              value=write8_vic2(address,value);
              break;
          }
          break;
        case 0xd1:
          // red palette registers
          if(vic3ModeFlag) {
            if(vic3Red) vic3Red[address&0xff]=value;
          }
          break;
        case 0xd2:
          // green palette registers
          if(vic3ModeFlag) {
            if(vic3Green) vic3Green[address&0xff]=value;
          }
          break;
        case 0xd3:
          // blue palette registers
          if(vic3ModeFlag) {
            if(vic3Blue) vic3Blue[address&0xff]=value;
          }
          break;
        case 0xd4: case 0xd5:
          value=write8_sid(address,value); break;
        case 0xd6:
          if(vic3ModeFlag) value=write8_c65uart(address,value);
          break;
        case 0xd7:
          if(vic3ModeFlag) value=write8_c65dma(address,value);
          break;
        case 0xd8: case 0xd9: case 0xda: case 0xdb:
          // color ram
          if(handler->getVideo()) value=handler->getVideo()->write8ColorMem(address,value);
          handler->getMap()->write8((address-0xd800)+0x1f800,value);
          break;
        case 0xdc: case 0xdd:
          if(colorRamExtend) {
            if(handler->getVideo()) value=handler->getVideo()->write8ColorMem(address,value);
            handler->getMap()->write8((address-0xd800)+0x1f800,value);
          }
          else value=write8_cias(address,value);
          break;
        case 0xdf:
          c65_df[(address-0xdf00)&0xff]=value;
          break;
        default:
          DEBUG_OUT<<"io write of "<<aDefaultStringHelper.myHex2(value)<<" at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    case N_CBM_IO_VIC:
      switch(page) {
        case 0x90:
          value=write8_vic1(address,value); break;
        case 0x91: case 0x92: case 0x93:
          value=write8_vias(address,value); break;
        case 0x94: case 0x95:  case 0x96:  case 0x97:
          // color ram
          if(handler->getVideo()) value=handler->getVideo()->write8ColorMem(address,value);
          break;
        default:
          DEBUG_OUT<<"io write of "<<aDefaultStringHelper.myHex2(value)<<" at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    case N_CBM_IO_B:
      switch(page) {
        case 0xd8:
          if(handler->getVariation()==0) value=write8_vic2(address,value);
          if(handler->getVariation()==1) value=write8_c128vdc(address,value);
          break;
        case 0xda:
          value=write8_sid(address,value); break;
        case 0xdc:
          value=write8_cias(address,value); break;
        case 0xdd:
          value=write8_c65uart(address,value); break;
        case 0xde: case 0xdf:
          value=write8_tias(address,value); break;
        default:
          DEBUG_OUT<<"io write of "<<aDefaultStringHelper.myHex2(value)<<" at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    default:
      std::cerr<<"bad io type in write8!\n";
      exit(5);
      break;
  }
  mem[address-location]=value;
  return value;
}


BYTE NCbmIO::read8(ULONG address)
{
  BYTE page=(address>>8)&0xff;
  BYTE nib2=address&0x00f0;
  BYTE value=mem[address-location];
  switch(type) {
    case N_CBM_IO_C64:
      switch(page) {
        case 0xd0: case 0xd1: case 0xd2: case 0xd3:
          return read8_vic2(address); ///break;
        case 0xd4: case 0xd5: case 0xd6: case 0xd7:
          return read8_sid(address); //break;
        case 0xd8: case 0xd9: case 0xda: case 0xdb:
          // color ram
          if(handler->getVideo()) return handler->getVideo()->read8ColorMem(address);
          else return page;
          //break;
        case 0xdc: case 0xdd:
          return read8_cias(address); //break;
        default:
          DEBUG_OUT<<"io read of at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    case N_CBM_IO_C128:
      switch(page) {
        case 0xd0: case 0xd1: case 0xd2: case 0xd3:
          return read8_vic2(address); ///break;
        case 0xd4:
          return read8_sid(address); //break;
        case 0xd5:
          return read8_c128mmu(address); //break;
        case 0xd6:
          return read8_c128vdc(address); //break;
        case 0xd8: case 0xd9: case 0xda: case 0xdb:
          // color ram
          if(handler->getVideo()) return handler->getVideo()->read8ColorMem(address);
          else return page;
          //break;
        case 0xdc: case 0xdd:
          return read8_cias(address); //break;
        default:
          DEBUG_OUT<<"io read of at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    case N_CBM_IO_C65:
      switch(page) {
        case 0xd0:
          switch(nib2) {
            case 0x80:  case 0x90:
              if(vic3ModeFlag) return fdc65->read8(address&0xf);
              else return read8_vic2(address);
              break;
            case 0xa0:  case 0xb0:  case 0xc0:
            case 0xd0:  case 0xe0:  case 0xf0:
              if(vic3ModeFlag) return read8_c65rec(address);
              else return read8_vic2(address);
              break;
            default:
              return read8_vic2(address);
              break;
          }
          break;
        case 0xd1:
          // red palette registers
          if(vic3ModeFlag) {
            if(vic3Red) return vic3Red[address&0xff];
          }
          break;
        case 0xd2:
          // green palette registers
          if(vic3ModeFlag) {
            if(vic3Green) return vic3Green[address&0xff];
          }
          break;
        case 0xd3:
          // blue palette registers
          if(vic3ModeFlag) {
            if(vic3Blue) return vic3Blue[address&0xff];
          }
          break;
        case 0xd4: case 0xd5:
          return read8_sid(address); //break;
        case 0xd6:
          if(vic3ModeFlag) return read8_c65uart(address);
          break;
        case 0xd7:
          if(vic3ModeFlag) return read8_c65dma(address);
          break;
        case 0xd8: case 0xd9: case 0xda: case 0xdb:
          // color ram
          return handler->getMap()->read8((address-0xd800)+0x1f800);
          //return handler->getVideo()->read8Color(address);
          //break;
        case 0xdc: case 0xdd:
          if(colorRamExtend) {
            return handler->getMap()->read8((address-0xd800)+0x1f800);
            //return handler->getVideo()->read8Color(address);
          }
          else return read8_cias(address);
          //break;
        case 0xdf:
          return c65_df[(address-0xdf00)&0xff];
          //break;
        default:
          DEBUG_OUT<<"io read of at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    case N_CBM_IO_VIC:
      switch(page) {
        case 0x90:
          return read8_vic1(address); //break;
        case 0x91: case 0x92: case 0x93:
          return read8_vias(address); //break;
        case 0x94: case 0x95:  case 0x96:  case 0x97:
          // color ram
          if(handler->getVideo()) return handler->getVideo()->read8ColorMem(address);
          else return page;
          //break;
        default:
          DEBUG_OUT<<"io read of at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    case N_CBM_IO_B:
      switch(page) {
        case 0xd8:
          if(handler->getVariation()==0) return read8_vic2(address); //break;
          if(handler->getVariation()==1) return read8_c128vdc(address); //break;
        case 0xda:
          return read8_sid(address); //break;
        case 0xdc:
          return read8_cias(address); //break;
        case 0xdd:
          return read8_c65uart(address); //break;
        case 0xde: case 0xdf:
          return read8_tias(address); //break;
        default:
          DEBUG_OUT<<"io read of at "<<aDefaultStringHelper.myHex4(address)<<"\n";
          value=page;
          break;
      }
      break;
    default:
      std::cerr<<"bad io type in read8!\n";
      exit(5);
      break;
  }
  return mem[address-location];
}


// ****************************
// Chips Specific to the Vic-20
// ****************************

// *** The Vic-20's VIA chips ***
BYTE NCbmIO::read8_vias(ULONG address)
{
  // TODO: mask register #
  unsigned int rloc=address-location;
  BYTE r,c,value=mem[rloc];
  switch(address) {
    case 0x9110:      // User port (b)
      break;
    case 0x911f:    case 0x9111:      // Serial/joy/tape (a)
      value=mem[0x9111-location];
      value|=0x3c;
      r=0xff;
      switch(((NCbmHandler *)handler)->joyKey[0]) {
/*
        case A_KEY_JOY_UP:
          r=0xfb;
          break;
        case A_KEY_JOY_DOWN:
          r=0xf7;
          break;
        case A_KEY_JOY_LEFT:
          r=0xef;
          break;
        case A_KEY_JOY_FIRE:
          r=0xdf;
          break;
*/
        default:
          break;
      }
      value&=r;
      ((NCbmHandler *)handler)->joyKey[0]=0;
      //DEBUG_OUT<<"0x911f value is "<<value<<"\n";
      break;
    case 0x9112:      // data direction for b
      break;
    case 0x9113:      // data direction for a
      break;
    case 0x9114:      // timer 1 low
      state1a++;
      break;
    case 0x9115:      // timer 1 high & counter
      break;
    case 0x9116:      // timer 1 low
      state1a++;
      break;
    case 0x9117:      // timer 1 high
      break;
    case 0x9118:      // timer 2 low
      state1b++;
      break;
    case 0x9119:      // timer 2 high
      break;
    case 0x911a:      // shift register
      break;
    case 0x911b:      // aux control reg
      break;
    case 0x911c:      // control reg (ca1, ca2, cb1, cb2)
      break;
    case 0x911d:      // interrupt flags
      break;
    case 0x911e:      // interrupt enable
      break;
    case 0x9120:      // keyboard (b)
      //DEBUG_OUT<<"read  at 0x20\n";
      value=mem[rloc];
      temp=mem[0x9122-location]&0x80; // TODO: is this right?
      if(!temp) {
        value|=0x80;
        r=0xff;
        switch(((NCbmHandler *)handler)->joyKey[0]) {
/*
          case A_KEY_JOY_RIGHT:
            r=0x7f;
            break;
*/
          default:
            break;
        }
        value&=r;
        ((NCbmHandler *)handler)->joyKey[0]=0;
      }
      break;
    case 0x912f:    case 0x9121:      // keyboard (a)
      if(!((NCbmHandler *)handler)->keyScan) value=0;
      else
      if(!handler->getKeypress()) value=0xff;
      else {
        value=0xff;
        if(((NCbmHandler *)handler)->keys[0]&&(!(((NCbmHandler *)handler)->keyScan&0x01))) { c=((NCbmHandler *)handler)->keys[0]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[1]&&(!(((NCbmHandler *)handler)->keyScan&0x02))) { c=((NCbmHandler *)handler)->keys[1]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[2]&&(!(((NCbmHandler *)handler)->keyScan&0x04))) { c=((NCbmHandler *)handler)->keys[2]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[3]&&(!(((NCbmHandler *)handler)->keyScan&0x08))) { c=((NCbmHandler *)handler)->keys[3]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[4]&&(!(((NCbmHandler *)handler)->keyScan&0x10))) { c=((NCbmHandler *)handler)->keys[4]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[5]&&(!(((NCbmHandler *)handler)->keyScan&0x20))) { c=((NCbmHandler *)handler)->keys[5]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[6]&&(!(((NCbmHandler *)handler)->keyScan&0x40))) { c=((NCbmHandler *)handler)->keys[6]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[7]&&(!(((NCbmHandler *)handler)->keyScan&0x80))) { c=((NCbmHandler *)handler)->keys[7]-1; value=~(1<<c); }
        //DEBUG_OUT<<keyScan<<":"<<value<<"\n";
        ((NCbmHandler *)handler)->key_count++;
        if(((NCbmHandler *)handler)->key_count==2) {
          handler->setKeypress(0);  ((NCbmHandler *)handler)->key_count=((NCbmHandler *)handler)->key_max;
          for(temp=0;temp<NCBM_MAX_KEY_ROWS;temp++) ((NCbmHandler *)handler)->keys[temp]=0;
        }
      }
      //DEBUG_OUT<<value<<":";
      //DEBUG_OUT<<mem[0xdc00-location]<<"\n";
      break;
    case 0x9122:      // data direction for b
      break;
    case 0x9123:      // data direction for a
      break;
    case 0x9124:      // timer 1 low latch
      state2a++;
      break;
    case 0x9125:      // timer 1 high latch
      break;
    case 0x9126:      // timer 1 low count
      state2a++;
      break;
    case 0x9127:      // timer 1 high count
      break;
    case 0x9128:      // timer 2 low latch
      state2b++;
      break;
    case 0x9129:      // timer 2 high latch
      break;
    case 0x912a:      // shift register
      break;
    case 0x912b:      // aux control reg
      break;
    case 0x912c:      // control reg (ca1, ca2, cb1, cb2)
      break;
    case 0x912d:      // interrupt flags
      break;
    case 0x912e:      // interrupt enable
      break;
    default:
      break;
  }
  return value;
}


// *** The Vic-20's VIC-I chip ***
BYTE NCbmIO::read8_vic1(ULONG address)
{
  // TODO: mask register #
  unsigned int rloc=address-location;
  BYTE value=mem[rloc];
  switch(address) {
    case 0x9003:      // # rows, etc
      mem[rloc]=mem[rloc]&0x7f;
      if(handler->getVideo()) {
        if(handler->getVideo()->getRaster()&0x100) mem[rloc]=mem[rloc]|0x80;
      }
      break;
    case 0x9004:      // read raster
      if(handler->getVideo()) value=handler->getVideo()->getRaster()&0xfe;
      // This rounds off the raster to even lines
      //DEBUG_OUT<<video_raster<<"\n";
      break;
    default:
      break;
  }
  return value;
}


// *** The Vic-20's VIA chips ***
BYTE NCbmIO::write8_vias(ULONG address, BYTE value)
{
  // TODO: mask register #
  BYTE tvalue;
  switch(address) {
    case 0x9110:      // User port (b)
      break;
    case 0x1f:    case 0x11:      // Serial/joy/tape (a)
      mem[0x911f-location]=value;
      mem[0x9111-location]=value;
      break;
    case 0x9112:      // data direction for b
      break;
    case 0x9113:      // data direction for a
      break;
    case 0x9114:      // timer 1 low latch
      tlatch1a=(tlatch1a&0xff00)+value;
      break;
    case 0x9115:      // timer 1 high latch
      tlatch1a=(tlatch1a&0x00ff)+(value<<8);
      // Load VIA 1 Timer #1
      timer1a=tlatch1a;
      move_timer(0x9114,timer1a);
      move_timer(0x9116,timer1a);
      run1a=true;
      if(state1a) {
        temp=read8(IFR1);
        temp=temp&(0xff-ta_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1a=0;
      }
      break;
    case 0x9116:      // timer 1 low load
      tlatch1a=(tlatch1a&0xff00)+value;
      break;
    case 0x9117:      // timer 1 high load
      tlatch1a=(tlatch1a&0x00ff)+(value<<8);
      run1a=true;
      if(state1a) {
        temp=read8(IFR1);
        temp=temp&(0xff-ta_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1a=0;
      }
      break;
    case 0x9118:      // timer 2 low
      tlatch1b=(tlatch1b&0xff00)+value;
      break;
    case 0x9119:      // timer 2 high
      tlatch1b=(tlatch1b&0x00ff)+(value<<8);
      // Load VIA 1 Timer #2
      timer1b=tlatch1b;
      move_timer(0x9118,timer1b);
      run1b=true;
      if(state1b) {
        temp=read8(IFR1);
        temp=temp&(0xff-tb_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1b=0;
      }
      break;
    case 0x911a:      // shift register
      break;
    case 0x911b:      // aux control reg #1
      if(value&0x40) free1a=true; else free1a=false;
      if(value&0x20) free1b=true; else free1b=false;
      break;
    case 0x911c:      // control reg (ca1, ca2, cb1, cb2)
      break;
    case 0x911d:      // interrupt flags
      //if(win) DEBUG_OUT<<"{write int flags #1: "<<value<<"}\n";
      break;
    case 0x911e:      // interrupt enable
      tvalue=value;
      if(tvalue&0x80) irq1en=irq1en|(tvalue&0x7f);  // set
      else irq1en=irq1en&(~tvalue);  // clear
      //if(win) DEBUG_OUT<<"{ints enabled #1: "<<irq1en<<"}\n";
      value=mem[address-location];
      break;
    case 0x9120:      // keyboard (b)
      //DEBUG_OUT<<"write of "<<value<<" at 0x9120\n";
      ((NCbmHandler *)handler)->keyScan=value;
      break;
    case 0x912f:    case 0x9121:      // keyboard (a)
      //DEBUG_OUT<<"write of "<<value<<" at 0x21\n";
      mem[0x912f-location]=value;
      mem[0x9121-location]=value;
      break;
    case 0x9122:      // data direction for b
      break;
    case 0x9123:      // data direction for b
      break;
    case 0x9124:      // timer 1 low latch
      tlatch2a=(tlatch2a&0xff00)+value;
      break;
    case 0x9125:      // timer 1 high latch
      tlatch2a=(tlatch2a&0x00ff)+(value<<8);
      // Load VIA 2 Timer #1
      timer2a=tlatch2a;
      move_timer(0x9124,timer2a);
      move_timer(0x9126,timer2a);
      run2a=true;
      if(state2a) {
        temp=read8(IFR2);
        temp=temp&(0xff-ta_int);
        if(temp==0x80) temp=0;
        write8(IFR2,temp);
        state2a=0;
      }
      break;
    case 0x9126:      // timer 1 low load
      tlatch2a=(tlatch2a&0xff00)+value;
      break;
    case 0x9127:      // timer 1 high load
      tlatch2a=(tlatch2a&0x00ff)+(value<<8);
      run2a=true;
      if(state2a) {
        temp=read8(IFR2);
        temp=temp&(0xff-ta_int);
        if(temp==0x80) temp=0;
        write8(IFR2,temp);
        state2a=0;
      }
      break;
    case 0x9128:      // timer 2 low latch
      tlatch2b=(tlatch2b&0xff00)+value;
      break;
    case 0x9129:      // timer 2 high latch
      tlatch2b=(tlatch2b&0x00ff)+(value<<8);
      // Load VIA 2 Timer #2
      timer2b=tlatch2b;
      move_timer(0x9128,timer2b);
      run2b=true;
      if(state2b) {
        temp=read8(IFR2);
        temp=temp&(0xff-tb_int);
        if(temp==0x80) temp=0;
        write8(IFR2,temp);
        state2b=0;
      }
      break;
    case 0x912a:      // shift register
      break;
    case 0x912b:      // aux control reg #2
      if(value&0x40) free2a=true; else free2a=false;
      if(value&0x20) free2b=true; else free2b=false;
      break;
    case 0x912c:      // control reg (ca1, ca2, cb1, cb2)
      break;
    case 0x912d:      // interrupt flags
      //if(win) DEBUG_OUT<<"{write int flags #2: "<<value<<"}\n";
      break;
    case 0x912e:      // interrupt enable
      tvalue=value;
      if(tvalue&0x80) irq2en=irq2en|(tvalue&0x7f);  // set
      else irq2en=irq2en&(~tvalue);  // clear
      //if(win) DEBUG_OUT<<"{ints enabled #2: "<<irq2en<<"}\n";
      value=mem[address-location];
      break;
    default:
      break;
  }
  return value;
}


// *** The Vic-20's VIC-I chips ***
BYTE NCbmIO::write8_vic1(ULONG address, BYTE value)
{
  // TODO: mask register #
  unsigned int ncols=0,nrows=0;
  switch(address) {
    case 0x9002:  // num cols, screen mem "36866"
      nscreen_addr=((mem[0x9005-location]&0x70)<<6);
      nscreen_addr+=(value&0x80)<<2;
      ncols=value&0x7f;
      nrows=(mem[0x9003-location]&0x7e)>>1;
      if(ncols>27) ncols=27;
      if(!ncols) ncols=22;  // what should it be?
      if(!nrows) nrows=23;  // what should it be?
      if(handler->getVideo()) {
        //DEBUG_OUT<<"Screen size is 0x"<<ncols<<" by 0x"<<nrows<<"\n";
        handler->getVideo()->setMemSize(ncols,nrows);
        //DEBUG_OUT<<"Screen addr is now:"<<nscreen_addr<<"\n";
        handler->getVideo()->setScreenChar(nscreen_addr,nchar_addr);
        ncolor_addr=0x9400;
        if(value&0x80) ncolor_addr=0x9600;
        handler->getVideo()->setColorMemAddr(ncolor_addr);
      }
      break;
    case 0x9003: // "36867"
      ncols=mem[0x9002-location]&0x7f;
      nrows=(value&0x7e)>>1;
      if(ncols>27) ncols=27;
      if(!ncols) ncols=1;
      if(!nrows) nrows=1;
      //DEBUG_OUT<<"Screen size is 0x"<<ncols<<" by 0x"<<nrows<<"\n";
      if(handler->getVideo()) handler->getVideo()->setMemSize(ncols,nrows);
      ((NCbmVideo *)handler->getVideo())->setVicDoubleHeightMode(value&1);
      break;
    case 0x9005:  // screen and character memory "36869"
      nscreen_addr=((value&0x70)<<6);
      nscreen_addr+=(mem[0x9002-location]&0x80)<<2;
      //DEBUG_OUT<<"Screen addr is now:"<<nscreen_addr<<"\n";
      temp=value&0xf;
      switch(temp) {
        case 0: nchar_addr=0x8000; break;
        case 1: nchar_addr=0x8400; break;
        case 2: nchar_addr=0x8800; break;
        case 3: nchar_addr=0x8c00; break;
        case 4: nchar_addr=0x9000; break;
        case 5: nchar_addr=0x9400; break;
        case 6: nchar_addr=0x9800; break;
        case 7: nchar_addr=0x9400; break;
        case 8: nchar_addr=0x0000; break;
        case 9: nchar_addr=0x0400; break;
        case 0xa: nchar_addr=0x0800; break;
        case 0xb: nchar_addr=0x0c00; break;
        case 0xc: nchar_addr=0x1000; break;
        case 0xd: nchar_addr=0x1400; break;
        case 0xe: nchar_addr=0x1800; break;
        case 0xf: nchar_addr=0x1c00; break;
        default: break;
      }
      //DEBUG_OUT<<"Char addr is now:"<<nchar_addr<<"\n";
      if(handler->getVideo()) handler->getVideo()->setScreenChar(nscreen_addr,nchar_addr);
      break;
    case 0x900f:  // border and background colors
      if(handler->getVideo()) {
        unsigned int newBackground=handler->getVideo()->getColors()->getColor((value&0xf0)>>4);
        if(newBackground!=handler->getVideo()->getBackground()) handler->getVideo()->setBackground(newBackground);
        handler->getVideo()->setBorder(16,handler->getVideo()->getColors()->getColor(value&0xf));
      }
      break;
    default:
      break;
  }
  return value;
}


// ******************************************
// Chips Specific to the C-64, C-128 and C-65
// ******************************************

// *** The C-64, C-128 and C-65's CIA chips ***
BYTE NCbmIO::read8_cias(ULONG address)
{
  unsigned int reg=address&0x1ff;
  // NOTE: this means 000-0ff are dc00 and 100-1ff are dd00 or db00
  BYTE c,r;
  BYTE value=mem[address-location];
  switch(reg) {
    case 0x000:      // Keyboard/joystick
      //temp=mem[0x002];
      //r=value;
      //if(!(temp&0x01)) r|=0x01;
      //if(!(temp&0x02)) r|=0x02;
      //if(!(temp&0x04)) r|=0x04;
      //if(!(temp&0x08)) r|=0x08;
      //if(!(temp&0x10)) r|=0x10;
      //switch(((NCbmHandler *)io)->joyKey[0]) {
        /*
        case A_KEY_JOY_UP:
          if(!(temp&0x01)) r&=0xfe;
          break;
        case A_KEY_JOY_DOWN:
          if(!(temp&0x02)) r&=0xfd;
          break;
        case A_KEY_JOY_LEFT:
          if(!(temp&0x04)) r&=0xfb;
          break;
        case A_KEY_JOY_RIGHT:
          if(!(temp&0x08)) r&=0xf7;
          break;
        case A_KEY_JOY_FIRE:
          if(!(temp&0x10)) r&=0xef;
          break;
        */
        //default:
          //break;
      //}
      //value=r;
      //((NCbmHandler *)io)->joyKey[0]=0;
      break;
    case 0x001:      // Keyboard/joystick
      if(!((NCbmHandler *)handler)->keyScan) value=0;
      else
      if(!handler->getKeypress()) value=0xff;
      else {
        value=0xff;
	//DEBUG_OUT<<(((NCbmHandler *)handler)->keyScan)<<"\n";
        if(((NCbmHandler *)handler)->keys[0]&&(!(((NCbmHandler *)handler)->keyScan&0x01))) {
	  c=((NCbmHandler *)handler)->keys[0]-1; value=~(1<<c);
	  //DEBUG_OUT<<"keys[0]!\n";
	}
        if(((NCbmHandler *)handler)->keys[1]&&(!(((NCbmHandler *)handler)->keyScan&0x02))) {
	  c=((NCbmHandler *)handler)->keys[1]-1; value=~(1<<c);
	  //DEBUG_OUT<<"keys[1]!\n";
	}
        if(((NCbmHandler *)handler)->keys[2]&&(!(((NCbmHandler *)handler)->keyScan&0x04))) {
	  c=((NCbmHandler *)handler)->keys[2]-1; value=~(1<<c);
	  //DEBUG_OUT<<"keys[2]!\n";
	}
        if(((NCbmHandler *)handler)->keys[3]&&(!(((NCbmHandler *)handler)->keyScan&0x08))) {
	  c=((NCbmHandler *)handler)->keys[3]-1; value=~(1<<c);
	  //DEBUG_OUT<<"keys[3]!\n";
	}
        if(((NCbmHandler *)handler)->keys[4]&&(!(((NCbmHandler *)handler)->keyScan&0x10))) {
	  c=((NCbmHandler *)handler)->keys[4]-1; value=~(1<<c);
	  //DEBUG_OUT<<"keys[4]!\n";
	}
        if(((NCbmHandler *)handler)->keys[5]&&(!(((NCbmHandler *)handler)->keyScan&0x20))) {
	  c=((NCbmHandler *)handler)->keys[5]-1; value=~(1<<c);
	  //DEBUG_OUT<<"keys[5]!\n";
	}
        if(((NCbmHandler *)handler)->keys[6]&&(!(((NCbmHandler *)handler)->keyScan&0x40))) {
	  c=((NCbmHandler *)handler)->keys[6]-1; value=~(1<<c);
	  //DEBUG_OUT<<"keys[6]!\n";
	}
        if(((NCbmHandler *)handler)->keys[7]&&(!(((NCbmHandler *)handler)->keyScan&0x80))) {
	  c=((NCbmHandler *)handler)->keys[7]-1; value=~(1<<c);
	  //DEBUG_OUT<<"keys[7]!\n";
	}
        //DEBUG_OUT<<(((NCbmHandler *)handler)->keyScan)<<":"<<value<<"\n";
        ((NCbmHandler *)handler)->key_count++;
        if(((NCbmHandler *)handler)->key_count==2) {
          handler->setKeypress(0);  ((NCbmHandler *)handler)->key_count=((NCbmHandler *)handler)->key_max;
          for(temp=0;temp<NCBM_MAX_KEY_ROWS;temp++) ((NCbmHandler *)handler)->keys[temp]=0;
        }
      }
      //temp=mem[0x003-location];
      //r=value;
      /*
      if(!(temp&0x01)) r|=0x01;
      if(!(temp&0x02)) r|=0x02;
      if(!(temp&0x04)) r|=0x04;
      if(!(temp&0x08)) r|=0x08;
      if(!(temp&0x10)) r|=0x10;
      */
      //switch(((NCbmHandler *)io)->joyKey[1]) {
        /*
        case A_KEY_JOY_UP:
          if(!(temp&0x01)) r&=0xfe;
          break;
        case A_KEY_JOY_DOWN:
          if(!(temp&0x02)) r&=0xfd;
          break;
        case A_KEY_JOY_LEFT:
          if(!(temp&0x04)) r&=0xfb;
          break;
        case A_KEY_JOY_RIGHT:
          if(!(temp&0x08)) r&=0xf7;
          break;
        case A_KEY_JOY_FIRE:
          if(!(temp&0x10)) r&=0xef;
          break;
        */
        //default:
          //break;
      //}
      //value=r;
      //((NCbmHandler *)io)->joyKey[1]=0;
      //DEBUG_OUT<<value<<":"<<mem[0x000]<<"\n";
      break;
    case 0x100:      // Serial port
      break;
    case 0x101:      // User port
      // Fake DTR signal
      value=0x04;
      break;
    case 0x002:      // DDR
      break;
    case 0x003:      // DDR
      break;
    case 0x102:      // DDR
      break;
    case 0x103:      // DDR
      break;
    case 0x004:      // Cia #1 Timer A
      break;
    case 0x005:      // Cia #1 Timer A
      break;
    case 0x006:      // Cia #1 Timer B
      break;
    case 0x007:      // Cia #1 Timer B
      break;
    case 0x104:      // Cia #2 Timer A
      break;
    case 0x105:      // Cia #2 Timer A
      break;
    case 0x106:      // Cia #2 Timer B
      break;
    case 0x107:      // Cia #2 Timer B
      break;
    case 0x008:      // CIA #1 TOD clock
      break;
    case 0x009:      // CIA #1 TOD clock
      break;
    case 0x00a:      // CIA #1 TOD clock
      break;
    case 0x00b:      // CIA #1 TOD clock
      break;
    case 0x108:      // CIA #2 TOD clock
      break;
    case 0x109:      // CIA #2 TOD clock
      break;
    case 0x10a:      // CIA #2 TOD clock
      break;
    case 0x10b:      // CIA #2 TOD clock
      break;
    case 0x00c:      // Sync SP port buffer
      break;
    case 0x10c:      // Sync SP port buffer
      break;
    case 0x00d: case 0x10d:      // IRQ
      value=mem[address-location];
      mem[address-location]=0;
      break;
    case 0x00e:      // CIA #1 control A
      break;
    case 0x00f:      // CIA #1 control B
      break;
    case 0x10e:      // CIA #2 control A
      break;
    case 0x10f:      // CIA #2 control B
      break;
    default:
      break;
  }
  return value;
}


// *** The C64's Vic2, C128's Vic2e and C65's Vic3 chips ***
BYTE NCbmIO::read8_vic2(ULONG address)
{
  // TODO: mask register #
  unsigned int rloc=address-location,adj;
  BYTE value=mem[rloc];
  switch(address) {
    case 0xd000: case 0xd002: case 0xd004: case 0xd006:
    case 0xd008: case 0xd00a: case 0xd00c: case 0xd00e:
      // Sprite X's
      adj=(address-0xd000)/2;
      // This may not be right...
      //if(handler->getVideo()) value=handler->getVideo()->getSprite(adj)->x+8;
      break;
    case 0xd001: case 0xd003: case 0xd005: case 0xd007:
    case 0xd009: case 0xd00b: case 0xd00d: case 0xd00f:
      // Sprite Y's
      adj=(address-0xd001)/2;
      // This may not be right...
     // if(handler->getVideo()) value=handler->getVideo()->getSprite(adj)->y+50;
      break;
    case 0xd010:
      // Sprite X MSB's
      break;
    case 0xd011:      // control #1
      mem[rloc]=mem[rloc]&0x7f;
      if(handler->getVideo()) {
        if(handler->getVideo()->getRaster()&0x100) mem[rloc]=mem[rloc]|0x80;
      }
      break;
    case 0xd012:      // read raster
      if(handler->getVideo()) value=handler->getVideo()->getRaster()&0xfe;
      // This rounds off the raster to even lines
      //DEBUG_OUT<<video_raster<<"\n";
      break;
    case 0xd013:  case 0xd014:      // light pen x and y s
      break;
    case 0xd015:      // sprite enable
      break;
    case 0xd016:      // control #2
      break;
    case 0xd017:      // sprite y expand
      break;
    case 0xd018:      // memory pointer
      break;
    case 0xd019:      // Int occured
      value=0;  // We didn't cause the IRQ
      if(handler->getVideo()) { if(RAST_IRQ&&handler->getVideo()->getIntCaused()) value=0x01; }
      if(value) value|=0x80;
      break;
    case 0xd01a:      // Int enable
      break;
    case 0xd01b:      // sprite priority
      break;
    case 0xd01c:      // sprite multicolor select
      break;
     case 0xd01d:      // sprite x expand
      break;
    case 0xd01e:      // sprite-sprite collision
      break;
    case 0xd01f:      // sprite-background collision
      break;
    case 0xd020: case 0xd021: case 0xd022: case 0xd023: case 0xd024:
      // border and background colors
      break;
    case 0xd025: case 0xd026:
      // sprite multi colors
      break;
    case 0xd027: case 0xd028: case 0xd029: case 0xd02a: case 0xd02b:
    case 0xd02c: case 0xd02d: case 0xd02e:
      // sprite colors
      break;
    case 0xd02f:
      switch(type) {
        case N_CBM_IO_C65:
          DEBUG_OUT<<"{Read of Vic3 "<<address<<": "<<(short)value<<"}\n";
          break;
        case N_CBM_IO_C128:
          DEBUG_OUT<<"{Read of Vic2e "<<address<<": "<<(short)value<<"}\n";
          break;
        default:
          break;
      }
      break;
    case 0xd030:
      switch(type) {
        case N_CBM_IO_C65:
          //DEBUG_OUT<<"{Read of Vic3 "<<address<<": "<<(short)value<<"}\n";
          break;
        case N_CBM_IO_C128:
          DEBUG_OUT<<"{Read of Vic2e "<<address<<": "<<(short)value<<"}\n";
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  if(((address&0x00f0)==8)&&(type==N_CBM_IO_C65)) {
    //drive controller
    DEBUG_OUT<<"{Read of FDC "<<address<<": "<<(short)value<<"}\n";
  }
  return value;
}


// *** The C-64, C-128 and C-65's SID chip(s) ***
BYTE NCbmIO::read8_sid(ULONG address)
{
  BYTE reg=address&0xff;
  BYTE value=mem[address-location];
  switch(reg) {
    default:
      break;
  }
  return value;
}


// *** The C-64, C-128 and C-65's CIA chips ***
BYTE NCbmIO::write8_cias(ULONG address, BYTE value)
{
  unsigned int reg=address&0x1ff;
  // NOTE: this means 000-0ff are dc00 and 100-1ff are dd00 or db00
  unsigned int adj=0;
  BYTE tvalue;
  switch(reg) {
    case 0x000:      // Keyboard
      ((NCbmHandler *)handler)->keyScan=value;
      break;
    case 0x001:      // Keyboard
      break;
    case 0x100:      // Serial port
      tvalue=read8(0xd018);
      nscreen_addr=((tvalue&0xf0)>>4)*1024;
      nchar_addr=((tvalue&0x0e)>>1)*2048;
      tvalue=value&3;
      switch(tvalue) {
        case 0: adj=0xc000; break;
        case 1: adj=0x8000; break;
        case 2: adj=0x4000; break;
        case 3: adj=0x0000; break;
        default: break;
      }
      nscreen_addr+=adj;
      nchar_addr+=adj;
      //DEBUG_OUT<<"Screen addr is now:"<<nscreen_addr<<"\n";
      //DEBUG_OUT<<"Char addr is now:"<<char_addr<<"\n";
      ((NCbmVideo *)handler->getVideo())->setScreenCharSprites(nscreen_addr,nchar_addr);
      break;
    case 0x101:      // User port
      break;
    case 0x002:      // DDR
      break;
    case 0x003:      // DDR
      break;
    case 0x102:      // DDR
      break;
    case 0x103:      // DDR
      break;
    case 0x004:      // Cia #1 Timer A
      tlatch1a=(tlatch1a&0xff00)+value;
      break;
    case 0x005:      // Cia #1 Timer A
      tlatch1a=(tlatch1a&0x00ff)+(value<<8);
      // Load CIA 1 Timer A
      timer1a=tlatch1a;
      move_timer(address-1,timer1a);
      break;
    case 0x006:      // Cia #1 Timer B
      tlatch1b=(tlatch1b&0xff00)+value;
      break;
    case 0x007:      // Cia #1 Timer B
      tlatch1b=(tlatch1b&0x00ff)+(value<<8);
      // Load CIA 1 Timer B
      timer1b=tlatch1a;
      move_timer(address-1,timer1b);
      break;
    case 0x104:      // Cia #2 Timer A
      tlatch2a=(tlatch2a&0xff00)+value;
      break;
    case 0x105:      // Cia #2 Timer A
      tlatch2a=(tlatch2a&0x00ff)+(value<<8);
      // Load CIA 2 Timer A
      timer2a=tlatch2a;
      move_timer(address-1,timer2a);
      break;
    case 0x106:      // Cia #2 Timer B
      tlatch2b=(tlatch2b&0xff00)+value;
      break;
    case 0x107:      // Cia #2 Timer B
      tlatch2b=(tlatch2b&0x00ff)+(value<<8);
      // Load CIA 2 Timer B
      timer2b=tlatch2b;
      move_timer(address-1,timer2b);
      break;
    case 0x008:      // CIA #1 TOD clock
      break;
    case 0x009:      // CIA #1 TOD clock
      break;
    case 0x00a:      // CIA #1 TOD clock
      break;
    case 0x00b:      // CIA #1 TOD clock
      break;
    case 0x108:      // CIA #2 TOD clock
      break;
    case 0x109:      // CIA #2 TOD clock
      break;
    case 0x10a:      // CIA #2 TOD clock
      break;
    case 0x10b:      // CIA #2 TOD clock
      break;
    case 0x00c:      // Sync SP port buffer
      break;
    case 0x10c:       // Sync SP port buffer
      break;
    case 0x00d:      // CIA #1 IRQ
      tvalue=value;
      if(tvalue&0x80) irq1en=irq1en|(tvalue&0x7f);  // set
      else irq1en=irq1en&(~tvalue);  // clear
      //if(win) DEBUG_OUT<<"{ints enabled #1: "<<irq1en<<"}\n";
      value=mem[address-location];
      break;
    case 0x10d:      // CIA #2 IRQ
      tvalue=value;
      if(tvalue&0x80) irq2en=irq2en|(tvalue&0x7f);  // set
      else irq2en=irq2en&(~tvalue);  // clear
      //if(win) DEBUG_OUT<<"{ints enabled #2: "<<irq2en<<"}\n";
      value=mem[address-location];
      break;
    case 0x00e:      // CIA #1 control A
      if(value&0x01) run1a=true; else run1a=false;
      if(value&0x08) free1a=false; else free1a=true;
      break;
    case 0x00f:      // CIA #1 control B
      if(value&0x01) run1b=true; else run1b=false;
      if(value&0x08) free1b=false; else free1b=true;
      break;
    case 0x10e:      // CIA #2 control A
      if(value&0x01) run2a=true; else run2a=false;
      if(value&0x08) free2a=false; else free2a=true;
      break;
    case 0x10f:      // CIA #2 control B
      if(value&0x01) run2b=true; else run2b=false;
      if(value&0x08) free2b=false; else free2b=true;
      break;
    default:
      break;
  }
  return value;
}


// *** The C64's Vic2, C128's Vic2e and C65's Vic3 chips ***
BYTE NCbmIO::write8_vic2(ULONG address, BYTE value)
{
  // TODO: mask register #
  unsigned int adj=0;
  unsigned int ncols=0,nrows=0;
  BYTE tvalue;
  unsigned int t;
  switch(address) {
    case 0xd000: case 0xd002: case 0xd004: case 0xd006:
    case 0xd008: case 0xd00a: case 0xd00c: case 0xd00e:
      // Sprite X's
      adj=(address-0xd000)/2;
      // This may not be right...
      //if(handler->getVideo()) handler->getVideo()->getSprite(adj)->x=value-8;
      break;
    case 0xd001: case 0xd003: case 0xd005: case 0xd007:
    case 0xd009: case 0xd00b: case 0xd00d: case 0xd00f:
      // Sprite Y's
      adj=(address-0xd001)/2;
      // This may not be right...
      //if(handler->getVideo()) handler->getVideo()->getSprite(adj)->y=value-50;
      break;
    case 0xd010:
      // TODO: Sprite X MSB's
      break;
    case 0xd011:      // control #1
      raster_check&=0xff;
      if(value&0x80) raster_check|=0x100;
      if(handler->getVideo()) {
        if(RAST_IRQ) handler->getVideo()->setRasterInt(raster_check,E6502_INT_IRQ);
      }
      ncols=handler->getVideo()->getTilesAcross();
      nrows=24;
      if(value&8) nrows++;
      //DEBUG_OUT<<"Screen size is 0x"<<ncols<<" by 0x"<<nrows<<"\n";
      if(handler->getVideo()) handler->getVideo()->setVideoSize(ncols,nrows);
      break;
    case 0xd012:      // raster compare
      raster_check&=0x100;
      raster_check|=value;
      if(handler->getVideo()) {
        if(RAST_IRQ) handler->getVideo()->setRasterInt(raster_check,E6502_INT_IRQ);
      }
      value=mem[address-location];
      break;
    case 0xd013:  case 0xd014:      // light pen x and y s
      break;
    case 0xd015:      // sprite enable
      if(handler->getVideo()) {
        for(t=0;t<8;t++) handler->getVideo()->setSpriteActive(t,(value>>t)&1);
      }
      break;
    case 0xd016:      // control #2
      break;
    case 0xd017:      // sprite y expand
      break;
    case 0xd018:      // memory pointer
      nscreen_addr=((value&0xf0)>>4)*1024;
      nchar_addr=((value&0x0e)>>1)*2048;
      tvalue=read8(0xdd00)&3;
      switch(tvalue) {
        case 0: adj=0xc000; break;
        case 1: adj=0x8000; break;
        case 2: adj=0x4000; break;
        case 3: adj=0x0000; break;
        default: break;
      }
      nscreen_addr+=adj;
      nchar_addr+=adj;
      //DEBUG_OUT<<"Screen addr is now:"<<nscreen_addr<<"\n";
      //DEBUG_OUT<<"Char addr is now:"<<char_addr<<"\n";
      ((NCbmVideo *)handler->getVideo())->setScreenCharSprites(nscreen_addr,nchar_addr);
      break;
    case 0xd019:      // Int latch
      tvalue=value;
      value=mem[address-location];
      value=value&(~tvalue);
      break;
    case 0xd01a:      // Int enable
      if(value&0x01) RAST_IRQ=true; else RAST_IRQ=false;
      if(handler->getVideo()) {
        if(RAST_IRQ) handler->getVideo()->setRasterInt(raster_check,E6502_INT_IRQ);
        else handler->getVideo()->unsetRasterInt();
      }
      break;
    case 0xd01b:      // sprite priority
      break;
    case 0xd01c:      // sprite multicolor select
      break;
     case 0xd01d:     // sprite x expand
      break;
    case 0xd01e:      // sprite-sprite collision
      break;
    case 0xd01f:      // sprite-background collision
      break;
    case 0xd020:
      // border color
      //std::cerr<<"border color "<<(value&0xf)<<" : "<<aDefaultStringHelper.myHex8(handler->getVideo()->getColors()->getColor(value&0xf))<<"\n";
      if(handler->getVideo()) handler->getVideo()->setBorder(16,handler->getVideo()->getColors()->getColor(value&0xf));
      break;
    case 0xd021:
      // background color
      //std::cerr<<"back color "<<(value&0xf)<<" : "<<aDefaultStringHelper.myHex8(handler->getVideo()->getColors()->getColor(value&0xf))<<"\n";
      if(handler->getVideo()) handler->getVideo()->setBackground(handler->getVideo()->getColors()->getColor(value&0xf));
      break;
    case 0xd022: case 0xd023: case 0xd024:
      // extended background colors
      break;
    case 0xd025: case 0xd026:
      // sprite multi colors
      break;
    case 0xd027: case 0xd028: case 0xd029: case 0xd02a: case 0xd02b:
    case 0xd02c: case 0xd02d: case 0xd02e:
      // sprite colors
      break;
    case 0xd02f:
      switch(type) {
        case N_CBM_IO_C65:
          // To set vic-3 mode, write a5, 96...
          //DEBUG_OUT<<"{Write to C65 Vic3 mode d02f: "<<value<<"}\n";
          if((vic3ModeKey==0x00)&&(value==0xa5)) vic3ModeKey=0xa5;
          if((vic3ModeKey==0xa5)&&(value==0x96)) vic3ModeKey=0x96;
          if((value!=0xa5)&&(value!=0x96)) {
            vic3ModeKey=0;
            DEBUG_OUT<<"Vic chip put into Vic-2 mode!\n";
            vic3ModeFlag=false;
            ((NCbmHandler *)handler)->setC64Mode(true);
          }
          value=vic3ModeKey;
          if(vic3ModeKey==0x96) {
            DEBUG_OUT<<"Vic chip put into Vic-3 mode!\n";
            vic3ModeFlag=true;
            ((NCbmHandler *)handler)->setC64Mode(false);
          }
          break;
        case N_CBM_IO_C128:
          DEBUG_OUT<<"{Write to Vic2e d02f: "<<(short)value<<"}\n";
          break;
        default:
          break;
      }
      break;
    case 0xd030:
      switch(type) {
        case N_CBM_IO_C65:
          ((NCbmHandler *)handler)->handleVic3MapC65(value);
          break;
        case N_CBM_IO_C128:
          //DEBUG_OUT<<"{Write to Vic2e "<<address<<": "<<(short)value<<"}\n";
          break;
        default:
          break;
      }
      break;
    case 0xd031:
      if(type==N_CBM_IO_C65) {
        //DEBUG_OUT<<"{Write to video mode "<<address<<": "<<value<<"  ";
        DEBUG_OUT<<"{";
        if(value&1) DEBUG_OUT<<"LCE "; else DEBUG_OUT<<"NOL ";
        if(value&2) DEBUG_OUT<<"MON "; else DEBUG_OUT<<"CLR ";
        if(value&4) DEBUG_OUT<<"1280 "; else DEBUG_OUT<<"NRM ";
        if(value&8) DEBUG_OUT<<"400 "; else DEBUG_OUT<<"200 ";
        if(value&0x10) DEBUG_OUT<<"BPL "; else DEBUG_OUT<<"Vic2 ";
        if(value&0x20) DEBUG_OUT<<"ATR "; else DEBUG_OUT<<"NOA ";
        if(value&0x40) DEBUG_OUT<<"FAST "; else DEBUG_OUT<<"SLOW ";
        if(value&0x80) {
          DEBUG_OUT<<"640";
          if(handler->getVideo()->getTilesAcross()!=80) {
            handler->getVideo()->setMemSize(80,25,true);
          }
        }
        else {
          DEBUG_OUT<<"320";
          if(handler->getVideo()->getTilesAcross()!=40) {
            handler->getVideo()->setMemSize(40,25,true);
          }
        }
        DEBUG_OUT<<"}\n";
      }
      break;
    case 0xd032:
      if(type==N_CBM_IO_C65) {
        //bitplane enable
        DEBUG_OUT<<"{Write to plane enable "<<address<<": "<<(short)value<<"}\n";
      }
      break;
    case 0xd033:  case 0xd034:  case 0xd035:  case 0xd036:
    case 0xd037:  case 0xd038:  case 0xd039:  case 0xd03a:
      if(type==N_CBM_IO_C65) {
        //bitplane addrs
        DEBUG_OUT<<"{Write to plane addr "<<address<<": "<<(short)value<<"}\n";
      }
      break;
    case 0xd03b:
      if(type==N_CBM_IO_C65) {
        //plane inverse
        DEBUG_OUT<<"{Write to plane inv "<<address<<": "<<(short)value<<"}\n";
      }
      break;
    case 0xd03c:  case 0xd03d:
      if(type==N_CBM_IO_C65) {
        //DAT x/y
        DEBUG_OUT<<"{Write to DAT x/y "<<address<<": "<<(short)value<<"}\n";
      }
      break;
    case 0xd03e:  case 0xd03f:
      if(type==N_CBM_IO_C65) {
        //display position
        DEBUG_OUT<<"{Write to display pos "<<address<<": "<<(short)value<<"}\n";
      }
      break;
    case 0xd040:  case 0xd041:  case 0xd042:  case 0xd043:
    case 0xd044:  case 0xd045:  case 0xd046:  case 0xd047:
      if(type==N_CBM_IO_C65) {
        //DAT ports
        DEBUG_OUT<<"{Write to DAT port "<<address<<": "<<(short)value<<"}\n";
      }
      break;
    default:
      break;
  }
  if(((address&0x00f0)==8)&&(type==N_CBM_IO_C65)) {
    //drive controller
    DEBUG_OUT<<"{Write to FDC "<<address<<": "<<(short)value<<"}\n";
  }
  return value;
}


// *** The C-64, C-128 and C-65's SID chip(s) ***
BYTE NCbmIO::write8_sid(ULONG address, BYTE value)
{
  BYTE reg=address&0xff;
  switch(reg) {
    default:
      break;
  }
  return value;
}


// ***************************
// Chips Specific to the C-128
// ***************************

BYTE NCbmIO::read8_c128mmu(ULONG address)
{
  // TODO: mask register #
  BYTE value=mem[address-location];
  switch(address) {
    case 0xd500:
      value=((NCbmHandler *)handler)->getCRC128(0);
      break;
    case 0xd501:
      value=((NCbmHandler *)handler)->getCRC128(1);
      break;
    case 0xd502:
      value=((NCbmHandler *)handler)->getCRC128(2);
      break;
    case 0xd503:
      value=((NCbmHandler *)handler)->getCRC128(3);
      break;
    case 0xd504:
      value=((NCbmHandler *)handler)->getCRC128(4);
      break;
    case 0xd505:
      // 8502,c128,40 col
      value=0x80|0x20|0x10|0x01;
      //value=0x40;
      break;
    case 0xd506:  case 0xd507:  case 0xd508:  case 0xd509:
    case 0xd50a:
      // Just let it read it
      break;
    case 0xd50b:
      // 128k, version 0
      value=0x20;
      break;
    default:
      DEBUG_OUT<<"read at "<<address<<" of "<<(short)value<<"\n";
      break;
  }
  return value;
}


BYTE NCbmIO::read8_c128vdc(ULONG address)
{
  // TODO: mask register #
  BYTE value=mem[address-location];
  BYTE reg=address&0xf;
  switch(reg) {
    case 0x0:  // VDC Status
      // return status = ready
      value=0x9f;
      break;
    case 0x1:  // VDC data
      value=vdc->read8(vdcReg);
      switch(vdcReg) {
        case 18: value=(vdcAddr&0xff)>>8; break;
        case 19: value=vdcAddr&0xff; break;
        case 31:
          value=vdcMem->read8(vdcAddr);
          vdcAddr++;
          break;
      }
      break;
    default:
      DEBUG_OUT<<"read at "<<address<<" of "<<(short)value<<"\n";
      break;
  }
  return value;
}


BYTE NCbmIO::write8_c128mmu(ULONG address, BYTE value)
{
  // TODO: mask register #
  ULONG addr;
  switch(address) {
    case 0xd500:
      ((NCbmHandler *)handler)->handleMMUC128(value);
      break;
    case 0xd501:
      ((NCbmHandler *)handler)->setCRC128(1,value);
      break;
    case 0xd502:
      ((NCbmHandler *)handler)->setCRC128(2,value);
      break;
    case 0xd503:
      ((NCbmHandler *)handler)->setCRC128(3,value);
      break;
    case 0xd504:
      ((NCbmHandler *)handler)->setCRC128(4,value);
      break;
    case 0xd505:
      if(value&0x40) handler->getCPU()->debugger("Switch to C64 mode!\n");
      if(!(value&0x01)) handler->getCPU()->debugger("Z80 activated!\n");
      break;
    case 0xd506:
      ((NCbmHandler *)handler)->setCommonC128(value);
      break;
    case 0xd508:  case 0xd507:
      mem[address-location]=value;
      addr=read8(0xd508)*256+read8(0xd507);
      DEBUG_OUT<<"Page 0 relocated to "<<addr<<"\n";;
      break;
    case 0xd50a:  case 0xd509:
      mem[address-location]=value;
      addr=read8(0xd50a)*256+read8(0xd509);
      DEBUG_OUT<<"Page 1 relocated to "<<addr<<"\n";;
      break;
    default:
      DEBUG_OUT<<"write at "<<address<<" of "<<(short)value<<"\n";
      break;
  }
  return value;
}


BYTE NCbmIO::write8_c128vdc(ULONG address, BYTE value)
{
  BYTE reg=address&0xf;
  switch(reg) {
    case 0x0:  // VDC register select
      vdcReg=value&0x3f;
      break;
    case 0x1:  // VDC data
      vdc->write8(vdcReg,value);
      value=vdc->read8(vdcReg);
      switch(vdcReg) {
        case 18:
          vdcAddr&=0xff00;
          vdcAddr|=value<<8;
          break;
        case 19:
          vdcAddr&=0x00ff;
          vdcAddr|=value;
          break;
        case 24:
          break;
        case 30:
          //DEBUG_OUT<<"VDC count "<<value<<"\n";
          break;
        case 31:
          vdcMem->write8(vdcAddr,value);
          vdcAddr++;
          break;
      }
      break;
      break;
    default:
      DEBUG_OUT<<"write at "<<address<<" of "<<(short)value<<"\n";
      break;
  }
  return value;
}


// **************************
// Chips Specific to the C-65
// **************************

// *** The C-65's UART ***
BYTE NCbmIO::read8_c65uart(ULONG address)
{
  BYTE reg=address&0xff;
  BYTE value=mem[address-location];
  switch(reg) {
    case 0x0: // DATA
      value=215;
      break;
    case 0x1: // STATUS
      value=215;
      break;
    case 0x2: // CONTROL
      value=215;
      break;
    case 0x3: // BAUD LO
      value=0;
      break;
    case 0x4: // BAUD HI
      value=215;
      break;
    case 0x5: // INT MASK
      value=215;
      break;
    case 0x6: // INT FLAG
      value=215;
      break;
    case 0x7: // ???
      value=215;
      break;
    case 0x8: // ???
      value=215;
      break;
    case 0x9: // ???
      value=215;
      break;
    default:
      DEBUG_OUT<<"UART read at "<<aDefaultStringHelper.myHex((unsigned short)address)<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


// *** The C-65's DMAGIC chip ***
BYTE NCbmIO::read8_c65dma(ULONG address)
{
  BYTE ad=address&0xff;
  BYTE value=mem[address-location];
  switch(ad) {
    case 0x03:
      // DMA status
      // bit 7=busy, bit 0=chained
      // NOTE: we're never busy since all DMA takes place instantly...
      value=0;  // we aren't busy, we aren't chained
      break;
    default:
      DEBUG_OUT<<"C65 DMA read at "<<aDefaultStringHelper.myHex((unsigned short)address)<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


// *** The C-65's REC (not on the motherboard) ***
BYTE NCbmIO::read8_c65rec(ULONG address)
{
  BYTE reg=address&0xff;
  BYTE value=mem[address-location];
  switch(reg) {
    default:
      DEBUG_OUT<<"REC read at "<<aDefaultStringHelper.myHex((unsigned short)address)<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


// *** The C-65's UART ***
BYTE NCbmIO::write8_c65uart(ULONG address, BYTE value)
{
  BYTE reg=address&0xff;
   switch(reg) {
    case 0x0: // DATA
      mem[address-location]=value;
      break;
    case 0x1: // STATUS
      mem[address-location]=value;
      break;
    case 0x2: // CONTROL
      mem[address-location]=value;
      break;
    case 0x3: // BAUD LO
      mem[address-location]=value;
      break;
    case 0x4: // BAUD HI
      mem[address-location]=value;
      break;
    case 0x5: // INT MASK
      mem[address-location]=value&0xf0;
      break;
    case 0x6: // INT FLAG
      mem[address-location]=value&0xf0;
      break;
    default:
      DEBUG_OUT<<"UART write at "<<aDefaultStringHelper.myHex((unsigned short)address)<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


// *** The C-65's DMAGIC chip ***
BYTE NCbmIO::write8_c65dma(ULONG address, BYTE value)
{
  unsigned int dmaListAddr=0;
  BYTE ad=address&0xff;
  switch(ad) {
    case 0x00:
      dma0=value;
      dmaListAddr=dma0|((dma1<<8)&0xff00)|((dma2<<16)&0xff0000);
      DEBUG_OUT<<"C65 DMA triggered: "<<aDefaultStringHelper.myHex((unsigned short)dmaListAddr)<<"...\n";
      c65DoDMA(dmaListAddr);
      break;
    case 0x01:
      dma1=value;
      break;
    case 0x02:
      dma1=value;
      break;
    default:
      DEBUG_OUT<<"C65 DMA write at "<<aDefaultStringHelper.myHex((unsigned short)address)<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


// Actually do the dirty work for the C-65's DMAGIC chip
void NCbmIO::c65DoDMA(ULONG address)
{
  unsigned int t=0;
  bool problem=false;
  if(address&0xfff00000) {
    std::cerr<<"DMA list is outside of CPU's address space!\n";
  }
  unsigned int command=0;
  bool chain=false;
  unsigned int cntVal=0;
  unsigned int srcAddr=0;
  bool srcIO=false,srcDir=false,srcMod=false,srcHold=false;
  unsigned int destAddr=0;
  bool destIO=false,destDir=false,destMod=false,destHold=false;
  unsigned int modVal=0;
  DEBUG_OUT<<"Executing DMA list at "<<address<<":\n";
  bool working=true;
  while(working) {
    command=handler->getMap()->read8(address)&0x03;
    cntVal=handler->getMap()->read16l(address+1);
    srcAddr=handler->getMap()->read16l(address+3);
    srcAddr|=(handler->getMap()->read8(address+5)<<16)&0x0f0000;
    srcIO=handler->getMap()->read8(address+5)&0x80;
    srcDir=handler->getMap()->read8(address+5)&0x40;
    srcMod=handler->getMap()->read8(address+5)&0x20;
    srcHold=handler->getMap()->read8(address+5)&0x10;
    destAddr=handler->getMap()->read16l(address+6);
    destAddr|=(handler->getMap()->read8(address+8)<<16)&0x0f0000;
    destIO=handler->getMap()->read8(address+8)&0x80;
    destDir=handler->getMap()->read8(address+8)&0x40;
    destMod=handler->getMap()->read8(address+8)&0x20;
    destHold=handler->getMap()->read8(address+8)&0x10;
    modVal=handler->getMap()->read16l(address+9);
    chain=handler->getMap()->read8(address)&0x04;
    if(chain) problem=true;
    working=false;
    DEBUG_OUT<<"Command is "<<command;
    switch(command) {
      case 0:
        DEBUG_OUT<<" (copy)";
        break;
      case 3:
        DEBUG_OUT<<" (fill)";
        break;
      default:
        DEBUG_OUT<<" (unknown)";
        break;
    }
    if(chain) DEBUG_OUT<<" (chain)";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"Souce="<<srcAddr;
    if(srcIO) DEBUG_OUT<<" (IO)";
    if(srcDir) DEBUG_OUT<<" (DIR)"; else DEBUG_OUT<<" (!DIR)";
    if(srcMod) DEBUG_OUT<<" (MOD)";
    if(srcHold) DEBUG_OUT<<" (HOLD)";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"Dest= "<<destAddr;
    if(destIO) DEBUG_OUT<<" (IO)";
    if(destDir) DEBUG_OUT<<" (DIR)"; else DEBUG_OUT<<" (!DIR)";
    if(destMod) DEBUG_OUT<<" (MOD)";
    if(destHold) DEBUG_OUT<<" (HOLD)";
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"Count="<<cntVal<<"\n";
    DEBUG_OUT<<"Mod=  "<<modVal<<"\n";
    switch(command) {
      case 0x00:
        // COPY
        for(t=0;t<cntVal;t++)
          handler->getMap()->write8(destAddr+t,handler->getMap()->read8(srcAddr+t));
        break;
      case 0x03:
        // FILL
        for(t=0;t<cntVal;t++) handler->getMap()->write8(destAddr+t,srcAddr&0xff);
        break;
      default:
        DEBUG_OUT<<"Unimplemented DMA command!\n";
        problem=true;
        break;
    }
    if(problem) handler->getCPU()->debugger("C-65 DMA problem!");
  }
}


// *** The C-65's REC (not on the motherboard)  ***
BYTE NCbmIO::write8_c65rec(ULONG address, BYTE value)
{
  BYTE reg=address&0xff;
   switch(reg) {
    default:
      DEBUG_OUT<<"REC write at "<<aDefaultStringHelper.myHex((unsigned short)address)<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


// ********************************
// Chips Specific to the B/P series
// ********************************

// *** The B/P series' TIA's  ***
BYTE NCbmIO::read8_tias(ULONG address)
{
  BYTE reg=address&0xff;
  BYTE value=mem[address-location];
  switch(reg) {
    case 0x02:
      value=0xff;
      break;
    default:
      DEBUG_OUT<<"TIA REC read at "<<aDefaultStringHelper.myHex((unsigned short)address)<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


// *** The B/P series' TIA's  ***
BYTE NCbmIO::write8_tias(ULONG address, BYTE value)
{
  BYTE reg=address&0xff;
   switch(reg) {
    default:
      DEBUG_OUT<<"TIA write at "<<aDefaultStringHelper.myHex((unsigned short)address)<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


BYTE NCbmIO::read8_via(ULONG address)
{
  BYTE tval=mem[address-location];  // TODO: range checking...
  BYTE reg=address&0xf;
  switch(reg) {
    case 0x0:      // port b
      break;
    case 0xf:    case 0x1:      // port a
      tval=mem[address-location];
      break;
    case 0x2:      // data direction for b
      break;
    case 0x3:      // data direction for a
      break;
    case 0x4:      // timer 1 low
      state1a++;
      break;
    case 0x5:      // timer 1 high & counter
      break;
    case 0x6:      // timer 1 low
      state1a++;
      break;
    case 0x7:      // timer 1 high
      break;
    case 0x8:      // timer 2 low
      state1b++;
      break;
    case 0x9:      // timer 2 high
      break;
    case 0xa:      // shift register
      break;
    case 0xb:      // aux control reg
      break;
    case 0xc:      // control reg (ca1, ca2, cb1, cb2)
      break;
    case 0xd:      // interrupt flags
      break;
    case 0xe:      // interrupt enable
      break;
    default:
      DEBUG_OUT<<"via read at "<<address<<" of "<<(short)tval<<"\n";
      break;
  }
  return tval;
}


BYTE NCbmIO::read8_petvdc(ULONG address)
{
  BYTE value=mem[address-location];  // TODO: range checking...
  BYTE reg=address&0xf;
  switch(reg) {
    case 0x0:  // VDC Status
      // return status = ready
      value=0x9f;
      break;
    case 0x1:  // VDC data
      break;
    default:
      DEBUG_OUT<<"vdc read at "<<address<<" of "<<(short)value<<"\n";
      break;
  }
  return value;
}


BYTE NCbmIO::read8_pias(ULONG address)
{
  BYTE tval=mem[address-location];  // TODO: range checking...
  BYTE reg=address&0xff;
  BYTE c,value=tval;
  switch(reg) {
    case 0x10:  // Port A
      tval=0xff;
      //DEBUG_OUT<<"Reading pia $10 of "<<(short)tval<<"\n";
      break;
    case 0x12:  // Port B
      if(!((NCbmHandler *)handler)->keyScan) {
        value=0;
      }
      else
      if(!handler->getKeypress()) {
        value=0xff;
      }
      else {
        value=0xff;
        if(((NCbmHandler *)handler)->keys[0]&&(!(((NCbmHandler *)handler)->keyScan&0x01))) {
	  c=((NCbmHandler *)handler)->keys[0]-1; value=~(1<<c);
	}
        if(((NCbmHandler *)handler)->keys[1]&&(!(((NCbmHandler *)handler)->keyScan&0x02))) {
	  c=((NCbmHandler *)handler)->keys[1]-1; value=~(1<<c);
	}
        if(((NCbmHandler *)handler)->keys[2]&&(!(((NCbmHandler *)handler)->keyScan&0x04))) {
	  c=((NCbmHandler *)handler)->keys[2]-1; value=~(1<<c);
	}
        if(((NCbmHandler *)handler)->keys[3]&&(!(((NCbmHandler *)handler)->keyScan&0x08))) {
	  c=((NCbmHandler *)handler)->keys[3]-1; value=~(1<<c);
	}
        if(((NCbmHandler *)handler)->keys[4]&&(!(((NCbmHandler *)handler)->keyScan&0x10))) {
	  c=((NCbmHandler *)handler)->keys[4]-1; value=~(1<<c);
	}
        if(((NCbmHandler *)handler)->keys[5]&&(!(((NCbmHandler *)handler)->keyScan&0x20))) {
	  c=((NCbmHandler *)handler)->keys[5]-1; value=~(1<<c);
	}
        if(((NCbmHandler *)handler)->keys[6]&&(!(((NCbmHandler *)handler)->keyScan&0x40))) {
	  c=((NCbmHandler *)handler)->keys[6]-1; value=~(1<<c);
	}
        if(((NCbmHandler *)handler)->keys[7]&&(!(((NCbmHandler *)handler)->keyScan&0x80))) {
	  c=((NCbmHandler *)handler)->keys[7]-1; value=~(1<<c);
	}
        DEBUG_OUT<<"keyScan read "<<(short)((NCbmHandler *)handler)->keyScan<<":"<<(short)value<<"\n";
        ((NCbmHandler *)handler)->key_count++;
        if(((NCbmHandler *)handler)->key_count==2) {
          handler->setKeypress(0);  ((NCbmHandler *)handler)->key_count=((NCbmHandler *)handler)->key_max;
          for(temp=0;temp<NCBM_MAX_KEY_ROWS;temp++) ((NCbmHandler *)handler)->keys[temp]=0;
        }
      }
      tval=value;
      break;
    default:
      tval=(address&0xff00)>>8;
      DEBUG_OUT<<"pia read at "<<address<<" of "<<(short)tval<<"\n";
      break;
  }
  return tval;
}


BYTE NCbmIO::read8_c128mmuff00(ULONG address)
{
  BYTE value=mem[address-location];  // TODO: range checking...
  switch(address) {
    case 0xff00:
      value=((NCbmHandler *)handler)->getCRC128(0);
      break;
    case 0xff01:
      value=((NCbmHandler *)handler)->getCRC128(1);
      break;
    case 0xff02:
      value=((NCbmHandler *)handler)->getCRC128(2);
      break;
    case 0xff03:
      value=((NCbmHandler *)handler)->getCRC128(3);
      break;
    case 0xff04:
      value=((NCbmHandler *)handler)->getCRC128(4);
      break;
    default:
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}


BYTE NCbmIO::read8_c264(ULONG address)
{
  BYTE value=0; //underMem->read8(address);  // NOTE: This is different...
  unsigned int rv=0;
  BYTE c;
  switch(address) {
    case 0xfd30:
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xfdd0:  case 0xfdd1:  case 0xfdd2:  case 0xfdd3:
    case 0xfdd4:  case 0xfdd5:  case 0xfdd6:  case 0xfdd7:
    case 0xfdd8:  case 0xfdd9:  case 0xfdda:  case 0xfddb:
    case 0xfddc:  case 0xfddd:  case 0xfdde:  case 0xfddf:
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff00:  // Timer 1 low
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff01:  // Timer 1 high
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff02:  // Timer 2 low
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff03:  // Timer 2 high
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff04:  // Timer 3 low
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff05:  // Timer 3 high
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff06:  // Various: test, ecm, bmm, blank, rows, y scroll
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff07:  // Various:  rcs, pal, freeze, mcm, cols, x scroll
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff08:  // keyb latch
      //DEBUG_OUT<<keyScan<<":"<<value<<"\n";
      if(!((NCbmHandler *)handler)->keyScan) value=0;
      else
      if(!handler->getKeypress()) value=0xff;
      else {
        value=0xff;
        if(((NCbmHandler *)handler)->keys[0]&&(!(((NCbmHandler *)handler)->keyScan&0x01))) { c=((NCbmHandler *)handler)->keys[0]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[1]&&(!(((NCbmHandler *)handler)->keyScan&0x02))) { c=((NCbmHandler *)handler)->keys[1]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[2]&&(!(((NCbmHandler *)handler)->keyScan&0x04))) { c=((NCbmHandler *)handler)->keys[2]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[3]&&(!(((NCbmHandler *)handler)->keyScan&0x08))) { c=((NCbmHandler *)handler)->keys[3]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[4]&&(!(((NCbmHandler *)handler)->keyScan&0x10))) { c=((NCbmHandler *)handler)->keys[4]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[5]&&(!(((NCbmHandler *)handler)->keyScan&0x20))) { c=((NCbmHandler *)handler)->keys[5]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[6]&&(!(((NCbmHandler *)handler)->keyScan&0x40))) { c=((NCbmHandler *)handler)->keys[6]-1; value=~(1<<c); }
        if(((NCbmHandler *)handler)->keys[7]&&(!(((NCbmHandler *)handler)->keyScan&0x80))) { c=((NCbmHandler *)handler)->keys[7]-1; value=~(1<<c); }
        ((NCbmHandler *)handler)->key_count++;
        //DEBUG_OUT<<((NCbmHandler *)handler)->keyScan<<":"<<value<<"\n";
        if(((NCbmHandler *)handler)->key_count==2) {
          handler->setKeypress(0);  ((NCbmHandler *)handler)->key_count=((NCbmHandler *)handler)->key_max;
          for(temp=0;temp<NCBM_MAX_KEY_ROWS;temp++) ((NCbmHandler *)handler)->keys[temp]=0;
        }
      }
      //DEBUG_OUT<<value<<":";
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff09:  // IRQ con
      value=0;  // We didn't cause the IRQ
      if(handler->getVideo()) { if(RAST_IRQ&&handler->getVideo()->getIntCaused()) value=0x02; }
      if(value) value|=0x80;
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0a:  // IRQ en
      value=irq1en;
      rv=0;
      if(handler->getVideo()) { rv=handler->getVideo()->getRaster(); }
      if(rv&0x100) value|=0x01;
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0b:  // Raster
      if(handler->getVideo()) { value=handler->getVideo()->getRaster()&0xfe; }
      // This rounds off the raster to even lines
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0c:  // ???
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0d:  // ???
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0e:  // SND 1
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0f:  // SND 2
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff10:  // SND 2 cont...
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff11:  // SND con
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff12:  // SND 1 cont..., ted ram/rom, bmap address...
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff13:  // clock, char address
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff14:  // screen/color address
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff15:  // Color: background
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff16:  // Color #1
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff17:  // Color #2
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff18:  // Color #3
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff19:  // Color #4
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1a:  // bitmap reload?
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1b:  // bitmap reload?
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1c:  // vert line bit 8?
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1d:  // vert line bits 0-7?
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1e:  // horiz pos?
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1f:  // blink, vert sub addr?
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff3e:  // ROM bank switch
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff3f:  // RAM bank switch
      value=mem[address-location];
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
    default:
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}


BYTE NCbmIO::write8_c264(ULONG address, BYTE value)
{
  BYTE tvalue;
  switch(address) {
    case 0xfd30:
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xfdd0:  case 0xfdd1:  case 0xfdd2:  case 0xfdd3:
    case 0xfdd4:  case 0xfdd5:  case 0xfdd6:  case 0xfdd7:
    case 0xfdd8:  case 0xfdd9:  case 0xfdda:  case 0xfddb:
    case 0xfddc:  case 0xfddd:  case 0xfdde:  case 0xfddf:
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff00:  // Timer 1 low
      tlatch1a=(tlatch1a&0xff00)+value;
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff01:  // Timer 1 high
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      tlatch1a=(tlatch1a&0x00ff)+(value<<8);
      // Load TED Timer #1
      timer1a=tlatch1a;
      move_timer(0xff00,timer1a);
      run1a=true;
      if(state1a) {
        temp=read8(IFR1);
        temp=temp&(0xff-ta_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1a=0;
      }
      break;
    case 0xff02:  // Timer 2 low
      tlatch1b=(tlatch1b&0xff00)+value;
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff03:  // Timer 2 high
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      tlatch1b=(tlatch1b&0x00ff)+(value<<8);
      // Load TED Timer #2
      timer1b=tlatch1b;
      move_timer(0xff00,timer1b);
      run1b=true;
      if(state1b) {
        temp=read8(IFR1);
        temp=temp&(0xff-tb_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1b=0;
      }
      break;
    case 0xff04:  // Timer 3 low
      tlatch1c=(tlatch1c&0xff00)+value;
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff05:  // Timer 3 high
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      tlatch1c=(tlatch1c&0x00ff)+(value<<8);
      // Load TED Timer #3
      timer1c=tlatch1c;
      move_timer(0xff04,timer1c);
      run1c=true;
      if(state1c) {
        temp=read8(IFR1);
        temp=temp&(0xff-tc_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1c=0;
      }
      break;
    case 0xff06:  // Various: test, ecm, bmm, blank, rows, y scroll
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff07:  // Various:  rcs, pal, freeze, mcm, cols, x scroll
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff08:  // keyb latch
      ((NCbmHandler *)handler)->keyScan=value;
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff09:  // IRQ con
      tvalue=value;
      value=mem[address-location];
      value=value&(~tvalue);
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0a:  // IRQ en
      raster_check&=0xff;
      if(value&0x01) raster_check|=0x100;
      irq1en=value;  // set/reset
      if(value&0x02) RAST_IRQ=true; else RAST_IRQ=false;
      if(handler->getVideo()) {
        if(RAST_IRQ) handler->getVideo()->setRasterInt(raster_check,E6502_INT_IRQ);
        else handler->getVideo()->unsetRasterInt();
      }
      //if(win) DEBUG_OUT<<"{ints enabled #1: "<<irq1en<<"}\n";
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0b:  // Raster
      raster_check&=0x100;
      raster_check|=value;
      if(handler->getVideo()) {
        if(RAST_IRQ) handler->getVideo()->setRasterInt(raster_check,E6502_INT_IRQ);
      }
      value=mem[address-location];
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0c:  // ???
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0d:  // ???
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0e:  // SND 1
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff0f:  // SND 2
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff10:  // SND 2 cont...
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff11:  // SND con
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff12:  // SND 1 cont..., ted ram/rom, bmap address...
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff13:  // clock, char address
      if(handler->getVideo()) {
        if(value>0x10) handler->getVideo()->setScreenChar(0x0c00,0xd000);
        else handler->getVideo()->setScreenChar(0x0c00,0xc000);
      }
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff14:  // screen/color address
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff15:  // Color: background
      if(handler->getVideo()) handler->getVideo()->setBackground(value);
      break;
    case 0xff16:  // Color #1
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff17:  // Color #2
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff18:  // Color #3
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff19:  // Color #4
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1a:  // bitmap reload?
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1b:  // bitmap reload?
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1c:  // vert line bit 8?
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1d:  // vert line bits 0-7?
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1e:  // horiz pos?
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff1f:  // blink, vert sub addr?
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      break;
    case 0xff3e:  // ROM bank switch
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      ((NCbmHandler *)handler)->handleMMUC264(0xff);
      break;
    case 0xff3f:  // RAM bank switch
      //DEBUG_OUT<<"write at "<<address<<" of "<<value<<"\n";
      ((NCbmHandler *)handler)->handleMMUC264(0);
      break;
    default:
      //underMem->write8(address,value);  // NOTE: This is different...
      break;
  }
  return value;
}


BYTE NCbmIO::write8_via(ULONG address, BYTE value)
{
  BYTE reg=address&0xf;
  BYTE tval=value;
  switch(reg) {
    case 0x0:      // port b
      break;
    case 0xf:    case 0x1:      // port a
      mem[0xe84f-location]=value;
      mem[0xe841-location]=value;
      break;
    case 0x2:      // data direction for b
      break;
    case 0x3:      // data direction for a
      break;
    case 0x4:      // timer 1 low latch
      tlatch1a=(tlatch1a&0xff00)+value;
      break;
    case 0x5:      // timer 1 high latch
      tlatch1a=(tlatch1a&0x00ff)+(value<<8);
      // Load VIA 1 Timer #1
      timer1a=tlatch1a;
      move_timer(0xe844,timer1a);
      move_timer(0xe846,timer1a);
      run1a=true;
      if(state1a) {
        temp=read8(IFR1);
        temp=temp&(0xff-ta_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1a=0;
      }
      break;
    case 0x6:      // timer 1 low load
      tlatch1a=(tlatch1a&0xff00)+value;
      break;
    case 0x7:      // timer 1 high load
      tlatch1a=(tlatch1a&0x00ff)+(value<<8);
      run1a=true;
      if(state1a) {
        temp=read8(IFR1);
        temp=temp&(0xff-ta_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1a=0;
      }
      break;
    case 0x8:      // timer 2 low
      tlatch1b=(tlatch1b&0xff00)+value;
      break;
    case 0x9:      // timer 2 high
      tlatch1b=(tlatch1b&0x00ff)+(value<<8);
      // Load VIA 1 Timer #2
      timer1b=tlatch1b;
      move_timer(0xe848,timer1b);
      run1b=true;
      if(state1b) {
        temp=read8(IFR1);
        temp=temp&(0xff-tb_int);
        if(temp==0x80) temp=0;
        write8(IFR1,temp);
        state1b=0;
      }
      break;
    case 0xa:      // shift register
      break;
    case 0xb:      // aux control reg #1
      if(value&0x40) free1a=true; else free1a=false;
      if(value&0x20) free1b=true; else free1b=false;
      break;
    case 0xc:      // control reg (ca1, ca2, cb1, cb2)
      break;
    case 0xd:      // interrupt flags
      //if(win) DEBUG_OUT<<"{write int flags #1: "<<value<<"}\n";
      break;
    case 0xe:      // interrupt enable
      if(tval&0x80) irq1en=irq1en|(tval&0x7f);  // set
      else irq1en=irq1en&(~tval);  // clear
      //if(win) DEBUG_OUT<<"{ints enabled #1: "<<irq1en<<"}\n";
      value=mem[address-location];
      break;
    default:
      break;
  }
  return value;
}


BYTE NCbmIO::write8_petvdc(ULONG address, BYTE value)
{
  BYTE reg=address&0xf;
  switch(reg) {
    case 0x0:  // VDC register select
      break;
    case 0x1:  // VDC data
      break;
    default:
      break;
  }
  return value;
}


BYTE NCbmIO::write8_pias(ULONG address, BYTE value)
{
  BYTE reg=address&0xff;
  unsigned int tint=0;
  switch(reg) {
    case 0x10:
      switch(value) {
        case 0x00: tint=0x01; break;
        case 0x01: tint=0x02; break;
        case 0x02: tint=0x04; break;
        case 0x03: tint=0x08; break;
        case 0x04: tint=0x10; break;
        case 0x05: tint=0x20; break;
        case 0x06: tint=0x40; break;
        case 0x07: tint=0x80; break;
	default:
          debugger("keyScan write value not decoded");
	  break;
      }
      DEBUG_OUT<<"keyScan write "<<(short)value<<" decoded as "<<tint<<"\n";
      ((NCbmHandler *)handler)->keyScan=tint;
      break;
    case 0x11:
      if(value&0x1) DEBUG_OUT<<"Pia #1 CA1 IRQ on!\n";
      if(value&0x4) DEBUG_OUT<<"Pia #1 CA2 IRQ on!\n";
      break;
    case 0x12:
      mem[reg]=value;
      DEBUG_OUT<<"write at pia $12 of "<<(short)value<<"\n";
      break;
    case 0x13:
      if(value&0x1) DEBUG_OUT<<"Pia #1 CB1 IRQ on!\n";
      if(value&0x4) DEBUG_OUT<<"Pia #1 CB2 IRQ on!\n";
      break;
    case 0x21:
      if(value&0x1) DEBUG_OUT<<"Pia #2 CA1 IRQ on!\n";
      if(value&0x4) DEBUG_OUT<<"Pia #2 CA2 IRQ on!\n";
      break;
    case 0x22:
      // IEEE data out
      break;
    case 0x23:
      if(value&0x1) DEBUG_OUT<<"Pia #2 CB1 IRQ on!\n";
      if(value&0x4) DEBUG_OUT<<"Pia #2 CB2 IRQ on!\n";
      break;
    default:
      DEBUG_OUT<<"Unknown pia write at "<<address<<" of "<<(short)value<<"\n";
      break;
  }
  return value;
}


BYTE NCbmIO::write8_c128mmuff00(ULONG address, BYTE value)
{
  switch(address) {
    case 0xff00:
      ((NCbmHandler *)handler)->handleMMUC128(value);
      break;
    case 0xff01:
      ((NCbmHandler *)handler)->handleMMUC128(((NCbmHandler *)handler)->getCRC128(1));
      break;
    case 0xff02:
      ((NCbmHandler *)handler)->handleMMUC128(((NCbmHandler *)handler)->getCRC128(2));
      break;
    case 0xff03:
      ((NCbmHandler *)handler)->handleMMUC128(((NCbmHandler *)handler)->getCRC128(3));
      break;
    case 0xff04:
      ((NCbmHandler *)handler)->handleMMUC128(((NCbmHandler *)handler)->getCRC128(4));
      break;
    default:
      break;
  }
  return value;
}




/*
NCbmIOHigh::NCbmIOHigh(NCbmHandler *parent) : EMemIO(parent)
{
  xia=(EIOChip *)NULL;
  letter='i';
  name=strdup("io.high"));
  ta_int=0; tb_int=0;  tc_int=0;
  IFR1=0;
  RAST_IRQ=false;
  raster_check=0;
  switch(type) {
    case N_CBM_IO_C128:
      location=0xff00;  resize(256);
      //specialStart=0;  specialEnd=5;
      //setUnder("kern128.rom"));
      break;
    case N_CBM_IO_TED:
      location=0xfd00;  resize(0x300);
      //specialStart=0;  specialEnd=0x23f;
      //setUnder("kern264.rom"));
      IFR1=0xff09;
      ta_int=0x08;  tb_int=0x10;  tc_int=0x40;
      break;
    case N_CBM_IO_PET:
      location=0xe800;  resize(256);
      //specialStart=0;  specialEnd=0xff;
      //setUnder("pet.rom"));
      ((NCbmHandler *)io)->hasVIA=true;
      break;
    default:
      break;
  }
  if(((NCbmHandler *)io)->hasVIA) {
    ta_int=0x40;  tb_int=0x20;
    IFR1=0xe8cd;  // Is this right?
  }
  irq1en=0;
  tlatch1a=0;  tlatch1b=0;  tlatch1c=0;
  timer1a=0;  timer1b=0;  timer1c=0;
  free1a=false;  free1b=false;  free1c=false;
  run1a=false;  run1b=false;  run1c=false;
  state1a=0;  state1b=0;  state1c=0;
  reset();
}
*/


/*
void NCbmIOHigh::reset()
{
  unsigned int t=0;
  ((NCbmHandler *)handler)->key_count=0;  ((NCbmHandler *)handler)->key_max=0;
  for(t=0;t<NCBM_MAX_KEY_ROWS;t++) ((NCbmHandler *)handler)->keys[t]=0;
  run1a=false;  run1b=false;  run1c=false;
  irq1en=0;
  for(t=0;t<memsize;t++) write8(location+t,0);
  switch(type) {
    case N_CBM_IO_TED:
      write8(0xff3e,0);
      break;
    case N_CBM_IO_C128:
      // write(0xff00,0);
      break;
    case N_CBM_IO_PET:
      // PET's have an IRQ hard wired to their raster
      RAST_IRQ=true;
      if(handler->getVideo()) {
        handler->getVideo()->setRasterFactor(10);
        handler->getVideo()->setRasterSpeed(1);
        handler->getVideo()->setRasterInt(raster_check,E6502_INT_IRQ);
	DEBUG_OUT<<"Sucessfully setRaster in handler->getVideo()\n";
      }
      else DEBUG_OUT<<"Couldn't setRaster, no handler->getVideo()!\n";
      break;
    default: break;
  }
}
*/


/*
void NCbmIOHigh::io_cycle()
{
  // Processing
  int speed=4;
  if(run1a) {
    if(timer1a) timer1a-=speed;
    if(timer1a<0) timer1a=0;
    if(!timer1a) {
      //DEBUG_OUT<<"%";
      timer1a=0;
      if(irq1en&ta_int) {
        if(handler->getCPU()) handler->getCPU()->cause_int(E6502_INT_IRQ);
        if(IFR1) {
          temp=read8(IFR1);
          temp=temp|0x80;
          temp=temp|ta_int;
          write8(IFR1,temp);
        }
      }
      if(free1a) timer1a=tlatch1a;
      else run1a=false;
    }
  }
  if(run1b) {
    if(timer1b) timer1b-=speed;
    if(timer1b<0) timer1b=0;
    if(!timer1b) {
      //DEBUG_OUT<<"%";
      timer1b=0;
      if(irq1en&tb_int) {
        if(handler->getCPU()) handler->getCPU()->cause_int(E6502_INT_IRQ);
        if(IFR1) {
          temp=read8(IFR1);
          temp=temp|0x80;
          temp=temp|tb_int;
          write8(IFR1,temp);
        }
      }
      if(free1b) timer1b=tlatch1b;
      else run1b=false;
    }
  }
  if(run1c) {
    if(timer1c) timer1c-=speed;
    if(timer1c<0) timer1c=0;
    if(!timer1c) {
      //DEBUG_OUT<<"%";
      timer1c=0;
      if(irq1en&tc_int) {
        if(handler->getCPU()) handler->getCPU()->cause_int(E6502_INT_IRQ);
        if(IFR1) {
          temp=read8(IFR1);
          temp=temp|0x80;
          temp=temp|tc_int;
          write8(IFR1,temp);
        }
      }
      if(free1c) timer1c=tlatch1c;
      else run1c=false;
    }
  }
  // Post processing
  if(((NCbmHandler *)io)->hasVIA) {
    move_timer(0xe844,timer1a);
    move_timer(0xe846,timer1a);
    move_timer(0xe848,timer1b);
  }
}
*/


/*
void NCbmIOHigh::ioregs()
{
  //if(win) {
    switch(type) {
      case N_CBM_IO_PET:
      case N_CBM_IO_TED:
        DEBUG_OUT<<"IRQs: ";
        DEBUG_OUT<<"irq1en="<<(short)irq1en<<"\n";
        DEBUG_OUT<<"Timers:\n";
        DEBUG_OUT<<"Latches="<<tlatch1a<<","<<tlatch1b<<","<<tlatch1c<<"\n";
        DEBUG_OUT<<"Values="<<timer1a<<","<<timer1b<<","<<timer1c<<"\n";
        DEBUG_OUT<<"Continuous="<<free1a<<","<<free1b<<","<<free1c<<"\n";
        DEBUG_OUT<<"Running="<<run1a<<","<<run1b<<","<<run1c<<"\n";
        DEBUG_OUT<<"key_count="<<(short)((NCbmHandler *)handler)->key_count<<"  key_max="<<(short)((NCbmHandler *)handler)->key_max<<"\n";
        DEBUG_OUT<<"RAST_IRQ="<<RAST_IRQ<<"\n";
        break;
      default:
        break;
    }
  //}
}
*/


/*
BYTE NCbmIOHigh::write8Special(ULONG address,BYTE value)
{
  BYTE page=(address>>8)&0xff;
  BYTE nib2=address&0x00f0;
  if(io->io_debug) DEBUG_OUT<<"I/O write at "<<address<<" of "<<(short)value<<"\n";
  switch(type) {
    case N_CBM_IO_TED:
      value=write8_c264(address,value);
      break;
    case N_CBM_IO_C128:
      value=write8_c128mmuff00(address,value);
      break;
    case N_CBM_IO_PET:
      value=mem[address-location];
      switch(nib2) {
        case 0x10:  case 0x20:
          value=write8_pias(address,value); break;
        case 0x80:
          value=write8_petvdc(address,value); break;
        case 0x40: case 0x50: case 0x60: case 0x70:
        case 0xc0: case 0xd0: case 0xe0: case 0xf0:
          value=write8_via(address,value); break;
        default:
          value=page;
	  DEBUG_OUT<<"write at "<<address<<" outside of chip ranges!\n";
          break;
      }
      break;
    default:
      break;
  }
  return value;
}
*/


/*
BYTE NCbmIOHigh::read8Special(ULONG address)
{
  BYTE page=(address>>8)&0xff;
  BYTE nib2=address&0x00f0;
  if(io->io_debug) DEBUG_OUT<<"I/O read  at "<<address<<"\n";
  switch(type) {
    case N_CBM_IO_TED:
      return read8_c264(address);
      break;
    case N_CBM_IO_C128:
      return read8_c128mmuff00(address);
      break;
    case N_CBM_IO_PET:
      switch(nib2) {
        case 0x10:  case 0x20:
          return read8_pias(address); //break;
        case 0x80:
          return read8_petvdc(address); //break;
        case 0x40: case 0x50: case 0x60: case 0x70:
        case 0xc0: case 0xd0: case 0xe0: case 0xf0:
          return read8_via(address); //break;
        default:
	  DEBUG_OUT<<"Read at "<<address<<" outside of chip ranges!\n";
          return page;  //break;
      }
      break;
    default:
      return page;  //break;

  }
  return page;  // as good as any value, eh?
}
*/
