
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdio.h>

#include "AOutputStream.h"


////////////////////////////////////////////////////////////////////////////////
//  AOutputStream Class
////////////////////////////////////////////////////////////////////////////////

AOutputStream::AOutputStream()
{
  init();
}


void AOutputStream::init()
{
  inHex=false;
  for(unsigned int t=0;t<128;t++) buffer[t]=0;
}


AOutputStream& AOutputStream::operator<<(float n)
{
  // NOTE: Should do hex too...
  sprintf(buffer,"%f",n);
  writeBuf(buffer);
  return *this;
}


AOutputStream& AOutputStream::operator<<(double n)
{
  // NOTE: Should do hex too...
  sprintf(buffer,"%g",n);
  writeBuf(buffer);
  return *this;
}


AOutputStream& AOutputStream::operator<<(unsigned long s)
{
  if(inHex) {
    // Not right for 64 bit archs, but we don't want to print tons of zeros
    sprintf(buffer,"%08x",(unsigned int)s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myHex((unsigned long)s));
  }
  else {
    sprintf(buffer,"%d",(int)s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myDec(s));
  }
  return *this;
}


AOutputStream& AOutputStream::operator<<(long s)
{
  if(inHex) {
    // Not right for 64 bit archs, but we don't want to print tons of zeros
    sprintf(buffer,"%08x",(unsigned int)s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myHex((unsigned long)s));
  }
  else {
    sprintf(buffer,"%d",(int)s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myDec(s));
  }
  return *this;
}


AOutputStream& AOutputStream::operator<<(unsigned short s)
{
  if(inHex) {
    sprintf(buffer,"%04x",s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myHex((unsigned short)s));
  }
  else {
    sprintf(buffer,"%d",s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myDec(s));
  }
  return *this;
}


AOutputStream& AOutputStream::operator<<(short s)
{
  if(inHex) {
    sprintf(buffer,"%04x",s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myHex((unsigned short)s));
  }
  else {
    sprintf(buffer,"%d",s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myDec(s));
  }
  return *this;
}


AOutputStream& AOutputStream::operator<<(unsigned int s)
{
  if(inHex) {
    // NOTE: Bit of a kludge so bytes get shown with a leading zero...
    //if(s<0x10) *this<<'0';
    sprintf(buffer,"%x",s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myHex((unsigned char)s));
  }
  else {
    sprintf(buffer,"%d",s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myDec(s));
  }
  return *this;
}


AOutputStream& AOutputStream::operator<<(int s)
{
  if(inHex) {
    // NOTE: Bit of a kludge so bytes get shown with a leading zero...
    //if(s<0x10) *this<<'0';
    sprintf(buffer,"%x",s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myHex((unsigned char)s));
  }
  else {
    sprintf(buffer,"%d",s);
    writeBuf(buffer);
    //writeBuf(aThisApp.myDec(s));
  }
  return *this;
}


// NOTE: This is the main output routine for strings.
AOutputStream& AOutputStream::operator<<(const char *foo)
{
  if(!foo) return *this;
  writeBuf(foo);
  return *this;
}


// NOTE: This is the main output routine for characters.
AOutputStream& AOutputStream::operator<<(char c)
{
  char s[2];
  s[0]=c;  s[1]=0;
  return *this<<s;
}


unsigned int AOutputStream::writeBuf(const char *s)
{
  // Implemented by children...
  return 0;
}


AOutputStream& AOutputStream::hex()
{
  inHex=true;
  return *this;
}


AOutputStream& AOutputStream::dec()
{
  inHex=false;
  return *this;
}

