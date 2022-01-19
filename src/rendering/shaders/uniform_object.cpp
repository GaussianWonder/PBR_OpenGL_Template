#include "uniform_object.h"
#include "logger.h"

namespace glt {

template<typename... UP>
UniformObject<UP...>::UniformObject(const char* name, UP... uniforms)
  :name(name), model(std::tuple{uniforms...})
{}

template<typename... UP>
const char * UniformObject<UP...>::getName() { return name; }

} // namespace glt