#include "log.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef LOGGING
#define log_va(type)             \
    if (is_silent(type)) return; \
    va_list args;                \
    va_start(args, format);      \
    log_msg(type, format, args); \
    va_end(args);

static char LOG_LEVEL_NAME[6] = {'S', 'E', 'W', 'I', 'D', 'V'};

static debby::log::LogLevel log_level = debby::log::LOG_LEVEL_WARNING;

static bool is_silent(debby::log::LogLevel level) {
    return log_level == debby::log::LOG_LEVEL_SILENT || level > log_level;
}

static void log_msg(debby::log::LogLevel level, const char *format,
                    va_list args, ...) {
    printf("[%c] ", LOG_LEVEL_NAME[level]);
    vprintf(format, args);
}

debby::log::LogLevel debby::log::get_level() noexcept { return log_level; }

void debby::log::set_level(LogLevel level) noexcept { log_level = level; }

void debby::log::verbose(const char *format, ...) noexcept {
    log_va(LOG_LEVEL_VERBOSE)
}

void debby::log::debug(const char *format, ...) noexcept {
    log_va(LOG_LEVEL_DEBUG)
}

void debby::log::info(const char *format, ...) noexcept {
    log_va(LOG_LEVEL_INFO)
}

void debby::log::warning(const char *format, ...) noexcept {
    log_va(LOG_LEVEL_WARNING)
}

void debby::log::error(const char *format,
                       ...) noexcept {log_va(LOG_LEVEL_ERROR)}
#else
debby::log::LogLevel debby::log::get_level() noexcept {
    return LOG_LEVEL_SILENT;
}

void debby::log::set_level(LogLevel level) noexcept {}

void debby::log::verbose(const char *format, ...) noexcept {}

void debby::log::debug(const char *format, ...) noexcept {}

void debby::log::info(const char *format, ...) noexcept {}

void debby::log::warning(const char *format, ...) noexcept {}

void debby::log::error(const char *format, ...) noexcept {}
#endif

