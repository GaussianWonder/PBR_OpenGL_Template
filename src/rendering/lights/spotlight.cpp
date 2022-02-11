#include "spotlight.h"

namespace glt {

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec3 color)
  :Light(position, color)
  ,direction(direction)
  ,cutoff(cutoff)
{}

} // namespace glt
