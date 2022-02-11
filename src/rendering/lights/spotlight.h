#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "light.h"
#include <glm/glm.hpp>

namespace glt {

class SpotLight : public Light {
public:
  glm::vec3 direction;
  float cutoff;

  SpotLight(glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec3 color);
};

} // namespace glt

#endif // _SPOTLIGHT_H_