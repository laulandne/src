
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

//#include <guilib/alib_carbon.h>

#include "AQTDecoderHelper.h"



//#include <guilib/ADisplay.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>


#ifdef USE_QUICKTIME
#ifdef USE_UMBRELLA_HEADERS
#include <QuickTime/QuickTime.h>
#else
#include <Movies.h>
#endif // USE_UMBRELLA_HEADERS
#endif // USE_QUICKTIME


////////////////////////////////////////////////////////////////////////////////
//  AQTDecoderHelper Class (Mac or Win32)
////////////////////////////////////////////////////////////////////////////////

AQTDecoderHelper::AQTDecoderHelper(const char *fname) : ADecoderHelper(fname)
{
  DEBUG_OUT<<"AQTDecoderHelper contructed.\n";
  init();
  colors=(APalette *)NULL;
  //initParts();
  //openFile(imgFile);
  //startQuickTime();
  //header();
  //if(!error()) if(!(initLineBuffer())) errorFlag=true;
  //if(!error()) color();
}


AQTDecoderHelper::~AQTDecoderHelper()
{
  freeParts();
#ifdef USE_QUICKTIME
  stopQuickTime();
#endif // USE_QUICKTIME
  DEBUG_OUT<<"AQTDecoderHelper destructed.\n";
}


void AQTDecoderHelper::setFile(const char *fname,FILE *theFd)
{
  filename=fname;
  fd=theFd;
}


/* STATIC */
bool AQTDecoderHelper::recognize(uint8_t *buffer,char *fname)
{
  if(recognizeAIFF(buffer)) return true;
  if(recognizeGIF(buffer)) return true;
  if(recognizePNG(buffer)) return true;
  if(recognizeRIFF(buffer)) return true;
  if(recognizeMidi(buffer)) return true;
  if(recognizeMPEG(buffer)) return true;
  if(recognizeJPEG(buffer)) return true;
  if(recognizeTGA(buffer)) return true;
  if(recognizeTIFF(buffer)) return true;
  return false;
}


void AQTDecoderHelper::initParts()
{
  gotQuickTime=false;
#ifdef USE_QUICKTIME
#ifdef ASYS_WINDOWS
  qtLib=NULL;
#endif
  origPort=(CGrafPtr)NULL;
  origDev=(GDevice **)NULL;
  theWorld=(GWorldPtr)NULL;
#if !__LP64__
  theController=(ComponentInstanceRecord *)NULL;
  theMoov=(Movie)NULL;
#endif
  thePM=(PixMap **)NULL;
  movieResFile=0;
#endif // USE_QUICKTIME
  fDepth=0;
}


void AQTDecoderHelper::freeParts()
{
#ifdef USE_QUICKTIME
#if !__LP64__
  if(theController) DisposeMovieController(theController);
  theController=(ComponentInstanceRecord *)NULL;
  if(theMoov) DisposeMovie(theMoov);
  theMoov=(Movie)NULL;
#endif
#ifdef USE_QUARTZ
#else
  if(thePM) UnlockPixels(thePM);
#endif // USE_QUARTZ
  thePM=(PixMap **)NULL;
#ifdef USE_QUARTZ
#else
  if(theWorld) DisposeGWorld(theWorld);
#endif // USE_QUARTZ
  theWorld=(GWorldPtr)NULL;
#endif // USE_QUICKTIME
}


void AQTDecoderHelper::stopQuickTime()
{
  if(!gotQuickTime) return;
#ifdef USE_QUICKTIME
  DEBUG_OUT<<"Shutting QuickTime down...\n";
#if !__LP64__
  ExitMovies();
#endif
#ifdef ASYS_WINDOWS
#ifdef OLD_QUICKTIME
  QTTerminate();
#endif // OLD_QUICKTIME
  FreeLibrary(qtLib);
#endif // ASYS_WINDOWS
  DEBUG_OUT<<"Done.\n";
#endif // USE_QUICKTIME
}


void AQTDecoderHelper::startQuickTime()
{
  DEBUG_OUT<<"Starting up QuickTime...\n";
  gotQuickTime=false;
#ifdef USE_QUICKTIME
  //aThisApp.initDefaultDisplay();
  //if(!aThisApp.defaultDisplay) { errorFlag=true; return; }
  bool foundQuickTime=false;
#ifdef ASYS_MAC
  if(alib_qtVer) foundQuickTime=true;
#endif
#ifdef ASYS_WINDOWS
#ifdef OLD_QUICKTIME
  qtLib=LoadLibrary("qtim.dll"));
  if(qtLib) foundQuickTime=true;
  if(foundQuickTime) {
    if(QTInitialize(NULL))
      { DEBUG_OUT<<"Couldn't init QuickTime!\n"; errorFlag=true; return; }
  }
