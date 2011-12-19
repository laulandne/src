
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include "APalette.h"

#include <stdlib.h>
#include <iostream>


static unsigned int my_dist(unsigned int a,unsigned int b)
{
  if(a<b) return b-a; else return a-b;
}


////////////////////////////////////////////////////////////////////////////////
//  APalette Class
////////////////////////////////////////////////////////////////////////////////

APalette::APalette(unsigned int num,unsigned int bitsNum)
{
  init();
  // If we're requested to calculate the number of colors from
  // bits per pixel, do so.
  if(!num) {
    num=1;
    for(unsigned int t=0;t<bitsNum;t++) num*=2;
  }
  // Always have at least 2 colors.
  if(num<2) num=2;
  numColors=num;
#ifdef DEBUG_VERBOSE
  dBug<<"Palette cons("<<num<<")\n";
#endif // DEBUG_VERBOSE
  if(numColors>512) {
    //AMsgWindow("TrueColor Palette!","Palette with more than 8 bits created!");
    tooBig=true;
  }
  allocParts();
  // Set default to default colors
  setToDefaultRGB();
  //setToDefaultGrayscale();
}


// FIXME: Make sure this works...I didn't check it
APalette::APalette(APalette *orig)
{
  init();
  if(!orig) { /*errorFlag=true;*/ return; }
  //if(orig->errorFlag) { errorFlag=true; return; }
  histComputed=orig->histComputed;
  tooBig=orig->tooBig;
  numColors=orig->numColors;
  significantBits=orig->significantBits;
  bitsHint=orig->bitsHint;  hBrite=orig->hBrite;
  changed=orig->changed;  blackBackground=orig->blackBackground;
#ifdef DEBUG_VERBOSE
  dBug<<"Palette cons(APalette)\n";
#endif // DEBUG_VERBOSE
  allocParts();
  for(unsigned int t=0;t<numColors;t++) {
    r[t]=orig->r[t];
    g[t]=orig->g[t];
    b[t]=orig->b[t];
    used[t]=orig->used[t];
  }
}


void APalette::init()
{
#ifdef ASYS_MFC
  setClassName("APalette");
#endif // ASYS_MFC
  //errorFlag=false;
  histComputed=false;
  tooBig=false;
  defaultRGB=false;
  readOnly=false;
  numColors=0;
  significantBits=24;  // Total, r+g+b...8 for each...
  bitsMask=0xff;
  bitsHint=0;  hBrite=false;
  changed=false;  blackBackground=false;
  r=(unsigned int *)NULL;  g=(unsigned int *)NULL;  b=(unsigned int *)NULL;
  used=(bool *)NULL;  score=(long *)NULL;  map=(int *)NULL;
  mapSigBits=0;
}


