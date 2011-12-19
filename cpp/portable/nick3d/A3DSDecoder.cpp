
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream



#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>
#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/ATextCoord3D.h>
#include <portable/nick3d/AMesh3D.h>
#include <portable/nick3d/ATexture3D.h>

#include "A3DSDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  A3DSDecoder Class
////////////////////////////////////////////////////////////////////////////////

A3DSDecoder::A3DSDecoder(const char *fname) : AMeshDecoder(fname)
{
  init();
  fixTextCoords=true;
  openFile(fname);
  readObjects();
}


/* STATIC */
bool A3DSDecoder::recognize(uint8_t *str)
{
  bool ret=false;
  short *tag=(short *)str;
  if(*tag==0x4d4d) ret=true;
  return ret;
}


void A3DSDecoder::readObjects()
{
  DEBUG_OUT<<"A3DSDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,2L,SEEK_SET);
  unsigned long chunkOffset=6;
  if(tagFindChunk(chunkOffset,0x0002,false)) readEdVer();
  if(tagFindChunk(chunkOffset,0x3d3d,false)) readEditor();
}


// Chunk 0x0002
void A3DSDecoder::readEdVer()
{
  DEBUG_OUT<<"A3DSDecoder::readEdVer()...\n";
  // ignore for now, assume we understand the format
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


// Chunk 0x3d3d
void A3DSDecoder::readEditor()
{
  DEBUG_OUT<<"A3DSDecoder::readEditor()...\n";
  unsigned long placeHolder=ftell(fd);
  unsigned long chunkOffset=placeHolder;
  unsigned int nmatChunks=tagCountChunks(chunkOffset,0xafff,false);
  DEBUG_OUT<<"There are "<<nmatChunks<<" mat chunks.\n";
  unsigned int i;
  for(i=0;i<nmatChunks;i++) {
	unsigned long csize=0;
	  if(csize=tagFindChunk(chunkOffset,0xafff,false)) {
	    chunkOffset=ftell(fd);
      readMaterialChunk();
	  }
	  chunkOffset+=csize;
  }
  chunkOffset=placeHolder;
  unsigned int nobjChunks=tagCountChunks(chunkOffset,0x4000,false);
  DEBUG_OUT<<"There are "<<nobjChunks<<" obj chunks.\n";
  nfaces=0;  nverts=0;
  verts=NULL;
  for(i=0;i<nobjChunks;i++) {
	unsigned long csize=0;
	  if(csize=tagFindChunk(chunkOffset,0x4000,false)) {
	    chunkOffset=ftell(fd);
      readObjectChunk();
	  }
	  chunkOffset+=csize;
  }
}


// Chunk 0xafff
bool A3DSDecoder::readMaterialChunk()
{
  DEBUG_OUT<<"A3DSDecoder::readMaterialChunk()...\n";
  bool ret=true;
  unsigned long chunkOffset=ftell(fd);
  if(tagFindChunk(chunkOffset,0xa000,false)) {
    char mname[256];
    readNullString(mname,256);
    DEBUG_OUT<<"Material name: "<<mname<<"\n";
  }
  if(tagFindChunk(chunkOffset,0xa010,false)) readMatAmbient();
  if(tagFindChunk(chunkOffset,0xa020,false)) readMatDiffuse();
  if(tagFindChunk(chunkOffset,0xa030,false)) readMatSpecular();
  if(tagFindChunk(chunkOffset,0xa040,false)) readMatShininess();
  if(tagFindChunk(chunkOffset,0xa050,false)) readMatTransparent();
  if(tagFindChunk(chunkOffset,0xa200,false)) readMatTexture();
  return ret;
}


// Chunk 0x4000
bool A3DSDecoder::readObjectChunk()
{
  DEBUG_OUT<<"A3DSDecoder::readObjectChunk()...\n";
  char oname[256];
  readNullString(oname,256);
  DEBUG_OUT<<"Object name: "<<oname<<"\n";
  unsigned long chunkOffset=ftell(fd);
  if(tagFindChunk(chunkOffset,0x4100,false)) {
    readTriMesh();
    return true;
  }
  else {
    return false;
  }
}


// Chunk 0x4100
void A3DSDecoder::readTriMesh()
{
  DEBUG_OUT<<"A3DSDecoder::readTriMesh()...\n";
  unsigned long chunkOffset=ftell(fd);
  if(tagFindChunk(chunkOffset,0x4110,false)) readMeshVertexl();
  if(tagFindChunk(chunkOffset,0x4120,false)) readMeshFace();
  if(tagFindChunk(chunkOffset,0x4140,false)) readMeshTexCoords();
}


// Chunk 0x4130
void A3DSDecoder::readMeshMatInfo()
{
  DEBUG_OUT<<"A3DSDecoder::readMeshMatInfo()...\n";
  char oname[256];
  readNullString(oname,256);
  DEBUG_OUT<<"Texture name: \""<<oname<<"\"\n";
  unsigned short n=readShortL();
  unsigned short fn=0;
  DEBUG_OUT<<"...used on "<<n<<" faces.\n";
  for(unsigned int i=0;i<n;i++) {
    fn=readShortL();
    DEBUG_OUT<<"matface "<<i<<": "<<fn<<"\n";
  }
}


// Chunk 0x4140
void A3DSDecoder::readMeshTexCoords()
{
  DEBUG_OUT<<"A3DSDecoder::readMeshTexCoords()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  unsigned n=readShortL();
  DEBUG_OUT<<"There are "<<n<<" more textcoords here.\n";
  if(!n) return;
  ATextCoord3D *theCoord;
  float vx,vy;
  for(unsigned int i=0;i<n;i++) {
    vx=readFloatL();
    vy=readFloatL();
    //DEBUG_OUT<<"textcoord "<<i+ntextcoords<<": "<<vx<<","<<vy<<"\n";
    theCoord=new ATextCoord3D;
    theCoord->u=vx; theCoord->v=vy;
    textCoords.append(theCoord);
  }
  ntextcoords+=n;
}


// Chunk 0x4110
void A3DSDecoder::readMeshVertexl()
{
  DEBUG_OUT<<"A3DSDecoder::readMeshVertexl()...\n";
  if(!nframes) return;
  if(!verts) verts=new AList[1];
  if(!verts) return;
  unsigned short newverts=readShortL();
  nverts+=newverts;
  DEBUG_OUT<<newverts<<" verts in chunk.\n";
  float x,y,z;
  AVertexF3D *theVert;
  for(unsigned int i=0;i<newverts;i++) {
    x=readFloatL();
    y=readFloatL();
    z=readFloatL();
    theVert=new AVertexF3D;
    theVert->x=x;
    theVert->y=y;
    theVert->z=z;
    //DEBUG_OUT<<"vert "<<i+vertOffset<<": "<<theVert->x<<","<<theVert->y<<","<<theVert->z<<"\n";
    verts[0].append(theVert);
  }
  oldVertOffset=vertOffset;
  vertOffset=nverts;
}


// Chunk 0x4120
void A3DSDecoder::readMeshFace()
{
  DEBUG_OUT<<"A3DSDecoder::readMeshFace()...\n";
  unsigned short newfaces=readShortL();
  nfaces+=newfaces;
  DEBUG_OUT<<newfaces<<" faces in chunk.\n";
  unsigned short vx,vy,vz;
  unsigned short fi;
  AFace3D *theFace;
  for(unsigned int i=0;i<newfaces;i++) {
    vx=readShortL();
    vy=readShortL();
    vz=readShortL();
    fi=readShortL();  // face info
    theFace=new AFace3D;
    theFace->a=vx+oldVertOffset;
    theFace->b=vy+oldVertOffset;
    theFace->c=vz+oldVertOffset;
    //DEBUG_OUT<<"face "<<i+faceOffset<<": "<<theFace->a<<","<<theFace->b<<","<<theFace->c<<"\n";
    faces.append(theFace);
  }
  faceOffset=nfaces;
  long chunkOffset=ftell(fd);
  if(tagFindChunk(chunkOffset,0x4130,false)) readMeshMatInfo();
}


// Chunk 0xa010
void A3DSDecoder::readMatAmbient()
{
  DEBUG_OUT<<"A3DSDecoder::readMatAmbient()...\n";
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


// Chunk 0xa020
void A3DSDecoder::readMatDiffuse()
{
  DEBUG_OUT<<"A3DSDecoder::readMatDiffuse()...\n";
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


// Chunk 0xa030
void A3DSDecoder::readMatSpecular()
{
  DEBUG_OUT<<"A3DSDecoder::readMatSpecular()...\n";
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


// Chunk 0xa040
void A3DSDecoder::readMatShininess()
{
  DEBUG_OUT<<"A3DSDecoder::readMatShininess()...\n";
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


// Chunk 0xa050
void A3DSDecoder::readMatTransparent()
{
  DEBUG_OUT<<"A3DSDecoder::readMatTransparent()...\n";
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


// Chunk 0xa200
void A3DSDecoder::readMatTexture()
{
  DEBUG_OUT<<"A3DSDecoder::readMatTexture()...\n";
  long chunkOffset=ftell(fd);
  if(tagFindChunk(chunkOffset,0xa300,false)) readMatTexFile();
}


// Chunk 0xa300
void A3DSDecoder::readMatTexFile()
{
  DEBUG_OUT<<"A3DSDecoder::readMatTexFile()...\n";
  char oname[256];
  readNullString(oname,256);
  DEBUG_OUT<<"Texture name: "<<oname<<"\n";
  ATexture3D *theSkin=new ATexture3D(oname);
  textures.append(theSkin);
  ntextures++;
}



