#ifndef ALIB_ALIB_GLUT_H
#define ALIB_ALIB_GLUT_H

/*

Use GLUT for all interface and graphics (portable to different actual platforms)

*/


#ifdef ASYS_GLUT
#define GLUT
#ifdef ASYS_DARWIN
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif // ASYS_DARWIN
#endif // ASYS_GLUT


#ifdef ASYS_GLUT
#ifdef __cplusplus
extern "C" {
#endif
int glut_start(int argc, char **argv);
void glut_myinit(void);
void glut_myReshape(int w, int h);
void glut_myDisplay(void);
void glut_myKeyboard(unsigned char key, int x, int y);
void glut_mySpecial(int key, int x, int y);
void glut_setupMenus(void);
void glut_menu_select(int mode);
void glut_null_select(int mode);
void glut_myMouse(int button, int state, int x, int y);
void glut_myMotion(int x, int y);
#ifdef __cplusplus
};
#endif
#endif // ASYS_GLUT


#endif
