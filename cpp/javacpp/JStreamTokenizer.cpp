
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JStreamTokenizer.h>


////////////////////////////////////////////////////////////////////////////////
//  JStreamTokenizer Class
////////////////////////////////////////////////////////////////////////////////

JStreamTokenizer::JStreamTokenizer(JInputStream *is)
{
  DEBUG_OUT<<"JStreamTokenizer::JStreamTokenizer()\n";
  if(!is) {
  DEBUG_OUT<<"passed a null JInputStream!\n";
	ist=NULL; data=NULL; size=0; return;
  }
  ist=is;
  eolSig=false;
  comment='#';
  sval=NULL;
  nval=0;
  ttype=JStreamTokenizer_TT_EOF;
  ptr=0;
  DEBUG_OUT<<"is size="<<size<<"\n";
  size=is->getSize();
  data=NULL;
  if(size) {
    data=(char *)malloc(size);
  }
  if(data) {
    is->read(data,size);
  }
}


JStreamTokenizer::~JStreamTokenizer()
{
  if(data) free(data);
  data=NULL;
}


void JStreamTokenizer::eolIsSignificant(bool t)
{
  DEBUG_OUT<<"JStreamTokenizer::eolIsSignificant("<<t<<")\n";
  eolSig=t;
}


void JStreamTokenizer::commentChar(char c)
{
  DEBUG_OUT<<"JStreamTokenizer::commentChar('"<<c<<"')\n";
  comment=c;
}


unsigned int JStreamTokenizer::nextToken()
{
  //DEBUG_OUT<<"JStreamTokenizer::nextToken()\n";
  if(!data) {
  DEBUG_OUT<<"nextToken, but no data stream!\n";
    ttype=JStreamTokenizer_TT_EOF;
	return JStreamTokenizer_TT_EOF;
  }
  if(ptr==size) {
    DEBUG_OUT<<"At EOF.\n";
    ttype=JStreamTokenizer_TT_EOF;
    return JStreamTokenizer_TT_EOF;
  }
  unsigned int ret=JStreamTokenizer_TT_WORD;
  char buf[256];
  char c;
  unsigned int t=0;
  bool reading=true;
  while(reading) {
    if(ptr>size) {
      DEBUG_OUT<<"nextToken ran past end of data!\n";
      reading=false;
      return JStreamTokenizer_TT_EOF;
    }
    c=data[ptr];
    buf[t]=c;
    //DEBUG_OUT<<"\""<<c<<"\"";
    if((c==' ')||(c=='\t')) {
      if(t) reading=false;
    }
    if(c=='\n') reading=false;
    if(reading) {
      if((c!=' ')&&(c!='\t')) t++;
      ptr++;
    }
  }
  buf[t]=0;
  DEBUG_OUT<<"JStreamTokenizer::nextToken() got \""<<buf<<"\"\n";
  if(!t) {
    //DEBUG_OUT<<"t was zero!\n";
    ptr++;
    if(c=='\n') ret=JStreamTokenizer_TT_EOL;
  }
  switch(buf[0]) {
    case '0':
    case '.':
    case '-':
      sscanf(buf,"%f",&nval);
      ret=JStreamTokenizer_TT_NUMBER;
      break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      sscanf(buf,"%f",&nval);
      if(nval!=0.0) ret=JStreamTokenizer_TT_NUMBER;
      break;
    default:
      break;
  }
  if(ret==JStreamTokenizer_TT_WORD) sval=strdup(buf);
  //DEBUG_OUT<<"Going to return "<<ret<<"\n";
  ttype=ret;
  return ret;
}


char *JStreamTokenizer::toString()
{
  DEBUG_OUT<<"JStreamTokenizer::toString()\n";
  char *ret=(char *)"";
  DEBUG_OUT<<"Going to return \""<<ret<<"\"\n";
  return ret;
}



