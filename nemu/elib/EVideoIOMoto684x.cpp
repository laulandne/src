
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/EVideoIOMoto684x.h>
#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/ANullStream.h>

#include <cpulib/ECPUAbstract.h>


////////////////////////////////////////////////////////////////////////////////
//  EVideoIOMoto684x Class
////////////////////////////////////////////////////////////////////////////////

EVideoIOMoto684x::EVideoIOMoto684x(EHandler *parent,unsigned int ncol,unsigned int nrow) : EVideoIOMultiMode(parent,ncol,nrow)
{
  DEBUG_OUT<<"EVideoIOMoto684x created.\n";
  redrawLock=true;
  setLocation(0);
  redrawLock=false;
}


EVideoIOMoto684x::~EVideoIOMoto684x()
{
  //if(!error()) freeVideo();
  //err.set();
}


