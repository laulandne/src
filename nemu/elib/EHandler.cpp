
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <elib/EHandler.h>
#include <elib/EMemMapAbstract.h>
#include <elib/ESoundIOAbstract.h>
#include <elib/EVideoIOAbstract.h>
#include <elib/EKeys.h>
#include <elib/EOSHelper.h>

#include <cpulib/ECPUAbstract.h>

#include <disklib/DVfs.h>


////////////////////////////////////////////////////////////////////////////////
//  EHandler Class
////////////////////////////////////////////////////////////////////////////////

EHandler::EHandler()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"EHandler created...\n";
#endif // DEBUG_VERBOSE
  init();
  // These values are arbitrary...
  timerFactor=1;  timerSpeed=6;
}


EHandler::~EHandler()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"EHandler destroyed.\n";
#endif // DEBUG_VERBOSE
  if(err.error()) { DEBUG_OUT<<"EHandler with error!\n"; return; }
  freeIO();
  init();
  //err.set();
}


void EHandler::init()
{
  // Init all data members
  io_debug=false;  io_stop=false;
  debug=false;
  map=(EMemMapAbstract *)NULL;
  osHelp=(EOSHelper *)NULL;
  cpu=(ECPUAbstract *)NULL;
  myVideo=(EVideoIOAbstract *)NULL;
  mySound=(ESoundIOAbstract *)NULL;
  myIO=(EMemIO *)NULL;
  vfs=(DVfs *)NULL;
  io_type=0;  io_variation=0;
  io_keypress=0;
  timerFactor=0;  timerSpeed=0;
  resetAddr=0;  resetStack=0;
  hasSound=false;
}


void EHandler::debugger(const char *str)
{
  debug=true;
  if(cpu) cpu->debugger(str);
}


const char *EHandler::getName()
{
  return "ERROR! Parent class EHandler!";
}


void EHandler::setupVfs()
{
  if(vfs) delete vfs;
  vfs=new DVfs(this);  // NOTE: perfectly legal to fail...
}


void EHandler::setKind(const char *name)
{
  std::cerr<<"parent setKind called!\n";
}


void EHandler::io_cycle()
{
  //*w<<"1";
  if(myVideo) { /* *w<<"2"; */ myVideo->io_cycle(); }
  if(mySound) { /* *w<<"2"; */ mySound->io_cycle(); }
  if(myIO) { /* *w<<"3"; */ myIO->io_cycle(); }
  my_io_cycle();
}


void EHandler::redraw()
{
  if(myVideo) myVideo->redraw();
}


void EHandler::io_init(BYTE type)
{
  std::cerr<<"parent io_init called!\n";
}


void EHandler::focusVideo()
{
  if(myVideo) {
    myVideo->focus();
    //redraw();
  }
}


/*
void EHandler::keypress(AEvent *e)
{
  io_keypress=(unsigned int)e->code;
  unsigned int k=io_keypress&0xff;
  if(k==E_KEY_GFX) myVideo->showGfx();
  if(k==E_KEY_SPRITES) myVideo->showSpriteGfx();
  if(k==E_KEY_RESET)  if(getCPU()) getCPU()->reset();
  if(k==E_KEY_NMI)  if(getCPU()) getCPU()->nmi();
}
*/


void EHandler::keypress(int c)
{
  io_keypress=(unsigned int)c;
  unsigned int k=io_keypress&0xff;
}


void EHandler::ioregs()
{
  if(myIO) myIO->ioregs();
  //*w<<"timerSpeed/Factor: "<<timerSpeed<<"/"<<timerFactor<<"\n";
  my_ioregs();
}


void EHandler::vregs()
{
  if(myVideo) myVideo->vregs();
}


void EHandler::reset()
{
  if(myIO) myIO->reset();
  if(vfs) vfs->reset();
  my_reset();
}


void EHandler::reset_postCPU()
{
  //if(myIO) myIO->reset();
  //if(myIOLow) myIOLow->reset();
  //if(myIOHigh) myIOHigh->reset();
  //if(dos) dos->reset();
  my_reset_postCPU();
}


bool EHandler::systemCall(unsigned int num)
{
  if(!osHelp) return false;
  bool ret=false;
  ULONG loc=cpu->getPC();
  //DEBUG_OUT<<"EHandler::systemCall("<<num<<") at "<<loc<<" going to call osHelp->systemCall...\n";
  ret=osHelp->systemCall(loc);
  //DEBUG_OUT<<"Back from osHelp->systemCall, which returned "<<ret<<".\n";
  return ret;
}


bool EHandler::ioDisHelper(ULONG addr)
{
  // We didn't handle it
  return false;
}


void EHandler::my_io_cycle()
{
  // This will be overridden by a derived class
  // with anything special it needs to do
}


void EHandler::my_ioregs()
{
  // This will be overridden by a derived class
  // with anything special it needs to do
}


void EHandler::my_reset()
{
  // This will be overridden by a derived class
  // with anything special it needs to do
}


void EHandler::my_reset_postCPU()
{
  // This will be overridden by a derived class
  // with anything special it needs to do
}


void EHandler::freeIO()
{
  unmapParts();
  if(myVideo) {
    //*w<<"(Getting rid of old video...)\n";
    delete myVideo;
    myVideo=(EVideoIOAbstract *)NULL;
  }
  if(mySound) {
    //*w<<"(Getting rid of old sound...)\n";
    delete mySound;
    mySound=(ESoundIOAbstract *)NULL;
  }
  //*w<<"Checking myIO...\n";
  if(myIO) {
    //*w<<"(Getting rid of old I/O...)\n";
    delete myIO;
    myIO=(EMemIO *)NULL;
  }
  //*w<<"Checking myIOLow...\n";
}


void EHandler::connectParts()
{
}


void EHandler::mapParts()
{
  if(map) {
    DEBUG_OUT<<"EHandler::mapParts Going to map I/O and video...\n";
    if(myVideo) {
      if(myVideo->err.error()) { err.set(); return; }
    }
    if(mySound) {
      if(mySound->err.error()) { err.set(); return; }
    }
    if(myIO) {
      if(myIO->err.error()) { err.set(); return; }
      map->map(myIO);
    }
  }
  else DEBUG_OUT<<"Can't map I/O or video, no map!\n";
  connectParts();
}


void EHandler::unmapParts()
{
  if(map) {
    //if(myVideo) map->clearVideoMap();
    //if(myIO) { map->unmap(myIO);  myIO->setVideo((EVideoIOAbstract *)NULL); }
  }
  else DEBUG_OUT<<"Can't unmap I/O or video, no map!\n";
}


bool EHandler::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  return false;
}


// TODO: Yes, this is here for a reason...
// ...not that I can remember it at the moment...
void EHandler::xia_write8(ULONG addr, BYTE val)
{
  debugger("EHandler::xia_write8 called");
}


// TODO: Yes, this is here for a reason...
// ...not that I can remember it at the moment...
BYTE EHandler::xia_read8(ULONG addr)
{
  BYTE ret=0;
  debugger("EHandler::xia_read8 called");
  return ret;
}


void EHandler::io_out8(ULONG addr, BYTE val)
{
  debugger("EHandler::io_out8 called");
}


BYTE EHandler::io_in8(ULONG addr)
{
  BYTE ret=0;
  debugger("EHandler::io_in8 called");
  return ret;
}


bool EHandler::read8ret(ULONG addr,BYTE *ret)
{
  return false;
}


bool EHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  return false;
}
