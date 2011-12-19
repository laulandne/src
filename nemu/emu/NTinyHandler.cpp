
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


/*

The name of this file is a bit of a misnomer...its more of a general Unix
system call handler...and in fact, not even just Unix...so maybe a better
name would be "system_call.cpp"...

NOTE: We assume a memory map with 2m of ram mapped at 0 and 2m of ram mapped
at the 2g mark. (What did I want it at the 2g mark for...?)
We'll put the stack at the 2m mark growing down.
We want a page fault for any unmapped memory (not just a no-op like usual)

*/


#include <emu/NTinyHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <cpulib/ECPUAbstract.h>

#include <elib/EMemMapAbstract.h>


#ifdef __MWERKS__
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif // __MWERKS__


#ifdef ALIB_HAVE_IOSTREAM
#include <iostream>
#endif // ALIB_HAVE_IOSTREAM
#include <stdlib.h>

#ifdef ALIB_HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif // ALIB_HAVE_SYS_FCNTL_H

#ifdef ALIB_HAVE_FCNTL_H
#include <fcntl.h>
#endif // ALIB_HAVE_FCNTL_H

#ifdef ALIB_HAVE_UNISTD_H
#include <unistd.h>
#endif // ALIB_HAVE_UNISTD_H

#ifdef ALIB_HAVE_IO_H
#include <io.h>
#endif // ALIB_HAVE_IO_H


////////////////////////////////////////////////////////////////////////////////
//  NTinyHandler Class
////////////////////////////////////////////////////////////////////////////////

NTinyHandler::NTinyHandler(EHandler *parent)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NTinyHandler constructor.\n";
#endif // DEBUG_VERBOSE
  io_type=0;
  init();
  io=parent;
}


NTinyHandler::~NTinyHandler()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NTinyHandler destructor.\n";
#endif // DEBUG_VERBOSE
  //if(!error()) freeIO();
  //err.set();
}


void NTinyHandler::init()
{
  io=NULL;
}


void NTinyHandler::reset()
{
  if(cpu) {
    cpu->setResetStack(resetStack);
    cpu->setStack(resetStack);
    cpu->setFramePtr(resetStack);
    cpu->setResetEntry(resetAddr);
    cpu->jump(resetAddr);
  }
}


bool NTinyHandler::systemCall(unsigned int num)
{
  //long dig=w->getHexDigits();
  //w->setHexDigits(1);
  bool ret=false;
  int retval=0;
  long p0=0,p1=0,p2=0,p3=0,p4=0,p5=0;
  p0=cpu->readParamReg(0);
  p1=cpu->readParamReg(1);
  p2=cpu->readParamReg(2);
  p3=cpu->readParamReg(3);
  p4=cpu->readParamReg(4);
  p5=cpu->readParamReg(5);
  if(num==15) {
    // It's one our built-in emulation calls...
    cpu->setReturnReg(builtinSysCall(p0,p1,p2,p3,p4,p5));
    //w->setHexDigits(dig);
    return true;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NTinyHandler::systemCall("<<num<<") "<<p0<<"("<<p1<<","<<p2<<","<<p3<<","<<p4<<","<<p5<<")\n";
#endif // DEBUG_VERBOSE
  switch(p0) {
    case 45:  // brk
      retval=-1;
      ret=true;
      break;
    case 136: // personality
      retval=-1;
      ret=true;
      break;
    default:
      debugger("systemCall: Not implemented!\n");
      //cpu->wregs();
      ret=false;
      break;
  }
  cpu->setReturnReg(retval);
  //w->setHexDigits(dig);
  return ret;
}


long NTinyHandler::builtinSysCall(unsigned int p0,long p1,long p2, long p3, long p4, long p5)
{
  long ret=0;
  switch(p0) {
    case N_TINY_INIT: ret=tinyInit(); break;
    case N_TINY_DEBUGGER: ret=tinyDebugger(p1,p2); break;
    case N_TINY_DEBUGSTR: ret=tinyDebugString(p1,p2); break;
    case N_TINY_OPEN: ret=tinyOpen(p1,p2); break;
    case N_TINY_CLOSE: ret=tinyClose(p1); break;
    case N_TINY_READ: ret=tinyRead(p1,p2,p3); break;
    case N_TINY_WRITE: ret=tinyWrite(p1,p2,p3); break;
    case N_TINY_SEEK: ret=tinySeek(p1,p2,p3); break;
    case N_TINY_TELL: ret=tinyTell(p1); break;
    case N_TINY_FLUSH: ret=tinyFlush(p1); break;
    case N_TINY_MALLOC: ret=tinyMalloc(p1); break;
    case N_TINY_FREE: ret=tinyFree(p1); break;
    case N_TINY_EXIT: ret=tinyExit(p1); break;
    case N_TINY_INT2STRING: ret=tinyInt2String(p1,p2); break;
    case N_TINY_STRING2INT: ret=tinyString2Int(p1); break;
    case N_TINY_SMUL: ret=tinySmul(p1,p2); break;
    case N_TINY_SDIV: ret=tinySdiv(p1,p2); break;
    case N_TINY_UMUL: ret=tinyUmul(p1,p2); break;
    case N_TINY_UDIV: ret=tinyUdiv(p1,p2); break;
    default:
      DEBUG_OUT<<"builtinSysCall("<<p0<<")\n";
      debugger("That call not implemented!\n");
      break;
  }
  return ret;
}


long NTinyHandler::tinyInit()
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: init()\n";
#endif // DEBUG_VERBOSE
  //debugger("tinyInit() not implemented!");
  return ret;
}


