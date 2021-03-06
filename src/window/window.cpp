#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger.h"

#include "global_settings.h"

namespace glt {

Window::Window (const char *title, int width, int height)
  :title(title), width(width), height(height)
{
  this->valid = initOpenGLWindow();
  ASSERT(this->valid == true, "The window should be valid to continue with the window setup and further rendering!");

  if (this->valid)
  {
    this->setup();
  }
}

Window::~Window()
{
  glfwDestroyWindow(this->glWindow);
}

void Window::operator() ()
{
  this->loop();
}

bool Window::isValid()
{
  return this->valid;
}

void Window::setup()
{
  GLERR( glEnable(GL_DEPTH_TEST) ); // enable depth-testing
  GLERR( glDepthFunc(GL_LESS) ); // depth-testing interprets a smaller value as "closer"
  GLERR( glEnable(GL_CULL_FACE) ); // cull face
  GLERR( glCullFace(GL_BACK) ); // cull back face
  GLERR( glFrontFace(GL_CCW) ); // GL_CCW for counter clock-wise
}

void Window::draw()
{
  GLERR( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
  GLERR( glClearColor(0.8, 0.8, 0.8, 1.0) );
  GLERR( glViewport(0, 0, this->retinaWidth, this->retinaHeight) );

  glfwSwapBuffers(this->glWindow);
}

void Window::events()
{
  glfwPollEvents();   
}

void Window::loop()
{
  ASSERT(this->valid == true, "The window should be valid to jump in the rendering loop!");
  if (!this->valid)
    return;

  while (!glfwWindowShouldClose(this->glWindow))
  {
    this->events();
    this->draw();
  }
}

bool Window::initOpenGLWindow()
{
  if (!glfwInit()) {
    FATAL("Could not start GLFW3");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // for multisampling/antialiasing
  glfwWindowHint(GLFW_SAMPLES, 4);

  glWindow = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
  ASSERT(glWindow, "No window handle was returned. You need a window handle for the window to be valid!");
  if (!glWindow) {
    FATAL("Could not open window with GLFW3");
    return false;
  }

  glfwMakeContextCurrent(this->glWindow);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  GlobalSettings *settings = GlobalSettings::instance();
  settings->renderer = reinterpret_cast<const char*>(renderer);
  settings->version = reinterpret_cast<const char*>(version);
  INFO("Renderer {}", renderer);
  INFO("OpenGL version supported {}", version);

  GLint max_vertices, max_components;
  GLERR( glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &max_vertices) );
  GLERR( glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &max_components) );
  INFO("GL_MAX_GEOMETRY_OUTPUT_VERTICES {}", max_vertices);
  INFO("GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS {}", max_components);

  //for RETINA display
  glfwGetFramebufferSize(this->glWindow, &(this->retinaWidth), &(this->retinaHeight));

  return true;
}

void Window::setCursorVisibility(bool visible)
{
  glfwSetInputMode(this->glWindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Window::resetFrameBufferSize(int width, int height)
{
  this->width = width;
  this->height = height;
  glfwSetWindowSize(this->glWindow, this->width, this->height);
  glfwGetFramebufferSize(this->glWindow, &(this->retinaWidth), &(this->retinaHeight));
  GLERR( glViewport(0, 0, this->retinaWidth, this->retinaHeight) );
}

} // namespace glt

