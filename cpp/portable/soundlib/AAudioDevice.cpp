
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdio.h>

#include <portable/nickcpp/ANullStream.h>


#include "AAudioDevice.h"
#include "ASound.h"
#include "ASong.h"


#ifdef ASYS_AMIGA
#include <exec/memory.h>
#include <exec/devices.h>
#include <devices/audio.h>
#ifdef __GNUC__
#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#else
#include <proto/exec.h>
#include <proto/alib.h>
#endif // __GNUC__
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_UMBRELLA_HEADERS
#else
#include <Traps.h>
#ifndef MAC_OLD_HEADERS
#include <Patches.h>
#endif // MAC_OLD_HEADERS
#endif // USE_UMBRELLA_HEADERS
#endif // ASYS_MAC

#ifdef ASYS_DOS32
#ifdef __SC__
#include <sound.h>
#endif // __SC__
#endif // ASYS_DOS32

#ifdef ASYS_WINDOWS
#define GET_PROC(mod,func) \
  GetProcAddress(GetModuleHandle(mod),func)
#ifdef ASYS_WIN32
#ifdef USE_DIRECTSOUND
static HRESULT (WINAPI *_nn_DirectSoundCreate)(GUID FAR *, LPDIRECTSOUND FAR *, IUnknown FAR *)=0;
#define DirectSoundCreate(x,y,z) _nn_DirectSoundCreate(x,y,z)
#endif // USE_DIRECTSOUND
#endif // ASYS_WIN32
#ifdef ASYS_WIN386
#define MMRESULT UINT
#endif // ASYS_WIN386
#ifdef ASYS_WIN16
#define MMRESULT UINT
#ifndef REALLY_OLD_WINDOWS
static UINT (WINAPI *_nn_waveOutGetDevCaps)(UINT, WAVEOUTCAPS FAR*,UINT)=0;
#define waveOutGetDevCaps(x,y,z)  _nn_waveOutGetDevCaps(x,y,z)
static UINT (WINAPI *_nn_waveOutGetErrorText)(UINT, LPSTR, UINT)=0;
#define waveOutGetErrorText(x,y,z)  _nn_waveOutGetErrorText(x,y,z)
static UINT (WINAPI *_nn_waveOutOpen)(HWAVEOUT FAR*, UINT,
    const WAVEFORMAT FAR*, DWORD, DWORD, DWORD)=0;
#define waveOutOpen(a,b,c,d,e,f)  _nn_waveOutOpen(a,b,c,d,e,f)
static UINT (WINAPI *_nn_waveOutClose)(HWAVEOUT)=0;
#define waveOutClose(x)  _nn_waveOutClose(x)
static UINT (WINAPI *_nn_waveOutPrepareHeader)(HWAVEOUT,WAVEHDR FAR*, UINT)=0;
#define waveOutPrepareHeader(x,y,z)  _nn_waveOutPrepareHeader(x,y,z)
static UINT (WINAPI *_nn_waveOutUnprepareHeader)(HWAVEOUT,WAVEHDR FAR*, UINT)=0;
#define waveOutUnprepareHeader(x,y,z)  _nn_waveOutUnprepareHeader(x,y,z)
static UINT (WINAPI *_nn_waveOutWrite)(HWAVEOUT, WAVEHDR FAR*,UINT)=0;
#define waveOutWrite(x,y,z)  _nn_waveOutWrite(x,y,z)
static UINT (WINAPI *_nn_waveOutReset)(HWAVEOUT)=0;
#define waveOutReset(x)  _nn_waveOutReset(x)
static UINT (WINAPI *_nn_midiOutGetDevCaps)(UINT,MIDIOUTCAPS FAR*, UINT)=0;
#define midiOutGetDevCaps(x,y,z)  _nn_midiOutGetDevCaps(x,y,z)
static UINT (WINAPI *_nn_midiOutOpen)(HMIDIOUT FAR*, UINT,DWORD, DWORD, DWORD)=0;
#define midiOutOpen(a,b,c,d,e)  _nn_midiOutOpen(a,b,c,d,e)
static UINT (WINAPI *_nn_midiOutClose)(HMIDIOUT)=0;
#define midiOutClose(x)  _nn_midiOutClose(x)
static UINT (WINAPI *_nn_midiOutShortMsg)(HMIDIOUT, DWORD)=0;
#define midiOutShortMsg(x,y)  _nn_midiOutShortMsg(x,y)
static UINT (WINAPI *_nn_midiOutReset)(HMIDIOUT)=0;
#define midiOutReset(x)  _nn_midiOutReset(x)
static DWORD (WINAPI *_nn_timeGetTime)()=0;
#define timeGetTime()  _nn_timeGetTime()
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN16
#endif // ASYS_WINDOWS


// TODO: This stuff really belongs in alib/defs.h
#ifdef ASYS_WIN32
#ifdef OLD_WINDOWS
#ifdef __WATCOMC__
#define A_LPBYTE LPSTR
#endif // __WATCOMC__
#ifndef A_LPBYTE
#define A_LPBYTE LPBYTE
#endif // A_LPBYTE
#define A_WAVEFORMATEX WAVEFORMAT
#else
#define A_LPBYTE LPSTR
#define A_WAVEFORMATEX WAVEFORMATEX
#endif // OLD_WINDOWS
#endif // ASYS_WIN32
#ifdef ASYS_WIN16
#define A_LPBYTE LPSTR
#define A_WAVEFORMATEX WAVEFORMAT
#endif // ASYS_WINDOWS


