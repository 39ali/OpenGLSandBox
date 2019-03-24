#pragma once
#include <glad/glad.h>
#include <cstdio>

#ifdef _DEBUG
#define ERR(x) \
{ \
 x; \
 GLenum err; \
 while((err= glGetError())!=GL_NO_ERROR) \
	   {printf("OpenGL Error number:0x%x\n",err); \
		/* __debugbreak();*/ \
		}\
} 
//checks if uniform location is valid
#define ERRU(x) \
if (x == -1) __debugbreak();
#else
#define ERR(x) x

#define ERRU(x)
#endif