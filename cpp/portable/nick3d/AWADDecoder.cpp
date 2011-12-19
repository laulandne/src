
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

#include "AWADDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AWADDecoder Class
////////////////////////////////////////////////////////////////////////////////

AWADDecoder::AWADDecoder(const char *fname) : AMapDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
}


/* STATIC */
bool AWADDecoder::recognize(uint8_t *str)
{
  DEBUG_OUT<<"AWADDecoder::recognize()...\n";
  bool ret=false;
  if(!strncmp((char *)str,"PWAD",4)) ret=true;
  if(!strncmp((char *)str,"IWAD",4)) ret=true;
  return ret;
}


void AWADDecoder::init()
{
  DEBUG_OUT<<"AWADDecoder::init()\n";
  pwad=false;
  numEntries=0;
  dirOffset=0;
  entryNames=(char **)NULL;
  entryLens=(uint32_t *)NULL;
  entryOffsets=(uint32_t *)NULL;
  numVertices=0;
  mapVerticesF=(AVertexF3D **)NULL;
  mapVerticesI=(AWADVertexI **)NULL;
  numSegs=0;
  mapSegs=(AWADSeg **)NULL;
  numSSectors=0;
  mapSSectors=(AWADSSector **)NULL;
  numLineDefs=0;
  mapLineDefs=(AWADLineDef **)NULL;
  numSideDefs=0;
  mapSideDefs=(AWADSideDef **)NULL;
  numThings=0;
  mapThings=(AWADThing **)NULL;
  numNodes=0;
  mapNodes=(AWADNode **)NULL;
  mapMesh=(AMesh3D *)NULL;
  playerX=0;  playerY=0;  playerAngle=0;
}



unsigned int AWADDecoder::getNumMaps()
{
  return numMaps;
}


void AWADDecoder::setMapNumber(unsigned int n)
{
  if(n<(numMaps+1)) curMapNum=n;
}



void AWADDecoder::readObjects()
{
  DEBUG_OUT<<"AWADDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0L,SEEK_SET);
  char sig=readByte();
  readByte(); readByte(); readByte();  // skip next 3...
  if(sig=='P') pwad=true;
  if(pwad) DEBUG_OUT<<"(This is a PWAD, not an IWAD)\n";
  numEntries=readLongL();
  dirOffset=readLongL();
  DEBUG_OUT<<"numEntries="<<numEntries<<"\n";
  DEBUG_OUT<<"dirOffset="<<dirOffset<<"\n";
  entryNames=(char **)calloc(numEntries,sizeof(char *));
  for(unsigned int t=0;t<numEntries;t++) {
    entryNames[t]=(char *)calloc(9,1);
  }
  entryOffsets=(uint32_t *)calloc(numEntries,sizeof(uint32_t));
  entryLens=(uint32_t *)calloc(numEntries,sizeof(uint32_t));
  readDirectory();
  buildMapArray();
  setMapNumber(0);
  readTheCurrentMap();
  buildMapMesh();
  DEBUG_OUT<<"AWADDecoder::readObjects() done.\n";
}


void AWADDecoder::readDirectory()
{
  DEBUG_OUT<<"AWADDecoder::readDirectory()...\n";
  fseek(fd,dirOffset,SEEK_SET);
  unsigned int t;
  for(t=0;t<numEntries;t++) {
    entryOffsets[t]=readLongL();
    entryLens[t]=readLongL();
    for(unsigned int i=0;i<8;i++) entryNames[t][i]=readByte();
  }
  /*
  for(t=0;t<numEntries;t++) {
    DEBUG_OUT<<t<<": ";
    DEBUG_OUT<<"entryNames=\""<<entryNames[t]<<"\"";
    DEBUG_OUT<<" entryOffsets="<<entryOffsets[t]<<" entryLens="<<entryLens[t]<<"\n";
  }
  */
}


