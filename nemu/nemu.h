#ifndef NEMU_H
#define NEMU_H


#include <portable/nickcpp/ANullStream.h>
//#include <guilib/AEvent.h>
//#include <guilib/AApplicationLoop.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/AError.h>

#include <elib/EMemAbstract.h>


//class AWindowCharMapped;
//class AWindowOutputStream;
//class AWindowInputStream;
//class AEvent;
class AList;
class AClock;
class ADecoder;
class AList;
class DVfs;
class EMemMapAbstract;
class EMapSymbol;
class NBinaryNode;
class EMemAbstract;
class EHandler;
class EVideoIOAbstract;
class ECPUAbstract;
class EModule;
class NLd;
class NEmu;


#define NEMU_DEBUGBUF_SIZE 2560

#define NEMU_MAX_CPUS 4
#define NEMU_MAX_MAPS 16


//void handleKeyEventForLoop(AEvent *e);
//void handleTickEventForLoop(AEvent *e);


class NCmd {
public:
  // Public Member Functions
  NCmd();
  virtual ~NCmd();
  void reset();
  void ram(bool needToMap);
  void mount();
  void go() { debug_arg[0]=(char *)NULL; g(); }
  void source(char *arg) { debug_arg[0]=arg; source(); }
  void debug_interp();
  void setDebugLoc(unsigned int val) { debug_loc=val; }
  void setDebugArg(unsigned int n,char *str) { debug_arg[n]=str; }
  // Public Data Members
  AError err;
  unsigned int debug_pointer;
  char debug_buffer[NEMU_DEBUGBUF_SIZE];
private:
  // Public Member Functions
#ifndef ALIB_16BIT_INTS
  void rsrcdir();
  void rsrcload();
#endif // ALIB_16BIT_INTS
  void print();
  void loadcolorrom();
  void addrbits();
  void cat();
  void more();
  void mv();
  void cp();
  void rm();
  void ls();
  void cd();
  void ld();
  void nm();
  void symbol();
  void strings();
  void symbols();
  void modules();
  void module();
  void nodes();
  void node();
  void rb();
  void disk();
  void unmount();
  void disks();
  void d();
  void g();
  void jsr();
  void list();
  void tm_row(unsigned int);
  void m_row(unsigned int);
  void m();
  void tm();
  void step();
  void showVars();
  void set();
  void s();
  void source();
  void l();
  void help();
  void romload();
  void unused();
  void romlist();
  void areas();
  void memr();
  void memw();
  void memv();
  void tmem();
  void segs();
  void fill();
  void bank();
  void cpu();
  void map();
  void unmap();
  void ihex();
  void srec();
  void tempload();
  void romtemp();
  void romfromrom();
  //void mirror();
  void poke();
  void rpoke();
  void peek();
  void tempfree();
  bool do_cmd(char *tdebug_cmd);
  void debug_parse();
  void checkOutputCount();
  void memHeader();
  void warnCmd();
  void printSymbol(EMapSymbol *s);
  void printNode(NBinaryNode *n);
  void printModule(EModule *m);
  char convertToPrintable(char c);
  unsigned int parseNumberOrSymbol(char *s,bool errIfNotFound=true);
  //char *mangleName(char *S);
  //char *unmangleName(char *S);
  unsigned int evaluate(char *s,bool errIfNotFound=true);
  void symbols_list(AList *syms);
  void init();
  // Private Data Members
  bool inComment;
  bool foundIt;
  bool sourcing;
  char *loadingName;
  bool inBracket;
  bool cmdError;
  unsigned int curLoadAddr;
  unsigned int defaultNumLines;
  unsigned int outputCount;
  char last_debug_buffer[NEMU_DEBUGBUF_SIZE];
  unsigned int last_debug_pointer;
  unsigned int debug_args;
  char *debug_arg[10];
  char *debug_cmd;
  unsigned int debug_loc;
  BYTE *tempmem;
  ULONG tempsize;
  char curMemLine[16];
  char prevMemLine[16];
};


class NEmu {
public:
  // Public Member Functions
  NEmu();
  virtual ~NEmu();
  void ver();
  void start(int argc,char *argv[]);
  void getReady();
  //void handleEvent(AEvent *e);
  void handleNewIO(char *name);
  void handleNewCPU(char *name);
  void setCPU(unsigned int n);
  ECPUAbstract *getCPU() { return mycpu[curCPU]; }
  void setMap(unsigned int n);
  EMemMapAbstract *getMap() { return mymap[curMap]; }
  void clearRam();
  void quit();
  bool handleFile(char *fname);
  bool isItATextFile(char *fname);
  void debugger(char *str=(char *)NULL); // jump to debugger, no error
  void check_ints();
  void drawPrompt();
  DVfs *getVfs() const { return myvfs; }
  unsigned int getSigDigits() const { return sigDigits; }
  void setAddrBits(unsigned int bits,char *mapType=(char *)NULL);
  void setPageSize(ULONG psize);
  unsigned int getAddrBits() const { return addrBits; }
  ULONG getNumBytes() const { return numBytes; }
  ULONG getPageSize() const { return pageSize; }
  ULONG getNumPages() const { return numPages; }
  ULONG getAddrMask() const { return addrMask; }
  void setCheckOften();
  void setCheckRarely();
  void setError() { errorLoading=true; }
  // Public Data Members
  AError err;
  bool debugASAP;
  bool quitASAP;
  AList *roms;
  ULONG ticks, op_ticks;
  //AWindowCharMapped *w;
  AClock *myClock;
  bool unknown_stop;
  ULONG startTime,endTime;
  unsigned int loadOffset;
  char *configName;
  bool parseEmu;
  NCmd cmd;
  bool ignore;
  bool logging;
  bool useVfs;
  EMemMapAbstract *mymap[NEMU_MAX_MAPS];
  unsigned int curMap;
  EMemAbstract *ram;
  EHandler *myio;
  EVideoIOAbstract *myvideo;
  ECPUAbstract *mycpu[NEMU_MAX_CPUS];
  unsigned int curCPU;
  DVfs *myvfs;
  EModule *module;
  NLd *ld;
private:
  // Private Member Functions
  void runLoop();
  void debugLoop();
  void init();
  bool setupDebugWindow();
  void redrawDebugWindow();
  //void readPrefs();
  // Private Data Members
  ULONG addrBits;
  ULONG numBytes;
  ULONG numPages;
  ULONG pageSize,spageSize;
  ULONG addrMask;
  ULONG ticksTilCheck;
  unsigned int sigDigits;
  bool errorLoading;
  bool justStarting;
  bool dontStopNextTime;
  bool doReset;
  bool setUpMap;
  bool userSize;
  bool debugCharMap;
  bool showDebug;
  bool disasmThenQuit;
  bool traceOn;
  bool goToDebuggerOnStart;
  unsigned int debugWidth,debugHeight;
  DVfs *defaultVfs;  // TODO: Check if this is really needed...
  //AApplicationLoop loop;
  //AWindowOutputStream *outStream;
  //AWindowInputStream *inStream;
};


extern NEmu emu;


#endif

