#ifndef PLATFORM_ALIB_X11_H
#define PLATFORM_ALIB_X11_H


#include "ATypeDefs.h"
#include "APlatform.h"


#ifdef ASYS_GNOME
extern "C" int alibGnomeMain (int argc, char *argv[]);
#endif // ASYS_GNOME


#ifdef ASYS_X11
char *x11_return_event_name(unsigned int t);
#endif // ASYS_X11


#endif // PLATFORM_ALIB_X11_H

