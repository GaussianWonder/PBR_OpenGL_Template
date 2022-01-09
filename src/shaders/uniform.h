#ifndef _UNIFORM_H_
#define _UNIFORM_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include <memory>

namespace glt {

template<typename T>
class Uniform {
  const char* name;
  T model;

public:
  Uniform(const char *name, T model);
  static std::shared_ptr<Uniform<T>> makeShared(const char *name, T model);

  const char * getName();

  void update(GLint location);
};

} // namespace glt

#endif // _UNIFORM_H_ 
