#define GLEW_STATIC

#include <GL/glew.h>

#include "window.h"
#include "paths.h"
#include "shader.h"
#include "uniform.h"
#include "model.h"

#include "widget_utils.h"
#include "global_context_widget.h"

#include <glm/glm.hpp>                  //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp>   //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp>         //glm extension for accessing the internal data structure of glm types

#include "logger.h"

class CustomWindow : glt::Window
{
public:
  CustomWindow(const char *name, int width, int height)
      :Window(name, width, height)
      ,shader(glt::Shader(
        PathConcat(ShaderFolder, "/basic/shader.vert"),
        PathConcat(ShaderFolder, "/basic/color/texture.frag")))
      ,rotMat(glt::Uniform("rotation", glm::mat4(1.0f)))
  {
    DEBUG("Window construct successful with valid state of {}", this->isValid());
  }

  ~CustomWindow()
  {
    glt::cleanup_widgets();
  }

  void setup() override
  {
    DEBUG("Window setup");

    // Because we can't pass lambdas with captures to calbacks
    //  make a 1:1 relationship between this window and this instance of the class
    glfwSetWindowUserPointer(this->glWindow, this);
    // Because of the lifetime of glfwGetWindowUserPointer is dependant of the lifetime of this class
    //  it is safe to never check it for NULL/nullptr

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);  // cull face
    glCullFace(GL_BACK);     // cull back face
    glFrontFace(GL_CCW);     // GL_CCW for counter clock-wise
    glViewport(0, 0, this->retinaWidth, this->retinaHeight); // TODO update when needed

    cube.loadModel(PathConcat(ModelFolder, "/test_cube/cube.obj"));

    // Now events might be processable on other threads alongside phisycs updates
    glfwSetKeyCallback(this->glWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      INFO("key call on {} with value {}", key, bool((action & GLFW_PRESS) | (action & GLFW_REPEAT)));
      CustomWindow* context = (CustomWindow*) glfwGetWindowUserPointer(window);
      // TODO https://www.glfw.org/docs/3.3/group__keys.html
      // I don't think i need so many keys and we can possibly remove the if statement all throughout with smart branchless programming 
      if (key >= 0 && key < 349) {
        context->pressedKeys[key] = bool((action & GLFW_PRESS) | (action & GLFW_REPEAT));
      }
    });

    // this->rotMat = glt::Uniform("rotation", glm::mat4(1.0f));
    this->shader.useShaderProgram();
    this->rotLoc = glGetUniformLocation(this->shader.shaderProgram, "rotation");
    this->rotMat.update(this->rotLoc);

    INFO("Uniform location is: {}", this->rotLoc);

    glt::init_widgets(this->glWindow);
  }

  void draw() override
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    cube.draw(shader);

    // Prepare rendering widgets
    glt::widget_frame();
    // Draw all widget
    globalWidget.draw();
    // Render drawn widgets
    glt::render_widgets();

    glfwSwapBuffers(this->glWindow);
  }


  void events() override
  {
    glfwPollEvents();
    this->key_processor();
  }

  void operator()() override
  {
    this->setup();
    this->loop();
  }

  void key_processor()
  {
    if (pressedKeys[GLFW_KEY_W]) {
      this->rotMat = glm::rotate(this->rotMat.model, glm::radians(this->angle), glm::vec3(1, 0, 0));
      this->rotMat.update(this->rotLoc); // works only when using one shader (since it is enabled all the time)
    }
    if (pressedKeys[GLFW_KEY_A]) {
      this->rotMat = glm::rotate(this->rotMat.model, glm::radians(this->angle), glm::vec3(0, 1, 0));
      this->rotMat.update(this->rotLoc);
    }
    if (pressedKeys[GLFW_KEY_S]) {
      this->rotMat = glm::rotate(this->rotMat.model, glm::radians(this->angle), glm::vec3(-1, 0, 0));
      this->rotMat.update(this->rotLoc);
    }
    if (pressedKeys[GLFW_KEY_D]) {
      this->rotMat = glm::rotate(this->rotMat.model, glm::radians(this->angle), glm::vec3(0, -1, 0));
      this->rotMat.update(this->rotLoc);
    }
  }

private:
  glt::GlobalContextWidget globalWidget = glt::GlobalContextWidget();
  bool pressedKeys[349] = {false};
  glt::Shader shader;
  glt::Model cube;
  glt::Uniform<glm::mat4> rotMat;
  GLint rotLoc = -1;
  GLfloat angle = 0.15;
};

int main(/* int argc, const char * argv[] */)
{
  glt::Logger::init();
  INFO("Logger init");

  CustomWindow window("OpenGL Template", 600, 600);
  window();

  INFO("Logger destroy");
  glt::Logger::destroy();

  glfwTerminate();
  return 0;
}