#else
  if(aThisApp.qtVer) foundQuickTime=true;
#endif
#endif
#if !__LP64__
  err=EnterMovies();
#endif
  if(err) {
    DEBUG_OUT<<"Error ("<<(int)err<<") from EnterMovies!\n"; foundQuickTime=false;
  }
  if(!foundQuickTime) {
    //DEBUG_OUT<<"Couldn't load QuickTime!\n"; errorFlag=true;
    return;
  }
  DEBUG_OUT<<"Looks like QuickTime started up fine.\n";
  gotQuickTime=true;
#endif // USE_QUICKTIME
}


// NOTE: This only works for images and movies
void AQTDecoderHelper::header()
{
  //if(errorFlag) return;
  DEBUG_OUT<<"AQTDecoderHelper::header...\n";
  fseek(fd,0,SEEK_SET);
  fread(tBuffer,1024,1,fd);
  bool recognized=false;
  // FIXME: Should openMovie and get bounds first...
  // FIXME: Shouldn't care about format, let qt handle it.
  if(recognizeGIF(tBuffer)) {
    DEBUG_OUT<<"Looks like a GIF!\n";
    headerGIF(); recognized=true;
  }
  if(recognizePNG(tBuffer)) {
    DEBUG_OUT<<"Looks like a PNG!\n";
    headerPNG(); recognized=true;
  }
  if(recognizeMPEG(tBuffer)) {
    DEBUG_OUT<<"Looks like a MPEG!\n";
    headerMPEG(); recognized=true;
  }
  if(recognizeJPEG(tBuffer)) {
    DEBUG_OUT<<"Looks like a JPEG!\n";
    headerJPEG(); recognized=true;
  }
  if(recognizeTGA(tBuffer)) {
    DEBUG_OUT<<"Looks like a TGA!\n";
    headerTGA(); recognized=true;
  }
  if(recognizeTIFF(tBuffer)) {
    DEBUG_OUT<<"Looks like a TIFF!\n";
    headerTIFF(); recognized=true;
  }
  if(!recognized) {
    DEBUG_OUT<<"Giving up, doesn't look like anything we recognize!\n";
    //errorFlag=true;
    return;
  }
  if(!boundsValid()) {
    DEBUG_OUT<<"Didn't get a legit size...failing...\n";
    //errorFlag=true;
    return;
  }
#ifdef USE_QUICKTIME
  DEBUG_OUT<<"Going to openMovie("<<filename<<")...\n";
  openMovie(filename);
  /*
  if(errorFlag) {
    DEBUG_OUT<<"Couldn't openMovie!\n";
#ifdef USE_QUARTZ
#else
    UnlockPixels(thePM);
#endif
	  freeParts();
    return;
  }
  */
  DEBUG_OUT<<"Movie opened and drawn in theWorld.\n";
#ifdef USE_QUARTZ
#else
  SetGWorld(origPort,origDev);
  UnlockPixels(thePM);
#endif // USE_QUARTZ
  unsigned int theWidth=0,theHeight=0;
  theWidth=((*thePM)->rowBytes)&0x3fff;
  theHeight=((*thePM)->bounds.bottom)-((*thePM)->bounds.top);
  DEBUG_OUT<<"thePM is "<<theWidth<<" bytes by "<<theHeight<<" lines...\n";
#endif
  if(lineBuffer) { free(lineBuffer); initLineBuffer(); }
}


