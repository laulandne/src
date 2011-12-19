
#include <iostream>

#include <guilib/AApp.h>

#include <guilib/ADisplay.h>
#include <guilib/AWindow.h>
#include <guilib/AEvent.h>
#include <guilib/ADialogMsg.h>
#include <guilib/ARequesterFile.h>
#include <guilib/APrivateBitmap.h>

#include <portable/ndecoder/ASmartDecoder.h>
#include <portable/ndecoder/AImageDecoder.h>

#include <portable/nickcpp/AStringHelper.h>

#ifdef ASYS_WINDOWS
#include <io.h>
#endif // ASYS_WINDOWS


// Code I wrote a LOOOOONNNNGGGG time ago...


#define BYTE_POINTER char *

extern AApp aThisApp;


bool DIED=true;

bool boardError=false;


int GhostEatCount=0;
int WakaFlag=0;
int TunnelFlag=0;


ADisplay *dests=(ADisplay *)NULL;
AWindow *destw=(AWindow *)NULL;

APrivateBitmap *srcs;


#define copy_image(t,w,h,s,x,y)  ((APrivateBitmap *)new ASubBitmap(s,x,y,w,h))

/* if set, sends info to stderr */
#ifndef MYDEBUG
#endif

/* If set, includes code to catch player and ghost movement off board */
#ifndef CATCH_OUT
#endif

/* The rate at which the itimer is set to update player movements, etc. */
#define TIMER_VAL	10

/* Speed...If your CPU is too fast, then increase this */
#define CPU_SPEED		1

/* Size of the bitmaps which define the players and the board pieces */
/* NOTE: as it all is, severe messup if these are different!!! */
#define ASPECT 1

/* Size of string for all filenames used by game */
#define STRING_LEN      32

/* Maximum size for a board */
#define MAXBOARD_X      40
#define MAXBOARD_Y      24

/* How many turns will ghosts remain blue */
#define BLUE_TIME	800*SPEED*CPU_SPEED

/* Make this as close as possible to your default system font */
unsigned int FONT_WIDE=9;
unsigned int FONT_HIGH=10;


#define G_SMART		1
#define G_SCARED	2
#define G_EYES		4
#define G_TRYING	8
#define G_HOME		16
#define G_OUT		32
#define G_DUMB		0

#define GINT_HORIZ	0
#define GINT_VERT	1

#define SPR_GHOSTDOWN	0
#define SPR_GHOSTLEFT	1
#define SPR_GHOSTRIGHT	2
#define SPR_GHOSTUP	3
#define SPR_LEFT	4
#define SPR_RIGHT	5
#define SPR_UP		6
#define SPR_DOWN	7
#define SPR_GHOST	8
#define SPR_SCARED	9
#define SPR_PACNONE	10
#define SPR_PACLEFT	11
#define SPR_PACRIGHT	12
#define SPR_PACUP	13
#define SPR_PACDOWN	14
#define SPR_SPACE	15
#define SPR_HGATE	16
#define SPR_CROSS	17
#define SPR_VERT	18
#define SPR_HORIZ	19
#define SPR_TRIGHT	20
#define SPR_TLEFT	21
#define SPR_TUP		22
#define SPR_TDOWN	23
#define SPR_UPRIGHT	24
#define SPR_UPLEFT	25
#define SPR_DOWNLEFT	26
#define SPR_DOWNRIGHT	27
#define SPR_EYES	28
#define SPR_POWER	29
#define SPR_DOT		30
#define SPR_ROUND	31
#define SPR_GLEFT	32
#define SPR_GRIGHT	33
#define SPR_GUP		34
#define SPR_GDOWN	35
#define SPR_BLEFT	36
#define SPR_BRIGHT	37
#define SPR_BUP		38
#define SPR_BDOWN	39
#define SPR_ELEFT	40
#define SPR_ERIGHT	41
#define SPR_EUP		42
#define SPR_EDOWN	43
#define LAST_SPR	43


#define DIR_STOP	0
#define DIR_UP		1
#define DIR_DOWN	2
#define DIR_LEFT	3
#define DIR_RIGHT	4

#define GO_UP		128
#define GO_DOWN		64
#define GO_LEFT		32
#define GO_RIGHT	16
#define GO_DOT		2
#define GO_EMPTY	1

#define LEFT_TUNNEL	1
#define RIGHT_TUNNEL	2

/*
typedef char[4] sharray;
*/

struct Player {
  /* Starting coord (in blocks) */
  int sx,sy;
  /* Current coord (in pixels) */
  int zx,zy;
  /* Current coord (in blocks) */
  int bx,by;
  /* Last block coord */
  int lx,ly;
  /* Current and last pix num for shape */
  int cur,old;
  /* Up down left right shapes...indices into picts array */
  /* sharray *shapes; */
  /* Count to determine which block a player is in */
  int mcnt;
  /* Speed and mc are used to count down and skip turns... */
  /* ...not used right now */
  int speed,mc;
  /* Countdown of a ghost in home */
  int homecnt;
  /* Ghost intelligence junk */
  int intel,lastint,intdir;
  /* Current dir and next dir */
  int dir,nextdir;
  /* colors for X or flat amiga*/
  int fg,bg;
};

/* Temporary sprite ID's before we can move to blits... */
#define PAC_ID	0
#define G0_ID	1
#define G1_ID	2
#define G2_ID	3
#define G3_ID	4


struct PacPictBlock {
  unsigned int x,y;
};


//APrivateBitmap *picts[LAST_SPR+1];
struct PacPictBlock *picts[LAST_SPR+1];


/* typdef struct Player * playptr; */
/* extern playptr[G3_ID+1]; */


#define debug_msg(a)  std::cerr<<a;
#define debug_int(a)  std::cerr<<a;
#define debug_nl();   std::cerr<<"\n";
#define console_msg(a) std::cerr<<a;



/* Functions: */

/* From pac.c */
void CleanupBitmaps(void);
void InitGame(void);
void InitRound(void);
void DoGameOver(void);
void DoTitle(void);
void StartLoading(void);
void EndLoading(void);
void DoQuit(void);
//int main(int argc, char *argv[]);
void MyReadString(FILE *tfh, char *s, int m);
void ReadIDString(FILE *tfh, char *name, char *id, char *ss);
void ReadIDNums(FILE *tfh, char *name,char *id, int sp, int *x, int *y);
void ReadBoardFromDisk(char *name);
void WriteBoard(int x, int y, int c);
char ReadBoard(int x, int y);
void DrawBoardPart(int x, int y, int c);
void DrawScreen(void);
void UpdateScore(void);
bool CheckSpace(int x, int y, int *dx, int *dy);
bool NewCheckSpace(int x, int y, int *dx, int *dy);
char CheckBoardInfo(int x, int y);
void ExploreBoard(void);

/* From pacloop.c */
void DoDie(void);
bool update(void);
bool mainloop(void);

/* From pacghost.c */
void DoTunnel(struct Player *p, int w);
void DoNormal(struct Player *g);
void GoHome(struct Player *g);
void DoBlueGhost(struct Player *g, int bit);
void DoMyBlue(void);
void DoEyes(struct Player *g);
void TheBlue(void);
void HomeIn(struct Player *g, int sx, int sy);
void Avoid(struct Player *g, int sx, int sy);
void Stupid(struct Player *g);
void GhostChoose(struct Player *g);
void MoveGhost(struct Player *g);
void MovePac(struct Player *P);

