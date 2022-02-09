#include "camera.h"

#include "glm/glm.hpp" //core glm functionality
#include "glm/gtc/matrix_transform.hpp" //glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp" //glm extension for computing inverse matrices
#include "glm/gtc/type_ptr.hpp" //glm extension for accessing the internal data structure of glm types

#include "logger.h"

namespace glt {

void Camera::onViewChanged(){}
void Camera::onProjectionChanged(){}

void Camera::setAxisDirections()
{
  relDirection = glm::normalize(position - target);
  relRight = glm::normalize(glm::cross(up, relDirection));
  relUp = glm::cross(relDirection, relRight);
}

void Camera::setViewMatrix()
{
  view = glm::lookAt(position, target, relUp);
  onViewChanged();
}

void Camera::init()
{
  initPosition = position;
  initTarget = target;
  initUp = up;

  setAxisDirections();
  setViewMatrix();
}

Camera::Camera()
  :position(glm::vec3(0.0f, 0.0f, -10.0f))
  ,target(glm::vec3(0.0f, 0.0f, 0.0f))
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
  projection = glm::perspective(args.fov, args.aspect, args.near, args.far);
  onProjectionChanged();
}

void Camera::setPerspective(Camera::OrthographicArgs args)
{
  projection = glm::ortho(args.left, args.right, args.bottom, args.top, args.near, args.far);
  onProjectionChanged();
}

void Camera::reset()
{
  position = initPosition;
  target = initTarget;
  up = initUp;
  setAxisDirections();
  setViewMatrix();
}

void Camera::reset(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
  this->position = position;
  this->target = target;
  this->up = up;
  setAxisDirections();
  setViewMatrix();
}

glm::mat4 Camera::move(Camera::Move direction, float speed)
{
  switch (direction) {
  case Camera::Move::Left:
    position -= speed * relRight;
    target -= speed * relRight;
    break;
  case Camera::Move::Right:
    position += speed * relRight;
    target += speed * relRight;
    break;
  case Camera::Move::Forward:
    position -= speed * relDirection;
    target -= speed * relDirection;
    break;
  case Camera::Move::Backward:
    position += speed * relDirection;
    target += speed * relDirection;
    break;
  case Camera::Move::Up:
    position += speed * relUp;
    target += speed * relUp;
    break;
  case Camera::Move::Down:
    position -= speed * relUp;
    target -= speed * relUp;
    break;
  }
  setViewMatrix();
  return view;
}

glm::mat4 Camera::rotate(float xoffset, float yoffset)
{
  yaw   += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
    pitch =  89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  relDirection = glm::normalize(glm::vec3(
    cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
    sin(glm::radians(pitch)),
    sin(glm::radians(yaw)) * cos(glm::radians(pitch))
  ));

  target = position - (10.0f * relDirection);

  setAxisDirections();
  setViewMatrix();
  return view;

  yaw += xoffset;
  pitch += yoffset;

  target.x = position.x + sin(pitch);
  target.z = position.z - cos(pitch);
  target.y = position.y + sin(yaw);

  setAxisDirections();
  setViewMatrix();
  return view;
}

} // namespace glt