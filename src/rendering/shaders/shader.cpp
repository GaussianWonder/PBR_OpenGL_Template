#include "shader.h"
#include "file_utils.h"
#include "logger.h"

#include <GL/glew.h>
#include <string>

namespace glt {

// assign the static shader used pointer to a default nullptr
Shader* Shader::shaderUsed = nullptr;
Shader* Shader::currentShader()
{
  ASSERT(Shader::shaderUsed != nullptr, "Use a shader with Shader::useShaderProgram at least once before trying to get the currently used shader!");
  return Shader::shaderUsed;
}

Shader::Shader(const char *vertexShaderFileName, const char *fragmentShaderFileName)
{
  this->loadBasicProgram(vertexShaderFileName, fragmentShaderFileName);
}

Shader::Shader(const char *vertexShaderFileName, const char *geometryShaderFileName, const char *fragmentShaderFileName)
{
  GLuint vertexShader = this->parseShader(
    this->readFile(vertexShaderFileName),
    GL_VERTEX_SHADER
  );
  GLuint geometryShader = this->parseShader(
    this->readFile(geometryShaderFileName),
    GL_GEOMETRY_SHADER
  );
  GLuint fragmentShader = this->parseShader(
    this->readFile(fragmentShaderFileName),
    GL_FRAGMENT_SHADER
  );
  DEBUG("Vertex shader at {}", vertexShader);
  DEBUG("Geometry shader at {}", geometryShader);
  DEBUG("Fragment shader at {}", fragmentShader);

  this->shaderProgram = glCreateProgram();
  glAttachShader(this->shaderProgram, vertexShader);
  glAttachShader(this->shaderProgram, geometryShader);
  glAttachShader(this->shaderProgram, fragmentShader);
  glLinkProgram(this->shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);
  Shader::shaderLinkLog(this->shaderProgram);
}

void Shader::loadBasicProgram(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
{
  GLuint vertexShader = this->parseShader(
    this->readFile(vertexShaderFileName),
    GL_VERTEX_SHADER
  );
  GLuint fragmentShader = this->parseShader(
    this->readFile(fragmentShaderFileName),
    GL_FRAGMENT_SHADER
  );
  DEBUG("Vertex shader at {}", vertexShader);
  DEBUG("Fragment shader at {}", fragmentShader);

  this->shaderProgram = glCreateProgram();
  glAttachShader(this->shaderProgram, vertexShader);
  glAttachShader(this->shaderProgram, fragmentShader);
  glLinkProgram(this->shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  Shader::shaderLinkLog(this->shaderProgram);
}

GLuint Shader::parseShader(const std::string &shaderContents, GLenum shaderType)
{
  const GLchar* shaderString = shaderContents.c_str();
  DEBUG("{} shader contents \n{}\n", shaderType, shaderString);
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderString, NULL);
  glCompileShader(shader);
  Shader::shaderCompileLog(shader);
  return shader;
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
    glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
    ERROR("Shader linking error {}", infoLog);
  }
}

void Shader::useShaderProgram()
{
  Shader::shaderUsed = this;
  GLERR( glUseProgram(this->shaderProgram) );
}

} // namespace glt
