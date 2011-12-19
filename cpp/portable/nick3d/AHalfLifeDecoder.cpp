
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream



#include <iostream>
#include <string.h>

#include <portable/nick3d/AFace3D.h>

#include <portable/nickcpp/ANullStream.h>

#include "AHalfLifeDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AHalfLifeDecoder Class
////////////////////////////////////////////////////////////////////////////////

AHalfLifeDecoder::AHalfLifeDecoder(const char *fname) : AMeshDecoder(fname)
{
  init();
  openFile(fname);
  readHeader();
  if(numbones) readBones();
  if(numbonecontrollers) readBoneControllers();
  if(numhitboxes) readHitBoxes();
  if(numseq) readSeqs();
  if(numseqgroups) readSeqGroups();
  if(numtextures) readTextures();
  if(numskinref*numskinfamilies) readSkins();
  if(numbodyparts) readBodyParts();
  if(numattachments) readAttachments();
  if(soundtable) readSounds();
  if(soundgroups) readSoundGroups();
  if(numtransitions) readTransitions();
}


/* STATIC */
bool AHalfLifeDecoder::recognize(uint8_t *str)
{
  bool ret=false;
  if(!strncmp("IDST",(char *)str,4)) return true;
  if(!strncmp("IDSQ",(char *)str,4)) return true;
  return ret;
}


void AHalfLifeDecoder::readHeader()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readHeader()...\n";
  fseek(fd,0,SEEK_SET);
  //
  unsigned long magic=readLongL();
  unsigned long vers=readLongL();  // Should always be 10?
  DEBUG_OUT<<"Half-Life MDL file version "<<vers<<"\n";
  std::cerr<<"*** Warning! Not really implemented! ***\n";
  char name[64];
  fread(name,64,1,fd);
  unsigned long length=readLongL();
  AVertexF3D eyeposition;
  readVectorL(&eyeposition);
  AVertexF3D min,max;
  readVectorL(&min);
  readVectorL(&max);
  AVertexF3D bbmin,bbmax;
  readVectorL(&bbmin);
  readVectorL(&bbmax);
  unsigned long flags=readLongL();
  //
  numbones=readLongL();
  bonesOffset=readLongL();
  numbonecontrollers=readLongL();
  bonecontrollersOffset=readLongL();
  //
  numhitboxes=readLongL();
  hitboxesOffset=readLongL();
  //
  numseq=readLongL();
  seqsOffset=readLongL();
  numseqgroups=readLongL();
  seqgroupsOffset=readLongL();
  //
  numtextures=readLongL();
  texturesOffset=readLongL();
  texturedatasOffset=readLongL();
  numskinref=readLongL();
  numskinfamilies=readLongL();
  skinsOffset=readLongL();
  //
  numbodyparts=readLongL();
  bodypartsOffset=readLongL();
  //
  numattachments=readLongL();
  attachmentsOffset=readLongL();
  //
  soundtable=readLongL();
  soundsOffset=readLongL();
  soundgroups=readLongL();
  soundgroupsOffset=readLongL();
  //
  numtransitions=readLongL();
  transitionsOffset=readLongL();
  //
  DEBUG_OUT<<"length="<<length<<"\n";
  DEBUG_OUT<<"flags="<<flags<<"\n";
  DEBUG_OUT<<"numbones="<<numbones<<"\n";
  DEBUG_OUT<<"bonesOffset="<<bonesOffset<<"\n";
  DEBUG_OUT<<"numbonecontrollers="<<numbonecontrollers<<"\n";
  DEBUG_OUT<<"bonecontrollersOffset="<<bonecontrollersOffset<<"\n";
  DEBUG_OUT<<"numhitboxes="<<numhitboxes<<"\n";
  DEBUG_OUT<<"hitboxesOffset="<<hitboxesOffset<<"\n";
  DEBUG_OUT<<"numseq="<<numseq<<"\n";
  DEBUG_OUT<<"seqsOffset="<<seqsOffset<<"\n";
  DEBUG_OUT<<"numseqgroups="<<numseqgroups<<"\n";
  DEBUG_OUT<<"seqgroupsOffset="<<seqgroupsOffset<<"\n";
  DEBUG_OUT<<"numtextures="<<numtextures<<"\n";
  DEBUG_OUT<<"texturesOffset="<<texturesOffset<<"\n";
  DEBUG_OUT<<"texturedatasOffset="<<texturedatasOffset<<"\n";
  DEBUG_OUT<<"numskinref="<<numskinref<<"\n";
  DEBUG_OUT<<"numskinfamilies="<<numskinfamilies<<"\n";
  DEBUG_OUT<<"skinsOffset="<<skinsOffset<<"\n";
  DEBUG_OUT<<"numbodyparts="<<numbodyparts<<"\n";
  DEBUG_OUT<<"bodypartsOffset="<<bodypartsOffset<<"\n";
  DEBUG_OUT<<"numattachments="<<numattachments<<"\n";
  DEBUG_OUT<<"attachmentsOffset="<<attachmentsOffset<<"\n";
  DEBUG_OUT<<"soundtable="<<soundtable<<"\n";
  DEBUG_OUT<<"soundsOffset="<<soundsOffset<<"\n";
  DEBUG_OUT<<"soundgroups="<<soundgroups<<"\n";
  DEBUG_OUT<<"soundgroupsOffset="<<soundgroupsOffset<<"\n";
  DEBUG_OUT<<"numtransitions="<<numtransitions<<"\n";
  DEBUG_OUT<<"transitionsOffset="<<transitionsOffset<<"\n";
}


