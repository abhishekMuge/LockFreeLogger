#pragma once
#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "LogLevel.h"

struct LogEntry {
    static constexpr size_t MAX_MSG_SIZE = 256;
    char message[MAX_MSG_SIZE];
    std::chrono::system_clock::time_point timestamp;
    LogLevel level;

    LogEntry() : level(LogLevel::INFO) {
        message[0] = '\0';
    }

    void set(const std::string& msg, LogLevel lvl) {
        timestamp = std::chrono::system_clock::now();
        level = lvl;
        std::strncpy(message, msg.c_str(), MAX_MSG_SIZE - 1);
        message[MAX_MSG_SIZE - 1] = '\0';
    }

    std::string formatted() const {
        using namespace std::chrono;

        auto time_t_time = system_clock::to_time_t(timestamp);
        auto ms = duration_cast<milliseconds>(timestamp.time_since_epoch()) % 1000;

        std::ostringstream oss;
        std::tm tm_time;
    #if defined(_WIN32)
        localtime_s(&tm_time, &time_t_time);
    #else
        localtime_r(&time_t_time, &tm_time);
    #endif

        oss << "[" << logLevelToString(level) << "] "
            << std::put_time(&tm_time, "%Y-%m-%d %H:%M:%S") << "."
            << std::setfill('0') << std::setw(3) << ms.count()
            << " | " << message;

        return oss.str();
    }
};
