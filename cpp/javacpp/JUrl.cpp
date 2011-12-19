
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <string.h>


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JUrl.h>


////////////////////////////////////////////////////////////////////////////////
//  JUrl Class
////////////////////////////////////////////////////////////////////////////////

JUrl::JUrl(JString tbase, JString tname)
{
  DEBUG_OUT<<"JUrl::JUrl(\""<<tbase<<"\",\""<<tname<<"\")\n";
  base=tbase; name=tname;
}


JInputStream *JUrl::openStream()
{
  DEBUG_OUT<<"JUrl::openStream()\n";
  char fn[256];
  fn[0]=0;
  if(base) strcpy(fn,base);
  DEBUG_OUT<<"strcpy'd...\n";
  DEBUG_OUT<<"fn is now \""<<fn<<"\"\n";
  if(base) strcat(fn,"/");
  DEBUG_OUT<<"/ strcat'd...\n";
  DEBUG_OUT<<"fn is now \""<<fn<<"\"\n";
  strcat(fn,name);
  DEBUG_OUT<<"name strcat'd...\n";
  DEBUG_OUT<<"fn is now \""<<fn<<"\"\n";
  char *n=strdup(fn);
  DEBUG_OUT<<"n is now \""<<n<<"\"\n";
  DEBUG_OUT<<"going to use path \""<<n<<"\"\n";
  JStream *st=new JStream(n);
  return new JInputStream(st);
}



