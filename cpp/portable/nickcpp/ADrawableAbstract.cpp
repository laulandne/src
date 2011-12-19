
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <stdlib.h>

#include <portable/nickcpp/ADrawableAbstract.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ADrawableAbstract Class
////////////////////////////////////////////////////////////////////////////////

ADrawableAbstract::ADrawableAbstract()
{
  DEBUG_OUT<<"ADrawableAbstract::ADrawableAbstract()\n";
  init();
}


ADrawableAbstract::~ADrawableAbstract()
{
  DEBUG_OUT<<"ADrawableAbstract::~ADrawableAbstract()\n";
}


unsigned int ADrawableAbstract::getFontHeight()
{
  std::cerr<<"ADrawableAbstract getFontHeight called!\n"; exit(5);
  return 0;
}


char *ADrawableAbstract::queryFontAvail(char *name,unsigned int size,unsigned int flags)
{
  std::cerr<<"ADrawableAbstract setFont called!\n"; exit(5);
  return NULL;
}


void ADrawableAbstract::setFont(char *name,unsigned int size,unsigned int flags)
{
  std::cerr<<"ADrawableAbstract setFont called!\n"; exit(5);
}


void ADrawableAbstract::setFontSize(unsigned int size,unsigned int flags)
{
  std::cerr<<"ADrawableAbstract setFontSize called!\n"; exit(5);
}


bool ADrawableAbstract::setColor(unsigned int colorNum, unsigned int r, unsigned int g,unsigned int b)
{
  std::cerr<<"ADrawableAbstract setColor called!\n"; exit(5);
}


bool ADrawableAbstract::setColor(unsigned int colorNum, unsigned long l)
{
  std::cerr<<"ADrawableAbstract setColor called!\n"; exit(5);
}


bool ADrawableAbstract::setColor(unsigned int index,AColor *aColor)
{
  std::cerr<<"ADrawableAbstract setColor called!\n"; exit(5);
}


void ADrawableAbstract::drawText(const char *str,unsigned int len,bool remember)
{
  std::cerr<<"ADrawableAbstract drawText called!\n"; exit(5);
}


void ADrawableAbstract::draw(ADrawableAbstract *source, unsigned int dx, unsigned int dy)
{
  std::cerr<<"ADrawableAbstract draw called!\n"; exit(5);
}


void ADrawableAbstract::copyBlock(ADrawableAbstract *source, unsigned int sx, unsigned int sy,unsigned int dx,unsigned int dy,unsigned bw,unsigned bh)
{
  std::cerr<<"ADrawableAbstract copyBlock called!\n"; exit(5);
}


void ADrawableAbstract::rotate(ADrawableAbstract *dest,int angle)
{
  std::cerr<<"ADrawableAbstract rotate called!\n"; exit(5);
}


void ADrawableAbstract::flip(bool dx, bool dy)
{
  std::cerr<<"ADrawableAbstract flip called!\n"; exit(5);
}


void ADrawableAbstract::scale(int xFactor, int yFactor)
{
  std::cerr<<"ADrawableAbstract scale called!\n"; exit(5);
}


void ADrawableAbstract::scale(ADrawableAbstract *dest,int xFactor, int yFactor)
{
  std::cerr<<"ADrawableAbstract scale called!\n"; exit(5);
}


void ADrawableAbstract::scroll(unsigned int tx,unsigned int ty,unsigned int tw,unsigned int th, int dx, int dy)
{
  std::cerr<<"ADrawableAbstract scroll called!\n"; exit(5);
}


void ADrawableAbstract::clear(bool preserveMap)
{
  std::cerr<<"ADrawableAbstract clear called!\n"; exit(5);
}


void ADrawableAbstract::clear(unsigned int x, unsigned int y,unsigned int tw,unsigned int th)
{
  std::cerr<<"ADrawableAbstract clear called!\n"; exit(5);
}


void ADrawableAbstract::line(unsigned int x1, unsigned int y1,unsigned int x2, unsigned int y2)
{
  std::cerr<<"ADrawableAbstract line called!\n"; exit(5);
}


void ADrawableAbstract::writePixel(unsigned int x, unsigned int y)
{
  std::cerr<<"ADrawableAbstract writePixel called!\n"; exit(5);
}


unsigned int ADrawableAbstract::readPixel(unsigned int x, unsigned int y)
{
  std::cerr<<"ADrawableAbstract readPixel called!\n"; exit(5);
}


void ADrawableAbstract::setForeground(unsigned long n,bool rgb)
{
  std::cerr<<"ADrawableAbstract setForeground called!\n"; exit(5);
}


void ADrawableAbstract::setBackground(unsigned long n,bool rgb)
{
  std::cerr<<"ADrawableAbstract setBackground called!\n"; exit(5);
}


void ADrawableAbstract::oval(int tx,int ty,int twidth, int theight)
{
  std::cerr<<"ADrawableAbstract oval called!\n"; exit(5);
}


void ADrawableAbstract::fillOval(int tx,int ty,int twidth, int theight)
{
  std::cerr<<"ADrawableAbstract fillOval called!\n"; exit(5);
}


void ADrawableAbstract::fillRect(int tx,int ty,int twidth, int theight)
{
  std::cerr<<"ADrawableAbstract fillRect called!\n"; exit(5);
}


int ADrawableAbstract::getPenX()
{
  std::cerr<<"ADrawableAbstract getPenX called!\n"; exit(5);
}


int ADrawableAbstract::getPenY()
{
  std::cerr<<"ADrawableAbstract getPenY called!\n"; exit(5);
}


void ADrawableAbstract::scroll(int dx,int dy)
{
  std::cerr<<"ADrawableAbstract scroll called!\n"; exit(5);
}


void ADrawableAbstract::moveTo(unsigned int newX, unsigned int newY)
{
  std::cerr<<"ADrawableAbstract moveTo called!\n"; exit(5);
}


void ADrawableAbstract::lineTo(unsigned int newX, unsigned int newY)
{
  std::cerr<<"ADrawableAbstract lineTo called!\n"; exit(5);
}


void ADrawableAbstract::eraseRect(unsigned int rwidth,unsigned int rheight)
{
  std::cerr<<"ADrawableAbstract eraseRect called!\n"; exit(5);
}


void ADrawableAbstract::drawRect(unsigned int rwidth,unsigned int rheight)
{
  std::cerr<<"ADrawableAbstract drawRect called!\n"; exit(5);
}


void ADrawableAbstract::drawFilledRect(unsigned int rwidth,unsigned int rheight)
{
  std::cerr<<"ADrawableAbstract drawFilledRect called!\n"; exit(5);
}

