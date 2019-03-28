#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <cstdio>
#include "math/Vector3f.h"
#include "math/Vector2f.h"
#include "Shader.h"
#include "math/Matrix4f.h"
#include "Camera.h"

#include <stddef.h>
#include "TextureFactory.h"
#include "Mesh.h"



GLuint VBO;
GLuint IBO;
const float width = 680;
const float height = 480;
GLFWwindow* window = nullptr;
Camera camera({ 0,0,-50 }, { 0,0,1 }, {0,1,0},width,height);
Mesh mesh1;
Mesh mesh2;

void BindAndDraw() {


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
	Scale.makeScale(vec3(0.5));
	Perspective.makePerspectiveProjection(1.0f, 1000.0f, width, height, 75.0f);
	Camera.makeCameraTransform(camera.m_Origin,camera.m_Target, camera.m_Up);
	Transform = Perspective* Camera* Translate*Rotate * Scale;
	ERR(glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, Transform));
	
	GLint textureLoc = glGetUniformLocation(shader.GetProgram(), "sampler");
	ERRU(textureLoc);
	ERR(glUniform1i(textureLoc, 0));
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

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//////
	


	mesh1.LoadMesh("rec/models/bike.fbx");
	mesh2.LoadMesh("rec/models/phoenix_ugv.md2");
	

	Shader shader("rec/shader/shader.vs","rec/shader/shader.fs");
	shader.Use();
	Texture tex=TextureFactory::CreateTexture("rec/test.png");
	
	glBindTexture(GL_TEXTURE_2D, tex.TexID);
	glActiveTexture(GL_TEXTURE0);
	
	while (!glfwWindowShouldClose(window)) {
		
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5, 0.5, 0.5, 1.0f);
	
		shader.Use();
		mesh1.Render();
	//	mesh2.Render();
		UpdateTransformMatrix(shader);
		BindAndDraw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	
	}
	glfwTerminate();
	return 0;
}