#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>

#include "model.h"
#include "shader.h"
#include "uniform.h"

namespace glt {

class Object : public Model {
  std::shared_ptr<Shader> shader;
  Uniform<glm::mat4> model;
  std::shared_ptr<Uniform<glm::mat4>> view;
  std::shared_ptr<Uniform<glm::mat4>> projection;
  GLint modelLoc = -1;
  GLint viewLoc = -1;
  GLint projectionLoc = -1;

public:
  Object(
    const char *path,
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Uniform<glm::mat4>> view,
    std::shared_ptr<Uniform<glm::mat4>> projection
  );
  ~Object();

  void draw();

  glm::mat4 getModel();
  void setModel(glm::mat4 model);
};

} // namespace glt

#endif // _OBJECT_H_