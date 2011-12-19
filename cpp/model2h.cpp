
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <string.h>
#include <stdlib.h>


#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/APalette.h>

#include <portable/nick3d/AMesh3D.h>
#include <portable/nick3d/AScene3D.h>

#include <portable/nick3d/ASmartMeshDecoder.h>
#include <portable/nick3d/ASmartSceneDecoder.h>
#include <portable/nick3d/AMeshDecoder.h>
#include <portable/nick3d/ASceneDecoder.h>



AScene3D *theScene=(AScene3D *)NULL;
AMesh3D *theMesh=(AMesh3D *)NULL;
char *theFilename=NULL;


bool handleMesh(char *fname,AMeshDecoder *decoder);
bool handleScene(char *fname,ASceneDecoder *decoder);


#ifdef USE_JAVACPP
void copyMesh2Model(AMesh3D *ms,Model3D *nm,unsigned int vertOffset);
#endif // USE_JAVACPP


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


bool handleScene(char *fname,ASceneDecoder *decoder)
{
  if(!fname) return false;
  if(!decoder) return false;
  bool ret=true;
  DEBUG_OUT<<"handleScene called!\n";
  //
  theScene=decoder->createNewSceneObject();
  //theScene->isValid();
  //theRenderer=new ASoftRenderer3D();
  //if(theWindow) theRenderer->setViewSize(theWindow->getWidth());
  //theRenderer->resizeToFit(theScene);
  // TODO: Check for error?
  //
  // TODO: Write output file here...
  //
  return ret;
}


bool handleMesh(char *fname,AMeshDecoder *decoder)
{
  if(!fname) return false;
  if(!decoder) return false;
  bool ret=true;
  DEBUG_OUT<<"handleMesh called!\n";
  //
  theMesh=decoder->createNewMeshObject();
  theMesh->isValid();
  //theMesh->normalizeScale();
  //theMesh->normalizeTranslation();
  theScene=new AScene3D();
  theScene->resizeToFit(theMesh);
  theScene->addMesh(theMesh);
  //theRenderer=new ASoftRenderer3D();
  //if(theWindow) theRenderer->setViewSize(theWindow->getWidth());
  //theRenderer->resizeToFit(theMesh);
  // TODO: check for error?
  //
  return ret;
}


int main(int argc, char **argv)
{
  char msg[256];
  //
  if(argc>1) theFilename=argv[1];
  if(!theFilename) {
    char msg[256];
    sprintf(msg,(char *)"usage: %s {model filename}",argv[0]);
    std::cerr<<"Error! "<<msg<<"\n";
    return 5;
  }
  //
  bool working=true;
  //while(working) {
    ADecoder *dec=(ADecoder *)NULL;
    if(!dec) { dec=ASmartMeshDecoder::pickMeshDecoder(theFilename); }
    if(!dec) { dec=ASmartSceneDecoder::pickSceneDecoder(theFilename); }
    if(!dec) {
      DEBUG_OUT<<"Couldn't recognize file type of that file\n";
      sprintf(msg,(char *)"Couldn't recognize file type of %s!\n",theFilename);
      std::cerr<<"Error! "<<msg<<"\n";
      exit(5);
    }
    if(dec->isMesh()) {
      DEBUG_OUT<<"isMesh is true.\n";
      working=handleMesh(theFilename,(AMeshDecoder *)dec);
    }
    if(dec->isScene()) {
      DEBUG_OUT<<"isScene is true.\n";
      working=handleScene(theFilename,(ASceneDecoder *)dec);
    }
  //}
  //
  unsigned int outLen=strlen(theFilename)+2;
  char *outName=(char *)calloc(outLen,1);
  strncpy(outName,theFilename,outLen);
  strncat(outName,".h",2);
  std::cerr<<"outName is \""<<outName<<"\"\n";
  if(theScene->getNumMeshes()>1) {
    std::cerr<<"Not doing anything, please specify only a single model!\n";
    exit(5);
  }
  if(theScene->getNumMeshes()<1) {
    std::cerr<<"Not doing anything, couldn't find a model!\n";
    exit(5);
  }
  AMesh3D *ms=theScene->getMeshes()[0];
  FILE *fout=fopen(outName,"w");
  if(fout) {
    fprintf(fout,"#ifndef TEAPOT_H\n");
    fprintf(fout,"#define TEAPOT_H\n");
    fprintf(fout,"\n");
    fprintf(fout,"const int num_teapot_indicies = %d;\n",ms->getNumFaces());
    fprintf(fout,"const int num_teapot_vertices = %d;\n",ms->getNumVerts());
    AVertexF3D *n=ms->getFaceNormals();
    unsigned int nn=0;
    if(n) nn=ms->getNumFaces(); 
    fprintf(fout,"const int num_teapot_normals = %d;\n",nn);
    fprintf(fout,"\n");
    fprintf(fout,"float teapot_vertices [] = {\n");
    AVertexF3D **v=ms->getVerts();
    for(unsigned int t=0;t<ms->getNumVerts();t++) {
      fprintf(fout,"  %f,%f,%f",v[0][t].x,v[0][t].y,v[0][t].z);
      if(t<(ms->getNumVerts()-1)) fprintf(fout,","); 
      fprintf(fout,"\n");
    }
	if(!ms->getNumVerts()) {
	  fprintf(fout,"  0.0,0.0,0.0  // Because Visual Studio won't let you have a zero sized array...\n");
	}
    fprintf(fout,"};\n");
    fprintf(fout,"\n");
    fprintf(fout,"float teapot_normals [] = {\n");
    for(unsigned int t=0;t<nn;t++) {
      fprintf(fout,"  %f,%f,%f",n[t].x,n[t].y,n[t].z);
      if(t<(nn-1)) fprintf(fout,","); 
      fprintf(fout,"\n");
    }
	if(!nn) {
	  fprintf(fout,"  0.0,0.0,0.0  // Because Visual Studio won't let you have a zero sized array...\n");
	}
    fprintf(fout,"};\n");
    fprintf(fout,"\n");
    fprintf(fout,"short teapot_indicies [] = {\n");
    AFace3D **f=ms->getFaces();
    for(unsigned int t=0;t<ms->getNumFaces();t++) {
      fprintf(fout,"  %d,%d,%d",f[t]->a,f[t]->b,f[t]->c);
      if(t<(ms->getNumFaces()-1)) fprintf(fout,","); 
      fprintf(fout,"\n");
    }
	if(!ms->getNumFaces()) {
	  fprintf(fout,"  0,0,0  // Because Visual Studio won't let you have a zero sized array...\n");
	}
    fprintf(fout,"};\n");
    fprintf(fout,"\n");
    fprintf(fout,"short new_teapot_indicies [] = {\n");
    for(unsigned int t=0;t<ms->getNumFaces();t++) {
      fprintf(fout,"  3,%d,%d,%d",f[t]->a,f[t]->b,f[t]->c);
      if(t<(ms->getNumFaces()-1)) fprintf(fout,","); 
      fprintf(fout,"\n");
    }
	if(!ms->getNumFaces()) {
	  fprintf(fout,"  3,0,0,0  // Because Visual Studio won't let you have a zero sized array...\n");
	}
    fprintf(fout,"};\n");
    fprintf(fout,"\n");
    fprintf(fout,"\n");
    fprintf(fout,"#endif // TEAPOT_H\n");
  }
  else {
    std::cerr<<"Couldn't create output file!\n";
  }
  //
  return 0;
}

