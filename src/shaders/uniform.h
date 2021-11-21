#ifndef _UNIFORM_H_
#define _UNIFORM_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include <memory>

#include "shader.h"

namespace glt {

class Shader; // let the compile know there will be such a class

template<typename T>
class Uniform {
  friend class Shader;
  using UniformUpdater = void(*)(T, GLint);

public:
  Uniform(const char *name, T model, UniformUpdater updateShader);
  static std::shared_ptr<Uniform<T>> makeShared(const char *name, T model, UniformUpdater updateShader);

private:
  T model;
  const char* name;
  UniformUpdater updateShader;
};

template<typename T>
Uniform<T>::Uniform(const char* name, T model, UniformUpdater updateShader)
  :model(model), name(name), updateShader(updateShader)
{
  // updateShader(model, );
}

template<typename T>
std::shared_ptr<Uniform<T>> Uniform<T>::makeShared(const char *name, T model, UniformUpdater updateShader)
{
  return std::make_shared<Uniform<T>>(name, model, updateShader);
}

} // namespace glt

#endif // _UNIFORM_H_ 
