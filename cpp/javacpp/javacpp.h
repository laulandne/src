#ifndef JAVAGLUE_JAVACPP_H
#define JAVAGLUE_JAVACPP_H


// Uncomment one of these three
// (This will be handled better later)
//#define NICK_USE_GUILIB
//#define NICK_USE_SDL
//#define NICK_USE_GLUT


//#define CRAZY_DEBUG 1


/*
Fun Java things to remember when translating to C++:

* data members are initialized to 0
* consts can be defined per class
* there are true class (vs instance) members
* sizeof an array actually works

*/


#define null NULL


typedef float Float;
typedef char * JString;
typedef bool boolean;



#endif // JAVAGLUE_JAVACPP_H
