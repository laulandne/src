
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <stdlib.h>
#include <iostream>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>


#include "AMesh3D.h"


////////////////////////////////////////////////////////////////////////////////
//  AMesh3D Class
////////////////////////////////////////////////////////////////////////////////

AMesh3D::AMesh3D()
{
  init();
}


AMesh3D::AMesh3D(AFace3D *f,AVertexF3D a,AVertexF3D b,AVertexF3D c,AVertexF3D d)
{
  DEBUG_OUT<<"AMesh3D created!\n";
  init();
  allocVertParts(1,0,4);
  verts[0][0].x=a.x; verts[0][0].y=a.y; verts[0][0].z=a.z;
  verts[0][1].x=b.x; verts[0][1].y=b.y; verts[0][1].z=b.z;
  verts[0][2].x=c.x; verts[0][2].y=c.y; verts[0][2].z=c.z;
  verts[0][3].x=d.x; verts[0][3].y=d.y; verts[0][3].z=d.z;
  DEBUG_OUT<<a.x<<","<<a.y<<","<<a.z<<"\n";
  DEBUG_OUT<<b.x<<","<<b.y<<","<<b.z<<"\n";
  DEBUG_OUT<<c.x<<","<<c.y<<","<<c.z<<"\n";
  DEBUG_OUT<<d.x<<","<<d.y<<","<<d.z<<"\n";
  nfaces=2;
  faces=(AFace3D **)calloc(nfaces,sizeof(AFace3D *)*nfaces);
  if(!faces) return;
  faces[0]=(AFace3D *)malloc(sizeof(AFace3D));
  faces[0]->a=3;  faces[0]->b=0;  faces[0]->c=1;
  faces[0]->surf=f->surf; faces[0]->fRed=f->fRed; faces[0]->fGreen=f->fGreen; faces[0]->fBlue=f->fBlue;
  DEBUG_OUT<<faces[0]->surf<<" "<<faces[0]->a<<","<<faces[0]->b<<","<<faces[0]->c<<"\n";
  faces[1]=(AFace3D *)malloc(sizeof(AFace3D));
  faces[1]->a=2;  faces[1]->b=1;  faces[1]->c=3;
  faces[1]->surf=f->surf; faces[1]->fRed=f->fRed; faces[1]->fGreen=f->fGreen; faces[1]->fBlue=f->fBlue;
  DEBUG_OUT<<faces[1]->surf<<" "<<faces[1]->a<<","<<faces[1]->b<<","<<faces[1]->c<<"\n";
  calcWidths();
  isValid();
}


AMesh3D::AMesh3D(unsigned int tnVerts,float *tVerts,unsigned int tnFaces,short *tFaces)
{
  DEBUG_OUT<<"AMesh3D("<<tnVerts<<",tVerts,"<<tnFaces<<",tFaces) created!\n";
  init();
  DEBUG_OUT<<"Building faces...\n";
  faces=(AFace3D **)calloc(tnFaces,sizeof(AFace3D *));
  setNumFaces(tnFaces);
  AFace3D *f;
  for(unsigned int t=0;t<tnFaces;t++) {
    //DEBUG_OUT<<tFaces[t*3]<<","<<tFaces[t*3+1]<<","<<tFaces[t*3+2]<<"\n";
    //f=(AFace3D *)calloc(sizeof(AFace3D),1);
    f=new AFace3D;
    f->a=tFaces[t*3];
    f->b=tFaces[t*3+1];
    f->c=tFaces[t*3+2];
    faces[t]=f;
    //DEBUG_OUT<<faces[t]->a<<","<<faces[t]->b<<","<<faces[t]->c<<"\n";
  }
  //
  DEBUG_OUT<<"Building frames/verts...\n";
  setNumFrames(1);
  allocVertParts(numFrames,0,tnVerts);
  nverts=tnVerts;
  float x,y,z;
  for(unsigned int t=0;t<tnVerts;t++) {
    x=tVerts[t*3];
    y=tVerts[t*3+1];
    z=tVerts[t*3+2];
    //DEBUG_OUT<<tVerts[t*3]<<","<<tVerts[t*3+1]<<","<<tVerts[t*3+2]<<"\n";
    //DEBUG_OUT<<x<<","<<y<<","<<z<<"\n";
    verts[0][t].x=x;
    verts[0][t].y=y;
    verts[0][t].z=z;
    //DEBUG_OUT<<verts[0][t].x<<","<<verts[0][t].y<<","<<verts[0][t].z<<"\n";
  }
  calcWidths();
  isValid();
}


