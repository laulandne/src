
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NCpmHandler.h>
#include <emu/NCpmIO.h>
#include <emu/NCpmDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EVideoIOMultiMode.h>


/*
Notes:

Programs loaded at 0100 (called "TBASE")
From 0100 to ccp is "TPA" where user progs loaded

bios at top
then bdos under that
and then ccp
(all three approx6.5k)
"CBASE" marks start of ccp (top of TPA)
"FBASE" marks start of bdos
(nothing marks start of bios above bdos)


call 0x0005 for system call


0x0000 (3 bytes) jump to WBOOT (warm boot, aka switched the disk)
0x0003 (1 byte) IOBYTE
0x0004 (1 byte) current drive number
0x0005 (3 bytes) jump to BDOS
0x0008 (40 bytes) for 8080 RSTXXX instructions (not used by CP/M)
0x0030 (16 bytes) reserved for future expansion
0x0040 (16 bytes) scrach area for bios
0x0050 (12 bytes) reserved for future expansion
0x005c (33 bytes) default FCB
0x007d (3 bytes) reserved for future expansion

0x0080-0x00ff: disk sector buffer (128 bytes)


BIOS calls: (jump table at start of bios...JMP BOOT...JMP WBOOT...etc)
(init routines)
BOOT: cold boot
WBOOT: warm boot
(slow io routines)
CONST: check char ready at console
CONIN: read char from console
CONOUT: write char to console
LIST: write char to listing device
PUNCH: write char to punch device
READER: read char from reader device
(fast io routines)
HOME: move head to track 0
SELDSK: identify disk drive to use
SETTRK: ident desired track
SETSEC: ident desired sector
SETDMA: prepare dma device for a data transfer
READ: read sector indicated by SETTRK and SETSEC
WRITE: read sector indicated by SETTRK and SETSEC


FCB Format: (note 1 byte larger than disk version)
0: ET entry type (not used in cp/m 1.4)
1-8: FN filename
9-11: FT file type
12: EX Extent (normally 0 except for really large files)
13-14: not used
15: RC record count # of recs for this extent
16-31: DM disk allocation map, shows clusters owned by file and sectors in use within clusters
32: NR next record for reading or writing

BDOS calls: (function # in c, call 5)
(init)
0 System reset
(console and list io)
1 read console
2 write console
3 read reader
4 write punch
5 write list
6 not used
7 interrogate i/o status
8 alter io status
9 print console buffer
10 read console buffer
11 check console status
(file management)
12 lift disk head
13 reset disk system
14 select disk
15 open file
16 close file
17 search first
18 search next
19 delete file
20 read record
21 write record
22 create file
23 rename file
24 interrogate login
25 interrogate disk
26 set dma address
27 interrogate allocation


*/


static ULONG NCpm_Dos[]={
  0,0,0,0
};


////////////////////////////////////////////////////////////////////////////////
//  NCpmHandler Class
////////////////////////////////////////////////////////////////////////////////

NCpmHandler::NCpmHandler()
{
  DEBUG_OUT<<"NCpmHandler constructor.\n";
  osHelp=(NCpmDos *)NULL;
  init();
}


NCpmHandler::~NCpmHandler()
{
  DEBUG_OUT<<"NCpmHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


void NCpmHandler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


const char *NCpmHandler::getName()
{
  if(io_type==N_CPM_IO_STANDARD) return "CP/M";
  if(io_type==N_CPM_IO_SPECTRUM) return "Spectrum CP/M";
  if(io_type==N_CPM_IO_SAMCOUPE) return "Sam Coupe CP/M";
  if(io_type==N_CPM_IO_AMSTRAD) return "Amstrad CP/M";
  if(io_type==N_CPM_IO_MICROBEE) return "Microbee CP/M";
  if(io_type==N_CPM_IO_KAYPRO) return "Kaypro CP/M";
  if(io_type==N_CPM_IO_OSBORNE) return "Osborne CP/M";
  if(io_type==N_CPM_IO_MSX) return "MSX CP/M";
  return "Unknown CP/M";
}


/* STATIC */
bool NCpmHandler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


/* STATIC */
unsigned int NCpmHandler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"kaypro")) return N_CPM_IO_KAYPRO;
  if(!strcmp(name,"osborne")) return N_CPM_IO_OSBORNE;
  if(!strcmp(name,"amstrad")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"cpc")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"amstradcpc")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"amstrad464")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"cpc464")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"amstradcpc464")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"amstrad664")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"cpc664")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"amstradcpc664")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"amstrad6128")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"cpc6128")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"amstradcpc6128")) return N_CPM_IO_AMSTRAD;
  if(!strcmp(name,"spectrum")) return N_CPM_IO_SPECTRUM;
  if(!strcmp(name,"sinclair")) return N_CPM_IO_SPECTRUM;
  if(!strcmp(name,"samcoupe")) return N_CPM_IO_SAMCOUPE;
  if(!strcmp(name,"sam")) return N_CPM_IO_SAMCOUPE;
  if(!strcmp(name,"coupe")) return N_CPM_IO_SAMCOUPE;
  if(!strcmp(name,"microbee")) return N_CPM_IO_MICROBEE;
  if(!strcmp(name,"msx")) return N_CPM_IO_MSX;
  if(!strcmp(name,"msx2")) return N_CPM_IO_MSX;
  if(!strcmp(name,"cpm")) return N_CPM_IO_STANDARD;
  if(!strcmp(name,"cp/m")) return N_CPM_IO_STANDARD;
  return N_CPM_IO_NONE;
}


void NCpmHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_CPM_IO_STANDARD:
      myVideo=new EVideoIOMultiMode(this,80,24);
      myIO=new NCpmIO(this);
      break;
    case N_CPM_IO_AMSTRAD:
      myVideo=new EVideoIOMultiMode(this,80,24);
      myIO=new NCpmIO(this);
      break;
    case N_CPM_IO_SPECTRUM:
      myVideo=new EVideoIOMultiMode(this,80,24);
      myIO=new NCpmIO(this);
      break;
    case N_CPM_IO_SAMCOUPE:
      myVideo=new EVideoIOMultiMode(this,80,24);
      myIO=new NCpmIO(this);
      break;
    case N_CPM_IO_MICROBEE:
      myVideo=new EVideoIOMultiMode(this,80,24);
      myIO=new NCpmIO(this);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_CPM_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  osHelp=new NCpmDos(this);
  osHelp->patchAll(NCpm_Dos);
}


bool NCpmHandler::read8ret(ULONG addr,BYTE *ret)
{
  return false;
}


bool NCpmHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  return false;
}


void NCpmHandler::io_out8(ULONG addr, BYTE val)
{
  DEBUG_OUT<<"out8 "<<addr<<","<<(int)val<<"\n";
  switch(addr) {
    case 0xff:
      // Cassette IO
      break;
    default:
      debugger("NCpmHandler::io_out8 unimplemented address");
      break;
  }
}


BYTE NCpmHandler::io_in8(ULONG addr)
{
  BYTE ret=0;
  DEBUG_OUT<<"in8 "<<addr<<"\n";
  switch(addr) {
    default:
      debugger("NCpmHandler::io_in8 unimplemented address");
      break;
  }
  return ret;
}

