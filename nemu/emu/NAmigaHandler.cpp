
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

NOTE: This code doesn't work if an int is 16-bits...

*/


#include <emu/NAmigaHandler.h>
#include <emu/NAmigaIO.h>
#include <emu/NTinyHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOCia8520.h>
#include <elib/EVideoIOBitmap.h>

#include <cpulib/Em68k.h>


/* Rough Amiga 24-bit memory map:
00 0000 - 20 0000  chip ram
20 0000 - a0 0000  fast ram
a0 0000 - b8 0000  empty?
b8 0000 - c0 0000  cias
c0 0000 - d8 0000  slow ram
d8 0000 - e0 0000  custom and clock
e0 0000 - f0 0000  autoconfig io
f0 0000 - ff ffff  rom
*/


/*

// NAmiga custom chip defs

struct Custom {
    UUINT16   bltddat;
    UUINT16   dmaconr;
    UUINT16   vposr;
    UUINT16   vhposr;
    UUINT16   dskdatr;
    UUINT16   joy0dat;
    UUINT16   joy1dat;
    UUINT16   clxdat;
    UUINT16   adkconr;
    UUINT16   pot0dat;
    UUINT16   pot1dat;
    UUINT16   potinp;
    UUINT16   serdatr;
    UUINT16   dskbytr;
    UUINT16   intenar;
    UUINT16   intreqr;
    APTR    dskpt;
    UUINT16   dsklen;
    UUINT16   dskdat;
    UUINT16   refptr;
    UUINT16   vposw;
    UUINT16   vhposw;
    UUINT16   copcon;
    UUINT16   serdat;
    UUINT16   serper;
    UUINT16   potgo;
    UUINT16   joytest;
    UUINT16   strequ;
    UUINT16   strvbl;
    UUINT16   strhor;
    UUINT16   strlong;
    UUINT16   bltcon0;
    UUINT16   bltcon1;
    UUINT16   bltafwm;
    UUINT16   bltalwm;
    APTR    bltcpt;
    APTR    bltbpt;
    APTR    bltapt;
    APTR    bltdpt;
    UUINT16   bltsize;
    UBYTE   pad2d;
    UBYTE   bltcon0l;	// low 8 bits of bltcon0, write only
    UUINT16   bltsizv;
    UUINT16   bltsizh;	// 5e
    UUINT16   bltcmod;
    UUINT16   bltbmod;
    UUINT16   bltamod;
    UUINT16   bltdmod;
    UUINT16   pad34[4];
    UUINT16   bltcdat;
    UUINT16   bltbdat;
    UUINT16   bltadat;
    UUINT16   pad3b[3];
    UUINT16   deniseid;	// 7c
    UUINT16   dsksync;
    ULONG   cop1lc;
    ULONG   cop2lc;
    UUINT16   copjmp1;
    UUINT16   copjmp2;
    UUINT16   copins;
    UUINT16   diwstrt;
    UUINT16   diwstop;
    UUINT16   ddfstrt;
    UUINT16   ddfstop;
    UUINT16   dmacon;
    UUINT16   clxcon;
    UUINT16   intena;
    UUINT16   intreq;
    UUINT16   adkcon;
    struct  AudChannel {
      UUINT16 *ac_ptr; // ptr to start of waveform data
      UUINT16 ac_len;	// length of waveform in words
      UUINT16 ac_per;	// sample period
      UUINT16 ac_vol;	// volume
      UUINT16 ac_dat;	// sample pair
      UUINT16 ac_pad[2];	// unused
    } aud[4];
    APTR    bplpt[8];
    UUINT16   bplcon0;
    UUINT16   bplcon1;
    UUINT16   bplcon2;
    UUINT16   bplcon3;
    UUINT16   bpl1mod;
    UUINT16   bpl2mod;
    UUINT16   bplcon4;
    UUINT16   clxcon2;
    UUINT16   bpldat[8];
    APTR    sprpt[8];
    struct  SpriteDef {
      UUINT16 pos;
      UUINT16 ctl;
      UUINT16 dataa;
      UUINT16 datab;
    } spr[8];
    UUINT16   color[32];
    UUINT16 htotal;
    UUINT16 hsstop;
    UUINT16 hbstrt;
    UUINT16 hbstop;
    UUINT16 vtotal;
    UUINT16 vsstop;
    UUINT16 vbstrt;
    UUINT16 vbstop;
    UUINT16 sprhstrt;
    UUINT16 sprhstop;
    UUINT16 bplhstrt;
    UUINT16 bplhstop;
    UUINT16 hhposw;
    UUINT16 hhposr;
    UUINT16 beamcon0;
    UUINT16 hsstrt;
    UUINT16 vsstrt;
    UUINT16 hcenter;
    UUINT16 diwhigh;	// 1e4
    UUINT16 padf3[11];
    UUINT16 fmode;
};

#ifdef ECS_SPECIFIC

// defines for beamcon register
#define VARVBLANK	0x1000	// Variable vertical blank enable
#define LOLDIS		0x0800	// long line disable
#define CSCBLANKEN	0x0400	// redirect composite sync
#define VARVSYNC	0x0200	// Variable vertical sync enable
#define VARHSYNC	0x0100	// Variable horizontal sync enable
#define VARBEAM	0x0080	// variable beam counter enable
#define DISPLAYDUAL	0x0040	// use UHRES pointer and standard pointers
#define DISPLAYPAL	0x0020	// set decodes to generate PAL display
#define VARCSYNC	0x0010	// Variable composite sync enable
#define CSBLANK	0x0008	// Composite blank out to CSY* pin
#define CSYNCtrue	0x0004	// composite sync true signal
#define VSYNCtrue	0x0002	// vertical sync true
#define HSYNCtrue	0x0001	// horizontal sync true

// new defines for bplcon0
#define USE_BPLCON3	1

// new defines for bplcon2
#define BPLCON2_ZDCTEN		(1<<10) // colormapped genlock bit
#define BPLCON2_ZDBPEN		(1<<11) // use bitplane as genlock bits
#define BPLCON2_ZDBPSEL0	(1<<12) // three bits to select one
#define BPLCON2_ZDBPSEL1	(1<<13) // of 8 bitplanes in
#define BPLCON2_ZDBPSEL2	(1<<14) // ZDBPEN genlock mode

// defines for bplcon3 register
#define BPLCON3_EXTBLNKEN	(1<<0)	// external blank enable
#define BPLCON3_EXTBLKZD	(1<<1)	// external blank ored into trnsprncy
#define BPLCON3_ZDCLKEN	(1<<2)	// zd pin outputs a 14mhz clock
#define BPLCON3_BRDNTRAN	(1<<4)	// border is opaque
#define BPLCON3_BRDNBLNK	(1<<5)	// border is opaque

#endif	// ECS_SPECIFIC


#define  INTB_SETCLR	(15)  // Set/Clear control bit. Determines if bits
	    // written with a 1 get set or cleared. Bits
	    // written with a zero are allways unchanged
#define  INTB_INTEN	(14)  // Master interrupt (enable only )
#define  INTB_EXTER	(13)  // External interrupt
#define  INTB_DSKSYNC	(12)  // Disk re-SYNChronized
#define  INTB_RBF	(11)  // serial port Receive Buffer Full
#define  INTB_AUD3	(10)  // Audio channel 3 block finished
#define  INTB_AUD2	(9)   // Audio channel 2 block finished
#define  INTB_AUD1	(8)   // Audio channel 1 block finished
#define  INTB_AUD0	(7)   // Audio channel 0 block finished
#define  INTB_BLIT	(6)   // Blitter finished
#define  INTB_VERTB	(5)   // start of Vertical Blank
#define  INTB_COPER	(4)   // Coprocessor
#define  INTB_PORTS	(3)   // I/O Ports and timers
#define  INTB_SOFTINT	(2)   // software interrupt request
#define  INTB_DSKBLK	(1)   // Disk Block done
#define  INTB_TBE	(0)   // serial port Transmit Buffer Empty

// write definitions for dmaconw
#define DMAF_SETCLR  0x8000
#define DMAF_AUDIO   0x000F   // 4 bit mask
#define DMAF_AUD0    0x0001
#define DMAF_AUD1    0x0002
#define DMAF_AUD2    0x0004
#define DMAF_AUD3    0x0008
#define DMAF_DISK    0x0010
#define DMAF_SPRITE  0x0020
#define DMAF_BLITTER 0x0040
#define DMAF_COPPER  0x0080
#define DMAF_RASTER  0x0100
#define DMAF_MASTER  0x0200
#define DMAF_BLITHOG 0x0400
#define DMAF_ALL     0x01FF   // all dma channels

// read definitions for dmaconr
// bits 0-8 correspnd to dmaconw definitions
#define DMAF_BLTDONE 0x4000
#define DMAF_BLTNZERO	0x2000

//
 * ciaa is on an ODD address (e.g. the low byte) -- $bfe001
 * ciab is on an EVEN address (e.g. the high byte) -- $bfd000
 *
 * do this to get the definitions:
 *    extern struct CIA ciaa, ciab;



struct CIA {
    UBYTE   ciapra;
    UBYTE   pad0[0xff];
    UBYTE   ciaprb;
    UBYTE   pad1[0xff];
    UBYTE   ciaddra;
    UBYTE   pad2[0xff];
    UBYTE   ciaddrb;
    UBYTE   pad3[0xff];
    UBYTE   ciatalo;
    UBYTE   pad4[0xff];
    UBYTE   ciatahi;
    UBYTE   pad5[0xff];
    UBYTE   ciatblo;
    UBYTE   pad6[0xff];
    UBYTE   ciatbhi;
    UBYTE   pad7[0xff];
    UBYTE   ciatodlow;
    UBYTE   pad8[0xff];
    UBYTE   ciatodmid;
    UBYTE   pad9[0xff];
    UBYTE   ciatodhi;
    UBYTE   pad10[0xff];
    UBYTE   unusedreg;
    UBYTE   pad11[0xff];
    UBYTE   ciasdr;
    UBYTE   pad12[0xff];
    UBYTE   ciaicr;
    UBYTE   pad13[0xff];
    UBYTE   ciacra;
    UBYTE   pad14[0xff];
    UBYTE   ciacrb;
};


// interrupt control register bit numbers
#define CIAICRB_TA	0
#define CIAICRB_TB	1
#define CIAICRB_ALRM	2
#define CIAICRB_SP	3
#define CIAICRB_FLG	4
#define CIAICRB_IR	7
#define CIAICRB_SETCLR	7

// control register A bit numbers
#define CIACRAB_START	0
#define CIACRAB_PBON	1
#define CIACRAB_OUTMODE 2
#define CIACRAB_RUNMODE 3
#define CIACRAB_LOAD	4
#define CIACRAB_INMODE	5
#define CIACRAB_SPMODE	6
#define CIACRAB_TODIN	7

// control register B bit numbers
#define CIACRBB_START	0
#define CIACRBB_PBON	1
#define CIACRBB_OUTMODE 2
#define CIACRBB_RUNMODE 3
#define CIACRBB_LOAD	4
#define CIACRBB_INMODE0 5
#define CIACRBB_INMODE1 6
#define CIACRBB_ALARM	7

// interrupt control register masks
#define CIAICRF_TA	(1L<<CIAICRB_TA)
#define CIAICRF_TB	(1L<<CIAICRB_TB)
#define CIAICRF_ALRM	(1L<<CIAICRB_ALRM)
#define CIAICRF_SP	(1L<<CIAICRB_SP)
#define CIAICRF_FLG	(1L<<CIAICRB_FLG)
#define CIAICRF_IR	(1L<<CIAICRB_IR)
#define CIAICRF_SETCLR	(1L<<CIAICRB_SETCLR)

// control register A register masks
#define CIACRAF_START	(1L<<CIACRAB_START)
#define CIACRAF_PBON	(1L<<CIACRAB_PBON)
#define CIACRAF_OUTMODE (1L<<CIACRAB_OUTMODE)
#define CIACRAF_RUNMODE (1L<<CIACRAB_RUNMODE)
#define CIACRAF_LOAD	(1L<<CIACRAB_LOAD)
#define CIACRAF_INMODE	(1L<<CIACRAB_INMODE)
#define CIACRAF_SPMODE	(1L<<CIACRAB_SPMODE)
#define CIACRAF_TODIN	(1L<<CIACRAB_TODIN)

// control register B register masks
#define CIACRBF_START	(1L<<CIACRBB_START)
#define CIACRBF_PBON	(1L<<CIACRBB_PBON)
#define CIACRBF_OUTMODE (1L<<CIACRBB_OUTMODE)
#define CIACRBF_RUNMODE (1L<<CIACRBB_RUNMODE)
#define CIACRBF_LOAD	(1L<<CIACRBB_LOAD)
#define CIACRBF_INMODE0 (1L<<CIACRBB_INMODE0)
#define CIACRBF_INMODE1 (1L<<CIACRBB_INMODE1)
#define CIACRBF_ALARM	(1L<<CIACRBB_ALARM)

// control register B INMODE masks
#define CIACRBF_IN_PHI2 0
#define CIACRBF_IN_CNT	(CIACRBF_INMODE0)
#define CIACRBF_IN_TA	(CIACRBF_INMODE1)
#define CIACRBF_IN_CNT_TA  (CIACRBF_INMODE0|CIACRBF_INMODE1)

//
 * Port definitions -- what each bit in a cia peripheral register is tied to


// ciaa port A (0xbfe001)
#define CIAB_GAMEPORT1	(7)   // gameport 1, pin 6 (fire button*)
#define CIAB_GAMEPORT0	(6)   // gameport 0, pin 6 (fire button*)
#define CIAB_DSKRDY	(5)   // disk ready*
#define CIAB_DSKTRACK0	(4)   // disk on track 00*
#define CIAB_DSKPROT	(3)   // disk write protect*
#define CIAB_DSKCHANGE	(2)   // disk change*
#define CIAB_LED	(1)   // led light control (0==>bright)
#define CIAB_OVERLAY	(0)   // memory overlay bit

// ciaa port B (0xbfe101) -- parallel port

// ciab port A (0xbfd000) -- serial and printer control
#define CIAB_COMDTR	(7)   // serial Data Terminal Ready*
#define CIAB_COMRTS	(6)   // serial Request to Send*
#define CIAB_COMCD	(5)   // serial Carrier Detect*
#define CIAB_COMCTS	(4)   // serial Clear to Send*
#define CIAB_COMDSR	(3)   // serial Data Set Ready*
#define CIAB_PRTRSEL	(2)   // printer SELECT
#define CIAB_PRTRPOUT	(1)   // printer paper out
#define CIAB_PRTRBUSY	(0)   // printer busy

// ciab port B (0xbfd100) -- disk control
#define CIAB_DSKMOTOR	(7)   // disk motorr*
#define CIAB_DSKSEL3	(6)   // disk select unit 3*
#define CIAB_DSKSEL2	(5)   // disk select unit 2*
#define CIAB_DSKSEL1	(4)   // disk select unit 1*
#define CIAB_DSKSEL0	(3)   // disk select unit 0*
#define CIAB_DSKSIDE	(2)   // disk side select*
#define CIAB_DSKDIREC	(1)   // disk direction of seek*
#define CIAB_DSKSTEP	(0)   // disk step heads*

// ciaa port A (0xbfe001)
#define CIAF_GAMEPORT1	(1L<<7)
#define CIAF_GAMEPORT0	(1L<<6)
#define CIAF_DSKRDY	(1L<<5)
#define CIAF_DSKTRACK0	(1L<<4)
#define CIAF_DSKPROT	(1L<<3)
#define CIAF_DSKCHANGE	(1L<<2)
#define CIAF_LED	(1L<<1)
#define CIAF_OVERLAY	(1L<<0)

// ciaa port B (0xbfe101) -- parallel port

// ciab port A (0xbfd000) -- serial and printer control
#define CIAF_COMDTR	(1L<<7)
#define CIAF_COMRTS	(1L<<6)
#define CIAF_COMCD	(1L<<5)
#define CIAF_COMCTS	(1L<<4)
#define CIAF_COMDSR	(1L<<3)
#define CIAF_PRTRSEL	(1L<<2)
#define CIAF_PRTRPOUT	(1L<<1)
#define CIAF_PRTRBUSY	(1L<<0)

// ciab port B (0xbfd100) -- disk control
#define CIAF_DSKMOTOR	(1L<<7)
#define CIAF_DSKSEL3	(1L<<6)
#define CIAF_DSKSEL2	(1L<<5)
#define CIAF_DSKSEL1	(1L<<4)
#define CIAF_DSKSEL0	(1L<<3)
#define CIAF_DSKSIDE	(1L<<2)
#define CIAF_DSKDIREC	(1L<<1)
#define CIAF_DSKSTEP	(1L<<0)

#define HSIZEBITS 6
#define VSIZEBITS 16-HSIZEBITS
#define HSIZEMASK 0x3f	      // 2^6 -- 1
#define VSIZEMASK 0x3FF       // 2^10 - 1

// all agnii support horizontal blit of at least 1024 bits (128 bytes) wide
// some agnii support horizontal blit of up to 32768 bits (4096 bytes) wide

#ifndef	 NO_BIG_BLITS
#define  MINBYTESPERROW 128
#define  MAXBYTESPERROW 4096
#else
#define  MAXBYTESPERROW 128
#endif

// definitions for blitter control register 0

#define ABC    0x80
#define ABNC   0x40
#define ANBC   0x20
#define ANBNC  0x10
#define NABC   0x8
#define NABNC  0x4
#define NANBC  0x2
#define NANBNC 0x1

// some commonly used operations
#define A_OR_B	  ABC|ANBC|NABC | ABNC|ANBNC|NABNC
#define A_OR_C	  ABC|NABC|ABNC | ANBC|NANBC|ANBNC
#define A_XOR_C   NABC|ABNC   | NANBC|ANBNC
#define A_TO_D	  ABC|ANBC|ABNC|ANBNC

#define BC0B_DEST 8
#define BC0B_SRCC 9
#define BC0B_SRCB   10
#define BC0B_SRCA 11
#define BC0F_DEST 0x100
#define BC0F_SRCC 0x200
#define BC0F_SRCB 0x400
#define BC0F_SRCA 0x800

#define BC1F_DESC   2	      // blitter descend direction

#define DEST 0x100
#define SRCC 0x200
#define SRCB 0x400
#define SRCA 0x800

#define ASHIFTSHIFT  12       // bits to right align ashift value
#define BSHIFTSHIFT  12       // bits to right align bshift value

// definations for blitter control register 1
#define LINEMODE     0x1
#define FILL_OR      0x8
#define FILL_XOR     0x10
#define FILL_CARRYIN 0x4
#define ONEDOT	     0x2      // one dot per horizontal line
#define OVFLAG	     0x20
#define SIGNFLAG     0x40
#define BLITREVERSE  0x2

#define SUD	     0x10
#define SUL	     0x8
#define AUL	     0x4

#define OCTANT8   24
#define OCTANT7   4
#define OCTANT6   12
#define OCTANT5   28
#define OCTANT4   20
#define OCTANT3   8
#define OCTANT2   0
#define OCTANT1   16

// stuff for blit qeuer
struct bltnode
{
    struct  bltnode *n;
    int     (*function)();
    char    stat;
    short   blitsize;
    short   beamsync;
    int     (*cleanup)();
};

// defined bits for bltstat
#define CLEANUP 0x40
#define CLEANME CLEANUP
#define  ADKB_SETCLR	15 // standard set/clear bit
#define  ADKB_PRECOMP1	14 // two bits of precompensation
#define  ADKB_PRECOMP0	13
#define  ADKB_MFMPREC	12 // use mfm style precompensation
#define  ADKB_UARTBRK	11 // force uart output to zero
#define  ADKB_UINT16SYNC	10 // enable DSKSYNC register matching
#define  ADKB_MSBSYNC	9  // (Apple GCR Only) sync on MSB for reading
#define  ADKB_FAST	8  // 1 -> 2 us/bit (mfm), 2 -> 4 us/bit (gcr)
#define  ADKB_USE3PN	7  // use aud chan 3 to modulate period of ??
#define  ADKB_USE2P3	6  // use aud chan 2 to modulate period of 3
#define  ADKB_USE1P2	5  // use aud chan 1 to modulate period of 2
#define  ADKB_USE0P1	4  // use aud chan 0 to modulate period of 1
#define  ADKB_USE3VN	3  // use aud chan 3 to modulate volume of ??
#define  ADKB_USE2V3	2  // use aud chan 2 to modulate volume of 3
#define  ADKB_USE1V2	1  // use aud chan 1 to modulate volume of 2
#define  ADKB_USE0V1	0  // use aud chan 0 to modulate volume of 1

#define  ADKF_SETCLR	(1L<<15)
#define  ADKF_PRECOMP1	(1L<<14)
#define  ADKF_PRECOMP0	(1L<<13)
#define  ADKF_MFMPREC	(1L<<12)
#define  ADKF_UARTBRK	(1L<<11)
#define  ADKF_UINT16SYNC	(1L<<10)
#define  ADKF_MSBSYNC	(1L<<9)
#define  ADKF_FAST	(1L<<8)
#define  ADKF_USE3PN	(1L<<7)
#define  ADKF_USE2P3	(1L<<6)
#define  ADKF_USE1P2	(1L<<5)
#define  ADKF_USE0P1	(1L<<4)
#define  ADKF_USE3VN	(1L<<3)
#define  ADKF_USE2V3	(1L<<2)
#define  ADKF_USE1V2	(1L<<1)
#define  ADKF_USE0V1	(1L<<0)

#define ADKF_PRE000NS	0			// 000 ns of precomp
#define ADKF_PRE140NS	(ADKF_PRECOMP0)	// 140 ns of precomp
#define ADKF_PRE280NS	(ADKF_PRECOMP1)	// 280 ns of precomp
#define ADKF_PRE560NS	(ADKF_PRECOMP0|ADKF_PRECOMP1) // 560 ns of precomp


*/


