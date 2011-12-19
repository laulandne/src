
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


// NOTE: iPhone is 320x480

#define THE_WINDOW_WIDTH 1024
#define THE_WINDOW_HEIGHT 600


#include <iostream>


//#define NICK_INLINE_HACK 1
#define USE_JAVACPP 1
//#define NICK_USE_GUILIB 1


//#define THE_FILENAME_TO_LOAD "/Users/nick/src/nick/dinasaur.obj"
//#define THE_FILENAME_TO_LOAD "/Users/nick/chapter4.map"
//#define THE_FILENAME_TO_LOAD "/home/nick/doom.wad"

#define THE_FILENAME_TO_LOAD "/Users/nick/doom.wad"
//#define THE_FILENAME_TO_LOAD "/Users/nick/other/fo_3dm/Chapter3/hellpig.md2"
//#define THE_FILENAME_TO_LOAD "/Users/nick/other/fo_3dm/Chapter4/lantern_genie.lw_obj"
//#define THE_FILENAME_TO_LOAD "/Users/nick/other/fo_3dm/Chapter7/jeep1.3ds"
//#define THE_FILENAME_TO_LOAD "/Users/nick/other/fo_3dm/Chapter8/evh.mdl"
//#define THE_FILENAME_TO_LOAD "/Users/nick/other/fo_3dm/Chapter9/chicken_head.md3"
//#define THE_FILENAME_TO_LOAD "/Users/nick/other/fo_3dm/Chapter9/chicken_upper.md3"
//#define THE_FILENAME_TO_LOAD "/Users/nick/other/fo_3dm/Chapter9/chicken_lower.md3"


//#include <cube.obj.h>
//#include <hughes_500.obj.h>
//#include <knoxS.obj.h>
//#include <dinasaur.obj.h>
//#include <warthog.obj.h>


#ifdef NICK_USE_GUILIB
#include <guilib/nickguilib.h>
#endif // NICK_USE_GUILIB

#ifdef NICK_USE_SDL
#include <sdl/nicksdl.h>
#endif // NICK_USE_SDL

#ifdef NICK_USE_OPENGL
#include <glut/nickglut.h>
#endif // NICK_USE_OPENGL


#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/APalette.h>

#include <portable/nick3d/AMesh3D.h>
#include <portable/nick3d/AScene3D.h>

#ifndef NICK_INLINE_HACK
#include <portable/nick3d/ASmartMeshDecoder.h>
#include <portable/nick3d/ASmartSceneDecoder.h>
#include <portable/nick3d/AMeshDecoder.h>
#include <portable/nick3d/ASceneDecoder.h>
#endif // NICK_INLINE_HACK


#ifdef USE_JAVACPP
#include <javacpp/ThreeDcpp.h>
#endif // USE_JAVACPP


#define A_MOVE_STEP 100


//#define USE_DOUBLE_BUFFERING 1
//#define NV_FIT_TO_SCREEN 1


AScene3D *theScene=(AScene3D *)NULL;
AMesh3D *theMesh=(AMesh3D *)NULL;
ACamera3D *theCamera=(ACamera3D *)NULL;
const char *theFilename=NULL;
//bool buttonPressed=false;
int prevx,prevy;
//AMatrix3D transMat,rotMat;
unsigned int theFrame=0;
float dx=0,dy=0,dz=0;


#ifdef NICK_USE_GUILIB
AApplicationLoop *appLoop=NULL;
#endif // NICK_USE_GUILIB


// Forward...
void drawMesh();
void drawScene();
void resizeMesh(int w, int h);
void resizeScene(int w, int h);
void keyboardScene(unsigned char key, int x, int y);
void mouseEvent(int button, int state, int x, int y);
void motionMesh(int x, int y);
void motionScene(int x, int y);

void registerMeshEvents();
void registerSceneEvents();


#ifndef NICK_INLINE_HACK
bool handleMesh(const char *fname,AMeshDecoder *decoder);
bool handleScene(const char *fname,ASceneDecoder *decoder);
#endif // NICK_INLINE_HACK


//#ifdef USE_JAVACPP
//void copyMesh2Model(AMesh3D *ms,Model3D *nm,unsigned int vertOffset);
//#endif // USE_JAVACPP