/* From pacbits.c */
PacPictBlock *newPacPictBlock(unsigned int x,unsigned int y);
void SetSpriteShapes(void);
void SetupSprites(void);
void SetupBitmaps(void);

void my_fatal(char *);


#define CHECK_BOARD



bool WAITING=false;
bool QUITING=false;

bool DoReverse=false;

struct Player Pac;
struct Player Ghost0;
struct Player Ghost1;
struct Player Ghost2;
struct Player Ghost3;

int NumMen;
int NumDots;
int NumPoints;
int BlueCount;
int WakaCount;
int DrawScreenFlag=0;

int SPEED=1;
int FOOBAR_PACMOVE=true;
int FOOBAR_MONO=0;

char Board[MAXBOARD_X][MAXBOARD_Y];
char BoardInfo[MAXBOARD_X][MAXBOARD_Y];

char *DisplayName=(char *)NULL;

FILE *bfh=(FILE *)NULL;
FILE *myfh=(FILE *)NULL;

/* These are the size of the actual maze in blocks */
int BoardX,BoardY;

/* Home position */
int HomeX,HomeY;

/* Size of window */
/* If we ask for zero's we'll get a system default */
int SCREEN_X=0;
int SCREEN_Y=0;
int SCREEN_D=0;

/* Size of source bitmap */
int SOURCE_X=0;
int SOURCE_Y=0;
int SOURCE_D=0;

/* Size of blocks in pixels */
/* Assuming we've got at least a 640x400 window... */
int BITMAP_X=32;
int BITMAP_Y=32;

/* These are for strange aspect ratios...like 640x200 and 320x400 */
int AspectX=1;
int AspectY=1;
int CountMax;  /* Used for the block to pixel conversion... */

char NextBoard[STRING_LEN];
char BitmapName[STRING_LEN];


ADisplay *ls=(ADisplay *)NULL;
AWindow *lw=(AWindow *)NULL;
//struct n_font *lf=NULL;


void my_fatal(char *s)
{
  std::cerr<<"Fatal error!\n";
  std::cerr<<s<<"\n";
}


void CleanupBitmaps(void)
{
  int t;
  std::cerr<<"DEBUG- in CleanupBitmaps\n";
  for(t=0;t<LAST_SPR;t++) {
    if(picts[t]) { delete picts[t];  picts[t]=(PacPictBlock *)NULL; }
  }
  if(srcs) { delete srcs;  srcs=(APrivateBitmap *)NULL; }
  if(destw) { delete destw;  destw=(AWindow *)NULL; }
  if(dests) { delete dests;  dests=(ADisplay *)NULL; }
  if(myfh) { fclose(myfh);  myfh=(FILE *)NULL; }
  if(bfh) { fclose(bfh);  bfh=(FILE *)NULL; }
  std::cerr<<"DEBUG- CleanupBitmaps done\n";
}


void InitGame(void)
{
  int t;
  std::cerr<<"DEBUG- in InitGame\n";
  NumPoints=0;  NumMen=5;  WakaCount=0;
  for(t=0;t<LAST_SPR;t++) {
    picts[t]=(PacPictBlock *)NULL;
  }
  myfh=(FILE *)NULL;
  bfh=(FILE *)NULL;
  dests=(ADisplay *)NULL;
  destw=(AWindow *)NULL;
  srcs=(APrivateBitmap *)NULL;
  std::cerr<<"DEBUG- InitGame done\n";
}


void InitRound(void)
{
  std::cerr<<"DEBUG- InitRound\n";
  Pac.zy=Pac.sy*BITMAP_Y; Pac.zx=Pac.sx*BITMAP_X; Pac.mcnt=0;
  Pac.nextdir=DIR_STOP; Pac.cur=SPR_PACLEFT; Pac.speed=1*SPEED*CPU_SPEED;
  Pac.mc=0; Pac.old=SPR_PACLEFT; Pac.dir=DIR_STOP;
  Pac.bx=Pac.sx;  Pac.by=Pac.sy;
  Ghost0.zy=Ghost0.sy*BITMAP_Y; Ghost0.zx=Ghost0.sx*BITMAP_X;
  Ghost0.old=SPR_GHOST; Ghost0.speed=1*SPEED*CPU_SPEED;
  Ghost0.lastint=G_SMART;  Ghost0.mc=0; Ghost0.cur=SPR_GHOST;
  Ghost0.ly=Ghost0.sy; Ghost0.lx=Ghost0.sx;
  Ghost0.bx=Ghost0.sx;  Ghost0.by=Ghost0.sy;
  Ghost0.dir=DIR_LEFT; Ghost0.nextdir=DIR_LEFT; Ghost0.mcnt=CountMax;
  Ghost1.zy=Ghost1.sy*BITMAP_Y; Ghost1.zx=Ghost1.sx*BITMAP_X;
  Ghost1.old=SPR_GHOST; Ghost1.speed=1*SPEED*CPU_SPEED;
  Ghost1.lastint=G_SMART;  Ghost1.mc=0; Ghost1.cur=SPR_GHOST;
  Ghost1.ly=Ghost1.sy; Ghost1.lx=Ghost1.sx;
  Ghost1.bx=Ghost1.sx;  Ghost1.by=Ghost1.sy;
  Ghost1.dir=DIR_RIGHT; Ghost1.nextdir=DIR_RIGHT; Ghost1.mcnt=CountMax;
  Ghost2.zy=Ghost2.sy*BITMAP_Y; Ghost2.zx=Ghost2.sx*BITMAP_X;
  Ghost2.old=SPR_GHOST; Ghost2.speed=1*SPEED*CPU_SPEED;
  Ghost2.lastint=G_SMART;  Ghost2.mc=0; Ghost2.cur=SPR_GHOST;
  Ghost2.ly=Ghost2.sy; Ghost2.lx=Ghost2.sx;
  Ghost2.bx=Ghost2.sx;  Ghost2.by=Ghost2.sy;
  Ghost2.dir=DIR_LEFT; Ghost2.nextdir=DIR_LEFT; Ghost2.mcnt=CountMax;
  Ghost3.zy=Ghost3.sy*BITMAP_Y; Ghost3.zx=Ghost3.sx*BITMAP_X;
  Ghost3.old=SPR_GHOST; Ghost3.speed=1*SPEED*CPU_SPEED;
  Ghost3.lastint=G_SMART;  Ghost3.mc=0; Ghost3.cur=SPR_GHOST;
  Ghost3.ly=Ghost3.sy; Ghost3.lx=Ghost3.sx;
  Ghost3.bx=Ghost3.sx;  Ghost3.by=Ghost3.sy;
  Ghost3.dir=DIR_RIGHT; Ghost3.nextdir=DIR_RIGHT; Ghost3.mcnt=CountMax;
  GoHome(&Ghost0);  GoHome(&Ghost1);
  GoHome(&Ghost2);  GoHome(&Ghost3);
  BlueCount=0;  GhostEatCount=100;
}


void DoGameOver(void)
{
  QUITING=true;
}


