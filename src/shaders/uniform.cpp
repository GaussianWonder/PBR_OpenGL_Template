#include "uniform.h"
#include "logger.h"

namespace glt {

template<typename T>
Uniform<T>::Uniform(const char* name, T model)
  :model(model), name(name)
{}

template<typename T>
std::shared_ptr<Uniform<T>> Uniform<T>::makeShared(const char *name, T model)
{
  return std::make_shared<Uniform<T>>(name, model);
}

template<typename T>
void Uniform<T>::update(GLint location)
{
  WARN("Use partial specialisation");
}

template<typename T>
const char * Uniform<T>::getName()
{
  return this->name;
}

} // namespace glt