#include "logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <vector>
#include <memory>

namespace glt {
namespace Logger {

void init()
{
  const char *globalSinkPattern = "%^(%P %t) from %n with [%l] at [%Y-%m-%d %H:%M%S.%e]\n%v\n%$";

  auto consoleSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
  consoleSink->set_pattern(globalSinkPattern);

  auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>("global.log");
  fileSink->set_pattern(globalSinkPattern);

  std::vector<spdlog::sink_ptr> sinks{ consoleSink, fileSink };
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