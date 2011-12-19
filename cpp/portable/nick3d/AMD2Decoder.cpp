
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream



#include <iostream>
#include <string.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/ATextCoord3D.h>
#include <portable/nick3d/AMesh3D.h>

#include "AMD2Decoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AMD2Decoder Class
////////////////////////////////////////////////////////////////////////////////

AMD2Decoder::AMD2Decoder(const char *fname) : AMeshDecoder(fname)
{
  init();
  openFile(fname);
  readHeader();
  /*if(!errorFlag)*/ readFrames();
  /*if(!errorFlag)*/ readTriangles();
  //if(!errorFlag) {
    if(ntextures) readSkins();
	else {
	  char *maybeName=strdup(fname);
	  unsigned int sl=strlen(maybeName);
	  maybeName[sl-3]='b';
	  maybeName[sl-2]='m';
	  maybeName[sl-1]='p';
	  ATexture3D *b=new ATexture3D(maybeName);
	  if(b) {
	    //if(!b->error()) {
      textures.append(b);
		  ntextures=1;
		//}
	  }
	}
  //}
  /*if(!errorFlag)*/ readTextCoords();
}


/* STATIC */
bool AMD2Decoder::recognize(uint8_t *str)
{
  bool ret=false;
  if(!strncmp("IDP2",(char *)str,4)) return true;
  return ret;
}


void AMD2Decoder::readHeader()
{
  DEBUG_OUT<<"AMD2Decoder::readHeader()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  //
  unsigned long magic=readLongL();
  unsigned long vers=readLongL();
  if(vers!=8) {
    //DEBUG_OUT.warning("Bad version number, giving up!\n");
    //errorFlag=true;
    return;
  }
  DEBUG_OUT<<"MD2 file version "<<vers<<"\n";
  //
  skinWidth=readLongL();
  skinHeight=readLongL();
  frameSize=readLongL();
  //
  ntextures=readLongL();
  nverts=readLongL();
  ntextcoords=readLongL();
  nfaces=readLongL();
  unsigned long numGLCmds=readLongL();
  nframes=readLongL();
  //
  skinsOffset=readLongL();
  textCoordsOffset=readLongL();
  trianglesOffset=readLongL();
  framesOffset=readLongL();
  unsigned long glOffset=readLongL();
  //
  unsigned long fileSize=readLongL();
  //
  DEBUG_OUT<<"skinWidth="<<skinWidth<<"\n";
  DEBUG_OUT<<"skinHeight="<<skinHeight<<"\n";
  DEBUG_OUT<<"frameSize="<<frameSize<<"\n";
  DEBUG_OUT<<"ntextures="<<ntextures<<"\n";
  DEBUG_OUT<<"nverts="<<nverts<<"\n";
  DEBUG_OUT<<"ntextcoords="<<ntextcoords<<"\n";
  DEBUG_OUT<<"nfaces="<<nfaces<<"\n";
  DEBUG_OUT<<"numGLCmds="<<numGLCmds<<"\n";
  DEBUG_OUT<<"nframes="<<nframes<<"\n";
  DEBUG_OUT<<"skinsOffset="<<skinsOffset<<"\n";
  DEBUG_OUT<<"textCoordsOffset="<<textCoordsOffset<<"\n";
  DEBUG_OUT<<"trianglesOffset="<<trianglesOffset<<"\n";
  DEBUG_OUT<<"framesOffset="<<framesOffset<<"\n";
  DEBUG_OUT<<"glOffset="<<glOffset<<"\n";
  DEBUG_OUT<<"fileSize="<<fileSize<<"\n";
}


