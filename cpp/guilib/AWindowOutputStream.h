#ifndef ALIB_AWINDOWOUTPUTSTREAM_H
#define ALIB_AWINDOWOUTPUTSTREAM_H


#include <portable/nickcpp/AOutputStream.h>

#include "AWindow.h"


class AFileOutputStream;


class AWindowOutputStream : public AOutputStream
{
public:
  AWindowOutputStream(AWindow *w=NULL,AFileOutputStream *f=NULL);
  virtual ~AWindowOutputStream();
  void setWindow(AWindow *w) { theWin=w; }
  AWindow *getWindow() { return theWin; }
  void warning(const char *s);
protected:
  void init();
  virtual unsigned int writeBuf(const char *s);
  AWindow *theWin;
  AFileOutputStream *theFile;
};


extern AWindowOutputStream *aDefaultAWindowOutputStream;


#endif // ALIB_AWINDOWOUTPUTSTREAM_H

