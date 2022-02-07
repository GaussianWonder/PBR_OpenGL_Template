#include <GL/glew.h>
#include <glm/glm.hpp>

#include "object.h"
#include "uniform.h"

#include "logger.h"

namespace glt {

Object::Object(const char *path, std::shared_ptr<Shader> shader)
  :Model()
  ,shader(shader)
  ,model(glt::Uniform("model", glm::mat4(1.0f)))
{
  loadModel(path);
  this->shader->useShaderProgram();
  this->modelLoc = glGetUniformLocation(this->shader->shaderProgram, model.getName());
  this->model.update(this->modelLoc);
}

Object::~Object()
{
}

void Object::draw()
{
  this->Model::draw(*shader);
}

glm::mat4 Object::getModel()
{
  return model.model;
}

void Object::setModel(glm::mat4 model)
{
  this->model = model;
  this->model.update(this->modelLoc);
}

};