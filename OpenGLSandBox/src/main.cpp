#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <cstdio>
#include "math/Vector3f.h"
#include "math/Vector2f.h"
#include "Shader.h"
#include "math/Matrix4f.h"
#include "Camera.h"
#include <FreeImage.h>
#include <stddef.h>

struct Vertex {

	Vertex() {};
	Vertex(const vec3& _pos, const vec2& _uv):pos(_pos),uv(_uv) {}
	vec3 pos;
	vec2 uv;
};

GLuint VBO;
GLuint IBO;
const float width = 680;
const float height = 480;
GLFWwindow* window = nullptr;
Camera camera({ 0,0,-3 }, { 0,0,1 }, {0,1,0},width,height);
void BindAndDraw() {

    ERR(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	
	ERR(glEnableVertexAttribArray(0));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(Vertex),(const void *)offsetof(Vertex,pos));
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,uv));
	
	

	ERR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
	ERR(glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0));
	ERR(glDisableVertexAttribArray(0));
	glDisableVertexAttribArray(1);
}

void UpdateTransformMatrix(const Shader& shader) {
	camera.Update(window,1.0f/60.f);
	static float scale = 0;
	GLint mvpLoc = glGetUniformLocation(shader.GetProgram(), "mvp");
	ERRU(mvpLoc);

	Matrix4f Scale, Rotate, Translate ,Camera,Perspective, Transform;

	scale += 0.001f;

	Translate.makeTranslate(vec3(0, 0, 0));
	Rotate.makeRotateTransform(vec3(0,scale*300, 0));
	Scale.makeScale(vec3(1, 1, 1));
	Perspective.makePerspectiveProjection(1.0f, 1000.0f, width, height, 75.0f);
	Camera.makeCameraTransform(camera.m_Origin,camera.m_Target, camera.m_Up);
	Transform = Perspective* Camera* Translate*Rotate * Scale;
	ERR(glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, Transform));
	
	GLint textureLoc = glGetUniformLocation(shader.GetProgram(), "sampler");
	ERRU(textureLoc);
	ERR(glUniform1i(textureLoc, 0));
}

bool Freeimg(const char* filename) {

	
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;
	unsigned int bpp = FreeImage_GetBPP(dib);
	
	GLuint m_texture;
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE
		, bits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_texture);


	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
	

}


int main() {

	assert(glfwInit());
	
	window = glfwCreateWindow((int)width,(int)height,"suuup",NULL,NULL);
	assert(window);
	glfwMakeContextCurrent(window);
	
	assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
	glfwSwapInterval(1);
	 const GLubyte * cc[4];
	cc[0] =glGetString(GL_VERSION);
	cc[1] = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cc[2] = glGetString(GL_VENDOR);
	printf("openGL:%s , shaderVersion:%s , vendor:%s \n", cc[0], cc[1] ,cc[2]);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	//////
	
	Vertex vertices[4];
	vertices[0] = { { -1, -1, 0 }, {0.0f,0.f} };
	vertices[1] = { {0, -1., 1.}, {0.5f,0.0f} };
	vertices[2] = { {1, -1, 0},{1.0f,0.0f} };
	vertices[3] = { {0.0, 1., 0},{0.5f,1.0f} };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices)*sizeof(Vertex),vertices,GL_STATIC_DRAW);
	
	unsigned int Indices[] = { 0, 3, 1,
							  1, 3, 2,
							  2, 3, 0,
							  0, 1, 2 };
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	Shader shader("rec/shader/shader.vs","rec/shader/shader.fs");
	shader.Use();
	Freeimg("rec/test.png");

	while (!glfwWindowShouldClose(window)) {
		
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.5, 0.5, 0.5, 1.0f);
	
		shader.Use();
	
		UpdateTransformMatrix(shader);
		BindAndDraw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	
	}
	glfwTerminate();
	return 0;
}