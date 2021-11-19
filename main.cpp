#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>

#include "window.h"
#include "paths.h"
#include "shader.h"

class CustomWindow: glt::Window
{
public:
  CustomWindow(const char *name, int width, int height)
    :Window(name, width, height)
    ,shader(std::move(
      glt::Shader(
        PathConcat(ShaderFolder, "/basic/uniform/shader.vert"),
        PathConcat(ShaderFolder, "/basic/uniform/shader.frag")
      )
    ))
  {}

  void setup()
  {
    std::cout << "SETUP"<<std::endl;
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);

    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    //vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    std::cout << "VBO VAO indexes" << verticesVBO << ' ' << objectVAO << std::endl;
  }

  void draw()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glViewport(0, 0, this->retinaWidth, this->retinaHeight);

    shader.useShaderProgram();
    //TODO test normal uniforms
    //TODO make bulk uniforms
    glBindVertexArray(objectVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(this->glWindow);
  }

  void operator() ()
  {
    this->setup();
    this->loop();
  }

private:
  GLfloat vertexData[9] = {
    0.0f,	0.5f,	0.0f,
    -0.5f, -0.5f,	0.0f,
    0.5f, -0.5f,	0.0f
  };
  GLuint verticesVBO;
  GLuint objectVAO;
  glt::Shader shader;
};

#include "spdlog/spdlog.h"

int main(/* int argc, const char * argv[] */)
{
  spdlog::info("Welcome to spdlog!");
  spdlog::error("Some error message with arg: {}", 1);

  spdlog::warn("Easy padding in numbers like {:08d}", 12);
  spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  spdlog::info("Support for floats {:03.2f}", 1.23456);
  spdlog::info("Positional args are {1} {0}..", "too", "supported");
  spdlog::info("{:<30}", "left aligned");

  // CustomWindow window("Test", 600, 400);
  // window();

  return 0;
}
