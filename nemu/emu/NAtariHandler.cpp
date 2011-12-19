
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariHandler.h>
#include <emu/NAtariIO.h>
#include <emu/NAtariVideo.h>
#include <emu/NAtariDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ESoundIOPokey.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOPia6520.h>

#include <cpulib/Em6502.h>


static ULONG NAtari_Dos[]={
  0,0,0,0
};


////////////////////////////////////////////////////////////////////////////////
//  NAtariHandler Class
////////////////////////////////////////////////////////////////////////////////

NAtariHandler::NAtariHandler()
{
  DEBUG_OUT<<"NAtariHandler constructor.\n";
  osHelp=(NAtariDos *)NULL;
  init();
}


NAtariHandler::~NAtariHandler()
{
  DEBUG_OUT<<"NAtariHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


void NAtariHandler::setKind(const char *tname)
{
  io_type=0;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"atari")) io_type=N_ATARI_IO_ATARI800; else
  if(!strcmp(name,"atarixl")) io_type=N_ATARI_IO_ATARIXL; else
  if(!strcmp(name,"atari_xl")) io_type=N_ATARI_IO_ATARIXL; else
  if(!strcmp(name,"atarixe")) io_type=N_ATARI_IO_ATARIXE; else
  if(!strcmp(name,"atari_xe")) io_type=N_ATARI_IO_ATARIXE; else
  if(!strcmp(name,"atari400")) io_type=N_ATARI_IO_ATARI800; else
  if(!strcmp(name,"atari800")) io_type=N_ATARI_IO_ATARI800; else
  if(!strcmp(name,"atari600xl")) io_type=N_ATARI_IO_ATARIXL; else
  if(!strcmp(name,"atari800xl")) io_type=N_ATARI_IO_ATARIXL; else
  if(!strcmp(name,"atari1200")) io_type=N_ATARI_IO_ATARIXL; else
  if(!strcmp(name,"atari1200xl")) io_type=N_ATARI_IO_ATARIXL; else
  if(!strcmp(name,"atari1400")) io_type=N_ATARI_IO_ATARIXL; else
  if(!strcmp(name,"atari1400xl")) io_type=N_ATARI_IO_ATARIXL; else
  if(!strcmp(name,"atari65xe")) io_type=N_ATARI_IO_ATARIXE; else
  if(!strcmp(name,"atari130xe")) io_type=N_ATARI_IO_ATARIXE; else
  if(!strcmp(name,"atari5200")) io_type=N_ATARI_IO_ATARI5200;
  if(!strcmp(name,"5200")) io_type=N_ATARI_IO_ATARI5200;
  if(!strcmp(name,"atari7800")) io_type=N_ATARI_IO_ATARI7800;
  if(!strcmp(name,"7800")) io_type=N_ATARI_IO_ATARI7800;
  if(!strcmp(name,"atari2600")) io_type=N_ATARI_IO_ATARI2600;
  if(!strcmp(name,"2600")) io_type=N_ATARI_IO_ATARI2600;
  io_init(io_type);
}


const char *NAtariHandler::getName()
{
  if(io_type==N_ATARI_IO_ATARI2600) return "Atari 2600"; else
  if(io_type==N_ATARI_IO_ATARI7800) return "Atari 7800"; else
  if(io_type==N_ATARI_IO_ATARI5200) return "Atari 5200"; else
  if(io_type==N_ATARI_IO_ATARI800) return "Atari 800"; else
  if(io_type==N_ATARI_IO_ATARIXL) return "Atari XL"; else
  if(io_type==N_ATARI_IO_ATARIXE) return "Atari XE"; else
  return "Unknown Atari";
}


/* STATIC */
bool NAtariHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"atari")) return true; else
  if(!strcmp(name,"atarixl")) return true; else
  if(!strcmp(name,"atari_xl")) return true; else
  if(!strcmp(name,"atarixe")) return true; else
  if(!strcmp(name,"atari_xe")) return true; else
  if(!strcmp(name,"atari400")) return true; else
  if(!strcmp(name,"atari800")) return true; else
  if(!strcmp(name,"atari600xl")) return true; else
  if(!strcmp(name,"atari800xl")) return true; else
  if(!strcmp(name,"atari1200xl")) return true; else
  if(!strcmp(name,"atari1200")) return true; else
  if(!strcmp(name,"atari1400xl")) return true; else
  if(!strcmp(name,"atari1200")) return true; else
  if(!strcmp(name,"atari65xe")) return true; else
  if(!strcmp(name,"atari130xe")) return true; else
  if(!strcmp(name,"atari5200")) return true; else
  if(!strcmp(name,"5200")) return true; else
  if(!strcmp(name,"atari7800")) return true; else
  if(!strcmp(name,"7800")) return true; else
  if(!strcmp(name,"atari2600")) return true; else
  if(!strcmp(name,"2600")) return true; else
  return false;
}


void NAtariHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_ATARI_IO_ATARI5200:
      myVideo=new NAtariVideo(this,40,24);
      if(myVideo) {
        myVideo->setLocation(0x3914);
        myVideo->setVideoSize(38,24);
      }
      myIO=new NAtariIO(this);
      break;
    case N_ATARI_IO_ATARI800:
    case N_ATARI_IO_ATARIXL:
    case N_ATARI_IO_ATARIXE:
      myVideo=new NAtariVideo(this,40,24);
      if(myVideo) {
        myVideo->setLocation(0x9c42);
        myVideo->setVideoSize(38,24);
      }
      myIO=new NAtariIO(this);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_ATARI_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  osHelp=new NAtariDos(this);
  osHelp->patchAll(NAtari_Dos);
}


void NAtariHandler::keypress(int c)
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
  ((NAtariIO *)myIO)->pokey->setKeyCountMax();
  if(((NAtariIO *)myIO)->pokey->getIRQen()&0x40) {
    getCPU()->cause_int(E6502_INT_IRQ);
  }
}


bool NAtariHandler::read8ret(ULONG addr,BYTE *ret)
{
  return false;
}


bool NAtariHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  return false;
}
