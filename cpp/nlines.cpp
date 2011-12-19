
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <javacpp/javacpp.h>


#ifdef NICK_USE_GUILIB
#include <guilib/nickguilib.h>
#endif // NICK_USE_GUILIB

#ifdef NICK_USE_OPENGL
#include <glut/nickglut.h>
#endif // NICK_USE_OPENGL

#ifdef NICK_USE_SDL
#include <sdl/nicksdl.h>
#endif // NICK_USE_SDL


#include <portable/nickcpp/APalette.h>


#define NLINES_WINDOW_WIDTH 320
#define NLINES_WINDOW_HEIGHT 480


// Forward...
void resizeLines(int w, int h);
void keyboardLines(unsigned char key, int x, int y);
void specialLines(int key, int x, int y);
void mouseLines(int button, int state, int x, int y);
void motionLines(int x, int y);
void drawLines();


void resizeLines(int w, int h)
{
  DEBUG_OUT<<"resizeLines!\n";
  nickActualWidth=w;  nickActualHeight=h;
#ifdef NICK_USE_OPENGL
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, nickActualWidth, nickActualHeight, 0, 0, 1);
  glMatrixMode (GL_MODELVIEW);
#endif // NICK_USE_OPENGL
  DEBUG_OUT<<"Now "<<nickActualWidth<<" by "<<nickActualHeight<<"\n";
  drawLines();
}


void keyboardLines(unsigned char key, int x, int y)
{
  DEBUG_OUT<<"keyboardLines!\n";
  drawLines();
}


void specialLines(int key, int x, int y)
{
  DEBUG_OUT<<"specialLines!\n";
  drawLines();
}


void mouseLines(int button, int state, int x, int y)
{
  DEBUG_OUT<<"mouseLines!\n";
  drawLines();
}


void motionLines(int x, int y)
{
  DEBUG_OUT<<"motionLines!\n";
  drawLines();
}


void drawLines()
{
  DEBUG_OUT<<"drawLines!\n";
  unsigned long t;
  unsigned long numColors=0x00ffffff;
  unsigned long inc=1;
  bool trueColor=false;
  APalette *colors=nickGetCurPalette();
  // If we don't have a palette, we assume its truecolor
  if(colors) {
    if(colors->getNumColors()>256) { trueColor=true; }
    numColors=colors->getNumColors();
  }
  else trueColor=true;
  DEBUG_OUT<<"trueColor="<<trueColor<<" numColors="<<numColors<<" "<<nickActualWidth<<"x"<<nickActualHeight<<"\n";
  unsigned long c=0;
#ifdef NICK_USE_OPENGL
  glBegin(GL_LINES);
#endif // NICK_USE_OPENGL
  nickLockDisplay();
  nickSetColorLong(0x000000);
  nickClearDisplay();
  c=0;
  if(trueColor) inc=numColors/nickActualWidth;
  for(t=0;t<nickActualWidth-1;t++) {
    c=c+inc;
    if(c>numColors) c=0;
    nickSetColorLong(c);
    nickDrawLine(t,0,nickActualWidth-t-1,nickActualHeight-1);
  }
  c=0;
  if(trueColor) inc=numColors/nickActualHeight;
  for(t=nickActualHeight-1;t>0;t--) {
    c=c+inc;
    if(c>numColors) c=0;
    nickSetColorLong(c);
    nickDrawLine(0,t,nickActualWidth-1,nickActualHeight-t);
  }
#ifdef NICK_USE_OPENGL
  glEnd();
#endif // NICK_USE_OPENGL
  nickUnlockDisplay();
  nickFlipDisplay();
}


int main(int argc,char **argv)
{
  nickAppInit(NLINES_WINDOW_WIDTH,NLINES_WINDOW_HEIGHT,24,argc,argv);
  //
#ifdef NICK_USE_GUILIB
  //
  AApplicationLoop appLoop(aDefaultAWindow);
  appLoop.regEventRedraw(drawLines);
  appLoop.regEventFocus(drawLines);
  appLoop.regEventResize(resizeLines);
  appLoop.regEventKeyboard(keyboardLines);
  appLoop.regEventMouse(mouseLines);
  //
  DEBUG_OUT<<"Going to apploop.run...\n";
  appLoop.run();
  DEBUG_OUT<<"Back from apploop.run.\n";
  //
#endif // NICK_USE_GUILIB
#ifdef NICK_USE_SDL
  //
  DEBUG_OUT<<"Going to enter loop...\n";
  bool done=false;
  while(!done){
    while(SDL_PollEvent(&nicksdl_theEvent)) {
      switch(nicksdl_theEvent.type) {
        case SDL_QUIT:
          done=true;
          break;
        case SDL_KEYDOWN:
          if(nicksdl_theEvent.key.keysym.sym==SDLK_ESCAPE) done=true;
          break;
      }
    }
    drawLines();
  }
  DEBUG_OUT<<"Done with loop.\n";
  //
#endif // NICK_USE_SDL
#ifdef NICK_USE_GLUT
  //
  glutDisplayFunc(drawLines);
  glutReshapeFunc(resizeLines);
  glutMouseFunc(mouseLines);
  glutMotionFunc(motionLines);
  glutKeyboardFunc(keyboardLines);
  glutSpecialFunc(specialLines);
  //
  DEBUG_OUT<<"Going to call glutMainLoop...\n";
  glutMainLoop();
  DEBUG_OUT<<"Back from glutMainLoop.\n";
  //
#endif // NICK_USE_GLUT
  exit(0);
  return 0;
}
