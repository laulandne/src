#ifndef BINARYLIB_NJAVACLASSDECODER_H
#define BINARYLIB_NJAVACLASSDECODER_H


#include <binarylib/NBinaryDecoder.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EMapSymbol.h>

#include "NJavaAttribute.h"
#include "NJavaCode.h"
#include "NJavaField.h"
#include "NJavaInterface.h"
#include "NJavaMethod.h"


// Extra Java specific symbol types...
#define NJAVA_SYM_CONSTANT 1024
#define NJAVA_SYM_INTERFACE 1025
#define NJAVA_SYM_FIELD 1026
#define NJAVA_SYM_METHOD 1027
#define NJAVA_SYM_METHOD_ATTRIBUTE 1028
#define NJAVA_SYM_ATTRIBUTE 1029
#define NJAVA_SYM_CODE 1030


class NJavaClassDecoder : public NBinaryDecoder
{
public:
  NJavaClassDecoder();
  virtual ~NJavaClassDecoder();
  static bool recognize(unsigned char *buf,char *fname=(char *)NULL);
  virtual char *whichCPU();
  virtual char *whichIO();
  virtual bool verifyOkay();
  virtual char *which() { return (char *)"NJavaClassDecoder"; }
  virtual bool decodeNodes();
protected:
  // these return the full size of the object (including trailing data, etc).
  unsigned int getConstantAtAddr(EMapSymbol *c,ULONG addr);
  unsigned int getInterfaceAtAddr(NJavaInterface *i,ULONG addr);
  unsigned int getFieldAtAddr(NJavaField *f,ULONG addr);
  unsigned int getMethodAtAddr(NJavaMethod *m,ULONG addr);
  unsigned int getAttributeAtAddr(NJavaAttribute *a,ULONG addr);
  unsigned int getCodeAtAddr(NJavaCode *c,ULONG addr);
  unsigned int getNameString(char *s,ULONG addr);
  unsigned int getUTF8Const(char *s,ULONG addr);
  // misc helper methods
  void getStringAtAddr(char *s,unsigned int len,ULONG addr);
  bool readConstants();
  bool readInterfaces();
  bool readFields();
  bool readMethods();
  bool readAttributes();
  // Private Data Members
  ULONG lastUsedAddress;
  EMapSymbol constants[1024];
  unsigned int lastConstant;
  AList methods;
  AList attributes;
  AList codes;
  char s[256];
  unsigned int numConstants;
  unsigned int numInterfaces;
  unsigned int numFields;
  unsigned int numMethods;
  unsigned int numAttributes;
  unsigned int thisClass,superClass;
  unsigned int accessFlags;
  ULONG theOffset;
};


#endif
