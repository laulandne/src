
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JStream.h>


////////////////////////////////////////////////////////////////////////////////
//  JStream Class
////////////////////////////////////////////////////////////////////////////////

JStream::JStream(char *fn)
{
  DEBUG_OUT<<"JStream::JStream(\""<<fn<<"\")\n";
  name=fn;
}


char *JStream::getName()
{
  return name;
}



