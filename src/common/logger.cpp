#include "logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

#include <vector>
#include <memory>

namespace glt {
namespace Logger {

void init()
{
  auto consoleSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
  consoleSink->set_pattern("%^(%P %t) from %n with [%l] at [%Y-%m-%d %H:%M%S.%e]\n%v\n%$");

  std::vector<spdlog::sink_ptr> sinks{ consoleSink };
  auto logger = std::make_shared<spdlog::logger>(DEFAULT_LOGGER, sinks.begin(), sinks.end());

  logger->set_level(spdlog::level::trace);
  logger->flush_on(spdlog::level::trace);
  spdlog::register_logger(logger);
}

void destroy()
{
  spdlog::shutdown();
}

} // namespace glt::Logger
} // namespace glt