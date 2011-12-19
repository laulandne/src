
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream



#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/ATextCoord3D.h>
#include <portable/nick3d/AMesh3D.h>

#include "AMD5Decoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AMD5Decoder Class
////////////////////////////////////////////////////////////////////////////////

AMD5Decoder::AMD5Decoder(const char *fname) : AMeshDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
  convertAll();
}


/* STATIC */
bool AMD5Decoder::recognize(uint8_t *str)
{
  bool ret=false;
  if(!strncmp("MD5",(char *)str,3)) return true;
  return ret;
}


// NOTE: Only reads first mesh
void AMD5Decoder::readObjects()
{
  DEBUG_OUT<<"AMD5Decoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0L,SEEK_SET);
  //
  fseek(fd,0L,SEEK_SET);
  char *line=(char *)malloc(256);
  char *theCmd=(char *)malloc(256);
  unsigned int numJoints=0,numMeshes=0;
  ndVerts=0;  ndTris=0;  ndWeights=0;
  unsigned int val;
  AMD5Vert *theVert=NULL;
  AMD5Tri *theTri=NULL;
  AMD5Weight *theWeight=NULL;
  bool reading=true;
  unsigned state=0;  // 0=outside, 1=inMesh, 2=verts, 3=tris, 4=weights
  while(reading) {
    fgets(line,256,fd);
    if(feof(fd)) {
      reading=false;
      break;
    }
    if(!feof(fd)) {
      stringHelp.parseString(line,NULL);
    }
    if(!strncmp(line,"numJoints",9)) {
      //sscanf(line,"%s %d %d,%d,%d %d,%d,%d",theCmd,&numJoints);
      DEBUG_OUT<<"numJoints is "<<numJoints<<"\n";
    }
    if(!strncmp(line,"numMeshes",9)) {
      //sscanf(line,"%s %d %d,%d,%d %d,%d,%d",theCmd,&numMeshes);
      DEBUG_OUT<<"numMeshes is "<<numMeshes<<"\n";
    }
    if(!strncmp(line,"mesh",4)) {
      if(state==1) { reading=false; break; }  // only read first mesh
      state=1;
    }
    if(state==1) {
      sscanf(line,"%s %d",theCmd,&val);
      if(!strcmp(theCmd,"numverts")) {
        ndVerts=val;
        DEBUG_OUT<<"numverts is "<<ndVerts<<"\n";
        state=2;
      }
      if(!strcmp(theCmd,"numtris")) {
        ndTris=val;
        DEBUG_OUT<<"numtris is "<<ndTris<<"\n";
        state=3;
      }
      if(!strcmp(theCmd,"numweights")) {
        ndWeights=val;
        DEBUG_OUT<<"numweights is "<<ndWeights<<"\n";
        state=4;
      }
    }
    if(state==2) {
      float s,t;
      unsigned int start,count;
      sscanf(line,"%s %d ( %f %f ) %d %d",theCmd,&val,&s,&t,&start,&count);
      if(!strcmp(theCmd,"vert")) {
        DEBUG_OUT<<"vert "<<val<<": "<<s<<","<<t<<" "<<start<<" "<<count<<"\n";
        theVert=new AMD5Vert;
        theVert->s=s;  theVert->t=t;  theVert->start=start;  theVert->count=count;
        dVerts.append(theVert);
        if(val==(ndVerts-1)) state=1;
      }
    }
    if(state==3) {
      unsigned int a,b,c;
      sscanf(line,"%s %d %d %d %d",theCmd,&val,&a,&b,&c);
      if(!strcmp(theCmd,"tri")) {
        DEBUG_OUT<<"tri "<<val<<": "<<a<<","<<b<<","<<c<<"\n";
        theTri=new AMD5Tri;
        theTri->a=a;  theTri->b=b;  theTri->c=c;
        dTris.append(theTri);
        if(val==(ndTris-1)) state=1;
      }
    }
    if(state==4) {
      unsigned int joint,bias;
      float x,y,z;
      sscanf(line,"%s %d %d %d ( %f %f %f )",theCmd,&val,&joint,&bias,&x,&y,&z);
      if(!strcmp(theCmd,"weight")) {
        DEBUG_OUT<<"weight "<<val<<": "<<joint<<" "<<bias<<" "<<x<<","<<y<<","<<z<<"\n";
        theWeight=new AMD5Weight;
        theWeight->joint=joint;  theWeight->bias=bias;  theWeight->x=x;  theWeight->y=y;  theWeight->z=z;
        dWeights.append(theWeight);
        if(val==(ndWeights-1)) state=1;
      }
    }
  }
}


void AMD5Decoder::convertAll()
{
  DEBUG_OUT<<"AMD5Decoder::convertAll()...\n";
  unsigned int t=0;
  // First triangles...
  DEBUG_OUT<<"Converting triangles...\n";
  AFace3D *theFace;
  AMD5Tri *theTri;
  t=0;
  dTris.reset();
  while(!dTris.atEnd()) {
    theTri=(AMD5Tri *)dTris.get();
    theFace=new AFace3D;
    theFace->a=theTri->a;
    theFace->b=theTri->b;
    theFace->c=theTri->c;
    theFace->surf=0;
    faces.append(theFace);
    dTris.advance();
    t++;
  }
  nfaces=t;
  // now points...
  DEBUG_OUT<<"Converting verts...\n";
  verts=new AList[nframes];
  if(!verts) return;
  AVertexF3D *theVert;
  ATextCoord3D *theCoord;
  AMD5Weight *theWeight;
  AMD5Vert *thedVert;
  t=0;
  dVerts.reset();
  while(!dVerts.atEnd()) {
    thedVert=(AMD5Vert *)dVerts.get();
    theVert=new AVertexF3D;
    theCoord=new ATextCoord3D;
    theCoord->u=thedVert->s;
    theCoord->v=thedVert->t;
    textCoords.append(theCoord);
    theWeight=getWeightNum(thedVert->start);
    theVert->x=theWeight->x;
    theVert->y=theWeight->y;
    theVert->z=theWeight->z;
    verts[0].append(theVert);
    dVerts.advance();
    t++;
  }
  nverts=t;
  ntextcoords=t;
}


AMD5Vert *AMD5Decoder::getVertNum(unsigned int n)
{
  DEBUG_OUT<<"AMD5Decoder::getVertNum()...\n";
  AMD5Vert *res=NULL;
  dVerts.reset();
  for(unsigned int t=0;t<n;t++) dVerts.advance();
  res=(AMD5Vert *)dVerts.get();
  return res;
}


AMD5Tri *AMD5Decoder::getTriNum(unsigned int n)
{
  DEBUG_OUT<<"AMD5Decoder::getTriNum()...\n";
  AMD5Tri *res=NULL;
  dTris.reset();
  for(unsigned int t=0;t<n;t++) dTris.advance();
  res=(AMD5Tri *)dTris.get();
  return res;
}


AMD5Weight *AMD5Decoder::getWeightNum(unsigned int n)
{
  DEBUG_OUT<<"AMD5Decoder::getWeightNum()...\n";
  AMD5Weight *res=NULL;
  dWeights.reset();
  for(unsigned int t=0;t<n;t++) dWeights.advance();
  res=(AMD5Weight *)dWeights.get();
  return res;
}
