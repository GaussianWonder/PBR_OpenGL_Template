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

public:
  T model; //! not good, keep it private
  Uniform(const char *name, T model);
  static std::shared_ptr<Uniform<T>> makeShared(const char *name, T model);

  const char * getName();

  void update(GLint location);

  // copy assignment
  T& operator=(const T& other);
  // move assignment
  T& operator=(T&& other) noexcept;
};

} // namespace glt

#endif // _UNIFORM_H_ 
