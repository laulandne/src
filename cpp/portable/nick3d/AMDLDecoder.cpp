
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream



#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/ATextCoord3D.h>
#include <portable/nick3d/AMesh3D.h>

#include "AMDLDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  Quake's palette
////////////////////////////////////////////////////////////////////////////////

unsigned char AQuakePalette[256][3]={
{  0,   0,   0}, { 15,  15,  15}, { 31,  31,  31}, { 47,  47,  47},
{ 63,  63,  63}, { 75,  75,  75}, { 91,  91,  91}, {107, 107, 107},
{123, 123, 123}, {139, 139, 139}, {155, 155, 155}, {171, 171, 171},
{187, 187, 187}, {203, 203, 203}, {219, 219, 219}, {235, 235, 235},
{ 15,  11,   7}, { 23,  15,  11}, { 31,  23,  11}, { 39,  27,  15},
{ 47,  35,  19}, { 55,  43,  23}, { 63,  47,  23}, { 75,  55,  27},
{ 83,  59,  27}, { 91,  67,  31}, { 99,  75,  31}, {107,  83,  31},
{115,  87,  31}, {123,  95,  35}, {131, 103,  35}, {143, 111,  35},
{ 11,  11,  15}, { 19,  19,  27}, { 27,  27,  39}, { 39,  39,  51},
{ 47,  47,  63}, { 55,  55,  75}, { 63,  63,  87}, { 71,  71, 103},
{ 79,  79, 115}, { 91,  91, 127}, { 99,  99, 139}, {107, 107, 151},
{115, 115, 163}, {123, 123, 175}, {131, 131, 187}, {139, 139, 203},
{  0,   0,   0}, {  7,   7,   0}, { 11,  11,   0}, { 19,  19,   0},
{ 27,  27,   0}, { 35,  35,   0}, { 43,  43,   7}, { 47,  47,   7},
{ 55,  55,   7}, { 63,  63,   7}, { 71,  71,   7}, { 75,  75,  11},
{ 83,  83,  11}, { 91,  91,  11}, { 99,  99,  11}, {107, 107,  15},
{  7,   0,   0}, { 15,   0,   0}, { 23,   0,   0}, { 31,   0,   0},
{ 39,   0,   0}, { 47,   0,   0}, { 55,   0,   0}, { 63,   0,   0},
{ 71,   0,   0}, { 79,   0,   0}, { 87,   0,   0}, { 95,   0,   0},
{103,   0,   0}, {111,   0,   0}, {119,   0,   0}, {127,   0,   0},
{ 19,  19,   0}, { 27,  27,   0}, { 35,  35,   0}, { 47,  43,   0},
{ 55,  47,   0}, { 67,  55,   0}, { 75,  59,   7}, { 87,  67,   7},
{ 95,  71,   7}, {107,  75,  11}, {119,  83,  15}, {131,  87,  19},
{139,  91,  19}, {151,  95,  27}, {163,  99,  31}, {175, 103,  35},
{ 35,  19,   7}, { 47,  23,  11}, { 59,  31,  15}, { 75,  35,  19},
{ 87,  43,  23}, { 99,  47,  31}, {115,  55,  35}, {127,  59,  43},
{143,  67,  51}, {159,  79,  51}, {175,  99,  47}, {191, 119,  47},
{207, 143,  43}, {223, 171,  39}, {239, 203,  31}, {255, 243,  27},
{ 11,   7,   0}, { 27,  19,   0}, { 43,  35,  15}, { 55,  43,  19},
{ 71,  51,  27}, { 83,  55,  35}, { 99,  63,  43}, {111,  71,  51},
{127,  83,  63}, {139,  95,  71}, {155, 107,  83}, {167, 123,  95},
{183, 135, 107}, {195, 147, 123}, {211, 163, 139}, {227, 179, 151},
{171, 139, 163}, {159, 127, 151}, {147, 115, 135}, {139, 103, 123},
{127,  91, 111}, {119,  83,  99}, {107,  75,  87}, { 95,  63,  75},
{ 87,  55,  67}, { 75,  47,  55}, { 67,  39,  47}, { 55,  31,  35},
{ 43,  23,  27}, { 35,  19,  19}, { 23,  11,  11}, { 15,   7,   7},
{187, 115, 159}, {175, 107, 143}, {163,  95, 131}, {151,  87, 119},
{139,  79, 107}, {127,  75,  95}, {115,  67,  83}, {107,  59,  75},
{ 95,  51,  63}, { 83,  43,  55}, { 71,  35,  43}, { 59,  31,  35},
{ 47,  23,  27}, { 35,  19,  19}, { 23,  11,  11}, { 15,   7,   7},
{219, 195, 187}, {203, 179, 167}, {191, 163, 155}, {175, 151, 139},
{163, 135, 123}, {151, 123, 111}, {135, 111,  95}, {123,  99,  83},
{107,  87,  71}, { 95,  75,  59}, { 83,  63,  51}, { 67,  51,  39},
{ 55,  43,  31}, { 39,  31,  23}, { 27,  19,  15}, { 15,  11,   7},
{111, 131, 123}, {103, 123, 111}, { 95, 115, 103}, { 87, 107,  95},
{ 79,  99,  87}, { 71,  91,  79}, { 63,  83,  71}, { 55,  75,  63},
{ 47,  67,  55}, { 43,  59,  47}, { 35,  51,  39}, { 31,  43,  31},
{ 23,  35,  23}, { 15,  27,  19}, { 11,  19,  11}, {  7,  11,   7},
{255, 243,  27}, {239, 223,  23}, {219, 203,  19}, {203, 183,  15},
{187, 167,  15}, {171, 151,  11}, {155, 131,   7}, {139, 115,   7},
{123,  99,   7}, {107,  83,   0}, { 91,  71,   0}, { 75,  55,   0},
{ 59,  43,   0}, { 43,  31,   0}, { 27,  15,   0}, { 11,   7,   0},
{  0,   0, 255}, { 11,  11, 239}, { 19,  19, 223}, { 27,  27, 207},
{ 35,  35, 191}, { 43,  43, 175}, { 47,  47, 159}, { 47,  47, 143},
{ 47,  47, 127}, { 47,  47, 111}, { 47,  47,  95}, { 43,  43,  79},
{ 35,  35,  63}, { 27,  27,  47}, { 19,  19,  31}, { 11,  11,  15},
{ 43,   0,   0}, { 59,   0,   0}, { 75,   7,   0}, { 95,   7,   0},
{111,  15,   0}, {127,  23,   7}, {147,  31,   7}, {163,  39,  11},
{183,  51,  15}, {195,  75,  27}, {207,  99,  43}, {219, 127,  59},
{227, 151,  79}, {231, 171,  95}, {239, 191, 119}, {247, 211, 139},
{167, 123,  59}, {183, 155,  55}, {199, 195,  55}, {231, 227,  87},
{127, 191, 255}, {171, 231, 255}, {215, 255, 255}, {103,   0,   0},
{139,   0,   0}, {179,   0,   0}, {215,   0,   0}, {255,   0,   0},
{255, 243, 147}, {255, 247, 199}, {255, 255, 255}, {159,  91,  83},
};


