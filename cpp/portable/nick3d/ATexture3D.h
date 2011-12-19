#ifndef ALIB_ATEXTURE3D_H
#define ALIB_ATEXTURE3D_H


class ATexture3D
{
public:
  ATexture3D(char *fname);
  ATexture3D(unsigned int w,unsigned int h,unsigned int d,unsigned int np);
  //ABitmap *getBitmap() { return bm; }
#ifdef USE_OPENGL
  unsigned int getTextureID(unsigned int n) { return textureID; }
  void setTextureID(unsigned int n,unsigned int t) { textureID=t; }
#endif
protected:
  //
  void init();
  //
  //ABitmap *bm;
#ifdef USE_OPENGL
  unsigned int textureID;
#endif
};


#endif // ALIB_ATEXTURE3D_H

