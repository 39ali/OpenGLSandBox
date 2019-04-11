#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "Core.h"
#include <assert.h>
class Shader {
public:
  Shader(const std::string &Vertexlocation,
         const std::string &fragmetLocation , const std::string& geometryShader=""):m_ShaderName(Vertexlocation) {
    m_ShaderProgram = glCreateProgram();
    m_Shaders[0]=AddShader(Vertexlocation, GL_VERTEX_SHADER);
    m_Shaders[1]=AddShader(fragmetLocation, GL_FRAGMENT_SHADER);
	if(!geometryShader.empty())
		m_Shaders[2] = AddShader(geometryShader, GL_GEOMETRY_SHADER);
	LinkProgram();
  }
  ~Shader() {
	  CheckERR();
	  glDeleteProgram(m_ShaderProgram);
	  CheckERR();
	  glDeleteShader(m_Shaders[0]);
	  CheckERR();
	  glDeleteShader(m_Shaders[1]);
	  CheckERR();
	  glDeleteShader(m_Shaders[2]);
	  CheckERR();
  }

  void Use() {
	  glUseProgram(m_ShaderProgram);
	  CheckERR();
  }
  GLuint GetProgram() const {
	  return m_ShaderProgram;
  }

  void Validate() const {
	  GLint Success = 0;
	  GLchar ErrorLog[1024] = { 0 };
	  glValidateProgram(m_ShaderProgram);
	  glGetProgramiv(m_ShaderProgram, GL_VALIDATE_STATUS, &Success);
	  if (!Success) {
		  glGetProgramInfoLog(m_ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		  fprintf(stderr, "Invalid shader program: '%s' \n", ErrorLog);
	  }
	  CheckERR();
  }




void SetUniform(const char* name , float val) {
	 
	GLuint loc =glGetUniformLocation(m_ShaderProgram, name);
	glUniform1f(loc, val);
	CheckUniform(loc, name, m_ShaderName);
	//std::cout << "uniform location can not be found!: " << name << "shader: " << m_ShaderName << "/n";
}



void SetUniform(const char* name, int val) {

	GLuint loc = glGetUniformLocation(m_ShaderProgram, name);
	glUniform1i(loc, val);
	CheckUniform(loc,name,m_ShaderName);
}

 void SetUniform(const char* name, const vec3& val) {

	GLuint loc = glGetUniformLocation(m_ShaderProgram, name);
	glUniform3fv(loc,1, &val);
	CheckUniform(loc, name, m_ShaderName);
}

 void SetUniform(const char* name, const mat4& val) {

	GLuint loc = glGetUniformLocation(m_ShaderProgram, name);
	glUniformMatrix4fv(loc,1,GL_TRUE, val);
	CheckUniform(loc, name, m_ShaderName);
	CheckERR();
}



private:
  struct ShaderInfo {
    std::string content;
    size_t size;
  };
  void LinkProgram() {

	  glLinkProgram(m_ShaderProgram);
	  GLint Success = 0;
	  GLchar ErrorLog[1024] = { 0 };
	  glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, (int *)&Success);
	  if (Success == GL_FALSE)
	  {
		  glGetProgramInfoLog(m_ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		  fprintf(stderr, "Error Linking Program : '%s'\n", 
			  ErrorLog);
		  assert(0);
		  glDeleteProgram(m_ShaderProgram);
		  glDeleteShader(m_Shaders[0]);
		  glDeleteShader(m_Shaders[1]);
		  glDeleteShader(m_Shaders[2]);
		  return;
	  }
	  CheckERR();
  }
  //read shader from file ,compile and attach to program
  GLuint AddShader(const std::string &location, GLenum shaderType) {

	 GLuint shaderID = glCreateShader(shaderType);
	std::string text= ReadShaderFromFile(location);
	const char* ptext = text.c_str();
    glShaderSource(shaderID, 1, &ptext, NULL);
   glCompileShader(shaderID);
   CheckERR();
	
    GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (success==GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> InfoLog(maxLength);
	
      glGetShaderInfoLog(shaderID, maxLength,&maxLength, &InfoLog[0]);
      fprintf(stderr, "Error compiling shader '%s': '%s'\n", location.c_str(),
              &InfoLog[0]);
    }
	glAttachShader(m_ShaderProgram, shaderID);

	CheckERR();
	return shaderID;
  }



  std::string ReadShaderFromFile(const std::string &loc) {
	  std::ifstream f(loc.c_str());
      std::string temp;
	  std::ostringstream iss;
    
	  assert(f.is_open());
		while (std::getline(f,temp)) {
			iss << temp << "\n";
		}   
	
    return iss.str();
  }
 

private:
  GLuint m_ShaderProgram;
  GLuint m_Shaders[3];
  std::string m_ShaderName;
};
