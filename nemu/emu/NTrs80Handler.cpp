
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NTrs80Handler.h>
#include <emu/NTrs80IO.h>
#include <emu/NTrs80Dos.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EVideoIOTextOnly.h>


/*
Memory Map for Model 1

0000-0fff Level 1 rom
1000-37ff Not used
3800-380f Keyboard
3810-3bff Not used
3c00-3fff Video display
4000-41ff Ram (used by level 1 basic)
4200-7fff Ram (usable)
8000-ffff Not used
*/

/*
Memory Map for newer moedls
0000-0fff level 1 rom
1000-2fff level 2 rom
3000-37de unused
37dd-37ff disk/casette/printer i/o
3800-387f keyboard
3880-3bff "phantom keyboard"
3c00-3fff video memory
4000-ffff ram
*/

/*
i/o info

37e0 drive select
37e4 casette selete
37e8 line printer select
37ec disk controller chip
37ed track select
37ee sector select
37ef disk data address
3801 keyboard: @ a-g
3802 keyboard: h-o
3804 keyboard: p-w
3808 keyboard: x-z
3810 keyboard: 0-7
3820 keyboard: 8-9 . , / etc
3840 keyboard: enter clear soarc cursor etc
3880 keyboard: shift (only)
*/

/*
video memory
3c00-3fff 64 x 16 characters
bit 6 was originally ignored so model 1 was all uppercase
codes are standard ascii
*/


static ULONG NTrs80_Dos[]={
  0,0,0,0
};


////////////////////////////////////////////////////////////////////////////////
//  NTrs80Handler Class
////////////////////////////////////////////////////////////////////////////////

NTrs80Handler::NTrs80Handler()
{
  DEBUG_OUT<<"NTrs80Handler constructor.\n";
  osHelp=(NTrs80Dos *)NULL;
  init();
}


NTrs80Handler::~NTrs80Handler()
{
  DEBUG_OUT<<"NTrs80Handler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


void NTrs80Handler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


const char *NTrs80Handler::getName()
{
  if(io_type==N_TRS80_IO_MODEL1) return "TRS80 Model 1";
  if(io_type==N_TRS80_IO_MODEL3) return "TRS80 Model 3";
  if(io_type==N_TRS80_IO_MODEL4) return "TRS80 Model 4";
  if(io_type==N_TRS80_IO_COLOURGENIE) return "Colour Genie";
  return "Unknown TRS80";
}


/* STATIC */
bool NTrs80Handler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


/* STATIC */
unsigned int NTrs80Handler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"model1")) return N_TRS80_IO_MODEL1;
  if(!strcmp(name,"model3")) return N_TRS80_IO_MODEL3;
  if(!strcmp(name,"model4")) return N_TRS80_IO_MODEL4;
  if(!strcmp(name,"trs80")) return N_TRS80_IO_MODEL1;
  if(!strcmp(name,"d80")) return N_TRS80_IO_MODEL1;
  if(!strcmp(name,"dicksmith")) return N_TRS80_IO_MODEL1;
  if(!strcmp(name,"videogenie")) return N_TRS80_IO_MODEL1;
  if(!strcmp(name,"colorgenie")) return N_TRS80_IO_COLOURGENIE;
  if(!strcmp(name,"colourgenie")) return N_TRS80_IO_COLOURGENIE;
  return N_TRS80_IO_NONE;
}


void NTrs80Handler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_TRS80_IO_MODEL1:
      myVideo=new EVideoIOTextOnly(this,64,16);
      if(myVideo) {
        myVideo->setLocation(0x3c00);
      }
      myIO=new NTrs80IO(this);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_TRS80_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  osHelp=new NTrs80Dos(this);
  osHelp->patchAll(NTrs80_Dos);
}


