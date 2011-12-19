#ifndef GUILIB_AREQUESTWINDOW_H
#define GUILIB_AREQUESTWINDOW_H


#include "ADialogAbstract.h"


class ARequestAbstract : public ADialogAbstract {
public:
  // Methods
  ARequestAbstract();
  virtual ~ARequestAbstract();
  // Data
protected:
  // Methods
  // Data
#ifdef ASYS_WIN16
  HINSTANCE commDlgHandle;
#endif // ASYS_WIN16
};


#endif // GUILIB_AREQUESTWINDOW_H
