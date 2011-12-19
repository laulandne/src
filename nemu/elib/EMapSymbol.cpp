
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <elib/EMapSymbol.h>

#include <portable/nickcpp/ANullStream.h>


///////////////////////////////////////////////////////////////////////////////
//  EMapSynbol Class
///////////////////////////////////////////////////////////////////////////////

EMapSymbol::EMapSymbol()
{
  init();
}


EMapSymbol::EMapSymbol(const char *tname,unsigned int tnum,ULONG taddr,ULONG tsize,unsigned int ttype,ULONG tassoc,ULONG tassoc2)
{
  //DEBUG_OUT<<"EMapSymbol...init\n";
  init();
  //DEBUG_OUT<<"EMapSymbol...name\n";
  if(tname) {
    //DEBUG_OUT<<"Gonna dup.\n";
    setName(strdup(tname));
  } else name="no name";
  //DEBUG_OUT<<"Name is "<<name<<"\n";
  num=tnum;
  addr=taddr;
  orig=addr;
  size=tsize;
  type=ttype;
  val=tassoc;
  val2=tassoc2;
}


void EMapSymbol::init()
{
  name=(char *)NULL;
  demangled=(char *)NULL;
  num=0;
  addr=0;
  size=0;
  type=EMAP_SYMBOL_NORMAL;
  val=0;
  val2=0;
  orig=0;
  misc=0;
}


const char *EMapSymbol::getName()
{
  return name;
}


const char *EMapSymbol::getDemangledName()
{
  if(demangled) return demangled;
  return name;
}


void EMapSymbol::setName(const char *n)
{
  if(!n) {name=NULL; return; }
  name=n;
  unsigned int l,t;
  // First check for name__F* format mangling...
  l=0;  t=0;
  for(t=0;t<strlen(n);t++) {
    if((t>2)&&(n[t]=='F')) {
      if((n[t-1]=='_')&&(n[t-2]=='_')) l=t;
    }
  }
  if(l) {
    l-=2;  // Skip two underscores in front of the F
    DEBUG_OUT<<"\nFound a mangled name: "<<name<<"\n";
    if(name[t]=='_') { t++; l--; }  // skip an underscore...
    if(name[t]=='_') { t++; l--; }  // ...and another one if its there...
    demangled=(char *)malloc(l);
    strncpy(demangled,name+t,l);
    demangled[l]=0;
    DEBUG_OUT<<"Demangled name is "<<demangled<<"\n";
    return;
  }
  // Next check for [_.]Z\dname format mangling...
  // ...also handle removing initial .'s and _'s
  if((name[0]=='_')||(name[0]=='.')) {
    if((name[1]=='Z')&&(isdigit(name[2]))) {
      // Looks like a mangled C++ name...
      // Assume name is less then 100 characters long...
      DEBUG_OUT<<"\nFound a mangled name: "<<name<<"\n";
      l=name[2]-'0';
      t=3;
      if(isdigit(name[3])) {
        l=l*10+(name[3]-'0');
        t++;
      }
      DEBUG_OUT<<"(len of demangled name is "<<l<<")\n";
      if(name[t]=='_') { t++; l--; }  // skip an underscore...
      if(name[t]=='_') { t++; l--; }  // ...and another one if its there...
      demangled=(char *)malloc(l);
      strncpy(demangled,name+t,l);
      demangled[l]=0;
      DEBUG_OUT<<"Demangled name is "<<demangled<<"\n";
    }
    return;
  }
}


bool EMapSymbol::compareName(const char *n)
{
  if(!strcmp(name,n)) return false;  // matched
  else {
    if(strlen(name)<=2) return true;  // give up on 1 or 2 character names
    // a simple match didn't work, maybe the name is mangled?
    if((name[0]=='_')||(name[0]=='.')) {
      if(demangled) { if(!strcmp(demangled,n)) return false; }
      else {
        // Has an underscore, but no Z, maybe just a prepended underscore?
        if(!strcmp(name+1,n)) return false;  // Just drop the underscore...
        if(!strcmp(name+2,n)) return false;  // Ignore first 2 characters.
      }
    }
  }
  return true;  // give up
}
