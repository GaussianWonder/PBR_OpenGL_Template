#include "global_settings.h"

namespace glt {

GlobalSettings* GlobalSettings::settings = nullptr;

GlobalSettings::GlobalSettings()
{}

GlobalSettings* GlobalSettings::instance()
{
  if(settings == nullptr)
    settings = new GlobalSettings();
  return settings;
}

GlobalSettings* GlobalSettings::unwrap()
{
  return settings;
}

} // namespace glt