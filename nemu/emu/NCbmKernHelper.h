#ifndef EMU_NCBMKERNHELPER_H
#define EMU_NCBMKERNHELPER_H


#include <elib/EOSHelper.h>


class NCbmKernHelper : public EOSHelper {
public:
  NCbmKernHelper(NCbmHandler *parent);
  virtual ~NCbmKernHelper();
  //
  virtual bool systemCall(ULONG loc);
  virtual void reset();
  virtual unsigned int load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes);
  virtual unsigned int save(const char *filename,ULONG loc,unsigned int size);
  virtual unsigned int loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes);
  //
protected:
  //
  void doLoad();
  void doDOSCommand();
  char ascii2petscii(char c);
  //
  unsigned int chanDev[NCBM_NUM_CHANNELS];
  unsigned int chanState[NCBM_NUM_CHANNELS];
  unsigned int chanUse[NCBM_NUM_CHANNELS];
  unsigned int chanSec[NCBM_NUM_CHANNELS];
  unsigned int curDev,curChan,curSecond;
};


#endif

