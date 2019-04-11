#include "TextureFactory.h"
#include <cinttypes>


 std::vector<Texture> TextureFactory::m_Textures;

FreeImageEntry TextureFactory::Freeimg(const char* filename) {
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		assert(0);

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib) {
		printf("Error! opening file: '%s' \n", filename);
		return {0,0,0};
	}
	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		assert(0);
	unsigned int bpp = FreeImage_GetBPP(dib);

	return { width,height,dib };
}

 Texture TextureFactory::CreateTexture(const char * fileName ) {
	FreeImageEntry fi = Freeimg(fileName);
	if (fi.Width == 0)
	{
		return {static_cast<GLuint>( -1) };
	}
	Texture tex;
	BYTE* bits = FreeImage_GetBits(fi.Dib);

	unsigned int bpp = FreeImage_GetBPP(fi.Dib);

	glGenTextures(1, &tex.TexID);
	glBindTexture(GL_TEXTURE_2D, tex.TexID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (bpp==32)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fi.Width, fi.Height, 0, GL_BGRA, GL_UNSIGNED_BYTE
		,bits );
	else if (bpp == 24)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fi.Width, fi.Height, 0, GL_BGR, GL_UNSIGNED_BYTE
			, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(fi.Dib);

	m_Textures.push_back(tex);
	return tex;
}

  Texture TextureFactory::CreateTexture3D(const char*fileName[6]) {
	 Texture t;
	 glGenTextures(1, &t.TexID);
	 glBindTexture(GL_TEXTURE_CUBE_MAP, t.TexID);
	
	 for (uint32_t i = 0; i < 6; ++i) {
		 FreeImageEntry fi = Freeimg(fileName[i]);
		
		 BYTE* bits = FreeImage_GetBits(fi.Dib);

		 unsigned int bpp = FreeImage_GetBPP(fi.Dib);
		 if (bpp == 32)
			 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, fi.Width, fi.Height, 0, GL_BGRA, GL_UNSIGNED_BYTE
				 , bits);
		 else if (bpp == 24)
			 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, fi.Width, fi.Height, 0, GL_BGR, GL_UNSIGNED_BYTE
				 , bits);
		
			 FreeImage_Unload(fi.Dib);
	 }
	 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 
	 m_Textures.push_back(t);
	 return t;
 
 }