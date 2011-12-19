
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <stdio.h>


#include <javacpp/JInputStream.h>


////////////////////////////////////////////////////////////////////////////////
//  JInputStream Class
////////////////////////////////////////////////////////////////////////////////

JInputStream::JInputStream(JStream *s)
{
  DEBUG_OUT<<"JInputStream::JInputStream()\n";
  if(!s) {
  DEBUG_OUT<<"passed a null JStream!\n";
	st=NULL; f=NULL; return;
  }
  st=s;
  f=fopen(s->getName(),"r");
  if(f) {
    DEBUG_OUT<<"fopen of \""<<s->getName()<<"\" was good.\n";
  }
  else {
    DEBUG_OUT<<"Couldn't fopen \""<<s->getName()<<"\"!\n";
  }
}


JInputStream::~JInputStream()
{
  this->close();
}


void JInputStream::close()
{
  DEBUG_OUT<<"JInputStream::close()\n";
  if(f) fclose(f);
  f=NULL;
}


int JInputStream::read(char *buf,unsigned int count)
{
  if(!f) return -1;
  return fread(buf,1,count,f);
}


int JInputStream::getSize()
{
  int ret=0;
  if(f) {
    int bookmark=ftell(f);
    fseek(f,0,SEEK_END);
    ret=ftell(f);
    fseek(f,bookmark,SEEK_SET);
  }
  return ret;
}