long NTinyHandler::tinyOpen(long p1,long p2)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: open("<<p1<<","<<p2<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: This won't work if the fd needs 32 bits and the cpu isn't 32 bits...
  readFilenameFromAddr(p1);
  long fd=0;
#ifdef ALIB_HAVE_UNIX_IO
#ifdef ALIB_HAVE_UNDERSCORE_OPEN
  fd=_open(tempBuffer,p2);
#else
  fd=open(tempBuffer,p2);
#endif // ALIB_HAVE_UNDERSCORE_OPEN
#else
  // TODO: kludge
  fd=(long)fopen(tempBuffer,"r");
#endif // ALIB_HAVE_UNIX_IO
  ret=translateFileHandleTo(fd);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"tiny open(\""<<tempBuffer<<"\","<<p2<<"...\n";
#endif // DEBUG_VERBOSE
  return ret;
}


long NTinyHandler::tinyClose(long p1)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: close("<<p1<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: This won't work if the fd needs 32 bits and the cpu isn't 32 bits...
  long fd=translateFileHandleFrom(p1);
#ifdef ALIB_HAVE_UNIX_IO
#ifdef ALIB_HAVE_UNDERSCORE_CLOSE
  ret=_close(fd);
#else
  ret=close(fd);
#endif // ALIB_HAVE_UNDERSCORE_CLOSE
#else
  // TODO: kludge
  ret=fclose((FILE *)fd);
#endif // ALIB_HAVE_UNIX_IO
  return ret;
}


long NTinyHandler::tinyRead(long p1,long p2,long p3)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: read("<<p1<<","<<p2<<","<<p3<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: This won't work if the fd needs 32 bits and the cpu isn't 32 bits...
  switch(p1) {
    case NTINY_STDOUT:
      debugger("tinyRead, attempt to read from STDOUT!\n");
      break;
    case NTINY_STDERR:
    case NTINY_STDIN:
      debugger("tinyRead, STDIN not implemented!\n");
      break;
    default:
      long len=limitToBufferSize(p3);
      long fd=translateFileHandleFrom(p1);
#ifdef ALIB_HAVE_UNIX_IO
#ifdef ALIB_HAVE_UNDERSCORE_READ
      ret=_read(fd,tempBuffer,len);
#else
      ret=read(fd,tempBuffer,len);
#endif // ALIB_HAVE_UNDERSCORE_READ
#else
      // TODO: kludge
      ret=fread(tempBuffer,len,1,(FILE *)fd);
#endif // ALIB_HAVE_UNIX_IO
      writeMemToAddr(p2,len);
      break;
  }
  return ret;
}


