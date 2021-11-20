#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLEW_STATIC

// #include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace glt {

class Window {
public:
  Window(const char *title, int width, int height);
  ~Window();

  virtual void operator() ();

  bool isValid();

protected:
  virtual void setup();
  virtual void draw();
  virtual void events();
  virtual void loop();

  const char *title;
  int width, height;
  int retinaWidth, retinaHeight;
  GLFWwindow* glWindow = NULL;
private:
  bool valid = false;
  bool initOpenGLWindow();
};

} // namespace glt

#endif // _WINDOW_H_ 
