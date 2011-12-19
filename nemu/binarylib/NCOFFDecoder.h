#ifndef BINARYLIB_NCOFFDECODER_H
#define BINARYLIB_NCOFFDECODER_H

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


struct NCOFF_Header {
  USHORT machine;
  USHORT numSections;
  ULONG dateStamp;
  ULONG symbolsPtr;
  ULONG numSym;
  USHORT sizeOption;
  USHORT charac;
};


class NCOFFDecoder : public NBinaryDecoder
{
public:
  NCOFFDecoder();
  static bool recognize(unsigned char *buf,char *fname=(char *)NULL);
  virtual char *whichCPU();
  virtual char *whichIO();
  virtual char *which() { return (char *)"NCOFFDecoder"; }
  virtual bool verifyOkay();
protected:
};


#endif