AMesh3D::AMesh3D(unsigned int tnVerts,AVertexF3D **tVerts,unsigned int tnFaces,AFace3D **tFaces)
{
 DEBUG_OUT<<"AMesh3D("<<tnVerts<<",tVerts,"<<tnFaces<<",tFaces) created!\n";
  init();
  DEBUG_OUT<<"Building faces...\n";
  faces=(AFace3D **)calloc(tnFaces,sizeof(AFace3D *));
  setNumFaces(tnFaces);
  AFace3D *f;
  for(unsigned int t=0;t<tnFaces;t++) {
    //DEBUG_OUT<<tFaces[t*3]<<","<<tFaces[t*3+1]<<","<<tFaces[t*3+2]<<"\n";
    //f=(AFace3D *)calloc(sizeof(AFace3D),1);
    f=new AFace3D;
    f->a=tFaces[t]->a;
    f->b=tFaces[t]->b;
    f->c=tFaces[t]->c;
    f->textID=tFaces[t]->textID;
    f->fRed=tFaces[t]->fRed;
    f->fGreen=tFaces[t]->fGreen;
    f->fBlue=tFaces[t]->fBlue;
    faces[t]=f;
    //DEBUG_OUT<<faces[t]->a<<","<<faces[t]->b<<","<<faces[t]->c<<"\n";
  }
  //
  DEBUG_OUT<<"Building frames/verts...\n";
  setNumFrames(1);
  allocVertParts(numFrames,0,tnVerts);
  nverts=tnVerts;
  float x,y,z;
  for(unsigned int t=0;t<tnVerts;t++) {
    x=tVerts[t]->x;
    y=tVerts[t]->y;
    z=tVerts[t]->z;
    //DEBUG_OUT<<tVerts[t*3]<<","<<tVerts[t*3+1]<<","<<tVerts[t*3+2]<<"\n";
    //DEBUG_OUT<<x<<","<<y<<","<<z<<"\n";
    verts[0][t].x=x;
    verts[0][t].y=y;
    verts[0][t].z=z;
    //DEBUG_OUT<<verts[0][t].x<<","<<verts[0][t].y<<","<<verts[0][t].z<<"\n";
  }
  calcWidths();
  isValid();
}


AMesh3D::~AMesh3D()
{
  DEBUG_OUT<<"AMesh3D Destroyed.\n";
}


void AMesh3D::init()
{
  bmin.x=0.0;  bmax.x=0.0;  bmin.y=0.0;  bmax.y=0.0;  bmin.z=0.0;  bmax.z=0.0;
  nparts=0;  parts=NULL;  partList=NULL;
  nfaces=0;  faces=NULL;  faceList=NULL;
  ntextcoords=0;  faceTextCoords=NULL;  faceTextCoordList=NULL;
  nftcs=0;
  nverts=0;
  ntextures=0;
  curFrameNum=0;  curSeqNum=0;
  numFrames=1;  numSeq=1;
  verts=NULL;
  drawVerts=NULL;
  vertNormals=NULL;  faceNormals=NULL;
  textCoords=NULL; textures=NULL;
  fixTextCoords=false;
  name=NULL;
  modelToDrawMatrix.unit();
  /*
  modelToDrawMatrix.translateTo(0,0,0);
  modelToDrawMatrix.scale(1,1,1);
  modelToDrawMatrix.xRotBy(0);
  modelToDrawMatrix.yRotBy(0);
  modelToDrawMatrix.zRotBy(0);
  */
  stale=true;
}