void APalette::allocParts()
{
  if(r) {
#ifdef DEBUG_VERBOSE
    dBug<<"Couldn't allocParts, already there!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  if(g) {
#ifdef DEBUG_VERBOSE
    dBug<<"Couldn't allocParts, already there!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  if(b) {
#ifdef DEBUG_VERBOSE
    dBug<<"Couldn't allocParts, already there!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  if(used) {
#ifdef DEBUG_VERBOSE
    dBug<<"Couldn't allocParts, already there!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  if(tooBig) {
#ifdef DEBUG_VERBOSE
    dBug<<"allocParts: Too many colors, palette too big!!!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  r=(unsigned int *)malloc(numColors*sizeof(unsigned int));
  if(!r) {
#ifdef DEBUG_VERBOSE
    dBug<<"No memory for red of palette!\n";
#endif // DEBUG_VERBOSE
    //errorFlag=true;
    return;
  }
  g=(unsigned int *)malloc(numColors*sizeof(unsigned int));
  if(!g) {
#ifdef DEBUG_VERBOSE
    dBug<<"No memory for green of palette!\n";
#endif // DEBUG_VERBOSE
    //errorFlag=true;
    return;
  }
  b=(unsigned int *)malloc(numColors*sizeof(unsigned int));
  if(!b) {
#ifdef DEBUG_VERBOSE
    dBug<<"No memory for blue of palette!\n";
#endif // DEBUG_VERBOSE
    //errorFlag=true;
    return;
  }
  used=(bool *)malloc(numColors*sizeof(bool));
  if(!used) {
#ifdef DEBUG_VERBOSE
    dBug<<"No memory for used of palette!\n";
#endif // DEBUG_VERBOSE
    //errorFlag=true;
    return;
  }
}


APalette::~APalette()
{
#ifdef DEBUG_VERBOSE
  dBug<<"Palette des\n";
#endif // DEBUG_VERBOSE
  /*if(!error())*/ freeParts();
  //errorFlag=true;
}


void APalette::freeParts()
{
  if(score) free(score);
  score=(long *)NULL;
  if(map) free(map);
  map=(int *)NULL;
  if(r) free(r);
  r=(unsigned int *)NULL;
  if(g) free(g);
  g=(unsigned int *)NULL;
  if(b) free(b);
  b=(unsigned int *)NULL;
  if(used) free(used);
  used=(bool *)NULL;
}


// FIXME: this is a hack, the colors (grays) selected won't be quite right...(too light?)
void APalette::setToDefaultGrayscale()
{
  if(readOnly) return;
#ifdef DEBUG_VERBOSE
  dBug<<"APalette::setToDefaultGrayscale()...\n";
#endif // DEBUG_VERBOSE
  /*
  if(error()) {
#ifdef DEBUG_VERBOSE
    dBug<<"Palette error, couldn't defGray!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  */
  if(tooBig) return;
  unsigned int factor=256/numColors;
#ifdef DEBUG_VERBOSE
  dBug<<"(factor is "<<factor<<")\n";
#endif // DEBUG_VERBOSE
  if(!factor) return;
  for(unsigned int t=0;t<numColors;t++) {
    r[t]=t*factor;
    g[t]=t*factor;
    b[t]=t*factor;
    used[t]=false;
  }
  setFirstAndLastColors();
  touch();
}


// FIXME: this is a hack, the colors selected won't be quite right...(too light?)
void APalette::setToDefaultRGB()
{
  if(readOnly) return;
#ifdef DEBUG_VERBOSE
  dBug<<"APalette::setToDefaultRGB()...\n";
#endif // DEBUG_VERBOSE
/*
  if(error()) {
#ifdef DEBUG_VERBOSE
    dBug<<"Palette error, couldn't defRGB!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  */
  if(tooBig) return;
  unsigned int factor=0x100/numColors;
#ifdef DEBUG_VERBOSE
  dBug<<"(factor is "<<factor<<")\n";
#endif // DEBUG_VERBOSE
  if(!factor) return;
  for(unsigned int t=0;t<numColors;t++) {
    //DEBUG_OUT<<t<<" : "<<t*factor<<" = ";
    r[t]=getRGBRed9(t*factor)<<5;
    g[t]=getRGBGreen9(t*factor)<<5;
    b[t]=getRGBBlue9(t*factor)<<5;
    //DEBUG_OUT<<r[t]<<","<<g[t]<<","<<b[t]<<"\n";
    used[t]=false;
  }
  setFirstAndLastColors();
  touch();
  defaultRGB=true;
}


void APalette::setFirstAndLastColors()
{
  if(readOnly) return;
  // Set first and last to black and white (take your pick which)
  // NOTE: also sets color #1 so colors #0 and #1 contrast...
  unsigned int last=numColors-1;
  bool blackOnWhite=true;
  // change blackOnWhite here if you like...
  if(blackOnWhite) {
    r[0]=0; g[0]=0; b[0]=0;
    r[1]=0xff; g[1]=0xff; b[1]=0xff;
    r[last]=0xff; g[last]=0xff; b[last]=0xff;
  }
  else {
    r[last]=0; g[last]=0; b[last]=0;
    r[1]=0; g[1]=0; b[1]=0;
    r[0]=0xff; g[0]=0xff; b[0]=0xff;
  }
}


// FIXME: Assumes an even number of colors!
void APalette::reverse()
{
  if(readOnly) return;
  /*
  if(error()) {
#ifdef DEBUG_VERBOSE
    dBug<<"Palette error, couldn't reverse!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  */
  if(tooBig) return;
  unsigned int csize=numColors/2,temp;
  bool tval;
  if((csize*2)!=numColors) {
#ifdef DEBUG_VERBOSE
    dBug<<"Not an even number of colors! Can't reverse!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  for(unsigned int t=0;t<csize;t++) {
    temp=r[t]; r[t]=r[numColors-t]; r[numColors-t]=temp;
    temp=g[t]; g[t]=g[numColors-t]; g[numColors-t]=temp;
    temp=b[t]; b[t]=b[numColors-t]; b[numColors-t]=temp;
    temp=used[t];
    used[t]=used[(unsigned int)numColors-t];
    tval=false;
    if(temp) tval=true;
    used[(unsigned int)numColors-t]=tval;
  }
  touch();
}


void APalette::resize(unsigned int newSize)
{
  if(readOnly) return;
  /*
  if(error()) {
#ifdef DEBUG_VERBOSE
    dBug<<"Palette error, couldn't resize!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  */
  if(newSize==numColors) return;  // Already that size...
  bool e=isEmpty();
  APalette *p=new APalette(numColors);
  if(!p) {
#ifdef DEBUG_VERBOSE
    dBug<<"resize:Couldn't get temp palette!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  /*
  if(p->error()) {
#ifdef DEBUG_VERBOSE
    dBug<<"temp copy palette error!\n"; delete p; return;
#endif // DEBUG_VERBOSE
  }
  */
  p->copy(this);
  freeParts();
  numColors=newSize;
  allocParts();
  copy(p);
  if(e) {
    setToDefaultRGB();
    //setToDefaultGrayscale();
  }
  delete p;
  touch();
}


// NOTE: This only copies colors that are marked as "used"
void APalette::copy(APalette *p)
{
  if(readOnly) return;
  /*
  if(error()) {
#ifdef DEBUG_VERBOSE
    dBug<<"Palette error, couldn't copy!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  */
  if(!p) {
#ifdef DEBUG_VERBOSE
    dBug<<"NULL palette in copy!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  /*
  if(p->error()) {
#ifdef DEBUG_VERBOSE
    dBug<<"Palette error, couldn't copy!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  */
  if(tooBig) return;
  if(p->tooBig) return;
  unsigned int copyNum=p->numColors;
  if(numColors<p->numColors) copyNum=numColors;
  for(unsigned int t=0;t<copyNum;t++) {
    if(p->used[t]) {
      r[t]=p->r[t]; g[t]=p->g[t]; b[t]=p->b[t]; used[t]=true;
    }
  }
  touch();
}


bool APalette::isEmpty()
{
/*
  if(error()) {
#ifdef DEBUG_VERBOSE
    dBug<<"palette error in isEmpty\n";
#endif // DEBUG_VERBOSE
    return true;
  }
  */
  if(tooBig) return true;
  bool ret=true;
  for(unsigned int t=0;t<numColors;t++) if(used[t]) ret=false;
  return ret;
}


unsigned long APalette::findBestColor(unsigned long triplet)
{
  return findBestColor(getRGBRed24(triplet),getRGBGreen24(triplet),getRGBBlue24(triplet));
}


// WARN: There is a bug in here that will prevent it from correctly finding
// the very first and/or the very last color in some cases...
unsigned long APalette::findBestColor(unsigned int red, unsigned int green,
  unsigned int blue)
{
  // FIXME: use map when we have it
  //if(error()) return 0;
  if(tooBig) return 0;
  unsigned int d,best=0,bestD=0;
  unsigned int r2,g2,b2;
  red&=bitsMask;  green&=bitsMask;  blue&=bitsMask;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Looking for "<<red<<" "<<green<<" "<<blue<<"...\n";
#endif // DEBUG_VERBOSE
  // "Prime the pump..."
  r2=r[0]; g2=g[0]; b2=b[0];
  bestD=my_dist(red,r2)*my_dist(red,r2)+my_dist(green,g2)*my_dist(green,g2)+my_dist(blue,b2)*my_dist(blue,b2);
  // Look at all the others...
  for(unsigned int n=0;n<numColors;n++) {
    r2=r[n]; g2=g[n]; b2=b[n];
    d=my_dist(red,r2)*my_dist(red,r2)+my_dist(green,g2)*my_dist(green,g2)+my_dist(blue,b2)*my_dist(blue,b2);
    //printColor(n);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Dist is "<<d<<"\n";
#endif // DEBUG_VERBOSE
    if(d<bestD) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"That's the best so far...\n";
#endif // DEBUG_VERBOSE
      bestD=d; best=n;
    }
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Found "<<r[best]<<" "<<g[best]<<" "<<b[best]<<"...\n";
#endif // DEBUG_VERBOSE
  return best;
}


void APalette::makeMap()
{
  //if(error()) return;
  if(!map) {
#ifdef DEBUG_VERBOSE
    dBug<<"No map!  Can't map!\n";
#endif // DEBUG_VERBOSE
    return;
  }
#ifdef DEBUG_VERBOSE
  dBug<<"Filling in identity map.\n";
#endif // DEBUG_VERBOSE
  // Identity map...
  for(unsigned int t=0;t<numColors;t++) map[t]=t;
}


void APalette::printMap()
{
  //if(error()) return;
  if(!map) {
#ifdef DEBUG_VERBOSE
    dBug<<"No map!  Can't print it!\n";
#endif // DEBUG_VERBOSE
    return;
  }
#ifdef DEBUG_VERBOSE
  dBug<<"Map: ";
  for(unsigned int o=0;o<getNumColors();o++) dBug<<map[o]<<" ";
  dBug<<"\n";
#endif // DEBUG_VERBOSE
}


/*
void APalette::printColor(unsigned int t)
{
  static char rh[64],gh[64],bh[64];
  //if(error()) return;
  strcpy(rh,aThisApp.myHex((unsigned long)r[t]));
  strcpy(gh,aThisApp.myHex((unsigned long)g[t]));
  strcpy(bh,aThisApp.myHex((unsigned long)b[t]));
#ifdef DEBUG_VERBOSE
  dBug<<t<<" "<<rh<<" "<<gh<<" "<<bh<<" "<<"\n";
#endif // DEBUG_VERBOSE
}
*/


void APalette::createSimpleMap(APalette *org)
{
  //if(error()) return;
  if(tooBig) return;
  if(!org) {
#ifdef DEBUG_VERBOSE
    dBug<<"NULL palette in remap!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  //if(org->error()) return;
  if(org->tooBig) return;
  if(getNumColors()!=org->getNumColors()) {
#ifdef DEBUG_VERBOSE
    dBug<<"Can't createSimpleMap, palettes aren't same size...\n";
#endif // DEBUG_VERBOSE
    return;
  }
  for(unsigned int o=0;o<getNumColors();o++) {
    map[o]=(int)org->findBestColor(r[o],g[o],b[o]);
    //printColor(o);
    //org->printColor(map[o]);
  }
  //printMap();
}


void APalette::remap(APalette *org)
{
  if(readOnly) return;
  //if(error()) return;
  if(tooBig) return;
  if(!org) {
#ifdef DEBUG_VERBOSE
    dBug<<"NULL palette in remap!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  //if(org->error()) return;
  if(org->tooBig) return;
  unsigned int dSize=numColors,sSize=org->numColors;
  if(isHBrite()) dSize/=2;
  if(sSize<=dSize) {
    if(sSize!=dSize) {
      // Source is smaller than dest, just copy and leave extra colors alone
#ifdef DEBUG_VERBOSE
      dBug<<"remap: src ("<<sSize<<") is smaller than dest ("<<dSize<<")...\n";
#endif // DEBUG_VERBOSE
    }
    unsigned int t;
    for(t=0;t<dSize;t++) used[t]=false;
    for(t=0;t<sSize;t++) {
      r[t]=org->r[t];  g[t]=org->g[t];  b[t]=org->b[t];  used[t]=true;
      if(score&&org->score) score[t]=org->score[t];
    }
  }
  if(sSize>dSize) {
    if(sSize!=dSize) {
      // Source is larger than dest, we need to remap using histogram...
#ifdef DEBUG_VERBOSE
      dBug<<"remap: dest ("<<dSize<<") is smaller than src ("<<sSize<<")...\n";
#endif // DEBUG_VERBOSE
    }
    if(!org->histogramComputed()) {
#ifdef DEBUG_VERBOSE
      dBug<<"Source doesn't have histogram!\n";
#endif // DEBUG_VERBOSE
      return;
    }
    // We want the first dSize colors with the highest score values...
    // NOTE: This destroys org's histogram which is marked as "not done"
    unsigned int p;
    long best;
    for(unsigned int t=0;t<sSize;t++) org->map[t]=-1;
#ifdef DEBUG_VERBOSE
    dBug<<"mapping first dSize popular colors...\n";
#endif // DEBUG_VERBOSE
    for(unsigned int d=0;d<dSize;d++) {
      best=0;  p=0;
      for(unsigned int s=0;s<sSize;s++) {
        if(org->score[s]>best) { p=s;  best=org->score[p]; }
      }
      if(score) score[d]=org->score[p];
      org->score[p]=0;  // Mark this color as "used"
      org->map[p]=d;
      r[d]=org->r[p];  g[d]=org->g[p];  b[d]=org->b[p];
      used[d]=true;
      if(isHBrite()) {
        r[d+32]=org->r[p]/2;  g[d+32]=org->g[p]/2;  b[d+32]=org->b[p]/2;
        used[d+32]=true;
      }
    }
#ifdef DEBUG_VERBOSE
    dBug<<"remapping unpopular colors...\n";
#endif // DEBUG_VERBOSE
    for(unsigned int o=0;o<sSize;o++) {
      if(org->map[o]== -1)
        org->map[o]=(int)findBestColor(org->r[o],org->g[o],org->b[o]);
    }
    org->histComputed=false;
  }
  org->printMap();
  // Now, I think since we copy the scores from org that this is true...
  // ...but I'm not sure...
  touch();
  histComputed=true;
}


void APalette::finishHistogram()
{
  //if(error()) return;
  if(!score) {
#ifdef DEBUG_VERBOSE
    dBug<<"Don't have a score!\n";
#endif // DEBUG_VERBOSE
    return;
  }
#ifdef DEBUG_VERBOSE
  dBug<<"Finishing up Histogram...\n";
#endif // DEBUG_VERBOSE
  int *a=(int *)malloc(sizeof(int)*numColors);
  if(!a) {
#ifdef DEBUG_VERBOSE
    dBug<<"Couldn't alloc temp score array!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  for(unsigned int t=0;t<numColors;t++) a[t]=0;
#ifdef DEBUG_VERBOSE
  dBug<<"Removing duplicate colors...\n";
#endif // DEBUG_VERBOSE
  if(!bitsHint) bitsHint=significantBits/3;
  // We only handle up to 24 bit color for now (not 32 or 48, etc)
  if(bitsHint>8) bitsHint=8;
  mapSigBits=bitsHint*3;
  unsigned int shift=8-bitsHint,mask=0xff;
  for(unsigned int qq=0;qq<shift;qq++) mask-=(1<<qq);
#ifdef DEBUG_VERBOSE
  dBug<<"(Mask is "<<mask<<", and shift is "<<shift<<")\n";
#endif // DEBUG_VERBOSE
  int tr,tg,tb,xr,xg,xb;
  for(unsigned int ds=0;ds<numColors;ds++) {
    tr=(r[ds]&mask)>>shift;  tg=(g[ds]&mask)>>shift;  tb=(b[ds]&mask)>>shift;
    for(unsigned int xs=0;xs<numColors;xs++) {
      // Same for these...
      xr=(r[xs]&mask)>>shift;  xg=(g[xs]&mask)>>shift;  xb=(b[xs]&mask)>>shift;
      if((ds!=xs)&&score[ds]&&score[xs]&&(tr==xr)&&(tg==xg)&&(tb==xb)) {
        score[ds]+=score[xs];  score[xs]=0;
      }
      if(isHBrite()) {
        tr/=2;  tg/=2;  tb/=2;
        if((ds!=xs)&&score[ds]&&score[xs]&&(tr==xr)&&(tg==xg)&&(tb==xb)) {
          score[ds]+=score[xs];  score[xs]=0;
        }
      }
    }
  }
#ifdef DEBUG_VERBOSE
  dBug<<"Pixels: ";
  for(unsigned int ps=0;ps<numColors;ps++) dBug<<score[ps]<<" ";
  dBug<<"\n";
#endif // DEBUG_VERBOSE
  long best;
  int p;
#ifdef DEBUG_VERBOSE
  dBug<<"Pass: ";
#endif // DEBUG_VERBOSE
  for(int cur=numColors-1;cur>=0;cur--) {
#ifdef DEBUG_VERBOSE
    dBug<<cur<<" ";
#endif // DEBUG_VERBOSE
    best=0;  p=-1;
    for(unsigned int s=0;s<numColors;s++) {
      if((best<=score[s])&&(!a[s])) { best=score[s];  p=s; }
    }
    if(p!=-1) a[p]=cur;
#ifdef DEBUG_VERBOSE
    else dBug<<"No best on this pass!\n";
#endif // DEBUG_VERBOSE
  }
#ifdef DEBUG_VERBOSE
  dBug<<"\n";
  dBug<<"Scores: ";
#endif // DEBUG_VERBOSE
  for(unsigned int s=0;s<numColors;s++)  {
    score[s]=a[s];
#ifdef DEBUG_VERBOSE
    dBug<<score[s]<<" ";
#endif // DEBUG_VERBOSE
  }
#ifdef DEBUG_VERBOSE
  dBug<<"\n";
#endif // DEBUG_VERBOSE
  free(a);
  if(map) {
#ifdef DEBUG_VERBOSE
    dBug<<"Already a map!!!\n";
#endif // DEBUG_VERBOSE
  }
  map=(int *)malloc(sizeof(int)*numColors);
  if(!map) {
#ifdef DEBUG_VERBOSE
    dBug<<"Couldn't alloc map array!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  histComputed=true;
  makeMap();
}


void APalette::resetHistogram()
{
  if(tooBig) return;
  if(score) free(score);
  score=(long *)malloc(sizeof(long)*getNumColors());
  if(!score) {
#ifdef DEBUG_VERBOSE
    dBug<<"Couldn't allocate a histogram!!!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  for(unsigned int t=0;t<getNumColors();t++) score[t]=0;
}


void APalette::createEmptyMapAndHistogram()
{
  resetHistogram();
  for(unsigned int t=0;t<getNumColors();t++) score[t]=getNumColors();
  if(map) {
#ifdef DEBUG_VERBOSE
    dBug<<"Already a map!!!\n";
#endif // DEBUG_VERBOSE
    free(map);
  }
  map=(int *)malloc(sizeof(int)*numColors);
  if(!map) {
#ifdef DEBUG_VERBOSE
    dBug<<"Couldn't alloc map array!\n";
#endif // DEBUG_VERBOSE
    return;
  }
  histComputed=true;
  makeMap();
}


unsigned long APalette::getColor(unsigned int index)
{
  unsigned long val;
  val=(r[index]<<16)|(g[index]<<8)|b[index];
  return val;
}


void APalette::setColor(unsigned int index,unsigned int red, unsigned int green, unsigned int blue)
{
  if(readOnly) return;
  //if(error()) return;
  if(tooBig) return;
  if(index>getNumColors()) return;
  r[index]=red&bitsMask;  g[index]=green&bitsMask;  b[index]=blue&bitsMask;
  used[index]=true;
  touch();
}


void APalette::setColor(unsigned int index,unsigned long triplet)
{
  if(readOnly) return;
  setColor(index,getRGBRed24(triplet),getRGBGreen24(triplet),getRGBBlue24(triplet));
}


void APalette::setColor(unsigned int index,AColor *aColor)
{
  if(readOnly) return;
  if(!aColor) return;
  setColor(index,aColor->red,aColor->green,aColor->blue);
}


unsigned long APalette::findLightest()
{
  return findBestColor(0xffffffL);
}


unsigned long APalette::findDarkest()
{
  return findBestColor(0x000000L);
}


void APalette::touch()
{
  defaultRGB=false;
  changed=true;
  histComputed=false;
}


unsigned int APalette::bits2Num(unsigned int bits)
{
  unsigned int ret=1<<bits;
  if(sizeof(int)==2) {
    if(bits>31) ret=0xffffffffL;  // FIXME: doesn't work for 16bit ints obviously...
  }
  return ret;
}


bool APalette::sameAs(APalette *otherPalette)
{
  if(!otherPalette) return false;
  if(otherPalette->numColors!=numColors) return false;
  //if(otherPalette->significantBits!=significantBits) return false;
  //if(otherPalette->defaultRGB!=defaultRGB) return false;
  bool ret=true;
  for(unsigned int t=0;t<numColors;t++) {
    if(otherPalette->r[t]!=r[t]) ret=false;
    if(otherPalette->g[t]!=g[t]) ret=false;
    if(otherPalette->b[t]!=b[t]) ret=false;
  }
  return ret;
}


bool APalette::closeEnoughTo(APalette *otherPalette)
{
  return sameAs(otherPalette);
}