#ifdef USE_QUICKTIME
void AQTDecoderHelper::openMovie(char *imgFile)
{
  DEBUG_OUT<<"Starting openMovie("<<imgFile<<")...\n";
  bool panic=false;
  DEBUG_OUT<<"Going to OpenMovieFile("<<imgFile<<",,)...\n";
#ifdef OLD_QUICKTIME
  err=OpenMovieFile(imgFile,(unsigned int *)&movieResFile,OF_READ);
  debugErrorIfAny();
#else
  DEBUG_OUT<<"Converting filename to something QuickTime can use...\n";
  FSSpec fileSpec;
  FSRef fileRef;
  err=FSPathMakeRef((UInt8 *)imgFile,&fileRef,NULL);
  debugErrorIfAny();
  err=FSGetCatalogInfo(&fileRef,0,NULL,NULL,&fileSpec,NULL);
  debugErrorIfAny();
  /*
  unsigned char *theFullPath=(unsigned char *)malloc(strlen(imgFile)+1);
  CopyCStringToPascal(imgFile,theFullPath);
  for(unsigned int t=0;t<theFullPath[0];t++) {
    if(theFullPath[t]=='/') theFullPath[t]=':';
  }
  err=FSMakeFSSpec(0,0L,(unsigned char *)theFullPath,&fileSpec);
  debugErrorIfAny();
  */
#if !__LP64__
  err=OpenMovieFile(&fileSpec,&movieResFile,fsRdWrPerm);
  debugErrorIfAny();
#endif
#endif // OLD_QUICKTIME
  /*if(errorFlag) {
    DEBUG_OUT<<"errorFlag set, giving up...\n";
    return;
  }*/
  DEBUG_OUT<<"Going to NewMovieFromFile(,"<<(int)movieResFile<<",,,,)...\n";
#if !__LP64__
  err=NewMovieFromFile(&theMoov,movieResFile,(short *)NULL,(unsigned char *)NULL,newMovieActive,(unsigned char *)NULL);
#endif
  if(err) { DEBUG_OUT<<"Error ("<<(int)err<<") from NewMovieFromFile!!!\n"; panic=true; }
  DEBUG_OUT<<"Going to CloseMovieFile...\n";
#if !__LP64__
  CloseMovieFile(movieResFile);
#endif
  if(panic) return;
  Rect theMovieRect,bounds;
#ifdef OLD_QUICKTIME
  GetMovieBox(theMoov,(LPRECT)&theMovieRect);
  theController=NewMovieController(theMoov,(LPRECT)&theMovieRect,mcTopLeftMovie,w->getWindow());
  MCGetControllerBoundsRect(theController,(LPRECT)&bounds);
  width=bounds.boxRight-bounds.boxLeft;
  height=bounds.boxBottom-bounds.boxTop;
#else
  DEBUG_OUT<<"Going to GetMovieBox and SetMovieActive...\n";
#if !__LP64__
  SetMovieActive(theMoov,true);
  GetMovieBox(theMoov,&theMovieRect);
#endif
  unsigned int mWidth=theMovieRect.right-theMovieRect.left;
  unsigned int mHeight=theMovieRect.bottom-theMovieRect.top;
  DEBUG_OUT<<"Movie says rect is "<<mWidth<<"x"<<mHeight<<"...\n";
  width=mWidth;  height=mHeight;
  fDepth=24; /*aThisApp.defaultDisplay->getDepth()*/;
  DEBUG_OUT<<"Actual depth is "<<depth<<", going to use "<<fDepth<<" instead.\n";
  depth=fDepth;  nPlanes=1;
  calc();
  createGWorld();
  if(!theWorld) { DEBUG_OUT<<"No theWorld!\n"; return; }
#ifdef USE_QUARTZ
#else
  DEBUG_OUT<<"Going to SetGWorld...\n";
  SetMovieGWorld(theMoov,(CGrafPtr)theWorld,(GDevice **)NULL);
  SetGWorld(theWorld,(GDevice **)NULL);
#endif // USE_QUARTZ
  DEBUG_OUT<<"Going to NewMovieController...\n";
#if !__LP64__
  theController=NewMovieController(theMoov,&theMovieRect,mcTopLeftMovie|mcWithFrame);
#endif
  DEBUG_OUT<<"Getting movie's bounds...\n";
#if !__LP64__
  MCGetControllerBoundsRect(theController,&bounds);
#endif
  mWidth=bounds.right-bounds.left;
  mHeight=bounds.bottom-bounds.top;
  DEBUG_OUT<<"Movie says controller is "<<mWidth<<"x"<<mHeight<<"...\n";
#endif // OLD_QUICKTIME
  //depth=aThisApp.defaultDisplay->getDepth();  // Is this right?
  //bytesPerLine=aThisApp.defaultDisplay->getBytesPerLine();  // NOTE!
  long controllerFlags=0;
  DEBUG_OUT<<"Using controller to set up movie...\n";
#if !__LP64__
  MCDoAction(theController,mcActionGetFlags,&controllerFlags);
  MCDoAction(theController,mcActionSetFlags,(void *)(controllerFlags/*|mcFlagsUseWindowPalette*/));
  MCDoAction(theController,mcActionActivate,0);
  MCDoAction(theController,mcActionIdle,0);
  MCDoAction(theController,mcActionPlay,(void *)(1<<16));
  MCDoAction(theController,mcActionDraw,theWorld);
#endif
#ifdef USE_QUARTZ
#else
  SetGWorld(origPort,origDev);
#endif // USE_QUARTZ
  DEBUG_OUT<<"Finished openMovie.\n";
}
#endif // USE_QUICKTIME


