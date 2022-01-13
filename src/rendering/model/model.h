#ifndef __MODEL_H__
#define __MODEL_H__

#include "shader.h"
#include "mesh.h"

#include <GL/glew.h>

#include <string>
#include <vector>

namespace glt {

class Model
{
  // Component meshes - group of objects
  std::vector<glt::Mesh> meshes;
  // Associated textures
  std::vector<glt::Texture> loadedTextures;

  // Does the parsing of the .obj file and fills in the data structure
  bool readOBJ(const std::string &fileName, const std::string &basePath);

  // Retrieves a texture associated with the object - by its name and type
  glt::Texture loadTexture(const std::string &path, const std::string &type);

  // Reads the pixel data from an image file and loads it into the video memory
  GLuint readTextureFromFile(const std::string &file_name);

public:
  ~Model();

  void loadModel(std::string fileName);

  void loadModel(const std::string &fileName, const std::string &basePath);

  void draw(const Shader &shaderProgram);
};

} // namespace glt

#endif // __MODEL_H__
