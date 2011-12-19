#ifndef DLIB_DFINDERDATENTRY_H
#define DLIB_DFINDERDATENTRY_H


#include <portable/nickcpp/AList.h>


#ifdef USE_FINDER_DAT
// Used for handling finder.dat files if we find them
class DFinderDatEntry : public AGenericNode
{
public:
  char shortName[DDISK_MAX_NAMELEN];
  char longName[DDISK_MAX_NAMELEN];
  char type[4];
  char creator[4];
protected:
};
#endif // USE_FINDER_DAT


#endif // DLIB_DFINDERDATENTRY_H