/*
struct NAmigaNode {
  NAmigaNode *ln_Succ;
  NAmigaNode *ln_Pred;
  BYTE ln_Type;
  BYTE ln_Pri;
  char *ln_Name;
};


struct NAmigaMinNode {
  NAmigaMinNode *mln_Succ;
  NAmigaMinNode *mln_Pred;
};


struct NAmigaList {
  NAmigaNode *lh_Head;
  NAmigaNode *lh_Tail;
  NAmigaNode *lh_TailPred;
  BYTE lh_Type;
  BYTE l_pad;
};


struct NAmigaMinList {
  NAmigaMinNode *mlh_Head;
  NAmigaMinNode *mlh_Tail;
  NAmigaMinNode *mlh_TailPred;
};


struct NAmigaLibrary {
  NAmigaNode lib_Node;
  BYTE lib_Flags;
  BYTE lib_pad;
  UINT16 lib_NegSize;
  UINT16 lib_PosSize;
  UINT16 lib_Version;
  UINT16 lib_Revision;
  BYTE *lib_IdString;
  ULONG lib_Sum;
  UINT16 lib_OpenCnt;
};


struct NAmigaIntVector {
  BYTE *iv_Data;
  void (*iv_Code)();
  NAmigaNode *iv_Node;
};


struct NAmigaSoftIntList {
  NAmigaList sh_List;
  UINT16 sh_Pad;
};


struct NAmigaTask {
  NAmigaNode tc_Node;
  BYTE tc_Flags;
  BYTE tc_State;
  BYTE tc_IDNestCnt;
  BYTE tc_TDNestCnt;
  ULONG tc_SigAlloc;
  ULONG tc_SigWait;
  ULONG tc_SigRecvd;
  ULONG tc_SigExcept;
  UINT16 tc_TrapAlloc;
  UINT16 tc_TrapAble;
  BYTE *tc_ExceptData;
  BYTE *tc_ExceptCode;
  BYTE *tc_TrapData;
  BYTE *tc_TrapCode;
  BYTE *tc_SPReg;
  BYTE *tc_SPLower;
  BYTE *tc_SPUpper;
  void (*tc_Switch)();
  void (*tc_Launch)();
  NAmigaList tc_MemEntry;
  BYTE *tc_UserData;
};


struct NAmigaExecBase {
	NAmigaLibrary LibNode;
	UINT16 SoftVer;
	UINT16 LowMemChkSum;
	ULONG ChkBase;
	BYTE *ColdCapture;
	BYTE *CoolCapture;
	BYTE dBugarmCapture;
	BYTE *SysStkUpper;
	BYTE *SysStkLower;
	ULONG MaxLocMem;
	BYTE *DebugEntry;
	BYTE *DebugData;
	BYTE *AlertData;
	BYTE *MaxExtMem;
	UINT16 ChkSum;
	NAmigaIntVector IntVects[16];
	NAmigaTask *ThisTask;
	ULONG IdleCount;
	ULONG DispCount;
	UINT16 Quantum;
	UINT16 Elapsed;
	UINT16 SysFlags;
	BYTE IDNestCnt;
	BYTE TDNestCnt;
	UINT16 AttnFlags;
	UINT16 AttnResched;
	BYTE *ResModules;
	BYTE *TaskTrapCode;
	BYTE *TaskExceptCode;
	BYTE *TaskExitCode;
	ULONG TaskSigAlloc;
	UINT16 TaskTrapAlloc;
  NAmigaList MemList;
	NAmigaList ResourceList;
	NAmigaList DeviceList;
	NAmigaList IntrList;
	NAmigaList LibList;
	NAmigaList PortList;
	NAmigaList TaskReady;
	NAmigaList TaskWait;
	NAmigaSoftIntList SoftInts[5];
	ULONG LastAlert[4];
	BYTE VBlankFrequency;
	BYTE PowerSupplyFrequency;
	NAmigaList SemaphoreList;
	BYTE *KickMemPtr;
	BYTE *KickTagPtr;
	BYTE *KickCheckSum;
	UINT16 ex_Pad0;
	ULONG ex_LaunchPoint;
	BYTE *ex_RamLibPrivate;
	ULONG ex_EClockFrequency;
	ULONG ex_CacheControl;
	ULONG ex_TaskID;
	ULONG ex_Reserved1[5];
	BYTE *ex_MMULock;
	ULONG ex_Reserved2[3];
	NAmigaMinList ex_MemHandlers;
	BYTE *ex_MemHandler;
};
*/


