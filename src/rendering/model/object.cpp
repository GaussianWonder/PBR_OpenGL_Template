#include <GL/glew.h>
#include <glm/glm.hpp>

#include "object.h"
#include "uniform.h"

#include "logger.h"

namespace glt {

using UniformUpdater = std::function<void(GLint)>;
using ExternalUpdater = std::pair<GLint, UniformUpdater>;
using ExternalUpdaters = std::vector<ExternalUpdater>;

Object::Object(
  const char *path,
  std::shared_ptr<Shader> shader,
  std::shared_ptr<Uniform<glm::mat4>> view,
  std::shared_ptr<Uniform<glm::mat4>> projection,
  ExternalUpdaters externalUpdates
)
  :Model()
  ,shader(shader)
  ,model(glt::Uniform("model", glm::mat4(1.0f)))
  ,view(view)
  ,projection(projection)
  ,externalUpdates(externalUpdates)
{
  shader->useShaderProgram();
  loadModel(path);
  GLuint shaderP = this->shader->shaderProgram;
  modelLoc = glGetUniformLocation(shaderP, model.getName());
  viewLoc = glGetUniformLocation(shaderP, view->getName());
  projectionLoc = glGetUniformLocation(shaderP, projection->getName());
}

Object::~Object()
{
  FATAL("DESTRUCOTR");
  // just the default destructor for now
}

void Object::draw()
{
  shader->useShaderProgram();
  std::size_t updaterCount = externalUpdates.size();
  for (std::size_t i = 0; i < updaterCount; ++i) {
    auto &updater = externalUpdates[i];
    updater.second(updater.first);
  }
  model.update(modelLoc);
  view->update(viewLoc);
  projection->update(projectionLoc);
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