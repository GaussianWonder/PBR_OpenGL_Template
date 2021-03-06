#include "shader.h"
#include "model.h"
#include "logger.h"
#include "stb_image.h"
#include "tiny_obj_loader.h"

namespace glt {

Model::~Model() {
  for (size_t i = 0; i < loadedTextures.size(); ++i) {
    glDeleteTextures(1, &loadedTextures.at(i).id);
  }

  for (size_t i = 0; i < meshes.size(); ++i) {
    GLuint VBO = meshes.at(i).getBuffers().VBO;
    GLuint EBO = meshes.at(i).getBuffers().EBO;
    GLuint VAO = meshes.at(i).getBuffers().VAO;
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
  }
}

void Model::loadModel(std::string fileName)
{
  // TODO tini_obj_loader supports async obj loading
  // texture loading can be async too
  // TODO texture and mesh binding MUST be decoupled 
  std::string basePath = fileName.substr(0, fileName.find_last_of('/')) + "/";
  readModel(fileName, basePath);
}

void Model::loadModel(const std::string &fileName, const std::string &basePath)
{
  readModel(fileName, basePath);
}

void Model::draw(glt::Shader &shaderProgram)
{
  shaderProgram.useShaderProgram();
  for (size_t i = 0; i < meshes.size(); ++i) {
    meshes[i].draw(shaderProgram);
  }
}

bool Model::readModel(const std::string &fileName, const std::string &basePath)
{
  INFO("Loading OBJ {}", fileName);
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  int materialId;

  std::string err;
  // std::string wrn;
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials,/* &wrn,*/ &err, fileName.c_str(), basePath.c_str());

  // if (!wrn.empty()) {
  //   WARN("{}", wrn);
  // }

  if (!err.empty()) { // `err` may contain warning message.
    ERROR("Failed to load OBJ {}:\n{}", fileName, err);
    return false;
  }

  if (!ret) {
    FATAL("Failed to load OBJ {}, aborting!", fileName);
    return false;
  }

  DEBUG("# of shapes: {}", shapes.size());
  DEBUG("# of materials: {}", materials.size());

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); ++s) {
    std::vector<glt::Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<glt::Texture> textures;

    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f) {
      size_t fv = shapes[s].mesh.num_face_vertices[f];

      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; ++v) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

        float vx = attrib.vertices[3 * idx.vertex_index + 0];
        float vy = attrib.vertices[3 * idx.vertex_index + 1];
        float vz = attrib.vertices[3 * idx.vertex_index + 2];
        float nx = attrib.normals[3 * idx.normal_index + 0];
        float ny = attrib.normals[3 * idx.normal_index + 1];
        float nz = attrib.normals[3 * idx.normal_index + 2];
        float tx = 0.0f;
        float ty = 0.0f;
        if (idx.texcoord_index != -1) {
          tx = attrib.texcoords[2 * idx.texcoord_index + 0];
          ty = attrib.texcoords[2 * idx.texcoord_index + 1];
        }

        glm::vec3 vertexPosition(vx, vy, vz);
        glm::vec3 vertexNormal(nx, ny, nz);
        glm::vec2 vertexTexCoords(tx, ty);

        glt::Vertex currentVertex;
        currentVertex.Position = vertexPosition;
        currentVertex.Normal = vertexNormal;
        currentVertex.TexCoords = vertexTexCoords;

        vertices.push_back(currentVertex);
        indices.push_back(index_offset + v);
      }

      index_offset += fv;
    }

    // get material id
    // Only try to read materials if the .mtl file is present
    int a = shapes[s].mesh.material_ids.size();
    if (a > 0 && materials.size() > 0) {
      materialId = shapes[s].mesh.material_ids[0];
      if (materialId != -1) {
        glt::Material currentMaterial;
        currentMaterial.ambient = glm::vec3(materials[materialId].ambient[0], materials[materialId].ambient[1], materials[materialId].ambient[2]);
        currentMaterial.diffuse = glm::vec3(materials[materialId].diffuse[0], materials[materialId].diffuse[1], materials[materialId].diffuse[2]);
        currentMaterial.specular = glm::vec3(materials[materialId].specular[0], materials[materialId].specular[1], materials[materialId].specular[2]);

        //ambient texture
        std::string ambientTexturePath = materials[materialId].ambient_texname;
        if (!ambientTexturePath.empty())
        {
          DEBUG("Loading ambient texture");
          glt::Texture currentTexture;
          currentTexture = loadTexture(basePath + ambientTexturePath, "ambientTexture");
          textures.push_back(currentTexture);
        }

        //diffuse texture
        std::string diffuseTexturePath = materials[materialId].diffuse_texname;
        if (!diffuseTexturePath.empty())
        {
          DEBUG("Loading diffuse texture");
          glt::Texture currentTexture;
          currentTexture = loadTexture(basePath + diffuseTexturePath, "diffuseTexture");
          textures.push_back(currentTexture);
        }

        //specular texture
        std::string specularTexturePath = materials[materialId].specular_texname;
        if (!specularTexturePath.empty())
        {
          DEBUG("Loading specular texture");
          glt::Texture currentTexture;
          currentTexture = loadTexture(basePath + specularTexturePath, "specularTexture");
          textures.push_back(currentTexture);
        }

        //metallic texture
        std::string metallicTexturePath = materials[materialId].metallic_texname;
        if (!metallicTexturePath.empty())
        {
          DEBUG("Loading metallic texture");
          glt::Texture currentTexture;
          currentTexture = loadTexture(basePath + metallicTexturePath, "metallicTexture");
          textures.push_back(currentTexture);
        }

        //roughness texture
        std::string roughnessTexturePath = materials[materialId].roughness_texname;
        if (!roughnessTexturePath.empty())
        {
          DEBUG("Loading roughness texture");
          glt::Texture currentTexture;
          currentTexture = loadTexture(basePath + roughnessTexturePath, "roughnessTexture");
          textures.push_back(currentTexture);
        }

        // normal texture
        std::string normalTexturePath = materials[materialId].normal_texname;
        if (!normalTexturePath.empty())
        {
          DEBUG("Loading normal map");
          glt::Texture currentTexture;
          currentTexture = loadTexture(basePath + normalTexturePath, "normalMap");
          textures.push_back(currentTexture);
        }
      }
    }

    meshes.push_back(glt::Mesh(vertices, indices, textures));
  }

  return true;
}

