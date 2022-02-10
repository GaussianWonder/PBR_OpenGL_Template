#ifndef _MAIN_CAMERA_H_
#define _MAIN_CAMERA_H_

#include "camera.h"
#include "uniform.h"

#include <memory>

namespace glt {

/**
 * @brief Just a wrapper over glt::Camera
 * Includes management of viewUni and projectionUni such that no more explicit assignment operations are needed
 * Each object must .update it itself before drawing.
 */
class MainCamera : public Camera {
  using SharedMat4Uni = std::shared_ptr<glt::Uniform<glm::mat4>>;
  using SharedVec3Uni = std::shared_ptr<glt::Uniform<glm::vec3>>;
  SharedMat4Uni viewUni;
  SharedMat4Uni projectionUni;

  void onViewChanged() override;
  void onProjectionChanged() override;

  SharedVec3Uni positionUni;
public:
  MainCamera(SharedMat4Uni viewUni, SharedMat4Uni projectionUni);

  SharedMat4Uni getViewUni();
  SharedMat4Uni getProjectionUni();
  SharedVec3Uni getCameraPositionUni();
};

} // namespace glt

#endif // _MAIN_CAMERA_H_