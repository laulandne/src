#ifndef JAVAGLUE_JFLOAT_H
#define JAVAGLUE_JFLOAT_H


#include <javacpp/javacpp.h>


class JFloat {
public:
  JFloat(Float t);
  Float floatValue();
protected:
  Float f;
};


JFloat *JFloat_valueOf(char *s);


#endif // JAVAGLUE_JFLOAT_H

