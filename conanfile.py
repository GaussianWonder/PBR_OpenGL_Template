from conans import ConanFile
from conan.tools.cmake import CMake

class OpenGLTemplateConan(ConanFile):
  name = "opengl_template"
  settings = "os", "compiler", "build_type", "arch"
  requires = (
    "glfw/3.3.6",
    "opengl/system",
    "glew/2.2.0",
    "glm/0.9.9.5",
    "ms-gsl/3.1.0",
    "spdlog/1.9.2",
    "tinyobjloader/1.0.6",
    "stb/cci.20210713",
    "imgui/1.86",
  )
  generators = "CMakeToolchain", "CMakeDeps"

  def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()