// Assumes you'll createVerts once and only once per frame
void AMesh3D::createVertsPerFrame(unsigned int frame,AList *theVerts,unsigned int numVerts)
{
  if(!numVerts) return;
  if(!theVerts) return;
  DEBUG_OUT<<"createVertsPerFrame("<<frame<<")...\n";
  //DEBUG_OUT<<"numVerts is "<<numVerts<<" numFrames is "<<numFrames<<"\n";
  allocVertParts(numFrames,frame,numVerts);
  unsigned int t=0;
  theVerts->reset();
  while(!theVerts->atEnd()) {
    AVertexF3D *thing=(AVertexF3D *)theVerts->get();
    //DEBUG_OUT<<"vert: "<<t<<" "<<thing->x<<","<<thing->y<<","<<thing->z<<"\n";
    verts[frame][t].x=thing->x;
    verts[frame][t].y=thing->y;
    verts[frame][t].z=thing->z;
    theVerts->advance();
    t++;
  }
  nverts=t;
  DEBUG_OUT<<"Added "<<t<<" verts.\n";
}


// NOTE: These aren't used yet...
void AMesh3D::createVertNormals(AList *theVerts,unsigned int numVerts)
{
  if(!numVerts) return;
  if(!theVerts) return;
  DEBUG_OUT<<"createVertNormals...\n";
  vertNormals=(AVertexF3D *)calloc(numVerts,sizeof(AVertexF3D));
  if(!vertNormals) return;
  unsigned int t=0;
  theVerts->reset();
  while(!theVerts->atEnd()) {
    AVertexF3D *thing=(AVertexF3D *)theVerts->get();
    //DEBUG_OUT<<"vertNorm: "<<t<<" "<<thing->x<<","<<thing->y<<","<<thing->z<<"\n";
    if(t<nverts) {
      vertNormals[t].x=thing->x;
      vertNormals[t].y=thing->y;
      vertNormals[t].z=thing->z;
      t++;
    }
    //else DEBUG_OUT<<"(skipped)\n";
    theVerts->advance();
  }
  DEBUG_OUT<<"Added "<<t<<" vertnormals.\n";
}


void AMesh3D::createTextCoords(AList *theCoords,unsigned int numCoords)
{
  if(!numCoords) return;
  if(!theCoords) return;
  DEBUG_OUT<<"createTextCoords...\n";
  textCoords=(ATextCoord3D *)calloc(numCoords,sizeof(ATextCoord3D));
  if(!textCoords) return;
  unsigned int t=0;
  theCoords->reset();
  while(!theCoords->atEnd()) {
    ATextCoord3D *thing=(ATextCoord3D *)theCoords->get();
	if(fixTextCoords) {
	  if(thing->v>1.0F) { /*dBug<<"(thing->v was "<<thing->v<<")\n";*/ thing->v=1.0F-thing->v; }
	}
    DEBUG_OUT<<"tc: "<<t<<" "<<thing->u<<","<<thing->v<<"\n";
    //if((thing->u<0.0)||(thing->u>1.0)) dBug.warning("thing->u is not between 0 and 1!\n");
    //if((thing->v<0.0)||(thing->v>1.0)) dBug.warning("thing->v is not between 0 and 1!\n");
    textCoords[t].u=thing->u;
    textCoords[t].v=thing->v;
    theCoords->advance();
    t++;
  }
  ntextcoords=t;
  DEBUG_OUT<<"Added "<<t<<" textCoords.\n";
}


void AMesh3D::createTextures(AList *theTextures,unsigned int numTextures)
{
  if(!numTextures) return;
  if(!theTextures) return;
  DEBUG_OUT<<"createTextures...\n";
  textures=(ATexture3D **)calloc(numTextures,sizeof(ATexture3D *));
  if(!textures) return;
  unsigned int t=0;
  theTextures->reset();
  while(!theTextures->atEnd()) {
    ATexture3D *thing=(ATexture3D *)theTextures->get();
    //DEBUG_OUT<<"tc: "<<t<<" "<<thing->x<<","<<thing->y<<"\n";
    textures[t]=thing;
    theTextures->advance();
    t++;
  }
  ntextures=t;
  DEBUG_OUT<<"Added "<<t<<" textures.\n";
}