void AHalfLifeDecoder::readBones()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readBones()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numbones<<" bones.\n";
  if(!numbones) return;
  fseek(fd,bonesOffset,SEEK_SET);
  char name[32];
  unsigned long parent,flags;
  unsigned long bc[6];
  float val[6];
  float scale[6];
  unsigned int j;
  for(unsigned int i=0;i<numbones;i++) {
    fread(name,32,1,fd);
    parent=readLongL();
    flags=readLongL();
    for(j=0;j<6;j++) bc[j]=readLongL();
    for(j=0;j<6;j++) val[j]=readFloatL();
    for(j=0;j<6;j++) scale[j]=readFloatL();
    DEBUG_OUT<<"bone "<<i<<": "<<name<<" "<<parent<<" "<<flags<<"\n";
  }
}


void AHalfLifeDecoder::readBoneControllers()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readBoneControllers()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numbonecontrollers<<" bonecontrollers.\n";
  if(!numbonecontrollers) return;
  fseek(fd,bonecontrollersOffset,SEEK_SET);
  unsigned long bone,type,rest,index;
  float start,end;
  for(unsigned int i=0;i<numbonecontrollers;i++) {
    bone=readLongL();
    type=readLongL();
    start=readFloatL();
    end=readFloatL();
    rest=readLongL();
    index=readLongL();
    DEBUG_OUT<<"bonecontroller "<<i<<": "<<bone<<" "<<type<<" "<<start<<","<<end<<" "<<rest<<" "<<index<<"\n";
  }
}


void AHalfLifeDecoder::readHitBoxes()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readHitBoxes()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numhitboxes<<" hitboxes.\n";
  if(!numhitboxes) return;
  fseek(fd,hitboxesOffset,SEEK_SET);
  unsigned long bone,group;
  AVertexF3D bbmin,bbmax;
  for(unsigned int i=0;i<numhitboxes;i++) {
    bone=readLongL();
    group=readLongL();
    readVectorL(&bbmin);
    readVectorL(&bbmax);
    DEBUG_OUT<<"hitbox "<<i<<": "<<bone<<" "<<group<<"\n";
  }
}


void AHalfLifeDecoder::readSeqs()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readSeqs()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numseq<<" seqs.\n";
  if(!numseq) return;
  fseek(fd,seqsOffset,SEEK_SET);
  for(unsigned int i=0;i<numseq;i++) {
    //
    DEBUG_OUT<<"seq "<<i<<": \n";
  }
}


