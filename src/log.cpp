// log.cpp
#include "log.h"

#include <chrono>
#include <cstdarg>
#include <filesystem>
#include <iostream>
#include <print>
#include <string>

void VGMLogger::log(LogLevel level, const char* fmt, ...) {
    const char* level_str = get_level_string(level);
    std::string timestamp = get_timestamp();

    bool use_stderr = (level == LOG_LEVEL_ERROR || level == LOG_LEVEL_WARN);
    FILE* out = use_stderr ? stderr : stdout;

    // Print prefix
    std::print(out, "[{}] [{}] ", timestamp, level_str);

    // Format actual message
    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);

    // Newline after message
    std::fputc('\n', out);
}

const char* VGMLogger::get_level_string(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_WARN:
            return "WARN";
        case LOG_LEVEL_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

std::string VGMLogger::get_timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::string str = std::ctime(&time);  // has newline
    if (!str.empty() && str.back() == '\n')
        str.pop_back();
    return str;
}