////////////////////////////////////////////////////////////////////////////////
// The event handlers
////////////////////////////////////////////////////////////////////////////////


/*
 void drawMesh()
{
  DEBUG_OUT<<"*** drawMesh...\n";
  theMesh->setAnimFrame(theFrame);
  theFrame++;
  if(theFrame==theMesh->getNumAnimFrames()) theFrame=0;
  theMesh->getModelToWorld()->mult(&rotMat);
  theRenderer->clear();
  theRenderer->prepareForDrawing(theMesh);
  nickDrawMesh3D(theScene);
}
*/


void drawScene()
{
  DEBUG_OUT<<"*** drawScene...\n";
  //
  ACamera3D *m=theScene->getCamera(0);
  //cerr<<"dx is "<<dx<<" dy is "<<dy<<" dz is "<<dz<<"\n";
  //
  if(m) {
    m->translate(dz,0,dx);
    //transMat.mult(&rotMat,m->getTrans());
  }
  //else cerr.warning("drawSceneEvent: no camera!");
  nickDrawScene3D(theScene);
}


void resizeMesh(int w, int h)
{
  DEBUG_OUT<<"*** resizeMesh...\n";
  nickActualWidth=w;  nickActualHeight=h;
#ifdef NICK_USE_OPENGL
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, nickActualWidth, nickActualHeight, 0, 0, 1);
  glMatrixMode (GL_MODELVIEW);
#endif // NICK_USE_OPENGL
  DEBUG_OUT<<"Now "<<nickActualWidth<<" by "<<nickActualHeight<<"\n";
  //drawMesh();
}


void resizeScene(int w, int h)
{
  DEBUG_OUT<<"*** resizeScene...\n";
  nickActualWidth=w;  nickActualHeight=h;
#ifdef NICK_USE_OPENGL
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, nickActualWidth, nickActualHeight, 0, 0, 1);
  glMatrixMode (GL_MODELVIEW);
#endif // NICK_USE_OPENGL
  DEBUG_OUT<<"Now "<<nickActualWidth<<" by "<<nickActualHeight<<"\n";
  drawScene();
}


void mouseEvent(int button, int state, int x, int y)
{
  DEBUG_OUT<<"*** mouseEvent...\n";
  //buttonPressed=true;
  prevx=x;  prevy=y;
  DEBUG_OUT<<prevx<<","<<prevy<<"\n";
}


void motionMesh(int x, int y)
{
  DEBUG_OUT<<"*** motionMesh...\n";
  //if(buttonPressed) {
    if(!prevy) prevy=y;
    if(!prevx) prevx=x;
    float xtheta=(prevy-y);
    float ytheta=(x-prevx);
    //DEBUG_OUT<<prevx<<","<<prevy<<"  "<<xtheta<<","<<ytheta<<"\n";
    xtheta=xtheta*360.0f;    ytheta=ytheta*360.0f;
    //DEBUG_OUT<<prevx<<","<<prevy<<"  "<<xtheta<<","<<ytheta<<"\n";
    xtheta=xtheta/nickActualWidth;  ytheta=ytheta/nickActualHeight;
    DEBUG_OUT<<prevx<<","<<prevy<<"  "<<xtheta<<","<<ytheta<<"\n";
    //rotMat.unit();
    //rotMat.xRotBy(xtheta);
    //rotMat.yRotBy(ytheta);
    //rotMat.debugDump();
    //drawMesh();
  //}
  //else {
    //DEBUG_OUT<<"(Button not pressed)\n";
    //DEBUG_OUT<<prevx<<","<<prevy<<"\n";
  //}
  prevx=x;  prevy=y;
}


