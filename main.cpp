#define GLEW_STATIC

#include <GL/glew.h>

#include "window.h"
#include "paths.h"
#include "shader.h"
#include "uniform.h"
#include "object.h"
#include "global_settings.h"

#include "widget_utils.h"
#include "global_context_widget.h"

#include <glm/glm.hpp>                  //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp>   //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp>         //glm extension for accessing the internal data structure of glm types

#include <vector>
#include <utility>
#include <optional>
#include <memory>

#include "logger.h"

#include "main_camera.h"
#include "skybox.h"

class CustomWindow : glt::Window
{
public:
  using SharedUMat4 = std::shared_ptr<glt::Uniform<glm::mat4>>;
  using SharedShader = std::shared_ptr<glt::Shader>;
 
  CustomWindow(const char *name, int width, int height)
      :Window(name, width, height)
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
    GLERR( glfwSetWindowUserPointer(this->glWindow, this) );
    // Because of the lifetime of glfwGetWindowUserPointer is dependant of the lifetime of this class
    //  it is safe to never check it for NULL/nullptr
    // glfwSwapInterval(0); // request vsync disabled

    GLERR( glEnable(GL_DEBUG_OUTPUT) );
    GLERR( glEnable(GL_DEPTH_TEST) ); // enable depth-testing
    GLERR( glDepthFunc(GL_LESS) );    // depth-testing interprets a smaller value as "closer"
    GLERR( glEnable(GL_CULL_FACE) );  // cull face
    GLERR( glCullFace(GL_BACK) );     // cull back face
    GLERR( glFrontFace(GL_CCW) );     // GL_CCW for counter clock-wise
    GLERR( glViewport(0, 0, this->retinaWidth, this->retinaHeight) ); // TODO update when needed

    this->setCursorVisibility(this->settings->cursor_visible);
    // Initial last mouse is in the center of the window
    lmX = (float) this->retinaWidth / 2;
    lmY = (float) this->retinaHeight / 2;

    glfwSetWindowSizeCallback(this->glWindow, [](GLFWwindow *window, int width, int height) {
      CustomWindow *context = (CustomWindow*) glfwGetWindowUserPointer(window);
      context->resetFrameBufferSize(width, height);

      context->camera.value().setPerspective((glt::Camera::PerspectiveArgs{
        .fov = glm::radians(45.0f),
        .aspect = (float) context->retinaWidth / (float) context->retinaHeight,
        .near = 0.1f,
        .far = 1000.0f
      }));
    });

