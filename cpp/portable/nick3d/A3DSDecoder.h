#ifndef ALIB_A3DSDECODER_H
#define ALIB_A3DSDECODER_H

#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "AMeshDecoder.h"


// 3ds model
class A3DSDecoder : public AMeshDecoder
{
public:
  A3DSDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readObjects();
  void readEdVer();
  void readEditor();
  //
  bool readMaterialChunk();  // true if all went well
  void readMatAmbient();
  void readMatDiffuse();
  void readMatSpecular();
  void readMatShininess();
  void readMatTransparent();
  void readMatTexture();
  void readMatTexFile();
  //
  bool readObjectChunk();  // true if all went well
  void readTriMesh();
  void readMeshVertexl();
  void readMeshFace();
  void readMeshTexCoords();
  void readMeshMatInfo();
};


#endif // ALIB_A3DSDECODER_H

