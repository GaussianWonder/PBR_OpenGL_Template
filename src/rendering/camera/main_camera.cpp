#include "main_camera.h"

using SharedMat4Uni = std::shared_ptr<glt::Uniform<glm::mat4>>;
using SharedVec3Uni = std::shared_ptr<glt::Uniform<glm::vec3>>;

namespace glt {

void MainCamera::onViewChanged() {
  (*positionUni) = position; // ? when just rotating the camera this operation is useless
  (*viewUni) = getViewMatrix();
};

void MainCamera::onProjectionChanged() {
  (*projectionUni) = getProjectionMatrix();
}

MainCamera::MainCamera(SharedMat4Uni viewUni, SharedMat4Uni projectionUni)
  :Camera()
  ,viewUni(viewUni)
  ,projectionUni(projectionUni)
  ,positionUni(Uniform<glm::vec3>::makeShared(
    "cameraPosition",
    glm::vec3(0.0f, 0.0f, -10.0f)))
{}

SharedMat4Uni MainCamera::getViewUni() {
  return viewUni;
}

SharedMat4Uni MainCamera::getProjectionUni() {
  return projectionUni;
}

SharedVec3Uni MainCamera::getCameraPositionUni()
{
  return positionUni;
}

} // namespace glt Eevee may not be calculating light clips