void DoTitle(void)
{
  AWindow *w;
  //struct n_font *f;
  AEvent e;
  bool W=true;
  std::cerr<<"DEBUG- in DoTitle\n";
  w=new AWindow((char *)"NuPac");
  if(!w) {
    std::cerr<<"Couldn't get title window!\n";
    return;
  }
  std::cerr<<"(got window)\n";
  unsigned long forec=0xffffff,backc=0;
  w->setForeground(forec);
  w->setBackground(backc);
  w->setFont((char *)"Helvetica");
  FONT_HIGH=w->getHeight()/25;
  // If the screen is smaller than an arbitrary height, make the size smaller
  if(w->getHeight()<350) FONT_HIGH=w->getHeight()/35;
  if(w->getHeight()<200) FONT_HIGH=w->getHeight()/40;
  std::cerr<<"(going to enter wait for click loop)\n";
  while(W) {
    std::cerr<<"(going to draw titles)\n";
    w->clear();
    w->setFontSize(FONT_HIGH*2);
    w->moveTo(0,w->getHeight()/4);
    w->center((char *)"Nick's Machine Independent\n");
    w->setFontSize(FONT_HIGH*3);
    w->moveTo(0,w->getHeight()/2);
    w->center((char *)"PAC-MAN\n");
    w->moveTo(0,(w->getHeight())-(w->getHeight()/4)-(FONT_HIGH*2));
    w->setFontSize(FONT_HIGH);
    w->center((char *)"Press any key or click mouse to continue\n");
    w->moveTo(0,(w->getHeight())-(w->getHeight()/4));
    w->center((char *)"(or close window to quit)\n");
    //e<<*w;
    std::cerr<<"(going to wait for click)\n";
    e.wait(w);
    if(e.type==A_EVENT_BREAK) W=false;
    if(e.type==A_EVENT_DESTROY) W=false;
    if(e.type==A_EVENT_KEYUP) W=false;
    if(e.type==A_EVENT_MOUSEUP) W=false;
  }
  std::cerr<<"(done waiting, going to close window)\n";
  delete w;  //delete s;
  w=(AWindow *)NULL;
  //if(e.type==A_EVENT_DESTROY) { std::cerr<<"Bye for now!\n"; exit(0); }
  //if(e.type==A_EVENT_BREAK) { std::cerr<<"***Break\n"; exit(0); }
  std::cerr<<"DEBUG- DoTitle done\n";
}


void StartLoading(void)
{
  std::cerr<<"DEBUG- in StartLoading\n";
  ls=new ADisplay;
  lw=new AWindow((char *)"Loading Pac Board...");
  if(!lw) {
    std::cerr<<"Couldn't get loading window!\n";
    return;
  }
  //lw->resize(300,100);
  unsigned long forec=0xffffff,backc=0;
  lw->setForeground(forec);
  lw->setBackground(backc);
  lw->setFont((char *)"Helvetica");
  FONT_HIGH=lw->getHeight()/25;
  // If the screen is smaller than an arbitrary height, halve the font size
  if(lw->getHeight()<350) FONT_HIGH=FONT_HIGH/2;
  lw->setFontSize(FONT_HIGH*2);
  lw->clear();
  lw->moveTo(0,(lw->getHeight()/2)-(lw->getFontHeight()*3));
  lw->center((char *)"Nick's Pac-Mac");
  lw->newline();
  lw->center((char *)"Loading Game Board...");
  std::cerr<<"DEBUG- StartLoading done\n";
}


void EndLoading(void)
{
  std::cerr<<"DEBUG- in EndLoading\n";
  //if(lf) free_font(lf);
  if(lw) delete lw;
  if(ls) delete ls;
  //lf=NULL;
  lw=(AWindow *)NULL;
  ls=NULL;
}


void DoQuit(void)
{
  CleanupBitmaps();
  if(NumMen>0) { std::cerr<<"Game Over!\n";  exit(0); }
  std::cerr<<"Quiting...\n";  exit(0);
}


int main(int argc, char **argv)
{
  aThisApp.start(argc,argv);
  char *fname=(char *)NULL;
  if(aThisApp.argc>1) fname=aThisApp.argv[1];
  if(aThisApp.argc<2) {
    ARequesterFile *ww=new ARequesterFile();
    if(ww) fname=ww->show((char *)"Open file");
    else std::cerr<<"(Didn't get a request window)\n";
    if(ww) delete ww;
    ww=NULL;
  }
  if(!fname) {
    char msg[256];
    sprintf(msg,(char *)"usage: nupac {board filename}",aThisApp.argv[0]);
    ADialogMsg mw((char *)"Error!",msg);
    //return 5;
    return 0;
  }
  char *bname=fname;
  unsigned int t;
  char *temp;
  InitGame();
  for(t=1;t<aThisApp.argc;t++) {
    temp=aThisApp.argv[t];
    if(temp[0]=='-') {
      if(temp[1]=='d') {
        DisplayName=aThisApp.argv[t+1]; t++;  std::cerr<<"Display name is ";
        std::cerr<<DisplayName<<"\n";
      } else
      if(temp[1]=='p') { FOOBAR_PACMOVE=false; } else
      if(temp[1]=='m') { FOOBAR_MONO=1; } else
      if(temp[1]=='s') {
        sscanf(aThisApp.argv[t+1],(char *)"%d",(unsigned int *)&SPEED);
        t++;
        std::cerr<<"New speed is";
        std::cerr<<SPEED<<"\n";
      } else
      std::cerr<<aThisApp.argv[0]<<": bad command option\n";
    }
    else {
      bname=temp;
    }
  }
  std::cerr<<"Initial board is "<<bname<<"\n";
  DoTitle();
  QUITING=false;
  while(!QUITING) {
    std::cerr<<"Looping in main while !QUITING...\n";
    //StartLoading();
    CleanupBitmaps();
    if(bname) ReadBoardFromDisk(bname);
    else boardError=true;
    if(boardError) {
      QUITING=true;
    }
    else {
      ExploreBoard();
      SetupBitmaps();
      if(!QUITING) {
        SetSpriteShapes();
        DrawScreen();
        UpdateScore();  InitRound();
        DIED=true;
        while(DIED) {
          mainloop();
          UpdateScore();  InitRound();
          if(QUITING) DoQuit();
        }
      }
      bname=NextBoard;  boardError=false;
    }
  }
  std::cerr<<"Normal exit\n";
  return 0;
}


void MyReadString(FILE *tfh, char *s, int m)
{
  int c=0,len;
  char foo;
  bool READING=true,QUOTED=false;
  while(READING) {
    len=fread((BYTE_POINTER)&foo,1,1,bfh);
    if((foo!='"')&&(foo!=0xa)) { s[c]=foo; if(READING) c++; }
    if(foo==0x0a) READING=false;
    if(foo=='"') {
      if(QUOTED) { READING=false; } else { QUOTED=true; }
    }
    if(len!=1) READING=false;
    if(m==c) READING=false;
  }
  s[c]=0;
}


void ReadIDString(FILE *tfh, char *name, char *id, char *ss)
{
  std::cerr<<"ReadIDString(,"<<name<<","<<id<<",)...\n";
  int len;
  char fid[5],c;
  fid[4]=0;
  len=fread((BYTE_POINTER)fid,1,1,tfh);
  if(fid[0]==13) len=fread((BYTE_POINTER)fid,1,1,tfh);
  if(fid[0]==10) len=fread((BYTE_POINTER)fid,1,1,tfh);
  len=fread((BYTE_POINTER)(fid+1),3,1,tfh);
  std::cerr<<"len is "<<len<<"\n";
  if(len!=1) { my_fatal((char *)"Read error in IDString\n"); exit(0); }
  if(strcmp(fid,id))  { std::cerr<<"ID error in IDString\n"; boardError=true; return; }
  MyReadString(tfh,ss,STRING_LEN);
  len=fread((BYTE_POINTER)&c,1,1,tfh);
  std::cerr<<"len is "<<len<<"\n";
  if(len!=1) { my_fatal((char *)"Read error NL\n");  exit(0); }
}