////////////////////////////////////////////////////////////////////////////////
//  NAmigaHandler Class
////////////////////////////////////////////////////////////////////////////////

NAmigaHandler::NAmigaHandler()
{
  DEBUG_OUT<<"NAmigaHandler constructor.\n";
  romName="amiga.rom";
  romSize=0;
}


NAmigaHandler::~NAmigaHandler()
{
  DEBUG_OUT<<"NAmigaHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


unsigned int NAmigaHandler::name2type(const char *name)
{
  std::cerr<<"NAmigaHandler name2type not implemented!\n"; exit(5);
}


/* STATIC */
bool NAmigaHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"amigados")) return true; else
  if(!strcmp(name,"amiga")) return true; else
  if(!strcmp(name,"amigaecs")) return true; else
  if(!strcmp(name,"amiga_ecs")) return true; else
  if(!strcmp(name,"amigaaga")) return true; else
  if(!strcmp(name,"amiga_aga")) return true; else
  if(!strcmp(name,"ecs")) return true; else
  if(!strcmp(name,"aga")) return true; else
  if(!strcmp(name,"amiga500")) return true; else
  if(!strcmp(name,"amiga600")) return true; else
  if(!strcmp(name,"amiga1000")) return true; else
  if(!strcmp(name,"amiga1500")) return true; else
  if(!strcmp(name,"amiga2000")) return true; else
  if(!strcmp(name,"amiga2500")) return true; else
  if(!strcmp(name,"amiga3000")) return true; else
  if(!strcmp(name,"amiga3000t")) return true; else
  if(!strcmp(name,"amiga1200")) return true; else
  if(!strcmp(name,"amiga4000")) return true; else
  if(!strcmp(name,"amiga4000t")) return true; else
  if(!strcmp(name,"cdtv")) return true; else
  if(!strcmp(name,"cd32")) return true; else
  return false;
}


