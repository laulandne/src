#ifndef GUILIB_AMSGDIALOG_H
#define GUILIB_AMSGDIALOG_H


#include "ADialogAbstract.h"


class ADialogMsg : public ADialogAbstract {
public:
  // Methods
  // If only one string is passed, that is the msg, and not the name...
  ADialogMsg(char *theName,char *theMsg=(char *)NULL);
  // Data
protected:
  // Methods
  virtual char *doTheHardWork(char *name,char *msg);
  // Data
};


#endif // GUILIB_AMSGDIALOG_H
