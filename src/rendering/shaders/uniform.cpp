#include "uniform.h"
#include "logger.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace glt {

template<typename T>
Uniform<T>::Uniform(const char* name, T model)
  :name(name), model(model)
{}

template<typename T>
std::shared_ptr<Uniform<T>> Uniform<T>::makeShared(const char *name, T model)
{
  return std::make_shared<Uniform<T>>(name, model);
}

template<typename T>
void Uniform<T>::update(GLint location)
{
  WARN("Use partial specialisation for update");
}

template<typename T>
T& Uniform<T>::operator=(const T& other)
{
  WARN("Use partial specialisation for copy assignment");
  this->model = other;
  return this->model;
}

template<typename T>
T& Uniform<T>::operator=(T&& other) noexcept
{
  WARN("Use partial specialisation for move assignment");
  this->model = other;
  return this->model;
}

// IMPLEMENTED UNIFORMS UPDATES
//*TESE CAN BE IMPLEMENTED ANYWHERE, IT DOES NOT HAVE TO BE THIS FILE

// === MAT4
template<>
void Uniform<glm::mat4>::update(GLint location)
{
  // reading https://stackoverflow.com/questions/20314202/actual-cost-of-transpose-gl-true-in-gluniformmatrix-functions
  // it seems that GL TRANSPOSE is just copy in transpose order, so i'll keep it hard coded to false
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
}

//? is not providing copy and move assignments overloads for mat4 lead to memory leaks?

// force instantiation of partial specialised templates
template class Uniform<glm::mat4>;

} // namespace glt