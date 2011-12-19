
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream

/*

I have reserved "NIK8" as the Apple creator code for this application...
I have reserved "NIK8" as the PalmPilot creator code for this application...

*/


#define NEMU_FOREGROUND_COLOR 0xffff00
#define NEMU_BACKGROUND_COLOR 0x800080


#include "nemu.h"


#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/AFileOutputStream.h>
#include <portable/nickcpp/AFileInputStream.h>
#include <portable/nickcpp/AClock.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/ndecoder/ADecoder.h>
#include <portable/ndecoder/ASmartDecoder.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EMemMapComplex.h>
#include <elib/EMemMapSimple.h>

#include <disklib/DVfs.h>

#include <binarylib/NLd.h>
#include <binarylib/NBinaryDecoder.h>

#include <cpulib/ECPUAbstract.h>


#include <cpulib/EAlpha.h>
#include <cpulib/EArm.h>
#include <cpulib/EHitachi.h>
#include <cpulib/EJvm.h>
#include <cpulib/EDotNet.h>
#include <cpulib/EMips.h>
#include <cpulib/Em6502.h>
#include <cpulib/Em68k.h>
#include <cpulib/Em6800.h>
#include <cpulib/Em6809.h>
#include <cpulib/Em6811.h>
#include <cpulib/EPowerPC.h>
#include <cpulib/ESparc.h>
#include <cpulib/ESh.h>
#include <cpulib/ESwf.h>
#include <cpulib/Ex86.h>
#include <cpulib/Ez80.h>
#include <cpulib/EZMachine.h>


#include <emu/NAcornHandler.h>
#include <emu/NAppleHandler.h>
#include <emu/NAmigaHandler.h>
#include <emu/NAtariHandler.h>
#include <emu/NAtariSTHandler.h>
#include <emu/NAtariArcadeHandler.h>
#include <emu/NAtariVectorHandler.h>
#include <emu/NCbmHandler.h>
#include <emu/NCocoHandler.h>
#include <emu/NCpmHandler.h>
#include <emu/NFamicomHandler.h>
#include <emu/NGenesisHandler.h>
#include <emu/NJavaHandler.h>
#include <emu/NMacHandler.h>
#include <emu/NOricHandler.h>
#include <emu/NPalmHandler.h>
#include <emu/NPcHandler.h>
#include <emu/NTrs80Handler.h>
#include <emu/NUnixHandler.h>
#include <emu/NSwfHandler.h>
#include <emu/NZMachineHandler.h>


NEmu emu;


int main(int argc,char **argv)
{
  //aThisApp.start(argc,argv);
  DEBUG_OUT<<"(Going to start...)\n";
  emu.start(argc,argv);
  DEBUG_OUT<<"(start returned)\n";
  return 0;
}


////////////////////////////////////////////////////////////////////////////////
//  NEmu Class
////////////////////////////////////////////////////////////////////////////////

NEmu::NEmu()
{
  std::cerr<<"Nick's Emulator version "<<"whatever"<<"\n";
  init();
  std::cerr<<"NEmu constructor done.\n";
}


void NEmu::init()
{
  debugASAP=false;
  ticks=0;  op_ticks=0;
  //w=(AWindowCharMapped *)NULL;
  unknown_stop=false;
  startTime=0;  endTime=0;
  loadOffset=0;
  configName=(char *)NULL;
  parseEmu=false;
  ignore=false;
  unsigned int i;
  for(i=0;i<NEMU_MAX_MAPS;i++) mymap[i]=(EMemMapAbstract *)NULL;
  curMap=0;
  module=(EModule *)NULL;
  ram=(EMemAbstract *)NULL;
  myio=(EHandler *)NULL;
  myvideo=(EVideoIOAbstract *)NULL;
  for(i=0;i<NEMU_MAX_CPUS;i++) mycpu[i]=(ECPUAbstract *)NULL;
  curCPU=0;
  myvfs=(DVfs *)NULL;
  defaultVfs=(DVfs *)NULL;
  addrBits=16;
  numBytes=65535;
  numPages=256;
  pageSize=256;
  spageSize=0;
  addrMask=EMemMapAbstract::bits2Bytes(addrBits)-1;
  ticksTilCheck=640;
  userSize=false;
  debugWidth=0;  debugHeight=0;
  debugCharMap=true;
  showDebug=true;
  traceOn=false;
  disasmThenQuit=false;
  errorLoading=false;
  quitASAP=false;
  doReset=true;
  sigDigits=2;
  setUpMap=true;
  justStarting=true;
  logging=false;
  useVfs=true;
  dontStopNextTime=false;
  goToDebuggerOnStart=true;
  roms=new AList;
  //outStream=new AWindowOutputStream(NULL,aStdOutStream);
  //inStream=new AWindowInputStream(NULL,aStdInStream);
  //loop.regEvent(A_EVENT_KEYUP,handleKeyEventForLoop);
  //loop.regEvent(A_EVENT_TICK,handleTickEventForLoop);
}


NEmu::~NEmu()
{
  std::cerr<<"NEmu destructor.\n";
  if(!err.error()) {
    // NOTE: Do NOT delete myvideo...
    if(myio) delete myio;
    myio=(EHandler *)NULL;  myvideo=(EVideoIOAbstract *)NULL;
    // NOTE: Doesn't handle multiple cpus
    if(mycpu[curCPU]) delete mycpu[curCPU];
    mycpu[curCPU]=(ECPUAbstract *)NULL;
    // NOTE: Doesn't handle multiple maps
    if(mymap[curMap]) delete mymap[curMap];
    mymap[curMap]=(EMemMapAbstract *)NULL;
    //if(w) delete w;
    //w=(AWindowCharMapped *)NULL;
  }
}


