#include "TextureFactory.h"

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
	if (!dib)
		assert(0);

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

 Texture TextureFactory::CreateTexture(const char * fileName) {
	FreeImageEntry fi = Freeimg(fileName);
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
	return tex;
}