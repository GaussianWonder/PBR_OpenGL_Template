#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "shader.h"

namespace glt {

class Skybox
{
  GLuint vao;
  GLuint vbo;
  GLuint texture;
  GLuint loadTextures(std::vector<const GLchar*> cubemapFaces);
  void init();

public:
  Skybox();
  void load(std::vector<const GLchar*> cubemapFaces);
  void draw(std::shared_ptr<glt::Shader> shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
  GLuint getTextureID();
};

} // namespace glt

#endif // _SKYBOX_H_