/*
void NEmu::readPrefs()
{
  //std::cerr<<"NEmu attempting to read preferences...\n";
  char *prefString=(char *)NULL;
  prefString=aThisApp.getAppPrefs("debugWidth"));
  if(prefString) {
    debugWidth=aDefaultStringHelper.parseNumber(prefString);
    //std::cerr<<"Set debugWidth from prefs to "<<debugWidth<<"\n";
    userSize=true;
  }
  prefString=aThisApp.getAppPrefs("debugHeight"));
  if(prefString) {
    debugHeight=aDefaultStringHelper.parseNumber(prefString);
    //std::cerr<<"Set debugHeight from prefs to "<<debugHeight<<"\n";
    userSize=true;
  }
  if(userSize) {
    if(!debugWidth) debugWidth=380;
    if(!debugHeight) {
      //debugHeight=600-taskbarHeight;
      debugHeight=560;
    }
  }
  prefString=aThisApp.getAppPrefs("debugCharMap"));
  if(prefString) {
    debugCharMap=aDefaultStringHelper.parseNumber(prefString);
    //std::cerr<<"Set debugCharMap from prefs to "<<debugCharMap<<"\n";
    userSize=true;
  }
}
*/


void NEmu::setPageSize(ULONG psize)
{
  spageSize=psize;
  //setAddrBits(addrBits);
}


// TODO: if anything goes wrong, put up a window explaining why before
// bailing...
// NOTE: trying more than 32 bits is totally experimental...
void NEmu::setAddrBits(unsigned int bits,char *mapType)
{
  // TODO: Put up warning that this will destroy current map...
  // For code common to Windows and vfs
  DEBUG_OUT<<"setAddrBits("<<bits<<",)\n";
  if(!bits) { debugger("Tried to setAddrBits to 0!\n"); return; }
  if(bits>128) { debugger("Tried to setAddrBits too high!\n"); return; }
  if(ld) ld->reset();
  if(mymap[curMap]) { delete mymap[curMap]; mymap[curMap]=(EMemMapAbstract *)NULL; }
  if(mycpu[curCPU]) mycpu[curCPU]->setMap((EMemMapAbstract *)NULL);
  if(myio) myio->setMap((EMemMapAbstract *)NULL);
  if(ld) ld->setMap((EMemMapAbstract *)NULL);
  addrBits=bits;
  sigDigits=2;
  if(bits>8) sigDigits=4;
  if(bits>16) sigDigits=6;
  if(bits>24) sigDigits=8;
  //if(w) w->setHexDigits(sigDigits);
  if(bits>32) {
    //std::cerr<<"Trying more than 32 bits!\n");
  }
  numBytes=EMemMapAbstract::bits2Bytes(addrBits);
  numPages=256;
  pageSize=numBytes/numPages;
  if(bits>32) {
    DEBUG_OUT<<"First guess:\n";
    DEBUG_OUT<<"numBytes="<<numBytes<<"\n";
    DEBUG_OUT<<"numPages="<<numPages<<"\n";
    DEBUG_OUT<<"pageSize="<<pageSize<<"\n";
  }
  // If pages are larger than 1m, use more to decrease size
  if(pageSize>0x100000) {
    DEBUG_OUT<<"(switching numpages from 256 bytes to 64k)\n";
    numPages=65536;
    pageSize=numBytes/numPages;
    if(addrBits==32) {
      // Hard code this...
      pageSize=65536;
      DEBUG_OUT<<"(switching page size to 64k)\n";
    }
  }
  if(bits>32) {
   DEBUG_OUT<<"Second guess:\n";
   DEBUG_OUT<<"numBytes="<<numBytes<<"\n";
   DEBUG_OUT<<"numPages="<<numPages<<"\n";
   DEBUG_OUT<<"pageSize="<<pageSize<<"\n";
  }
  if(spageSize) {
    DEBUG_OUT<<"(using calc'd spagesize)\n";
    pageSize=spageSize;
    numPages=numBytes/pageSize;
  }
  if(bits>32) {
    DEBUG_OUT<<"Third guess:\n";
    DEBUG_OUT<<"numBytes="<<numBytes<<"\n";
    DEBUG_OUT<<"numPages="<<numPages<<"\n";
    DEBUG_OUT<<"pageSize="<<pageSize<<"\n";
  }
  if(bits<32) addrMask=numBytes-1;
  else addrMask=numBytes;
  DEBUG_OUT<<"map, abits:"<<addrBits<<" nbytes:"<<numBytes;
  DEBUG_OUT<<"="<<numPages<<"*"<<pageSize<<" (amask:";
  DEBUG_OUT<<aDefaultStringHelper.myHex(addrMask)<<")\n";
  DEBUG_OUT<<"Creating map of "<<aDefaultStringHelper.myHex(numBytes)<<" bytes,\n";
  DEBUG_OUT<<aDefaultStringHelper.myDec(numPages)<<" pages...";
  if(!mymap[curMap]) {
    if(numPages==256) mymap[curMap]=new EMemMapSimple(numPages,pageSize,bits);
    else mymap[curMap]=new EMemMapComplex(numPages,pageSize,bits);
  }
  if(!mymap[curMap]) {
    std::cerr<<"NULL map pointer!\n"; err.set(); return;
  }
  if(mymap[curMap]->err.error()) {
    std::cerr<<"Couldn't create map!\n";
    delete mymap[curMap];
    mymap[curMap]=(EMemMapAbstract *)NULL;
    err.set();
    exit(5);
  }
  //ram=mymap[curMap]->getMem();
  //mymap[curMap]->setWindow(w);
  if(mycpu[curCPU]) mycpu[curCPU]->setMap(mymap[curMap]);
  if(myio) myio->setMap(mymap[curMap]);
  if(ld) ld->setMap(mymap[curMap]);
  DEBUG_OUT<<"ok.\n";
  if(err.error()) quit();
}


