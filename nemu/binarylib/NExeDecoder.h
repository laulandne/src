#ifndef BINARYLIB_NEXEDECODER_H
#define BINARYLIB_NEXEDECODER_H

/*

Nick's Emulator: a Multi-Machine Experimental Emulator
Copyright (C) 2007 Nicholas Lauland

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.

-------------------------------------------------------------------

*/


//#include <alib/abaseclass.h>
#include <binarylib/NBinaryDecoder.h>
#include <binarylib/NCOFFDecoder.h>


struct NExe_MZ_Header {
  USHORT mzMagic;
  USHORT lastPageSize;
  USHORT totalPages;
  USHORT relocItems;
  USHORT parasHeader;
  USHORT minExtraPara;
  USHORT maxExtraPara;
  USHORT initalSS;
  USHORT initalSP;
  USHORT checksum;
  USHORT initalIP;
  USHORT initalCS;
  USHORT relocOffset;
  USHORT overlayNum;
};


struct NExe_PE_Optional {
  unsigned int linkerVer;
  unsigned int linkerRev;
  ULONG sizeCode;
  ULONG sizeData;
  ULONG sizeBSS;
  ULONG addrEntry;
  ULONG baseCode;
  ULONG baseData;
  //imageBase;
  ULONG sectionAlign;
  ULONG fileAlign;
  USHORT osVer;
  USHORT osRev;
  USHORT imVer;
  USHORT imRev;
  USHORT subVer;
  USHORT subRev;
  ULONG reserved1;
  ULONG sizeImage;
  ULONG sizeHeaders;
  ULONG checksum;
  USHORT subsystem;
  USHORT dllChars;
  ULONG sizeStackRes;
  ULONG sizeStackCom;
  ULONG sizeHeapRes;
  ULONG sizeHeapCom;
  ULONG loaderFlags;
  ULONG sizeDataDir;
};


struct NExe_PE_Section {
  char sectName[9];
  ULONG vSize;
  ULONG rva;
  ULONG pSize;
  ULONG pOff;
  ULONG rOff;
  ULONG lOff;
  USHORT rNum;
  USHORT lNum;
  ULONG flags;
};


class NExeDecoder : public NBinaryDecoder
{
public:
  NExeDecoder();
  static bool recognize(unsigned char *buf,char *fname=(char *)NULL);
  virtual char *whichCPU();
  virtual char *whichIO();
  virtual bool decodeNodes();
  virtual char *which() { return (char *)"NExeDecoder"; }
  virtual bool verifyOkay();
protected:
  //
  bool decodeNodesMZ();
  bool decodeNodesNE();
  bool decodeNodesLE();
  bool decodeNodesPE();
  bool decodeNodesLX();
  bool decodeNodesObj();
  void handleCLR();
  bool handleSections(unsigned int numSections);
  bool handleSection(unsigned int num);
  //
  unsigned int os;
  bool clr;
  ULONG imageBase;
  ULONG guessedEntry;
};


#endif
