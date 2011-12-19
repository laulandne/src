
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JSystem.h>


////////////////////////////////////////////////////////////////////////////////
//  JSystem Class
////////////////////////////////////////////////////////////////////////////////

void JSystem::arraycopy(float *src, int srcPos, float *dest, int destPos, int length)
{
  DEBUG_OUT<<"JSystem::arraycopy(float *src,"<<srcPos<<",float *dest,"<<destPos<<","<<length<<")\n";
  if(!src) return;
  for(int t=0;t<length;t++) {
    dest[destPos+t]=src[srcPos+t];
  }
}


void JSystem::arraycopy(int *src, int srcPos, int *dest, int destPos, int length)
{
  DEBUG_OUT<<"JSystem::arraycopy(int *src,"<<srcPos<<",int *dest,"<<destPos<<","<<length<<")\n";
  if(!src) return;
  for(int t=0;t<length;t++) {
    dest[destPos+t]=src[srcPos+t];
  }
}