long NTinyHandler::tinyWrite(long p1,long p2,long p3)
{
  long ret=0;
  long t=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: write("<<p1<<","<<p2<<","<<p3<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: This won't work if the fd needs 32 bits and the cpu isn't 32 bits...
  switch(p1) {
    case NTINY_STDIN:
      debugger("tinyWrite, attempt to write to STDIN!\n");
      break;
    case NTINY_STDOUT:
      for(t=0;t<p3;t++) {
        DEBUG_OUT<<(char)io->getMap()->read8(p2+t);
        //if(aDefaultStringHelper.hasConsole) aStdOutStream<<(char)io->getMap()->read8(p2+t);
        //else DEBUG_OUT<<(char)io->getMap()->read8(p2+t);
      }
      break;
    case NTINY_STDERR:
      for(t=0;t<p3;t++) {
        DEBUG_OUT<<(char)io->getMap()->read8(p2+t);
        //if(aDefaultStringHelper.hasConsole) aStdErrStream<<(char)io->getMap()->read8(p2+t);
        //else DEBUG_OUT<<(char)io->getMap()->read8(p2+t);
      }
      break;
    default:
      DEBUG_OUT<<(char)io->getMap()->read8(p2+t);
      long len=limitToBufferSize(p3);
      readMemFromAddr(p2,len);
      long fd=translateFileHandleFrom(p1);
#ifdef ALIB_HAVE_UNIX_IO
#ifdef ALIB_HAVE_UNDERSCORE_WRITE
      ret=_write(fd,tempBuffer,len);
#else
      ret=write(fd,tempBuffer,len);
#endif // ALIB_HAVE_UNDERSCORE_WRITE
#else
      // TODO: kludge
      ret=fwrite(tempBuffer,len,1,(FILE *)fd);
#endif // ALIB_HAVE_UNIX_IO
      break;
  }
  return ret;
}


long NTinyHandler::tinySeek(long p1,long p2,long p3)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: seek("<<p1<<","<<p2<<","<<p3<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: This won't work if the fd needs 32 bits and the cpu isn't 32 bits...
  switch(p1) {
    case NTINY_STDIN:
      debugger("tinySeek(STDIN,,) not implemented!");
      break;
    case NTINY_STDERR:
      debugger("tinySeek(STDERR,,) not implemented!");
      break;
    case NTINY_STDOUT:
      debugger("tinySeek(STDOUT,,) not implemented!");
      break;
      break;
    default:
      long fd=translateFileHandleFrom(p1);
#ifdef ALIB_HAVE_UNIX_IO
#ifdef ALIB_HAVE_UNDERSCORE_LSEEK
      ret=_lseek(fd,p2,p3);
#else
      ret=lseek(fd,p2,p3);
#endif // ALIB_HAVE_UNDERSCORE_LSEEK
#else
      // TODO: kludge
      ret=fseek((FILE *)fd,p2,p3);
#endif // ALIB_HAVE_UNIX_IO
      break;
  }
  return ret;
}


long NTinyHandler::tinyTell(long p1)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: tell("<<p1<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: This won't work if the fd needs 32 bits and the cpu isn't 32 bits...
  switch(p1) {
    case NTINY_STDIN:
      ret=0;
      break;
    case NTINY_STDERR:
    case NTINY_STDOUT:
      ret=0;
      break;
    default:
      long fd=translateFileHandleFrom(p1);
      bool done=false;
#ifdef ALIB_HAVE_UNDERSCORE_TELL
      ret=_tell(fd);
      done=true;
#endif // ALIB_HAVE_UNDERSCORE_TELL
#ifdef ALIB_HAVE_TELL
      ret=tell(fd);
      done=true;
#endif // ALIB_HAVE_TELL
      // TODO: kludge
      if(!done) ret=ftell((FILE *)fd);
      break;
  }
  return ret;
}


long NTinyHandler::tinyFlush(long p1)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: flush("<<p1<<")\n";
#endif // DEBUG_VERBOSE
  //debugger("tinyFlush() not implemented!");
  return ret;
}


long NTinyHandler::tinyMalloc(long p1)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: malloc("<<p1<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: braindead heap management...we just push the heap pointer
  // up each time...see tinyFree...
  ret=heapCur;
  heapCur+=p1;
  if(heapCur>heapTop) debugger("Ran out of heap!\n");
  return ret;
}


long NTinyHandler::tinyFree(long p1)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: free("<<p1<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: we don't ever actually do anything
  return ret;
}


long NTinyHandler::tinyExit(long p1)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: exit("<<p1<<")\n";
#endif // DEBUG_VERBOSE
  // TODO: close all files
  // TODO: free mem
  ret=p1;
  //if(p1) debugger("Program exited with error\n");
  //else debugger("Program exited normally.\n");
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ntiny going to call exit() on behalf of program...\n";
#endif // DEBUG_VERBOSE
  // TODO: Shouldn't we warn nemu with a quitASAP flag?
  exit(0);
  return ret;
}


long NTinyHandler::tinyInt2String(long p1,long p2)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: int2string("<<p1<<","<<p2<<")\n";
#endif // DEBUG_VERBOSE
  // NOTE: p1 is int n, p2 is char *s
  char buffer[256];
  sprintf(buffer,"%d",p1);
  int l=strlen(buffer);
  for(int t=0;t<l;t++) io->getMap()->write8(p2+t,buffer[t]);
  ret=l;
  return ret;
}


