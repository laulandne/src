#ifndef GUILIB_AWINDOW_H
#define GUILIB_AWINDOW_H


#include "AWindowAbstract.h"


class AWindow : public AWindowAbstract
{
public:
  // Various constructors
  AWindow();
  AWindow(const char *newName);
  AWindow(const char *newName, unsigned int newWidth, unsigned int newHeight);
  AWindow(const char *newName, unsigned int newWidth, unsigned int newHeight, int newx, int newy);
  AWindow(unsigned int newWidth, unsigned int newHeight);
  AWindow(ADisplay *someDisplay);
  AWindow(ADisplay *someDisplay, const char *newName);
  AWindow(ADisplay *someDisplay, const char *newName,unsigned int newWidth, unsigned int newHeight);
  AWindow(ADisplay *someDisplay, const char *newName,unsigned int newWidth, unsigned int newHeight, int newx, int newy);
  AWindow(ADisplay *someDisplay,unsigned int newWidth, unsigned int newHeight);
  virtual ~AWindow();
protected:
};


extern AWindow *aDefaultAWindow;


#endif // GUILIB_AWINDOW_H

