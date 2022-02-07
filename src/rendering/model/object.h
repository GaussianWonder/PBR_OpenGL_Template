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
  glt::Uniform<glm::mat4> model;
  GLint modelLoc = -1;

public:
  Object(const char *path, std::shared_ptr<Shader> shader);
  ~Object();

  void draw();

  glm::mat4 getModel();
  void setModel(glm::mat4 model);
};

} // namespace glt

#endif // _OBJECT_H_