void AWADDecoder::buildMapArray()
{
  DEBUG_OUT<<"AWADDecoder::buildMapArray()...\n";
  unsigned int t=0,i=0;
  bool reading=true;
  while(reading) {
    if(isNameAMap(entryNames[t])) {
      //DEBUG_OUT<<"Found map \""<<entryNames[t]<<"\"at entry #"<<t<<"\n";
      mapEntries[i]=t;
      i++;
      if(i==64) {
        DEBUG_OUT<<"More than 64 maps!  Giving up!\n";
        return;
      }
    }
    t++;
    if(t==numEntries) reading=false;
  }
  numMaps=i;
  DEBUG_OUT<<"There were "<<numMaps<<" maps total.\n";
}


// TODO: Make this slightly more sophisticated
bool AWADDecoder::isNameAMap(char *name)
{
  if((name[0]=='E')&&(name[2]=='M')) {
    if(strlen(name)==4) {
      return true;
    }
  }
  if((name[0]=='M')&&(name[1]=='A')&&(name[2]=='P')) {
    if(strlen(name)==5) {
      return true;
    }
  }
  return false;
}


bool AWADDecoder::readTheCurrentMap()
{
  DEBUG_OUT<<"AWADDecoder::readTheCurrentMap()...\n";
  return readMapEntry(mapEntries[curMapNum]);
}


bool AWADDecoder::readMapEntry(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readMapEntry("<<entry<<")...\n";
  DEBUG_OUT<<"Map name: "<<entryNames[entry]<<"\n";
  unsigned int n=entry+1;
  bool reading=true;
  while(reading) {
    if(isNameAMap(entryNames[n])) { reading=false; }
    else {
      if(!strcmp(entryNames[n],"THINGS")) { DEBUG_OUT<<"Found THINGS!\n"; readThings(n); }
      if(!strcmp(entryNames[n],"LINEDEFS")) { DEBUG_OUT<<"Found LINEDEFS!\n"; readLineDefs(n); }
      if(!strcmp(entryNames[n],"SIDEDEFS")) { DEBUG_OUT<<"Found SIDEDEFS!\n"; readSideDefs(n); }
      if(!strcmp(entryNames[n],"VERTEXES")) { DEBUG_OUT<<"Found VERTEXES!\n"; readVertices(n); }
      if(!strcmp(entryNames[n],"SEGS")) { DEBUG_OUT<<"Found SEGS!\n"; readSegs(n); }
      if(!strcmp(entryNames[n],"SSECTORS")) { DEBUG_OUT<<"Found SSECTORS!\n"; readSSectors(n); }
      if(!strcmp(entryNames[n],"NODES")) { DEBUG_OUT<<"Found NODES!\n"; readNodes(n); }
      if(!strcmp(entryNames[n],"SECTORS")) { DEBUG_OUT<<"Found SECTORS!\n"; readSectors(n); }
      if(!strcmp(entryNames[n],"REJECT")) { DEBUG_OUT<<"Found REJECT!\n"; readReject(n); }
      if(!strcmp(entryNames[n],"BLOCKMAP")) { DEBUG_OUT<<"Found BLOCKMAP!\n"; readBlockMap(n); }
    }
    n++;
  }
  DEBUG_OUT<<"AWADDecoder::readMapEntry("<<entry<<") done.\n";
  return true;
}


void AWADDecoder::readVertices(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readVertices("<<entry<<")...\n";
  numVertices=entryLens[entry]/4;
  DEBUG_OUT<<"numVertices="<<numVertices<<"\n";
  mapVerticesF=(AVertexF3D **)calloc(sizeof(AVertexF3D *),numVertices);
  if(!mapVerticesF) { std::cerr<<"Couldn't calloc mapVerticesF!\n"; return; }
  mapVerticesI=(AWADVertexI **)calloc(sizeof(AWADVertexI *),numVertices);
  if(!mapVerticesI) { std::cerr<<"Couldn't calloc mapVerticesI!\n"; return; }
  fseek(fd,entryOffsets[entry],SEEK_SET);
  AVertexF3D *vf;
  AWADVertexI *vi;
  int16_t sx,sy;
  for(unsigned int t=0;t<numVertices;t++) {
    vf=new AVertexF3D;
    if(!vf) { std::cerr<<"Couldn't get new AVertexF3D!\n"; return; }
    vi=new AWADVertexI;
    if(!vi) { std::cerr<<"Couldn't get new AWADVertexI!\n"; return; }
    sx=readSShortL();  sy=readSShortL();
    vi->x=sx;  vi->y=sy;
    if(sx&0x8000) vi->x=-vi->x;
    if(sy&0x8000) vi->y=-vi->y;
    vf->x=0.0;  vf->y=0.0;  vf->z=0.0;
    if(vi->x) vf->x=((float)vi->x)/32768.0F;
    if(vi->y) vf->y=-((float)vi->y)/32768.0F;
    // NOTE: vi->y is NOT corrected!
    mapVerticesF[t]=vf;
    mapVerticesI[t]=vi;
    //DEBUG_OUT<<t<<": float "<<vf->x<<","<<vf->y<<"  int "<<vi->x<<","<<vi->y<<"  (was "<<sx<<","<<sy<<")\n";
  }
}


