#include "pointlight.h"

namespace glt {

PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 color)
  :Light(position, color)
  ,constant(constant)
  ,linear(linear)
  ,quadratic(quadratic)
{}

} // namespace glt