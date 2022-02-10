#include "mesh.h"
#include "logger.h"

namespace glt {

/* Mesh Constructor */
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
  this->vertices = vertices;
  // for (int i=0; i<vertices.size(); ++i) {
  //   auto &vertex = this->vertices[i];
  //   WARN("Vertex tex coord {}: {} {}", i, vertex.TexCoords.x, vertex.TexCoords.y);
  // }
  this->indices = indices;
  this->textures = textures;
  this->setupMesh();
}

Buffers Mesh::getBuffers() {
  return this->buffers;
}

/* Mesh drawing function - also applies associated textures */
void Mesh::draw(Shader &shader)
{
  shader.useShaderProgram();
  //set textures
  for (GLuint i = 0; i < textures.size(); ++i)
  {
    GLERR( glActiveTexture(GL_TEXTURE0 + i) );
    GLERR( glUniform1i(glGetUniformLocation(shader.shaderProgram, this->textures[i].type.c_str()), i) );
    GLERR( glBindTexture(GL_TEXTURE_2D, this->textures[i].id) );
  }

  GLERR( glBindVertexArray(this->buffers.VAO) );
  GLERR( glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0) );
  GLERR( glBindVertexArray(0) );

  for(GLuint i = 0; i < this->textures.size(); ++i)
  {
    GLERR( glActiveTexture(GL_TEXTURE0 + i) );
    GLERR( glBindTexture(GL_TEXTURE_2D, 0) );
  }
}

// Initializes all the buffer objects/arrays
void Mesh::setupMesh()
{
  // Create buffers/arrays
  GLERR( glGenVertexArrays(1, &this->buffers.VAO) );
  GLERR( glGenBuffers(1, &this->buffers.VBO) );
  GLERR( glGenBuffers(1, &this->buffers.EBO) );

  GLERR( glBindVertexArray(this->buffers.VAO) );
  // Load data into vertex buffers
  GLERR( glBindBuffer(GL_ARRAY_BUFFER, this->buffers.VBO) );
  GLERR( glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW) );

  GLERR( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers.EBO) );
  GLERR( glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW) );

  // Set the vertex attribute pointers
  // Vertex Positions
  GLERR( glEnableVertexAttribArray(0) );
  GLERR( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0) );
  // Vertex Normal
  GLERR( glEnableVertexAttribArray(1) );
  GLERR( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal)) );
  // Vertex Texture Coords
  GLERR( glEnableVertexAttribArray(2) );
  GLERR( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords)) );

  GLERR( glBindVertexArray(0) );
}

} // namespace glt
