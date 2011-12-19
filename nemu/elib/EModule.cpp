
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  EModule Class
////////////////////////////////////////////////////////////////////////////////

EModule::EModule(char *tname) :
   EMapSymbol(tname,0,0,0,0,0,0)
{
  init();
  name=tname;
}


EModule::~EModule()
{
  symbols.kill();
  strings.kill();
  nodes.kill();
}


void EModule::init()
{
  EMapSymbol::init();
  cpuType=(char *)NULL;
  ioType=(char *)NULL;
  alreadyUsedZero=false;
  highestAddressUsed=0;
  isUnknown=true;
  needsReloc=false;
  LSB=false;
  dynamic=false;
  bitness=32;
  shouldDemandPage=false;
  entry=0;
}


EMapSymbol *EModule::findExactSymbol(ULONG addr)
{
  symbols.reset();
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      if(s->addr==addr) {
        return s;
      }
      symbols.advance();
    }
  }
  return (EMapSymbol *)NULL;
}


EMapSymbol *EModule::findNearestSymbol(ULONG addr)
{
  symbols.reset();
  EMapSymbol *ret=(EMapSymbol *)symbols.info();
  unsigned int dist=0xffffffffL;
  unsigned int newdist=0xffffffffL;
  if(!ret) return (EMapSymbol *)NULL;
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      newdist=(unsigned int)(s->addr-addr);
      //DEBUG_OUT<<"findNearestSymbol: newdist is "<<newdist<<"\n";
      if(newdist<dist)  {
        //DEBUG_OUT<<"findNearestSymbol: we like that better...\n";
        ret=s;
        dist=newdist;
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::findNearestSymbolOfType(ULONG addr,unsigned int type)
{
  symbols.reset();
  EMapSymbol *ret=(EMapSymbol *)symbols.info();
  unsigned int dist=0xffffffffL;
  unsigned int newdist=0xffffffffL;
  if(!ret) return (EMapSymbol *)NULL;
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      newdist=(unsigned int)(s->addr-addr);
      //DEBUG_OUT<<"findNearestSymbolOfType: newdist is "<<newdist<<"\n";
      if((s->type==type)&&(newdist<dist))  {
        //DEBUG_OUT<<"findNearestSymbolOfType: we like that better...\n";
        ret=s;
        dist=newdist;
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::findNearestPrevSymbol(ULONG addr)
{
  symbols.reset();
  EMapSymbol *ret=(EMapSymbol *)symbols.info();
  unsigned int dist=0xffffffffL;
  unsigned int newdist=0xffffffffL;
  if(!ret) return (EMapSymbol *)NULL;
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      newdist=(unsigned int)(addr-s->addr);
      //DEBUG_OUT<<"findNearestPrevSymbol: newdist is "<<newdist<<"\n";
      if((newdist<dist)&&(!(s->addr>addr)))  {
        //DEBUG_OUT<<"findNearestPrevSymbol: we like that better...\n";
        ret=s;
        dist=newdist;
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::findNearestPrevSymbolOfType(ULONG addr,unsigned int type)
{
  symbols.reset();
  EMapSymbol *ret=(EMapSymbol *)symbols.info();
  unsigned int dist=0xffffffffL;
  unsigned int newdist=0xffffffffL;
  if(!ret) return (EMapSymbol *)NULL;
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      newdist=(unsigned int)(addr-s->addr);
      //DEBUG_OUT<<"findNearestPrevSymbolOfType: newdist is "<<newdist<<"\n";
      if((s->type==type)&&(newdist<dist)&&(!(s->addr>addr)))  {
        //DEBUG_OUT<<"findNearestPrevSymbolOfType: we like that better...\n";
        ret=s;
        dist=newdist;
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupNextSymbolByName(char *str)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  if(!str) return ret;
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      //DEBUG_OUT<<"Comparing "<<str<<" and "<<s->getName()<<"...\n";
      if(!s->compareName(str)) {
        //DEBUG_OUT<<"Got it!\n";
        ret=s; reading=false;
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupNextClosestSymbolByName(char *str)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  if(!str) return ret;
  unsigned int clen=strlen(str);
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      //DEBUG_OUT<<"Comparing "<<str<<" and "<<s->getName()<<"...\n";
      if(!strncmp(s->getName(),str,clen)) {
        //DEBUG_OUT<<"Got it!\n";
        ret=s; reading=false;
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupSymbolByName(char *str)
{
  symbols.reset();
  return lookupNextSymbolByName(str);
}


EMapSymbol *EModule::lookupClosestSymbolByName(char *str)
{
  symbols.reset();
  return lookupNextClosestSymbolByName(str);
}


EMapSymbol *EModule::lookupSymbolByNameAndNum(char *str,unsigned int num)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  if(!str) return ret;
  symbols.reset();
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      //DEBUG_OUT<<"Comparing "<<str<<" and "<<s->getName()<<"...\n";
      if(!s->compareName(str)) {
        //DEBUG_OUT<<"Comparing "<<num<<" and "<<s->getNum()<<"\n";
        if(s->num==num) {
          //DEBUG_OUT<<"num matched too!\n";
          ret=s; reading=false;
        }
        //else DEBUG_OUT<<"num didn't match\n";
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupSymbolByNameAndType(char *str,unsigned int t)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  if(!str) return ret;
  symbols.reset();
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      //DEBUG_OUT<<"Comparing "<<str<<" and "<<s->getName()<<"...\n";
      if(!s->compareName(str)) {
        //DEBUG_OUT<<"Comparing "<<num<<" and "<<s->getNum()<<"\n";
	    if(s->type==t) {
		  //DEBUG_OUT<<"type matched too!\n";
		  ret=s; reading=false;
		}
		// else DEBUG_OUT<<"type didn't match\n";
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupSymbolByNameAndNumAndType(char *str,unsigned int num,unsigned int t)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  if(!str) return ret;
  symbols.reset();
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      //DEBUG_OUT<<"Comparing "<<str<<" and "<<s->getName()<<"...\n";
      if(!s->compareName(str)) {
        //DEBUG_OUT<<"Comparing "<<num<<" and "<<s->getNum()<<"\n";
        if(s->num==num) {
          //DEBUG_OUT<<"num matched too!\n";
		  if(s->type==t) {
            //DEBUG_OUT<<"type matched too!\n";
            ret=s; reading=false;
		  }
		  // else DEBUG_OUT<<"type didn't match\n";
        }
        //else DEBUG_OUT<<"num didn't match\n";
      }
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupSymbolByNumAndType(unsigned int num,unsigned int t)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  symbols.reset();
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
	  //DEBUG_OUT<<"Comparing "<<num<<" and "<<s->getNum()<<"\n";
	  if(s->num==num) {
		//DEBUG_OUT<<"num matched too!\n";
		if(s->type==t) {
		  //DEBUG_OUT<<"type matched too!\n";
		  ret=s; reading=false;
		}
		// else DEBUG_OUT<<"type didn't match\n";
      }
	  //else DEBUG_OUT<<"num didn't match\n";
      symbols.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupStringByNum(unsigned int num)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  strings.reset();
  bool reading=true;
  while(reading) {
    if(strings.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)strings.info();
	  //DEBUG_OUT<<"Comparing "<<num<<" and "<<s->getNum()<<"\n";
	  if(s->num==num) {
		//DEBUG_OUT<<"num matched!\n";
		ret=s; reading=false;
      }
	  //else DEBUG_OUT<<"num didn't match\n";
      strings.advance();
    }
  }
  return ret;
}


unsigned int EModule::countSymbolWithName(char *str)
{
  unsigned int count=0;
  if(!str) return count;
  symbols.reset();
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      if(!s->compareName(str)) {
        count++;
      }
      symbols.advance();
    }
  }
  return count;
}


unsigned int EModule::countSymbolWithType(unsigned int t)
{
  unsigned int count=0;
  symbols.reset();
  bool reading=true;
  while(reading) {
    if(symbols.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)symbols.info();
      if(s->type==t) {
        count++;
      }
      symbols.advance();
    }
  }
  return count;
}


void EModule::calcAddrAndSizeFromNodes()
{
  unsigned int lowest=0,highest=0;
  nodes.reset();
  while(!nodes.atEnd()) {
    NBinaryNode *n=(NBinaryNode *)nodes.info();
	if(n->addr<lowest) lowest=n->addr;
	if((n->addr+n->size)>highest) highest=(n->addr+n->size);
    nodes.advance();
  }
  size=highest-lowest;
  addr=lowest;
}


EMapSymbol *EModule::lookupStringByName(char *name)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  strings.reset();
  bool reading=true;
  while(reading) {
    if(strings.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)strings.info();
	  //DEBUG_OUT<<"Comparing "<<num<<" and "<<s->getNum()<<"\n";
	  if(!s->compareName(name)) {
		//DEBUG_OUT<<"num matched!\n";
		ret=s; reading=false;
      }
	  //else DEBUG_OUT<<"num didn't match\n";
      strings.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupNextNodeByName(char *str)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  if(!str) return ret;
  bool reading=true;
  while(reading) {
    if(nodes.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)nodes.info();
      //DEBUG_OUT<<"Comparing "<<str<<" and "<<s->getName()<<"...\n";
      if(!s->compareName(str)) {
        //DEBUG_OUT<<"Got it!\n";
        ret=s; reading=false;
      }
      nodes.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::lookupNodeByName(char *name)
{
  nodes.reset();
  return lookupNextNodeByName(name);
}


EMapSymbol *EModule::lookupNodeByNameAndNum(char *str,unsigned int num)
{
  EMapSymbol *ret=(EMapSymbol *)NULL;
  if(!str) return ret;
  nodes.reset();
  bool reading=true;
  while(reading) {
    if(nodes.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)nodes.info();
      //DEBUG_OUT<<"Comparing "<<str<<" and "<<s->getName()<<"...\n";
      if(!s->compareName(str)) {
        //DEBUG_OUT<<"Comparing "<<num<<" and "<<s->getNum()<<"\n";
        if(s->num==num) {
          //DEBUG_OUT<<"num matched too!\n";
          ret=s; reading=false;
        }
        //else DEBUG_OUT<<"num didn't match\n";
      }
      nodes.advance();
    }
  }
  return ret;
}


EMapSymbol *EModule::findNearestPrevNode(ULONG addr)
{
  nodes.reset();
  EMapSymbol *ret=(EMapSymbol *)nodes.info();
  unsigned int dist=0xffffffffL;
  unsigned int newdist=0xffffffffL;
  if(!ret) return (EMapSymbol *)NULL;
  bool reading=true;
  while(reading) {
    if(nodes.atEnd()) reading=false;
    else {
      EMapSymbol *s=(EMapSymbol *)nodes.info();
      newdist=(unsigned int)(addr-s->addr);
      //DEBUG_OUT<<"findNearestPrevNode: newdist is "<<newdist<<"\n";
      if((newdist<dist)&&(!(s->addr>addr)))  {
        //DEBUG_OUT<<"findNearestPrevNode: we like that better...\n";
        ret=s;
        dist=newdist;
      }
      nodes.advance();
    }
  }
  return ret;
}


void EModule::setHighestAddressUsed(ULONG t)
{
  DEBUG_OUT<<"setHighestAddressUsed("<<t<<")...\n";
  highestAddressUsed=t;
}