void AHalfLifeDecoder::readSeqGroups()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readSeqGroups()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numseqgroups<<" seqgroups.\n";
  if(!numseqgroups) return;
  fseek(fd,seqgroupsOffset,SEEK_SET);
  char name[64];
  /*
  unsigned long vers,len;
  char magic[4];
  */
  char label[32];
  for(unsigned int i=0;i<numseqgroups;i++) {
  /*
    fread(magic,4,1,fd);
    vers=readLongL();
    fread(name,64,1,fd);
    len=readLongL();
    DEBUG_OUT<<"seqgroup "<<i<<": "<<magic<<" "<<vers<<" "<<len<<" "<<name<<"\n";
    */
    fread(label,32,1,fd);
    fread(name,64,1,fd);
    readLongL();  // cache pointer
    readLongL();  // hack for group 0?
    DEBUG_OUT<<"seqgroup "<<i<<": "<<label<<" "<<name<<"\n";
  }
}


void AHalfLifeDecoder::readTextures()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readTextures()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numtextures<<" textures.\n";
  if(!numtextures) return;
  fseek(fd,texturesOffset,SEEK_SET);
  for(unsigned int i=0;i<numtextures;i++) {
    //
    DEBUG_OUT<<"texture "<<i<<": \n";
  }
}


void AHalfLifeDecoder::readSkins()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readSkins()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numskinref<<" skins.\n";
  if(!numskinref) return;
  fseek(fd,skinsOffset,SEEK_SET);
  char name[64];
  unsigned long flags,width,height,index;
  for(unsigned int i=0;i<numskinref;i++) {
  /*
    fread(name,64,1,fd);
    flags=readLongL();
    width=readLongL();
    height=readLongL();
    index=readLongL();
    DEBUG_OUT<<"skin "<<i<<": "<<name<<" "<<flags<<" "<<width<<"x"<<height<<" "<<index<<"\n";
    */
  }
}


void AHalfLifeDecoder::readBodyParts()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readBodyParts()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numbodyparts<<" bodyparts.\n";
  if(!numbodyparts) return;
  fseek(fd,bodypartsOffset,SEEK_SET);
  char name[64];
  unsigned long nummodels,base,modelOffset;
  for(unsigned int i=0;i<numbodyparts;i++) {
    fread(name,64,1,fd);
    nummodels=readLongL();
    base=readLongL();
    modelOffset=readLongL();
    DEBUG_OUT<<"bodypart "<<i<<": "<<name<<" "<<nummodels<<" "<<base<<" "<<modelOffset<<"\n";
    for(unsigned int j=0;j<nummodels;j++) {
      fseek(fd,modelOffset,SEEK_SET);
      readModel();
    }
  }
}


void AHalfLifeDecoder::readAttachments()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readAttachments()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numattachments<<" attachments.\n";
  if(!numattachments) return;
  fseek(fd,attachmentsOffset,SEEK_SET);
  char name[32];
  unsigned long type,bone;
  AVertexF3D dummy;
  for(unsigned int i=0;i<numattachments;i++) {
    fread(name,32,1,fd);
    type=readLongL();
    bone=readLongL();
    readVectorL(&dummy);
    readVectorL(&dummy);
    readVectorL(&dummy);
    readVectorL(&dummy);
    DEBUG_OUT<<"attachment "<<i<<": "<<name<<" "<<type<<" "<<bone<<"\n";
  }
}


void AHalfLifeDecoder::readSounds()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readSounds()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<soundtable<<" sounds.\n";
  if(!soundtable) return;
  fseek(fd,soundsOffset,SEEK_SET);
  for(unsigned int i=0;i<soundtable;i++) {
    //
    DEBUG_OUT<<"sound "<<i<<": \n";
  }
}


