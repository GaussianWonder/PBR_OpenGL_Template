#define GLEW_STATIC

#include <GL/glew.h>

#include "window.h"
#include "paths.h"
#include "shader.h"
#include "uniform.h"

#include "logger.h"

class CustomWindow: glt::Window
{
public:
  CustomWindow(const char *name, int width, int height)
    :Window(name, width, height)
    ,shader(glt::Shader(
        PathConcat(ShaderFolder, "/basic/shader.vert"),
        PathConcat(ShaderFolder, "/basic/shader.frag")
      ))
  {
    DEBUG("Window construct successful with valid state of {}", this->isValid());
  }

  void setup() override
  {
    DEBUG("Window setup");

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

    DEBUG("VBO at {}, VAO at {}", verticesVBO, objectVAO);
  }

  void draw() override
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glViewport(0, 0, this->retinaWidth, this->retinaHeight);

    shader.useShaderProgram();
    glBindVertexArray(objectVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(this->glWindow);
  }

  void operator() () override
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
  GLuint verticesVBO = 0;
  GLuint objectVAO = 0;
  glt::Shader shader;
};

int main(/* int argc, const char * argv[] */)
{
  glt::Logger::init();
  INFO("Logger init");

  CustomWindow window("OpenGL Template", 600, 400);
  window();

  INFO("Logger destroy");
  glt::Logger::destroy();

  glfwTerminate();
  return 0;
}