#ifdef ASYS_MAC
#ifdef MAC_USE_SOUND_MANAGER
static SndChannelPtr alib_mac_chan;
#endif
#if TARGET_RT_BIG_ENDIAN
#define makeSureBigEndianShort(x) (x)
#define makeSureBigEndianLong(x) (x)
#define makeSureBigEndianOSType(x) (x)
#define makeSureBigEndianFixed(x) (x)
#endif // TARGET_RT_BIG_ENDIAN
#endif // ASYS_MAC



////////////////////////////////////////////////////////////////////////////////
//  Global junk
////////////////////////////////////////////////////////////////////////////////

struct aSoundFormat {
  bool avail;
  unsigned long format;
  unsigned long rate;
  unsigned int bits;
  bool stereo;
};


#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
aSoundFormat nWndFormats[]={
  { false,WAVE_FORMAT_1M08,11025,8,false },  // 0
  { false,WAVE_FORMAT_1S08,11025,8,true },   // 1
  { false,WAVE_FORMAT_1M16,11025,16,false }, // 2
  { false,WAVE_FORMAT_1S16,11025,16,true },  // 3
  { false,WAVE_FORMAT_2M08,22500,8,false },  // 4
  { false,WAVE_FORMAT_2S08,22500,8,true },   // 5
  { false,WAVE_FORMAT_2M16,22500,16,false }, // 6
  { false,WAVE_FORMAT_2S16,22500,16,true },  // 7
  { false,WAVE_FORMAT_4M08,44100L,8,false }, // 8
  { false,WAVE_FORMAT_4S08,44100L,8,true },  // 9
  { false,WAVE_FORMAT_4M16,44100L,16,false },// a
  { false,WAVE_FORMAT_4S16,44100L,16,true }, // b
  { false,0,0,0,0 }
};
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS


////////////////////////////////////////////////////////////////////////////////
//  AAudioDevice Class
////////////////////////////////////////////////////////////////////////////////

AAudioDevice::AAudioDevice()
{
  DEBUG_OUT<<"AAudioDevice()::AAudioDevice\n";
  init();
  open();
}


