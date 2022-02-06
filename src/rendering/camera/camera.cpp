#include "camera.h"

#include "glm/glm.hpp" //core glm functionality
#include "glm/gtc/matrix_transform.hpp" //glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp" //glm extension for computing inverse matrices
#include "glm/gtc/type_ptr.hpp" //glm extension for accessing the internal data structure of glm types

#include "logger.h"

namespace glt {

void Camera::setViewMatrix()
{
  view = glm::lookAt(position, target, up);
}

void Camera::init()
{
  initPosition = position;
  initTarget = target;
  initUp = up;
  setViewMatrix();
}

Camera::Camera()
  :position(glm::vec3(0.0f, 0.0f, 0.0f))
  ,target(glm::vec3(0.0f, 0.0f, 10.0f))
  ,up(glm::vec3(0.0f, 1.0f, 0.0f))
{
  init();
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
  :position(position)
  ,target(target)
  ,up(up)
{
  init();
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, Camera::PerspectiveArgs args)
  :position(position)
  ,target(target)
  ,up(up)
{
  init();
  setPerspective(args);
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, Camera::OrthographicArgs args)
  :position(position)
  ,target(target)
  ,up(up)
{
  init();
  setPerspective(args);
}

glm::mat4 Camera::getViewMatrix()
{
  return view;
}

glm::mat4 Camera::getProjectionMatrix()
{
  return projection;
}

void Camera::setPerspective(Camera::PerspectiveArgs args)
{
  INFO("GOOD");
  projection = glm::perspective(args.fov, args.aspect, args.near, args.far);
}

void Camera::setPerspective(Camera::OrthographicArgs args)
{
  FATAL("WRONG");
  projection = glm::ortho(args.left, args.right, args.bottom, args.top, args.near, args.far);
}

void Camera::reset()
{
  position = initPosition;
  target = initTarget;
  up = initUp;
  setViewMatrix();
}

void Camera::reset(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
  this->position = position;
  this->target = target;
  this->up = up;
  setViewMatrix();
}

glm::mat4 Camera::move(Camera::Move direction, float speed)
{
  switch (direction) {
  case Camera::Move::Left:
    position += glm::vec3(-speed, 0.0f, 0.0f);
    target += glm::vec3(-speed, 0.0f, 0.0f);
    break;
  case Camera::Move::Right:
    position += glm::vec3(speed, 0.0f, 0.0f);
    target += glm::vec3(speed, 0.0f, 0.0f);
    break;
  case Camera::Move::Forward:
    position += glm::vec3(0.0f, 0.0f, speed);
    target += glm::vec3(0.0f, 0.0f, speed);
    break;
  case Camera::Move::Backward:
    position += glm::vec3(0.0f, 0.0f, -speed);
    target += glm::vec3(0.0f, 0.0f, -speed);
    break;
  }
  setViewMatrix();
  return view;
}

} // namespace glt