void ReadIDNums(FILE *tfh, char *name, char *id, int sp, int *x, int *y)
{
  std::cerr<<"ReadIDNums(,"<<name<<","<<id<<",,)...\n";
  *x=0;  *y=0;
  int len;
  char c,fid[5];
  char num[5];
  fid[4]=0;
  len=fread((BYTE_POINTER)fid,1,1,tfh);
  if(fid[0]==13) len=fread((BYTE_POINTER)fid,1,1,tfh);
  if(fid[0]==10) len=fread((BYTE_POINTER)fid,1,1,tfh);
  len=fread((BYTE_POINTER)(fid+1),3,1,tfh);
  std::cerr<<"len is "<<len<<"\n";
  if(len!=1) { my_fatal((char *)"Read error in IDNums\n"); exit(0); }
  std::cerr<<"Looking in "<<name<<" for '"<<id<<"', read '"<<fid<<"'\n";
  if(strcmp(fid,id))  {
    std::cerr<<"ID error in IDNums ((char *)"<<fid<<"!="<<id<<")\n"; boardError=true; return;
  }
  len=fread((BYTE_POINTER)num,2,1,tfh);
  if(len!=1) { my_fatal((char *)"Read error num1\n"); exit(0); }
  std::cerr<<"len is "<<len<<"\n";
  num[2]=0;
  *x=aDefaultStringHelper.parseNumber(num);
  std::cerr<<"Num is "<<num<<", x is "<<(*x)<<"\n";
  len=fread((BYTE_POINTER)&c,1,1,tfh);
  if(c!=sp)  { my_fatal((char *)"Spacer error\n"); exit(0); }
  len=fread((BYTE_POINTER)num,2,1,tfh);
  std::cerr<<"len is "<<len<<"\n";
  if(len!=1) { my_fatal((char *)"Read error num2\n"); exit(0); }
  num[2]=0;
  *y=aDefaultStringHelper.parseNumber(num);
  std::cerr<<"Num is "<<num<<", y is "<<(*y)<<"\n";
  len=fread((BYTE_POINTER)&c,1,1,tfh);
  std::cerr<<"len is "<<len<<"\n";
  if(len!=1) { my_fatal((char *)"Read error\n"); exit(0); }
  std::cerr<<"good.\n";
}


void ReadBoardFromDisk(char *name)
{
  int x,y,len;
  char foo[2];
  std::cerr<<"DEBUG- ReadBoardFromDisk\n";
  for(y=0;y<MAXBOARD_Y;y++) {
    for(x=0;x<MAXBOARD_X;x++) {
      WriteBoard(x,y,0);
    }
  }
  bfh=(FILE *)NULL;  foo[1]=0;
  char pathName[1024];
  if(aDefaultStringHelper.getPathFromFullName(pathName,name)) {
    // If there's a prefixed path to the config file, we'll go to that directory
    std::cerr<<"path is "<<pathName<<"\n";
#ifdef ASYS_WINDOWS
    _chdir(pathName);
#else
    chdir(pathName);
#endif // ASYS_WINDOWS
  }
  char fileName[1024];
  if(aDefaultStringHelper.getNameFromFullName(fileName,name)) {
    std::cerr<<"file is "<<fileName<<"\n";
    name=fileName;
  }
  bfh=fopen(name,"r");
  if(!bfh) {
    ADialogMsg mw((char *)"Error!",(char *)"Couldn't open board datafile!");
    //if(lw) delete lw;
    boardError=true;
    return;
  }
  std::cerr<<"Opened "<<name<<" fine.\n";
  BoardX=0;  BoardY=0;
  ReadIDNums(bfh,name,(char *)"PAC:",'x',&BoardX,&BoardY);
  for(y=0;y<BoardY;y++) {
    for(x=0;x<BoardX;x++) {
      len=fread((BYTE_POINTER)foo,1,1,bfh);
      //if(len!=1) { my_fatal((char *)"Read error!\n"); exit(5); }
      if((foo[0]!=13)&&(foo[0]!=10)) {
        WriteBoard(x,y,foo[0]);
        std::cerr<<foo<<"((char *)"<<(int)foo[0]<<")";
      }
      if(foo[0]==10) {
        x--; std::cerr<<"(skipped a 10)";
      }
      if(foo[0]==13) {

        x--; std::cerr<<"(skipped a 13)";
      }
    }
    len=fread((BYTE_POINTER)foo,1,1,bfh); std::cerr<<"\n";
  }
  if(!boardError) ReadIDNums(bfh,name,(char *)"hom:",',',&HomeX,&HomeY);
  if(!boardError) ReadIDNums(bfh,name,(char *)"pac:",',',&Pac.sx,&Pac.sy);
  if(!boardError) ReadIDNums(bfh,name,(char *)"gh0:",',',&Ghost0.sx,&Ghost0.sy);
  if(!boardError) ReadIDNums(bfh,name,(char *)"gh1:",',',&Ghost1.sx,&Ghost1.sy);
  if(!boardError) ReadIDNums(bfh,name,(char *)"gh2:",',',&Ghost2.sx,&Ghost2.sy);
  if(!boardError) ReadIDNums(bfh,name,(char *)"gh3:",',',&Ghost3.sx,&Ghost3.sy);
  if(!boardError) ReadIDString(bfh,name,(char *)"BIT:",BitmapName);
  if(!boardError) ReadIDString(bfh,name,(char *)"NXT:",NextBoard);
  fclose(bfh);  bfh=(FILE *)NULL;
  if(boardError) {
    ADialogMsg mw((char *)"Board format error!",(char *)"There was a problem reading the board!");
  }
}


void WriteBoard(int x, int y, int c)
{
#ifdef CHECK_BOARD
  if((x>=0)&&(y>=0)&&(x<MAXBOARD_X)&&(y<MAXBOARD_Y))
#endif
  { Board[x][y]=c; }
#ifdef CHECK_BOARD
  else {
    my_fatal((char *)"Illegal write\n");
    exit(0);
  }
#endif
}


char ReadBoard(int x, int y)
{
#ifdef CHECK_BOARD
  if((x>=0)&&(y>=0)&&(x<MAXBOARD_X)&&(y<MAXBOARD_Y))
#endif
  { return Board[x][y]; }
#ifdef CHECK_BOARD
  else { my_fatal((char *)"Illegal Read error!\n"); exit(5); }
  return 0;
#endif
}


void DrawBoardPart(int x, int y, int c)
{
  int p;
  switch(c) {
    case ',': p=SPR_SPACE; break;
    case '.': p=SPR_DOT; break;
    case '*': p=SPR_POWER; break;
    case 'n': p=SPR_HORIZ; break;
    case 'j': p=SPR_VERT; break;
    case 'b': p=SPR_DOWNRIGHT; break;
    case 'd': p=SPR_DOWNLEFT; break;
    case 'k': p=SPR_UPRIGHT; break;
    case 'm': p=SPR_UPLEFT; break;
    case 'u': p=SPR_ROUND; break;
    case 't': p=SPR_UP; break;
    case 'p': p=SPR_LEFT; break;
    case 'o': p=SPR_RIGHT; break;
    case 'c': p=SPR_TDOWN; break;
    case 'l': p=SPR_TUP; break;
    case 'h': p=SPR_TRIGHT; break;
    case 'f': p=SPR_TLEFT; break;
    case 'e': p=SPR_HGATE; break;
    case 's': p=SPR_DOWN; break;
    case 'g': p=SPR_CROSS; break;
    default:
      { my_fatal((char *)"Illegal draw char error!\n"); exit(5); }
      break;
  }
  //destw->draw(picts[p],x*BITMAP_X,y*BITMAP_Y);
  destw->copyBlock(srcs,picts[p]->x,picts[p]->y,x*BITMAP_X,y*BITMAP_Y,BITMAP_X,BITMAP_Y);
}