void AAudioDevice::init()
{
  // Assume we're mono at first...
  stereo=false;
  bool flag=false;
  // We assume we can't handle anything until we find out otherwise...
  doesWaves=false;
  doesMidi=false;
  patch=0;
#ifdef ASYS_WIN16
#ifndef REALLY_OLD_WINDOWS
  mmSystemHandle=(HINSTANCE)NULLHANDLE;
  mmSystemHandle=LoadLibrary("mmsystem.dll");
  if((int)mmSystemHandle<33) {
    DEBUG_OUT<<"Couldn't load mmsystem.dll!\n";
    return;
  }
  // First dynamically resolve the functions we need
  if(!_nn_waveOutGetDevCaps) {
    _nn_waveOutGetDevCaps=(UINT (WINAPI *)(UINT, WAVEOUTCAPS FAR*,UINT))GET_PROC("MMSYSTEM","WAVEOUTGETDEVCAPS");
    if(!_nn_waveOutGetDevCaps) {
      DEBUG_OUT<<"Couldn't getProc waveOutGetDevCaps!\n";
    }
  }
  if(!_nn_waveOutGetErrorText) {
    _nn_waveOutGetErrorText=(UINT (WINAPI *)(UINT, LPSTR, UINT))GET_PROC("MMSYSTEM","WAVEOUTGETERRORTEXT");
    if(!_nn_waveOutGetErrorText) {
      DEBUG_OUT<<"Couldn't getProc waveOutGetErrorText!\n";
    }
  }
  if(!_nn_waveOutOpen) {
    _nn_waveOutOpen=(UINT (WINAPI *)(HWAVEOUT FAR*, UINT,const WAVEFORMAT FAR*, DWORD, DWORD, DWORD))GET_PROC("MMSYSTEM","WAVEOUTOPEN");
    if(!_nn_waveOutOpen) {
      DEBUG_OUT<<"Couldn't getProc waveOutOpen!\n";
    }
  }
  if(!_nn_waveOutClose) {
    _nn_waveOutClose=(UINT (WINAPI *)(HWAVEOUT))GET_PROC("MMSYSTEM","WAVEOUTCLOSE");
    if(!_nn_waveOutClose) {
      DEBUG_OUT<<"Couldn't getProc waveOutClose!\n";
    }
  }
  if(!_nn_waveOutPrepareHeader) {
    _nn_waveOutPrepareHeader=(UINT (WINAPI *)(HWAVEOUT,WAVEHDR FAR*, UINT))GET_PROC("MMSYSTEM","WAVEOUTPREPAREHEADER");
    if(!_nn_waveOutPrepareHeader) {
      DEBUG_OUT<<"Couldn't getProc waveOutPrepareHeader!\n";
    }
  }
  if(!_nn_waveOutUnprepareHeader) {
    _nn_waveOutUnprepareHeader=(UINT (WINAPI *)(HWAVEOUT,WAVEHDR FAR*, UINT))GET_PROC("MMSYSTEM","WAVEOUTUNPREPAREHEADER");
    if(!_nn_waveOutUnprepareHeader) {
      DEBUG_OUT<<"Couldn't getProc waveOutUnprepareHeader!\n";
    }
  }
  if(!_nn_waveOutWrite) {
    _nn_waveOutWrite=(UINT (WINAPI *)(HWAVEOUT,WAVEHDR FAR*, UINT))GET_PROC("MMSYSTEM","WAVEOUTWRITE");
    if(!_nn_waveOutWrite) {
      DEBUG_OUT<<"Couldn't getProc waveOutWrite!\n";
    }
  }
  if(!_nn_waveOutReset) {
    _nn_waveOutReset=(UINT (WINAPI *)(HWAVEOUT))GET_PROC("MMSYSTEM","WAVEOUTRESET");
    if(!_nn_waveOutReset) {
      DEBUG_OUT<<"Couldn't getProc waveOutReset!\n";
    }
  }
  if(!_nn_midiOutGetDevCaps) {
    _nn_midiOutGetDevCaps=(UINT (WINAPI *)(UINT,MIDIOUTCAPS FAR*, UINT))GET_PROC("MMSYSTEM","MIDIOUTGETDEVCAPS");
    if(!_nn_midiOutGetDevCaps) {
      DEBUG_OUT<<"Couldn't getProc midiOutGetDevCaps!\n";
    }
  }
  if(!_nn_midiOutOpen) {
    _nn_midiOutOpen=(UINT (WINAPI *)(HMIDIOUT FAR*, UINT,DWORD, DWORD, DWORD))GET_PROC("MMSYSTEM","MIDIOUTOPEN");
    if(!_nn_midiOutOpen) {
      DEBUG_OUT<<"Couldn't getProc midiOutOpen!\n";
    }
  }
  if(!_nn_midiOutClose) {
    _nn_midiOutClose=(UINT (WINAPI *)(HMIDIOUT))GET_PROC("MMSYSTEM","MIDIOUTCLOSE");
    if(!_nn_midiOutClose) {
      DEBUG_OUT<<"Couldn't getProc midiOutClose!\n";
    }
  }
  if(!_nn_midiOutShortMsg) {
    _nn_midiOutShortMsg=(UINT (WINAPI *)(HMIDIOUT,DWORD))GET_PROC("MMSYSTEM","MIDIOUTSHORTMSG");
    if(!_nn_midiOutShortMsg) {
      DEBUG_OUT<<"Couldn't getProc midiOutShortMsg!\n";
    }
  }
  if(!_nn_midiOutReset) {
    _nn_midiOutReset=(UINT (WINAPI *)(HMIDIOUT))GET_PROC("MMSYSTEM","MIDIOUTRESET");
    if(!_nn_midiOutReset) {
      DEBUG_OUT<<"Couldn't getProc midiOutReset!\n";
    }
  }
  if(!_nn_timeGetTime) {
    _nn_timeGetTime=(DWORD (WINAPI *)())GET_PROC("MMSYSTEM","TIMEGETTIME");
    if(!_nn_timeGetTime) {
      DEBUG_OUT<<"Couldn't getProc timeGetTime!\n";
    }
  }
#endif  // REALLY_OLD_WINDOWS
#endif  // ASYS_WIN16
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
  // Now that we've resolved the linking, lets get busy...
  MMRESULT res=1;
  // open midi device #0
  // TODO: Pick the device that the user selected, not just the first one...
  MIDIOUTCAPS mcaps;
#ifdef ASYS_WIN16
  if(_nn_midiOutGetDevCaps) {
#endif  // ASYS_WIN16
    res=midiOutGetDevCaps(0,&mcaps,sizeof(MIDIOUTCAPS));
#ifdef ASYS_WIN16
  }
#endif  // ASYS_WIN16
  if(res) DEBUG_OUT<<"Something went wrong ("<<res<<") getting midi out caps!\n";
  else {
    DEBUG_OUT<<"Midi out device #0 is: \n"<<mcaps.szPname;
    switch(mcaps.wTechnology) {
      case MOD_MIDIPORT:  DEBUG_OUT<<" - midi hardware port"; break;
      case MOD_SYNTH:  DEBUG_OUT<<" - internal synth"; break;
      case MOD_SQSYNTH:  DEBUG_OUT<<" - square wave synth"; break;
      case MOD_FMSYNTH:  DEBUG_OUT<<" - fm synth"; break;
      case MOD_MAPPER:  DEBUG_OUT<<" - midi mapper"; break;
      default:
        DEBUG_OUT<<" - unknown technology ("<<(int)mcaps.wTechnology<<")"; break;
    }
    DEBUG_OUT<<"\n";
    DEBUG_OUT<<"voices: "<<(int)mcaps.wVoices<<"  notes: "<<(int)mcaps.wNotes;
    DEBUG_OUT<<"  channelMask: "<<(int)mcaps.wChannelMask<<"\n";
    midiHand=(HMIDIOUT)NULLHANDLE;
    res=midiOutOpen(&midiHand,0,(unsigned long)NULL,(unsigned long)NULL,CALLBACK_NULL);
    if(res) {
      DEBUG_OUT<<"Something went wrong ("<<res<<") opening midi out!\n";
      flag=true;
    }
    else {
      DEBUG_OUT<<"midiOut opened okay.\n";
      doesMidi=true;
    }
  }
#ifdef USE_DIRECTSOUND
  ddSound=NULL;
  doesWaves=true;
  dSoundHandle=LoadLibrary(ATEXT("dsound.dll"));
  if(dSoundHandle) {
    if(!_nn_DirectSoundCreate) {
      _nn_DirectSoundCreate=(HRESULT (WINAPI *)(GUID FAR *, LPDIRECTSOUND FAR *, IUnknown FAR *))GET_PROC(ATEXT("dsound"),"DirectSoundCreate");
      if(!_nn_DirectSoundCreate) {
        DEBUG_OUT<<"Couldn't getProc DirectSoundCreate!\n";
      }
      DEBUG_OUT<<"Using DirectSound for wave output.\n";
    }
  }
#else
  // open wave device (let system pick)
  WAVEOUTCAPS wcaps;
  res=1;
#ifdef ASYS_WIN16
  if(_nn_waveOutGetDevCaps) {
#endif // ASYS_WIN16
    res=waveOutGetDevCaps((UINT)WAVE_MAPPER,&wcaps,sizeof(WAVEOUTCAPS));
#ifdef ASYS_WIN16
  }
#endif // ASYS_WIN16
  if(res) DEBUG_OUT<<"Something went wrong ("<<res<<") getting wave out caps!\n";
  else {
    DEBUG_OUT<<"WAVE_MAPPER is: \n"<<wcaps.szPname<<"\n";
    DEBUG_OUT<<"format mask: "<<(int)wcaps.dwFormats<<"  channels: "<<(int)wcaps.wChannels<<"\n";
    unsigned int t;
    for(t=0;t<0xc;t++) if(wcaps.dwFormats&(1<<t)) nWndFormats[t].avail=true;
    DEBUG_OUT<<"Availiable formats:\n";
    for(t=0;t<0xc;t++)
      if(nWndFormats[t].avail) {
        DEBUG_OUT<<t<<": "<<nWndFormats[t].rate<<" ";
        DEBUG_OUT<<nWndFormats[t].bits<<" "<<nWndFormats[t].stereo<<"\n";
      }
    DEBUG_OUT<<"pitch change? ";  if(wcaps.dwSupport&WAVECAPS_PITCH) DEBUG_OUT<<"Yes\n"; else DEBUG_OUT<<"No\n";
    DEBUG_OUT<<"rate change? ";  if(wcaps.dwSupport&WAVECAPS_PLAYBACKRATE) DEBUG_OUT<<"Yes\n"; else DEBUG_OUT<<"No\n";
    doesWaves=true;
  }
#endif // USE_DIRECTSOUND
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
#ifdef USE_QUICKTIME
  if(alib_qtVer<2) {
    DEBUG_OUT<<"MIDI support requires QuickTime 2 or better...sorry!\n";
    flag=true;
  }
  else {
#ifdef MAC_OLD_HEADERS
    theNA=OpenDefaultComponent(kNoteAllocatorType,0);
    //theNA=OpenDefaultComponent('nota',0);
    myNR.polyphony=1;
    myNR.typicalPolyphony=1<<16;
#else
#ifdef MAC_USE_QT_MUSIC
    theNA=OpenDefaultComponent(kNoteAllocatorComponentType,0);
    // TODO: Check for fail?
    myNR.info.polyphony=makeSureBigEndianShort(1);
    myNR.info.typicalPolyphony=makeSureBigEndianFixed(1<<16);
    myNR.info.flags=0;
#ifndef USE_UMBRELLA_HEADERS
    myNR.info.reserved=0;
#endif // USE_UMBRELLA_HEADERS
#endif
#endif // MAC_OLD_HEADERS
#ifdef MAC_USE_QT_MUSIC
    myNR.tone.synthesizerType=makeSureBigEndianOSType(0);
    myNR.tone.instrumentNumber=makeSureBigEndianLong(0);
    myNR.tone.gmNumber=makeSureBigEndianLong(1);
    myNR.tone.synthesizerName[0]=0;
    myNR.tone.instrumentName[0]=0;
    NANewNoteChannel(theNA,&myNR,&myNC);
    // TODO: Check for fail?
    NAPrerollNoteChannel(theNA,myNC);
#endif
    // TODO: Check for fail?
    DEBUG_OUT<<"Using QuickTime musical instruments.\n";
    doesMidi=true;
  }
#endif // USE_QUICKTIME
  void *trap1=NULL,*trap2=NULL;
  bool sndAvail=true;
#ifdef REALLY_OLD_MAC
  trap1=(void *)GetToolTrapAddress(_SoundDispatch);
  trap2=(void *)GetToolTrapAddress(_Unimplemented);
  if(trap1==trap2) sndAvail=false;
#endif // REALLY_OLD_MAC
  if(sndAvail) {
    NumVersion nver;
#ifdef MAC_USE_SOUND_MANAGER
    nver=SndSoundManagerVersion();
#endif
    if(nver.majorRev>=3) {
      DEBUG_OUT<<"Sound Manager 3 or better detected.\n";
#ifdef MAC_USE_SOUND_MANAGER
      OSErr err=SndNewChannel(&alib_mac_chan,sampledSynth,initMono,(SndCallBackUPP)NULL);
#endif
      // TODO: check for fail?
      doesWaves=true;
    }
  }
  if(!doesWaves) {
    DEBUG_OUT<<"Wave support requires Sound Manager 3 or better...sorry!\n";
    flag=true;
  }
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  doesWaves=true;
  flag=false;
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  doesWaves=true;
  flag=false;
#endif // ASYS_X11
#ifdef ASYS_DOS
  flag=true;
#endif // ASYS_DOS
#ifdef ASYS_OS2
  flag=true;
#endif // ASYS_OS2
  if(doesMidi) setPatch(0);
}


