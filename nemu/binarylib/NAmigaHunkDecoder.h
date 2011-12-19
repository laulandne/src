#ifndef BINARYLIB_NAMIGAHUNKDECODER_H
#define BINARYLIB_NAMIGAHUNKDECODER_H

// FIXME: Could be spread out into many plug-ins

/*

Nick's Emulator: a Multi-Machine Experimental Emulator
Copyright (C) 2005 Nicholas Lauland

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


class NAmigaHunkDecoder : public NBinaryDecoder
{
public:
  NAmigaHunkDecoder();
  static bool recognize(unsigned char *buf,char *fname=(char *)NULL);
  virtual char *whichCPU();
  virtual char *whichIO();
  virtual char *which() { return (char *)"NAmigaHunkDecoder"; }
  virtual bool verifyOkay();
  virtual bool decodeNodes();
protected:
  bool readHeader();
  bool readNodes();
  bool readReloc(unsigned int size);
  bool readSymbols(bool external);
  unsigned int numHunks;
  unsigned int nextHunk;
};


#endif
