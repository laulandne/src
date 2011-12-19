#ifndef JAVAGLUE_JSTREAMTOKENIZER_H
#define JAVAGLUE_JSTREAMTOKENIZER_H


#include <javacpp/javacpp.h>


#include <javacpp/JInputStream.h>


const unsigned int JStreamTokenizer_TT_EOL=1;
const unsigned int JStreamTokenizer_TT_EOF=2;
const unsigned int JStreamTokenizer_TT_WORD=10;
const unsigned int JStreamTokenizer_TT_NUMBER=20;


class JStreamTokenizer
{
public:
  JStreamTokenizer(JInputStream *is);
  virtual ~JStreamTokenizer();
  void eolIsSignificant(boolean t);
  void commentChar(char c);
  unsigned int nextToken();
  char *toString();
  unsigned int ttype;
  Float nval;
  char *sval;
protected:
  boolean eolSig;
  char comment;
  JInputStream *ist;
  char *data;
  unsigned int size;
  unsigned int ptr;
};


#endif // JAVAGLUE_JSTREAMTOKENIZER_H