AAudioDevice::~AAudioDevice()
{
  DEBUG_OUT<<"AAudioDevice()::~AAudioDevice\n";
  stop();
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
  if(doesMidi) if(midiHand) midiOutClose(midiHand);
  midiHand=(HMIDIOUT)NULLHANDLE;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN32CE
#ifdef USE_DIRECTSOUND
  if(ddSound) ddSound->Release();
  ddSound=NULL;
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
#ifdef ASYS_WIN16
#ifndef REALLY_OLD_WINDOWS
  if((int)mmSystemHandle>32) FreeLibrary(mmSystemHandle);
  mmSystemHandle=(HINSTANCE)NULLHANDLE;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN16
#ifdef ASYS_MAC
  if(doesMidi) {
#ifdef MAC_USE_QTMUSIC
    NADisposeNoteChannel(theNA,myNC);
    CloseComponent(theNA);
#endif // MAC_USE_QTMUSIC
  }
  if(doesWaves) {
#ifdef MAC_USE_QTMUSIC
    cmd.cmd=flushCmd;
    SndDoImmediate(alib_mac_chan,&cmd);
    SndDisposeChannel(alib_mac_chan,true);
#endif // MAC_USE_QTMUSIC
  }
#endif // ASYS_MAC
  //if(this==aThisApp.defaultAudioDevice) aThisApp.defaultAudioDevice=(AAudioDevice *)NULL;
}


