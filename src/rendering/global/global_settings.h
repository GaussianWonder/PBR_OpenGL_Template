#ifndef _GLOBAL_SETTINGS_H_
#define _GLOBAL_SETTINGS_H_

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "ImCurveEdit.h"

namespace glt {

enum RenderType {
  Normal,
  Debug,
  Wireframe,
  Depth,
};

class GlobalSettings {
protected:
  GlobalSettings();
  static GlobalSettings* settings;

public:
  GlobalSettings(GlobalSettings &other) = delete;
  void operator=(const GlobalSettings &) = delete;

  // nullptr check + constructor
  static GlobalSettings * instance();
  // unsafe, no nullptr check. usable when an initial render settings are set before using this
  static GlobalSettings * unwrap(); // identical to GlobalSettings::settings if it were to be public

  bool isPerspective = true;
  RenderType render_type = RenderType::Normal;
  ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;

  // RealTime edit globals
  bool useSnap = false;
  float snap[3] = { 1.f, 1.f, 1.f };
  float bounds[6] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
  float boundsSnap[3] = { 0.1f, 0.1f, 0.1f };
  bool boundSizing = false;
  bool boundSizingSnap = false;
};

} // namespace glt

#endif // _GLOBAL_SETTINGS_H_