bool NEmu::setupDebugWindow()
{
  if(err.error()) { std::cerr<<"errorFlag, can't setupDebugWindow!\n";  return false;  }
  //if(w) { std::cerr<<"(setupDebugWindow::already did it)\n"; return true; }
  std::cerr<<"NEmu::setupDebugWindow().\n";
  if(!userSize) { debugWidth=320;  debugHeight=400; }
  //w=new AWindowCharMapped("Nick's emulator"),debugWidth,debugHeight);
  //if(!w) { std::cerr<<"NULL w pointer\n"; err.set(); return false; }
  //if(w->err.error()) {  std::cerr<<"Window has error!\n"; err.set(); return false; }
  //outStream->setWindow(w);  inStream->setWindow(w);
  //outStream->hex();
  //loop.associateWithWindow(inStream);
  //w->hex();
  //w->setForeground(NEMU_FOREGROUND_COLOR); w->setBackground(NEMU_BACKGROUND_COLOR);
  //if(!userSize) {
  //  debugHeight=w->getParent()->getHeight();
  //  w->resize(debugWidth,debugHeight);
  //}
  //char *fontName=w->queryFontAvail(NULL,0,A_FONT_NONPROP);
  //w->setFont(fontName);
  //w->focus();
  //w->sync();
  //w->move(0,w->getParent()->getHeight()-w->getHeight()-w->getBorderHeight());
  //if(map) mymap[curMap]->setWindow(w);
  //defaultVfs->setWindow(w);
  //aThisApp.activateMenu("File"),"Open"));  // kinda hack-ish...
  //w->clear();
  ver();
   //w->focus();
  DEBUG_OUT<<"NEmu::setupDebugWindow() done.\n";
  return true;
}


void NEmu::start(int argc, char *argv[])
{
  DEBUG_OUT<<"NEmu::start()...\n";
  //w=(AWindowCharMapped *)NULL;
  DEBUG_OUT<<"start Going to readPrefs...\n";
  //readPrefs();
  DEBUG_OUT<<"start Going to create default vfs...\n";
  defaultVfs=new DVfs();
  if(!defaultVfs) {  std::cerr<<"(No default vfs!)\n";  exit(5);  }
  myvfs=defaultVfs;
  setUpMap=true;
  aDefaultStringHelper.setHexMode();
  char tc[2];
  configName=(char *)NULL;
  DEBUG_OUT<<"start Going to process argv...\n";
  unsigned int whichArg=1;
  while(whichArg<argc) {
    DEBUG_OUT<<"whichArg="<<whichArg<<"\n";
    if(!strcmp("-debug",argv[whichArg])) {
      whichArg++;
      showDebug=true;
      goToDebuggerOnStart=true;
    } else
    if(!strcmp("-norun",argv[whichArg])) {
      whichArg++;
      showDebug=true;
      goToDebuggerOnStart=true;
      traceOn=true;
    } else
    if(!strcmp("-disasm",argv[whichArg])) {
      whichArg++;
      showDebug=true;
      disasmThenQuit=true;
    } else
    if(!strcmp("-tron",argv[whichArg])) {
      whichArg++;
      showDebug=true;
      traceOn=true;
    } else
    if(whichArg<argc) {
      configName=strdup(argv[whichArg]);
      whichArg++;
    }
  }
  if(argc==1) {
    //if(!setupDebugWindow()) return;
    if(err.error()) return;
    DEBUG_OUT<<"NEmu::start() Going to ask for a file...\n";
    DEBUG_OUT<<"Please choose a file...\n";
    //ARequesterFile *rw=new ARequesterFile();
    //if(rw) {
      //configName=rw->show("Pick a file to load:"));
      //delete rw;  rw=(ARequesterFile *)NULL;
      //redrawDebugWindow();
    //}
  }
  DEBUG_OUT<<"start Done processing argv.\n";
  if(showDebug) setupDebugWindow();
  tc[1]=0;
  errorLoading=true;
  if(configName) {
    DEBUG_OUT<<"start Going to handleFile if given...\n";
    if(handleFile(configName)) errorLoading=false;
    DEBUG_OUT<<"start got back from handleFile if given.\n";
  }
  else {
    goToDebuggerOnStart=true;
  }
  if(quitASAP) {
    std::cerr<<"start returning due to quitASAP!\n";
    return;
  }
  //if(setUpMap) setAddrBits(16);
  if(err.error()) {
    std::cerr<<"start returning due to errorFlag!\n";
    quit();
    return;
  }
  setUpMap=false;
  DEBUG_OUT<<"start Going to myio->redraw...\n";
  if(myio) myio->redraw();
  justStarting=false;
  if(quitASAP) {
    std::cerr<<"start returning due to quitASAP!\n";
    return;
  }
  if(mycpu[curCPU]) mycpu[curCPU]->reset();
  if(goToDebuggerOnStart) {
    DEBUG_OUT<<"start goToDebuggerOnStart was set\n";
    debugger();
  }
  else {
    DEBUG_OUT<<"Starting CPU...\n";
    //aDefaultAWindowOutputStream->getWindow()->clear();
    cmd.go();
  }
  while(!quitASAP) {
    if(mycpu[curCPU]) {
      if(mycpu[curCPU]->getRunning()) { runLoop(); }
      else { debugLoop(); }
    }
    else { debugLoop(); }
  }
  DEBUG_OUT<<"NEmu::start is done.\n";
}


