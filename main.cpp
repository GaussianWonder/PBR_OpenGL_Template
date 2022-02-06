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

#include "camera.h"

class CustomWindow : glt::Window
{
public:
  CustomWindow(const char *name, int width, int height)
      :Window(name, width, height)
      ,shader(glt::Shader(
        PathConcat(ShaderFolder, "/basic/camera/shader.vert"),
        PathConcat(ShaderFolder, "/basic/camera/shader.frag")))
      ,view(glt::Uniform("view", glm::mat4(1.0f)))
      ,projection(glt::Uniform("projection", glm::mat4(1.0f)))
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

    this->shader.useShaderProgram();

    glt::init_widgets(this->glWindow);

    camera.setPerspective(glt::Camera::PerspectiveArgs{
      .fov = glm::radians(45.0f),
      .aspect = (float) this->retinaWidth / (float) this->retinaHeight,
      .near = 0.1f,
      .far = 1000.0f
    });

    this->viewLoc = glGetUniformLocation(this->shader.shaderProgram, "view");
    this->projectionLoc = glGetUniformLocation(this->shader.shaderProgram, "projection");

    this->view = camera.getViewMatrix();
    this->projection = camera.getProjectionMatrix();

    this->view.update(this->viewLoc);
    this->projection.update(this->projectionLoc);
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
      this->view = camera.move(glt::Camera::Move::Forward, 0.01f);
      this->view.update(this->viewLoc);
    }
    if (pressedKeys[GLFW_KEY_A]) {
      this->view = camera.move(glt::Camera::Move::Left, 0.01f);
      this->view.update(this->viewLoc);
    }
    if (pressedKeys[GLFW_KEY_S]) {
      this->view = camera.move(glt::Camera::Move::Backward, 0.01f);
      this->view.update(this->viewLoc);
    }
    if (pressedKeys[GLFW_KEY_D]) {
      this->view = camera.move(glt::Camera::Move::Right, 0.01f);
      this->view.update(this->viewLoc);
    }
  }

private:
  glt::GlobalContextWidget globalWidget = glt::GlobalContextWidget();
  bool pressedKeys[349] = {false};
  glt::Shader shader;
  glt::Model cube;

  glt::Camera camera = glt::Camera();
  glt::Uniform<glm::mat4> view;
  glt::Uniform<glm::mat4> projection;
  GLint viewLoc = -1;
  GLint projectionLoc = -1;
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
