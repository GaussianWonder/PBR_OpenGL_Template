#ifndef _LOGGER_H_
#define _LOGGER_H_

#define IS_LOGGING true // i don't know yet if this is a good idea, but at least i can turn them off
//TODO It can be a good idea to split debugging and info logging from the other log types, and have only those be completely turned off
#define DEFAULT_LOGGER "global"

#include "spdlog/spdlog.h"

#define TRACE(...) TRACE_NAME(DEFAULT_LOGGER, __VA_ARGS__)
#define DEBUG(...) DEBUG_NAME(DEFAULT_LOGGER, __VA_ARGS__)
#define INFO(...) INFO_NAME(DEFAULT_LOGGER, __VA_ARGS__)
#define WARN(...) WARN_NAME(DEFAULT_LOGGER, __VA_ARGS__)
#define ERROR(...) ERROR_NAME(DEFAULT_LOGGER, __VA_ARGS__)
#define FATAL(...) FATAL_NAME(DEFAULT_LOGGER, __VA_ARGS__)

#ifdef IS_LOGGING
  #define TRACE_NAME(logger_name, ...)  if (spdlog::get(logger_name)!=nullptr) {spdlog::get(logger_name)->trace(__VA_ARGS__);}
  #define DEBUG_NAME(logger_name, ...)  if (spdlog::get(logger_name)!=nullptr) {spdlog::get(logger_name)->debug(__VA_ARGS__);}
  #define INFO_NAME(logger_name, ...)   if (spdlog::get(logger_name)!=nullptr) {spdlog::get(logger_name)->info(__VA_ARGS__);}
  #define WARN_NAME(logger_name, ...)   if (spdlog::get(logger_name)!=nullptr) {spdlog::get(logger_name)->warn(__VA_ARGS__);}
  #define ERROR_NAME(logger_name, ...)  if (spdlog::get(logger_name)!=nullptr) {spdlog::get(logger_name)->error(__VA_ARGS__);}
  #define FATAL_NAME(logger_name, ...)  if (spdlog::get(logger_name)!=nullptr) {spdlog::get(logger_name)->critical(__VA_ARGS__);}
#else
  #define TRACE_NAME(logger_name, ...) ;
  #define DEBUG_NAME(logger_name, ...) ;
  #define INFO_NAME(logger_name, ...) ;
  #define WARN_NAME(logger_name, ...) ;
  #define ERROR_NAME(logger_name, ...) ;
  #define FATAL_NAME(logger_name, ...) ;
#endif

namespace glt {
namespace Logger {

void init();
void destroy();

} // namespace glt::Logger
} // namespace glt

#endif // _LOGGER_H_