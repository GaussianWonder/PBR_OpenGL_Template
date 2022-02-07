#include "global_context_widget.h"
#include "widget.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace glt {

GlobalContextWidget::GlobalContextWidget()
  :Widget<GlobalSettings*>(GlobalSettings::instance())
{}

void GlobalContextWidget::draw()
{
  ImGui::Begin("Stats");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::SliderFloat("Sensitivity", &this->model->sensitivity, 0.1f, 7.0f, "%.3f", 1.0f);
  ImGui::End();
}

} // namespace glt