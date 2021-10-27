#define GLEW_STATIC

#include "window.h"

using namespace glt;

int main(int argc, const char * argv[])
{
  glt::Window window("Test", 600, 400);
  window();
  return 0;
}
