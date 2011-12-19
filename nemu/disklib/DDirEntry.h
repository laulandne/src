#ifndef DLIB_DDIRENTRY_H
#define DLIB_DDIRENTRY_H


#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/AGenericNode.h>

#include <elib/EMemAbstract.h>


// for DDirEntry.type
// (Not all of these will be used)
#define DDISK_TYPE_NONE   0
#define DDISK_TYPE_DIR    1
#define DDISK_TYPE_DEVICE 2
#define DDISK_TYPE_BINARY 3
#define DDISK_TYPE_BASIC  4
#define DDISK_TYPE_TEXT   5
#define DDISK_TYPE_FS     6


// See alib/filedefs.h
// Max filename len we'll handle
#define DDISK_MAX_NAMELEN 256
// Max pathname len we'll handle
#define DDISK_MAX_PATHLEN 256


// NOTE: Any of these fields may be unused in any particular format
class DDirEntry : public AGenericNode
{
public:
  //
  DDirEntry();
  //
  unsigned int signature;  // MUST be 0xface to be valid
  char name[DDISK_MAX_NAMELEN];
  unsigned int nativeType;  // defined by the disk type
  unsigned int type; // as above...may not fit exactly...
  ULONG sizeBlocks;  // In blocks (whatever size they may be)
  ULONG size;  // In bytes (actual size or sizeBlocks*blockSize)
  unsigned int track,sector;  // If known and makes sense...
  unsigned int block;  // If known and makes sense...
  BYTE *data;  // File buffer, if cached (May be larger than actual)
  ULONG curSeek;  // ??? how far we're seeked into this file ???
  bool locked;
  ULONG loadAddress;  // May be 0
  ULONG execAddress;  // May be 0
  //
protected:
  //
};

#endif // DLIB_DDIRENTRY_H

