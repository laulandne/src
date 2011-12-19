
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <stdio.h>


#include <javacpp/JFloat.h>


////////////////////////////////////////////////////////////////////////////////
//  global funcs
////////////////////////////////////////////////////////////////////////////////


JFloat* JFloat_valueOf(char *s)
{
  char *t=s;
  if(!t) t=(char *)"NULL";
  DEBUG_OUT<<"JFloat_valueOf(\""<<t<<"\")\n";
  float f=0.0;
  if(s) sscanf(s,"%f",&f);
  DEBUG_OUT<<"going to return "<<f<<"\n";
  return new JFloat(f);
}


////////////////////////////////////////////////////////////////////////////////
//  JFloat Class
////////////////////////////////////////////////////////////////////////////////

JFloat::JFloat(float t)
{
  f=t;
}


float JFloat::floatValue()
{
  DEBUG_OUT<<"JFloat::floatValue()\n";
  DEBUG_OUT<<"going to return "<<f<<"\n";
  return f;
}