// If we have textCoords, but not faceTextCoords, then synthesize them
// NOTE: This isn't really correct, just a guess for now
void AMesh3D::buildFTCsFromTCs()
{
  DEBUG_OUT<<"buildFTCsFromTCs...\n";
  unsigned int tx,ty,tz;
  faceTextCoords=(AFace3D **)malloc(sizeof(AFace3D *)*nfaces);
  for(unsigned int i=0;i<nfaces;i++) {
    tx=faces[i]->a;
    ty=faces[i]->b;
    tz=faces[i]->c;
    //DEBUG_OUT<<"ftc "<<i<<": "<<tx<<","<<ty<<","<<tz<<"\n";
    faceTextCoords[i]=(AFace3D *)malloc(sizeof(AFace3D));
    faceTextCoords[i]->a=tx;
    faceTextCoords[i]->b=ty;
    faceTextCoords[i]->c=tz;
  }
}


/*
void AMesh3D::addFace(AFace3D *f)
{
  DEBUG_OUT<<"addFace...\n";
  std::cerr<<"AMesh3D::addFace not implemented yet!\n";
  exit(5);
}
*/


void AMesh3D::calcBoundingBox()
{
  bmin.x=0.0;  bmax.x=0.0;  bmin.y=0.0;  bmax.y=0.0;  bmin.z=0.0;  bmax.z=0.0;
  for(unsigned int t=0;t<nverts;t++) {
    AVertexF3D *dude=&verts[curFrameNum][t];
    if(bmin.x>(dude->x)) bmin.x=dude->x;
    if(bmax.x<(dude->x)) bmax.x=dude->x;
    if(bmin.y>(dude->y)) bmin.y=dude->y;
    if(bmax.y<(dude->y)) bmax.y=dude->y;
    if(bmin.z>(dude->z)) bmin.z=dude->z;
    if(bmax.z<(dude->z)) bmax.z=dude->z;
  }
  debugDump();
}


void AMesh3D::calcWidths()
{
  calcBoundingBox();
  // Not really right, neg values are screwed, or are they?
  xwidth=bmax.x-bmin.x;
  ywidth=bmax.y-bmin.y;
  zwidth=bmax.z-bmin.z;
  //DEBUG_OUT<<"calcWidths: "<<xwidth<<","<<ywidth<<","<<zwidth<<"\n";
}


// Scale model so size is 1.0,1.0,1.0
float AMesh3D::normalizeScale()
{
  calcWidths();
  float xfactor=(float)1.0/(float)xwidth;
  float yfactor=(float)1.0/(float)ywidth;
  float zfactor=(float)1.0/(float)zwidth;
  DEBUG_OUT<<"normalizeScale: "<<xfactor<<","<<yfactor<<","<<zfactor<<"\n";
  float theFactor=xfactor;
  if(yfactor<theFactor) theFactor=yfactor;
  if(zfactor<theFactor) theFactor=zfactor;
  scaleAllVerts(theFactor,theFactor,theFactor);
  calcWidths();
  //debugDump();
  return theFactor;
}


// Center model at 0,0,0
void AMesh3D::normalizeTranslation()
{
  calcWidths();
  float centerx=xwidth/2+bmin.x;
  float centery=ywidth/2+bmin.y;
  float centerz=zwidth/2+bmin.z;
  DEBUG_OUT<<"normalizeTranslation: "<<centerx<<","<<centery<<","<<centerz<<"\n";
  translateAllVerts(-centerx,-centery,-centerz);
  calcWidths();
  //debugDump();
}


void AMesh3D::transformVertsToDraw()
{
  //DEBUG_OUT<<"transformToWorld...\n";
  if(!verts) { std::cerr<<"No verts!\n"; exit(5); }
  if(!verts[curFrameNum]) { std::cerr<<"No verts[curFrameNum]!\n"; exit(5); }
  if(!drawVerts) { std::cerr<<"No worldVerts!\n"; exit(5); }
  modelToDrawMatrix.transformI(verts[curFrameNum],drawVerts,nverts);
  stale=false;
}


