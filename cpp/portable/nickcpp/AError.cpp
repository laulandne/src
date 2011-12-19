
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include "AError.h"

#include <stdio.h>
#include <stdlib.h>


////////////////////////////////////////////////////////////////////////////////
//  AError Class
////////////////////////////////////////////////////////////////////////////////

AError::AError()
{
  errorFlag=false;
}


bool AError::error()
{
  return errorFlag;
}


void AError::set()
{
  errorFlag=true;
  //exit(5);
}
