#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>

#include "window.h"
#include "paths.h"
#include "shader.h"

// #include <ranges>
// #include <vector>

// #include <coroutine>

// struct simple {
//     static inline int x = 0;
//     int id = 0;
//     simple() : id{ x++ } { std::cout << id << " constructed\n"; }
//     simple(simple&&) : id{ x++ } { std::cout << id << " move constructed\n"; }
//     ~simple() { std::cout << id << " destructed\n"; }

//     struct promise_type {
//         simple get_return_object() { return {}; }
//         void return_void() {}
//         void unhandled_exception() { std::terminate(); }
//         auto initial_suspend() noexcept { return std::suspend_never{}; }
//         auto final_suspend() noexcept { return std::suspend_never{}; }
//     };
// };

// simple f() { co_return; }

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

int main(/* int argc, const char * argv[] */)
{
  // std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
  // auto results = numbers
  //   | std::views::filter([](int n){ return n % 2 == 0; })
  //   | std::views::transform([](int n){ return n * 2; });
  // for (auto v: results)
  //   std::cout << v << " ";

  std::cout << PathConcat(ShaderFolder, "/basic/uniform/shader.vert") << PathConcat(ShaderFolder, "/basic/uniform/shader.frag") << std::endl;

  CustomWindow window("Test", 600, 400);
  window();

  return 0;
}
