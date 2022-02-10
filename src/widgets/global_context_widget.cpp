#include "global_context_widget.h"
#include "widget.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "logger.h"

#include <GL/glew.h>

namespace glt {

const char* render_type_names[] = {
  "Normal",
  "Debug",
  "Wireframe",
  "Unknown"
};

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
  // Render type
  int render_type = static_cast<int>(this->model->render_type);
  const char *render_name = (render_type >= 0 && render_type < 3) ? render_type_names[render_type] : render_type_names[3];
  if ( ImGui::SliderInt("Render type", reinterpret_cast<int*>(&this->model->render_type), 0, 2, render_name) )
  {
    // this control is currently pressed or active
    switch (this->model->render_type) {
    case RenderType::Normal:
      GLERR ( glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) );
      break;
    case RenderType::Debug:
      // TODO gizmos and stuff
      break;
    case RenderType::Wireframe:
      GLERR( glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) );
      break;
    default:
      GLERR( glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) );
      break;
    }
  }
  ImGui::SliderFloat("Gamma", &this->model->gamma->model, 1.0f, 3.0f, "%.5f", 1.0f);
  ImGui::SliderFloat("Exposure", &this->model->exposure->model, 0.0001f, 20.0f, "%.5f", 1.0f);
  ImGui::End();

  // Movement controls
  ImGui::Begin("Controls");
  ImGui::SliderFloat("Sensitivity", &this->model->sensitivity, 0.015625f, 0.5f, "%.5f", 1.0f);
  ImGui::SliderFloat("Movement Speed", &this->model->move_speed, 0.25f, 8.0f, "%.3f", 1.0f);
  ImGui::End();
}

} // namespace glt