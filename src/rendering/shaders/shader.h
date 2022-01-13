#ifndef _SHADER_H_
#define _SHADER_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>

#include "file_utils.h"

namespace glt {

class Shader : public FileUtils {
public:
  // Load shader manually using helper functions
  Shader() = default;
  // Load minimal shader from file
  Shader(const char *vertexShaderFileName, const char *fragmentShaderFileName);

  void loadBasicProgram(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
  void useShaderProgram();

  static GLuint parseShader(const std::string &shaderContents, GLenum shaderType);
  static void shaderCompileLog(GLuint shaderId);
  static void shaderLinkLog(GLuint shaderProgramId);

  GLuint shaderProgram = 0;

  //? Is this really necessary?
  static Shader* shaderUsed; // Not the best idea, but it's ok since at all times at least one shader is used
  static Shader* currentShader(); // The developer is constrained to first call Shader::useShaderProgram() at least once before this
};

} // namespace glt

#endif // _SHADER_H_ 
