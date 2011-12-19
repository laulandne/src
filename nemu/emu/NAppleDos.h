#ifndef EMU_NAPPLEDOS_H
#define EMU_NAPPLEDOS_H


#include <elib/EOSHelper.h>


#define N_APPLE_TRACKBUF_SIZE 10*1024


class NAppleDos : public EOSHelper {
public:
  // public member functions
  NAppleDos(EHandler *parent);
  virtual ~NAppleDos();
  //
  virtual bool systemCall(ULONG loc);
  virtual void reset();
  virtual unsigned int load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes);
  virtual unsigned int save(const char *filename,ULONG loc,unsigned int size);
  virtual unsigned int loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes);
  //
protected:
  // private member functions
  virtual void readTrack(unsigned int tnum);
  virtual void readSector(unsigned int tnum,unsigned int snum, ULONG addr);
  //
  void decodeTrack();
  void clearBuffer();
  // private data members
  unsigned int curSlot,curDrive,curVolume;
  unsigned int a,x,y;
  BYTE trackBuf[N_APPLE_TRACKBUF_SIZE];
};


#endif
