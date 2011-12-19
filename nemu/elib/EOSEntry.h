#ifndef ELIB_EOSENTRY_H
#define ELIB_EOSENTRY_H


#include <elib/EMemAbstract.h>


#define EOSENTRY_PATCH 0x0001
#define EOSENTRY_DONT_PATCH 0x0000

#define EOSENTRY_PATCH_SIZE 4


class EOSEntry {
public:
  char *name;
  unsigned int origAddr;
  unsigned int actualAddr;
  BYTE call;
  BYTE origByte;
  bool present;
};


#endif

