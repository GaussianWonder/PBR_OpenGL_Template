#include "skybox.h"

#include <GL/glew.h>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"

#include "logger.h"

namespace glt {

Skybox::Skybox()
{
}

void Skybox::load(std::vector<const GLchar*> cubemapFaces)
{
  texture = loadTextures(cubemapFaces);
  init();
}

void Skybox::draw(std::shared_ptr<glt::Shader> shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
  shader->useShaderProgram();

  //set the view and projection matrices
  glm::mat4 transformedView = glm::mat4(glm::mat3(viewMatrix));
  glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(transformedView));
  glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

  glDepthFunc(GL_LEQUAL);

  glBindVertexArray(vao);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(shader->shaderProgram, "skybox"), 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  glDepthFunc(GL_LESS);
}

GLuint Skybox::loadTextures(std::vector<const GLchar*> cubemapFaces)
{
  DEBUG("Loading skybox textures");

  GLuint textureID;
  glGenTextures(1, &textureID);
  glActiveTexture(GL_TEXTURE0);

  int width,height, n;
  unsigned char* image;
  int force_channels = 3;

  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
  for(GLuint i = 0; i < cubemapFaces.size(); i++)
  {
    image = stbi_load(cubemapFaces[i], &width, &height, &n, force_channels);
    if (!image) {
      ERROR("Failed to load texture {}", cubemapFaces[i]);
      return false;
    }
    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
      GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
    );
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  return textureID;
}

void Skybox::init()
{
  GLfloat vertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
  };

  glGenVertexArrays(1, &(vao));
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

  glBindVertexArray(0);
}

GLuint Skybox::getTextureID()
{
  return texture;
}

} // namespace glt