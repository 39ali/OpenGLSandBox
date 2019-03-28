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
         const std::string &fragmetLocation) {
    m_ShaderProgram = glCreateProgram();
    m_Shaders[0]=AddShader(Vertexlocation, GL_VERTEX_SHADER);
    m_Shaders[1]=AddShader(fragmetLocation, GL_FRAGMENT_SHADER);
	LinkProgram();
  }
  ~Shader() {
	  ERR(glDeleteProgram(m_ShaderProgram));
	  ERR(glDeleteShader(m_Shaders[0]));
	  ERR(glDeleteShader(m_Shaders[1]));
  }

  void Use() {
	  ERR(glUseProgram(m_ShaderProgram));
  }
  GLuint GetProgram() const {
	  return m_ShaderProgram;
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
		  return;
	  }

	  glValidateProgram(m_ShaderProgram);
	  glGetProgramiv(m_ShaderProgram, GL_VALIDATE_STATUS, &Success);
	  if (!Success) {
		  glGetProgramInfoLog(m_ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		  fprintf(stderr, "Invalid shader program: '%s' \n", ErrorLog);
	  }
  }
  //read shader from file ,compile and attach to program
  GLuint AddShader(const std::string &location, GLenum shaderType) {

	 GLuint shaderID = glCreateShader(shaderType);
	std::string text= ReadShaderFromFile(location);
	const char* ptext = text.c_str();
   ERR( glShaderSource(shaderID, 1, &ptext, NULL));
   ERR(glCompileShader(shaderID));
	
	
    GLint success;
	ERR(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success));
    if (success==GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> InfoLog(maxLength);
	
      glGetShaderInfoLog(shaderID, maxLength,&maxLength, &InfoLog[0]);
      fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType,
              &InfoLog[0]);
    }
	ERR(glAttachShader(m_ShaderProgram, shaderID));

	
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
  GLuint m_Shaders[2];
};