void DrawScreen(void)
{
  int x,y,c;
  /* debug_msg((char *)"DEBUG- DrawScreen\n"); */
  destw->setForeground(0xffffff);  destw->setBackground(0);
  destw->clear();
  NumDots=0;
  for(y=0;y<BoardY;y++) {
    for(x=0;x<BoardX;x++) {
      c=ReadBoard(x,y);  DrawBoardPart(x,y,c);
      if(c==(int)'.') NumDots++;
      if(c==(int)'*') NumDots++;
    }
  }
  UpdateScore();
}


void UpdateScore(void)
{
  char foo[3];
  char num[100];
  if(NumMen<0) DoGameOver();
  //set_foregroundc(destw,fore);
  FONT_HIGH=destw->getHeight()/25;
  destw->setFontSize(FONT_HIGH);
  destw->moveTo(FONT_WIDE*1,SCREEN_Y-FONT_HIGH-8);  *destw<<(char *)"Score: ";
  sprintf(num,(char *)"%d",NumPoints);
  destw->moveTo(7*FONT_WIDE,SCREEN_Y-FONT_HIGH-8);
  *destw<<num;
  destw->moveTo(SCREEN_X/2,SCREEN_Y-FONT_HIGH-8); *destw<<(char *)"Men: ";
  foo[1]=0;
  foo[0]=(char)NumMen+'0';
  destw->moveTo(SCREEN_X/2+5*FONT_WIDE,SCREEN_Y-FONT_HIGH-8);
  *destw<<foo;
}


bool CheckSpace(int x, int y, int *dx, int *dy)
{
  int bx,by,foo;
  bx=(x/BITMAP_X)+(*dx);  by=(y/BITMAP_Y)+(*dy);  foo=ReadBoard(bx,by);
  if(foo>='a') { return true; } else { return false; }
}


/* NOTE: This function isn't finished yet... */
bool NewCheckSpace(int x, int y, int *dx, int *dy)
{
  int foo;
  foo=ReadBoard(x,y);
  if(foo>'a') { return true; } else { return false; }
}


/* NOTE: This function isn't finished yet... */
char CheckBoardInfo(int x, int y)
{
  char foo,goo;
  char foor,fool,foou,food;
  goo=0;
  foo=Board[x][y];
  if(foo==',') goo&=GO_EMPTY;
  if((foo=='.')||(foo=='*')) { goo&=GO_DOT; goo&=GO_EMPTY; }
  fool=Board[x-1][y];  foor=Board[x+1][y];
  foou=Board[x][y-1];  food=Board[x][y+1];
  if((fool=='.')||(fool=='*')||(fool==',')) goo&=GO_LEFT;
  if((foor=='.')||(foor=='*')||(foor==',')) goo&=GO_RIGHT;
  if((foou=='.')||(foou=='*')||(foou==',')) goo&=GO_UP;
  if((food=='.')||(food=='*')||(food==',')) goo&=GO_DOWN;
  return goo;
}


void ExploreBoard(void)
{
  int x,y;
  for(x=0;x<BoardX;x++)
    for(y=0;y<BoardY;y++)
      BoardInfo[x][y]=0;
  for(x=1;x<BoardX-1;x++)
    for(y=1;y<BoardY-1;y++) {
      BoardInfo[x][y]=CheckBoardInfo(x,y);
    }
}


PacPictBlock *newPacPictBlock(unsigned int x,unsigned int y)
{
  PacPictBlock *p=new PacPictBlock;
  p->x=x;  p->y=y;
  return p;
}


void SetSpriteShapes(void)
{
  debug_msg((char *)"DEBUG- SetSpriteShapes\n");
  picts[SPR_PACNONE]=newPacPictBlock(3*BITMAP_X,3*BITMAP_Y);
  picts[SPR_PACLEFT]=newPacPictBlock(0*BITMAP_X,4*BITMAP_Y);
  picts[SPR_PACRIGHT]=newPacPictBlock(0*BITMAP_X,5*BITMAP_Y);
  picts[SPR_PACUP]=newPacPictBlock(0*BITMAP_X,3*BITMAP_Y);
  picts[SPR_PACDOWN]=newPacPictBlock(0*BITMAP_X,6*BITMAP_Y);
  picts[SPR_GHOST]=newPacPictBlock(12*BITMAP_X,10*BITMAP_Y);
  picts[SPR_EYES]=newPacPictBlock(16*BITMAP_X,7*BITMAP_Y);
  picts[SPR_SCARED]=newPacPictBlock(18*BITMAP_X,14*BITMAP_Y);
  picts[SPR_GRIGHT]=newPacPictBlock(6*BITMAP_X,3*BITMAP_Y);
  picts[SPR_ERIGHT]=newPacPictBlock(6*BITMAP_X,7*BITMAP_Y);
  picts[SPR_BRIGHT]=newPacPictBlock(6*BITMAP_X,8*BITMAP_Y);
  picts[SPR_GLEFT]=newPacPictBlock(4*BITMAP_X,3*BITMAP_Y);
  picts[SPR_ELEFT]=newPacPictBlock(4*BITMAP_X,7*BITMAP_Y);
  picts[SPR_BLEFT]=newPacPictBlock(4*BITMAP_X,8*BITMAP_Y);
  picts[SPR_GDOWN]=newPacPictBlock(8*BITMAP_X,3*BITMAP_Y);
  picts[SPR_EDOWN]=newPacPictBlock(8*BITMAP_X,7*BITMAP_Y);
  picts[SPR_BDOWN]=newPacPictBlock(8*BITMAP_X,8*BITMAP_Y);
  picts[SPR_GUP]=newPacPictBlock(10*BITMAP_X,3*BITMAP_Y);
  picts[SPR_EUP]=newPacPictBlock(10*BITMAP_X,7*BITMAP_Y);
  picts[SPR_BUP]=newPacPictBlock(10*BITMAP_X,8*BITMAP_Y);
/*
  for(unsigned int t=0;t<LAST_SPR;t++) {
    if(picts[t]) { picts[t]->setLetter('A'+t); }
  }
*/
}