////////////////////////////////////////////////////////////////////////////////
//  AMDLDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMDLDecoder::AMDLDecoder(const char *fname) : AMeshDecoder(fname)
{
  init();
  openFile(fname);
  readHeader();
  /*if(!errorFlag)*/ readTextures();
  /*if(!errorFlag)*/ readTextCoords();
  //if(!errorFlag) {
    readTriangles();
    buildAndFixFaceAndTextCoordLists();
  //}
  /*if(!errorFlag)*/ readFrames();
}


/* STATIC */
bool AMDLDecoder::recognize(uint8_t *str)
{
  bool ret=false;
  if(!strncmp("IDPO",(char *)str,4)) return true;
  return ret;
}


void AMDLDecoder::readHeader()
{
  DEBUG_OUT<<"AMDLDecoder::readHeader()...\n";
  fseek(fd,0,SEEK_SET);
  //
  unsigned long magic=readLongL();
  unsigned long vers=readLongL();  // Should always be 6
  if(vers!=6) {
    //DEBUG_OUT.warning("Bad version number, giving up!\n");
    //errorFlag=true;
    return;
  }
  DEBUG_OUT<<"MDL file version "<<vers<<"\n";
  //
  readVectorL(&scale);
  readVectorL(&translate);
  boundingradius=readFloatL();
  readVectorL(&eyeposition);
  //
  ntextures=readLongL();
  skinwidth=readLongL();
  skinheight=readLongL();
  //
  nverts=readLongL();
  nfaces=readLongL();
  //
  nframes=readLongL();
  synctype=readLongL();  // 0=synchron, 1=random
  if((synctype!=0)&&(synctype!=1)) {
    //DEBUG_OUT.warning("synctype field has bad value, giving up!\n");
    //errorFlag=true;
    return;
  }
  flags=readLongL();
  theSize=readLongL();
  //
  DEBUG_OUT<<"ntextures="<<ntextures<<"\n";
  DEBUG_OUT<<"skinwidth="<<skinwidth<<"\n";
  DEBUG_OUT<<"skinheight="<<skinheight<<"\n";
  DEBUG_OUT<<"nverts="<<nverts<<"\n";
  DEBUG_OUT<<"nfaces="<<nfaces<<"\n";
  DEBUG_OUT<<"nframes="<<nframes<<"\n";
  DEBUG_OUT<<"synctype="<<synctype<<"\n";
  DEBUG_OUT<<"flags="<<flags<<"\n";
  DEBUG_OUT<<"theSize="<<theSize<<"\n";
}


