#ifndef GUILIB_ADialogAbstract_H
#define GUILIB_ADialogAbstract_H


#include "AWindow.h"


class ADialogAbstract : public AWindowAbstract {
public:
  // Methods
  ADialogAbstract();
  virtual ~ADialogAbstract();
  virtual char *show(char *theName,char *theMsg=(char *)NULL);
  virtual char *hide();
  // Data
  unsigned int argc;  // should and will be protected soon...
  char *argv[10];  // should and will be protected soon...
protected:
  // Methods
  void init();
  unsigned int countLinesInMsg(char *theMsg);
  virtual char *doTheHardWork(char *name,char *msg);
  // Data
  bool currentlyShown;
  char *returnValue;
};


#endif // GUILIB_ADialogAbstract_H
