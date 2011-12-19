
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>


#ifdef NICK_USE_GUILIB
#include <guilib/nickguilib.h>
#endif // NICK_USE_GUILIB

#ifdef NICK_USE_GLUT
#include <glut/nickglut.h>
#endif // NICK_USE_GLUT

#ifdef NICK_USE_SDL
#include <sdl/nicksdl.h>
#endif // NICK_USE_SDL


#include <portable/nickcpp/ABitmap.h>

#include <portable/ndecoder/ADecoder.h>
#include <portable/ndecoder/ASmartDecoder.h>
#include <portable/ndecoder/AImageDecoder.h>


//#define NV_FIT_TO_SCREEN 1

#define THE_WINDOW_WIDTH 640
#define THE_WINDOW_HEIGHT 480


ABitmap *theImage=(ABitmap *)NULL;
nickDrawableBitmap *drawableImage=(nickDrawableBitmap *)NULL;
bool buttonPressed=false;
int prevx,prevy;
char *theFilename=NULL;


// Forward...
void drawImage();
void resizeImage(int w, int h);
void keyboardImage(unsigned char key, int x, int y);
void specialImage(int key, int x, int y);
void mouseImage(int button, int state, int x, int y);
void motionImage(int x, int y);

bool handleImage(char *fname,AImageDecoder *decoder);


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

void resizeImage(int w, int h)
{
  DEBUG_OUT<<"resizeImage...\n";
  drawImage();
}


void keyboardImage(unsigned char key, int x, int y)
{
  DEBUG_OUT<<"keyboardImage...\n";
  drawImage();
}


void specialImage(int key, int x, int y)
{
  DEBUG_OUT<<"specialImage...\n";
  drawImage();
}


void mouseImage(int button, int state, int x, int y)
{
  DEBUG_OUT<<"mouseImage...\n";
  drawImage();
}


void motionImage(int x, int y)
{
  DEBUG_OUT<<"motionImage...\n";
  drawImage();
}


void drawImage()
{
  DEBUG_OUT<<"drawImage...\n";
  nickLockDisplay();
  nickSetColorRGB(0,0,0);
  nickClearDisplay();
  nickDrawImage(drawableImage,(nickActualWidth/2)-(theImage->getWidth()/2),(nickActualHeight/2)-(theImage->getHeight()/2));
  nickUnlockDisplay();
  nickFlipDisplay();
}


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
  drawableImage=nickGetDrawableBitmap(theImage);
  //
  DEBUG_OUT<<"Decoder is "<<decoder->getWidth()<<"x"<<decoder->getHeight()<<"\n";
  DEBUG_OUT<<"Image is "<<theImage->getWidth()<<"x"<<theImage->getHeight()<<"\n";
  ret=true;
  return ret;
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  //
  nickAppInit(THE_WINDOW_WIDTH,THE_WINDOW_HEIGHT,24,argc,argv);
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
#ifdef NICK_USE_GUILIB
  //
  AApplicationLoop appLoop(aDefaultAWindow);
  appLoop.regEventRedraw(drawImage);
  appLoop.regEventFocus(drawImage);
  appLoop.regEventMotion(motionImage);
  appLoop.regEventResize(resizeImage);
  appLoop.regEventKeyboard(keyboardImage);
  appLoop.regEventMouse(mouseImage);
  //
  DEBUG_OUT<<"Going to apploop.run...\n";
  appLoop.run();
  DEBUG_OUT<<"Back from apploop.run.\n";
  //
#endif // NICK_USE_GUILIB
#ifdef NICK_USE_SDL
  DEBUG_OUT<<"Going to enter loop...\n";
  bool done=false;
  while(!done){
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
          done=true;
          break;
        case SDL_KEYDOWN:
          if(event.key.keysym.sym==SDLK_ESCAPE) done=true;
          break;
      }
    }
    drawImage();
  }
  DEBUG_OUT<<"Done with loop.\n";
#endif // NICK_USE_SDL
#ifdef NICK_USE_GLUT
  //
  glutDisplayFunc(drawImage);
  glutReshapeFunc(resizeImage);
  glutMouseFunc(mouseImage);
  glutMotionFunc(motionImage);
  glutKeyboardFunc(keyboardImage);
  glutSpecialFunc(specialImage);
  //
  DEBUG_OUT<<"Going to call glutMainLoop...\n";
  glutMainLoop();
  DEBUG_OUT<<"Back from glutMainLoop.\n";
  //
#endif // NICK_USE_GLUT
  //
  exit(0);
  return 0;
}

