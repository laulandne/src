#ifndef JAVAGLUE_JEXCEPTION_H
#define JAVAGLUE_JEXCEPTION_H


#include <javacpp/javacpp.h>


class JException {
public:
  JException(char *s);
  char *toString();
protected:
  char *name;
};


#endif // JAVAGLUE_JEXCEPTION_H

