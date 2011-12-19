
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JGraphics.h>


#ifdef NICK_USE_GUILIB
#include <guilib/nickguilib.h>
#endif // NICK_USE_GUILIB

#ifdef NICK_USE_SDL
#include <sdl/nicksdl.h>
#endif // NICK_USE_SDL

#ifdef NICK_USE_GLUT
#include <glut/nickglut.h>
#endif // NICK_USE_GLUT


////////////////////////////////////////////////////////////////////////////////
//  JGraphics Class
////////////////////////////////////////////////////////////////////////////////

JGraphics::JGraphics()
{
}


void JGraphics::drawString(JString text,unsigned int x, unsigned int y)
{
  DEBUG_OUT<<"JGraphics::drawString()\n";
}


void JGraphics::setColor(JColor *c)
{
  DEBUG_OUT<<"JGraphics::setColor()\n";
  if(c) {
    if(c->c) {
      unsigned int r=c->c->red,g=c->c->green,b=c->c->blue;
      unsigned long rgb=(r<<16)|(b<<8)|g;
      DEBUG_OUT<<"rgb is "<<rgb<<"\n";
      nickSetColorRGB(r,g,b);
    }
    else DEBUG_OUT<<"No c->c.\n";
  }
  else DEBUG_OUT<<"No c.\n";
}


void JGraphics::drawLine(int a,int b,int c,int d)
{
  DEBUG_OUT<<"JGraphics::drawLine("<<a<<","<<b<<","<<c<<","<<d<<")\n";
  nickDrawLine(a,b,c,d);
}


void JGraphics::eraseWindow()
{
  nickSetColorRGB(0,0,0);
  //nickSetColorRGB(0xff,0xff,0xff);
  nickClearDisplay();
}



