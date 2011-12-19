#ifndef ALIB_AWADDECODER_H
#define ALIB_AWADDECODER_H


#include "AMapDecoder.h"


#define AWADDECODER_UNIT 1.0/65536.0


#define AWADDECODER_PLAYER_HEIGHT 56
#define AWADDECODER_PLAYER_RADIUS 16


// Linedef attr's
#define AWADDECODER_LINEDEF_BLOCKBOTH 0x1
#define AWADDECODER_LINEDEF_BLOCKMONSTERS  0x2
#define AWADDECODER_LINEDEF_TWOSIDED  0x4
#define AWADDECODER_LINEDEF_UNPEGUPPER  0x8
#define AWADDECODER_LINEDEF_UNPEGLOWER  0x10
#define AWADDECODER_LINEDEF_SECRET  0x20
#define AWADDECODER_LINEDEF_BLOCKSOUND  0x40
#define AWADDECODER_LINEDEF_NOAUTOMAP  0x80
#define AWADDECODER_LINEDEF_ALWAYSAUTOMAP  0x100
#define AWADDECODER_LINEDEF_MORETHANONCE  0x200
#define AWADDECODER_LINEDEF_WHENPLAYERUSES  0x400
#define AWADDECODER_LINEDEF_WHENMONSTERCROSSES  0x800
#define AWADDECODER_LINEDEF_WHENPROJECTILEHITS  0xc00
#define AWADDECODER_LINEDEF_WHENPLAYERBUMPS  0x1000
#define AWADDECODER_LINEDEF_WHENPROJECTILECROSSES  0x1400
#define AWADDECODER_LINEDEF_WHENPLAYERPASSES  0x1800
#define AWADDECODER_LINEDEF_PLAYERANDMONSTERS  0x2000
#define AWADDECODER_LINEDEF_BLOCKALL  0x8000


// Thing attr's
#define AWADDECODER_THING_LEVEL1_2  0x1
#define AWADDECODER_THING_LEVEL3  0x2
#define AWADDECODER_THING_LEVEL4_5  0x4
#define AWADDECODER_THING_DEAF  0x8
#define AWADDECODER_THING_MULTIONLY  0x10


struct AWADVertexI {
  int16_t x,y;
};


struct AWADSeg {
  unsigned int start,end,angle,top,side,offset;
};


struct AWADSector {
  unsigned int floorht,ceilinght,brightness,special,tag;
  char floortx[9];
  char ceilingtx[9];
};


struct AWADSSector {
  unsigned int numSegs,firstSeg;
};


struct AWADLineDef {
  unsigned int lfrom,lto,attr,special,tag,side1,side2;
};


struct AWADSideDef {
  unsigned int x,y,sector;
  char upper[9];
  char lower[9];
  char normal[9];
};


struct AWADThing {
  unsigned int x,y,angle,type,attr;
};


struct AWADNode {
  unsigned int xstart,ystart,dx,dy;
  unsigned int yup1,ylower1,xlower1,xup1;
  unsigned int yup2,ylower2,xlower2,xup2;
  unsigned int right,left;
};


// Doom 1 and 2 style WAD
class AWADDecoder : public AMapDecoder
{
public:
  //
  AWADDecoder(const char *fname);
  static bool recognize(uint8_t *str);
  //
  virtual unsigned int getNumMaps();
  virtual void setMapNumber(unsigned int n);
  virtual bool readTheCurrentMap();
  //
protected:
  //
  void init();
  void readObjects();
  void readDirectory();
  void buildMapArray();
  bool readMapEntry(unsigned int entry);
  bool isNameAMap(char *name);
  //
  void readVertices(unsigned int entry);
  void readSegs(unsigned int entry);
  void readSectors(unsigned int entry);
  void readSSectors(unsigned int entry);
  void readLineDefs(unsigned int entry);
  void readSideDefs(unsigned int entry);
  void readReject(unsigned int entry);
  void readBlockMap(unsigned int entry);
  void readThings(unsigned int entry);
  void readNodes(unsigned int entry);
  //
  void buildMapMesh();
  //
  bool pwad;
  uint32_t dirOffset;
  unsigned long numEntries;
  char **entryNames;  // ptr to numEntries strings...
  uint32_t *entryOffsets;  // ptr to numEntries ulongs...
  uint32_t *entryLens;  // ptr to numEntries ulongs...
  uint32_t mapEntries[64];  // naievely assume there are less than 65 maps...
  //
  unsigned int numVertices;
  AVertexF3D **mapVerticesF;
  AWADVertexI **mapVerticesI;
  unsigned int numSegs;
  AWADSeg **mapSegs;
  unsigned int numSectors;
  AWADSector **mapSectors;
  unsigned int numSSectors;
  AWADSSector **mapSSectors;
  unsigned int numLineDefs;
  AWADLineDef **mapLineDefs;
  unsigned int numSideDefs;
  AWADSideDef **mapSideDefs;
  unsigned int numThings;
  AWADThing **mapThings;
  unsigned int numNodes;
  AWADNode **mapNodes;
  //
  AMesh3D *mapMesh;
  unsigned int playerX,playerY,playerAngle;
};


#endif // ALIB_AWADDECODER_H

