
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImageFat.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


/*
NOTES:

0xeb 0x34 0x90 first three bytes?
0xeb 0x3c 0x90 first three bytes?

0x03-0x0a creator dos name?
0x2b-0x35 disk name?
0x36-0x3d fat name?

*/


#define DFAT_FORMAT_UNKNOWN   0
#define DFAT_FORMAT_RAW       1
#define DFAT_FORMAT_MFM_DISK  2


////////////////////////////////////////////////////////////////////////////////
//  DImageFat Class
////////////////////////////////////////////////////////////////////////////////

DImageFat::DImageFat(DVfs *parent) : DDiskImageMem(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImageFat::DImageFat(DVfs *parent,const char *fname) : DDiskImageMem(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImageFat::constructorHelper()
{
  noFileSystem=false;
  allTracksSameSectors=true;
  imageFormat=DFAT_FORMAT_UNKNOWN;
  // NOTE: 1.4m floppy geometry
  numTracks=80; numSectors=18; blockSize=512; numSides=2;
  //
  fatSectorsPerCluster=0;
  fatReservedSectors=0;
  fatTotalFats=0;
  fatMaxRootEntries=0;
  fatTotalSectors=0;
  fatMediaDescriptor=0;
  fatSectorsPerFat=0;
  fatHiddenSectors=0;
  fatTotalSectorsLarge=0;
  fatDriveNumber=0;
  fatFlags=0;
  fatSignature=0;
}


DImageFat::~DImageFat()
{
  freeDirCache();
  diskMounted=false;
}


/* STATIC */unsigned int DImageFat::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  char *tstring=NULL;
  if((tbuf[0]==0xeb)&&(tbuf[2]==0x90)) ret=DFAT_FORMAT_RAW;
  tstring="FAT12   ";
  if(!strncmp((char *)(tbuf+0x36),tstring,strlen(tstring))) ret=DFAT_FORMAT_RAW;
  tstring="MSXDOS  SYS";
  if(!strncmp((char *)(tbuf+0x7a),tstring,strlen(tstring))) ret=DFAT_FORMAT_RAW;
  tstring="MFM_DISK";
  if(!strncmp((char *)tbuf,tstring,strlen(tstring))) ret=DFAT_FORMAT_MFM_DISK;  // Oric disk image?
  // TODO: Some more checking here before we recognize it
  return ret;
}


void DImageFat::readDirectory()
{
  DEBUG_OUT<<"DImageFat::readDirectory...\n";
  freeDirCache();
  unsigned int t=0;
  for(t=0;t<256;t++) diskName[t]=0;
  strcpy(diskName,"NO NAME");
  //
  imageFormat=recognize(imageData,NULL,imageDataSize);
  switch(imageFormat) {
    case DFAT_FORMAT_RAW:
      break;
    case DFAT_FORMAT_MFM_DISK:
      DEBUG_OUT<<"Unimplemented DFAT_FORMAT_MFM_DISK, sorry!\n";
      break;
    default:
      DEBUG_OUT<<"Unimplemented format, sorry!\n";
      diskError=true;
      return;
      break;
  }
  //
  readBlock(0);
  unsigned offset=0xb;
  blockSize=(imageData[offset+1]*256)+imageData[offset];  offset+=2;
  fatSectorsPerCluster=imageData[offset]; offset++;
  fatReservedSectors=(imageData[offset+1]*256)+imageData[offset];  offset+=2;
  fatTotalFats=imageData[offset]; offset++;
  fatMaxRootEntries=(imageData[offset+1]*256)+imageData[offset];  offset+=2;
  fatTotalSectors=(imageData[offset]*256)+imageData[offset];  offset+=2;
  fatMediaDescriptor=imageData[offset]; offset++;
  fatSectorsPerFat=(imageData[offset+1]*256)+imageData[offset];  offset+=2;
  numSectors=(imageData[offset+1]*256)+imageData[offset];  offset+=2;
  numSides=(imageData[offset+1]*256)+imageData[offset];  offset+=2;
  fatHiddenSectors=(imageData[offset+3]<<24)+(imageData[offset+2]<<16)+(imageData[offset+1]<<8)+imageData[offset];  offset+=4;
  fatTotalSectorsLarge=(imageData[offset+3]<<24)+(imageData[offset+2]<<16)+(imageData[offset+1]<<8)+imageData[offset];  offset+=4;
  fatDriveNumber=imageData[offset]; offset++;
  fatFlags=imageData[offset]; offset++;
  fatSignature=imageData[offset]; offset++;
  DEBUG_OUT<<"fatSectorsPerCluster="<<fatSectorsPerCluster<<"\n";
  DEBUG_OUT<<"fatReservedSectors="<<fatReservedSectors<<"\n";
  DEBUG_OUT<<"fatTotalFats="<<fatTotalFats<<"\n";
  DEBUG_OUT<<"fatMaxRootEntries="<<fatMaxRootEntries<<"\n";
  DEBUG_OUT<<"fatTotalSectors="<<fatTotalSectors<<"\n";
  DEBUG_OUT<<"fatMediaDescriptor="<<fatMediaDescriptor<<"\n";
  DEBUG_OUT<<"fatSectorsPerFat="<<fatSectorsPerFat<<"\n";
  DEBUG_OUT<<"fatHiddenSectors="<<fatHiddenSectors<<"\n";
  DEBUG_OUT<<"fatTotalSectorsLarge="<<fatTotalSectorsLarge<<"\n";
  DEBUG_OUT<<"fatDriveNumber="<<fatDriveNumber<<"\n";
  DEBUG_OUT<<"fatFlags="<<fatFlags<<"\n";
  DEBUG_OUT<<"fatSignature="<<fatSignature<<"\n";
  //
  DEBUG_OUT<<"numTracks="<<numTracks<<" numSectors="<<numSectors<<" numSides="<<numSides<<" blockSize="<<blockSize<<"\n";
  if(!numSectors) {
    DEBUG_OUT<<"numSectors is zero, we messed up someplace, bailing!\n";
    diskError=true;
    exit(5);
  }
  //
  // NOTE: We should do some sanity checking on the directory entries, etc...
  // Now read directory entries
  // NOTE: Don't know what the right sectors are for this yet...
  for(t=1;t<8;t++) {
    readTrackSector(1,t);
    for(unsigned int i=0;i<(blockSize/32);i++) {
      readImageDirEntry(i*32);
    }
  }
}


void DImageFat::cacheFile(void *fp)
{
  std::cerr<<"DImageFat::cacheFile not implemented!\n";
  diskError=true;
}


void DImageFat::readImageDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageFat::readImageDirEntry("<<off<<")...\n";
  if(true) {
    unsigned int t=0;
    DDirEntry *theFile=new DDirEntry;
    if(theFile) {
      theFile->signature=0xface;
      for(t=0;t<DDISK_MAX_NAMELEN;t++) theFile->name[t]=0;
      theFile->name[DDISK_MAX_NAMELEN]=0;
      for(t=0;t<8;t++) theFile->name[t]=buf[off+t];
      theFile->name[8]='.';
      for(t=8;t<11;t++) theFile->name[t+1]=buf[off+t];
      DEBUG_OUT<<"name is \""<<theFile->name<<"\"\n";
      theFile->nativeType=buf[off+12];
      DEBUG_OUT<<"nativeType="<<theFile->nativeType<<"\n";
      theFile->type=DDISK_TYPE_BINARY; // TODO
      unsigned int entryCluster=(buf[off+0x1b]<<8)+buf[off+0x1a];
      DEBUG_OUT<<"entryCluster is "<<entryCluster<<"\n";
      theFile->track=0; // TODO:
      theFile->sector=0; // TODO:
      theFile->data=(BYTE *)NULL;
      theFile->size=(buf[off+0x1f]<<24)+(buf[off+0x1e]<<16)+(buf[off+0x1d]<<8)+buf[off+0x1c];
      theFile->sizeBlocks=theFile->size/512;
      DEBUG_OUT<<"size is "<<theFile->size<<" bytes\n";
      DEBUG_OUT<<"size is "<<theFile->sizeBlocks<<" blocks\n";
      theFile->block=0;
      theFile->curSeek=0;
      theDir.append((void *)theFile);
    }
    else DEBUG_OUT<<"Couldn't alloc file entry!\n";
  }
}