void AAudioDevice::open()
{
  DEBUG_OUT<<"AAudioDevice()::open\n";
  // TODO: a lot of stuff in init() should be here...
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  if(!dSoundHandle) return;
  if(!_nn_DirectSoundCreate) return;
  unsigned long err=DirectSoundCreate(NULL,&ddSound,NULL);
  if(err==DS_OK) {
    DEBUG_OUT<<"DirectSound created.\n";
    // NOTE: Because we use ahWnd, the last window opened must still
    // be valid!
    err=ddSound->SetCooperativeLevel(alib_ahWnd,DSSCL_PRIORITY);
    if(err==DS_OK) DEBUG_OUT<<"SetCooperativeLevel(...) okay.\n";
    else {
      DEBUG_OUT<<"Error SetCooperativeLevel!\n";
      ddSound=NULL;  // so we don't use it accidentally...
      doesWaves=false;
    }
  }
  else {
    DEBUG_OUT<<"Error creating DirectSound!\n";
    ddSound=NULL;  // so we don't use it accidentally...
    doesWaves=false;
  }
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
}


void AAudioDevice::stop()
{
  DEBUG_OUT<<"AAudioDevice()::stop\n";
  // TODO: stop all playing sound (if any)
}


void AAudioDevice::play(ASound *snd, unsigned long pitch)
{
  DEBUG_OUT<<"AAudioDevice()::play\n";
  if(!snd) { DEBUG_OUT<<"no sound!\n"; return; }
  if(!snd->getSamples()) { DEBUG_OUT<<"(No samples)\n"; return; }
  if(!snd->getNumSamples()) { DEBUG_OUT<<"(numSamp=0)\n"; return; }
  if(!doesWaves) { DEBUG_OUT<<"Sorry, device can't handle digital sound data!\n"; return; }
  // TODO: Is this right?
  pitch=(int)snd->getSampleRate();
  // play the sound...
#ifdef ASYS_X11
  // TODO: This doesn't actually work for most things...
  FILE *fd=fopen("/dev/audio","w");
  if(fd) {
    fwrite(snd->getSamples(),snd->getNumSamples(),1,fd);
    fclose(fd);
  }
  else {
    //DEBUG_OUT.warning("Couldn't open /dev/audio!\n");
  }
#endif // ASYS_X11
#ifdef ASYS_AMIGA
  struct IOAudio s;
  unsigned char unit[]={ 1,2,4,8 };
  struct MsgPort *myPort=(struct MsgPort *)CreatePort("AlibAudio",0);
  struct Message *msg;
  if(!myPort) {
    DEBUG_OUT<<"Couldn't create audio port!\n";
    return;
  }
  DEBUG_OUT<<"Created port.\n";
  s.ioa_Request.io_Message.mn_ReplyPort=myPort;
  s.ioa_Request.io_Message.mn_Node.ln_Pri=126;
  s.ioa_Request.io_Command=ADCMD_ALLOCATE;
  s.ioa_Request.io_Flags=ADIOF_NOWAIT;
  s.ioa_AllocKey=0;
  s.ioa_Data=(unsigned char *)unit;p
  s.ioa_Length=sizeof(unit);
  int response=OpenDevice((unsigned char *)"audio.device",0,(IORequest *)&s,0);
  if(response) {
    DEBUG_OUT<<"Couldn't open the audio device!\n";
    DeletePort(myPort);  return;
  }
  DEBUG_OUT<<"Opened device.\n";
  s.ioa_Request.io_Message.mn_ReplyPort=myPort;
  s.ioa_Request.io_Command=CMD_WRITE;
  s.ioa_Request.io_Flags=ADIOF_PERVOL;
  s.ioa_Data=snd->getSamples();
  s.ioa_Cycles=1;
  s.ioa_Length=snd->getNumSamples();
  s.ioa_Period=pitch;  // TODO: probably needs correcting
  s.ioa_Volume=64;
  DEBUG_OUT<<"Going to play.\n";
  BeginIO((IORequest *)&s);
  Wait(1<<myPort->mp_SigBit);
  msg=GetMsg(myPort);
  DEBUG_OUT<<"Shutting audio down.\n";
  CloseDevice((IORequest *)&s);
  DeletePort(myPort);
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef MAC_USE_SOUND_MANAGER
  hdr.samplePtr=(Ptr)snd->getSamples();
  hdr.numChannels=1;
  if(snd->getStereo()) hdr.numChannels=2;
  hdr.sampleRate=pitch<<16;
  hdr.encode=extSH;
  hdr.numFrames=snd->getNumSamples();
  if(snd->getStereo()) hdr.numFrames/=2;
  if(snd->getNumBits()==16) hdr.numFrames/=2;
  hdr.sampleSize=snd->getNumBits();
  cmd.param1=0;
  cmd.param2=(long)&hdr;
  cmd.cmd=bufferCmd;
  OSErr err=SndDoCommand(alib_mac_chan,&cmd,false);
#endif
  // Check for fail?
  unsigned int len=snd->calcPlayingTime();
  unsigned long tickc=TickCount()+(len/16);
  while(tickc>TickCount()) {};
#ifdef MAC_USE_SOUND_MANAGER
  cmd.param1=0;
  cmd.param2=0;
  cmd.cmd=quietCmd;
  SndDoImmediate(alib_mac_chan,&cmd);
  cmd.cmd=flushCmd;
  SndDoImmediate(alib_mac_chan,&cmd);
#endif
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
  MMRESULT res;
  char s[256];
  HWAVEOUT waveHand;
#ifndef OLD_WINDOWS
  WAVEFORMATEX wf;
#else
  WAVEFORMAT wf;
#endif // OLD_WINDOWS
  wf.wFormatTag=WAVE_FORMAT_PCM;
  if(snd->getStereo()) wf.nChannels=2; else wf.nChannels=1;
  wf.nSamplesPerSec=snd->getSampleRate();
  wf.nAvgBytesPerSec=snd->getSampleRate();
  if(snd->getStereo()) wf.nAvgBytesPerSec*=2;
  if(snd->getNumBits()>8) wf.nAvgBytesPerSec*=2;
  wf.nBlockAlign=1;
  if(snd->getStereo()) wf.nBlockAlign*=2;
  if(snd->getNumBits()>8) wf.nBlockAlign*=2;
#ifndef OLD_WINDOWS
  wf.wBitsPerSample=snd->getNumBits();
  wf.cbSize=0;
#endif // OLD_WINDOWS
  res=waveOutOpen(&waveHand,(UINT)WAVE_MAPPER,(A_WAVEFORMATEX *)&wf,(unsigned long)NULL,(unsigned long)NULL,WAVE_ALLOWSYNC|CALLBACK_NULL);
  HGLOBAL hdata=GlobalAlloc(GMEM_MOVEABLE,snd->getNumSamples());
  if(!hdata) { DEBUG_OUT<<"Couldn't global alloc!\n"; return; }
  char *data=(char *)GlobalLock(hdata);
  if(!data) { DEBUG_OUT<<"Couldn't global lock!\n"; return; }
  memcpy(data,snd->getSamples(),(int)snd->getNumSamples());
  WAVEHDR hdr;
  hdr.lpData=(A_LPBYTE)data;
  hdr.dwBufferLength=snd->getNumSamples();
  hdr.dwFlags=0;
  hdr.dwLoops=0;
  res=waveOutPrepareHeader(waveHand,&hdr,sizeof(WAVEHDR));
  if(res) { waveOutGetErrorText(res,s,256); ADialogMsg mw(s); }
  res=waveOutWrite(waveHand,&hdr,sizeof(WAVEHDR));
  if(res) { waveOutGetErrorText(res,s,256); ADialogMsg mw(s); }
  unsigned long len=snd->calcPlayingTime();
  // FIX: Wait some...really braindead right now...
  unsigned long start=timeGetTime();
  unsigned long end=timeGetTime();
  while((end-start)<len) end=timeGetTime();
  res=waveOutReset(waveHand);
  if(res) { waveOutGetErrorText(res,s,256); ADialogMsg mw(s); }
  res=waveOutUnprepareHeader(waveHand,&hdr,sizeof(WAVEHDR));
  if(res) { waveOutGetErrorText(res,s,256); ADialogMsg mw(s); }
  GlobalUnlock(hdata);
  GlobalFree(hdata);
  res=waveOutClose(waveHand);
  if(res) { waveOutGetErrorText(res,s,256); ADialogMsg mw(s); }
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS
}


