#ifndef GUILIB_ASKSTRINGDIALOG_H
#define GUILIB_ASKSTRINGDIALOG_H


#include "ADialogAbstract.h"


// Default button labels will be "OK" and "Cancel"
class ADialogAskString : public ADialogAbstract {
public:
  // Methods
  ADialogAskString(char *theName=(char *)NULL,char *theMsg=(char *)NULL);
  // Data
protected:
  // Methods
  // Data
};


#endif // ALIB_ASKSTRINGDIALOG_H
