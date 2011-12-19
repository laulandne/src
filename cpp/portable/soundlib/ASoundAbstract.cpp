
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include "ASoundAbstract.h"
#include "ASound.h"
#include "ASong.h"


////////////////////////////////////////////////////////////////////////////////
//  ASoundAbstract Class
////////////////////////////////////////////////////////////////////////////////

ASoundAbstract::ASoundAbstract()
{
  DEBUG_OUT<<"ASoundAbstract::ASoundAbstract()\n";
  init();
}


void ASoundAbstract::init()
{
  name=(char *)NULL;
  okayDokey=false;
}


ASoundAbstract::~ASoundAbstract()
{
  DEBUG_OUT<<"ASoundAbstract::~ASoundAbstract()\n";
}