void AAudioDevice::play(ASong *s)
{
  DEBUG_OUT<<"AAudioDevice()::play\n";
  if(!s) { DEBUG_OUT<<"no song!\n"; return; }
  // TODO: check song for any other things that can go wrong...
  // TODO: play the song...
}


unsigned int AAudioDevice::lookupKeyByFreq(unsigned long freq)
{
  // TODO: not implemented yet...
  return 0;
}


unsigned int AAudioDevice::lookupFreqByKey(unsigned int key)
{
  // TODO: not implemented yet...
  return 0;
}


// pitch is in hz, len is in millisec
void AAudioDevice::playTone(unsigned long pitch,unsigned int len)
{
  DEBUG_OUT<<"AAudioDevice()::playTone\n";
#ifdef ASYS_DOS32
  int freq=(int)(pitch*1.331);
#ifdef __SC__
  sound_beep(freq);
#endif
  //sound_tone(cycles,uptime,dntime);
#endif
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
  playNote(lookupKeyByFreq(pitch),len);
#endif
#endif
#endif
#ifdef ASYS_MAC
  playNote(lookupKeyByFreq(pitch),len);
#endif
}


// keys are 0-127, len is in millisec, vel is 0-127, chan is 0-15
void AAudioDevice::playNote(unsigned int key,unsigned int len,
  unsigned int vel, unsigned int chan)
{
  DEBUG_OUT<<"AAudioDevice()::playNote\n";
  if(!doesMidi) { DEBUG_OUT<<"Sorry, device can't handle midi sound data!\n"; return; }
#ifdef ASYS_DOS32
  playTone(lookupFreqByKey(key),len);
#endif
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
  MMRESULT res;
  union {
    DWORD dwData;
    BYTE bData[4];
  } u;
  // Note on, veloticy is 64
  u.bData[0]=(BYTE)(0x90+(chan&0xf));
  u.bData[1]=(BYTE)(key&0x7f);
  u.bData[2]=(BYTE)(vel&0x7f);
  u.bData[3]=0;
  midiOutShortMsg(midiHand,u.dwData);
  // FIX: Wait some...really braindead right now...
  unsigned long start=timeGetTime();
  unsigned long end=timeGetTime();
  while((end-start)<len) end=timeGetTime();
  // Note on, veloticy 0, really a note off
  u.bData[0]=(BYTE)(0x90+(chan&0xf));
  u.bData[1]=(BYTE)(key&0x7f);
  u.bData[2]=0;
  u.bData[3]=0;
  midiOutShortMsg(midiHand,u.dwData);
  res=midiOutReset(midiHand);
  if(res) DEBUG_OUT<<"Couldn't reset midiout ("<<res<<")!\n";
#endif
#endif
#endif
#ifdef ASYS_MAC
#ifdef USE_QUICKTIME
  // rough millisec to tick conversion...
#ifdef MAC_USE_QT_MUSIC
  NAPlayNote(theNA,myNC,key&0x7f,vel&0x7f);
#endif
  unsigned long tickc=TickCount()+(len/16);
  while(tickc>TickCount()) {};
#ifdef MAC_USE_QT_MUSIC
  NAPlayNote(theNA,myNC,key&0x7f,0);
#endif
#endif
#endif
}


