#ifndef JAVAGLUE_JURL_H
#define JAVAGLUE_JURL_H


#include <javacpp/javacpp.h>


#include <javacpp/JInputStream.h>


class JUrl {
public:
  JUrl(JString base, JString name);
  JInputStream *openStream();
protected:
  char *base;
  char *name;
};


#endif // AJAVAGLUE_JURL_H

