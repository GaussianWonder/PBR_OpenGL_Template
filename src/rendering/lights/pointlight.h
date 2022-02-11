#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include "light.h"
#include <glm/glm.hpp>

namespace glt {

class PointLight : public Light {
public:
  float constant;
  float linear;
  float quadratic;

  PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 color);
};

} // namespace glt

#endif // _POINTLIGHT_H_