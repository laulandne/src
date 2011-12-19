
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>
#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/ATextCoord3D.h>
#include <portable/nick3d/AMesh3D.h>
#include <portable/nick3d/ATexture3D.h>

#include "AMeshDecoder.h"

#include "AWLDDecoder.h"

#include "ASmartMeshDecoder.h"


#ifdef __MWERKS__
#define strdup(x) strcpy((char *)malloc(strlen(x)+1),x)
#endif // __MWERKS__


////////////////////////////////////////////////////////////////////////////////
//  AWLDDecoder Class
////////////////////////////////////////////////////////////////////////////////

AWLDDecoder::AWLDDecoder(const char *fname) : ASceneDecoder(fname)
{
  init();
  openFile(fname);
  curSurfMap=NULL;
  readObjectsFromFile(fd);
}


/* STATIC */
bool AWLDDecoder::recognize(uint8_t *str)
{
  bool ret=false;
  bool isText=recognizeTextFile(str);
  //DEBUG_OUT<<"isText is "<<isText<<"\n";
  // TODO: finish
  // If it isn't a text file, we KNOW it isn't a WLD file...
  if(isText) ret=true;  // good enough for now...we assume ASmartDecoder checked the file extension
  return ret;
}


void AWLDDecoder::readObjectsFromFile(FILE *fd)
{
  //DEBUG_OUT<<"AWLDDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0L,SEEK_SET);
  char *line=(char *)malloc(256);
  char *theCmd=(char *)malloc(256);
  char *theName=(char *)malloc(256);
  char *finalName=NULL;
  char *misc1=(char *)malloc(256);
  char *misc2=(char *)malloc(256);
  char *misc3=(char *)malloc(256);
  char *actualName=NULL;
  int x,y,z;
  //int a,b,c;
  float fx,fy,fz;
  float ffa,ffb,ffc;
  int n;
  float fa,fb,fc;
  ACamera3D *theCam=NULL;
  ALight3D *theLight=NULL;
  AMeshDecoder *decoder=NULL;
  AMesh3D *theMesh=NULL;
  AFace3D *theFace=NULL;
  AWLDDecoderSurfaceDef *theSD;
  AWLDDecoderSurfaceMap *theSM;
  AWLDDecoderSurface *theS;
  AWLDDecoderObject *theObj;
  //fgets(line,256,fd);
  //if(line[0]=='#') { while(line[0]=='#') fgets(line,256,fd); }
  bool reading=true;
  while(reading) {
    line[0]=0;
    fgets(line,256,fd);
    if(!feof(fd)) {
      stringHelp.parseString(line,NULL,false,',');
      //
    }
    switch(line[0]) {
      case '#':  // comment
        break;
      case '\n':  // blank line
        break;
      case 's':  // stepsize or surface def/map/etc?
        if(!strncmp(line,"surfacedef",10)) {
          sscanf(line,"%s %s %x",theCmd,theName,&x);
          DEBUG_OUT<<"surfacedef "<<theName<<": "<<x<<"\n";
          theSD=new AWLDDecoderSurfaceDef;
          theSD->name=strdup(theName);
          theSD->value=x;
          surfaceDefs.append(theSD);
        }
        if(!strncmp(line,"surfacemap",10)) {
          sscanf(line,"%s %s",theCmd,theName);
          DEBUG_OUT<<"surfacemap "<<theName<<"\n";
          theSM=new AWLDDecoderSurfaceMap;
          theSM->name=strdup(theName);
          surfaceMaps.append(theSM);
          curSurfMap=theSM;
        }
        if(!strncmp(line,"surface ",8)) {
         sscanf(line,"%s %x %s",theCmd,&x,theName);
          DEBUG_OUT<<"surface "<<x<<": "<<theName<<"\n";
          theS=new AWLDDecoderSurface;
          theS->name=strdup(theName);
          theS->num=x;
          if(curSurfMap) curSurfMap->surfaces.append(theS);
        }
        break;
      case 'i':  // include?
        if(!strncmp(line,"include",7)) {
          sscanf(line,"%s %s",theCmd,theName);
          DEBUG_OUT<<"include "<<theName<<"\n";
          FILE *myfd=fopen(theName,"r");
          if(myfd) {
            readObjectsFromFile(myfd);
            fclose(myfd);
          }
        }
        break;
      case 'g':  // groundcolor?
        if(!strncmp(line,"groundcolor",11)) {
          sscanf(line,"%s %n",theCmd,&n);
          DEBUG_OUT<<"groundcolor "<<n<<"\n";
          lookupRend386Color(n,&theGround.fRed,&theGround.fGreen,&theGround.fBlue);
        }
        break;
      case 't':  // title?
        break;
      case 'a':  // ambient? anglestep?
        break;
      case 'p':  // polyobj or polyobj2?
        // NOTE: This only handles 4 sides!
        if(!strncmp(line,"polyobj",7)) {
          float ax,ay,az,bx,by,bz,cx,cy,cz,dx,dy,dz;
          sscanf(line,"%s %d %s %f,%f,%f %f,%f,%f %f,%f,%f %f,%f,%f",theCmd,&n,misc1,&ax,&ay,&az,&bx,&by,&bz,&cx,&cy,&cz,&dx,&dy,&dz);
          DEBUG_OUT<<"polyobj "<<n<<" "<<misc1<<" ";
          DEBUG_OUT<<ax<<","<<ay<<","<<az<<" ";
          DEBUG_OUT<<bx<<","<<by<<","<<bz<<" ";
          DEBUG_OUT<<cx<<","<<cy<<","<<cz<<" ";
          DEBUG_OUT<<dx<<","<<dy<<","<<dz<<" ";
          DEBUG_OUT<<"\n";
          // TODO: This doesn't seem to work correctly to turn a quad into a triangle...wrong order of verts?
          AVertexF3D va,vb,vc,vd;
          va.x=ax;  va.y=ay;  va.z=az;
          vb.x=bx;  vb.y=by;  vb.z=bz;
          vc.x=cx;  vc.y=cy;  vc.z=cz;
          vd.x=dx;  vd.y=dy;  vd.z=dz;
          theFace=new AFace3D;
          theFace->surf=1;  // for now
          lookupRend386Color(theFace->surf,&theFace->fRed,&theFace->fGreen,&theFace->fBlue);
          theMesh=new AMesh3D(theFace,vd,vc,vb,va);
          //theMesh->addFace(theFace);
          meshes.append(theMesh);
          nmeshes++;
          theObj=new AWLDDecoderObject;
          theObj->tag=NULL;  theObj->surfMapName=NULL;
          theObj->name=(char *)"polyobj";
          theObj->mesh=theMesh;
          theObj->surfMapName=NULL;
          objects.append(theObj);
        }
        break;
      case 'l':  // light?
        x=0; y=0; z=0;
        // Should these be floats?
        sscanf(line,"%s %d,%d,%d",theCmd,&x,&y,&z);
        //DEBUG_OUT<<"Light at "<<x<<","<<y<<","<<z<<"\n";
        theLight=new ALight3D(x,y,z);
        lights.append(theLight);
        nlights++;
        break;
      case 'c':  // camera?
        n=0;
        fa=0; fb=0; fc=0;  fx=0; fy=0; fz=0;
        sscanf(line,"%s %d %f,%f,%f %f,%f,%f",theCmd,&n,&fx,&fy,&fz,&fa,&fb,&fc);
        // Because people are sloppy and sometimes use commas and sometimes spaces...
        if((fy==0.0)&&(fz==0.0)&&(fb==0.0)&&(fc==0.0)) sscanf(line,"%s %d %f %f %f %f %f %f",theCmd,&n,&fx,&fy,&fz,&fa,&fb,&fc);
        DEBUG_OUT<<"Camera #"<<n<<" at "<<fx<<","<<fy<<","<<fz<<" rot "<<fa<<","<<fb<<","<<fc<<"\n";
        theCam=new ACamera3D(fx,fy,fz,fa,fb,fc);
        cameras.append(theCam);
        ncameras++;
        break;
      case 'o':  // object? option?
        if(!strncmp(line,"object",6)) {
          fa=0.0; fb=0.0; fc=0.0;
          ffa=0; ffb=0; ffc=0;  fx=0; fy=0; fz=0;
          misc1[0]=0;  misc2[0]=0;  misc3[0]=0;
          sscanf(line,"%s %s %f,%f,%f %f,%f,%f %f,%f,%f %s %s %s",theCmd,theName,&fa,&fb,&fc,&fx,&fy,&fz,&ffa,&ffb,&ffc,misc1,misc2,misc3);
          DEBUG_OUT<<"Object "<<theName<<" scale "<<fa<<","<<fb<<","<<fc<<" rot "<<fx<<","<<fy<<","<<fz<<" at "<<ffa<<","<<ffb<<","<<ffc<<"\n";
          DEBUG_OUT<<"Misc: "<<misc1<<","<<misc2<<","<<misc3<<"\n";
          actualName=theName;
          for(unsigned int t=0;t<strlen(theName);t++) {
            if(theName[t]=='=') actualName=theName+t+1;
          }
          finalName=stringHelp.toLower(actualName);
          if(strncmp(".plg",finalName+strlen(finalName)-4,4)) strcat(finalName,".plg");
          decoder=(AMeshDecoder *)ASmartMeshDecoder::pickMeshDecoder(finalName);
          theMesh=NULL;
          if(decoder) theMesh=decoder->createNewMeshObject();
          else DEBUG_OUT<<"ASmartMeshDecoder couldn't figure out the right decoder!\n";
          if(theMesh) {
            // NOTE: Strange order...
            theMesh->rotateYAllVerts(fy);
            theMesh->rotateXAllVerts(fx);
            theMesh->rotateZAllVerts(fz);
            theMesh->scaleAllVerts(fa,fb,fc);
            theMesh->translateAllVerts(ffa,ffb,ffc);
            meshes.append(theMesh);
            nmeshes++;
            theObj=new AWLDDecoderObject;
            theObj->tag=NULL;  theObj->surfMapName=NULL;
            theObj->name=finalName;
            theObj->mesh=theMesh;
            if(misc2) theObj->surfMapName=strdup(misc2);
            objects.append(theObj);
            fixObjectSurfMaps(theObj);
          }
          else DEBUG_OUT<<"Didn't get theMesh!\n";
        }
        break;
      default:
        // ignore
        DEBUG_OUT<<"Ignoring line beginning with '"<<line[0]<<"'\n";
        break;
    }
    if(feof(fd)) reading=false;
  }
}


