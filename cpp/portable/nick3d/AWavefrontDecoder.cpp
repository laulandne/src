
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <stdlib.h>
#include <iostream>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/ATextCoord3D.h>
#include <portable/nick3d/AMesh3D.h>

#include "AWavefrontDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AWavefrontDecoder Class
////////////////////////////////////////////////////////////////////////////////

AWavefrontDecoder::AWavefrontDecoder(const char *fname) : AMeshDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
}


AWavefrontDecoder::~AWavefrontDecoder()
{
}


/* STATIC */
bool AWavefrontDecoder::recognize(uint8_t*str)
{
  bool ret=false;
  bool isText=recognizeTextFile(str);
  DEBUG_OUT<<"isText is "<<isText<<"\n";
  // TODO: finish
  // If it isn't a text file, we KNOW it isn't a WaveFront file...
  if(isText) ret=true;  // good enough for now...we assume ASmartDecoder checked the file extension
  return ret;
}


void AWavefrontDecoder::readObjects()
{
  DEBUG_OUT<<"AWavefrontDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  verts=new AList[nframes];
  if(!verts) return;
  hasvns=false; hasvts=false;
  fseek(fd,0L,SEEK_SET);
  char *line=(char *)malloc(256);
  int a,b;
  //int c,d;
  float fa,fb,fc;
  bool reading=true;
  AVertexF3D *theVert=NULL;
  ATextCoord3D *theTC=NULL;
  AFace3D *theFace=NULL;
  unsigned int nvertnormals=0;
  while(reading) {
    if(feof(fd)) {
      reading=false;
      break;
    }
    fgets(line,256,fd);
    stringHelp.parseString(line,NULL,false,'\\');
    switch(line[0]) {
      case '#':  case ' ':  case '\n':
        // Comment or white space
        break;
      case 's':
        // ???, not handled
        break;
      case 'o':
        // Object name(?), not handled
        break;
      case 'g':
        // Group name, not handled
        // May have name or not, faces that follow go into that group, may be repeated, out of order, etc.
        break;
      case 'u':
        // Probably usemtl, not handled
        // material to use for following faces
        break;
      case 'm':
        // Probably mtllib, not handled
        // name of material library
        break;
      case 'v':
        fa=0.0; fb=0.0; fc=0.0;
        switch(line[1]) {
          case ' ': case '\t':
            fa=0.0; fb=0.0; fc=0.0;
            sscanf(line+1,"%f %f %f",&fa,&fb,&fc);
            DEBUG_OUT<<"v: "<<fa<<","<<fb<<","<<fc<<"\n";
            theVert=new AVertexF3D;
            theVert->x=fa;  theVert->y=fb;  theVert->z=fc;
            verts[0].append(theVert);
            nverts++;
            break;
          case 't':
            fa=0.0; fb=0.0; fc=0.0;
            sscanf(line+1,"%f %f",&fa,&fb);
            DEBUG_OUT<<"vt: "<<fa<<","<<fb<<"\n";
            theTC=new ATextCoord3D;
            theTC->u=fa;  theTC->v=fb;
            textCoords.append(theTC);
            ntextcoords++;
            hasvts=true;
            break;
          case 'n':
            fa=0.0; fb=0.0; fc=0.0;
            sscanf(line+1,"%f %f %f",&fa,&fb,&fc);
            DEBUG_OUT<<"vn: "<<fa<<","<<fb<<","<<fc<<"\n";
            theVert=new AVertexF3D;
            theVert->x=fa;  theVert->y=fb;  theVert->z=fc;
            vertNormals.append(theVert);
            nvertnormals++;
            hasvns=true;
            break;
          default:
            break;
        }
        break;
      case 'l':
        // A line outside of a face, convert to a flat triangle
        a=0; b=0;
        sscanf(line+1,"%d %d\n",&a,&b);
        DEBUG_OUT<<"l: "<<a<<","<<b<<"\n";
        theFace=new AFace3D;
        theFace->a=a-1; theFace->b=b-1; theFace->c=b-1;
        theFace->surf=0;
        faces.append(theFace);
        nfaces++;
        break;
      case 'f':
        nfaces+=handleFace(&faces,1,-1,0);
        break;
      default:
        DEBUG_OUT<<"Got "<<line[0]<<" unknown token...\n";
        break;
    }
  }
  // TODO: Check nvertnormals is either 0 or equal to nverts
}


/*
void AWavefrontDecoder::handleFace(char *line)
{
  dBug<<"AWavefrontDecoder::handleFace()...\n";
  if(errorFlag) return;
  AList faceStack;
  faceToStack(&faceStack);
  unsigned int nv=countSpaces(line);
  if(nv!=3) dBug<<"Found a face with "<<nv<<" vertices...ignoring extras\n";
  unsigned int a=0,b=0,c=0;
  unsigned int a1=0,b1=0,c1=0;
  unsigned int a2=0,b2=0,c2=0;
  AFace3D *theFace=NULL;
  if(hasvts&&hasvns) {
     sscanf(line+1,"%d/%d/%d %d/%d/%d %d/%d/%d\n",&a,&a1,&a2,&b,&b1,&b2,&c,&c1,&c2);
    // TODO: Use the a1, a2, etc normal data...
    dBug<<"f: "<<a<<","<<b<<","<<c<<"\n";
    theFace=new AFace3D;
    theFace->a=a-1; theFace->b=b-1; theFace->c=c-1;
    theFace->surf=0;
    faces.append(theFace);
    nfaces++;
    return;
  }
  if(hasvts&&!hasvns) {
    a2=0; b2=0; c2=0;
    sscanf(line+1,"%d/%d %d/%d %d/%d\n",&a,&a2,&b,&b2,&c,&c2);
    // TODO: Use the a1, a2, etc normal data...
    dBug<<"f: "<<a<<","<<b<<","<<c<<"\n";
    theFace=new AFace3D;
    theFace->a=a-1; theFace->b=b-1; theFace->c=c-1;
    theFace->surf=0;
    faces.append(theFace);
    nfaces++;
    return;
  }
  if(!hasvts&&hasvns) {
    a1=0; b1=0; c1=0;
    sscanf(line+1,"%d//%d %d//%d %d//%d\n",&a,&a1,&b,&b1,&c,&c1);
    // TODO: Use the a1, a2, etc normal data...
    dBug<<"f: "<<a<<","<<b<<","<<c<<"\n";
    theFace=new AFace3D;
    theFace->a=a-1; theFace->b=b-1; theFace->c=c-1;
    theFace->surf=0;
    faces.append(theFace);
    nfaces++;
    return;
  }
  // Just a plain old face if we got here...
  sscanf(line+1,"%d %d %d\n",&a,&b,&c);
  dBug<<"f: "<<a<<","<<b<<","<<c<<"\n";
  theFace=new AFace3D;
  theFace->a=a-1; theFace->b=b-1; theFace->c=c-1;
  theFace->surf=0;
  faces.append(theFace);
  nfaces++;
}
*/

