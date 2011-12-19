
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JMath.h>


#define _USE_MATH_DEFINES 1
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
//  JMath Class
////////////////////////////////////////////////////////////////////////////////

double JMath::pow(double a,double b)
{
  DEBUG_OUT<<"JMath::pow()\n";
  return ::pow(a,b);
}


double JMath::cos(double a)
{
  DEBUG_OUT<<"JMath::cos()\n";
  return ::cos(a);
}


double JMath::sin(double a)
{
  DEBUG_OUT<<"JMath::sin()\n";
  return ::sin(a);
}




