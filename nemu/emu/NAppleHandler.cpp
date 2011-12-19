
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAppleHandler.h>
#include <emu/NAppleDos.h>
#include <emu/NAppleIO.h>
#include <emu/NAppleVideo.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>

#include <cpulib/Em6502.h>

#include <portable/nickcpp/AStringHelper.h>


/* Slot usage in 2gs
slot 0: Language card
slot 1: Printer/modem
slot 2: Modem/printer
slot 3: 80 column card or no card allowed
slot 4: Mouse card or clock card or any card
slot 5: 3.5 in disks
slot 6: 5.25 in disks
slot 7: AppleTalk card or any card
*/


/* Floppy boot sequence
Read track 0, sector 0
Read track 0, sectors 0-9
Read Tracks 0-2
*/


/* IIgs memory banks
00-7f fast ram
e0-e1 slow ram mega2 (video)
f0-ff fast rom
fe-ff rom in 256k system
fc-ff rom in 1m system (with toolbox in rom)
*/

/* apple 2 maps
0000-0100 direct page
0100-0200 stack
0200-0300 input buffer
0300-0400 link addrs and asm routines
0400-0800 text page 1 and lo res gfx (both bank 0 and 1)
0800-2000 ram
2000-4000 hires page 1 (bank 0 only) (double hires both banks)
4000-6000 hires page 2 (bank 0 only) (double hires both banks)
2000-a000 super hires page
6000-c000 ram
c000-d000 i/o (both banks)
d000-ffff language card area (both banks)

hires pages can be shadowed to appear in both banks
d000-dfff can be switched between two 4k blocks
*/

/* language card switches
c080 reaad to read ram then wp ram and use d000 bank 2
c081 read 2 times to read rom, we ram and use d000 bank 2
c082 read to read rom, wp ram and use d000 bank 2
c083 read 2 times to read rom, we ram and use d000 bank 2
c088 read to read rom, wp ram and use d000 bank 2
c089 read 2 times to read rom, we ram and use d000 bank 2
c08a read to read rom, wp ram and use d000 bank 2
c08b read 2 times to read ram, we ram and use d000 bank 2
c011 test bit 7 for swtich status d000 1=bank2 0=bank1
c012 test bit 7 for swtich status 1=ram 0=rom
c008 write to use main bank page 0 and page 1
c009 wrute to use aux bank page 0 and page 1
c016 test bit 7 for switch status 1=aux 0=main
*/

/* soft switches
c000 w 80 col off
c001 w 80 col on
c002 w read main
c003 w read aux
c004 w write main
c005 w write aux
c008 w main stack and zp
c009 w aux stack and zp
c054 w text page 1 main mem
c055 w text page 2 aux mem
c056 w off, use ram swtiches
c057 w on, use hires pages
c013 r bit 7
c014 r bit 7
c016 r bit 7
c018 r bit 7
c01c r bit 7
c01d r bit 7
*/

/* lores colors (also text and background)
0 black
1 deep red
2 dark blue
3 purple
4 dark green
5 dark gray
6 med blue
7 light blue
8 brown
9 orange
10 light gray
11 pink
12 light green
13 yellow
14 aqua
15 white
*/

/* hires colors
bit 7 on
both cols off black
even on purple
odd on green
both cols on white
bit 7 off
both cols off black
even on blue
odd on orange
both cols on white
*/

/* double hires color bit patterns (not nibbles)
0 black
1 deep red
2 brown
3 orange
4 dark green
5 dark gray
6 green
7 yellow
8 dark blue
9 purple
10 light gray
11 pink
12 med blue
13 light blue
14 aqua
15 white
*/

/* char sets
00-1f upper inv
20-3f special inv
40-5f upper flashing
60-7f special flashing
80-9f upper normal
a0-bf special normal
c0-df upper normal
e0-ff lower normal
or alt
00-1f upper inv
20-3f special inv
40-5f mousetxt inv
60-7f lower inv
80-9f upper normal
a0-bf special normal
c0-df upper normal
e0-ff lower normal
*/

