#ifndef _UNIFORM_OBJECT_H_
#define _UNIFORM_OBJECT_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include <memory>
#include <tuple>

namespace glt {

template<typename... UniformPartials>
class UniformObject {
  const char* name;
public:
  std::tuple<UniformPartials...> model; //! not good, keep it private or make friends
  UniformObject(const char *name, UniformPartials... args);

  const char * getName();

  void update(GLint location);
};

} // namespace glt

#endif // _UNIFORM_OBJECT_H_ 