bool NEmu::handleFile(char *ffname)
{
  if(err.error()) return false;
  bool oldUseVfs=useVfs;
  useVfs=false;
#ifdef ALIB_HAVE_CHDIR
  char pathName[1024];
  if(aDefaultStringHelper.getPathFromFullName(pathName,ffname)) {
    // If there's a prefixed path to the config file, we'll go to that directory
    DEBUG_OUT<<"path is "<<pathName<<"\n";
    chdir(pathName);
  }
  char fileName[1024];
  if(aDefaultStringHelper.getNameFromFullName(fileName,ffname)) {
    DEBUG_OUT<<"file is "<<fileName<<"\n";
    ffname=fileName;
  }
#endif // ALIB_HAVE_CHDIR
  char *fname=strdup(ffname);
  DEBUG_OUT<<"NEmu::handleFile("<<fname<<")\n";
  FILE *ff=(FILE *)NULL;
  if(useVfs) {
    if(!myvfs) {
      std::cerr<<"No vfs!\n";
      return false;
    }
    ff=(FILE *)myvfs->Fopen(fname,"r");
  }
  else ff=fopen(fname,"r");
  if(!ff) ff=fopen(aDefaultStringHelper.removeQuotes(fname),"r");
  if(!ff) ff=fopen(aDefaultStringHelper.flipSlashes(fname),"r");
  if(!ff) {
    std::cerr<<"No such file as \""<<fname<<"\"!\n";
    return false;
  }
  if(!ff) return false;
  //if(!s) setupDebugWindow()) return false;
  if(err.error()) return false;
  DEBUG_OUT<<"Nemu giving NLD a crack at "<<fname<<"...\n";
  NBinaryDecoder *nbd=NLd::pickDecoder(fname,defaultVfs);
  if(nbd) {
    //w->setDebugOutput(true);
    DEBUG_OUT<<"Loading binary \""<<fname<<"\"...\n";
    DEBUG_OUT<<"\""<<fname<<"\"\n";
    DEBUG_OUT<<"is a "; DEBUG_OUT<<nbd->which(); DEBUG_OUT<<" binary.\n";
    delete nbd;  nbd=(NBinaryDecoder *)NULL;
    cmd.reset();
    setAddrBits(32);  // 32 bit address map
    setUpMap=false;
    mymap[curMap]->setPageFaults(true);
    mymap[curMap]->setDemandPaging(true);
    mymap[curMap]->setVirtualMemory(true);
    //mymap[curMap]->clearVideoMap(); // so we don't double fault
    myvfs=new DVfs();
    module=new EModule(fname);
    if(!module) {
      std::cerr<<"Couldn't get new emodule!!!\n";
    }
    mymap[curMap]->addModule(module);
    ld=new NLd(module);
    if(ld) {
      if(!ld->err.error()) {
        ld->setVfs(myvfs);
        ld->setMap(mymap[curMap]);
        ld->ld(fname);
        module->getNodes()->reset();
        if(module->getUnknown()) {
          std::cerr<<"An error occured loading \""<<fname<<"\"!\n";
          return false;
        }
      }
      if(ld->err.error()) {
        std::cerr<<"An error occured loading \""<<fname<<"\"!\n";
        return false;
      }
    }
    DEBUG_OUT<<"Setting up environment...\n";
    doReset=false;
    if(ld) {
      if(module->getShouldDemandPage()) {
        mymap[curMap]->setPageFaults(true);
        mymap[curMap]->setDemandPaging(true);
        mymap[curMap]->setVirtualMemory(true);
      }
      handleNewCPU(module->whichCPUType());
      handleNewIO(module->whichIOType());
      module->calcAddrAndSizeFromNodes();
    }
    if(myio) myio->reset();
    if(mycpu[curCPU]) {
      if(ld) {
        DEBUG_OUT<<"Setting up cpu parameters...\n";
        mycpu[curCPU]->setBitness(module->getBitness());
        mycpu[curCPU]->setLSB(module->getLSB());
        mycpu[curCPU]->setResetEntry(module->getEntry());
      }
      mycpu[curCPU]->reset();
      if(ld) {
        DEBUG_OUT<<"Setting initial stack...\n";
        //if(!ld->error()) mycpu[curCPU]->jump(module->getEntry());
        mycpu[curCPU]->setStack(ECPUABSTRACT_DEFAULT_STACK);
        mycpu[curCPU]->setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
      }
    }
    if(myio) myio->reset_postCPU();
    //cmd.go();
    useVfs=oldUseVfs;
    if(ld) cmd.setDebugLoc(module->getEntry());
    DEBUG_OUT<<"Done.\n";
    return true;
  }
  DEBUG_OUT<<"Nope, NLD doesn't recognize it as a binary.\n";
  DEBUG_OUT<<"(Not a binary)\n";
  if(myvfs) {
    DEBUG_OUT<<"Nemu giving myvfs->recognize a crack at "<<fname<<"...\n";
    if(myvfs->recognize(fname)) {
      DEBUG_OUT<<"recognize returned true...\n";
      cmd.setDebugArg(0,strdup(fname));
      cmd.mount();
      useVfs=oldUseVfs;
      //w->focus();
      if(mycpu[curCPU]) { if(!mycpu[curCPU]->getRunning()) drawPrompt(); }
      return true;
    }
    DEBUG_OUT<<"Nope, myvfs doesn't recognize it as a disk image.\n";
  }
  DEBUG_OUT<<"Nemu checking if "<<fname<<" is a text file...\n";
  if(isItATextFile(fname)) {
    DEBUG_OUT<<"Looks like a text file...\n";
    DEBUG_OUT<<"Loading config from '"<<fname<<"'\n";
    //if(setUpMap) setAddrBits(16);
    setUpMap=false;
    DEBUG_OUT<<"Going to ask cmd to source...\n";
    if(!setupDebugWindow()) return false;
    cmd.source(fname);
    DEBUG_OUT<<"cmd done sourcing.\n";
    useVfs=oldUseVfs;
    return true;
  }
  else {
    DEBUG_OUT<<"(Doesn't look like a text file either)\n";
  }
  if(ff) {
    DEBUG_OUT<<"Can't tell what \""<<fname<<"\" is!\n";
    DEBUG_OUT<<"Can't tell what \""<<fname<<"\" is!\n";
    if(myvfs) myvfs->Fclose(ff); else fclose(ff);
  }
  else {
    // TODO: We never get here...do we?
    DEBUG_OUT<<"Couldn't open \""<<fname<<"\"!\n";
  }
  return false;
}


