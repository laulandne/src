
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <binarylib/NBFDDecoder.h>
#include <binarylib/NLd.h>
#include <binarylib/NBinaryNode.h>

#include <portable/nickcpp/ANullStream.h>


#ifdef USE_LIBBFD
#include <bfd.h>
#endif // USE_LIBBFD


////////////////////////////////////////////////////////////////////////////////
//  NBFDDecoder Class
////////////////////////////////////////////////////////////////////////////////

#ifdef USE_LIBBFD
/* STATIC */
bool NBFDDecoder::recognize(unsigned char *buffer,char *fname)
{
  DEBUG_OUT<<"NBFDDecoder looking for signature...\n";
  bool ret=false;
  bfd *my_bfd=NULL;
  asection *s=NULL;
  ret=false;
  bfd_init();
  my_bfd=bfd_openr(fname,NULL);
  if(my_bfd) {
    if(bfd_check_format_matches(my_bfd,bfd_object,NULL)) ret=true;
  }
  if(!ret) {
    DEBUG_OUT<<"File is not in format that BFD understands!\n";
    return false;
  }
  return ret;
}
#endif // USE_LIBBFD


#ifdef USE_LIBBFD
NBFDDecoder::NBFDDecoder()
{
  init();
}
#endif // USE_LIBBFD


#ifdef USE_LIBBFD
char *NBFDDecoder::whichCPU()
{
  return "unknown");
}
#endif // USE_LIBBFD


#ifdef USE_LIBBFD
char *NBFDDecoder::whichIO()
{
  return "unix");
}
#endif // USE_LIBBFD


#ifdef USE_LIBBFD
bool NBFDDecoder::verifyOkay()
{
  DEBUG_OUT<<"NBFDDecoder verify...\n";
  if(!checkSanity()) return false;
  bool ret=false;
  ld->reset();
  bfd *my_bfd=NULL;
  asection *s=NULL;
  ret=false;
  bfd_init();
  my_bfd=bfd_openr(fname,NULL);
  if(my_bfd) {
    if(bfd_check_format_matches(my_bfd,bfd_object,NULL)) ret=true;
  }
  if(!ret) {
    DEBUG_OUT<<"File is not in format that BFD understands!\n";
    return false;
  }
  return ret;
}
#endif // USE_LIBBFD