void AWADDecoder::readSegs(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readSegs("<<entry<<")...\n";
  numSegs=entryLens[entry]/12;
  DEBUG_OUT<<"numSegs="<<numSegs<<"\n";
  mapSegs=(AWADSeg **)calloc(sizeof(AWADSeg *),numSegs);
  if(!mapSegs) { std::cerr<<"Couldn't calloc mapSegs!\n"; return; }
  fseek(fd,entryOffsets[entry],SEEK_SET);
  AWADSeg *s;
  for(unsigned int t=0;t<numSegs;t++) {
    s=new AWADSeg;
    if(!s) { std::cerr<<"Couldn't get new AWADSeg!\n"; return; }
    s->start=readShortL();
    s->end=readShortL();
    s->angle=readShortL();
    s->top=readShortL();
    s->side=readShortL();
    s->offset=readShortL();
    mapSegs[t]=s;
    //DEBUG_OUT<<t<<": start="<<s->start<<" end="<<s->end<<" angle="<<s->angle<<" top="<<s->top<<" side="<<s->side<<" offset="<<s->offset<<"\n";
  }
}


void AWADDecoder::readSectors(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readSectors("<<entry<<")...\n";
  numSectors=entryLens[entry]/26;
  DEBUG_OUT<<"numSectors="<<numSectors<<"\n";
  mapSectors=(AWADSector **)calloc(sizeof(AWADSector *),numSectors);
  if(!mapSectors) { std::cerr<<"Couldn't calloc mapSectors!\n"; return; }
  fseek(fd,entryOffsets[entry],SEEK_SET);
  AWADSector *s;
  for(unsigned int t=0;t<numSectors;t++) {
    s=new AWADSector;
    if(!s) { std::cerr<<"Couldn't get new AWADSector!\n"; return; }
    s->floorht=readShortL();
    s->ceilinght=readShortL();
    memset(s->floortx,0,9);
    readString(s->floortx,8);
    memset(s->ceilingtx,0,9);
    readString(s->ceilingtx,8);
    s->brightness=readShortL();
    s->special=readShortL();
    s->tag=readShortL();
    mapSectors[t]=s;
    //DEBUG_OUT<<t<<": floorht="<<s->floorht<<" ceilinght="<<s->ceilinght<<" brightness="<<s->brightness<<" special="<<s->special<<" tag="<<s->tag<<"\n";
    //DEBUG_OUT<<"  : floortx="<<s->floortx<<" ceilingtx="<<s->ceilingtx<<"\n";
  }
}


void AWADDecoder::readSSectors(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readSSectors("<<entry<<")...\n";
  numSSectors=entryLens[entry]/4;
  DEBUG_OUT<<"numSSectors="<<numSSectors<<"\n";
  mapSSectors=(AWADSSector **)calloc(sizeof(AWADSSector *),numSSectors);
  if(!mapSSectors) { std::cerr<<"Couldn't calloc mapSSectors!\n"; return; }
  fseek(fd,entryOffsets[entry],SEEK_SET);
  AWADSSector *s;
  for(unsigned int t=0;t<numSSectors;t++) {
    s=new AWADSSector;
    if(!s) { std::cerr<<"Couldn't get new AWADSSector!\n"; return; }
    s->numSegs=readShortL();
    s->firstSeg=readShortL();
    mapSSectors[t]=s;
    //DEBUG_OUT<<t<<": numSegs="<<s->numSegs<<" firstSeg="<<s->firstSeg<<"\n";
  }
}


