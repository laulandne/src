#ifndef EMU_NAPPLEIO_H
#define EMU_NAPPLEIO_H


#include <elib/EMemIO.h>


class DControllerDisk2;
class DControllerIWM;
class EIOScc8530;
class EIOVia6522;
class NAppleHandler;


class NAppleIO : public EMemIO {
public:
  // Public Member Functions
  NAppleIO(NAppleHandler *parent);
  virtual ~NAppleIO();
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  //
  BYTE write8apple3(ULONG addr,BYTE data);
  BYTE read8apple3(ULONG addr);
  // Public Data Members
private:
  // Private Member Functions
  //
  BYTE strobe(ULONG address,BYTE val);
  BYTE read8_slot(unsigned int num,unsigned int ad);
  BYTE write8_slot(unsigned int num,unsigned int ad,BYTE val);
  BYTE strobe8_slot(unsigned int num,unsigned int ad,BYTE val);
  void setNewVideoReg(BYTE val);
  void setSpeedReg(BYTE val);
  void setStateReg(BYTE val);
  BYTE readNewVideoReg();
  BYTE readSpeedReg();
  BYTE readStateReg();
  // Private Data Members
  DControllerDisk2 *disk2;
  DControllerIWM *smartDisk;
  EIOScc8530 *scc;
  bool slotHandled[8];
  bool sRamrd,sRamwrt,sAltzp;
  bool sText,sHires,sMixed,sPage2;
  bool srdrom,slcbnk2,srombank,sintcxrom;
  bool sDhires,sIoudis;
  bool s80col,s80store;
  unsigned int ann0,ann1,ann2,ann3;
  BYTE newVideoReg,speedReg;
  EIOVia6522 *via1,*via2;
};


#endif