#ifdef USE_QUICKTIME
void AQTDecoderHelper::createGWorld()
{
#ifdef USE_QUARTZ
#else
  GetGWorld(&origPort,&origDev);
#endif // USE_QUARTZ
  theRect.left=0;  theRect.right=width;
  theRect.top=0;  theRect.bottom=height;
  err=0;
#ifdef USE_QUARTZ
#else
  DEBUG_OUT<<"Going to create GWorld("<<width<<","<<height<<","<<fDepth<<")...\n";
  err=NewGWorld(&theWorld,fDepth,&theRect,(ColorTable **)NULL,(GDevice **)NULL,
    (unsigned long)NULL);
#endif // USE_QUARTZ
  if(theWorld) DEBUG_OUT<<"Looks like GWorld created fine.\n";
  if(err) DEBUG_OUT<<"Got an error ("<<(int)err<<") creating GWorld!\n";
  if(!theWorld) {
    //errorFlag=true;
    DEBUG_OUT<<"AQTDecoderHelper: Couldn't find theWorld!\n";
    freeParts();
    return;
  }
  DEBUG_OUT<<"Going to SetGWorld...\n";
#ifdef USE_QUARTZ
#else
  SetGWorld(theWorld,(GDevice **)NULL);
  ForeColor(blackColor);  BackColor(whiteColor);
#endif // USE_QUARTZ
#if TARGET_CARBON
  Rect theRect;
#ifdef USE_QUARTZ
#else
  GetPortBounds(theWorld,&theRect);
  EraseRect(&theRect);
#endif // USE_QUARTZ
#else
  EraseRect(&(theWorld->portRect));
#endif // TARGET_CARBON
#ifdef USE_QUARTZ
#else
  DEBUG_OUT<<"Going to GetGWorldPixMap...\n";
  thePM=GetGWorldPixMap(theWorld);
  DEBUG_OUT<<"Going to LockPixels...\n";
  if(!LockPixels(thePM)) {
    //errorFlag=true;
    DEBUG_OUT<<"AQTDecoderHelper couldn't lock gWorld pixels!\n";
    freeParts();
    return;
  }
#endif // USE_QUARTZ
}
#endif // USE_QUICKTIME


// NOTE: This only works for images and movies
void AQTDecoderHelper::color()
{
  //if(errorFlag) return;
#ifdef USE_QUICKTIME
  if(!theWorld) return;
#endif // USE_QUICKTIME
  if(depth<9) {
    if(!colors) colors=new APalette(1>>depth);
    // FIXME: set up colors here
  }
}


// FIXME: Doesn't work in m68k 24 bit addressing mode...(see AImage::blastMacBits)
// NOTE: This only works for images and movies
uint8_t *AQTDecoderHelper::readLine(unsigned int lineNum, unsigned int numPlane)
{
  //if(errorFlag) return (uint8_t *)NULL;
  if(!lineBuffer) { initLineBuffer(); }
  if(lineNum>height) {
     DEBUG_OUT<<"line ref out of bounds!\n";  return (uint8_t *)NULL;
  }
  if(numPlane>depth) {
    DEBUG_OUT<<"plane ref out of bounds!\n";  return (uint8_t *)NULL;
  }
#ifdef USE_QUICKTIME
  if(!theWorld) { DEBUG_OUT<<"no theWorld!\n"; return (uint8_t *)NULL; }
  if(!thePM) { DEBUG_OUT<<"no thePM!\n"; return (uint8_t *)NULL; }
  unsigned int theWidth=((*thePM)->rowBytes)&0x3fff;
  unsigned int writeWidth=theWidth;
  if(writeWidth>bytesPerLine) writeWidth=bytesPerLine;
  char *ptr=NULL;
#ifdef USE_QUARTZ
#else
  ptr=GetPixBaseAddr(thePM);
#endif // USE_QUARTZ
  char *src=(char *)(ptr+(theWidth*lineNum));
#if TARGET_CPU_68K
  SInt8 oldMode=GetMMUMode();
  SInt8 newMode=true32b;
  if(PixMap32Bit(thePM)) { if(oldMode==false32b) SwapMMUMode(&newMode); }
#endif // TARGET_CPU_68K
  for(unsigned int t=0;t<writeWidth;t++) lineBuffer[t]=src[t];
#if TARGET_CPU_68K
  if(oldMode==false32b) { newMode=false32b; SwapMMUMode(&newMode); }
#endif // TARGET_CPU_68K
#endif // USE_QUICKTIME
  return lineBuffer;
}


#ifdef USE_QUICKTIME
void AQTDecoderHelper::debugErrorIfAny()
{
  if(err) {
    DEBUG_OUT<<"debugErrorIfAny: Got ";
    switch(err) {
      case nsvErr: DEBUG_OUT<<"nsvErr"; /*errorFlag=true;*/ break;
      case fnfErr: DEBUG_OUT<<"fnfErr"; /*errorFlag=true;*/ break;
      case paramErr: DEBUG_OUT<<"paramErr"; /*errorFlag=true;*/ break;
      case dirNFErr: DEBUG_OUT<<"dirNFErr"; /*errorFlag=true;*/ break;
      default:
        DEBUG_OUT<<"unknown error "<<(int)err;
        break;
    }
    DEBUG_OUT<<"\n";
    //errorFlag=true;
  }
}
#endif


