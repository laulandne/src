

#ifdef NICK_USE_GUILIB
#include <guilib/nickguilib.h>
#endif // NICK_USE_GUILIB

#ifdef NICK_USE_GLUT
#include <glut/nickglut.h>
#endif // NICK_USE_GLUT

#ifdef NICK_USE_SDL
#include <sdl/nicksdl.h>
#endif // NICK_USE_SDL


#include <javacpp/ThreeDcpp.h>


#define THREED_WINDOW_WIDTH 320
#define THREED_WINDOW_HEIGHT 480


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  nickAppInit(THREED_WINDOW_WIDTH,THREED_WINDOW_HEIGHT,24,argc,argv);
  //
  ThreeD td;
  td.run();
  td.start();
  //
  return 0;
}

