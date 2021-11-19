#ifndef _UNIFORM_H_
#define _UNIFORM_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include <vector>

#include "shader.h"

namespace glt {

class Shader; // let the compile know there will be such a class

template<typename T>
class Uniform {
  friend class Shader;

public:
  Uniform(const char* name, T model, void (*updateShader)(T, GLint));

private:
  const char* name;
  T model;
  void (*updateShader)(T);
};

} // namespace glt

#endif // _UNIFORM_H_ 
