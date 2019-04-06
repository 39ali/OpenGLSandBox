#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <assert.h>
#include <string>
#include "Camera.h"
#include "Core.h"
#include "math\Matrix4f.h"
#include "math\Vector3f.h"
#include <iostream>
class Application {

public :

	void Run() {

		

		Application::Init();
		Init();
		while (!glfwWindowShouldClose(m_Window)) {
			Application::Update();
			Update();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.5, 0.5, 0.5, 1.0f);
			Render();
			
			glfwSwapBuffers(m_Window);
			glfwPollEvents();

		}
		glfwTerminate();
	
	}

 protected:
	 Application(unsigned int width, unsigned int height, const std::string& name)
		 : m_Width(width), m_Height(height) {
		 glfwInit();
		 m_Window =
			 glfwCreateWindow((int)width, (int)height, name.c_str(), NULL, NULL);
		 assert(m_Window);
		 glfwMakeContextCurrent(m_Window);
		 assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		
		 glfwSwapInterval(1);
		 const GLubyte * cc[4];
		 cc[0] = glGetString(GL_VERSION);
		 cc[1] = glGetString(GL_SHADING_LANGUAGE_VERSION);
		 cc[2] = glGetString(GL_VENDOR);
		 printf("openGL:%s , shaderVersion:%s , vendor:%s \n", cc[0], cc[1], cc[2]);

		 CheckERR();
		 glFrontFace(GL_CW);
		 glCullFace(GL_BACK);
		 glEnable(GL_CULL_FACE);
		 glEnable(GL_DEPTH_TEST);
		 CheckERR();
	 }

	 virtual ~Application() {
	 
		 delete m_Camera;
		 glfwDestroyWindow(m_Window);
	
	 }

	 virtual void Init() {
		 m_Camera = new Camera({ 0, 5, -30 }, { 0, 0, 0 }, { 0, 1, 0 }, m_Width, m_Height);
	 };
  virtual void Render() = 0;
  virtual void Update() {
	  m_Camera->Update(m_Window, 1.0f / 60.f); 
  }


  inline void SetCamera(const vec3& origin ,const vec3& target , const vec3& up) {
	  delete m_Camera;
	  m_Camera = new Camera(origin, target, up, m_Width, m_Height);

  }

  unsigned int m_Width, m_Height;
  Camera* m_Camera = nullptr;
  GLFWwindow* m_Window = nullptr;
};
