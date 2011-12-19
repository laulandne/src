
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/EVideoIOTms99xx.h>
#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/ANullStream.h>

#include <cpulib/ECPUAbstract.h>


////////////////////////////////////////////////////////////////////////////////
//  EVideoIOTms99xx Class
////////////////////////////////////////////////////////////////////////////////

EVideoIOTms99xx::EVideoIOTms99xx(EHandler *parent,unsigned int ncol,unsigned int nrow) : EVideoIOMultiMode(parent,ncol,nrow)
{
  DEBUG_OUT<<"EVideoIOTms99xx created.\n";
  redrawLock=true;
  //setMemSize(36,30);
  setLocation(0);
  if(!err.error()) {
    setOffsets();
    setValid();
    redrawLock=false;
    // TODO: Not actually correct...
    for(unsigned int t=0;t<8;t++) {
      setSpriteActive(t,false);
      sprites[t].x=t*16; sprites[t].y=0;
      sprites[t].width=32;  sprites[t].height=200;
    }
  }
}


EVideoIOTms99xx::~EVideoIOTms99xx()
{
  //if(!error()) freeVideo();
  //err.set();
}