// Retrieves a texture associated with the object - by its name and type
glt::Texture Model::loadTexture(const std::string &path, const std::string &type)
{
  for (size_t i = 0; i < loadedTextures.size(); ++i) {
    if (loadedTextures[i].path == path)
    {
      //already loaded texture
      return loadedTextures[i];
    }
  }

  glt::Texture currentTexture;
  currentTexture.id = readTextureFromFile(path, type);
  currentTexture.type = std::string(type);
  currentTexture.path = path;

  loadedTextures.push_back(currentTexture);

  return currentTexture;
}

// Reads the pixel data from an image file and loads it into the video memory
GLuint Model::readTextureFromFile(const std::string &file_name, const std::string &type)
{
  DEBUG("Reading {} from {}", type, file_name);
  int x, y, n;
  int force_channels = 4;
  unsigned char* image_data = stbi_load(file_name.c_str(), &x, &y, &n, force_channels);
  if (!image_data) {
    ERROR("Failed to load texture {}", file_name);
    return false;
  }
  // NPOT check
  if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
    WARN("Texture {} is not power-of-2 dimensions", file_name);
  }

  int width_in_bytes = x * 4;
  unsigned char *top = NULL;
  unsigned char *bottom = NULL;
  unsigned char temp = 0;
  int half_height = y / 2;

  for (int row = 0; row < half_height; ++row) {
    top = image_data + row * width_in_bytes;
    bottom = image_data + (y - row - 1) * width_in_bytes;
    for (int col = 0; col < width_in_bytes; ++col) {
      temp = *top;
      *top = *bottom;
      *bottom = temp;
      ++top;
      ++bottom;
    }
  }

  GLuint textureID;
  GLERR( glGenTextures(1, &textureID) );
  GLERR( glBindTexture(GL_TEXTURE_2D, textureID) );
  if (type == "normalMap") {
    GLERR( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data) );
  }
  else {
    // color correction and hdr exposure adjustment is done in the shader
    GLERR( glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data) );
  }
  GLERR( glGenerateMipmap(GL_TEXTURE_2D) );

  GLERR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
  GLERR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
  GLERR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR) );
  GLERR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
  GLERR( glBindTexture(GL_TEXTURE_2D, 0) );

  stbi_image_free(image_data);
  DEBUG("ID of texture {} is {}", type, textureID);
  return textureID;
}

} // namespace glt