void motionScene(int x, int y)
{
  DEBUG_OUT<<"*** motionScene...\n";
  //if(buttonPressed) {
    ACamera3D *m=theScene->getCamera(0);
    if(!prevy) prevy=y;
    if(!prevx) prevx=x;
    float xtheta=(prevy-y);
    float ytheta=(x-prevx);
    //cerr<<prevx<<","<<prevy<<"  "<<xtheta<<","<<ytheta<<"\n";
    xtheta=xtheta*360.0f;    ytheta=ytheta*360.0f;
    //cerr<<prevx<<","<<prevy<<"  "<<xtheta<<","<<ytheta<<"\n";
    xtheta=xtheta/nickActualWidth;  ytheta=ytheta/nickActualHeight;
    DEBUG_OUT<<prevx<<","<<prevy<<"  "<<xtheta<<","<<ytheta<<"\n";
    if(m) { m->xRotBy(xtheta); m->yRotBy(ytheta); }
    dx=0;  dy=0;  dz=0;
    drawScene();
  //}
  //else {
    //DEBUG_OUT<<"(Button not pressed)\n";
    //DEBUG_OUT<<prevx<<","<<prevy<<"\n";
  //}
  prevx=x;  prevy=y;
}


void keyboardScene(unsigned char key, int x, int )
{
  DEBUG_OUT<<"*** keyboardScene code was "<<key<<"\n";
  switch(key) {
    case 'w':
      dx=A_MOVE_STEP;  dz=0;
      break;
    case 's':
      dx=-A_MOVE_STEP;  dz=0;
      break;
    case 'a':
      dx=0;  dz=-A_MOVE_STEP;
      break;
    case 'd':
      dx=0;  dz=A_MOVE_STEP;
      break;
    default:
      //dx=0;  dy=0;  dz=0;
      break;
  }
  //cerr<<"dx is "<<dx<<" dy is "<<dy<<" dz is "<<dz<<"\n";
  //transMat.translate(dz,0,dx);
  drawScene();
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


#ifndef NICK_INLINE_HACK
bool handleScene(const char *fname,ASceneDecoder *decoder)
{
  if(!fname) return false;
  if(!decoder) return false;
  bool ret=true;
  DEBUG_OUT<<"*** handleScene...\n";
  //
  theScene=decoder->createNewSceneObject();
  DEBUG_OUT<<"Scene contains "<<theScene->getNumMeshes()<<" meshes...\n";
  //theScene->isValid();
  //theRenderer=new ASoftRenderer3D();
  //if(theWindow) theRenderer->setViewSize(theWindow->getWidth());
  //theRenderer->resizeToFit(theScene);
  theCamera=theScene->getCamera(0);
  // TODO: Check for error?
  //buttonPressed=false;
  prevx=0;  prevy=0;
  //transMat.unit();  rotMat.unit();
  //
#ifndef USE_JAVACPP
#ifdef NICK_USE_GUILIB
  if(appLoop) {
    appLoop->regEventRedraw(drawScene);
    appLoop->regEventFocus(drawScene);
    appLoop->regEventMotion(motionScene);
    appLoop->regEventResize(resizeScene);
    appLoop->regEventMouse(mouseEvent);
    appLoop->regEventKeyboard(keyboardScene);
  }
#endif // NICK_USE_GUILIB
#endif // USE_JAVACPP
  return ret;
}
#endif // NICK_INLINE_HACK


#ifndef NICK_INLINE_HACK
bool handleMesh(const char *fname,AMeshDecoder *decoder)
{
  if(!fname) return false;
  if(!decoder) return false;
  bool ret=true;
  DEBUG_OUT<<"*** handleMesh...\n";
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
  theCamera=theScene->getCamera(0);
  // TODO: check for error?
  //buttonPressed=false;
  prevx=0;  prevy=0;
  //transMat.unit();  rotMat.unit();
  //
#ifndef USE_JAVACPP
#ifdef NICK_USE_GUILIB
  if(appLoop) {
    appLoop->regEventRedraw(drawMesh);
    appLoop->regEventFocus(drawMesh);
    appLoop->regEventMotion(motionMesh);
    appLoop->regEventResize(resizeMesh);
    appLoop->regEventMouse(mouseEvent);
  }
#endif // NICK_USE_GUILIB
#endif // USE_JAVACPP
  return ret;
}
#endif // NICK_INLINE_HACK


/*
#ifdef USE_JAVACPP
void copyMesh2Model(AMesh3D *ms,Model3D *nm,unsigned int vertOffset)
{
  DEBUG_OUT<<"copyMesh2Model...vertOffset is "<<vertOffset<<"\n";
  if(!ms) { std::cerr<<"copyMesh2Model: No AMesh3D!\n"; exit(5); }
  if(!nm) { std::cerr<<"copyMesh2Model: No Model3D!\n"; exit(5); }
  DEBUG_OUT<<"Working...";
  AVertexF3D **v=ms->getVerts();
  unsigned int i=0;
  DEBUG_OUT<<ms->getNumVerts()<<" verts...\n";
  for(i=0;i<ms->getNumVerts();i++) {
    nm->addVert(v[0][i].x,v[0][i].y,v[0][i].z);
    DEBUG_OUT<<v[0][i].x<<","<<v[0][i].y<<","<<v[0][i].z<<"\n";
  }
  DEBUG_OUT<<ms->getNumFaces()<<" faces...\n";
  AFace3D **f=ms->getFaces();
  for(i=0;i<ms->getNumFaces();i++) {
    nm->add(f[i]->a+vertOffset,f[i]->b+vertOffset);
    nm->add(f[i]->b+vertOffset,f[i]->c+vertOffset);
    nm->add(f[i]->c+vertOffset,f[i]->a+vertOffset);
    DEBUG_OUT<<f[i]->a<<","<<f[i]->b<<","<<f[i]->c<<"\n";
  }
  DEBUG_OUT<<"done.\n";
}
#endif // USE_JAVACPP
*/


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  char msg[256];
  //
  nickAppInit(THE_WINDOW_WIDTH,THE_WINDOW_HEIGHT,24,argc,argv);
  //
#ifdef NICK_INLINE_HACK
  DEBUG_OUT<<"main() Using NICK_INLINE_HACK to find the file!\n";
#else
  if(argc>1) theFilename=argv[1];
  //
  theFilename=THE_FILENAME_TO_LOAD;
  system("pwd");
  //
  if(!theFilename) {
    char msg[256];
    sprintf(msg,(char *)"usage: %s {model filename}",argv[0]);
    std::cerr<<"Error! "<<msg<<"\n";
    return 5;
  }
  DEBUG_OUT<<"main() theFilename is "<<theFilename<<"!\n";
#endif // NICK_INLINE_HACK
  //
#ifdef USE_JAVACPP
  DEBUG_OUT<<"main() Going to create td...\n";
  ThreeD td;
  //td.runBegin();
#endif // USE_JAVACPP
  //
#ifdef NICK_USE_GUILIB
#ifdef USE_JAVACPP
  DEBUG_OUT<<"main() Getting appLoop from td...\n";
  appLoop=td.getLoop();
#else
  DEBUG_OUT<<"main() Going to create appLoop...\n";
  appLoop=new AApplicationLoop(aDefaultAWindow);
#endif // USE_JAVACPP
  //
#endif // NICK_USE_GUILIB
  //
  bool working=true;
  //while(working) {
#ifdef NICK_INLINE_HACK
  DEBUG_OUT<<"main() NICK_INLINE_HACK going to create theMesh...\n";
  theMesh=new AMesh3D(num_teapot_vertices,(float *)teapot_vertices,num_teapot_indicies,(short *)teapot_indicies);
#ifndef USE_JAVACPP
#ifdef NICK_USE_GUILIB
  DEBUG_OUT<<"main() NICK_USE_GUILIB going to reg event handles with appLoop...\n";
  if(appLoop) {
    appLoop->regEventRedraw(drawScene);
    appLoop->regEventFocus(drawScene);
    appLoop->regEventMotion(motionScene);
    appLoop->regEventResize(resizeScene);
    appLoop->regEventMouse(mouseEvent);
  }
  else DEBUG_OUT<<"main() No appLoop?!?\n";
  DEBUG_OUT<<"main() Going to create theScene...\n";
  theScene=new AScene3D();
  theScene->resizeToFit(theMesh);
  theScene->addMesh(theMesh);
  //theRenderer=new ASoftRenderer3D();
  //if(theWindow) theRenderer->setViewSize(theWindow->getWidth());
  //theRenderer->resizeToFit(theMesh);
  theCamera=theScene->getCamera(0);
  // TODO: check for error?
  //buttonPressed=false;
  prevx=0;  prevy=0;
#endif // NICK_USE_GUILIB
#endif // USE_JAVACPP
#else
    DEBUG_OUT<<"main() Going to create pick a decoder...\n";
    ADecoder *dec=(ADecoder *)NULL;
    if(!dec) {
      DEBUG_OUT<<"main() going to try ASmartMeshDecoder::pickMeshDecoder...\n";
      dec=ASmartMeshDecoder::pickMeshDecoder(theFilename);
    }
    if(!dec) {
      DEBUG_OUT<<"main() going to try ASmartSceneDecoder::pickSceneDecoder...\n";
      dec=ASmartSceneDecoder::pickSceneDecoder(theFilename);
    }
    DEBUG_OUT<<"main() (past the picking decoders part of the code)\n";
    if(!dec) {
      DEBUG_OUT<<"Couldn't recognize file type of that file\n";
      sprintf(msg,(char *)"Couldn't recognize file type of %s!\n",theFilename);
      std::cerr<<"Error! "<<msg<<"\n";
      exit(5);
    }
    bool handled=false;
    DEBUG_OUT<<"main() Going to call appropriate handle function for what we got...\n";
    if(dec->isMesh()) {
      DEBUG_OUT<<"main() isMesh is true.\n";
      working=handleMesh(theFilename,(AMeshDecoder *)dec);
      handled=true;
    }
    if(dec->isScene()) {
      DEBUG_OUT<<"main() isScene is true.\n";
      working=handleScene(theFilename,(ASceneDecoder *)dec);
      handled=true;
    }
    if(!handled) {
      std::cerr<<"main() Something went wrong...file was neither a mesh nor a scene!\n";
      return 5;
    }
#endif // NICK_INLINE_HACK
  DEBUG_OUT<<"main() Going to see if we have theMesh...\n";
  if(theMesh) theMesh->debugDump();
  DEBUG_OUT<<"main() Going to see if we have theScene...\n";
  if(theScene) {
    theScene->debugDump();
    if(!theMesh) {
      DEBUG_OUT<<"main() Grabbing first mesh from theScene...\n";
      theMesh=theScene->getMeshes()[0];
    }
    /*
    else {
      std::cerr<<"Somehow we have theMesh and theScene, obviously confused, giving up!\n";
      exit(5);
    }
    */
  }
  //transMat.unit();  rotMat.unit();
#ifdef USE_JAVACPP
  // Now build a model that ThreeD will like...
  //theMesh->normalizeTranslation();
  //theMesh->normalizeScale();
  /*
  Model3D nm;
  DEBUG_OUT<<"Going to build Model3D verts based on AMesh3D's...\n";
  if(theScene) {
    // We have a scene, possibly with multiple meshes...
    DEBUG_OUT<<"We have a scene with "<<theScene->getNumMeshes()<<" meshes...\n";
    unsigned int curOffset=0;
    for(unsigned int i=0;i<theScene->getNumMeshes();i++) {
      AMesh3D *ms=theScene->getMeshes()[i];
      copyMesh2Model(ms,&nm,curOffset);
      curOffset+=ms->getNumVerts();
    }
  }
  else {
    // We have a single mesh only...
    DEBUG_OUT<<"We have a single mesh...\n";
    copyMesh2Model(theMesh,&nm,0);
  }
  //
  DEBUG_OUT<<"Done, now swapping td's Model3D for ours.\n";
  td.md=&nm;
  */
  DEBUG_OUT<<"main() USE_JAVACPP setting theMesh for td and going to runContinued and then start!\n";
  td.md=theMesh;
  // skip its reading routine and continue as if all is normal...
  td.runContinued();
  //
  td.start();
#else
#ifdef NICK_USE_GUILIB
  DEBUG_OUT<<"main() Going to apploop.run...\n";
  appLoop->run();
  DEBUG_OUT<<"main() Back from apploop.run.\n";
#endif // NICK_USE_GUILIB
#endif // USE_JAVACPP
  //
  DEBUG_OUT<<"End of main().\n";
  return 0;
}