void NEmu::debugLoop()
{
  //if(!w) {std::cerr<<"No debug window, can't debugLoop!\n"; quitASAP=true; return; }
  //if(w->err.error()) { std::cerr<<"debug window has error, can't debugLoop!\n"; quitASAP=true; return; }
  std::cerr<<"Entering debugLoop...\n";
  //unsigned int tw=w->getWidth();
  //unsigned int th=w->getHeight();
  //if(tw<600) tw=600;
  //if(th<400) th=400;
  //w->resize(tw,th);
  //w->setForeground(NEMU_FOREGROUND_COLOR);
  //w->setBackground(NEMU_BACKGROUND_COLOR);
  //w->clear();
  cmd.debug_buffer[0]=0;
  cmd.debug_pointer=0;
  //AEvent e;
  //e.type=A_EVENT_NONE; // so its valid, even if we never read a real event
  //w->focus();
  bool waiting=true;
  while(waiting) {
    std::cerr<<"debugLoop looping...\n";
    drawPrompt();
    //e.type=A_EVENT_NONE;
    //w->drawCursor();
    //e.wait(w);
    //handleEvent(&e);
    //loop.run();
    //w->eraseCursor();
    fgets(cmd.debug_buffer,NEMU_DEBUGBUF_SIZE,stdin);
    cmd.debug_pointer=strlen(cmd.debug_buffer);
    cmd.debug_interp();
    if(quitASAP) waiting=false;
    if(mycpu[curCPU]) { if(mycpu[curCPU]->getRunning()) waiting=false; }
  }
  std::cerr<<"Exiting debugLoop...\n";
}


void NEmu::runLoop()
{
  std::cerr<<"Entering runLoop...\n";
  if(!mycpu[curCPU]) { debugger("can't runLoop, no cpu!"); return; }
  if(!mycpu[curCPU]->getRunning()) { debugger("can't runLoop, cpu not running!"); return; }
  if(traceOn) {
    if(mycpu[curCPU]) mycpu[curCPU]->setTrace(true);
  }
  debugASAP=false;
  //AEvent e;
  //e.type=A_EVENT_NONE; // so its valid, even if we never read a real event
  bool waiting=true;
  while(waiting) {
    //std::cerr<<"runLoop looping...\n";
    ticks++;
    if(ticks==ticksTilCheck) {
      //if(w) e<<(w);
      //if(e.type!=A_EVENT_NONE) handleEvent(&e);
      ticks=0;
    }
    if(mycpu[curCPU]->getTrace()) mycpu[curCPU]->disasmPC();
    mycpu[curCPU]->execute();
    if(mycpu[curCPU]->getDebug()) {
      DEBUG_OUT<<"\n";
      DEBUG_OUT<<"CPU caused debug.\n";
      debugASAP=true;
      mycpu[curCPU]->setDebug(false);
    }
    if(myio) {
      myio->io_cycle();
      if(myio->getDebug()) {
        DEBUG_OUT<<"\n";
        DEBUG_OUT<<"IO/OS caused debug.\n";
        debugASAP=true;
        myio->setDebug(false);
      }
    }
    if(mymap[curMap]) {
      if(mymap[curMap]->getDebug()) {
        DEBUG_OUT<<"\n";
        DEBUG_OUT<<"Map caused debug.\n";
        debugASAP=true;
        mymap[curMap]->setDebug(false);
      }
    }
    if(quitASAP) waiting=false;
    if(debugASAP) {
      waiting=false; mycpu[curCPU]->setRunning(false);
      mycpu[curCPU]->wregs();
    }
  }
  std::cerr<<"Exiting runLoop...\n";
}


