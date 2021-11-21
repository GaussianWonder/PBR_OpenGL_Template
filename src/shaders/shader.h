#ifndef _SHADER_H_
#define _SHADER_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include <tuple>
#include <memory>

#include "logger.h"
#include "uniform.h"

namespace glt {

template<typename T>
class Uniform; // let the compile know there will be such a class

// Concepts for later use
template<typename T> struct is_sharable_uniform : std::false_type {};
template<typename T> struct is_sharable_uniform<std::shared_ptr<Uniform<T>>> : std::true_type {};
template<typename ...T> concept IsSharableUniform = (is_sharable_uniform<T>::value && ...);

class Shader {
  template<typename T>
  friend class Uniform;

public:
  Shader(const char *vertexShaderFileName, const char *fragmentShaderFileName);

  void loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
  void useShaderProgram();

  template<typename ...Args>
    requires IsSharableUniform<Args...>
  void withUniforms(Args... sharableUniforms)
  {
    constexpr std::size_t uniformCount = sizeof...(Args);
    DEBUG("Shader {} received {} uniforms", this->shaderProgram, uniformCount);
    ASSERT(uniformCount > 0, "With uniform expects at least one uniform. Send all uniforms, they will be replaced at the next assignment.");
    std::tuple<Args...> uniforms{sharableUniforms...};
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
