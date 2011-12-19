#ifndef ALIB_AOUTPUTSTREAM_H
#define ALIB_AOUTPUTSTREAM_H


// So you don't have to include it yourself, and can swap it in for a real stream any time
#include "ANullStream.h"


class AOutputStream {
public:
  AOutputStream();
  AOutputStream& operator<<(unsigned long s);
  AOutputStream& operator<<(long s);
  AOutputStream& operator<<(unsigned short s);
  AOutputStream& operator<<(short s);
  AOutputStream& operator<<(unsigned int s);
  AOutputStream& operator<<(int s);
  AOutputStream& operator<<(char s);
  AOutputStream& operator<<(float s);
  AOutputStream& operator<<(double s);
  //
  AOutputStream& operator<<(const char *s);
  //
  AOutputStream& hex();
  AOutputStream& dec();
protected:
  void init();
  virtual unsigned int writeBuf(const char *s);
  bool inHex;
  char buffer[128];
};


#endif // ALIB_AOUTPUTSTREAM_H

