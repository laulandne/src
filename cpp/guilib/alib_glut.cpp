
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

Use GLUT for all interface and graphics (portable to different actual platforms)

*/


#include <guilib/alib_glut.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef ASYS_GLUT
#include <guilib/AApplicationLoop.h>
#include <guilib/AWindow.h>
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
GLUquadricObj *glut_qobj=NULL;
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
int glut_start(int argc, char **argv)
{
  DEBUG_OUT<<"glut_start...\n";
#ifdef GLUT
  /* start of glut windowing and control functions */
  DEBUG_OUT<<"(Going to call glutInit...)\n";
  glutInit(&argc, argv);
  DEBUG_OUT<<"(Going to call glutInitDisplayMode...)\n";
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  DEBUG_OUT<<"(Going to call glutInitWindowSize...)\n";
  glutInitWindowSize(800, 600);
  DEBUG_OUT<<"(Going to call glutCreateWindow...)\n";
  glutCreateWindow(argv[0]);
  {
  /* Check that renderer has the GL_EXT_texture_object
     extension or supports OpenGL 1.1 */
    char *version = (char *) glGetString(GL_VERSION);
    double gl_vers = strtod(version, NULL);
    if ((gl_vers < 1.1) && !glutExtensionSupported("GL_EXT_texture_object")) {
      fprintf(stderr, "GL version must be 1.1 or greater.\n");
      return 1;  // Just exit if the support is not there.
    }
  }
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glPolygonOffset(1.0, 1.0);
  glEnable(GL_CULL_FACE);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glut_myinit();
  glutDisplayFunc(glut_myDisplay);
  glutReshapeFunc(glut_myReshape);
  glutMouseFunc(glut_myMouse);
  glutMotionFunc(glut_myMotion);
  glutKeyboardFunc(glut_myKeyboard);
  glutSpecialFunc(glut_mySpecial);
  //glut_setupMenus();
  //DEBUG_OUT<<"(Going to call glutMainLoop...)\n";
  //glutMainLoop();
  /* end of glut windowing and control functions */
#endif // GLUT
  return 0;
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
void glut_myinit(void)
{
  DEBUG_OUT<<"glut_myinit...\n";
  glut_qobj = gluNewQuadric();
#ifdef LIGHT
  SetMaterial(mat_specular2, mat_ambient2, mat_diffuse2, mat_shininess2);
#endif
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
void glut_myReshape(int w, int h)
{
  DEBUG_OUT<<"glut_myReshape("<<w<<","<<h<<")...\n";
  if(aDefaultAWindow) {
    aDefaultAWindow->glutSetSize(w,h);
  }
  if(aDefaultAApplicationLoop) {
    AEvent *e=new AEvent;
    e->type=A_EVENT_RESIZE;
    e->code=0;
    e->modifier=0;
    e->number=0;
    e->x=0;  e->y=0;
    e->setWindow(aDefaultAWindow);
    aDefaultAApplicationLoop->dispatchEvent(e);
  }
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(65.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,0.0);  /* viewing transform  */
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
void glut_myDisplay(void)
{
  DEBUG_OUT<<"glut_myDisplay...\n";
  if(aDefaultAApplicationLoop) {
    AEvent *e=new AEvent;
    e->type=A_EVENT_REDRAW;
    e->code=0;
    e->modifier=0;
    e->number=0;
    e->x=0;  e->y=0;
    e->setWindow(aDefaultAWindow);
    aDefaultAApplicationLoop->dispatchEvent(e);
  }
  glFlush();
  glutSwapBuffers();
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
#ifdef GLUT
/* ARGSUSED1 */
void glut_myKeyboard(unsigned char key, int x, int y)
{
  DEBUG_OUT<<"glut_myKeyboard("<<(int)key<<","<<x<<","<<y<<")...\n";
  int i = 0;
  AEvent *e=NULL;
  switch (key) {
    default:
      if(aDefaultAApplicationLoop) {
        e=new AEvent;
        e->type=A_EVENT_KEYUP;
        e->code=key;
        e->modifier=0;
        e->number=0;
        e->x=x;  e->y=y;
        e->setWindow(aDefaultAWindow);
        aDefaultAApplicationLoop->dispatchEvent(e);
      }
      break;
  }
  if (i)
    glutPostRedisplay();
}
#endif // GLUT
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
/* ARGSUSED1 */
void glut_mySpecial(int key, int x, int y)
{
  DEBUG_OUT<<"glut_mySpecial("<<key<<","<<x<<","<<y<<")...\n";
  int i = 0;
  switch (key) {
    default:
      break;
  }
  if (i)
    glutPostRedisplay();
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
void glut_setupMenus(void)
{
  DEBUG_OUT<<"glut_setupMenus...\n";
  int glut_menu[13];
  glut_menu[5] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("forward       : q,w", 0);
  glutAddMenuEntry("backwards     : a,s", 0);
  glutAddMenuEntry("outwards      : z,x", 0);
  glutAddMenuEntry("inwards       : Z,X", 0);
  glut_menu[6] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("upwards       : Q,W", 0);
  glutAddMenuEntry("downwards     : A,S", 0);
  glutAddMenuEntry("outwards      : 1,2", 0);
  glutAddMenuEntry("inwards       : 3,4", 0);
  glut_menu[1] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry(" : Page_up", 0);
  glut_menu[8] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("forward       : y,u", 0);
  glutAddMenuEntry("backwards     : h.j", 0);
  glutAddMenuEntry("outwards      : Y,U", 0);
  glutAddMenuEntry("inwards       : H,J", 0);
  glut_menu[9] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("forward       : n,m", 0);
  glutAddMenuEntry("backwards     : N,M", 0);
  glut_menu[9] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("forward       : n,m", 0);
  glutAddMenuEntry("backwards     : N,M", 0);
  glut_menu[10] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("toes up       : K,L", 0);
  glutAddMenuEntry("toes down     : k,l", 0);
  glut_menu[11] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("right         : right arrow", 0);
  glutAddMenuEntry("left          : left arrow", 0);
  glutAddMenuEntry("down          : up arrow", 0);
  glutAddMenuEntry("up            : down arrow", 0);
  glut_menu[12] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("right         : p", 0);
  glutAddMenuEntry("left          : i", 0);
  glutAddMenuEntry("up            : 9", 0);
  glutAddMenuEntry("down          : o", 0);
  glut_menu[4] = glutCreateMenu(NULL);
  glutAddSubMenu("at the shoulders? ", glut_menu[5]);
  glutAddSubMenu("at the elbows?", glut_menu[6]);
  glut_menu[7] = glutCreateMenu(NULL);
  glutAddSubMenu("at the hip? ", glut_menu[8]);
  glutAddSubMenu("at the knees?", glut_menu[9]);
  glutAddSubMenu("at the ankles? ", glut_menu[10]);
  glut_menu[2] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("turn left    : d", 0);
  glutAddMenuEntry("turn right    : g", 0);
  glut_menu[3] = glutCreateMenu(glut_null_select);
  glutAddMenuEntry("tilt backwards : f", 0);
  glutAddMenuEntry("tilt forwards  : r", 0);
  glut_menu[0] = glutCreateMenu(NULL);
  glutAddSubMenu("move the arms.. ", glut_menu[4]);
  glutAddSubMenu("fire the vulcan guns?", glut_menu[1]);
  glutAddSubMenu("move the legs.. ", glut_menu[7]);
  glutAddSubMenu("move the torso?", glut_menu[2]);
  glutAddSubMenu("move the hip?", glut_menu[3]);
  glutAddSubMenu("rotate the scene..", glut_menu[11]);
#ifdef MOVE_LIGHT
  glutAddSubMenu("rotate the light source..", glut_menu[12]);
#endif
  glutCreateMenu(glut_menu_select);
#ifdef ANIMATION
  glutAddMenuEntry("Start Walk", 1);
  glutAddMenuEntry("Stop Walk", 2);
#endif
  glutAddMenuEntry("Toggle Wireframe", 3);
  glutAddSubMenu("How do I ..", glut_menu[0]);
  glutAddMenuEntry("Quit", 4);
  glutAttachMenu(GLUT_LEFT_BUTTON);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
void glut_menu_select(int mode)
{
  DEBUG_OUT<<"glut_menu_select("<<mode<<")...\n";
  switch (mode) {
#ifdef ANIMATION
  case 1:
    glutIdleFunc(animation);
    break;
#endif
  case 2:
    glutIdleFunc(NULL);
    break;
  case 3:
    //Toggle();
    glutPostRedisplay();
    break;
  case 4:
    exit(0);
  }
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
/* ARGSUSED */
void glut_null_select(int mode)
{
  DEBUG_OUT<<"glut_null_select("<<mode<<")...\n";
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
void glut_myMouse(int button, int state, int x, int y)
{
  DEBUG_OUT<<"glut_mouse("<<button<<","<<state<<","<<x<<","<<y<<")...\n";
  if(aDefaultAApplicationLoop) {
    AEvent *e=new AEvent;
    e->type=A_EVENT_MOUSEUP;
    e->code=1;
    e->modifier=0;
    e->number=1;
    e->x=x;  e->y=y;
    e->setWindow(aDefaultAWindow);
    aDefaultAApplicationLoop->dispatchEvent(e);
  }
}
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
void glut_myMotion(int x, int y)
{
  DEBUG_OUT<<"glut_motion("<<x<<","<<y<<")...\n";
  if(aDefaultAApplicationLoop) {
    AEvent *e=new AEvent;
    e->type=A_EVENT_MOUSEMOVE;
    e->code=0;
    e->modifier=0;
    e->number=0;
    e->x=x;  e->y=y;
    e->setWindow(aDefaultAWindow);
    aDefaultAApplicationLoop->dispatchEvent(e);
  }
}
#endif // ASYS_GLUT
