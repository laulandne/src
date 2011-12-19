#ifndef ALIB_AAPP_H
#define ALIB_AAPP_H


#include <guilib/ATypeDefs.h>

#ifdef ASYS_WINDOWS
#include <guilib/alib_win.h>
#endif // ASYS_WINDOWS


class AList;
class ADisplay;
class AWindowAbstract;
//class AAudioDevice;
class AApplicationLoop;


// These should be moved to the event class or a new AMenu class...
// The various standard menu items
// These are returned in the code field of an A_EVENT_MENU event
// FIXME: This is not fully implemented yet.
// If your application receives an A_EVENT_MENU event, the code field
// is currently undefined.
#define A_MENU_ABOUT  1
#define A_MENU_NEW  2
#define A_MENU_OPEN  3
#define A_MENU_CLOSE  4
#define A_MENU_SAVE  5
#define A_MENU_SAVEAS  6
#define A_MENU_PRINT  7
#define A_MENU_QUIT  8
#define A_MENU_UNDO  9
#define A_MENU_CUT  10
#define A_MENU_COPY  11
#define A_MENU_PASTE  12
#define A_MENU_CLEAR  13
#define A_MENU_LAST  14
#define A_MAX_MENUS     64


#define ATEMPSTRINGBUF_SIZE 256
#define ALIB_MAX_ARGV 25


#ifndef MAXPATHLEN
#define MAXPATHLEN ATEMPSTRINGBUF_SIZE
#endif


// Application class:
// In the current implementation of ALib there is always a single
// instance of AApp called "aThisApp".
class AApp {
public:
  //
  // Public Member Functions
  //
  AApp();
  virtual ~AApp();
  void start(int aargc, char **aargv/*,unsigned int ourSize=sizeof(AApp)*/);
  void start(char *cmdline);  // we have to parse them...
  void start();
#ifdef ALIB_LIBRARY
  // Our main(), which isn't the real main() if we're working with a framework
#ifdef ASYS_WINDOWS
  INT PASCAL ourWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, INT nCmdShow);
#endif // ASYS_WINDOWS
  int ourMain(int aargc, char **aargv);
#else // ALIB_LIBRARY
  // The actual main() function the user has in their code
  int myMain(int aargc, char **aargv);
#endif // ALIB_LIBRARY
  //
  // System-style functions
  //
  //void about();
  void systemBeep();
  void displayUnfocus();
  bool setAppPrefs(char *name, char *val);
  char *getAppPrefs(char *name);
  //void initDefaultDisplay();
  //void initDefaultAudioDevice();
  //
  // Thead and multiprocessing stuff
  //
  void yieldToOtherProcesses();
  bool getCanThread();
  //
  // Menu functions...not really working...
  //
  //bool userMenu(unsigned int menuNum);
  bool activateMenu(char *menuName,char *itemName=(char *)NULL);
  bool disactivateMenu(char *menuName,char *itemName=(char *)NULL);
  //
  // String functions
  //
  void n_c2pas(char *s, char *d);
  //
  // Native host functions
  //
  char *getNativeCPUType();
  char *getNativeOSType();
  char *getNativeOSVersion();
  //
  // OS-dependant functions
  //
  //
  // Public Data Members
  //
  unsigned int version,versionMinor;  // Versions of host os
  // Command line handling
  unsigned int argc;
  char *argv[ALIB_MAX_ARGV];
  // true if system is little-endian.
  bool littleEndian;
  // The next two are used for the unfinished menuing system...
  bool menuItem[A_MAX_MENUS];  // "Does the user want to hear about the menu?"
  bool itemEnabled[A_MAX_MENUS];  // "Is the menu selectable?"
  // for windows to open onto if the user doesn't have their own ADisplay
  //AWindowAbstract *defaultWindow;
  //AAudioDevice *defaultAudioDevice;
  // true if there is a pending AWindow::sync call, not all systems use it
  bool sync;
  // Used by some systems, will be a copy of the currently occuring event
  // This is used when we need a pointer to the last window opened for something
  // List of availiable display modes...see display.cpp
  // Values are private and defined in display.cpp
  // if true, decoder will tell you any file it can't recognize is a
  // "raw sound file"
  bool allowRawDecode;
  bool joyNotKeys;
  unsigned int whichJoy;
  bool hasOpenGL;  // or Mesa, or similar
  bool has3DHardware;  // If this isn't true, don't push too many pixels!
  //
  // OS-dependant data members
  //
#ifdef ASYS_PALM
  bool hasThreePointFive;
  bool hasIntl;
  bool hasJapanese;
  bool hasWireless;
  bool hasColor;
  bool hasHighRes;
  bool hasVFS;
  bool isHandspring;
  bool isSony;
#endif // ASYS_PALM
#ifdef ASYS_OS2
  HAB ahAB;
  HMQ ahMQ;
  QMSG aQMsg;
  HWND ahWnd;
#endif // ASYS_OS2
#ifdef ASYS_DOS
  // true if a msmouse driver has been detected
  bool hasMouse;
  bool hercGfx;
  bool hasMono,monoMode,hasHerc;
  bool setMode;
  // The version of the vesa bios found or 0.0 if none.
  unsigned int vesaMajor,vesaMinor;
  unsigned int videoMemory;
  // These are the centered joystick values
  unsigned short center1x,center1y;
  unsigned short center2x,center2y;
  bool extendedKey;
#endif // ASYS_DOS
#ifdef ASYS_X11_BASED
  bool visualWarned;
  bool hasJoystick;
  int joyfd;
  char numJoyAxes;
  char numJoyButtons;
  unsigned int axe0middle,axe1middle;
#endif // ASYS_X11_BASED
#ifdef ASYS_AMIGA
  // Lock on current dir when we started or NULL
  BPTR oldDir;
  bool hasECS,hasAGA,hasPicasso;
#endif // ASYS_AMIGA
#ifdef USE_ASYNC_STDIO_INPUT
  int input_fd;
#endif // USE_ASYNC_STDIO_INPUT
  //
  //
  //
protected:
  //
  // Private Member Functions
  //
  void init();
  void handleArgs(int aargc,char **aargv, bool maybeWideChars=true);
  bool turnMenuNumOn(unsigned int menuNum);
  bool turnMenuNumOff(unsigned int menuNum);
  //
#ifdef ASYS_MAC
  void mac_start();
#endif // ASYS_MAC
#ifdef ASYS_COCOA
  void cocoa_start();
#endif // ASYS_COCOA
#ifdef ASYS_PALM
  void palm_start();
#endif // ASYS_PALM
#ifdef ASYS_AMIGA
  void amiga_start();
#endif // ASYS_AMIGA
#ifdef ASYS_DOS
  void dos_start();
#endif // ASYS_DOS
#ifdef ASYS_X11
  void x11_start();
#endif // ASYS_X11
#ifdef ASYS_OS2
  void os2_start();
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
  void win_start();
#endif // ASYS_WINDOWS
  //
  // Private data members
  //
  // OS-Dependant data members
  //
#ifdef ASYS_DOS
  // The video bios mode that the system was in when started
  unsigned int origMode;
  unsigned char defaultAttrib;
#endif // ASYS_DOS
};


extern AApp aThisApp;


#endif // ALIB_AAPP_H
