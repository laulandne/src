
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <javacpp/JApplet.h>

#include <portable/nickcpp/APalette.h>


JApplet *aDefaultApplet=NULL;


#ifdef NICK_USE_GUILIB
void JAppletMousedownGuilib(AEvent *e);
void JAppletMouseupGuilib(AEvent *e);
void JAppletMousemoveGuilib(AEvent *e);
void JAppletMousedragGuilib(AEvent *e);
void JAppletRedrawGuilib(AEvent *e);
void JAppletResizeGuilib(AEvent *e);
void JAppletDestroyGuilib(AEvent *e);
void JAppletKeyupGuilib(AEvent *e);
void JAppletKeydownGuilib(AEvent *e);
void JAppletKeymoveGuilib(AEvent *e);
#endif // NICK_USE_GUILIB

#ifdef NICK_USE_GLUT
void JAppletDisplayGlut();
void JAppletReshapeGlut(int w, int h);
void JAppletMouseGlut(int button, int state, int x, int y);
void JAppletMotionGlut(int x, int y);
void JAppletKeyboardGlut(unsigned char key, int x, int y);
void JAppletSpecialGlut(int key, int x, int y);
#endif // NICK_USE_GLUT

#ifdef NICK_USE_SDL
#endif // NICK_USE_SDL


////////////////////////////////////////////////////////////////////////////////
//  JApplet Class
////////////////////////////////////////////////////////////////////////////////

JApplet::JApplet()
{
  DEBUG_OUT<<"JApplet::JApplet()\n";
  init();
  //gfx=new JGraphics();
  //defaultThread=new JThread(this);
}


JApplet::~JApplet()
{
}


void JApplet::init()
{
  DEBUG_OUT<<"JApplet::init()\n";
#ifdef NICK_USE_GUILIB
  loop=new AApplicationLoop(aDefaultAWindow);
#endif // NICK_USE_GUILIB
  base=(char *)".";
  curParam=1;
  aDefaultApplet=this;
  needsPaint=true;
  buttonPressed=false;
  argc=0;
  for(unsigned int t=0;t<10;t++) argv[t]=(char *)NULL;
  state=JAVACPP_STATE_INITED;
  nickRegisterEventHandlers();
}


void JApplet::start()
{
  DEBUG_OUT<<"JApplet::start()\n";
  state=JAVACPP_STATE_STARTED;
  repaint();
  nickRunEventLoop();
}


bool JApplet::mouseDown(nickEvent *e,int x,int y)
{
  DEBUG_OUT<<"JApplet::mouseDown()\n";
  return true;
}


bool JApplet::mouseUp(nickEvent *e,int x,int y)
{
  DEBUG_OUT<<"JApplet::mouseUp()\n";
  return true;
}


bool JApplet::mouseMove(nickEvent *e,int x,int y)
{
  DEBUG_OUT<<"JApplet::mouseMove()\n";
  return true;
}


bool JApplet::mouseDrag(nickEvent *e,int x,int y)
{
  DEBUG_OUT<<"JApplet::mouseDrag()\n";
  return true;
}


void JApplet::repaint()
{
  DEBUG_OUT<<"JApplet::repaint()\n";
  needsPaint=true;
  paint(/*gfx*/);
}


void JApplet::paint(/*JGraphics *g*/)
{
  DEBUG_OUT<<"JApplet::paint()\n";
  needsPaint=false;
}

