
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NFamicomHandler.h>
#include <emu/NFamicomIO.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em6502.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EVideoIOMultiMode.h>


/*
Notes:

memory map
0000-1fff 2k ram, mirrored 4 times
2000-4fff I/O
5000-5fff Expansion modules
6000-7fff cart ram, may be batt backed up
8000-bfff lower bank of cart
c000-ffff upper bank of cart

iNES (,nes) file format:
0-2 'NES'
3 $1a
4 16k prg-rom page count
5 8k chr-rom page count
6 rom control byte #1
7 rom control byte #2
8-15 $00
16-... Actual 16k prg-rom pages in linear order...
(if trainer exists, it precedes the first prg-rom page)
...-EOF chr-rom pages in ascending order

*/


////////////////////////////////////////////////////////////////////////////////
//  NFamicomHandler Class
////////////////////////////////////////////////////////////////////////////////

NFamicomHandler::NFamicomHandler()
{
  DEBUG_OUT<<"NFamicomHandler constructor.\n";
  theRam=NULL;
}


NFamicomHandler::~NFamicomHandler()
{
  DEBUG_OUT<<"NFamicomHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


void NFamicomHandler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


const char *NFamicomHandler::getName()
{
  if(io_type==N_FAMICOM_IO_FAMICOM) return "Nintendo Famicom/NES";
  return "Unknown Famicom";
}


/* STATIC */
bool NFamicomHandler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


/* STATIC */
unsigned int NFamicomHandler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"nintendo")) return N_FAMICOM_IO_FAMICOM;
  if(!strcmp(name,"famicom")) return N_FAMICOM_IO_FAMICOM;
  if(!strcmp(name,"nes")) return N_FAMICOM_IO_FAMICOM;
  return N_FAMICOM_IO_NONE;
}


void NFamicomHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_FAMICOM_IO_FAMICOM:
      myIO=new NFamicomIO(this);
      myVideo=new EVideoIOMultiMode(this,20,20);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_FAMICOM_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
}


bool NFamicomHandler::read8ret(ULONG addr,BYTE *ret)
{
  unsigned int bpage=(addr>>12);
  unsigned int actual=0;
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(bpage) {
    case 0x0: case 0x1: case 0x2:
      if(!theRam) theRam=(ERam *)map->findArea("ram");
      actual=addr&0x7ff;
      DEBUG_OUT<<"NFamicomHandler::read8ret mirroring "<<addr<<" to "<<actual<<"\n";
      *ret=theRam->read8(actual);
      break;
    default:
      return false;
      break;
  }
  return false;
}


bool NFamicomHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  unsigned int bpage=(addr>>12);
  unsigned int actual=0;
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(bpage) {
    case 0x0: case 0x1: case 0x2:
      if(!theRam) theRam=(ERam *)map->findArea("ram");
      actual=addr&0x7ff;
      DEBUG_OUT<<"NFamicomHandler::write8ret mirroring "<<addr<<" to "<<actual<<"\n";
      *ret=theRam->write8(actual,val);
      break;
    default:
      return false;
      break;
  }
  return false;
}


bool NFamicomHandler::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool done=false;
  if(!strcmp("nescart",cmd)) { nescart(a1,a2,a3); done=true; }
  return done;
}


void NFamicomHandler::nescart(const char *a1,const char *a2,const char *a3)
{
  if(!a1) { DEBUG_OUT<<"Missing param!\n"; return; }
  DEBUG_OUT<<"nescart("<<a1<<")\n";
  FILE *fd=(FILE *)fopen(a1,"rb");
  if(fd) {
    char sig[4];
    fread(sig,3,1,fd);
    sig[3]=0;
    if(!strcmp(sig,"NES")) {
      char c;
      fread(&c,1,1,fd);
      if(c==0x1a) {
        char prgCount=0;
        fread(&prgCount,1,1,fd);
        DEBUG_OUT<<"16k prg-rom page count is "<<(int)prgCount<<"\n";
        char chrCount=0;
        fread(&chrCount,1,1,fd);
        DEBUG_OUT<<"8k chr-rom page count is "<<(int)chrCount<<"\n";
        fread(&c,1,1,fd);
        DEBUG_OUT<<"rom control #1 is "<<(int)c<<"\n";
        fread(&c,1,1,fd);
        DEBUG_OUT<<"rom control #2 is "<<(int)c<<"\n";
        unsigned int t=0;
        for(t=0;t<8;t++) { fread(&c,1,1,fd); }  // skip some padding zeros
        char *tmem=NULL;
        ERom *rom=NULL;
        unsigned int laddr=0xc000;
        for(t=0;t<prgCount;t++) {
          DEBUG_OUT<<"Reading prg-rom #"<<t<<"...\n";
          tmem=(char *)malloc(16*1024);
          fread(tmem,16*1024,1,fd);
          rom=new ERom("prg.rom",laddr,(BYTE *)tmem,16*1024);
          if(rom) {
            if(!rom->err.error()) {
              DEBUG_OUT<<"Read successfully, going to add to map...\n";
              map->addArea(rom);
            } else { DEBUG_OUT<<"ERom had an error!\n"; exit(5); }
          } else { DEBUG_OUT<<"Didn't get ERom!\n"; exit(5); }
          free(tmem);
          laddr-=16384;
        }
        laddr=0;
        for(t=0;t<chrCount;t++) {
          DEBUG_OUT<<"Reading chr-rom #"<<t<<"...\n";
          tmem=(char *)malloc(8*1024);
          fread(tmem,8*1024,1,fd);
          rom=new ERom("chr.rom",laddr,(BYTE *)tmem,8*1024);
          if(rom) {
            if(!rom->err.error()) {
              DEBUG_OUT<<"Read successfully, going to add to map...\n";
              map->addArea(rom);
            } else { DEBUG_OUT<<"ERom had an error!\n"; exit(5); }
          } else { DEBUG_OUT<<"Didn't get ERom!\n"; exit(5); }
          free(tmem);
          laddr+=8192;
        }
      }
      else debugger("File didn't have $1a signature!\n");
    }
     else debugger("File didn't have NES signature!\n");
    fclose(fd);
  }
  else debugger("Couldn't open that file!\n");
/*
iNES (,nes) file format:
0-2 'NES'
3 $1a
4 16k prg-rom page count
5 8k chr-rom page count
6 rom control byte #1
7 rom control byte #2
8-15 $00
16-... Actual 16k prg-rom pages in linear order...
(if trainer exists, it precedes the first prg-rom page)
...-EOF chr-rom pages in ascending order
*/
}
