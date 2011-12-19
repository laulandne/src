
#include <iostream>

#include <guilib/AApp.h>

#include <portable/netlib/ANetwork.h>
#include <portable/netlib/AHost.h>
#include <portable/netlib/ASocketNonBlocking.h>

#include <guilib/AWindow.h>
#include <guilib/AEvent.h>
#include <guilib/ADialogMsg.h>
#include <guilib/ADialogAskString.h>

#include <portable/nickcpp/AStringHelper.h>


extern AApp aThisApp;
//extern ADebug std::cerr;


bool output=true;
bool wantOption=true;
unsigned int p=0;
unsigned char request[256];
unsigned char mesg[256];


void handleRequests(AWindow *w,ASocketNonBlocking *theSock,unsigned char *ostr);

void handleRequests(AWindow *w,ASocketNonBlocking *theSock,unsigned char *ostr)
{
  unsigned int n=theSock->recv(ostr,1);
  if(n!=ANET_SOCKET_ERROR) {
    //std::cerr<<"Received "<<aThisApp.myHex(ostr[0])<<" ("<<n<<" bytes)\n";
    bool understood=false;
    understood=false;
    if(ostr[0]==0xff) { 
      p=0; wantOption=false; output=false; understood=true; 
    }
    if(output) {
      ostr[1]=0;
      w->eraseCursor();
      *w<<(char *)ostr;
      w->drawCursor();
    }
    else {
      switch(ostr[0]) {
        case 0xfb: understood=true;  wantOption=true; break;
        case 0xfc: understood=true;  wantOption=true; break;
        case 0xfd: understood=true;  wantOption=true; break;
        case 0xfe: understood=true;  wantOption=true; break;
      }
      if(!understood) {
        if(!wantOption) std::cerr<<"Got code "<<aDefaultStringHelper.myHex(ostr[0])<<"?\n";
        else {
          wantOption=false;
          output=true;
          std::cerr<<"Got ";
          switch(request[1]) {
            case 0xfb: std::cerr<<"WILL "; break;
            case 0xfc: std::cerr<<"WON'T "; break;
            case 0xfd: std::cerr<<"DO "; break;
            case 0xfe: std::cerr<<"DON'T "; break;
          }
          std::cerr<<ostr[0]<<"\n";
          mesg[0]=0xff;
          if(request[1]==0xfb) { // We're asked WILL we...
            mesg[1]=0xfe;  // DON'T (We don't care what they WILL do!)
          }
          if(request[1]==0xfc) { // They WON'T...
            mesg[1]=0xfe;  // DON'T (We don't care what they WON'T do!)
          }
          if(request[1]==0xfd) { // We're asked DO we...
            mesg[1]=0xfc;  // WON'T (We refuse to DO anything!)
          }
          if(request[1]==0xfe) { // We're told to DON'T do something...
            mesg[1]=0xfc;  // WON'T (Don't worry, we DON'T anything!)
          }
          mesg[2]=ostr[0];
          std::cerr<<"Sending ";
          switch(mesg[1]) {
            case 0xfb: std::cerr<<"WILL "; break;
            case 0xfc: std::cerr<<"WON'T "; break;
            case 0xfd: std::cerr<<"DO "; break;
            case 0xfe: std::cerr<<"DON'T "; break;
          }
          std::cerr<<" "<<mesg[2]<<" mesg...\n";
          unsigned int n=theSock->send(mesg,3);
          //std::cerr<<"Sent "<<n<<" bytes\n";
        }
      }
      request[p]=ostr[0]; p++;
    }
  }
}


bool eventToString(AEvent &e,char *s);

bool eventToString(AEvent &e,char *s)
{
  if(e.type==A_EVENT_KEYUP) {
    //std::cerr<<"Got keypress "<<e.code<<"\n";
    s[0]=(char)e.code;
    s[1]=0;
    return true;
  }
  else {
    if(e.type!=A_EVENT_NONE) std::cerr<<"Tossed event type "<<(int)e.type<<"\n";
  }
  return false;
}


int main(int argc,char **argv)
{
#ifdef DEBUG_VERBOSE
  std::cerr<<"In main()...\n";
#endif // DEBUG_VERBOSE
  aThisApp.start(argc,argv);
#ifdef DEBUG_VERBOSE
  std::cerr<<"did start()...\n";
#endif // DEBUG_VERBOSE
  ANetwork *theNet=new ANetwork;
  if(theNet) {
  }
  else { std::cerr<<"Couldn't allocate theNet...\n";  return 5; }
#ifdef DEBUG_VERBOSE
  std::cerr<<"got theNet...\n";
#endif // DEBUG_VERBOSE
  unsigned int port=23;  // Standard telnet port
  char *name=(char *)NULL;
  if(aThisApp.argc>1) {
    name=aThisApp.argv[1];
  }
  if(aThisApp.argc>2) {
    unsigned int temp;
    sscanf(aThisApp.argv[2],(char *)"%d",&temp);
    port=temp;
  }
  if(!name) {
    std::cerr<<"No hostname given, going to ask for one...\n";
	char *theName=(char *)"NTelnet";
	char *theMsg=(char *)"Please choose a host to connect to via telnet";
    ADialogAskString *dialog=new ADialogAskString(theName,theMsg);
    if(dialog) {
      name=dialog->show(theName,theMsg);
    }
    if(!strcmp(name,"OK")) name=NULL;
    if(!name) {
      std::cerr<<"Still didn't get a name, going to default to localhost\n";
      name=(char *)"127.0.0.1";
    }
  }
#ifdef DEBUG_VERBOSE
  std::cerr<<"Looking for port "<<port<<" on "<<name<<"\n";
#endif // DEBUG_VERBOSE
  AHost *theHost=new AHost(theNet,name);
  if(theHost) {
  }
  else { std::cerr<<"Couldn't allocate theHost...\n";  return 5; }
#ifdef DEBUG_VERBOSE
  std::cerr<<"Got theHost\n";
#endif // DEBUG_VERBOSE
  ASocketNonBlocking *theSock=new ASocketNonBlocking(theHost,port);
  if(theSock) {
  }
  else { std::cerr<<"Couldn't allocate theSock...\n";  return 5; }
#ifdef DEBUG_VERBOSE
  std::cerr<<"Got theSock\n";
#endif // DEBUG_VERBOSE
  AWindow *w=new AWindow(name,320,200);
  if(w) {
  }
  else { std::cerr<<"Couldn't allocate w...\n";  return 5; }
  w->setForeground(0xffff00);
  w->setBackground(0x000000);
  w->clear();
  w->move(0,0);
  *w<<(char *)"Connected on port "<<port<<(char *)" to host "<<name<<(char *)"...\n";
  char istr[256];
  unsigned char ostr[256];
  istr[1]=0; ostr[1]=0;
  request[0]=0; mesg[0]=0;
  AEvent e;
  bool working=true;
  while(working) {
    e<<w;
    //e.wait(w);
    if(eventToString(e,istr)) {
      unsigned int n=theSock->send(istr,strlen(istr));
      if(n==ANET_SOCKET_ERROR) {
        ADialogMsg mw((char *)"Couldn't send message!");
        return 5;
      }
      //std::cerr<<"Sent "<<(int)(istr[0])<<" ("<<n<<" bytes)\n";
      *w<<istr[0];
    }
    handleRequests(w,theSock,ostr);
  }
  return 0;
}