void AMD2Decoder::readFrames()
{
  DEBUG_OUT<<"AMD2Decoder::readFrames()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,framesOffset,SEEK_SET);
  DEBUG_OUT<<"There are "<<nframes<<" frames.\n";
  if(!nframes) return;
  verts=new AList[nframes];
  if(!verts) return;
  AVertexF3D scale;
  AVertexF3D trans;
  float temp;
  char name[16];
  AVertexF3D *theVert;
  for(unsigned int i=0;i<nframes;i++) {
    readVectorL(&scale);
    readVectorL(&trans);
    fread(name,16,1,fd);  // TODO: How do I tell if this is valid?
    DEBUG_OUT<<"frame "<<i<<": "/*<<name*/;
    DEBUG_OUT<<" "<<scale.x<<","<<scale.y<<","<<scale.z;
    DEBUG_OUT<<" "<<trans.x<<","<<trans.y<<","<<trans.z;
    DEBUG_OUT<<"\n";
    for(unsigned int j=0;j<nverts;j++) {
      theVert=new AVertexF3D;
      readC1VectorL(theVert);
      //DEBUG_OUT<<"rawv "<<j<<": "<<theVert->x<<","<<theVert->y<<","<<theVert->z<<"\n";
      theVert->x=theVert->x*scale.x+trans.x;
      theVert->y=theVert->y*scale.y+trans.y;
      theVert->z=theVert->z*scale.z+trans.z;
      temp=theVert->y; theVert->y=theVert->z;  theVert->z=temp;
      //DEBUG_OUT<<"vert "<<j<<": "<<theVert->x<<","<<theVert->y<<","<<theVert->z<<"\n";
      verts[i].append(theVert);
    }
  }
}


void AMD2Decoder::readTriangles()
{
  DEBUG_OUT<<"AMD2Decoder::readTriangles()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,trianglesOffset,SEEK_SET);
  DEBUG_OUT<<"There are "<<nfaces<<" triangles.\n";
  if(!nfaces) return;
  unsigned short va,vb,vc;
  unsigned short ta,tb,tc;
  AFace3D *theFace;
  for(unsigned i=0;i<nfaces;i++) {
    va=readShortL();
    vb=readShortL();
    vc=readShortL();
    DEBUG_OUT<<"face "<<i<<": "<<va<<","<<vb<<","<<vc;
    theFace=new AFace3D;
    theFace->a=va; theFace->b=vb; theFace->c=vc;
    faces.append(theFace);
    ta=readShortL();
    tb=readShortL();
    tc=readShortL();
    DEBUG_OUT<<" (ftc "<<ta<<","<<tb<<","<<tc<<")\n";
    theFace=new AFace3D;
    theFace->a=ta; theFace->b=tb; theFace->c=tc;
    theFace->surf=0;
    faceTextCoords.append(theFace);
  }
}


void AMD2Decoder::readSkins()
{
  DEBUG_OUT<<"AMD2Decoder::readSkins()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,skinsOffset,SEEK_SET);
  DEBUG_OUT<<"There are "<<ntextures<<" skins.\n";
  if(!ntextures) return;
  char skinName[64];
  char actualName[64];
  ATexture3D *theSkin=NULL;
  for(unsigned i=0;i<ntextures;i++) {
    fread(skinName,64,1,fd);
    actualName[0]=0;
    stringHelp.fillInNameFromFullName(actualName,skinName);
    DEBUG_OUT<<"skin "<<i<<": "<<skinName<<" ("<<actualName<<")\n";
    theSkin=new ATexture3D(actualName);
    textures.append(theSkin);
  }
}


void AMD2Decoder::readTextCoords()
{
  DEBUG_OUT<<"AMD2Decoder::readTextCoords()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,textCoordsOffset,SEEK_SET);
  DEBUG_OUT<<"There are "<<ntextcoords<<" textcoords.\n";
  if(!ntextcoords) return;
  unsigned short tw,th;
  float tcw,tch;
  ATextCoord3D *theCoord=NULL;
  for(unsigned i=0;i<ntextcoords;i++) {
    tw=readShortL();
    th=readShortL();
    tcw=(float)tw/skinWidth;
    tch=(float)th/skinHeight;
    DEBUG_OUT<<"textcoord "<<i<<": "<<tcw<<","<<tch<<"\n";
    theCoord=new ATextCoord3D;
    theCoord->u=tcw; theCoord->v=tch;
    textCoords.append(theCoord);
  }
}

