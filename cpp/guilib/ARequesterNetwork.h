#ifndef GUILIB_ANETWORKREQUESTER_H
#define GUILIB_ANETWORKREQUESTER_H


#include "ARequestAbstract.h"


class AHost;


class ARequesterNetwork : public ARequestAbstract {
public:
  // Methods
  AHost *getSelectedHost();
  // Data
protected:
  // Methods
  virtual void init();
  virtual char *doTheHardWork(char *name,char *msg);
  // Data
  AHost *chosenHost;
};


#endif // GUILIB_ANETWORKREQUESTER_H