void AMDLDecoder::readTextures()
{
  DEBUG_OUT<<"AMDLDecoder::readTextures()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<ntextures<<" textures.\n";
  if(!ntextures) return;
  unsigned int group,nb;
  float time;
  unsigned int i;
  unsigned int bmap_size=skinwidth*skinheight;
  ATexture3D *theText=NULL;
  for(unsigned int t=0;t<ntextures;t++) {
    group=readLongL();
    DEBUG_OUT<<"tex "<<t<<": "<<group<<" "<<skinwidth<<"x"<<skinheight;
    if((group!=0)&&(group!=1)) {
      DEBUG_OUT<<"\n";
      //DEBUG_OUT.warning("Group field has bad value, giving up!\n");
      //errorFlag=true;
      return;
    }
    if(group) {
      // group of pictures
      nb=readLongL();  // number of picts
      DEBUG_OUT<<nb<<"\n";
      // error out here, we don't understand this format fully
      //DEBUG_OUT.warning("We don't understand the format!\n");
      //errorFlag=true;
      return;
	  /*
      for(i=0;i<nb;i++) {
        time=readFloatL();
        DEBUG_OUT<<"time "<<i<<" :\n";
      }
      for(i=0;i<nb;i++) {
        DEBUG_OUT<<"bitmap "<<i<<" :\n";
        // skip bitmap for now...
        fseek(fd,bmap_size,SEEK_CUR);
      }
	  */
    }
    else {
      // single skin
      DEBUG_OUT<<"\n";
      theText=new ATexture3D(skinwidth,skinheight,8,1);
      uint8_t *ptr=(uint8_t *)NULL; //theText->getBitmap()->getPlane(0);
	  fread(ptr,skinwidth*skinheight,1,fd);
	  APalette *theColors=(APalette *)NULL; //theText->getBitmap()->getColors();
	  for(unsigned int j=0;j<256;j++) {
	    theColors->setColor(j,AQuakePalette[j][0],AQuakePalette[j][1],AQuakePalette[j][2]);
	  }
      textures.append(theText);
    }
  }
}


void AMDLDecoder::readTextCoords()
{
  DEBUG_OUT<<"AMDLDecoder::readTextCoords()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  ntextcoords=nverts;
  DEBUG_OUT<<"There are "<<ntextcoords<<" textcoords.\n";
  if(!ntextcoords) return;
  unsigned long isOnSeam,tw,th;
  float tcw,tch;
  ttcs=(ATextCoord3D *)malloc(sizeof(ATextCoord3D)*ntextcoords);
  onSeam=(bool *)malloc(sizeof(bool)*ntextcoords);
  for(unsigned int i=0;i<ntextcoords;i++) {
    isOnSeam=readLongL();
    tw=readLongL();
    th=readLongL();
    tcw=(float)tw/skinwidth;
    tch=(float)th/skinheight;
    DEBUG_OUT<<"textcoord "<<i<<": "<<isOnSeam<<" "<<tcw<<","<<tch<<"\n";
    ttcs[i].u=tcw;
    ttcs[i].v=tch;
    onSeam[i]=isOnSeam;
  }
}