void NTrs80Handler::keypress(int c)
{
  //io_keypress=code;
  switch(c) {
    case 'j':  io_keypress=0x01;  break;
    case ';':  io_keypress=0x02;  break;
    case 'l':  io_keypress=0x03;  // fake, actually 0
      break;
    case 'k':  io_keypress=0x05;  break;
    case '+':  io_keypress=0x06;  break;
    case '*':  io_keypress=0x07;  break;
    case 'o':  io_keypress=0x08;  break;
    case 'p':  io_keypress=0x0a;  break;
    case 'u':  io_keypress=0x0b;  break;
    //case A_KEY_NEWLINE:  io_keypress=0x0c;  break;
    case 'i':  io_keypress=0x0d;  break;
    case '-':  io_keypress=0x0e;  break;
    case '=':  io_keypress=0x0f;  break;
    case 'v':  io_keypress=0x10;  break;
    case 'c':  io_keypress=0x12;  break;
    case 'b':  io_keypress=0x15;  break;
    case 'x':  io_keypress=0x16;  break;
    case 'z':  io_keypress=0x17;  break;
    case '4':  io_keypress=0x18;  break;
    case '3':  io_keypress=0x1a;  break;
    case '6':  io_keypress=0x1b;  break;
    case '5':  io_keypress=0x1d;  break;
    case '2':  io_keypress=0x1e;  break;
    case '1':  io_keypress=0x1f;  break;
    case ',':  io_keypress=0x20;  break;
    case ' ':  io_keypress=0x21;  break;
    case '.':  io_keypress=0x22;  break;
    case 'n':  io_keypress=0x23;  break;
    case 'm':  io_keypress=0x25;  break;
    case '/':  io_keypress=0x26;  break;
    case '9':  io_keypress=0x30;  break;
    case '0':  io_keypress=0x32;  break;
    case '7':  io_keypress=0x33;  break;
    //case A_KEY_DELETE:  io_keypress=0x34;  break;
    case '8':  io_keypress=0x35;  break;
    case '<':  io_keypress=0x36;  break;
    case '>':  io_keypress=0x37;  break;
    case ':':  io_keypress=0x42;  break;
    case '$':  io_keypress=0x58;  break;
    case '#':  io_keypress=0x5a;  break;
    case '&':  io_keypress=0x5b;  break;
    case '%':  io_keypress=0x5d;  break;
    case '"':  io_keypress=0x5e;  break;
    case '!':  io_keypress=0x5f;  break;
    case '[':  io_keypress=0x60;  break;
    case ']':  io_keypress=0x62;  break;
    case '?':  io_keypress=0x66;  break;
    case 'r':  io_keypress=0x68;  break;
    case 'e':  io_keypress=0x6a;  break;
    case 'y':  io_keypress=0x6b;  break;
    case 't':  io_keypress=0x6d;  break;
    case 'w':  io_keypress=0x6e;  break;
    case 'q':  io_keypress=0x6f;  break;
    case '(':  io_keypress=0x70;  break;
    case ')':  io_keypress=0x72;  break;
    case '\'':  io_keypress=0x73;  break;
    case '@':  io_keypress=0x75;  break;
    case 'f':  io_keypress=0x78;  break;
    case 'h':  io_keypress=0x79;  break;
    case 'd':  io_keypress=0x7a;  break;
    case 'g':  io_keypress=0x7d;  break;
    case 's':  io_keypress=0x7e;  break;
    case 'a':  io_keypress=0x7f;  break;
    case 'L':  io_keypress=0x80;  break;
    case 'J':  io_keypress=0x81;  break;
    case 'K':  io_keypress=0x85;  break;
    //case A_KEY_LEFT:  io_keypress=0x86;  break;
    //case A_KEY_RIGHT:  io_keypress=0x87;  break;
    case 'O':  io_keypress=0x88;  break;
    case 'P':  io_keypress=0x8a;  break;
    case 'U':  io_keypress=0x8b;  break;
    case 'I':  io_keypress=0x8d;  break;
    //case A_KEY_UP:  io_keypress=0x8e;  break;
    //case A_KEY_DOWN:  io_keypress=0x8f;  break;
    case 'V':  io_keypress=0x90;  break;
    case 'C':  io_keypress=0x92;  break;
    case 'B':  io_keypress=0x95;  break;
    case 'X':  io_keypress=0x96;  break;
    case 'Z':  io_keypress=0x97;  break;
    case '`':  io_keypress=0xa0;  break;
    case 'N':  io_keypress=0xa3;  break;
    case 'M':  io_keypress=0xa5;  break;
    case 'F':  io_keypress=0xb8;  break;
    case 'H':  io_keypress=0xb9;  break;
    case 'D':  io_keypress=0xba;  break;
    case 'G':  io_keypress=0xbd;  break;
    case 'S':  io_keypress=0xbe;  break;
    case 'A':  io_keypress=0xbf;  break;
    case 'R':  io_keypress=0xa8;  break;
    case 'E':  io_keypress=0xaa;  break;
    case 'Y':  io_keypress=0xab;  break;
    case 'T':  io_keypress=0xad;  break;
    case 'W':  io_keypress=0xae;  break;
    case 'Q':  io_keypress=0xaf;  break;
    //case A_KEY_UP:  DEBUG_OUT<<silly<<"\n";  io_keypress=silly; silly++;  break;
    //case A_KEY_DOWN:  DEBUG_OUT<<silly<<"\n";  io_keypress=silly; silly--;  break;
    //case A_KEY_RIGHT:  DEBUG_OUT<<silly<<"\n";  io_keypress=silly; break;
    //case A_KEY_LEFT:  DEBUG_OUT<<silly<<"\n";  silly=io_keypress; break;
    default:
      io_keypress=0;
      break;
  }
}


bool NTrs80Handler::read8ret(ULONG addr,BYTE *ret)
{
  return false;
}


bool NTrs80Handler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  return false;
}


void NTrs80Handler::io_out8(ULONG addr, BYTE val)
{
  DEBUG_OUT<<"out8 "<<addr<<","<<(int)val<<"\n";
  switch(addr) {
    case 0xff:
      // Cassette IO
      break;
    default:
      debugger("NTrs80Handler::io_out8 unimplemented address");
      break;
  }
}


BYTE NTrs80Handler::io_in8(ULONG addr)
{
  BYTE ret=0;
  DEBUG_OUT<<"in8 "<<addr<<"\n";
  switch(addr) {
    default:
      debugger("NTrs80Handler::io_in8 unimplemented address");
      break;
  }
  return ret;
}
