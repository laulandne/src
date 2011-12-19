
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include "AStringHelper.h"

#include <iostream>

#include <string.h>
#include <stdlib.h>


#ifdef __MWERKS__
#define strdup(x) strcpy((char *)malloc(strlen(x)+1),x)
#endif // __MWERKS__


AStringHelper aDefaultStringHelper;


////////////////////////////////////////////////////////////////////////////////
//  AStringHelper Class
////////////////////////////////////////////////////////////////////////////////


AStringHelper::AStringHelper()
{
  decMode=true;
}


void AStringHelper::fatal(const char *msg)
{
  std::cerr<<msg<<"\n";
  exit(5);
}


unsigned int AStringHelper::countSpaces(const char *s)
{
  // TODO: Count contiguous spaces as a single space...?
  unsigned int ret=0;
  unsigned int l=strlen(s);
  for(unsigned int t=0;t<l;t++)
    if(s[t]==' ') ret++;
  return ret;
}


char *AStringHelper::removeQuotes(const char *s)
{
  if(!s) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  int len=strlen(s);
  int t=0,tt=0;
  while(t<len) {
    if(s[t]!='\"') {
      tempStringBuf[tt]=s[t];
      tt++;
    }
    t++;
  }
  return tempStringBuf;
}


char *AStringHelper::flipSlashes(const char *s)
{
  if(!s) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  strcpy(tempStringBuf,s);
  int len=strlen(tempStringBuf);
  for(int t=0;t<len;t++) {
    char c=tempStringBuf[t];
    if(c=='\\') tempStringBuf[t]='/';
    if(c=='/') tempStringBuf[t]='\\';
  }
  return tempStringBuf;
}


bool AStringHelper::extensionMatch(const char *filename, const char *extension)
{
  bool ret=false;
  unsigned int l=strlen(filename);
  unsigned int e=strlen(extension);
#ifdef WIN32
  if(!_strnicmp(filename+l-e,extension,e)) ret=true;
#else
  if(!strncasecmp(filename+l-e,extension,e)) ret=true;
#endif
  return ret;
}


// NOTE: This is designed to parse a single line (only one newline)
// TODO: Handle escapes
void AStringHelper::parseString(const char *str, const char *cmd, bool ignoreSpaces,char treatThisAsASpace)
{
  // States:
  // false - base
  // true - in quote, waiting for close quote
  bool state=false;
  unsigned int t;
  theArgc=0;
  for(t=0;t<16;t++) theArgv[t]=(char *)NULL;
  //DEBUG_OUT<<"*** parseString starting\n";
  unsigned int n=0;
  if(cmd) { n++; theArgv[0]=(char *)cmd; theArgc=1; }
  if(!str) return;
  unsigned int arglen=strlen(str);
  if(!arglen) return;
  char *src=strdup(str);
  //DEBUG_OUT<<"parseString ("<<arglen<<" chars): '"<<src<<"'\n";
  for(t=0;t<arglen;t++) {
    if(src[t]=='"') {
      state=!state;
      src[t]=0;
    }
    if((src[t]==treatThisAsASpace)&&treatThisAsASpace) src[t]=0;
    if(src[t]=='\t') src[t]=0;
    if(src[t]==10) src[t]=0;
    if(src[t]==13) src[t]=0;
    if(src[t]==' ') { if(!state) { if(!ignoreSpaces) src[t]=0; } }
  }
  if(strlen(src)) theArgv[n]=src; else n--;
  for(t=0;t<arglen;t++) {
    if(!src[t]) {
      //DEBUG_OUT<<strlen(src+t+1)<<" "<<((char *)src+t+1)<<"\n";
      if(strlen(src+t+1)) { n++; theArgv[n]=src+t+1; }
    }
  }
  theArgc=n+1;
  //DEBUG_OUT<<"theArgc is "<<theArgc<<"\n";
  for(t=0;t<theArgc;t++) {
    if(theArgv[t]) {
      //DEBUG_OUT<<t<<": '"<<theArgv[t]<<"' ("<<((long)strlen(theArgv[t]))<<" chars)\n";
    }
  }
}


unsigned int AStringHelper::parseNumber(const char *str)
{
  if(!str) return 0;
  //if(str[strlen(str)-1]=='h') str[strlen(str)-1]=0;
  unsigned int val,offset=0;
  char *format=(char *)NULL;
  bool tDecMode=decMode;
  if(str[0]=='#') { tDecMode=true;  offset=1; }
  if(str[0]=='$') { tDecMode=false;  offset=1; }
  if((str[0]=='0')&&(str[1]=='x')) { tDecMode=false;  offset=2; }
  DEBUG_OUT<<"Parsing number '"<<str<<"' (tDecMode="<<tDecMode<<")\n";
  if(tDecMode) format=(char *)"%d"; else format=(char *)"%x";
  sscanf(str+offset,format,&val);
  return val;
}


char *AStringHelper::toLower(const char *us)
{
  if(!us) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  strcpy(tempStringBuf,us);
  unsigned int l=strlen(tempStringBuf);
  for(unsigned int t=0;t<l;t++) {
    if((tempStringBuf[t]>='A')&&(tempStringBuf[t]<='Z'))
      tempStringBuf[t]=(char)(tempStringBuf[t]+('a'-'A'));
  }
  return tempStringBuf;
}


char *AStringHelper::toUpper(const char *ls)
{
  if(!ls) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  strcpy(tempStringBuf,ls);
  unsigned int l=strlen(tempStringBuf);
  for(unsigned int t=0;t<l;t++) {
    if((tempStringBuf[t]>='a')&&(tempStringBuf[t]<='z'))
      tempStringBuf[t]=(char)(tempStringBuf[t]-('a'-'A'));
  }
  return tempStringBuf;
}


