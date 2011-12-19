#ifndef JAVAGLUE_JAPPLET_H
#define JAVAGLUE_JAPPLET_H


#include <javacpp/javacpp.h>


#ifdef NICK_USE_GUILIB
#include <guilib/AWindow.h>
#include <guilib/AApplicationLoop.h>
#endif // NICK_USE_GUILIB


//#include <javacpp/nickEvent.h>
//#include <javacpp/JGraphics.h>
//#include <javacpp/JThread.h>
//#include <javacpp/jbox_thing_class.h>


#ifdef NICK_USE_GUILIB
#include <guilib/nickguilib.h>
#endif // NICK_USE_GUILIB

#ifdef NICK_USE_GLUT
#include <glut/nickglut.h>
#endif // NICK_USE_GLUT

#ifdef NICK_USE_SDL
#include <sdl/nicksdl.h>
#endif // NICK_USE_SDL


#define JAVACPP_STATE_INITED  0
#define JAVACPP_STATE_STOPPED 1
#define JAVACPP_STATE_STARTED 2
#define JAVACPP_STATE_RUNNING 3
#define JAVACPP_STATE_WAITING 4


class JApplet {
public:
  JApplet();
  virtual ~JApplet();
  //char *getParameter(char *p);
  //char *getDocumentBase();
  //void notifyAll();
  //jbox_thing_class size();
  //
  //virtual void wait();
  virtual void repaint();
  virtual void paint(/*JGraphics *g*/);
  virtual void init();
  //virtual void run();
  virtual void start();
  //virtual void stop();
  //virtual void resize(unsigned int w, unsigned int h);
  virtual boolean mouseUp(nickEvent *e,int x,int y);
  virtual boolean mouseDown(nickEvent *e,int x,int y);
  virtual boolean mouseMove(nickEvent *e,int x,int y);
  virtual boolean mouseDrag(nickEvent *e,int x,int y);
  //
  nickEvent *getEvent() { return &event; }
  //JThread *getThread() { return defaultThread; }
#ifdef NICK_USE_GUILIB
  AApplicationLoop *getLoop() { return loop; }
#endif // NICK_USE_GUILIB
#ifdef NICK_USE_GLUT
  //void translateEventGlut(unsigned int code,unsigned int x,unsigned int y);
#endif // NICK_USE_GLUT
  //
  boolean buttonPressed;
  //
//protected:
#ifdef NICK_USE_GUILIB
  AApplicationLoop *loop;
#endif // NICK_USE_GUILIB
  //jbox_thing_class box;
  //JGraphics *gfx;
  //JThread *defaultThread;
  nickEvent event;
  char *base;
  unsigned int curParam;
  boolean needsPaint;
  unsigned int argc;
  char *argv[10];
  unsigned int state;
};


extern JApplet *aDefaultApplet;


#endif // JAVAGLUE_JAPPLET_H

