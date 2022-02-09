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
  // Renderer info
  ImGui::Text(this->model->renderer.c_str());
  ImGui::Text(this->model->version.c_str());
  // FPS
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  // Movement controls
  ImGui::End();

  ImGui::Begin("Controls");
  ImGui::SliderFloat("Sensitivity", &this->model->sensitivity, 0.015625f, 0.5f, "%.5f", 1.0f);
  ImGui::SliderFloat("Movement Speed", &this->model->move_speed, 0.25f, 8.0f, "%.3f", 1.0f);
  ImGui::End();
}

} // namespace glt