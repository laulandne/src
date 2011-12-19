
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <string.h>
#include <stdlib.h>


#include <portable/nickcpp/ABitmap.h>

#include <portable/ndecoder/ADecoder.h>
#include <portable/ndecoder/ASmartDecoder.h>
#include <portable/ndecoder/AImageDecoder.h>


ABitmap *theImage=(ABitmap *)NULL;
char *theFilename=NULL;


bool handleImage(char *fname,AImageDecoder *decoder);


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


bool handleImage(char *fname,AImageDecoder *decoder)
{
  DEBUG_OUT<<"handleImage called...\n";
  if(!fname) { std::cerr<<"no fname!\n"; return false; }
  if(!decoder) { std::cerr<<"no decoder!\n"; return false; }
  if(decoder->err.error()) { std::cerr<<"decoder error!\n"; return false; }
  bool ret=false;
  //
  bool rgbReorder=decoder->getRGBReorder();
  APalette *colors=decoder->getColors();
  uint8_t *lineBuffer=decoder->getLineBuffer();
  unsigned int lineBufferSize=decoder->getLineBufferSize();
  DEBUG_OUT<<"Going to call createNewBitmapObject()...\n";
  theImage=decoder->createNewBitmapObject();
  DEBUG_OUT<<"(back from createNewBitmapObject)\n";
  if(!theImage) { std::cerr<<"no theImage!\n"; return false; }
  if(theImage->err.error()) { std::cerr<<"theImage error!\n"; return false; }
  //
  theImage->convert(24,A_BITMAP_CHUNKY,NULL);
  //
  DEBUG_OUT<<"Decoder is "<<decoder->getWidth()<<"x"<<decoder->getHeight()<<"\n";
  DEBUG_OUT<<"Image is "<<theImage->getWidth()<<"x"<<theImage->getHeight()<<"\n";
  ret=true;
  //
  return ret;
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  //
  if(argc>1) theFilename=argv[1];
  if(!theFilename) {
    std::cerr<<"usage: "<<argv[0]<<" {filename}\n";
    return 5;
  }
  //
  bool working=false;
  ADecoder *dec=(ADecoder *)NULL;
  DEBUG_OUT<<"Going to call ASmartDecoder::pickDecoder...\n";
  dec=ASmartDecoder::pickBestDecoder(theFilename);
  DEBUG_OUT<<"(back from pickDecoder)\n";
  if(!dec) {
    std::cerr<<"Couldn't recognize file type of that file!\n";
    return 5;
  }
  if(dec->err.error()) {
    std::cerr<<"Problem decoding file!\n";
    return 5;
  }
  if(dec->isImage()) {
    DEBUG_OUT<<"isImage is true.\n";
    working=handleImage(theFilename,(AImageDecoder *)dec);
  }
  else {
    std::cerr<<"Not an image file!\n";
    return 5;
  }
  //
  unsigned int outLen=strlen(theFilename)+2;
  char *outName=(char *)calloc(outLen,1);
  strncpy(outName,theFilename,outLen);
  strncat(outName,".h",2);
  std::cerr<<"outName is \""<<outName<<"\"\n";
  /*
  if(theScene->getNumMeshes()>1) {
    std::cerr<<"Not doing anything, please specify only a single model!\n";
    exit(5);
  }
  if(theScene->getNumMeshes()<1) {
    std::cerr<<"Not doing anything, couldn't find a model!\n";
    exit(5);
  }
  AMesh3D *ms=theScene->getMeshes()[0];
  */
  FILE *fout=fopen(outName,"w");
  if(fout) {
    fprintf(fout,"#ifdef IMAGE2H_H\n");
    fprintf(fout,"#define IMAGE2H_H\n");
    fprintf(fout,"\n");
/*
    fprintf(fout,"const int num_teapot_indices = %d;\n",ms->getNumFaces());
    fprintf(fout,"const int num_teapot_vertices = %d;\n",ms->getNumVerts());
    AVertex3D *n=ms->getFaceNormals();
    unsigned int nn=0;
    if(n) nn=ms->getNumFaces(); 
    fprintf(fout,"const int num_teapot_normals = %d;\n",nn);
    fprintf(fout,"\n");
    fprintf(fout,"float teapot_vertices [] = {\n");
    AVertex3D **v=ms->getVerts();
    for(unsigned int t=0;t<ms->getNumVerts();t++) {
      fprintf(fout,"  %f,%f,%f",v[0][t].x,v[0][t].y,v[0][t].z);
      if(t<(ms->getNumVerts()-1)) fprintf(fout,","); 
      fprintf(fout,"\n");
    }
    fprintf(fout,"};\n");
    fprintf(fout,"\n");
    fprintf(fout,"float teapot_normals [] = {\n");
    for(unsigned int t=0;t<nn;t++) {
      fprintf(fout,"  %f,%f,%f",n[t].x,n[t].y,n[t].z);
      if(t<(nn-1)) fprintf(fout,","); 
      fprintf(fout,"\n");
    }
    fprintf(fout,"};\n");
    fprintf(fout,"\n");
    fprintf(fout,"short teapot_indices [] = {\n");
    AFace3D **f=ms->getFaces();
    for(unsigned int t=0;t<ms->getNumFaces();t++) {
      fprintf(fout,"  %d,%d,%d",f[t]->a,f[t]->b,f[t]->c);
      if(t<(ms->getNumFaces()-1)) fprintf(fout,","); 
      fprintf(fout,"\n");
    }
    fprintf(fout,"};\n");
    fprintf(fout,"\n");
    fprintf(fout,"short new_teapot_indices [] = {\n");
    for(unsigned int t=0;t<ms->getNumFaces();t++) {
      fprintf(fout,"  3,%d,%d,%d",f[t]->a,f[t]->b,f[t]->c);
      if(t<(ms->getNumFaces()-1)) fprintf(fout,","); 
      fprintf(fout,"\n");
    }
    fprintf(fout,"};\n");
    fprintf(fout,"\n");
*/
    fprintf(fout,"\n");
    fprintf(fout,"#endif // IMAGE2H_H\n");
  }
  else {
    std::cerr<<"Couldn't create output file!\n";
  }
  //
  exit(0);
  return 0;
}

