from conans import ConanFile, tools
from conan.tools.cmake import CMake
import shutil
import os

# defining the function to ignore the files
# if present in any folder
def ignore_files(dir, files):
  return [f for f in files if os.path.isfile(os.path.join(dir, f))]

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

  def source(self):
    workingDir = os.getcwd()
    bindingDir = "%s/../../bindings" % workingDir

    # Perform all clone operations first
    self.output.info("Cloning CedricGuillemet/ImGuizmo...")
    git = tools.Git(folder="imguizmo")
    git.clone("https://github.com/CedricGuillemet/ImGuizmo.git", "master")
    imguizmoDir = "%s/imguizmo" % workingDir
    # Copy the folder structure
    shutil.rmtree(bindingDir)
    shutil.copytree(workingDir, bindingDir, ignore=ignore_files)
    # Copy required files / dependency

    self.output.info("Copying imguizmo sources...")
    imguizmo_files = ["ImZoomSlider.h", "ImSequencer.h", "ImSequencer.cpp", "ImGuizmo.h", "ImGuizmo.cpp", "ImGradient.h", "ImGradient.cpp", "ImCurveEdit.h", "ImCurveEdit.cpp", "GraphEditor.h", "GraphEditor.cpp"]
    for file in imguizmo_files:
      src = "%s/%s" % (imguizmoDir, file)
      dst = "%s/imguizmo/%s" % (bindingDir, file)
      shutil.copyfile(src, dst)

    # Due to some weird referencing / linking error, i will remove everything since relevant content is copied to /binding anyways
    self.output.info("Clearing the dependency folder...")
    shutil.rmtree(workingDir)

  def imports(self):
    self.output.info("Importing imgui bindings...")
    imgui_files = ["imgui_impl_glfw.cpp", "imgui_impl_opengl3.cpp", "imgui_impl_opengl3_loader.h", "imgui_impl_glfw.h", "imgui_impl_opengl3.h"]
    for file in imgui_files:
      self.copy(file, "../bindings/imgui", "./res/bindings")

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()