void SetupBitmaps(void)
{
  debug_msg((char *)"DEBUG- SetupBitmaps\n");
  AImageDecoder *dec=(AImageDecoder *)ASmartDecoder::pickBestDecoder(BitmapName);
  // TODO: Should check for error here
  ABitmap *tbm=dec->createNewBitmapObject();
  srcs=new APrivateBitmap(tbm);
  if(!srcs) QUITING=true;
  if(QUITING) {
    ADialogMsg mw((char *)"Problem with bitmap!",(char *)"There was a problem reading the image file!");
    return;
  }
  //load_image_size(BitmapName,&SOURCE_X,&SOURCE_Y,&SOURCE_D);
  // WARNING, HACK here.
  //SOURCE_X=320;  SOURCE_Y=200; SOURCE_D=8;
  SOURCE_X=srcs->getWidth();  SOURCE_Y=srcs->getHeight();
  SOURCE_D=srcs->getDepth();
  BITMAP_X=(int)(SOURCE_X/20);  BITMAP_Y=(int)(SOURCE_Y/12.5);
  //BITMAP_X=16;  BITMAP_Y=16;
  /* SCREEN_X=BITMAP_X*BoardX;  SCREEN_Y=BITMAP_Y*BoardY; */
  SCREEN_X=SOURCE_X;  SCREEN_Y=SOURCE_Y;
  SCREEN_D=SOURCE_D;
  std::cerr<<"desired screensize is "<<SCREEN_X<<"x"<<SCREEN_Y<<"\n";
  std::cerr<<"bitsize is "<<BITMAP_X<<"x"<<BITMAP_Y<<"\n";
  std::cerr<<"bitname is "<<BitmapName<<"\n";
  if(SOURCE_Y) {
    if((SOURCE_X/SOURCE_Y)==0) {
      /* like 320x400 */
      AspectX=1; AspectY=2;
    }
    if((SOURCE_X/SOURCE_Y)==1) {
      /* like 640x480 */
      AspectX=1; AspectY=1;
    }
    if((SOURCE_X/SOURCE_Y)>1) {
      /* like 640x200 */
      AspectX=2; AspectY=1;
    }
  }
  std::cerr<<"Aspect is "<<AspectX<<":"<<AspectY<<"\n";
  if(BITMAP_X>BITMAP_Y) { CountMax=BITMAP_Y; } else { CountMax=BITMAP_X; }
  EndLoading();
  dests=new ADisplay(/**srcs*/);
  /* NOTE: Could open a smaller window sometimes... */
  destw=new AWindow(dests,(char *)"Pac",SCREEN_X,SCREEN_Y);
  if(!destw) {
    std::cerr<<"Couldn't get game window!\n";
    return;
  }
  unsigned long forec=0xffffff,backc=0;
  destw->setForeground(forec);
  destw->setBackground(backc);
  destw->setFont((char *)"Helvetica");
  SCREEN_X=destw->getWidth();  SCREEN_Y=destw->getHeight();
  SCREEN_D=dests->getDepth();
  std::cerr<<"actual display: "<<SCREEN_X<<"x"<<SCREEN_Y<<"x"<<SCREEN_D<<"\n";
  srcs->convert(SCREEN_D,dests->getBitmapType(),destw->getColors());
  std::cerr<<"Setting up picts[]...\n";
  picts[SPR_ROUND]=newPacPictBlock(7*BITMAP_X,0);
  picts[SPR_POWER]=newPacPictBlock(8*BITMAP_X,2*BITMAP_Y);
  picts[SPR_DOT]=newPacPictBlock(8*BITMAP_X,0);
  picts[SPR_SPACE]=newPacPictBlock(9*BITMAP_X,0);
  picts[SPR_HGATE]=newPacPictBlock(7*BITMAP_X,BITMAP_Y);
  picts[SPR_CROSS]=newPacPictBlock(BITMAP_X,BITMAP_Y);
  picts[SPR_VERT]=newPacPictBlock(3*BITMAP_X,BITMAP_Y);
  picts[SPR_HORIZ]=newPacPictBlock(3*BITMAP_X,0);
  picts[SPR_TDOWN]=newPacPictBlock(BITMAP_X,0);
  picts[SPR_TLEFT]=newPacPictBlock(2*BITMAP_X,BITMAP_Y);
  picts[SPR_TRIGHT]=newPacPictBlock(0,BITMAP_Y);
  picts[SPR_TUP]=newPacPictBlock(BITMAP_X,2*BITMAP_Y);
  picts[SPR_UP]=newPacPictBlock(6*BITMAP_X,2*BITMAP_Y);
  picts[SPR_DOWN]=newPacPictBlock(6*BITMAP_X,1*BITMAP_Y);
  picts[SPR_LEFT]=newPacPictBlock(4*BITMAP_X,2*BITMAP_Y);
  picts[SPR_RIGHT]=newPacPictBlock(3*BITMAP_X,2*BITMAP_Y);
  picts[SPR_UPRIGHT]=newPacPictBlock(0,2*BITMAP_Y);
  picts[SPR_UPLEFT]=newPacPictBlock(2*BITMAP_X,2*BITMAP_Y);
  picts[SPR_DOWNRIGHT]=newPacPictBlock(0,0);
  picts[SPR_DOWNLEFT]=newPacPictBlock(2*BITMAP_X,0);
  std::cerr<<"Done with picts[].\n";
}


void DoTunnel(struct Player *p, int w)
{
  p->dir=DIR_STOP;  p->nextdir=DIR_STOP;
  p->mcnt=0;  p->mc=0;
  DrawBoardPart(p->lx,p->ly,ReadBoard(p->lx,p->ly));
  DrawBoardPart(p->bx,p->by,ReadBoard(p->bx,p->by));
  switch(w) {
    case LEFT_TUNNEL:
      p->zx=17*BITMAP_X;  p->zy=5*BITMAP_Y;  p->lx=17;  p->ly=5;  break;
    case RIGHT_TUNNEL:
      p->zx=1*BITMAP_X;  p->zy=5*BITMAP_Y;  p->lx=1;  p->ly=5;  break;
    default: debug_msg((char *)"ILLEGAL tunnel!\n"); break;
  }
}


void DoNormal(struct Player *g)
{
  if(g->lastint!=G_EYES) {
    g->intel=G_SMART;  g->lastint=G_SMART;
    g->cur=g->old;
  }
}


void GoHome(struct Player *g)
{
  g->lastint=G_SMART;  g->intel=G_SMART;  g->homecnt=0;
  g->zx=g->sx*BITMAP_X;  g->zy=g->sy*BITMAP_Y;
  g->bx=g->sx;   g->by=g->sy;
  DoNormal(g);  g->mcnt=0;
}


void DoBlueGhost(struct Player *g, int bit)
{
  if((g->lastint!=G_EYES)&&(g->lastint!=G_HOME)) {
    g->old=g->cur;
    g->intel=G_SCARED;  g->lastint=G_SCARED;
  }
}


void DoMyBlue(void)
{
  DoBlueGhost(&Ghost0,SPR_SCARED);  DoBlueGhost(&Ghost1,SPR_SCARED);
  DoBlueGhost(&Ghost2,SPR_SCARED);  DoBlueGhost(&Ghost3,SPR_SCARED);
  BlueCount=BLUE_TIME;
}


void DoEyes(struct Player *g)
{
  g->intel=G_EYES;  g->lastint=G_EYES;
  NumPoints+=GhostEatCount;
  GhostEatCount*=2;
  UpdateScore();
}


void TheBlue(void)
{
    BlueCount--;
    if(BlueCount<=0) {
      DoNormal(&Ghost0);  DoNormal(&Ghost1);
      DoNormal(&Ghost2);  DoNormal(&Ghost3);
      GhostEatCount=100;
    }
}


