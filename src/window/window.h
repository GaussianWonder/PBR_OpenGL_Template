#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace glt {

class Window {
public:
  Window(const char *title, int width, int height);
  ~Window();

  virtual void operator() ();

  bool valid = false;

protected:
  virtual void setup();
  virtual void draw();
  virtual void events();
  virtual void loop();

private:
  const char *title;
  int width, height;
  int retinaWidth, retinaHeight;
  GLFWwindow* glWindow = NULL;

  bool initOpenGLWindow();
};

} // namespace glt

#endif // _WINDOW_HPP_ 
