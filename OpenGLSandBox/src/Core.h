#pragma once
#include <glad/glad.h>
#include <cstdio>

#ifdef _DEBUG
#define CheckERR() \
{ \
 GLenum err; \
 while((err= glGetError())!=GL_NO_ERROR) \
	   {printf("OpenGL Error :0x%x\n",err); \
		 __debugbreak(); \
		}\
} 
//checks if uniform location is valid
#define CheckUniform(x) \
if (x == -1) __debugbreak();
#else
#define ERR(x) x

#define ERRU(x)
#endif