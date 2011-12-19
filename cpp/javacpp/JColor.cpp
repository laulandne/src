
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JColor.h>

#include <portable/nickcpp/APalette.h>


////////////////////////////////////////////////////////////////////////////////
//  JColor Class
////////////////////////////////////////////////////////////////////////////////

JColor::JColor()
{
  DEBUG_OUT<<"JColor::JColor()\n";
  c=new AColor;
  c->red=0;
  c->green=0;
  c->blue=0;
}


JColor::JColor(int r,int g,int b)
{
  DEBUG_OUT<<"JColor::JColor("<<r<<","<<g<<","<<b<<")\n";
  c=new AColor;
  c->red=r;
  c->green=g;
  c->blue=b;
}


void JColor::set(int r,int g,int b)
{
  DEBUG_OUT<<"JColor::set("<<r<<","<<g<<","<<b<<")\n";
  c->red=r;
  c->green=g;
  c->blue=b;
}


