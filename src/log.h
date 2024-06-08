#ifndef DEBBY_LOG_H_
#define DEBBY_LOG_H_

namespace debby::log {
enum LogLevel {
    LOG_LEVEL_SILENT,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE
};

LogLevel get_level() noexcept;
void set_level(LogLevel level) noexcept;
void error(const char *format, ...) noexcept;
void warning(const char *format, ...) noexcept;
void info(const char *format, ...) noexcept;
void debug(const char *format, ...) noexcept;
void verbose(const char *format, ...) noexcept;
}  // namespace debby::log

#endif  // DEBBY_LOG_H_
