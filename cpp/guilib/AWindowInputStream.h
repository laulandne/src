#ifndef ALIB_AWINDOWINPUTSTREAM_H
#define ALIB_AWINDOWINPUTSTREAM_H


#include <portable/nickcpp/AInputStream.h>

#include "AWindow.h"


class AFileInputStream;


class AWindowInputStream : public AInputStream
{
public:
  AWindowInputStream(AWindow *w=NULL,AFileInputStream *f=NULL);
  virtual ~AWindowInputStream();
  void setWindow(AWindow *w) { theWin=w; }
  AWindow *getWindow() { return theWin; }
protected:
  void init();
  virtual unsigned int readBuf(char *s, unsigned int limit);
  AWindow *theWin;
  AFileInputStream *theFile;
};


extern AWindowInputStream *aDefaultAWindowInputStream;


#endif // ALIB_AWINDOWINPUTSTREAM_H

