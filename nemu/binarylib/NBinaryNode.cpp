
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NBinaryNode.h>
#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>


#define LOAD_INTO_MAP 1


////////////////////////////////////////////////////////////////////////////////
//  NBinaryNode Class
////////////////////////////////////////////////////////////////////////////////

NBinaryNode::NBinaryNode(const char *tname,unsigned int tnum,unsigned int ttype,ULONG taddr,ULONG tsize, ULONG toff, unsigned int tflags, BYTE *tdata,ULONG tassoc,ULONG tassoc2) :
   EMapSymbol(tname,tnum,taddr,tsize,ttype,tassoc,tassoc2)
{
  init();
  offsetToData=toff;
  permFlags=tflags;
  data=tdata;
  // redundant...
  name=strdup(tname);
  num=tnum;
  type=ttype;
  size=tsize;
  addr=taddr;
  orig=addr;
  val=tassoc;
  val2=tassoc2;
  DEBUG_OUT<<"New NBinaryNode name="<<name<<"\n";
}


void NBinaryNode::init()
{
  EMapSymbol::init();
  offsetToData=0;
  permFlags=0;
  data=(BYTE *)NULL;
}


/*
void NBinaryNode::print(AWindow dBug)
{
  if(num<10) DEBUG_OUT<<"0";
  DEBUG_OUT<<aDefaultStringHelper.myDec(num)<<": "<<size<<"@"<<addr<<" ";
  DEBUG_OUT<<" (";
  if(permFlags&E_MEM_READ) DEBUG_OUT<<"r"; else DEBUG_OUT<<"-";
  if(permFlags&E_MEM_WRITE) DEBUG_OUT<<"w"; else DEBUG_OUT<<"-";
  if(permFlags&E_MEM_EXEC) DEBUG_OUT<<"x"; else DEBUG_OUT<<"-";
  DEBUG_OUT<<") ";
  switch(type) {
    case N_BIN_NULL: DEBUG_OUT<<"NULL"; break;
    case N_BIN_PROGBITS: DEBUG_OUT<<"PROGBITS"; break;
    case N_BIN_SYMTAB: DEBUG_OUT<<"SYMTAB"; break;
    case N_BIN_STRTAB: DEBUG_OUT<<"STRTAB"; break;
    case N_BIN_RELA: DEBUG_OUT<<"RELA"; break;
    case N_BIN_HASH: DEBUG_OUT<<"HASH"; break;
    case N_BIN_DYNAMIC: DEBUG_OUT<<"DYNAMIC"; break;
    case N_BIN_NOTE: DEBUG_OUT<<"NOTE"; break;
    case N_BIN_NOBITS: DEBUG_OUT<<"NOBITS"; break;
    case N_BIN_REL: DEBUG_OUT<<"REL"; break;
    case N_BIN_SHLIB: DEBUG_OUT<<"SHLIB"; break;
    case N_BIN_DYNSYM: DEBUG_OUT<<"DYNSYM"; break;
    case N_BIN_DEBUG: DEBUG_OUT<<"DEBUG"; break;
    default: DEBUG_OUT<<"Unknown"; break;
  };
  if(name) DEBUG_OUT<<" \""<<name<<"\"";
  DEBUG_OUT<<"\n";
}
*/