void AHalfLifeDecoder::readSoundGroups()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readSoundGroups()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<soundgroups<<" soundgroups.\n";
  if(!soundgroups) return;
  fseek(fd,soundgroupsOffset,SEEK_SET);
  for(unsigned int i=0;i<soundgroups;i++) {
    //
    DEBUG_OUT<<"soundgroup "<<i<<": \n";
  }
}


void AHalfLifeDecoder::readTransitions()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readTransitions()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"There are "<<numtransitions<<" transitions.\n";
  if(!numtransitions) return;
  fseek(fd,transitionsOffset,SEEK_SET);
  for(unsigned int i=0;i<numtransitions;i++) {
    //
    DEBUG_OUT<<"transition "<<i<<": \n";
  }
}


// First sequence only, for now
void AHalfLifeDecoder::readSeqHeader()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readSeqHeader()...\n";
  fseek(fd,seqsOffset,SEEK_SET);
  char name[32];
  fread(name,32,1,fd);
  float fps=readFloatL();
  unsigned long flags=readLongL();
  //
  unsigned long activity=readLongL();
  unsigned long actweight=readLongL();
  //
  unsigned long numevents=readLongL();
  unsigned long eventsOffset=readLongL();
  //
  unsigned long numframes=readLongL();
  //
  unsigned long numpivots=readLongL();
  unsigned long pivotsOffset=readLongL();
  //
  unsigned long motiontype=readLongL();
  unsigned long motionbone=readLongL();
  AVertexF3D linearmovement;
  readVectorL(&linearmovement);
  unsigned long automoveposesOffset=readLongL();
  unsigned long automoveanglesOffset=readLongL();
  AVertexF3D bbmin,bbmax;
  readVectorL(&bbmin);
  readVectorL(&bbmax);
  //
  unsigned long numblends=readLongL();
  unsigned long animsOffset=readLongL();
  unsigned long blendtype1=readLongL();
  unsigned long blendtype2=readLongL();
  float blendstart1=readFloatL();
  float blendstart2=readFloatL();
  float blendend1=readFloatL();
  float blendend2=readFloatL();
  unsigned long blendparent=readLongL();
  //
  unsigned long seqgroup=readLongL();
  unsigned long entrynode=readLongL();
  unsigned long exitnode=readLongL();
  unsigned long nodeflags=readLongL();
  unsigned long nextseq=readLongL();
  //
  DEBUG_OUT<<"fps="<<fps<<"\n";
  DEBUG_OUT<<"flags="<<flags<<"\n";
  DEBUG_OUT<<"activity="<<activity<<"\n";
  DEBUG_OUT<<"actweight="<<actweight<<"\n";
  DEBUG_OUT<<"numevents="<<numevents<<"\n";
  DEBUG_OUT<<"eventsOffset="<<eventsOffset<<"\n";
  DEBUG_OUT<<"numframes="<<numframes<<"\n";
  DEBUG_OUT<<"numpivots="<<numpivots<<"\n";
  DEBUG_OUT<<"pivotsOffset="<<pivotsOffset<<"\n";
  DEBUG_OUT<<"motiontype="<<motiontype<<"\n";
  DEBUG_OUT<<"motionbone="<<motionbone<<"\n";
  DEBUG_OUT<<"automoveposesOffset="<<automoveposesOffset<<"\n";
  DEBUG_OUT<<"automoveanglesOffset="<<automoveanglesOffset<<"\n";
  DEBUG_OUT<<"numblends="<<numblends<<"\n";
  DEBUG_OUT<<"animsOffset="<<animsOffset<<"\n";
  DEBUG_OUT<<"blendtype1="<<blendtype1<<"\n";
  DEBUG_OUT<<"blendtype2="<<blendtype2<<"\n";
  DEBUG_OUT<<"blendstart1="<<blendstart1<<"\n";
  DEBUG_OUT<<"blendstart2="<<blendstart2<<"\n";
  DEBUG_OUT<<"blendend1="<<blendend1<<"\n";
  DEBUG_OUT<<"blendend2="<<blendend2<<"\n";
  DEBUG_OUT<<"blendparent="<<blendparent<<"\n";
  DEBUG_OUT<<"seqgroup="<<seqgroup<<"\n";
  DEBUG_OUT<<"entrynode="<<entrynode<<"\n";
  DEBUG_OUT<<"exitnode="<<exitnode<<"\n";
  DEBUG_OUT<<"nodeflags="<<nodeflags<<"\n";
  DEBUG_OUT<<"nextseq="<<nextseq<<"\n";
}


