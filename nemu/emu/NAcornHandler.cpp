
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAcornHandler.h>
#include <emu/NAcornIO.h>
#include <emu/NAcornDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EVideoIOTextOnly.h>


static ULONG NAcorn_Dos[]={
  0,0,0,0
};


////////////////////////////////////////////////////////////////////////////////
//  NAcornHandler Class
////////////////////////////////////////////////////////////////////////////////

NAcornHandler::NAcornHandler()
{
  DEBUG_OUT<<"NAcornHandler constructor.\n";
  osHelp=(NAcornDos *)NULL;
  init();
}


NAcornHandler::~NAcornHandler()
{
  DEBUG_OUT<<"NAcornHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


void NAcornHandler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


const char *NAcornHandler::getName()
{
  if(io_type==N_ACORN_IO_ATOM) return "Acorn Atom";
  if(io_type==N_ACORN_IO_BBC) return "Acorn BBC";
  return "Unknown Acorn";
}


/* STATIC */
bool NAcornHandler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


/* STATIC */
unsigned int NAcornHandler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"acorn")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"atom")) return N_ACORN_IO_ATOM;
  if(!strcmp(name,"electron")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"bbc")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"bbca")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"bbcb")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"bbc1")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"bbc2")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"bbcbp")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"bbcb+")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"master")) return N_ACORN_IO_BBC;
  if(!strcmp(name,"master128")) return N_ACORN_IO_BBC;
  return N_ACORN_IO_NONE;
}


void NAcornHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  DEBUG_OUT<<"io_type is "<<(int)io_type<<"\n";
  switch(io_type) {
    case N_ACORN_IO_ATOM:
      myVideo=new EVideoIOTextOnly(this,32,24);
      if(myVideo) {
        myVideo->setBackground(0x004000);
        myVideo->setDefaultForeground(0x00ff00);
        myVideo->setLocation(0x8000);
      }
      myIO=new NAcornIO(this);
      break;
    case N_ACORN_IO_BBC:
      myVideo=new EVideoIOTextOnly(this,40,24);
      if(myVideo) {
        myVideo->setBackground(0x000000);
        myVideo->setDefaultForeground(0xffffff);
        myVideo->setLocation(0x7c00);
      }
      myIO=new NAcornIO(this);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_ACORN_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  osHelp=new NAcornDos(this);
  osHelp->patchAll(NAcorn_Dos);
}


bool NAcornHandler::read8ret(ULONG addr,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    case 0xfc:  // "fred"
      return false;
      break;
    case 0xfd:  // "jim"
      return false;
      break;
    case 0xfe:  // "sheila"
      //DEBUG_OUT<<"read8ret at "<<addr<<"\n";
      if(io_type==N_ACORN_IO_BBC) {
        *ret=myIO->read8(addr);
        return true;
      }
      else return false;
      break;
    default:
      return false;
      break;
  }
  return false;
}


bool NAcornHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    case 0xfc:  // "fred"
      return false;
      break;
    case 0xfd:  // "jim"
      return false;
      break;
    case 0xfe:  // "sheila"
      //DEBUG_OUT<<"write8ret at "<<addr<<"\n";
      if(io_type==N_ACORN_IO_BBC) {
        *ret=myIO->write8(addr,val);
        return true;
      }
      else return false;
      break;
    default:
      return false;
      break;
  }
  return false;
}

