#ifndef BINARYLIB_NXO65DECODER_H
#define BINARYLIB_NXO65DECODER_H


//#include <alib/abaseclass.h>
#include <binarylib/NBinaryDecoder.h>


/*
// Doesn't belong here, but will be until it gets its own file, etc.
class NXO65Decoder : public NBinaryDecoder
{
public:
  NXO65Decoder();
  static bool recognize(unsigned char *buf,char *fname=(char *)NULL);
  virtual char *whichCPU();
  virtual char *whichIO();
  virtual bool verifyOkay();
  virtual char *which() { return (char *)"NXO65Decoder"; }
  //virtual ULONG getImageOffset();
  virtual bool decodeNodes();
protected:
  void init();
  bool readHeader();
  void handleStrings();
  void handleImports();
  void handleExports();
  void handleSegments();
  void handleFrags(unsigned int num,unsigned int offset);
  //
  ULONG optOff,optSize,optNum;
  ULONG fileOff,fileSize,fileNum;
  ULONG segOff,segSize,segNum;
  ULONG impOff,impSize,impNum;
  ULONG expOff,expSize,expNum;
  ULONG debugOff,debugSize,debugNum;
  ULONG lineOff,lineSize,lineNum;
  ULONG stringOff,stringSize,stringNum;
  ULONG assOff,assSize,assNum;
  ULONG scopeOff,scopeSize,scopeNum;
};
*/


#endif
