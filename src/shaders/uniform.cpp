#include "uniform.h"

namespace glt {

template<typename T>
Uniform<T>::Uniform(const char* name, T model, void (*updateShader)(T, GLint))
  :model(model), name(name), updateShader(updateShader)
{
  // updateShader(model, );
}

} // namespace glt