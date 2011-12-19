#ifndef GUILIB_ADISPLAYMODEREQUESTER_H
#define GUILIB_ADISPLAYMODEREQUESTER_H


#include "ARequestAbstract.h"
#include "ADisplayMode.h"


class ARequesterDisplayMode : public ARequestAbstract {
public:
  // Methods
  ADisplayMode *getSelectedMode();
  // Data
protected:
  // Methods
  virtual void init();
  virtual char *doTheHardWork(char *name,char *msg);
  // Data
  ADisplayMode *chosenMode;
};


#endif // GUILIB_ADISPLAYMODEREQUESTER_H
