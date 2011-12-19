
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


/*

NOTE: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


#include <elib/EResource.h>


////////////////////////////////////////////////////////////////////////////////
//  EResource Class
////////////////////////////////////////////////////////////////////////////////

EResource::EResource(const char *tname,const char *ttype,unsigned int tnum,unsigned int toffset,
  unsigned int tsize)
{
  name=tname;
  type=ttype;
  num=tnum;
  offset=toffset;
  size=tsize;
}


void EResource::init()
{
  name=(char *)NULL;
  type=(char *)NULL;
  num=0;
  offset=0;
  size=0;
}

