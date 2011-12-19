
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include "ADataSourceAbstract.h"


////////////////////////////////////////////////////////////////////////////////
//  ADataSourceAbstract Class
////////////////////////////////////////////////////////////////////////////////

ADataSourceAbstract::ADataSourceAbstract()
{
  init();
}


ADataSourceAbstract::ADataSourceAbstract(char *tname,char *ttable,char *thost,
  char *tuser,char *tpass)
{
  init();
  connected=false;
  name=tname;
  host=thost;
  user=tuser;
  table=ttable;
}


void ADataSourceAbstract::init()
{
  connected=true;
  name=(char *)NULL;
  host=(char *)NULL;
  user=(char *)NULL;
  table=(char *)NULL;
}


ADataSourceAbstract::~ADataSourceAbstract()
{
  // Nothing to do
}


void ADataSourceAbstract::kill()
{
  // Nothing to do
}


void ADataSourceAbstract::insert(void *n)
{
  // Nothing to do
}


void ADataSourceAbstract::append(void *n)
{
  // Nothing to do
}


void* ADataSourceAbstract::remove()
{
  // Because there's nothing there...
  return NULL;
}


void* ADataSourceAbstract::info()
{
  // Because there's nothing there...
  return NULL;
}


void ADataSourceAbstract::advance()
{
  // Nothing to do
}


void ADataSourceAbstract::reset()
{
  // Nothing to do
}


void ADataSourceAbstract::sort()
{
  // Nothing to do
}


bool ADataSourceAbstract::atEnd()
{
  // Because there's nothing there...
  return true;
}


bool ADataSourceAbstract::isEmpty()
{
  // Because there's nothing there...
  return true;
}


bool ADataSourceAbstract::isSorted()
{
  // Because there's nothing there...
  return true;
}


bool ADataSourceAbstract::placeBookmark()
{
  // Because there's nothing there...
  return false;
}


bool ADataSourceAbstract::gotoBookmark()
{
  // Because there's nothing there...
  return false;
}


bool ADataSourceAbstract::isConnected()
{
  // Because we're always "connected"...and always local...by default.
  return true;
}





