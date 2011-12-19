
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JException.h>


////////////////////////////////////////////////////////////////////////////////
//  JException Class
////////////////////////////////////////////////////////////////////////////////

JException::JException(char *s)
{
  DEBUG_OUT<<"JException::JException(\""<<s<<"\")\n";
  name=s;
}


char *JException::toString()
{
  DEBUG_OUT<<"JException::toString()\n";
  return (char *)"JException_toString";
}



