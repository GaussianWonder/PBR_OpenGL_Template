#define GLEW_STATIC

#include <iostream>

#include "window.h"
#include "paths.h"

using namespace glt;

int main(int argc, const char * argv[])
{
  std::cout << PathConcat(ShaderFolder, "/basic/shader.frag");
  return 0;
  Window window("Test", 600, 400);
  window();
  return 0;
}