void AWLDDecoder::fixObjectSurfMaps(AWLDDecoderObject *theObj)
{
  if(!theObj) return;
  if(!theObj->surfMapName) return;
  if(!strcmp(".",theObj->surfMapName)) return;  // dot means no surfmap
  if(!strlen(theObj->surfMapName)) return;
  if(theObj->surfMapName[0]=='#') return;
  DEBUG_OUT<<"AWLDDecoder::fixObjectSurfMaps...\n";
  DEBUG_OUT<<"name is "<<theObj->name<<" surfmap is "<<theObj->surfMapName<<"\n";
  if(!theObj->mesh) return;
  unsigned int nfaces=theObj->mesh->getNumFaces();
  AFace3D **faces=theObj->mesh->getFaces();
  unsigned int cnum;
  if(!faces) return;
  for(unsigned int i=0;i<nfaces;i++) {
    // look at mapped surfaces only
    if(faces[i]->surf&0x8000) {
      cnum=faces[i]->surf&0xfff;
      DEBUG_OUT<<"face "<<i<<": cnum is "<<cnum<<" ";
      faces[i]->surf=lookupSurfNum(theObj->surfMapName,cnum);
      lookupRend386Color(faces[i]->surf,&faces[i]->fRed,&faces[i]->fGreen,&faces[i]->fBlue);
      //DEBUG_OUT<<"\n";
    }
  }
}