void AMDLDecoder::readTriangles()
{
  DEBUG_OUT<<"AMDLDecoder::readTriangles()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<nfaces<<" triangles.\n";
  if(!nfaces) return;
  unsigned long facesfront;
  unsigned int a,b,c;
  frontBack=(char *)malloc(nfaces);
  tfaces=(AFace3D *)malloc(sizeof(AFace3D)*nfaces);
  for(unsigned int i=0;i<nfaces;i++) {
    facesfront=readLongL();  // 0=back, 1=front
    a=readLongL();
    b=readLongL();
    c=readLongL();
    DEBUG_OUT<<"face "<<i<<": "<<facesfront<<" "<<a<<","<<b<<","<<c<<"\n";
    tfaces[i].a=a;
    tfaces[i].b=b;
    tfaces[i].c=c;
    frontBack[i]=(char)facesfront;
  }
}


void AMDLDecoder::readFrames()
{
  DEBUG_OUT<<"AMDLDecoder::readFrames()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<nframes<<" frames.\n";
  if(!nframes) return;
  verts=new AList[nframes];
  if(!verts) return;
  AVertexF3D bboxmin,bboxmax;
  AVertexF3D *theVert;
  float temp;
  unsigned int type;
  char name[16];
  for(unsigned int i=0;i<nframes;i++) {
    type=readLongL();  // 0=simple, 1=group
    DEBUG_OUT<<"frame "<<i<<": "<<type<<"\n";
    if((type!=0)&&(type!=1)) {
      DEBUG_OUT<<"\n";
      //DEBUG_OUT.warning("type field has bad value, giving up!\n");
      //errorFlag=true;
      return;
    }
    if(type) {
      // group frame
      readC1VectorL(&bboxmin);  // actually min pos
      readC1VectorL(&bboxmax);  // actually max pos
      // error out here, we don't understand this format fully
      //DEBUG_OUT.warning("We don't understand the format!\n");
      //errorFlag=true;
      return;
    }
    else {
      // simple frame
      readC1VectorL(&bboxmin);
      readC1VectorL(&bboxmax);
      fread(name,16,1,fd);
      for(unsigned int j=0;j<nverts;j++) {
        theVert=new AVertexF3D;
        readC1VectorL(theVert);
        //DEBUG_OUT<<"vert "<<j<<": "<<theVert->x<<","<<theVert->y<<","<<theVert->z<<"\n";
        temp=theVert->y; theVert->y=theVert->z;  theVert->z=temp;
        verts[i].append(theVert);
        //nverts++;
      }
    }
  }
}


// TODO: This isn't right yet...
// Until this works, all back facing triangles will get the wrong texture
void AMDLDecoder::buildAndFixFaceAndTextCoordLists()
{
  DEBUG_OUT<<"AMDLDecoder::buildAndFixFaceAndTextCoordLists()...\n";
  float tx,ty;
  ATextCoord3D *theTC=NULL;
  AFace3D *theFace=NULL;
  unsigned int i,j;
  alreadyFixed=(bool *)malloc(sizeof(bool)*ntextcoords);
  for(i=0;i<ntextcoords;i++) alreadyFixed[i]=false;
  for(i=0;i<nfaces;i++) {
    if(frontBack[i]) {
      j=tfaces[i].a;
      if(!alreadyFixed[j]) {
        ttcs[j].u+=.5;
        alreadyFixed[j]=true;
      }
      j=tfaces[i].b;
      if(!alreadyFixed[j]) {
        ttcs[j].u+=.5;
        alreadyFixed[j]=true;
      }
      j=tfaces[i].c;
      if(!alreadyFixed[j]) {
        ttcs[j].u+=.5;
        alreadyFixed[j]=true;
      }
    }
  }
  for(i=0;i<ntextcoords;i++) {
    theTC=new ATextCoord3D;
    theTC->u=ttcs[i].u;
    theTC->v=ttcs[i].v;
    textCoords.append(theTC);
  }
  for(i=0;i<nfaces;i++) {
    theFace=new AFace3D;
    theFace->a=tfaces[i].a;
    theFace->b=tfaces[i].b;
    theFace->c=tfaces[i].c;
    theFace->surf=0;
    faces.append(theFace);
  }
}

