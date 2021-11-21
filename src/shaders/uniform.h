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
  // This is the so-called "constructor"
  static std::shared_ptr<Uniform<T>> make(const char *name, T model, UniformUpdater updateShader)
  {
    // TODO This unfortunately does not work because the constructor is private... stuck with 2 heap allocations for now
    // return std::make_shared<Uniform<T>>(name, model, updateShader);
    return std::shared_ptr<Uniform<T>>(new Uniform<T>(name, model, updateShader));
  }

private:
  Uniform(const char *name, T model, UniformUpdater updateShader);

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

} // namespace glt

#endif // _UNIFORM_H_ 