/*

midi messages, first nibble of status bit

8 = Note Off
9 = Note On
A = AfterTouch (ie, key pressure)
B = Control Change
C = Program (patch) change
D = Channel Pressure
E = Pitch Wheel

*/


// Patches are 0-127, chan is 0-15
void AAudioDevice::setPatch(unsigned int num, unsigned int chan)
{
  DEBUG_OUT<<"AAudioDevice()::setPatch\n";
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
  if(midiHand) {
    union {
      DWORD dwData;
      BYTE bData[4];
    } u;
    // patch change...
    u.bData[0]=(BYTE)(0xc0+(chan&0xf));
    u.bData[1]=(BYTE)(num&0x7f);
    u.bData[2]=0;
    u.bData[3]=0;
    midiOutShortMsg(midiHand,u.dwData);
  }
#endif
#endif
#endif
#ifdef ASYS_MAC
#ifdef USE_QUICKTIME
  if(doesMidi) {
#ifdef MAC_USE_QT_MUSIC
    NADisposeNoteChannel(theNA,myNC);
#ifdef MAC_OLD_HEADERS
    myNR.polyphony=1;
    myNR.typicalPolyphony=1<<16;
#else
    myNR.info.polyphony=makeSureBigEndianShort(1);
    myNR.info.typicalPolyphony=makeSureBigEndianFixed(1<<16);
    myNR.info.flags=0;
#ifndef USE_UMBRELLA_HEADERS
    myNR.info.reserved=0;
#endif // USE_UMBRELLA_HEADERS
#endif // MAC_OLD_HEADERS
    myNR.tone.synthesizerType=makeSureBigEndianOSType(0);
    myNR.tone.instrumentNumber=makeSureBigEndianLong(0);
    myNR.tone.gmNumber=makeSureBigEndianLong((num&0x7f)+1);
    myNR.tone.synthesizerName[0]=0;
    myNR.tone.instrumentName[0]=0;
    NANewNoteChannel(theNA,&myNR,&myNC);
    // TODO: Check for fail?
    NAPrerollNoteChannel(theNA,myNC);
#endif
  }
#endif
#endif
  patch=num;
}


