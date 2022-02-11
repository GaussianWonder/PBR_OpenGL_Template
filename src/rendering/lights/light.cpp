#include "light.h"

#include <glm/glm.hpp>

namespace glt {

Light::Light(glm::vec3 position, glm::vec3 color)
  :position(position)
  ,color(color)
{}

} // namespace glt