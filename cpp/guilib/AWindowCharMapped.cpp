
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/AWindowCharMapped.h>

#include <portable/nickcpp/ANullStream.h>


#ifdef ASYS_X11_BASED
#include <X11/Xutil.h>
#endif // ASYS_X11_BASED

#ifdef ASYS_DOS
#include <guilib/alib_dos.h>
#endif // ASYS_DOS

#ifdef ASYS_WINDOWS
#include <guilib/alib_win.h>
#endif // ASYS_DOS

#ifdef ASYS_AMIGA
#include <guilib/alib_amiga.h>
#include <exec/libraries.h>
#include <intuition/intuition.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>
#include <clib/exec_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/wb_protos.h>
#endif // ASYS_AMIGA

#ifdef ASYS_MAC
#ifdef ALIB_NO_PCH
//#include <Windows.h>
//#include <Fonts.h>
#include <Carbon/Carbon.h>
#endif // ALIB_NO_PCH
#include <guilib/alib_carbon.h>
#endif // ASYS_MAC

#ifdef ASYS_SDL
#include <SDL.h>
#endif // ASYS_SDL


#ifdef ASYS_OS2
MRESULT EXPENTRY windowFunc(HWND handle, ULONG mess,
  MPARAM parm1, MPARAM parm2);
#endif // ASYS_OS2

#ifdef ASYS_AMIGA
extern Library *WorkbenchBase;
#endif // ASYS_AMIGA


//#define DEBUG_WINDOW_RESIZES_MOVES 1
#define PEDANTIC_WINDOW_CRAP 1


////////////////////////////////////////////////////////////////////////////////
//  AWindowCharMapped Class
////////////////////////////////////////////////////////////////////////////////

AWindowCharMapped::AWindowCharMapped(char *newName,unsigned int newWidth,unsigned int newHeight)
{
  // Nothing to do, parent class has done it (including init'd unused charmap)
}


void AWindowCharMapped::setCharMapping(bool t)
{
  charMapped=t;
  if(charMapped) {
    newCharMap();
  }
  else {
    freeCharMap();
  }
}


void AWindowCharMapped::freeCharMap()
{
  if(!charMapped) return;
  if(charMap) free(charMap);
  charMap=(char *)NULL;
  charMapped=false;
  mapSize=0;
}


void AWindowCharMapped::newCharMap()
{
  //if(!charMapped) return;
  if(charMap) freeCharMap();
  calcCharMapSize();
  mapSize=mapCols*mapRows;  // NOTE: in char's (not bytes)
  DEBUG_OUT<<"ncm: mapSize is "<<mapSize<<" characters.\n";
  if(mapSize) {
    charMap=(char *)malloc(mapSize*sizeof(char));
    clearCharMap();
  }
  else //DEBUG_OUT.warning("ncm: mapSize was zero!\n");
  charMapped=true;
  DEBUG_OUT<<"(Got charmap)\n";
}


void AWindowCharMapped::drawCharMap()
{
  if(!charMapped) return;
  if(!charMap) { //DEBUG_OUT.warning("dcm: no charMap!\n");
    return; }
  if(!mapSize) { //DEBUG_OUT.warning("dcm: mapSize is zero!\n");
    return; }
#ifdef ASYS_POSIX
  return;
#endif // ASYS_POSIX
  //DEBUG_OUT<<"dcm\n";
  unsigned int oldX=penX,oldY=penY;
  unsigned int offset=0;
  for(unsigned int yy=0;yy<mapRows;yy++) {
    for(unsigned int xx=0;xx<mapCols;xx++) {
      moveTo(xx*avgCharWidth,yy*fontHeight);
      offset=yy*mapCols+xx;
      if(offset<mapSize) {
        drawText(&charMap[offset],1,false);
        //DEBUG_OUT<<charMap[offset]<<" offset is "<<offset<<"\n";
      }
    }
  }
  moveTo(oldX,oldY);
}


void AWindowCharMapped::clearCharMap()
{
  if(!charMapped) return;
  if(!charMap) { //DEBUG_OUT.warning("ccm: no charMap!\n");
    return; }
  if(!mapSize) { //DEBUG_OUT.warning("ccm: mapSize is zero!\n");
    return; }
  //DEBUG_OUT<<"ccm\n";
  for(unsigned int t=0;t<mapSize;t++) charMap[t]=(char)' ';
}


void AWindowCharMapped::calcCharMapSize()
{
  if(!avgCharWidth) avgCharWidth=fontHeight;
  mapCols=(width/avgCharWidth);  // FIXME: Isn't right for prop fonts
  mapRows=(height/fontHeight);
  DEBUG_OUT<<"AWindow::calcCharMapSize: "<<mapCols<<"x"<<mapRows;
  DEBUG_OUT<<" font: "<<avgCharWidth<<"x"<<fontHeight<<"\n";
}


void AWindowCharMapped::resizeCharMap()
{
  //DEBUG_OUT<<"rcm\n";
  // save old map, and sizes
  char *oldMap=charMap;
  unsigned oldCols=mapCols,oldRows=mapRows;
  unsigned int oldSize=mapSize;
  charMap=(char *)NULL;  // to fool newCharMap...
  // create new map matching new size...it will be empty initially
  newCharMap();
  // now figure how much of the old map we need to copy
  unsigned int smallerCols=oldCols,smallerRows=oldRows;
  if(mapCols<smallerCols) smallerCols=mapCols;
  if(mapRows<smallerRows) smallerRows=mapRows;
  // now copy from the old to the new
  unsigned int soffset,doffset;
  for(unsigned int yy=0;yy<smallerRows;yy++) {
    for(unsigned int xx=0;xx<smallerCols;xx++) {
      doffset=yy*mapCols+xx;
      soffset=yy*oldCols+xx;
      if((doffset<mapSize)&&(soffset<oldSize)) charMap[doffset]=oldMap[soffset];
    }
  }
  // throw out the old map since we no longer need it
  free(oldMap);
  oldMap=(char *)NULL;
  // finally draw the results
  drawCharMap();
}
