#ifndef ALIB_AMD3DECODER_H
#define ALIB_AMD3DECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "AMeshDecoder.h"


// Quake 3 model
class AMD3Decoder : public AMeshDecoder
{
public:
  AMD3Decoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readHeader();
  void readBFrames();
  void readTags();
  void readMeshes();
  void readMeshHeader();  // for each mesh
  void readVertices();  // for each mesh
  void readTriangles();  // for each mesh
  void readTextures();  // for each mesh
  void readTextCoords();  // for each mesh
  //
  unsigned long nbframes;
  unsigned long ntags;
  unsigned long nmeshes;
  unsigned long maxSkins;
  unsigned long framesOffset;
  unsigned long tagsOffset;  // from start of file
  unsigned long meshesOffset;  // from start of file
  unsigned long curHeaderOffset;
  unsigned long lastVert;
  unsigned long lastMesh;
  //
  unsigned long nkframes;  // per mesh
  unsigned long texturesOffset;  // per mesh, from start of mesh header
  unsigned long triOffset;  // per mesh, from start of mesh header
  unsigned long textCoordsOffset;  // per mesh, from start of mesh header
  unsigned long vertsOffset;  // per mesh, from start of mesh header
};


#endif // ALIB_AMD3DECODER_H

