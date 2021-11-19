#ifndef _SHADER_H_
#define _SHADER_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include "uniform.h"

namespace glt {

template<typename T>
class Uniform; // let the compile know there will be such a class

class Shader {
  template<typename T>
  friend class Uniform;

public:
  Shader(const char *vertexShaderFileName, const char *fragmentShaderFileName);

  void loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
  void useShaderProgram();

  template<typename T>
  void subscribe(Uniform<T> &uniform)
  {

  }

private:
  GLuint parseShader(const std::string &fileName, GLenum shaderType);
  std::string readShaderFile(const std::string &fileName);
  void shaderCompileLog(GLuint shaderId);
  void shaderLinkLog(GLuint shaderProgramId);

  GLuint shaderProgram;

  static Shader* shaderUsed;
  // Not really the best idea, but this should only be used as an unique identifier
};

} // namespace glt

#endif // _SHADER_H_ 
