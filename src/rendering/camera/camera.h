#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "glm/glm.hpp"

namespace glt {

class Camera {
  glm::vec3 initPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 initTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 initUp = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 up;

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);

  void init();
  void setViewMatrix();

public:
  struct PerspectiveArgs {
    float fov;
    float aspect;
    float near;
    float far;
  };

  struct OrthographicArgs {
    float left;
    float right;
    float bottom;
    float top;
    float near;
    float far;
  };

  enum Move {
    Left, Right, Forward, Backward,
  };

  Camera();
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, PerspectiveArgs args);
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, OrthographicArgs args);

  void reset();
  void reset(glm::vec3 position, glm::vec3 target, glm::vec3 up);

  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();

  void setPerspective(PerspectiveArgs args);
  void setPerspective(OrthographicArgs args);

  glm::mat4 move(Move direction, float speed);
};

} // namespace glt

#endif // _CAMERA_H_