void HomeIn(struct Player *g, int sx, int sy)
{
  if(g->intdir==GINT_HORIZ) {
    if(sy<g->zy) { g->nextdir=DIR_UP;    g->intdir=GINT_VERT; }
    if(sy>g->zy) { g->nextdir=DIR_DOWN;  g->intdir=GINT_VERT; }
  }
  else {
    if(sx<g->zx) { g->nextdir=DIR_LEFT;  g->intdir=GINT_HORIZ; }
    if(sx>g->zx) { g->nextdir=DIR_RIGHT; g->intdir=GINT_HORIZ; }
  }
}


void Avoid(struct Player *g, int sx, int sy)
{
  if(g->intdir==GINT_HORIZ) {
    if(sy>g->zy) { g->nextdir=DIR_UP;    g->intdir=GINT_VERT; }
    if(sy<g->zy) { g->nextdir=DIR_DOWN;  g->intdir=GINT_VERT; }
  }
  else {
    if(sx>g->zx) { g->nextdir=DIR_LEFT;  g->intdir=GINT_HORIZ; }
    if(sx<g->zx) { g->nextdir=DIR_RIGHT; g->intdir=GINT_HORIZ; }
  }
}


void Stupid(struct Player *g)
{
  switch(g->dir) {
    case DIR_UP:    g->nextdir=DIR_LEFT; break;
    case DIR_DOWN:  g->nextdir=DIR_RIGHT;  break;
    case DIR_LEFT:  g->nextdir=DIR_DOWN; break;
    case DIR_RIGHT: g->nextdir=DIR_UP;  break;
  }
}


void GhostChoose(struct Player *g)
{
  if(g->intel==G_TRYING) {
    g->intel=g->lastint;
    Stupid(g);
  } else
  if(g->intel==G_EYES) {
    g->intel=G_TRYING;  g->lastint=G_EYES;
    HomeIn(g,HomeX*BITMAP_X,HomeY*BITMAP_Y);
  } else
  if(g->intel==G_SMART) {
    g->intel=G_TRYING;  g->lastint=G_SMART;
    HomeIn(g,Pac.zx,Pac.zy);
  } else
  if(g->intel==G_SCARED) {
    g->intel=G_TRYING;  g->lastint=G_SCARED;
    Avoid(g,Pac.zx,Pac.zy);
  }
}


void MoveGhost(struct Player *g)
{
  int nx,ny,goo=0,dx=0,dy=0;
  bool Check=false;
  g->mc++;
  if(g->mc==g->speed) {
    g->mc=0;
    nx=g->zx;  ny=g->zy;
    if(g->mcnt==CountMax) Check=true;
    if(g->mcnt<0) Check=true;
    if(Check) {
      g->mcnt=0;  g->dir=g->nextdir;
    }
    if(g->lastint==G_SMART) {
      switch(g->dir) {
        case DIR_UP:    dy= -1; g->cur=SPR_GUP;    break;
        case DIR_DOWN:  dy=1;   g->cur=SPR_GDOWN;  break;
        case DIR_LEFT:  dx= -1; g->cur=SPR_GLEFT;  break;
        case DIR_RIGHT: dx=1;   g->cur=SPR_GRIGHT; break;
        default: break;
      }
    }
    if(g->lastint==G_SCARED) {
      switch(g->dir) {
        case DIR_UP:    dy= -1; g->cur=SPR_BUP;    break;
        case DIR_DOWN:  dy=1;   g->cur=SPR_BDOWN;  break;
        case DIR_LEFT:  dx= -1; g->cur=SPR_BLEFT;  break;
        case DIR_RIGHT: dx=1;   g->cur=SPR_BRIGHT; break;
        default: break;
      }
    }
    if(g->lastint==G_EYES) {
      switch(g->dir) {
        case DIR_UP:    dy= -1; g->cur=SPR_EUP;    break;
        case DIR_DOWN:  dy=1;   g->cur=SPR_EDOWN;  break;
        case DIR_LEFT:  dx= -1; g->cur=SPR_ELEFT;  break;
        case DIR_RIGHT: dx=1;   g->cur=SPR_ERIGHT; break;
        default: break;
      }
    }
    dx=dx*AspectX;  dy=dy*AspectY;
    if(Check) {
      DrawBoardPart(g->lx,g->ly,ReadBoard(g->lx,g->ly));
      g->lx=(nx+BITMAP_X/2)/BITMAP_X;  g->ly=(ny+BITMAP_Y/2)/BITMAP_Y;
      goo=CheckSpace(nx,ny,&dx,&dy);
      if(goo) {
        GhostChoose(g); g->mcnt=CountMax; MoveGhost(g);
      }
    }
    if(!goo) {
      if(FOOBAR_PACMOVE) DrawBoardPart(g->lx,g->ly,ReadBoard(g->lx,g->ly));
      nx+=dx;  ny+=dy;
      g->mcnt++;
#ifdef CATCH_OUT
      if(ny<0) { ny=1; g->dir=DIR_DOWN; }
      if(ny>SCREEN_Y) { ny=SCREEN_Y-2; g->dir=DIR_UP; }
      if(nx<0) { nx=1; g->dir=DIR_RIGHT; }
      if(nx>SCREEN_X) { nx=SCREEN_X-2; g->dir=DIR_LEFT; }
#endif
      //destw->draw(picts[g->cur],nx,ny);
      destw->copyBlock(srcs,picts[g->cur]->x,picts[g->cur]->y,nx,ny,BITMAP_X,BITMAP_Y);
      g->zx=nx;  g->zy=ny;
      g->bx=(nx+BITMAP_X/2)/BITMAP_X;  g->by=(ny+BITMAP_Y/2)/BITMAP_Y;
      if(g->lastint==G_EYES)
        if((g->bx==HomeX)&&(g->by==HomeY))
          { g->lastint=G_SMART; DoNormal(g); }
    }
  }
}


