#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

GLFWwindow* windowInit()
{
  // Init GLFW
  if (!glfwInit())
  {
    std::cout << "ERROR: could not start GLFW3" << std::endl;
    return nullptr;
  };

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create a window
  GLFWwindow* wnd = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);
  if (!wnd)
  {
      std::cout << "ERROR: could not open window with GLFW3" << std::endl;
      glfwTerminate();
      return nullptr;
  }
  glfwMakeContextCurrent(wnd);

  GLenum glContextError = glGetError();
  if (glContextError != GL_NO_ERROR)
  {
      std::cout << "ERROR: OpenGL Error: " << glContextError << std::endl;
      return nullptr;
  }

  // Start GLEW extension handler
  glewExperimental = GL_TRUE;
  GLenum glewInitError = glewInit();
  if (glewInitError != GLEW_OK) {
      std::cout << "ERROR: GLEW Error: " << glewInitError;
      return nullptr;
  }

  // Print relevant info
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << '\n'
    << "Version: " << version << std::endl;

  return wnd;
}

void terminate()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

void render()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void loop()
{
  while (!glfwWindowShouldClose(window))
  {
    render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

int main()
{
  window = windowInit();
  if (window == nullptr)
  {
    std::cout << "ERROR: Could not retrieve a window and an OpenGL context" << std::endl;
    return -1;
  }

  loop();

  terminate();

  return 0;
}