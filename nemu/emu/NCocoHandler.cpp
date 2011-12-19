
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NCocoHandler.h>
#include <emu/NCocoIO.h>
#include <emu/NCocoDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EVideoIOMultiMode.h>


/*
NOTES:

Usable basic ram usually between 501 and b3ff
with hires, top becomes 97ff

screen at bb80

character set at b400
alt chars at b800

chars are 6x8 pixels

via at 300
sound chip on one of the via's ports
keyboard read by one via port and other sound port
(write col to one and row to other then read a single bit)
printer port is another via port

if screen value is 0-31, then it is an attribute and not a char

sceen values over 128 are drawn inverted

note alt char defs overlap text screen!

high res is a000-bf3f
part of text screen at bootom at bf68 to bfdf, but with char defs from 9800-9fff
(because highres overlays where normal chars are)

400-4ff used by disk

sound chip is "8912"

*/

static ULONG NCoco_Dos[]={
  0,0,0,0
};


////////////////////////////////////////////////////////////////////////////////
//  NCocoHandler Class
////////////////////////////////////////////////////////////////////////////////

NCocoHandler::NCocoHandler()
{
  DEBUG_OUT<<"NCocoHandler constructor.\n";
  osHelp=(NCocoDos *)NULL;
  init();
}


NCocoHandler::~NCocoHandler()
{
  DEBUG_OUT<<"NCocoHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


void NCocoHandler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


const char *NCocoHandler::getName()
{
  if(io_type==N_COCO_IO_STANDARD) return "Coco";
  return "Unknown Coco";
}


/* STATIC */
bool NCocoHandler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


/* STATIC */
unsigned int NCocoHandler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"coco")) return N_COCO_IO_STANDARD;
  if(!strcmp(name,"coco1")) return N_COCO_IO_STANDARD;
  if(!strcmp(name,"coco2")) return N_COCO_IO_STANDARD;
  if(!strcmp(name,"coco3")) return N_COCO_IO_STANDARD;
  return N_COCO_IO_NONE;
}


void NCocoHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_COCO_IO_STANDARD:
      myVideo=new EVideoIOMultiMode(this,40,28);
      if(myVideo) {
        myVideo->setBackground(0x0000ff);
        myVideo->setDefaultForeground(0xffffff);
        myVideo->setLocation(0xbb80);
      }
      myIO=new NCocoIO(this);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_COCO_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  osHelp=new NCocoDos(this);
  osHelp->patchAll(NCoco_Dos);
}


bool NCocoHandler::read8ret(ULONG addr,BYTE *ret)
{
  return false;
}


bool NCocoHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  return false;
}


