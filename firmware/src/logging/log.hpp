// printf-style log to stdout through SEGGER RTT syscalls, after cybsp_init().
#pragma once

#include <cstdarg>
#include <cstdio>
#include <string_view>

namespace app::log {

#if defined(__GNUC__) || defined(__clang__)
#define APP_LOG_FMT_ATTR(fmt_index, first_arg) __attribute__((format(printf, fmt_index, first_arg)))
#else
#define APP_LOG_FMT_ATTR(fmt_index, first_arg)
#endif

APP_LOG_FMT_ATTR(1, 2)
inline int info(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::fputs("[I] ", stdout);
    const int n = std::vfprintf(stdout, fmt, ap);
    va_end(ap);
    return n;
}

APP_LOG_FMT_ATTR(1, 2)
inline int warn(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::fputs("[W] ", stdout);
    const int n = std::vfprintf(stdout, fmt, ap);
    va_end(ap);
    return n;
}

APP_LOG_FMT_ATTR(1, 2)
inline int error(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::fputs("[E] ", stdout);
    const int n = std::vfprintf(stdout, fmt, ap);
    va_end(ap);
    return n;
}

inline void line(std::string_view sv) {
    std::fputs("[I] ", stdout);
    std::fwrite(sv.data(), 1U, sv.size(), stdout);
    std::fputc('\n', stdout);
}

#undef APP_LOG_FMT_ATTR

} // namespace app::log
