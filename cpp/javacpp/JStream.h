#ifndef JAVAGLUE_JSTREAM_H
#define JAVAGLUE_JSTREAM_H


#include <javacpp/javacpp.h>


class JStream {
public:
  JStream(char *fn);
  char *getName();
protected:
  char *name;
};


#endif // JAVAGLUE_JSTREAM_H