void NEmu::getReady()
{
  if(doReset) cmd.reset();
  //if(w) {
    //char *fontName=w->queryFontAvail(NULL,0,A_FONT_NONPROP);
    //w->setFont(fontName);
    //w->clear();  w->moveTo(0,0);
    //w->focus();
  //}
  DEBUG_OUT<<"\n";
  if(mycpu[curCPU]) {
    ticks=0;
    if(mycpu[curCPU]->getRunning()) { startTime=myClock->read(); }
  }
  else {
    startTime=0;
    debugger();
  }
}


/*
void handleKeyEventForLoop(AEvent *e)
{
  std::cerr<<"handleKeyEventForLoop...\n";
  if(emu.quitASAP) exit(0);
  emu.handleEvent(e);
}
*/


/*
void handleTickEventForLoop(AEvent *e)
{
  if(emu.quitASAP) exit(0);
  std::cerr<<"handleTickEventForLoop...\n";
  emu.handleEvent(e);
}
*/


/*
void NEmu::handleEvent(AEvent *e)
{
  //std::cerr<<"handleEvent: "<<e->type<<"\n";
  ARequesterFile *rw=NULL;
  switch(e->type) {
  case A_EVENT_NONE:
    // Ignore
    break;
  case A_EVENT_MENU:
    switch(e->code) {
    case A_MENU_OPEN:
      rw=new ARequesterFile();
      configName=rw->show("Pick a file to load:"));
      delete rw;  rw=(ARequesterFile *)NULL;
      redrawDebugWindow();
      if(configName) {
        if(handleFile(configName)) errorLoading=false;
        dontStopNextTime=true;
      }
      // TODO: Probably should stop whatever was running before we handleFile, etc.
      break;
    default:
      std::cerr<<"Unhandled memu code="<<e->code<<"!\n";
      exit(5);
    }
    break;
  case A_EVENT_KEYDOWN:
    // Ignore, and we'll handle the cooked keypress as an A_EVENT_KEYPRESS event instead...
    break;
  case A_EVENT_KEYUP:
    switch(e->code) {
      case A_KEY_ESC:
        debugASAP=true;
        break;
      default:
        if(mycpu[curCPU]) {
          if(!mycpu[curCPU]->getRunning()) {
            // We've got a cpu, but we aren't running
            if(e->input_string(cmd.debug_buffer,&cmd.debug_pointer,NEMU_DEBUGBUF_SIZE)) {
              cmd.debug_interp();
              drawPrompt();
            }
          }
          else {
          // We've got a cpu and we're running code
          //if(ignore) {
            //ignore=false;
            //std::cerr<<"(Ignored a keypress while running)\n";
          //}
          //else {
            if(myio) {
              e->handleModKeys();
              myio->keypress(e->code);
              std::cerr<<"Running keypress was: "<<(long)e->code<<"\n";
            }
          //}
          }
        }
        else {
          // No cpu present
          std::cerr<<"Debug keypress was: "<<(long)e->code<<"\n";
          if(e->input_string(cmd.debug_buffer,&cmd.debug_pointer,NEMU_DEBUGBUF_SIZE)) {
            cmd.debug_interp();
            drawPrompt();
          }
        }
        break;
    }
    break;
  case A_EVENT_BREAK:
    quitASAP=true;
    break;
  case A_EVENT_DESTROY:
    quitASAP=true;
    break;
  case A_EVENT_RESIZE:
  case A_EVENT_MOVED:
  case A_EVENT_DISPLAY:
    w->sync();
    DEBUG_OUT<<"Resized to "<<w->getWidth()<<"x"<<w->getHeight()<<"\n";
    redrawDebugWindow();
    if(myio) {
      myio->focusVideo();
      myio->redraw();
    }
    break;
  case A_EVENT_REDRAW:
  case A_EVENT_FOCUS:
    if(myio) myio->redraw();
    redrawDebugWindow();
    break;
  case A_EVENT_MOUSEUP:
  case A_EVENT_MOUSEDOWN:
  case A_EVENT_MOUSEMOVE:
   // Ignore for now, later pass on to emulated system...
    break;
  case A_EVENT_UNFOCUS:
    // Ignore
    break;
  default:
    std::cerr<<"Unhandled event type="<<e->type<<"!\n";
    exit(5);
    break;
  }
}
*/


void NEmu::redrawDebugWindow()
{
  return;
  //std::cerr<<"(redraw)\n";
  if(debugCharMap) {
    DEBUG_OUT<<"#";
    //w->saveCursorPosition();
    //w->clear(true);
    //w->restoreCursorPosition();
    //char *fontName=w->queryFontAvail(NULL,0,A_FONT_NONPROP);
    //w->setFont(fontName);
    //w->redraw();
    drawPrompt();
  }
}