const char *NAmigaHandler::getName()
{
  if(io_type==N_NAMIGA_IO_ECS) return "amiga (ECS chipset)"; else
  if(io_type==N_NAMIGA_IO_AGA) return "amiga (AGA chipset)"; else
  return "NAmiga";
}


// Note: No else on last one...
void NAmigaHandler::setKind(const char *tname)
{
  io_type=0;
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"aga")) io_type=N_NAMIGA_IO_AGA; else
  if(!strcmp(name,"amiga_aga")) io_type=N_NAMIGA_IO_AGA; else
  if(!strcmp(name,"amigaaga")) io_type=N_NAMIGA_IO_AGA; else
  if(!strcmp(name,"ecs")) io_type=N_NAMIGA_IO_ECS; else
  if(!strcmp(name,"amiga_ecs")) io_type=N_NAMIGA_IO_ECS; else
  if(!strcmp(name,"amigaecs")) io_type=N_NAMIGA_IO_ECS; else
  if(!strcmp(name,"amiga")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amigados")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga500")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga600")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga1000")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga1500")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga2000")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga2500")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga3000")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga3000t")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"amiga1200")) io_type=N_NAMIGA_IO_AGA;
  if(!strcmp(name,"amiga4000")) io_type=N_NAMIGA_IO_AGA;
  if(!strcmp(name,"amiga4000t")) io_type=N_NAMIGA_IO_AGA;
  if(!strcmp(name,"cdtv")) io_type=N_NAMIGA_IO_ECS;
  if(!strcmp(name,"cd32")) io_type=N_NAMIGA_IO_AGA;
  io_init(io_type);
}


void NAmigaHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_NAMIGA_IO_ECS:
    case N_NAMIGA_IO_AGA:
      myIO=new NAmigaIO(this);
      myVideo=new EVideoIOBitmap(this,20,20);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_NAMIGA_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  resetStack=0x200000-256;
  romOverlay=true;
}


void NAmigaHandler::my_reset()
{
  Em68kHandler::my_reset();
  addRomTagSymbols(theRom);
  if(myIO) myIO->write8(0xbfe001,0xff);  // map rom at 0, led dim...
}


void NAmigaHandler::my_reset_postCPU()
{
  if(cpu) {
    if(!cpu->getTrapHandler()) {
      NTinyHandler *foo=new NTinyHandler(this);
      if(foo) cpu->setTrapHandler(foo);
    }
  }
}


void NAmigaHandler::addRomTagSymbols(ERom *rom)
{
  if(!rom) { std::cerr<<"Can't add symbols, no rom!\n"; return; }
  if(!romSize) { std::cerr<<"Can't add symbols, no rom!\n"; return; }
  if(!map) { std::cerr<<"Can't add symbols, no map!\n"; return; }
  if(map->lookupSymbolByName("exec.library")) {
    // already added the symbols
    return;
  }
  DEBUG_OUT<<"NAmigaHandler::addRomTagSymbols()\n";
  ULONG base=rom->getLocation(),size=rom->getSize();
  DEBUG_OUT<<"Rom is at "<<base<<"\n";
  AList *syms=map->getSymbols();
  char s[256];
  for(ULONG t=0;t<size;t+=2) {
    unsigned int offset=0;
    USHORT matchWord=rom->read16b(base+t);  offset+=2;
    if(matchWord==0x4afc) {
      ULONG matchTag=rom->read32b(base+t+offset);  offset+=4;
      if(matchTag==(base+t)) {
        ULONG endSkip=rom->read32b(base+t+offset);  offset+=4;
        unsigned int flags=rom->read8(base+t+offset);  offset++;
        unsigned int version=rom->read8(base+t+offset);  offset++;
        unsigned int type=rom->read8(base+t+offset);  offset++;
        unsigned int pri=rom->read8(base+t+offset);  offset++;
        ULONG namePtr=rom->read32b(base+t+offset);  offset+=4;
        ULONG idString=rom->read32b(base+t+offset);  offset+=4;
        ULONG init=rom->read32b(base+t+offset);  offset+=4;
        char c=1;
        unsigned int i=0;
        while(c) {
          c=rom->read8(namePtr+i);
          s[i]=c;
          i++;
        }
        DEBUG_OUT<<"\""<<s<<"\" addr="<<base+t;
        DEBUG_OUT<<" flags="<<flags<<" vers="<<version<<" type="<<type<<" pri="<<pri<<"\n";
        if(syms) {
          EMapSymbol *e=new EMapSymbol(s,0,base+t,endSkip-base+t,EMAP_SYMBOL_BINNODE);
          syms->append(e);
        }
      }
    }
  }
}


bool NAmigaHandler::read8ret(ULONG addr,BYTE *ret)
{
  if(Em68kHandler::read8ret(addr,ret)) return true;
  unsigned int apage=(addr>>16);
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}


bool NAmigaHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  if(Em68kHandler::write8ret(addr,val,ret)) return true;
  unsigned int apage=(addr>>16);
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}
