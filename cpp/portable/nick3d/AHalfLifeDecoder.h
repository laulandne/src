#ifndef ALIB_AHALFLIFEDECODER_H
#define ALIB_AHALFLIFEDECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "AMeshDecoder.h"


// Half-Life (one) model
class AHalfLifeDecoder : public AMeshDecoder
{
public:
  AHalfLifeDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readHeader();
  void readBones();
  void readBoneControllers();
  void readHitBoxes();
  void readSeqs();
  void readSeqHeader();
  void readSeqGroups();
  void readTextures();
  void readSkins();
  void readBodyParts();
  void readAttachments();
  void readSounds();
  void readSoundGroups();
  void readTransitions();
  void readModel();
  //
  unsigned long bonesOffset;
  unsigned long bonecontrollersOffset;
  unsigned long hitboxesOffset;
  unsigned long seqsOffset;
  unsigned long seqgroupsOffset;
  unsigned long texturesOffset;
  unsigned long texturedatasOffset;
  unsigned long skinsOffset;
  unsigned long bodypartsOffset;
  unsigned long attachmentsOffset;
  unsigned long soundsOffset;
  unsigned long soundgroupsOffset;
  unsigned long transitionsOffset;
  //
  unsigned long numbones;
  unsigned long numbonecontrollers;
  unsigned long numhitboxes;
  unsigned long numseq;
  unsigned long numseqgroups;
  unsigned long numtextures;
  unsigned long numskinref;
  unsigned long numskinfamilies;
  unsigned long numbodyparts;
  unsigned long numattachments;
  unsigned long soundtable;
  unsigned long soundgroups;
  unsigned long numtransitions;
};


#endif // ALIB_AHALFLIFEDECODER_H

