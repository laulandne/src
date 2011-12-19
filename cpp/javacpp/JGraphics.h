#ifndef JAVAGLUE_JGRAPHICS_H
#define JAVAGLUE_JGRAPHICS_H


#include <javacpp/javacpp.h>


#include <javacpp/JColor.h>


class JGraphics {
public:
  JGraphics();
  void drawString(JString text,unsigned int x, unsigned int y);
  void setColor(JColor *c);
  void drawLine(int,int,int,int);
  //
  void eraseWindow();
protected:
};


#endif // JAVAGLUE_JGRAPHICS_H

