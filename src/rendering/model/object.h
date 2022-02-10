#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <functional>
#include <utility>
#include <memory>

#include "model.h"
#include "shader.h"
#include "uniform.h"

namespace glt {

class Object : public Model {
  using UniformUpdater = std::function<void(GLint)>;
  using ExternalUpdater = std::pair<GLint, UniformUpdater>;
  using ExternalUpdaters = std::vector<ExternalUpdater>;

  std::shared_ptr<Shader> shader;
  Uniform<glm::mat4> model;
  std::shared_ptr<Uniform<glm::mat4>> view;
  std::shared_ptr<Uniform<glm::mat4>> projection;
  GLint modelLoc = -1;
  GLint viewLoc = -1;
  GLint projectionLoc = -1;

  // poor man's uniform location v_table
  ExternalUpdaters externalUpdates;
public:
  Object(
    const char *path,
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Uniform<glm::mat4>> view,
    std::shared_ptr<Uniform<glm::mat4>> projection,
    ExternalUpdaters externalUpdates
  );
  ~Object();

  void draw();

  glm::mat4 getModel();
  void setModel(glm::mat4 model);
};

} // namespace glt

#endif // _OBJECT_H_