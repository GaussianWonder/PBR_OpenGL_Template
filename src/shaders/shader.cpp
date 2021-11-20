#include "shader.h"

#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <string>

#include "logger.h"

namespace glt {

// assign the static shader used pointer to a default nullptr
Shader* Shader::shaderUsed = nullptr;

Shader::Shader(const char *vertexShaderFileName, const char *fragmentShaderFileName)
{
  this->loadShader(vertexShaderFileName, fragmentShaderFileName);
}

GLuint Shader::parseShader(const std::string &fileName, GLenum shaderType)
{
  std::string contents = this->readShaderFile(fileName);
  const GLchar* shaderString = contents.c_str();
  GLuint shader;
  shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderString, NULL);
  glCompileShader(shader);
  this->shaderCompileLog(shader);
  return shader;
}

void Shader::loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
{
  GLuint vertexShader = this->parseShader(vertexShaderFileName, GL_VERTEX_SHADER);
  GLuint fragmentShader = this->parseShader(fragmentShaderFileName, GL_FRAGMENT_SHADER);

  this->shaderProgram = glCreateProgram();
  glAttachShader(this->shaderProgram, vertexShader);
  glAttachShader(this->shaderProgram, fragmentShader);
  glLinkProgram(this->shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  this->shaderLinkLog(this->shaderProgram);
}

std::string Shader::readShaderFile(const std::string &fileName)
{
  std::ifstream shaderFile;
  std::string shaderString;
  
  //open shader file
  shaderFile.open(fileName);
  
  std::stringstream shaderStringStream;
  
  //read shader content into stream
  shaderStringStream << shaderFile.rdbuf();
  
  //close shader file
  shaderFile.close();
  
  //convert stream into GLchar array
  shaderString = shaderStringStream.str();

  return shaderString;
}
 
void Shader::shaderCompileLog(GLuint shaderId)
{
  GLint success;
  GLchar infoLog[512];
        
  //check compilation info
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  ASSERT(success, "Shader compilation failed. Rendering will lack!");
  if(!success)
  {
    glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
    ERROR("Shader compilation error {}", infoLog);
  }
}
 
void Shader::shaderLinkLog(GLuint shaderProgramId)
{
  GLint success;
  GLchar infoLog[512];

  //check linking info
  glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
  ASSERT(success, "Shader linking failed. Rendering will lack!");
  if(!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    ERROR("Shader linking error {}", infoLog);
  }
}

void Shader::useShaderProgram()
{
  Shader::shaderUsed = this;
  glUseProgram(this->shaderProgram);
}

} // namespace glt
