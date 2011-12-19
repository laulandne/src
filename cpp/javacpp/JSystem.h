#ifndef JAVAGLUE_JSYSTEM_H
#define JAVAGLUE_JSYSTEM_H


#include <javacpp/javacpp.h>


class JSystem {
public:
  void arraycopy(Float *src, int srcPos, Float *dest, int destPos, int length);
  void arraycopy(int *src, int srcPos, int *dest, int destPos, int length);
};


#endif // JAVAGLUE_JSYSTEM_H

