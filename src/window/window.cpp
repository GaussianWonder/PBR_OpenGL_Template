#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace glt {

Window::Window (const char *title, int width, int height)
  :title(title), width(width), height(height)
{
  this->valid = initOpenGLWindow();

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

void Window::setup()
{
}

void Window::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.8, 0.8, 0.8, 1.0);
  glViewport(0, 0, this->retinaWidth, this->retinaHeight);

  glfwSwapBuffers(this->glWindow);
}

void Window::events()
{
  glfwPollEvents();   
}

void Window::loop()
{
  while (!glfwWindowShouldClose(this->glWindow))
  {
    this->draw();
    this->events();
  }
}

bool Window::initOpenGLWindow()
{
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // for multisampling/antialising
  glfwWindowHint(GLFW_SAMPLES, 4);

  glWindow = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
  if (!glWindow) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    return false;
  }

  glfwMakeContextCurrent(this->glWindow);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  //for RETINA display
  glfwGetFramebufferSize(this->glWindow, &(this->retinaWidth), &(this->retinaHeight));

  return true;
}

} // namespace glt

