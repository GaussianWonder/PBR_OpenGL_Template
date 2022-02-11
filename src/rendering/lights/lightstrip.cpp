#include <GL/glew.h>

#include "lightstrip.h"
#include "logger.h"
#include "global_settings.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/gtc/type_ptr.hpp"

#include <string>

namespace glt {

void LightStrip::addPointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic)
{
  pointLights.emplace_back(position, constant, linear, quadratic, color);
}

void LightStrip::setLightInformation(GLuint shaderProgram)
{
  const size_t numLights = pointLights.size();
  for (size_t i = 0; i < numLights; ++i) {
    auto &light = pointLights[i];
    pointLPositions.push_back(light.position);
    pointLColors.push_back(light.color);
    pointLLinears.push_back(light.linear);
    pointLQuadratics.push_back(light.quadratic);
  }
  pointLPositionsFlat = &pointLPositions[0].x;
  pointLColorsFlat = &pointLColors[0].x;
  pointLLinearsFlat = &pointLLinears[0];
  pointLQuadraticsFlat = &pointLQuadratics[0];

  pointLLoc = glGetUniformLocation(
    shaderProgram,
    "pointLPos"
  );

  pointLCol = glGetUniformLocation(
    shaderProgram,
    "pointLCol"
  );

  pointLLin = glGetUniformLocation(
    shaderProgram,
    "pointLLin"
  );

  pointLQuad = glGetUniformLocation(
    shaderProgram,
    "pointLQuad"
  );
}

void LightStrip::updateLightInformation(GLuint shaderProgram)
{
  GLERR( glUseProgram(shaderProgram) );
  const GLsizei numLights = pointLights.size();
  GLERR( glUniform3fv(pointLLoc, numLights, pointLPositionsFlat) );
  GLERR( glUniform3fv(pointLCol, numLights, pointLColorsFlat) );
  GLERR( glUniform1fv(pointLLin, numLights, pointLLinearsFlat) );
  GLERR( glUniform1fv(pointLQuad, numLights, pointLQuadraticsFlat) );
}

void LightStrip::debugWidget(GLuint shaderProgram)
{
  if (GlobalSettings::unwrap()->render_type == RenderType::Debug) {
    bool updated = false;
    const size_t numLights = pointLights.size();

    ImGui::Begin("Light Strip");
    ImGui::Text("%ld point lights", numLights);

    size_t i = 0;
    for(PointLight &pl : pointLights) {
      ImGui::Text("Point light %ld", i);
      std::string l = "Linear";
      std::string q = "Quadratic";
      std::string p = "Position xyz";
      l += std::to_string(i);
      q += std::to_string(i);
      p += std::to_string(i);

      const bool lineUsed = ImGui::SliderFloat(l.c_str(), &(pl.linear), 0.0000001f, 1.0f, "%.4f", 1.0f);
      const bool quadUsed = ImGui::SliderFloat(q.c_str(), &(pl.quadratic), 0.0000001f, 2.0f, "%.6f", 1.0f);
      const bool posUsed = ImGui::InputFloat3(p.c_str(), glm::value_ptr(pl.position), "%.3f");
      updated = updated | lineUsed | quadUsed | posUsed;
      ++i;
    }

    ImGui::End();

    if (updated) {
      pointLPositions.clear();
      pointLColors.clear();
      pointLLinears.clear();
      pointLQuadratics.clear();
      setLightInformation(shaderProgram);
      updateLightInformation(shaderProgram);
    }
  }
}

} // namespace glt