// NOTE: This is a bit of a mess!
unsigned int AWLDDecoder::lookupSurfNum(char *surfMapName,unsigned int surfNum)
{
  if(!surfMapName) return 0;
  unsigned int res=0;
  AWLDDecoderSurfaceMap *cmap=NULL;
  AWLDDecoderSurfaceMap *map=NULL;
  surfaceMaps.reset();
  bool looking=true;
  while(looking) {
    cmap=(AWLDDecoderSurfaceMap *)surfaceMaps.get();
    if(!cmap) { looking=false; break; }
    if(!strcmp(cmap->name,surfMapName)) { map=cmap; looking=false; break; }
    if(surfaceMaps.atEnd()) { looking=false; break; }
    surfaceMaps.advance();
  }
  if(map) {
    AWLDDecoderSurface *csurf=NULL;
    AWLDDecoderSurface *surf=NULL;
    map->surfaces.reset();
    looking=true;
    while(looking) {
      csurf=(AWLDDecoderSurface *)map->surfaces.get();
      if(!csurf) { looking=false; break; }
      if(surfNum==csurf->num) { surf=csurf; looking=false; break; }
      if(map->surfaces.atEnd()) { looking=false; break; }
      map->surfaces.advance();
    }
    if(surf) {
      DEBUG_OUT<<" (lookup got "<<surf->name<<" for "<<surfNum<<") ";
      AWLDDecoderSurfaceDef *csurfdef=NULL;
      AWLDDecoderSurfaceDef *surfdef=NULL;
      surfaceDefs.reset();
      looking=true;
      while(looking) {
        csurfdef=(AWLDDecoderSurfaceDef *)surfaceDefs.get();
        if(!csurfdef) { looking=false; break; }
        if(!strcmp(csurfdef->name,surf->name)) { surfdef=csurfdef; looking=false; break; }
        if(surfaceDefs.atEnd()) { looking=false; break; }
        surfaceDefs.advance();
      }
      if(surfdef) {
        res=surfdef->value;
      }
      //else DEBUG_OUT.warning("lookupSurfNum didn't find that surfdef!\n");
    }
    //else DEBUG_OUT.warning("lookupSurfNum didn't find that surf!\n");
  }
  //else DEBUG_OUT.warning("lookupSurfNum didn't find that map!\n");
  return res;
}