void AWADDecoder::readLineDefs(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readLineDefs("<<entry<<")...\n";
  numLineDefs=entryLens[entry]/14;
  DEBUG_OUT<<"numLineDefs="<<numLineDefs<<"\n";
  mapLineDefs=(AWADLineDef **)calloc(sizeof(AWADLineDef *),numLineDefs);
  if(!mapLineDefs) { std::cerr<<"Couldn't calloc mapLineDefs!\n"; return; }
  fseek(fd,entryOffsets[entry],SEEK_SET);
  AWADLineDef *l;
  for(unsigned int t=0;t<numLineDefs;t++) {
    l=new AWADLineDef;
    if(!l) { std::cerr<<"Couldn't get new AWADLineDef!\n"; return; }
    l->lfrom=readShortL();
    l->lto=readShortL();
    l->attr=readShortL();
    l->special=readShortL();
    l->tag=readShortL();
    l->side1=readShortL();
    l->side2=readShortL();
    mapLineDefs[t]=l;
    //DEBUG_OUT<<t<<": lfrom="<<l->lfrom<<" lto="<<l->lto<<" attr="<<l->attr<<" special="<<l->special<<" tag="<<l->tag<<" side1="<<l->side1<<" side2="<<l->side2<<"\n";
  }
}


void AWADDecoder::readSideDefs(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readSideDefs("<<entry<<")...\n";
  numSideDefs=entryLens[entry]/30;
  DEBUG_OUT<<"numSideDefs="<<numSideDefs<<"\n";
  mapSideDefs=(AWADSideDef **)calloc(sizeof(AWADSideDef *),numSideDefs);
  if(!mapSideDefs) { std::cerr<<"Couldn't calloc mapSideDefs!\n"; return; }
  fseek(fd,entryOffsets[entry],SEEK_SET);
  AWADSideDef *s;
  for(unsigned int t=0;t<numSideDefs;t++) {
    s=new AWADSideDef;
    if(!s) { std::cerr<<"Couldn't get new AWADSideDef!\n"; return; }
    s->x=readShortL();
    s->y=readShortL();
    memset(s->upper,0,9);
    readString(s->upper,8);
    memset(s->lower,0,9);
    readString(s->lower,8);
    memset(s->normal,0,9);
    readString(s->normal,8);
    s->sector=readShortL();
    mapSideDefs[t]=s;
    //DEBUG_OUT<<t<<": x="<<s->x<<" y="<<s->y<<" upper="<<s->upper<<" lower="<<s->lower<<" normal="<<s->normal<<" sector="<<s->sector<<"\n";
  }
}


void AWADDecoder::readReject(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readReject("<<entry<<")...\n";
  fseek(fd,entryOffsets[entry],SEEK_SET);
  // variable size...
  DEBUG_OUT<<"(Reading not implemented yet...)\n";
}


void AWADDecoder::readBlockMap(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readBlockMap("<<entry<<")...\n";
  fseek(fd,entryOffsets[entry],SEEK_SET);
  // variable size...
  DEBUG_OUT<<"(Reading not implemented yet...)\n";
}


void AWADDecoder::readThings(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readThings("<<entry<<")...\n";
  numThings=entryLens[entry]/10;
  DEBUG_OUT<<"numThings="<<numThings<<"\n";
  mapThings=(AWADThing **)calloc(sizeof(AWADThing *),numThings);
  if(!mapThings) { std::cerr<<"Couldn't calloc mapThings!\n"; return; }
  fseek(fd,entryOffsets[entry],SEEK_SET);
  AWADThing *th;
  for(unsigned int t=0;t<numThings;t++) {
    th=new AWADThing;
    if(!th) { std::cerr<<"Couldn't get new AWADThing!\n"; return; }
    th->x=readShortL();
    th->y=readShortL();
    th->angle=readShortL();
    th->type=readShortL();
    th->attr=readShortL();
    mapThings[t]=th;
    //DEBUG_OUT<<t<<": x="<<th->x<<" y="<<th->y<<" angle="<<th->angle<<" type="<<th->type<<" attr="<<th->attr<<"\n";
    if(th->type==1) {
      DEBUG_OUT<<"(Found the player start!\n";
      playerX=th->x;  playerY=th->y;  playerAngle=th->angle;
    }
  }
}


