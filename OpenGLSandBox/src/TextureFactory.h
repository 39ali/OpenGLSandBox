#pragma once
#include <FreeImage.h>
#include <assert.h>
#include <glad/glad.h>

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
  static Texture CreateTexture(const char *fileName);

private:
  static FreeImageEntry Freeimg(const char *filename);
};
