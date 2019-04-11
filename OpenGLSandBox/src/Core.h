#pragma once
#include <glad/glad.h>
#include <cstdio>
#include <iostream>
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
#define CheckUniform(loc, uniformName , shaderName) \
if (loc == -1) std::cout<< "uniform location can not be found!: " << uniformName << "shader: "<< shaderName<<"\n";
#else
#define CheckERR() 

#define CheckUniform(x)
#endif