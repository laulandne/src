#ifndef ASTRINGHELPER_H
#define ASTRINGHELPER_H


#include <stdio.h>


#define ATEMPSTRINGBUF_SIZE 256


class AStringHelper
{
public:
  AStringHelper();
  //
  void fatal(const char *msg);
  //
  unsigned int getArgc() const { return theArgc; }
  char *getArgv(unsigned int i) const { return theArgv[i]; }
  char **getAllArgv() const { return (char **)theArgv; }
  //
  bool extensionMatch(const char *filename, const char *extension);
  unsigned int countSpaces(const char *s);
  //
  // Quick convert to hex string
  char *myHex2(unsigned char val);
  char *myHex4(unsigned short val);
  char *myHex8(unsigned long val);
  char *myHex(unsigned long val);
  char *myDec(unsigned long val);
  //
  // These all don't modify the source, but instead return a new modified string
  char *removeQuotes(const char *s);
  char *removeSpaces(const char *s);
  char *removeSpaces2(const char *s);
  char *flipSlashes(const char *s);
  char *toLower(const char *us);
  char *toUpper(const char *us);
  //
    //
  bool getDecMode() { return decMode; }
  void setDecMode(bool tMode=true) { decMode=tMode; }
  void setHexMode() { decMode=false; }
  //
  // Break up a string at whitespace...args are in theArgc and theArgv
  void parseString(const char *str, const char *cmd=(char *)NULL, bool ignoreSpaces=false, char treatThisAsASpace=0);
  unsigned int parseNumber(const char *str);
  //
  // NOTE: These return false if they didn't find anything to do.
  static bool getPathFromFullName(char *dest,const char *fullname);
  static bool fillInPathFromFullName(char *dest,const char *fullname);
  static void fillInString(char *dest,const char *str);
  static bool getNameFromFullName(char *dest,const char *fullname);
  static bool fillInNameFromFullName(char *dest, const char *fullname);
  //
protected:
  // Private data members
  bool decMode;
  unsigned int theArgc;
  char *theArgv[16];
  char tempStringBuf[ATEMPSTRINGBUF_SIZE];
  char tempStringBuf2[ATEMPSTRINGBUF_SIZE];
};


extern AStringHelper aDefaultStringHelper;


#endif // ASTRINGHELPER_H