/* gfx modes
40 col text
80 col text
lores 40x48 16 colors
hires 150x192 6 colors
hires hires 280x192 mono
double hires 140x192 16 colors
double hires 560x192 mono
*/

/* display soft switches
c000 w disable 80 col store
c001 w enable 80 col store
c00c w disable 80 col hardware
c00d w enable 80 col hardware
c00e w chars norm lower, flashing upper
c00f w chars nrom, inv no flashing
c018 read bit 7 80 store enabled
c019 read bit 7 not vblank
c01a read bit 7 text mode
c01b read bit 7 mixed mode
c01c read bit 7 text page 2
c01d read bit 7 hires
c01e read bit 7 alt char set
c01f read bit 7 80 col hardware in use
c07f read bit 7 double hires
c050 r/w standard gfx or mixed mode
c051 r/w text only
c052 r/w clear mixed mode
c053 r/w mixed mode
c054 r/w text page 1
c055 r/w text page 2 or text page 1 in aux
c056 r/w lo res gfx
c057 r/w hires gfx or double hires
c05e r/w enable double hires
c05f r/w disable double hires
*/

/* glu registers
c03c r/w sound control
c03d data
c03e address low byte
c03f address high byte
*/

/* scc registers
c038 scc command chan b
c039 scc command chan a
c03a scc data chan b
c03b scc data chan a
*/

/* annunciator registers
c058-c05f clear/set 0-3
c060-c063 read switches
c064-c067 paddles
c070 analog reset
*/

/* misc
scan line control bytes 9d00-9dc7
color palettes 9e00-9fff
c035 shadow reg
c036 speed reg
c068 state reg
c023 vgc int reg
c032 vgc int clear reg
c022 scren color
c034 border color
c021 mono/color
c026 adb command/data
c025 mod keys
c024 mouse data
c027 adb status
c034 control reg
c02d slot reg
c029 new video
128k or 256k rom
256k or 1m ram
*/

/* shadow reg at c035
from e0/e1 to 00/01
bit 7 reserved
bit 6 inhibit io and lang card
bit 5 inhibit text page 2 shadow
bit 4 inhibit aux hires shadow
bit 3 inhibit super hires shadow
bit 2 inhibit hires page 2 shadow
bit 1 inhibit hires page 1 shadow
bit 0 inhibit text page 1 shadow
*/

/* vgc int reg at c023
bit 7 vgc int status
bit 6 1 sec int status
bit 5 scan line int status
bit 4 reserved
bit 3 reserved
bit 2 1 sec int enable
bit 1 scan line int enable
bit 0 reserved
*/

/* vgc int clear reg at c032
bit 7 reserved
bit 6 clear 1 sec int
bit 5 clear scan line int
bit 4-0 reserved
*/

/* color reg at c022
bits 7-4 text color
bits 3-0 background color
*/

/* reg at c034
bits 7-4 real time clcck control
bits 3-0 border color
*/

/* mono color reg at c021
bit 7 mono if 1
*/

/* scanline control byte
bit 7 640/320
bit 6 int enabled for this line
bit 5 color fill enabled
bit 4 reserved
bit 3-0 palette for this line
*/

/* color palettes
even byte: green, blue
odd byte: reserved, red
*/

/* what are they?
c007 write only?
c015 read only?
c033 read/write color of some sort?
*/


