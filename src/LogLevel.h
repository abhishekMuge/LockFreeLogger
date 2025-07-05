// LogLevel.h
#pragma once
#include <string>

enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

inline std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default:              return "UNKNOWN";
    }
}
