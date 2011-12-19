#ifndef JAVAGLUE_JCOLOR_H
#define JAVAGLUE_JCOLOR_H


#include <javacpp/javacpp.h>


#include <portable/nickcpp/AColor.h>


class JColor {
public:
  JColor();
  JColor(int,int,int);
  void set(int,int,int);
  AColor *c;
protected:
};


#endif // JAVAGLUE_JCOLOR_H