void AMesh3D::scaleAllVerts(float sx,float sy,float sz)
{
  DEBUG_OUT<<"scaleAllVerts: "<<sx<<","<<sy<<","<<sz<<"\n";
  for(unsigned int i=0;i<numFrames;i++) {
    for(unsigned int t=0;t<nverts;t++) {
      AVertexF3D *dude=&verts[i][t];
      dude->x=(float)dude->x*sx;
      dude->y=(float)dude->y*sy;
      dude->z=(float)dude->z*sz;
    }
  }
  calcWidths();
  //debugDump();
}


void AMesh3D::translateAllVerts(float sx,float sy,float sz)
{
  DEBUG_OUT<<"translateAllVerts: "<<sx<<","<<sy<<","<<sz<<"\n";
  for(unsigned int i=0;i<numFrames;i++) {
    for(unsigned int t=0;t<nverts;t++) {
      AVertexF3D *dude=&verts[curFrameNum][t];
      dude->x=dude->x+sx;
      dude->y=dude->y+sy;
      dude->z=dude->z+sz;
    }
  }
  calcWidths();
  //debugDump();
}


void AMesh3D::rotateXAllVerts(float r)
{
  DEBUG_OUT<<"rotateXAllVerts: "<<r<<"\n";
  AVertexF3D tv;
  AMatrix3D m;
  m.xRotBy(r);
  for(unsigned int i=0;i<numFrames;i++) {
    for(unsigned int t=0;t<nverts;t++) {
      AVertexF3D *dude=&verts[curFrameNum][t];
      m.mult(dude,&tv);
      dude->x=tv.x;
      dude->y=tv.y;
      dude->z=tv.z;
    }
  }
  calcWidths();
  //debugDump();
}


void AMesh3D::rotateYAllVerts(float r)
{
  DEBUG_OUT<<"rotateYAllVerts: "<<r<<"\n";
  AVertexF3D tv;
  AMatrix3D m;
  m.yRotBy(r);
  for(unsigned int i=0;i<numFrames;i++) {
    for(unsigned int t=0;t<nverts;t++) {
      AVertexF3D *dude=&verts[curFrameNum][t];
      m.mult(dude,&tv);
      dude->x=tv.x;
      dude->y=tv.y;
      dude->z=tv.z;
    }
  }
  calcWidths();
  //debugDump();
}


void AMesh3D::rotateZAllVerts(float r)
{
  DEBUG_OUT<<"rotateZAllVerts: "<<r<<"\n";
  AVertexF3D tv;
  AMatrix3D m;
  m.zRotBy(r);
  for(unsigned int i=0;i<numFrames;i++) {
    for(unsigned int t=0;t<nverts;t++) {
      AVertexF3D *dude=&verts[curFrameNum][t];
      m.mult(dude,&tv);
      dude->x=tv.x;
      dude->y=tv.y;
      dude->z=tv.z;
    }
  }
  calcWidths();
  //debugDump();
}


void AMesh3D::scale(float sx,float sy,float sz)
{
  DEBUG_OUT<<"scale: "<<sx<<","<<sy<<","<<sz<<"\n";
  modelToDrawMatrix.scale(sx,sy,sz);
}


void AMesh3D::translate(float sx,float sy,float sz)
{
  DEBUG_OUT<<"translate: "<<sx<<","<<sy<<","<<sz<<"\n";
  modelToDrawMatrix.translate(sx,sy,sz);
}


void AMesh3D::translateTo(float sx,float sy,float sz)
{
  DEBUG_OUT<<"translateTo: "<<sx<<","<<sy<<","<<sz<<"\n";
  modelToDrawMatrix.translateTo(sx,sy,sz);
}


void AMesh3D::rotateBy(float sx,float sy,float sz)
{
  DEBUG_OUT<<"rotateBy: "<<sx<<","<<sy<<","<<sz<<"\n";
  rotateByX(sx);
  rotateByY(sy);
  rotateByZ(sz);
}