// TODO: implement this
bool AAudioDevice::verifyOkay(ASong *s)
{
  DEBUG_OUT<<"AAudioDevice()::verifyOkay\n";
  bool ret=false;
  //
  return ret;
}


// TODO: Make more general, like ADisplay:query(), etc
bool AAudioDevice::verifyOkay(ASound *s)
{
  DEBUG_OUT<<"AAudioDevice()::verifyOkay\n";
  unsigned long targetRate=s->getSampleRate();
  unsigned int targetBits=s->getNumBits();
  bool st=s->getStereo();
#ifdef ASYS_MAC
  if(s->getNumBits()>12) targetBits=16;
  if(s->getNumBits()<13) targetBits=8;
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
  aSoundFormat *choice=(aSoundFormat *)NULL;
  DEBUG_OUT<<"Searching for nearest playback for "<<s->getSampleRate()<<" "<<s->getNumBits()<<" ";
  DEBUG_OUT<<st<<"...\n";
  // look for best match
  DEBUG_OUT<<"Looking for best match...\n";
  for(unsigned int t=0;t<0xc;t++) {
  long signedRateDiff=(long)(s->getSampleRate()-nWndFormats[t].rate);
    unsigned long rateDiff=labs(signedRateDiff);
    // If we fit arbitrarily close to the desired one, just let it go...
    if((rateDiff<1010)&&(s->getNumBits()==nWndFormats[t].bits)) choice=&nWndFormats[t];
  }
  // No best match, find closest with # of bits...
  if(!choice) {
    DEBUG_OUT<<"Now looking for closest, ignoring rate...\n";
    for(unsigned int t=0;t<0xc;t++) {
      if(nWndFormats[t].bits==s->getNumBits()) choice=&nWndFormats[t];
    }
  }
  // No closest with # of bits..., find closest rate...
  if(!choice) {
    DEBUG_OUT<<"Now looking for closest, ignoring bits...\n";
    for(unsigned int t=0;t<0xc;t++) {
      long signedRateDiff=(long)(s->getSampleRate()-nWndFormats[t].rate);
      unsigned long rateDiff=labs(signedRateDiff);
      // If we fit arbitrarily close to the desired one, just let it go...
      if(rateDiff<1010) choice=&nWndFormats[t];
    }
  }
  if(!choice) { DEBUG_OUT<<"Couldn't find a match!  Giving up!\n";  return false; }
  targetRate=choice->rate;  targetBits=choice->bits;  st=choice->stereo;
  DEBUG_OUT<<"Choice is "<<targetRate<<" "<<targetBits<<" "<<st<<"\n";
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN32CE
#endif // ASYS_WINDOWS
  return s->convert(targetRate,targetBits,st);
}


#ifdef ASYS_MAC
#if TARGET_RT_LITTLE_ENDIAN
BigEndianFixed AAudioDevice::makeSureBigEndianFixed(unsigned long val)
{
  bf.bigEndianValue=EndianS32_NtoB(val);
  return bf;
}
#endif // TARGET_RT_LITTLE_ENDIAN
#endif // ASYS_MAC


#ifdef ASYS_MAC
#if TARGET_RT_LITTLE_ENDIAN
BigEndianShort AAudioDevice::makeSureBigEndianShort(unsigned short val)
{
  bs.bigEndianValue=EndianS16_NtoB(val);
  return bs;
}
#endif // TARGET_RT_LITTLE_ENDIAN
#endif // ASYS_MAC


#ifdef ASYS_MAC
#if TARGET_RT_LITTLE_ENDIAN
BigEndianLong AAudioDevice::makeSureBigEndianLong(unsigned long val)
{
  bl.bigEndianValue=EndianS32_NtoB(val);
  return bl;
}
#endif // TARGET_RT_LITTLE_ENDIAN
#endif // ASYS_MAC


#ifdef ASYS_MAC
#if TARGET_RT_LITTLE_ENDIAN
BigEndianOSType AAudioDevice::makeSureBigEndianOSType(unsigned long val)
{
  bo.bigEndianValue=EndianS32_NtoB(val);
  return bo;
}
#endif // TARGET_RT_LITTLE_ENDIAN
#endif // ASYS_MAC