char *AStringHelper::removeSpaces(const char *s)
{
  if(!s) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  unsigned int l=strlen(s),i=0;
  for(unsigned int t=0;t<l;t++) {
    if(s[t]!=' ') {
      tempStringBuf[i]=s[t];  i++;
    }
  }
  tempStringBuf[i]=0;
  return tempStringBuf;
}


char *AStringHelper::removeSpaces2(const char *s)
{
  if(!s) {
    tempStringBuf2[0]=0;
    return tempStringBuf2;
  }
  unsigned int l=strlen(s),i=0;
  for(unsigned int t=0;t<l;t++) {
    if(s[t]!=' ') {
      tempStringBuf2[i]=s[t];  i++;
    }
  }
  tempStringBuf2[i]=0;
  return tempStringBuf2;
}


/* STATIC */
bool AStringHelper::getPathFromFullName(char *dest,const char *fullname)
{
  dest[0]=0;
  return fillInPathFromFullName(dest,fullname);
}


/* STATIC */
bool AStringHelper::fillInPathFromFullName(char *dest,const char *fullname)
{
  if(!fullname) return false;
  if(!dest) return false;
  unsigned int slen=strlen(fullname);
  unsigned int n=0,t=0;
  for(t=0;t<slen;t++) {
    //DEBUG_OUT<<"Looking at :"<<fullname[t]<<"\n";
    if(fullname[t]=='=') n=t; // hehe...
    if(fullname[t]==':') n=t;
    if(fullname[t]=='/') n=t;
    if(fullname[t]=='\\') n=t;
  }
  // Don't do anything if there isn't a leading path in the pathname...
  if(!n) return false;
  //DEBUG_OUT<<"Looks like the offset is "<<n<<"\n";
  unsigned int dlen=strlen(dest);
  for(t=0;t<n;t++) dest[dlen+t]=fullname[t];
  dest[n+dlen]=0;
  /*
  char tc=fullname[n+1];
  fullname[n+1]=0;
  char *tname=fullname;
  //DEBUG_OUT<<"tname is "<<tname<<"\n";
  fillInString(dest,tname);
  fullname[n+1]=tc;
  */
  return true;
}


/* STATIC */
void AStringHelper::fillInString(char *dest,const char *str)
{
  if(!str) return;
  if(!dest) return;
  unsigned int dlen=strlen(dest);
  unsigned int slen=strlen(str);
  //DEBUG_OUT<<"Len of src is "<<slen<<", dest is "<<dlen<<"\n";
  for(unsigned int t=0;t<slen;t++) dest[dlen+t]=str[t];
  dest[slen+dlen]=0;
}


/* STATIC */
bool AStringHelper::getNameFromFullName(char *dest,const char *fullname)
{
  if(!fullname) return false;
  if(!dest) return false;
  dest[0]=0;
  return fillInNameFromFullName(dest,fullname);
}


/* STATIC */
bool AStringHelper::fillInNameFromFullName(char *dest,const char *fullname)
{
  if(!fullname) return false;
  if(!dest) return false;
  unsigned int slen=strlen(fullname);
  unsigned int n=0;
  for(unsigned int t=0;t<slen;t++) {
    //DEBUG_OUT<<"Looking at :"<<fullname[t]<<"\n";
    if(fullname[t]=='=') n=t; // hehe...
    if(fullname[t]==':') n=t;
    if(fullname[t]=='/') n=t;
    if(fullname[t]=='\\') n=t;
  }
  //DEBUG_OUT<<"Looks like the offset is "<<n<<"\n";
  unsigned int p=1;
  // If there isn't a leading path in the pathname don't skip anything...
  if(!n) p=0;
  const char *tname=fullname+n+p;
  //DEBUG_OUT<<"tname is "<<tname<<"\n";
  fillInString(dest,tname);
  return true;
}


char *AStringHelper::myDec(unsigned long val)
{
  // NOTE: Since this is using a common buffer, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%ld",val);
  return tempStringBuf;
}


// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper::myHex(unsigned long val)
{
  // NOTE: Since this is using a common buffer, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%lx",val);
  return tempStringBuf;
}


// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper::myHex8(unsigned long val)
{
  // NOTE: Since this is using a common buffer, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%08lx",val);
  return tempStringBuf;
}


char *AStringHelper::myHex4(unsigned short val)
{
  // NOTE: Since this is using a common buffer, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%04x",val);
  return tempStringBuf;
}


char *AStringHelper::myHex2(unsigned char val)
{
  // NOTE: Since this is using a common buffer, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%02x",val);
  return tempStringBuf;
}


#ifdef UNICODE
wchar_t *AStringHelper::toWide(char *ss,unsigned int offset)
{
  if(!ss) {
    unicodew[0]=0;  unicodew[1]=0;
    return unicodew;
  }
  unsigned int len=strlen(ss);
  for(unsigned int t=0;t<len;t++) {
    unicodew[offset+t]=ss[t];
  }
  unicodew[offset+len]=0;
  return unicodew+offset;
}
#endif // UNICODE


#ifdef UNICODE
char *AStringHelper::toNarrow(wchar_t *ws,unsigned int offset)
{
  if(!ws) {
    unicodes[0]=0;
    return unicodes;
  }
  unsigned int len=wcslen(ws);
  for(unsigned int t=0;t<len;t++) {
    unicodes[offset+t]=(char)ws[t];
  }
  unicodes[offset+len]=0;
  return unicodes+offset;
}
#endif // UNICODE