void NEmu::debugger(char *str)
{
  std::cerr<<"Entering debugger!\n";
  //exit(5);
  //w->setForeground(NEMU_FOREGROUND_COLOR); w->setBackground(NEMU_BACKGROUND_COLOR);
  if(debugASAP) { std::cerr<<"debugger returning because debugASAP is set!\n"; return; }
  debugASAP=true;
  if(mymap[curMap]) { if(mymap[curMap]->err.error()) quitASAP=true; }
  if(dontStopNextTime) {
    dontStopNextTime=false;
    std::cerr<<"debugger returning because dontStopNextTime was set!\n";
    return;
  }
  //if(!w) { setupDebugWindow(); }
  //w->clear();  w->moveTo(0,0);
  if(err.error()) { std::cerr<<"debugger returning because errorFlag was set!\n"; return; }
  endTime=myClock->read();
  if(str) { DEBUG_OUT<<str<<"\n"; }
  if(mycpu[curCPU]) {
    mycpu[curCPU]->setRunning(false);
    mycpu[curCPU]->wregs();
    cmd.setDebugLoc(mycpu[curCPU]->getPC());
  }
  if(startTime) {
    DEBUG_OUT<<"Clock: "<<(unsigned int)(endTime-startTime)<<"\n";
    startTime=0;
  }
  //if(str) drawPrompt();
  cmd.debug_pointer=0;
  //if(myvideo) myvideo->redraw();
  redrawDebugWindow();
  //w->setForeground(NEMU_FOREGROUND_COLOR); w->setBackground(NEMU_BACKGROUND_COLOR);
  //w->focus();
  //if(mycpu[curCPU]) { if(mymap[curMap]) { mycpu[curCPU]->disasmPC(); } }
  //std::cerr<<"Exiting debugger!\n";
}


void NEmu::clearRam()
{
  if(!ram) return;
  unsigned int t,size=ram->getSize();
  for(t=0;t<size;t++) ram->write8(t,0);
}


void NEmu::setCheckOften()
{
  ticksTilCheck=10;
  DEBUG_OUT<<"(check often)\n";
}


void NEmu::setCheckRarely()
{
  // The ticksTilCheck value used to be 640
  ticksTilCheck=320;
  DEBUG_OUT<<"(check rarely)\n";
}


// Must be a valid file with no non-ascii characters in the first 16 bytes
// NOTE: rejects anything over 128, or below 30 not 9, 10, 13 or 14
bool NEmu::isItATextFile(char *fname)
{
  FILE *f=(FILE *)NULL;
  if(useVfs) {
    if(!myvfs) {
      std::cerr<<"No vfs!\n";
      return false;
    }
    DEBUG_OUT<<"isItATextFile going to try to open "<<fname<<" using myvfs\n";
    f=(FILE *)myvfs->Fopen(fname,"r");
  }
  else {
    f=fopen(fname,"r");
    DEBUG_OUT<<"isItATextFile going to try to open "<<fname<<" using fopen\n";
  }
  if(!f) {
    DEBUG_OUT<<"Couldn't open "<<fname<<"\n";
    return false;
  }
  DEBUG_OUT<<"isItATextFile looking at "<<fname<<"\n";
  bool ret=true;
  unsigned int t=16;
  BYTE c;
  while(t) {
    if(!ret) break;
    if(useVfs) myvfs->Fread(&c,1,1,f); else fread(&c,1,1,f);
    if(c<30) {
      switch(c) {
        case 9:  case 10:  case 13:  case 14: break;
        default:
          ret=false;
          std::cerr<<"Found a non-tab/newline/etc character "<<(int)c<<"!\n";
          break;
      }
    }
    if(c>128) ret=false;
    t--;
  }
  if(useVfs) myvfs->Fclose(f); else fclose(f);
  DEBUG_OUT<<"We think its ";
  //if(!ret) DEBUG_OUT<<"not ";
  DEBUG_OUT<<"a text file\n";
  return ret;
}


void NEmu::drawPrompt()
{
  //w->setForeground(NEMU_FOREGROUND_COLOR); w->setBackground(NEMU_BACKGROUND_COLOR);
  DEBUG_OUT<<">";
}


void NEmu::setCPU(unsigned int n)
{
  if(n<NEMU_MAX_CPUS) {
    curCPU=n;
  }
  else std::cerr<<"CPU num too high!\n";
}


void NEmu::setMap(unsigned int n)
{
  if(n<NEMU_MAX_MAPS) {
    curMap=n;
  }
  else std::cerr<<"Map num too high!\n";
}


void NEmu::quit()
{
  //std::cerr<<"NEmu::quit()!\n";
  quitASAP=true;
  exit(5);
}