long NTinyHandler::tinyString2Int(long p1)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: string2int("<<p1<<")\n";
#endif // DEBUG_VERBOSE
  debugger("tinyString2Int() not implemented!");
  return ret;
}


long NTinyHandler::tinyDebugger(long p1,long p2)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: debugger("<<p1<<","<<p2<<")\n";
#endif // DEBUG_VERBOSE
  // NOTE: p1 is command (ignored), p2 is string
  readStringFromAddr(p2);
  debugger(tempBuffer);
  return ret;
}


long NTinyHandler::tinyDebugString(long p1,long p2)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: debugString("<<p1<<","<<p2<<")\n";
#endif // DEBUG_VERBOSE
  // NOTE: p1 is command (ignored), p2 is string
  readStringFromAddr(p2);
  DEBUG_OUT<<tempBuffer<<"\n";
  return ret;
}


long NTinyHandler::tinySmul(long p1,long p2)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: smul("<<p1<<","<<p2<<")\n";
#endif // DEBUG_VERBOSE
  //debugger("tinySmul() not implemented!");
  ret=p1*p2;
  return ret;
}


long NTinyHandler::tinyUmul(long p1,long p2)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: umul("<<p1<<","<<p2<<")\n";
#endif // DEBUG_VERBOSE
  //debugger("tinyUmul() not implemented!");
  ret=p1*p2;
  return ret;
}


long NTinyHandler::tinySdiv(long p1,long p2)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: sdiv("<<p1<<","<<p2<<")\n";
#endif // DEBUG_VERBOSE
  debugger("tinySdiv() not implemented!");
  return ret;
}


long NTinyHandler::tinyUdiv(long p1,long p2)
{
  long ret=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"builtinSysCall: udiv("<<p1<<","<<p2<<")\n";
#endif // DEBUG_VERBOSE
  debugger("tinyUdiv() not implemented!");
  return ret;
}


void NTinyHandler::readMemFromAddr(long p1,long size)
{
  long t;
  for(t=0;t<ATEMPSTRINGBUF_SIZE;t++) tempBuffer[t]=0;
  long len=limitToBufferSize(size);
  for(t=0;t<len;t++) {
    char c=io->getMap()->read8(p1+t);
    tempBuffer[t]=c;
  }
}


void NTinyHandler::readFilenameFromAddr(long p1)
{
  readStringFromAddr(p1);
  char *tmp=strdup(tempBuffer);
  tempBuffer[0]=0;
  aDefaultStringHelper.fillInString(tempBuffer,NTINY_FS_PATH);
  aDefaultStringHelper.fillInString(tempBuffer,tmp);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"readFilenameFromAddr:"<<tmp<<"->"<<tempBuffer<<"\n";
#endif // DEBUG_VERBOSE
  free(tmp);
}


void NTinyHandler::readStringFromAddr(long p1)
{
  long t;
  for(t=0;t<ATEMPSTRINGBUF_SIZE;t++) tempBuffer[t]=0;
  bool reading=true;
  t=0;
  while(reading) {
    char c=io->getMap()->read8(p1+t);
    if(c) {
      tempBuffer[t]=c;
      t++;
    }
    else reading=false;
    if(t==ATEMPSTRINGBUF_SIZE) {
      debugger("readMemFromAddr, too big for ATEMPSTRINGBUF_SIZE!\n");
      reading=false;
    }
  }
  tempBuffer[ATEMPSTRINGBUF_SIZE-1]=0;
}


void NTinyHandler::writeMemToAddr(long p1,long size)
{
  long len=limitToBufferSize(size);
  for(long t=0;t<len;t++) {
    char c=tempBuffer[t];
    io->getMap()->write8(p1+t,c);
  }
}


void NTinyHandler::writeStringToAddr(long p1)
{
  long len=strlen(tempBuffer);
  writeMemToAddr(p1,len);
  // TODO: NULL terminate?
}


long NTinyHandler::limitToBufferSize(long suggest)
{
  long actual=suggest;
  if(actual>ATEMPSTRINGBUF_SIZE) {
    actual=ATEMPSTRINGBUF_SIZE;
    DEBUG_OUT<<"limitToBufferSize, want "<<suggest<<">"<<actual<<"!\n";
    debugger("");
  }
  return actual;
}


long NTinyHandler::translateFileHandleTo(long p1)
{
  // TODO: This won't work if the fd needs 32 bits and the cpu isn't 32 bits...
  return p1;
}


long NTinyHandler::translateFileHandleFrom(long p1)
{
  // TODO: This won't work if the fd needs 32 bits and the cpu isn't 32 bits...
  return p1;
}


