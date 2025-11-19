// log.h
#pragma once

#include <string>

enum LogLevel {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
};

// Simple logging class for VGMCreator
class VGMLogger {
   public:
    VGMLogger() { m_log_file = nullptr; }
    ~VGMLogger() { delete[] m_log_file; }

    // Logs a message with time stamp and level to stdout or error stream depending on level
    void log(LogLevel level, const char* fmt, ...);

   private:
    char* m_log_file;
    const char* get_level_string(LogLevel level);  // Returns string representation of log level
    std::string get_timestamp();                   // Returns current timestamp as string
};