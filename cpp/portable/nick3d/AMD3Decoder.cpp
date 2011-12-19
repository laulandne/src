
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream



#include <iostream>
#include <string.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/ATextCoord3D.h>
#include <portable/nick3d/AMesh3D.h>

#include "AMD3Decoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AMD3Decoder Class
////////////////////////////////////////////////////////////////////////////////

AMD3Decoder::AMD3Decoder(const char *fname) : AMeshDecoder(fname)
{
  init();
  openFile(fname);
  readHeader();
  /*if(!errorFlag)*/ readBFrames();
  /*if(!errorFlag)*/ readTags();
  /*if(!errorFlag)*/ readMeshes();
}


/* STATIC */
bool AMD3Decoder::recognize(uint8_t *str)
{
  bool ret=false;
  if(!strncmp("IDP3",(char *)str,4)) return true;
  return ret;
}


void AMD3Decoder::readHeader()
{
  DEBUG_OUT<<"AMD3Decoder::readHeader()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  //
  unsigned long magic=readLongL();
  unsigned long vers=readLongL();
  if(vers!=15) {
    //DEBUG_OUT.warning("Bad version number, giving up!\n");
    //errorFlag=true;
    return;
  }
  DEBUG_OUT<<"MD3 file version "<<vers<<"\n";
  //
  char fname[64];
  fread(fname,64,1,fd);
  DEBUG_OUT<<"name is "<<fname<<"\n";
  unsigned long flags=readLongL();
  //
  nbframes=readLongL();
  ntags=readLongL();
  nmeshes=readLongL();
  //
  maxSkins=readLongL();
  framesOffset=readLongL();
  //
  tagsOffset=readLongL();
  meshesOffset=readLongL();
  //
  unsigned long fileSize=readLongL();
  //
  DEBUG_OUT<<"flags="<<flags<<"\n";
  DEBUG_OUT<<"nbframes="<<nbframes<<"\n";
  DEBUG_OUT<<"ntags="<<ntags<<"\n";
  DEBUG_OUT<<"nmeshes="<<nmeshes<<"\n";
  DEBUG_OUT<<"maxSkins="<<maxSkins<<"\n";
  DEBUG_OUT<<"framesOffset="<<framesOffset<<"\n";
  DEBUG_OUT<<"tagsOffset="<<tagsOffset<<"\n";
  DEBUG_OUT<<"meshesOffset="<<meshesOffset<<"\n";
  DEBUG_OUT<<"fileSize="<<fileSize<<"\n";
}


void AMD3Decoder::readBFrames()
{
  DEBUG_OUT<<"AMD3Decoder::readBFrames()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,framesOffset,SEEK_SET);
  DEBUG_OUT<<"There are "<<nbframes<<" bframes.\n";
  if(!nbframes) return;
  AVertexF3D min,max,origin;  // "compressed"
  float radius;
  char name[16];
  for(unsigned int i=0;i<nbframes;i++) {
    readC1VectorL(&min);
    readC1VectorL(&max);
    readC1VectorL(&origin);
    radius=readFloatL();
    fread(name,16,1,fd);
    DEBUG_OUT<<"bframe "<<i<<": "<<radius<<" "<<name<<"\n";
  }
}


void AMD3Decoder::readTags()
{
  DEBUG_OUT<<"AMD3Decoder::readTags()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,tagsOffset,SEEK_SET);
  DEBUG_OUT<<"There are "<<ntags<<" tags.\n";
  if(!ntags) return;
  char tagName[64];
  AVertexF3D tagPos;  // "compressed"
  AMatrix3D rotMatrix;
  for(unsigned int i=0;i<ntags;i++) {
    fread(tagName,64,1,fd);
    readC1VectorL(&tagPos);
    readMatrixL(&rotMatrix);
    DEBUG_OUT<<"tag "<<i<<": "<<tagName<<"\n";
  }
}


void AMD3Decoder::readMeshes()
{
  DEBUG_OUT<<"AMD3Decoder::readMeshes()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<nmeshes<<" meshes.\n";
  if(!nmeshes) return;
  fseek(fd,meshesOffset,SEEK_SET);
  for(unsigned int i=0;i<nmeshes;i++) {
    lastMesh=i;
    DEBUG_OUT<<"mesh "<<i<<": \n";
    // NOTE: Only read first mesh, since we lose track of the offset to the next mesh header
    if(!i) {
      /*if(!errorFlag)*/ readMeshHeader();
      /*if(!errorFlag)*/ readVertices();
      /*if(!errorFlag)*/ readTriangles();
      /*if(!errorFlag)*/ readTextures();
      /*if(!errorFlag)*/ readTextCoords();
    }
  }
}


