#pragma once
#include <FreeImage.h>
#include <assert.h>
#include <glad/glad.h>
#include <vector>
struct FreeImageEntry {
  FreeImageEntry(unsigned int width, unsigned int height, FIBITMAP *dib)
      : Width(width), Height(height), Dib(dib) {}
  unsigned int Width;
  unsigned int Height;
  FIBITMAP* Dib;
};
struct Texture {
  GLuint TexID;
};

class TextureFactory {
public:
  static Texture CreateTexture(const char *fileName );
  static Texture CreateTexture3D(const char*fileNames[6]);

private:
  static FreeImageEntry Freeimg(const char *filename);
  static std::vector<Texture> m_Textures;
};
//TODO:delete textures