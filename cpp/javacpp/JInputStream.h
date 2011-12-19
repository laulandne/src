#ifndef JAVAGLUE_JINPUTSTREAM_H
#define JAVAGLUE_JINPUTSTREAM_H


#include <javacpp/javacpp.h>


#include <javacpp/JStream.h>


class JInputStream {
public:
  JInputStream(JStream *s);
  virtual ~JInputStream();
  void close();
  int read(char *buf,unsigned int count);
  int getSize();
protected:
  JStream *st;
  FILE *f;
};


#endif // JAVAGLUE_JINPUTSTREAM_H

