
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#define CHECK_SIZE 1


#include <elib/ERom.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


///////////////////////////////////////////////////////////////////////////////
//  ERom Class
///////////////////////////////////////////////////////////////////////////////

ERom::ERom(char *qname,ULONG addr,BYTE *data,unsigned int tsize) : ERam(addr,tsize)
{
  DEBUG_OUT<<"ERom("<<qname<<","<<addr<<",data,"<<tsize<<")...\n";
  char *tname=aDefaultStringHelper.toLower(qname);
  name=strdup(tname);
  DEBUG_OUT<<"Rom name is "<<name<<"\n";
  //init();
  if(!data) { DEBUG_OUT<<"bad pointer passed to copy!\n";  err.set(); return; }
  //DEBUG_OUT<<"Copying rom data...";
  for(unsigned int t=0;t<memsize;t++) {
    //if(!(t&0xff)) DEBUG_OUT<<t<<"...";
    mem[t]=data[t];
  }
  //DEBUG_OUT<<"\n";
  letter='R';
}


ERom::ERom(ULONG addr,char *fname,ULONG offset,char *mname, bool flag) : ERam(addr,0)
{
  DEBUG_OUT<<"ERom("<<addr<<","<<fname<<","<<offset<<","<<mname<<")...\n";
  //init();
  char *tname=(char *)NULL;
  if(mname) tname=strdup(mname); else tname=strdup(fname);
  tname=aDefaultStringHelper.toLower(tname);
  name=strdup(tname);
  DEBUG_OUT<<"Rom name is "<<name<<"\n";
  bool err2;
  unsigned int romsize=0;
  FILE *fd=fopen(fname,"rb");
  if(!fd) fd=fopen(aDefaultStringHelper.removeQuotes(fname),"rb");
  if(!fd) fd=fopen(aDefaultStringHelper.flipSlashes(fname),"rb");
  if(fd) {
    if(fseek(fd,0,SEEK_END)) DEBUG_OUT<<"Couldn't fseek end!\n";
    romsize=(unsigned int)(ftell(fd)-offset);
    if(fseek(fd,0,SEEK_SET)) DEBUG_OUT<<"Couldn't fseek set!\n";
    resize(romsize);
    if(offset) {
      BYTE foo;
      for(unsigned int tt=0;tt<offset;tt++) err2=fread((BYTE *)&foo,1,1,fd);
    }
    DEBUG_OUT<<"Loading "<<fname<<"@"<<addr<<" ";
    //unsigned int num=newsize/1024;
    //if((num*1024)<newsize) num++;
    err2=fread(mem,romsize,1,fd);
    DEBUG_OUT<<romsize<<" bytes.\n";
  }
  else { DEBUG_OUT<<"Couldn't load rom from file!\n"; err.set(); }
  if(fd) fclose(fd);
  letter='R';
}


BYTE ERom::write8(ULONG addr,BYTE data)
{
  //DEBUG_OUT<<"#";
  //
  return 0;
}


BYTE ERom::reallyWrite8(ULONG addr,BYTE data)
{
  DEBUG_OUT<<"ERom::reallyWrite8("<<addr<<","<<(int)data<<")...\n";
  return ERam::write8(addr,data);
}
