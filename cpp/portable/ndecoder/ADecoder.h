#ifndef DECODERLIB_ADECODER_H
#define DECODERLIB_ADECODER_H


#define int8_t unsigned char
#define uint8_t unsigned char
#define int16_t unsigned short
#define uint16_t unsigned short
#if __LP64__
#define int32_t unsigned int
#define uint32_t unsigned int
#else
#define int32_t unsigned long
#define uint32_t unsigned long
#endif // __LP64__


#include <portable/nickcpp/AError.h>
#include <portable/nickcpp/AStringHelper.h>


class ADecoder
{
public:
  ADecoder(const char *fname);
  virtual ~ADecoder();
  //
  bool isScene() { return isAScene; }
  bool isMesh() { return isAMesh; }
  bool isMovie() { return isAMovie; }
  bool isImage() { return isAnImage; }
  bool isSound() { return isASound; }
  bool isSong() { return isASong; }
  //
  AError err;
protected:
  // Private member function
  virtual void init();
  virtual void header();
  virtual void body();
  void openFile(const char *imgFname);
  void byteSwap(uint16_t *val);
  void byteSwap(uint32_t *val);
  unsigned long iffFindChunk(unsigned long chunkOffset,char *findName,bool bigEndian);
  unsigned long tagFindChunk(unsigned long chunkOffset,uint16_t tag,bool bigEndian);
  unsigned long findChunk(unsigned long chunkOffset,int nameSize,uint32_t tagName,bool bigEndian,bool fixOdd);
  unsigned long iffCountChunks(unsigned long chunkOffset,char *findName,bool bigEndian);
  unsigned long tagCountChunks(unsigned long chunkOffset,uint16_t tag,bool bigEndian);
  unsigned long countChunks(unsigned long chunkOffset,int nameSize,uint32_t tagName,bool bigEndian,bool fixOdd);
  int8_t readSByte();
  uint8_t readByte();
  uint32_t readLongL();
  uint32_t readLongB();
  int16_t readSShortL();
  uint16_t readShortL();
  uint16_t readShortB();
  float readFloatL();
  float readFloatB();
  void readNullString(char *s,unsigned int limit);
  void readString(char *s,unsigned int len);
  unsigned int bits2Num(unsigned int bits);
  static bool recognizeTextFile(uint8_t *buf);
  // Private data members
  FILE *fd;
  const char *filename;
  bool compressed;
  bool littleEndian;
  // Yes, this is a stupid way to do this...
  bool isAnImage, isAMovie, isAMesh, isAScene, isASound, isASong;
  AStringHelper stringHelp;
};


#endif // DECODERLIB_ADECODER
