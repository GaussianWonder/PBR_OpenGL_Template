#include "main_camera.h"

using SharedMat4Uni = std::shared_ptr<glt::Uniform<glm::mat4>>;

namespace glt {


void MainCamera::onViewChanged() {
  (*viewUni) = getViewMatrix();
};

void MainCamera::onProjectionChanged() {
  (*projectionUni) = getProjectionMatrix();
}

MainCamera::MainCamera(SharedMat4Uni viewUni, SharedMat4Uni projectionUni)
  :Camera()
  ,viewUni(viewUni)
  ,projectionUni(projectionUni)
{}

SharedMat4Uni MainCamera::getViewUni() {
  return viewUni;
}

SharedMat4Uni MainCamera::getProjectionUni() {
  return projectionUni;
}

} // namespace glt Eevee may not be calculating light clips