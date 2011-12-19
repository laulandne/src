#ifndef GUILIB_AFILEREQUESTER_H
#define GUILIB_AFILEREQUESTER_H


#include "ARequestAbstract.h"


class ARequesterFile : public ARequestAbstract {
public:
  // Methods
  // Data
protected:
  // Methods
  virtual char *doTheHardWork(char *name,char *msg);
  // Data
};


#endif // GUILIB_AFILEREQUESTER_H