    // Now events might be processable on other threads alongside phisycs updates
    glfwSetKeyCallback(this->glWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      CustomWindow* context = (CustomWindow*) glfwGetWindowUserPointer(window);
      // I don't think i need so many keys and we can possibly remove the if statement all throughout with smart branchless programming 
      if (key >= 0 && key < 349) {
        context->pressedKeys[key] = bool((action & GLFW_PRESS) | (action & GLFW_REPEAT));
        if (key == GLFW_KEY_ESCAPE && action & GLFW_PRESS) {
          context->settings->cursor_visible = !context->settings->cursor_visible;
          context->setCursorVisibility(context->settings->cursor_visible);
        }
      }
    });

    glfwSetCursorPosCallback(this->glWindow, [](GLFWwindow* window, double xpos, double ypos) {
      CustomWindow* context = (CustomWindow*) glfwGetWindowUserPointer(window);

      float xoffset = xpos - context->lmX;
      float yoffset = context->lmY - ypos;
      context->lmX = xpos;
      context->lmY = ypos;

      // abort just after setting last xpos and ypos
      if (context->settings->cursor_visible)
        return;

      float delta = context->settings->deltaFrameTime;
      float sensitivity = context->settings->sensitivity;

      xoffset *= sensitivity;
      yoffset *= sensitivity;

      context->camera.value().rotate(xoffset, -yoffset);
    });

    this->camera.emplace(
      this->view,
      this->projection
    );

    this->shader->useShaderProgram();

    camera.value().setPerspective(glt::Camera::PerspectiveArgs{
      .fov = glm::radians(45.0f),
      .aspect = (float) this->retinaWidth / (float) this->retinaHeight,
      .near = 0.1f,
      .far = 1000.0f
    });

    GLuint shaderProgram = this->shader->shaderProgram;
    GLint cameraPositionUniLoc = glGetUniformLocation(
      shaderProgram,
      this->camera->getCameraPositionUni()->getName()
    );
    GLint gammaUniLoc = glGetUniformLocation(
      shaderProgram,
      this->settings->gamma->getName()
    );
    GLint exposureUniLoc = glGetUniformLocation(
      shaderProgram,
      this->settings->exposure->getName()
    );
    std::vector<std::pair<GLint, std::function<void(GLint)>>> uniform_updaters = {
      {
        cameraPositionUniLoc,
        [this](GLint location) {
          this->camera->getCameraPositionUni()->update(location);
        }
      },
      {
        gammaUniLoc,
        [this](GLint location) {
          this->settings->gamma->update(location);
        }
      },
      {
        exposureUniLoc,
        [this](GLint location) {
          this->settings->exposure->update(location);
        }
      }
    };
    this->cube.emplace(
      PathConcat(ModelFolder, "/icosphere/icosphere.obj"),
      this->shader,
      this->view,
      this->projection,
      uniform_updaters
    );

    this->skyboxShader->useShaderProgram();

    skybox.load({
      PathConcat(TextureFolder, "/skybox/right.tga"),
      PathConcat(TextureFolder, "/skybox/left.tga"),
      PathConcat(TextureFolder, "/skybox/top.tga"),
      PathConcat(TextureFolder, "/skybox/bottom.tga"),
      PathConcat(TextureFolder, "/skybox/back.tga"),
      PathConcat(TextureFolder, "/skybox/front.tga")
    });

    glt::init_widgets(this->glWindow);
  }

  void draw() override
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    cube.value().draw();

    skybox.draw(skyboxShader, view->model, projection->model);

    // Prepare rendering widgets
    glt::widget_frame();
    // Draw all widget
    globalWidget.draw();
    // Render drawn widgets
    glt::render_widgets();

    glfwSwapBuffers(this->glWindow);

    float currentFrameTime = glfwGetTime();
    settings->deltaFrameTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
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
      camera.value().move(glt::Camera::Move::Forward, settings->move_speed * settings->deltaFrameTime);
    }
    if (pressedKeys[GLFW_KEY_A]) {
      camera.value().move(glt::Camera::Move::Left, settings->move_speed * settings->deltaFrameTime);
    }
    if (pressedKeys[GLFW_KEY_S]) {
      camera.value().move(glt::Camera::Move::Backward, settings->move_speed * settings->deltaFrameTime);
    }
    if (pressedKeys[GLFW_KEY_D]) {
      camera.value().move(glt::Camera::Move::Right, settings->move_speed * settings->deltaFrameTime);
    }
    if (pressedKeys[GLFW_KEY_SPACE]) {
      camera.value().move(glt::Camera::Move::Up, settings->move_speed * settings->deltaFrameTime);
    }
    if (pressedKeys[GLFW_KEY_LEFT_SHIFT]) {
      camera.value().move(glt::Camera::Move::Down, settings->move_speed * settings->deltaFrameTime);
    }
    if (pressedKeys[GLFW_KEY_R]) {
      this->cube.value().setModel(glm::rotate(
        this->cube.value().getModel(),
        glm::radians(settings->move_speed * settings->deltaFrameTime),
        glm::vec3(0.25f, 0.5f, 0.25f)
      ));
    }
  }

  glt::GlobalSettings *settings = glt::GlobalSettings::instance();
  glt::GlobalContextWidget globalWidget = glt::GlobalContextWidget();
  bool pressedKeys[349] = {false};

  SharedShader shader = std::make_shared<glt::Shader>(
    PathConcat(ShaderFolder, "/pbr_test/pbr_t.vert"),
    PathConcat(ShaderFolder, "/pbr_test/pbr_t.geom"),
    PathConcat(ShaderFolder, "/pbr_test/pbr_t.frag"));
  std::optional<glt::Object> cube;

  SharedUMat4 view = glt::Uniform<glm::mat4>::makeShared(
    "view",
    glm::mat4(1.0f));
  SharedUMat4 projection = glt::Uniform<glm::mat4>::makeShared(
    "projection",
    glm::mat4(1.0f));

  std::optional<glt::MainCamera> camera;

  // Last mouse X/Y & Current mouse X/Y
  // Pointer to these are safe to be passed while considering the lifetime of the window
  float lmX = 0.0f;
  float lmY = 0.0f;
  float cmX = 0.0f;
  float cmY = 0.0f;

  // used to calculate delta frame time
  float lastFrameTime = 0.0f;

  SharedShader skyboxShader = std::make_shared<glt::Shader>(
    PathConcat(ShaderFolder, "/skybox/shader.vert"),
    PathConcat(ShaderFolder, "/skybox/shader.frag"));
  glt::Skybox skybox;
};

int main(/* int argc, const char * argv[] */)
{
  glt::Logger::init();
  INFO("Logger init");

  CustomWindow window("OpenGL Template", 1000, 800);
  window();

  INFO("Logger destroy");
  glt::Logger::destroy();

  glfwTerminate();
  return 0;
}