void MovePac(struct Player *P)
{
  int nx,ny,goo=0,dx=0,dy=0,part;
  bool Check=false;
  P->mc++;
  if(WakaFlag) { WakaFlag=0; P->cur=P->old; }
  else {
    WakaFlag=1;  P->old=P->cur;  P->cur=SPR_PACNONE;
    //destw->draw(picts[SPR_PACNONE],P->zx,P->zy);
    destw->copyBlock(srcs,picts[SPR_PACNONE]->x,picts[SPR_PACNONE]->y,P->zx,P->zy,BITMAP_X,BITMAP_Y);
  }
  if(P->mc==P->speed) {
    if((P->bx==0)&&(P->by==5)) { DoTunnel(P,LEFT_TUNNEL); } else
    if((P->bx==18)&&(P->by==5)) { DoTunnel(P,RIGHT_TUNNEL); }
    P->mc=0;
    nx=P->zx;  ny=P->zy;
    if(P->mcnt==CountMax) Check=true;
    if(P->mcnt<0) Check=true;
    if(Check) {
      P->mcnt=0;  P->dir=P->nextdir;
    }
    switch(P->dir) {
      case DIR_UP:    dy= -1; P->cur=SPR_PACUP; break;
      case DIR_DOWN:  dy=1;  P->cur=SPR_PACDOWN; break;
      case DIR_LEFT:  dx= -1; P->cur=SPR_PACLEFT; break;
      case DIR_RIGHT: dx=1;  P->cur=SPR_PACRIGHT; break;
      default: break;
    }
    dx=dx*AspectX;  dy=dy*AspectY;
    //dy=DirY[P->dir];  dx=DirX[P->dir];
    //P->cur=(*shapes)[P->dir];
    if(Check) {
      P->lx=(nx+BITMAP_X/2)/BITMAP_X;  P->ly=(ny+BITMAP_Y/2)/BITMAP_Y;
      part=ReadBoard(P->bx,P->by);
      if((part)==(int)'.') {
        WriteBoard(P->bx,P->by,(int)',');  NumDots--;  NumPoints+=1;
        UpdateScore();
      } else
      if((part)==(int)'*') {
        WriteBoard(P->bx,P->by,(int)',');  NumDots--;  NumPoints+=10;
        UpdateScore();  DoMyBlue();
      }
      /*P->lx=(nx+BITMAP_X/2)/BITMAP_X;  P->ly=(ny+BITMAP_Y/2)/BITMAP_Y;*/
      goo=CheckSpace(nx,ny,&dx,&dy);
      if(goo) {
        P->dir=DIR_STOP;  P->nextdir=DIR_STOP;
      }
    }
    if(!goo) {
      if(FOOBAR_PACMOVE)
        DrawBoardPart(P->lx,P->ly,ReadBoard(P->lx,P->ly));
      nx+=dx;  ny+=dy;
      P->mcnt++;
#ifdef CATCH_OUT
      if(ny<0) { ny=1; P->dir=DIR_DOWN; }
      if(ny>SCREEN_Y+BITMAP_X) { ny=SCREEN_Y-2; P->dir=DIR_UP; }
      if(nx<0) { nx=1; P->dir=DIR_RIGHT; }
      if(nx>SCREEN_X) { nx=SCREEN_X-2; P->dir=DIR_LEFT; }
#endif
      if(WakaFlag) {
        //destw->draw(picts[P->cur],nx,ny);
        destw->copyBlock(srcs,picts[P->cur]->x,picts[P->cur]->y,nx,ny,BITMAP_X,BITMAP_Y);
      }
      else {
        //destw->draw(picts[SPR_PACNONE],nx,ny);
        destw->copyBlock(srcs,picts[SPR_PACNONE]->x,picts[SPR_PACNONE]->y,nx,ny,BITMAP_X,BITMAP_Y);
      }
      P->zx=nx;  P->zy=ny;
      P->bx=(nx+BITMAP_X/2)/BITMAP_X;  P->by=(ny+BITMAP_Y/2)/BITMAP_Y;
    }
  }
}


void DoDie(void)
{//sound_beep(destw,0);
  NumMen--;
  if(NumMen<0) { QUITING=true;  DoGameOver(); }
  UpdateScore();
  WAITING=false;
  DIED=true;
  DrawScreen();
}


bool update(void)
{
  bool GotEm, GotEm0, GotEm1, GotEm2, GotEm3;
  MoveGhost(&Ghost0); MoveGhost(&Ghost1); MoveGhost(&Ghost2);
  MoveGhost(&Ghost3); MovePac(&Pac);
  if(BlueCount) TheBlue();
  GotEm0=false; GotEm1=false; GotEm2=false; GotEm3=false;
  if((Pac.bx==Ghost0.bx)&&(Pac.by==Ghost0.by)) { GotEm=true; GotEm0=true; }
  if((Pac.bx==Ghost1.bx)&&(Pac.by==Ghost1.by)) { GotEm=true; GotEm1=true; }
  if((Pac.bx==Ghost2.bx)&&(Pac.by==Ghost2.by)) { GotEm=true; GotEm2=true; }
  if((Pac.bx==Ghost3.bx)&&(Pac.by==Ghost3.by)) { GotEm=true; GotEm3=true; }
  if(GotEm) {
    if(GotEm0) {
      if(Ghost0.lastint==G_SCARED) DoEyes(&Ghost0);
      if(Ghost0.lastint==G_SMART) DoDie();
    }
    if(GotEm1) {
      if(Ghost1.lastint==G_SCARED) DoEyes(&Ghost1);
      if(Ghost1.lastint==G_SMART) DoDie();
    }
    if(GotEm2) {
      if(Ghost2.lastint==G_SCARED) DoEyes(&Ghost2);
      if(Ghost2.lastint==G_SMART) DoDie();
    }
    if(GotEm3) {
      if(Ghost3.lastint==G_SCARED) DoEyes(&Ghost3);
      if(Ghost3.lastint==G_SMART) DoDie();
    }
  }
  if(NumDots==0) {
    WAITING=false; DIED=false;
  }
  //update_window(destw);
  return GotEm;
}


bool mainloop(void)
{
  bool GotEm;
  AEvent myevent;
  debug_msg((char *)"DEBUG- Entering MAINLOOP...\n");
  WAITING=true;
  while(WAITING) {
    GotEm=update();
    //while(check_event(destw)) {
      myevent<<destw;
      //myevent.wait(*destw);
      switch(myevent.type) {
      case A_EVENT_KEYUP:
      if(myevent.code==(int)'i') Pac.nextdir=DIR_UP; else
      if(myevent.code==(int)'m') Pac.nextdir=DIR_DOWN; else
      if(myevent.code==(int)'j') Pac.nextdir=DIR_LEFT; else
      if(myevent.code==(int)'k') Pac.nextdir=DIR_RIGHT; else
      if(myevent.code==A_KEY_UP) Pac.nextdir=DIR_UP; else
      if(myevent.code==A_KEY_DOWN) Pac.nextdir=DIR_DOWN; else
      if(myevent.code==A_KEY_LEFT) Pac.nextdir=DIR_LEFT; else
      if(myevent.code==A_KEY_RIGHT) Pac.nextdir=DIR_RIGHT; else
      if(myevent.code==A_KEY_F1) {
	    AWindow *ww=new AWindow(srcs->getWidth(),srcs->getHeight());
	    ww->moveTo(0,0);
	    *ww<<srcs;
	    bool qq=true;
	    while(qq) {
	      myevent.wait(ww);
	      if(myevent.type==A_EVENT_DESTROY) qq=false;
	    }
	    ww->clear();
	    delete ww;
	    ww=(AWindow *)NULL;
	    DrawScreen();
	  }
	  else {
	    if(myevent.code==(int)'q') {
	      QUITING=true; WAITING=false;
	    }
	  }
	  //} else
	  //if(myevent.code==0) // ??? ;
	  //else
	  //if(myevent.code==(int)' ') console_msg((char *)"(space)\n");
	  //else debug_int(myevent.code);
	  break;
	case A_EVENT_MOUSEUP:
	  // Ignore it
	  break;
	case A_EVENT_MOVED:
	  // Ignore it
	  break;
	case A_EVENT_REDRAW:
	  DrawScreen(); break;
	case A_EVENT_RESIZE:
	  // Later scale or scroll here...
	  DrawScreen(); break;
	case A_EVENT_FOCUS:
	  // Unpause when we're active again.
	  break;
	case A_EVENT_UNFOCUS:
	  // Another window is active, pause us.
	  break;
	case A_EVENT_BREAK:
	  QUITING=true; WAITING=false; break;
	case A_EVENT_DESTROY:
	  QUITING=true; WAITING=false; break;
	case A_EVENT_UNKNOWN:
	  // This is for debugging only!
	  // debug_msg((char *)"@");
	  break;
	case A_EVENT_NONE:
	  // This is for debugging only!
	  // debug_msg((char *)"#");
	  break;
	default:
	  // debug_msg((char *)"Unknown event:");
	  //debug_int(myevent.type);
	  //debug_nl();
	  break;
      //}
    }
    //update_window(destw);
  }
  return GotEm;
}