void AMesh3D::rotateByX(float r)
{
  modelToDrawMatrix.xRotBy(r);
}


void AMesh3D::rotateByY(float r)
{
  modelToDrawMatrix.yRotBy(r);
}


void AMesh3D::rotateByZ(float r)
{
  modelToDrawMatrix.zRotBy(r);
}


void AMesh3D::debugDump()
{
  DEBUG_OUT<<"***\n";
  DEBUG_OUT<<"AMesh3D: bbox x=("<<bmin.x<<","<<bmax.x<<") y=("<<bmin.y<<","<<bmax.y<<") z=("<<bmin.z<<","<<bmax.z<<")\n";
  DEBUG_OUT<<"getNumParts: "<<getNumParts()<<"\n";
  DEBUG_OUT<<"getNumVerts: "<<getNumVerts()<<"\n";
  DEBUG_OUT<<"getVertNormals: "; if(getVertNormals()) DEBUG_OUT<<"present\n"; else DEBUG_OUT<<"not present\n";
  DEBUG_OUT<<"getFaceNormals: "; if(getFaceNormals()) DEBUG_OUT<<"present\n"; else DEBUG_OUT<<"not present\n";
  DEBUG_OUT<<"getNumTextCoords: "<<getNumTextCoords()<<"\n";
  DEBUG_OUT<<"getNumTextures: "<<getNumTextures()<<"\n";
  DEBUG_OUT<<"getNumFaces: "<<getNumFaces()<<"\n";
  DEBUG_OUT<<"getNumFaceTextCoords: "<<getNumFaceTextCoords()<<"\n";
  DEBUG_OUT<<"getNumAnimSeq: "<<getNumAnimSeq()<<"\n";
  DEBUG_OUT<<"getNumAnimFrames: "<<getNumAnimFrames()<<"\n";
  DEBUG_OUT<<"***\n";
  /*
  DEBUG_OUT<<"Has "<<nverts<<" verts.\n";
  for(unsigned int t=0;t<nverts;t++) {
    DEBUG_OUT<<"v: "<<verts[t].x<<","<<verts[t].y<<","<<verts[t].z<<"\n";
  }
  DEBUG_OUT<<"Has "<<nfaces<<" faces.\n";
  for(unsigned int t=0;t<nfaces;t++) {
    DEBUG_OUT<<"f: "<<faces[t]->a<<","<<faces[t]->b<<","<<faces[t]->c<<"\n";
  }
  */
}


bool AMesh3D::isValid()
{
  bool val=true;
  if(!verts) val=false;
  if(nfaces) {
    // TODO: Make sure all are valid triangles, if not, drop them
    // zero out non-existant vert refs
    for(unsigned int t=0;t<nfaces;t++) {
      if(faces[t]->a>nverts) faces[t]->a=0;
      if(faces[t]->b>nverts) faces[t]->b=0;
      if(faces[t]->c>nverts) faces[t]->c=0;
    }
  }
  //if(val==false) errorFlag=true;
  return val;
}


void AMesh3D::allocVertParts(unsigned int tNumFrames,unsigned int whichFrame,unsigned int tNumVerts)
{
  DEBUG_OUT<<"allocVertParts: "<<tNumFrames<<","<<whichFrame<<","<<tNumVerts<<"\n";
  nverts=tNumVerts;
  numFrames=tNumFrames;
  if(!verts) verts=(AVertexF3D **)calloc(numFrames,sizeof(void *));
  if(!verts) { std::cerr<<"Didn't get verts!\n"; exit(5); }
  if(!verts[whichFrame]) verts[whichFrame]=(AVertexF3D *)calloc(nverts,sizeof(AVertexF3D));
  if(!verts[whichFrame]) { std::cerr<<"Didn't get verts["<<whichFrame<<"]!\n"; exit(5); }
  if(!drawVerts) drawVerts=(AVertexI3D *)calloc(nverts,sizeof(AVertexI3D));
  if(!drawVerts) { std::cerr<<"Didn't get drawVerts!\n"; exit(5); }
}


