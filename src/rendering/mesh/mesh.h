#ifndef __MESH_H__
#define __MESH_H__

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "shader.h"

#include <string>
#include <vector>

namespace glt {

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture
{
  GLuint id;
  // TODO adapt to PBR
  // ambientTexture, diffuseTexture, specularTexture
  std::string type;
  std::string path;
};

struct Material
{
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

struct Buffers {
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
};

// TODO convert to std::array
class Mesh
{
public:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

  Buffers getBuffers();

  void draw(Shader &shader);

private:
  Buffers buffers;

  // Initializes all the buffer objects/arrays
  void setupMesh();
};

} // namespace glt

#endif // __MESH_H__