void AWADDecoder::readNodes(unsigned int entry)
{
  DEBUG_OUT<<"AWADDecoder::readNodes("<<entry<<")...\n";
  numNodes=entryLens[entry]/14;
  DEBUG_OUT<<"numNodes="<<numNodes<<"\n";
  mapNodes=(AWADNode **)calloc(sizeof(AWADNode *),numNodes);
  if(!mapNodes) { std::cerr<<"Couldn't calloc mapNodes!\n"; return; }
  fseek(fd,entryOffsets[entry],SEEK_SET);
  AWADNode *n;
  for(unsigned int t=0;t<numNodes;t++) {
    n=new AWADNode;
    if(!n) { std::cerr<<"Couldn't get new AWADNode!\n"; return; }
    n->xstart=readShortL();
    n->ystart=readShortL();
    n->dx=readShortL();
    n->dy=readShortL();
    //
    n->yup1=readShortL();
    n->ylower1=readShortL();
    n->xlower1=readShortL();
    n->xup1=readShortL();
    //
    n->yup2=readShortL();
    n->ylower2=readShortL();
    n->xlower2=readShortL();
    n->xup2=readShortL();
    //
    n->right=readShortL();
    n->left=readShortL();
    //
    mapNodes[t]=n;
    //DEBUG_OUT<<t<<": xstart="<<n->xstart<<" ystart="<<n->ystart<<" dx="<<n->dx<<" dy="<<n->dy<<"\n";
    //DEBUG_OUT<<"  : yup1="<<n->yup1<<" ylower1="<<n->ylower1<<" xlower1="<<n->xlower1<<" xup1="<<n->xup1<<"\n";
    //DEBUG_OUT<<"  : yup2="<<n->yup2<<" ylower2="<<n->ylower2<<" xlower2="<<n->xlower2<<" xup2="<<n->xup2<<"\n";
    //DEBUG_OUT<<"  : right="<<n->right<<" left="<<n->left<<"\n";
  }
}


void AWADDecoder::buildMapMesh()
{
  DEBUG_OUT<<"AWADDecoder::buildMapMesh()...\n";
  ABrush2D *b=NULL;
  AVertexF3D *v=NULL;
  float lineHeight=AWADDECODER_UNIT*AWADDECODER_PLAYER_HEIGHT*6;
  numBrushes=0;
  float br=0.0,bg=0.0,bb=0.0;
  for(unsigned int t=0;t<numLineDefs;t++) {
    br=0.3; bg=0.3; bb=0.3;
    if(mapLineDefs[t]->attr&AWADDECODER_LINEDEF_BLOCKBOTH) { br+=0.2;  bg+=0.2;  bb+=0.2; }
    //if(mapLineDefs[t]->attr&AWADDECODER_LINEDEF_BLOCKMONSTERS) { br+=0.2;  bg+=0.2;  bb+=0.2; }
    if(mapLineDefs[t]->attr&AWADDECODER_LINEDEF_BLOCKALL) { br+=0.2;  bg+=0.2;  bb+=0.2; }
    b=new ABrush2D;
    v=mapVerticesF[mapLineDefs[t]->lfrom];
    b->v[0].x=v->x;  b->v[0].y=v->y;  b->v[0].z=0.0;
    b->v[3].x=v->x;  b->v[3].y=v->y;  b->v[3].z=lineHeight;
    v=mapVerticesF[mapLineDefs[t]->lto];
    b->v[1].x=v->x;  b->v[1].y=v->y;  b->v[1].z=0.0;
    b->v[2].x=v->x;  b->v[2].y=v->y;  b->v[2].z=lineHeight;
    b->fRed=br;  b->fGreen=bg;  b->fBlue=bb;
    //DEBUG_OUT<<"brush: "<<br<<" "<<bg<<" "<<bb<<"\n";
    brushes.append(b);
    numBrushes++;
  }
  brushes2Mesh();
}

