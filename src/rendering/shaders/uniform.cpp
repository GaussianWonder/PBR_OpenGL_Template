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
const char * Uniform<T>::getName() { return name; }

template<typename T>
void Uniform<T>::update(GLint location)
{
  DEBUG("Use partial specialisation for update");
}

template<typename T>
T& Uniform<T>::operator=(const T& other)
{
  DEBUG("Use partial specialisation for copy assignment");
  this->model = other;
  return this->model;
}

template<typename T>
T& Uniform<T>::operator=(T&& other) noexcept
{
  DEBUG("Use partial specialisation for move assignment");
  this->model = other;
  return this->model;
}

// IMPLEMENTED UNIFORMS UPDATES
//*TESE CAN BE IMPLEMENTED ANYWHERE, IT DOES NOT HAVE TO BE THIS FILE

// === MAT4
template<>
void Uniform<glm::mat4>::update(GLint location)
{
  GLERR( glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model)) );
}

template<>
glm::mat4& Uniform<glm::mat4>::operator=(const glm::mat4& other)
{
  this->model = other;
  return this->model;
}

template<>
glm::mat4& Uniform<glm::mat4>::operator=(glm::mat4&& other) noexcept
{
  this->model = other;
  return this->model;
}

// === VEC3
template<>
void Uniform<glm::vec3>::update(GLint location)
{
  GLERR( glUniform3fv(location, 1, glm::value_ptr(model)) );
}

template<>
glm::vec3& Uniform<glm::vec3>::operator=(const glm::vec3& other)
{
  this->model = other;
  return this->model;
}

template<>
glm::vec3& Uniform<glm::vec3>::operator=(glm::vec3&& other) noexcept
{
  this->model = other;
  return this->model;
}

// === float
template<>
void Uniform<float>::update(GLint location)
{
  GLERR( glUniform1f(location, model) );
}

template<>
float& Uniform<float>::operator=(const float& other)
{
  this->model = other;
  return this->model;
}

template<>
float& Uniform<float>::operator=(float&& other) noexcept
{
  this->model = other;
  return this->model;
}


//? is not providing copy and move assignments overloads for mat4 lead to memory leaks?

// force instantiation of partial specialised templates

template class Uniform<glm::mat4>;
template class Uniform<glm::vec3>;
template class Uniform<float>;

} // namespace glt