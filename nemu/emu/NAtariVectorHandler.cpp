
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

Code for running old Atari vector based arcade games such as Asteroids

*/


#include <emu/NAtariVectorHandler.h>
#include <emu/NAtariVectorIO.h>
#include <emu/NAtariVectorVideo.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NAtariVectorHandler Class
////////////////////////////////////////////////////////////////////////////////

NAtariVectorHandler::NAtariVectorHandler()
{
  DEBUG_OUT<<"NAtariVectorHandler constructor.\n";
}


NAtariVectorHandler::~NAtariVectorHandler()
{
  DEBUG_OUT<<"NAtariVectorHandler destructor.\n";
}


/* STATIC */
unsigned int NAtariVectorHandler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"lunarlander")) return N_ATARIVEC_IO_LUNARLANDER;
  if(!strcmp(name,"battlezone")) return N_ATARIVEC_IO_BATTLEZONE;
  if(!strcmp(name,"asteroids")) return N_ATARIVEC_IO_ASTEROIDS;
  if(!strcmp(name,"redbaron")) return N_ATARIVEC_IO_REDBARON;
  if(!strcmp(name,"blackwidow")) return N_ATARIVEC_IO_BLACKWIDOW;
  if(!strcmp(name,"tempest")) return N_ATARIVEC_IO_TEMPEST;
  if(!strcmp(name,"majorhavoc")) return N_ATARIVEC_IO_MAJORHAVOC;
  return N_ATARIVEC_IO_NONE;
}


/* STATIC */
bool NAtariVectorHandler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


const char *NAtariVectorHandler::getName()
{
  if(io_type==N_ATARIVEC_IO_LUNARLANDER) return "Arcade Lunar Lander"; else
  if(io_type==N_ATARIVEC_IO_BATTLEZONE) return "Arcade Battlezone"; else
  if(io_type==N_ATARIVEC_IO_ASTEROIDS) return "Arcade Asteroids"; else
  if(io_type==N_ATARIVEC_IO_REDBARON) return "Arcade Red Baron"; else
  if(io_type==N_ATARIVEC_IO_BLACKWIDOW) return "Arcade Black Widow"; else
  if(io_type==N_ATARIVEC_IO_TEMPEST) return "Arcade Tempest"; else
  if(io_type==N_ATARIVEC_IO_MAJORHAVOC) return "Arcade Major Havoc"; else
  return "Atari Vector Arcade";
}


void NAtariVectorHandler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


void NAtariVectorHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_ATARIVEC_IO_LUNARLANDER:
    case N_ATARIVEC_IO_BATTLEZONE:
    case N_ATARIVEC_IO_ASTEROIDS:
    case N_ATARIVEC_IO_REDBARON:
    case N_ATARIVEC_IO_BLACKWIDOW:
    case N_ATARIVEC_IO_TEMPEST:
    case N_ATARIVEC_IO_MAJORHAVOC:
      myIO=new NAtariVectorIO(this);
      myVideo=new NAtariVectorVideo(this,20,20);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_ATARIVEC_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
}


bool NAtariVectorHandler::read8ret(ULONG addr,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}


bool NAtariVectorHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}