void NEmu::handleNewIO(char *tname)
{
  if(!mymap[curMap]) { std::cerr<<"No map!\n"; return; }
  if(!tname) { std::cerr<<"handleNewIO, no name!\n"; return; }
  if(mycpu[curCPU]) mycpu[curCPU]->setIO((EHandler *)NULL);
  if(ld) ld->setVfs((DVfs *)NULL);
  if(myio) delete myio;
  myio=(EHandler *)NULL;
  myvideo=(EVideoIOAbstract *)NULL;
  //if(!strcmp("native",tname)) tname=aThisApp.getNativeOSType();
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  //
  if(NAcornHandler::recognize(name)) myio=new NAcornHandler(); else
  if(NAmigaHandler::recognize(name)) myio=new NAmigaHandler(); else
  if(NAppleHandler::recognize(name)) myio=new NAppleHandler(); else
  if(NAtariVectorHandler::recognize(name)) myio=new NAtariVectorHandler(); else
  if(NAtariHandler::recognize(name)) myio=new NAtariHandler(); else
  if(NAtariSTHandler::recognize(name)) myio=new NAtariSTHandler(); else
  if(NAtariArcadeHandler::recognize(name)) myio=new NAtariArcadeHandler(); else
  if(NCbmHandler::recognize(name)) myio=new NCbmHandler(); else
  if(NCocoHandler::recognize(name)) myio=new NCocoHandler(); else
  if(NCpmHandler::recognize(name)) myio=new NCpmHandler(); else
  if(NFamicomHandler::recognize(name)) myio=new NFamicomHandler(); else
  if(NGenesisHandler::recognize(name)) myio=new NGenesisHandler(); else
  if(NJavaHandler::recognize(name)) myio=new NJavaHandler(); else
  if(NMacHandler::recognize(name)) myio=new NMacHandler(); else
  if(NOricHandler::recognize(name)) myio=new NOricHandler(); else
  if(NPalmHandler::recognize(name)) myio=new NPalmHandler(); else
  if(NPcHandler::recognize(name)) myio=new NPcHandler(); else
  if(NTrs80Handler::recognize(name)) myio=new NTrs80Handler(); else
  if(NSwfHandler::recognize(name)) myio=new NSwfHandler(); else
  if(NUnixHandler::recognize(name)) myio=new NUnixHandler(); else
  if(NZMachineHandler::recognize(name)) myio=new NZMachineHandler(); else
  //
  if(!strcmp("none",name)) { return; }
  else {
    DEBUG_OUT<<"'"<<name<<"' Unknown I/O.\n";
  }
  if(myio) {
    if(myio->err.error()) {
      std::cerr<<"Error creating I/O!\n";
      delete myio;
      myio=(EHandler *)NULL;
      myvideo=(EVideoIOAbstract *)NULL;
      return;
    }
    else {
      myio->setMap(mymap[curMap]);
      myio->setKind(name);
      myvideo=myio->getVideo();
      if(mycpu[curCPU]) mycpu[curCPU]->setIO(myio);
      myio->setCPU(mycpu[curCPU]);
      myio->setupVfs();
      myvfs=myio->getVfs();
      if(myvfs) {
        if(ld) ld->setVfs(myvfs);
      }
      else {
        DEBUG_OUT<<"Warning, new IO didn't have it's own vfs, using default.\n";
        myvfs=defaultVfs;
      }
      myvfs->setMap(mymap[curMap]);
      myvfs->setCPU(mycpu[curCPU]);
      mymap[curMap]->setIOHandler(myio);
    }
  }
  //if(w) w->focus();
}


// TODO: Only handles one cpu for now...
void NEmu::handleNewCPU(char *tname)
{
  if(!tname) {
    std::cerr<<"handleNewIO, no name!\n";
    return;
  }
  if(myio) myio->setCPU((ECPUAbstract *)NULL);
  if(mycpu[curCPU]) delete mycpu[curCPU];
  mycpu[curCPU]=(ECPUAbstract *)NULL;
  //if(!strcmp("native",tname)) tname=aThisApp.getNativeCPUType();
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  //
  if(EAlpha::recognize(name)) mycpu[curCPU]=new EAlpha(); else
  if(EArm::recognize(name)) mycpu[curCPU]=new EArm(); else
  if(EDotNet::recognize(name)) mycpu[curCPU]=new EDotNet(); else
  if(EHitachi::recognize(name)) mycpu[curCPU]=new EHitachi(); else
  if(EJvm::recognize(name)) mycpu[curCPU]=new EJvm(); else
  if(EMips::recognize(name)) mycpu[curCPU]=new EMips(); else
  if(Em6502::recognize(name)) mycpu[curCPU]=new Em6502(); else
  if(Em68k::recognize(name)) mycpu[curCPU]=new Em68k(); else
  if(Em6800::recognize(name)) mycpu[curCPU]=new Em6800(); else
  if(Em6809::recognize(name)) mycpu[curCPU]=new Em6809(); else
  if(Em6811::recognize(name)) mycpu[curCPU]=new Em6811(); else
  if(EPowerPC::recognize(name)) mycpu[curCPU]=new EPowerPC(); else
  if(ESh::recognize(name)) mycpu[curCPU]=new ESh(); else
  if(ESparc::recognize(name)) mycpu[curCPU]=new ESparc(); else
  if(ESwf::recognize(name)) mycpu[curCPU]=new ESwf(); else
  if(Ex86::recognize(name)) mycpu[curCPU]=new Ex86(); else
  if(EZMachine::recognize(name)) mycpu[curCPU]=new EZMachine(); else
  if(Ez80::recognize(name)) mycpu[curCPU]=new Ez80(); else
  //
  if(!strcmp("none",name)) { return; }
  else {
    DEBUG_OUT<<"'"<<name<<"' Unknown cpu.\n";
  }
  if(mycpu[curCPU]) {
    if(mycpu[curCPU]->err.error()) {
      std::cerr<<"Error creating cpu!\n";
      delete mycpu[curCPU];
      mycpu[curCPU]=(ECPUAbstract *)NULL;
      return;
    }
    else {
      if(mymap[curMap]) mycpu[curCPU]->setMap(mymap[curMap]);
      mycpu[curCPU]->setKind(name);
      if(myio) {
        myio->setCPU(mycpu[curCPU]);
        mycpu[curCPU]->setIO(myio);
      }
    }
  }
  //if(w) w->focus();
}


void NEmu::ver()
{
  DEBUG_OUT<<(char *)"Nick's Emulator\n";
  DEBUG_OUT<<"NEmu version "<<"whatever"<<"\n";
  DEBUG_OUT<<"\n";
}