void AHalfLifeDecoder::readModel()
{
  DEBUG_OUT<<"AHalfLifeDecoder::readModel()...\n";
  char name[64];
  fread(name,64,1,fd);
  DEBUG_OUT<<"Model name is "<<name<<"\n";
  unsigned long type=readLongL();
  float boundingradius=readFloatL();
  //
  unsigned long nummesh=readLongL();
  unsigned long meshesOffset=readLongL();
  //
  unsigned long numverts=readLongL();
  unsigned long vertinfosOffset=readLongL();
  unsigned long vertsOffset=readLongL();
  //
  unsigned long numnorms=readLongL();
  unsigned long norminfosOffset=readLongL();
  unsigned long normsOffset=readLongL();
  //
  unsigned long numgroups=readLongL();
  unsigned long groupsOffset=readLongL();
  //
  DEBUG_OUT<<"type="<<type<<"\n";
  DEBUG_OUT<<"boundingradius="<<boundingradius<<"\n";
  DEBUG_OUT<<"nummesh="<<nummesh<<"\n";
  DEBUG_OUT<<"meshesOffset="<<meshesOffset<<"\n";
  DEBUG_OUT<<"numverts="<<numverts<<"\n";
  DEBUG_OUT<<"vertinfosOffset="<<vertinfosOffset<<"\n";
  DEBUG_OUT<<"vertsOffset="<<vertsOffset<<"\n";
  DEBUG_OUT<<"numnorms="<<numnorms<<"\n";
  DEBUG_OUT<<"norminfosOffset="<<norminfosOffset<<"\n";
  DEBUG_OUT<<"normsOffset="<<normsOffset<<"\n";
  DEBUG_OUT<<"numgroups="<<numgroups<<"\n";
  DEBUG_OUT<<"groupsOffset="<<groupsOffset<<"\n";
  //
  fseek(fd,meshesOffset,SEEK_SET);
  unsigned long numTri,triOffset,skinref,numnorms2,norms2Offset;
  // just first mesh for now
  for(unsigned int i=0;i<1;i++) {
    numTri=readLongL();
    triOffset=readLongL();
    skinref=readLongL();
    numnorms2=readLongL();
    norms2Offset=readLongL();
    DEBUG_OUT<<"mesh "<<i<<": "<<numTri<<" "<<triOffset<<" "<<skinref<<" "<<numnorms2<<" "<<norms2Offset<<"\n";
    fseek(fd,triOffset,SEEK_SET);
    unsigned short vertIndex,normIndex,x,y;
    AFace3D *theFace;
    for(unsigned int j=0;j<numTri;j++) {
      vertIndex=readShortL();
      normIndex=readShortL();
      x=readShortL();
      y=readShortL();
      DEBUG_OUT<<"face "<<j<<": "<<vertIndex<<" "<<normIndex<<" "<<x<<"x"<<y<<"\n";
      //theFace=new AFace3D;
      //theFace->a=vx; theFace->b=vy; theFace->c=vz;
      //theFace->surf=0;
      //faces.append(theFace);
    }
  }
  //
  fseek(fd,vertsOffset,SEEK_SET);
  float x,y,z;
  AVertexF3D *theVert;
  for(unsigned int j=0;j<numverts;j++) {
      x=readFloatL();
      y=readFloatL();
      z=readFloatL();
      DEBUG_OUT<<"vert "<<j<<": "<<x<<" "<<y<<" "<<z<<"\n";
      //theVert=new theVert;
      //theVert->a=vx; theVert->b=vy; theVert->c=vz;
      //faces.append(theVert);
    }
}