void AMD3Decoder::readMeshHeader()
{
  DEBUG_OUT<<"AMD3Decoder::readMeshHeader()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  //
  curHeaderOffset=ftell(fd);
  DEBUG_OUT<<"curHeaderOffset is "<<curHeaderOffset<<"\n";
  lastVert=0;
  //
  char magic[4];
  fread(magic,4,1,fd);
  if(strncmp("IDP3",(char *)magic,4)) {
    //DEBUG_OUT.warning("Bad mesh magic number, giving up!\n");
    //errorFlag=true;
    return;
  }
  char meshName[64];
  fread(meshName,64,1,fd);
  DEBUG_OUT<<"name is "<<meshName<<"\n";
  //
  unsigned long flags=readLongL();
  nkframes=readLongL();  // should match nbframes!
  nframes=nkframes;
  ntextures=readLongL();
  nverts=readLongL();
  nfaces=readLongL();
  //
  triOffset=readLongL();
  texturesOffset=readLongL();
  textCoordsOffset=readLongL();
  ntextcoords=nfaces;
  vertsOffset=readLongL();
  unsigned long meshSize=readLongL();
  //
  DEBUG_OUT<<"flags="<<flags<<"\n";
  DEBUG_OUT<<"nkframes="<<nkframes<<"\n";
  DEBUG_OUT<<"ntextures="<<ntextures<<"\n";
  DEBUG_OUT<<"nverts="<<nverts<<"\n";
  DEBUG_OUT<<"nfaces="<<nfaces<<"\n";
  DEBUG_OUT<<"triOffset="<<triOffset<<"\n";
  DEBUG_OUT<<"texturesOffset="<<texturesOffset<<"\n";
  DEBUG_OUT<<"textCoordsOffset="<<textCoordsOffset<<"\n";
  DEBUG_OUT<<"vertsOffset="<<vertsOffset<<"\n";
  DEBUG_OUT<<"meshSize="<<meshSize<<"\n";
}


void AMD3Decoder::readVertices()
{
  DEBUG_OUT<<"AMD3Decoder::readVertices()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,vertsOffset+curHeaderOffset,SEEK_SET);
  DEBUG_OUT<<"There are "<<nkframes<<" kframes.\n";
  if(!nframes) return;
  verts=new AList[nframes];
  if(!verts) return;
  AVertexF3D *theVert;
  for(unsigned i=0;i<nkframes;i++) {
    DEBUG_OUT<<"kframe "<<i<<": \n";
    for(unsigned int j=0;j<nverts;j++) {
      theVert=new AVertexF3D;
      readC2VectorL(theVert);
      //DEBUG_OUT<<"vert "<<j+lastVert<<": "<<theVert->x<<","<<theVert->y<<","<<theVert->z<<"\n";
      verts[i].append(theVert);
     }
    lastVert+=nverts;
  }
}


void AMD3Decoder::readTriangles()
{
  DEBUG_OUT<<"AMD3Decoder::readTriangles()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,triOffset+curHeaderOffset,SEEK_SET);
  DEBUG_OUT<<"There are "<<nfaces<<" triangles.\n";
  if(!nfaces) return;
  unsigned long vx,vy,vz;
  AFace3D *theFace;
  for(unsigned i=0;i<nfaces;i++) {
    vx=readLongL();
    vy=readLongL();
    vz=readLongL();
    if(lastMesh) { vx+=lastVert; vy+=lastVert; vz+=lastVert; }
    DEBUG_OUT<<"face "<<i<<": "<<vx<<","<<vy<<","<<vz<<"\n";
    theFace=new AFace3D;
    theFace->a=vx; theFace->b=vy; theFace->c=vz;
    theFace->surf=0;
    faces.append(theFace);
    theFace=new AFace3D;
    theFace->a=vx; theFace->b=vy; theFace->c=vz;
    faceTextCoords.append(theFace);
  }
}


void AMD3Decoder::readTextures()
{
  DEBUG_OUT<<"AMD3Decoder::readTextures()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<ntextures<<" textures.\n";
  if(!ntextures) return;
  fseek(fd,texturesOffset+curHeaderOffset,SEEK_SET);
  char name[64];
  char actualName[64];
  unsigned int index;
  ATexture3D *theSkin=NULL;
  for(unsigned int i=0;i<ntextures;i++) {
    fread(name,64,1,fd);
    index=readLongL();
    actualName[0]=0;
    stringHelp.fillInNameFromFullName(actualName,name);
    DEBUG_OUT<<"texture "<<i<<": "<<index<<" "<<name<<"\n";
    theSkin=new ATexture3D(actualName);
    textures.append(theSkin);
  }
}


void AMD3Decoder::readTextCoords()
{
  DEBUG_OUT<<"AMD3Decoder::readTextCoords()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<ntextcoords<<" textcoords.\n";
  if(!ntextcoords) return;
  fseek(fd,textCoordsOffset+curHeaderOffset,SEEK_SET);
  ATextCoord3D *theCoord;
  float vx,vy;
  for(unsigned int i=0;i<ntextcoords;i++) {
    vx=readFloatL();
    vy=readFloatL();
    // TODO: Do I need to fix these up or are they good to go?
    DEBUG_OUT<<"textcoord "<<i<<": "<<vx<<","<<vy<<"\n";
    theCoord=new ATextCoord3D;
    theCoord->u=vx; theCoord->v=vy;
    textCoords.append(theCoord);
  }
}

