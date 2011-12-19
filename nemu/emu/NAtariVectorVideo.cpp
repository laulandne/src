
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
//  NAtariVectorVideo Class
////////////////////////////////////////////////////////////////////////////////

NAtariVectorVideo::NAtariVectorVideo(NAtariVectorHandler *parent,unsigned int ncol,unsigned int nrow)
  : EVideoIOVector(parent,ncol,nrow)
{
  DEBUG_OUT<<"NAtariVectorVideo created.\n";
  redrawLock=true;
  //setMemSize(36,30);
  setLocation(0);
  redrawLock=false;
}


NAtariVectorVideo::~NAtariVectorVideo()
{
}


