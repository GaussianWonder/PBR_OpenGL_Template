#ifndef _LIGHTSTRIP_H_
#define _LIGHTSTRIP_H_

#include <GL/glew.h>

#include "spotlight.h"
#include "pointlight.h"

#include <vector>
#include <glm/glm.hpp>

namespace glt {

class LightStrip {
  std::vector<PointLight> pointLights = {};

  GLint pointLLoc = -1;
  std::vector<glm::vec3> pointLPositions = {};
  float *pointLPositionsFlat = nullptr;

  GLint pointLCol = -1;
  std::vector<glm::vec3> pointLColors = {};
  float *pointLColorsFlat = nullptr;

  GLint pointLLin = -1;
  std::vector<float> pointLLinears = {};
  float *pointLLinearsFlat = nullptr;

  GLint pointLQuad = -1;
  std::vector<float> pointLQuadratics = {};
  float *pointLQuadraticsFlat = nullptr;

public:
  void addPointLight(glm::vec3 position, glm::vec3 color = glm::vec3(1.0f), float constant = 1.0f, float linear = 0.7, float quadratic = 1.8f);
  void setLightInformation(GLuint shaderProgram);
  void updateLightInformation(GLuint shaderProgram);
  void debugWidget(GLuint shaderProgram);
};

} // namespace glt

#endif // _LIGHTSTRIP_H_