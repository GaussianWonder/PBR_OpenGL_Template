#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>

namespace glt {

class Light {
// TODO have a static object that can be rendered as a light bulb indicator
public:
  glm::vec3 position;
  glm::vec3 color;
  Light(glm::vec3 position, glm::vec3 color);
};

} // namespace glt

#endif // _LIGHT_H_