static ULONG NApple_Dos[]={
  // dos calls
  (ULONG)"dos-warm-start",0x03d0,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-cold-start",0x03d3,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-file-manager",0x03d6,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-rwts",0x03d9,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-input-param",0x03dc,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-iob-param",0x03e3,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-set-vectors",0x03ea,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-handle-brk",0x03ef,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-ampersand",0x03f5,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-control-y",0x03f8,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"dos-nmi",0x03fb,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  // slot inits...
  (ULONG)"Slot1Init",0xc101,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  (ULONG)"Slot2Init",0xc201,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  (ULONG)"Slot3Init",0xc301,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  (ULONG)"Slot4Init",0xc401,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  (ULONG)"Slot5Init",0xc501,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  (ULONG)"Slot6Init",0xc601,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  (ULONG)"Slot7Init",0xc701,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  // aux/80 col card
  (ULONG)"AUXMOVE",0xc311,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  (ULONG)"XFER",0xc314,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  // disk2 card
  (ULONG)"BTRDSEC",0xc65c,EOSENTRY_PATCH,N_APPLE_IO_APPLE2,
  // monitor calls
  (ULONG)"keyboard_in",0xfd1b,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  (ULONG)"display_out",0xfdf0,EOSENTRY_DONT_PATCH,N_APPLE_IO_NONE,
  0,0,0,0
};


//////////////////////////////////////////////////////////////////////////////
//  NAppleHandler Class
//////////////////////////////////////////////////////////////////////////////

NAppleHandler::NAppleHandler()
{
  DEBUG_OUT<<"NAppleHandler constructor.\n";
  osHelp=(NAppleDos *)NULL;
  init();
}


NAppleHandler::~NAppleHandler()
{
  DEBUG_OUT<<"NAppleHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


unsigned int NAppleHandler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"apple1")) return N_APPLE_IO_APPLE1;
  if(!strcmp(name,"apple2")) return N_APPLE_IO_APPLE2;
  if(!strcmp(name,"apple2+")) return N_APPLE_IO_APPLE2;
  if(!strcmp(name,"apple2plus")) return N_APPLE_IO_APPLE2;
  if(!strcmp(name,"apple2e")) return N_APPLE_IO_APPLE2E;
  if(!strcmp(name,"apple2c")) return N_APPLE_IO_APPLE2E;
  if(!strcmp(name,"apple2gs")) return N_APPLE_IO_APPLE2GS;
  if(!strcmp(name,"apple2x")) return N_APPLE_IO_APPLE2GS;
  if(!strcmp(name,"apple3")) return N_APPLE_IO_APPLE3;
  return 0;
}


const char *NAppleHandler::getName()
{
  if(io_type==N_APPLE_IO_APPLE1) return "Apple-1"; else
  if(io_type==N_APPLE_IO_APPLE2) return "Apple-2"; else
  if(io_type==N_APPLE_IO_APPLE2E) return "Apple-2e"; else
  if(io_type==N_APPLE_IO_APPLE3) return "Apple-3"; else
  if(io_type==N_APPLE_IO_APPLE2GS) return "Apple-IIgs"; else
  return "Unknown Apple";
}


/* STATIC */
bool NAppleHandler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


void NAppleHandler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


void NAppleHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_APPLE_IO_APPLE1:
    case N_APPLE_IO_APPLE2:
    case N_APPLE_IO_APPLE2E:
    case N_APPLE_IO_APPLE2GS:
      myIO=new NAppleIO(this);
      myVideo=new NAppleVideo(this,40,24);
      break;
    case N_APPLE_IO_APPLE3:
      myIO=new NAppleIO(this);
      myVideo=new NAppleVideo(this,40,24);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_APPLE_IO_NONE;
      exit(5);
      return;
      break;
  }
  connectParts();
  mapParts();
  osHelp=new NAppleDos(this);
  osHelp->patchAll(NApple_Dos);
}


// NOTE: Assumes 256 pages of 256 bytes each
bool NAppleHandler::read8ret(ULONG addr,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    case 0xff:
      if(io_type==N_APPLE_IO_APPLE3) { *ret=((NAppleIO *)myIO)->read8apple3(addr); return true; }
      break;
    default:
      return false;
      break;
  }
  return false;
}


// NOTE: Assumes 256 pages of 256 bytes each
bool NAppleHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    case 0xff:
      if(io_type==N_APPLE_IO_APPLE3) { *ret=((NAppleIO *)myIO)->write8apple3(addr,val); return true; }
      break;
    default:
      return false;
      break;
  }
  return false;
}

