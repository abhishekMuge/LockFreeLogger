#pragma once
#include "LockFreeRingBuffer.h"
#include "LogLevel.h"
#include "LogEntry.h"

#include <thread>
#include <fstream>
#include <atomic>
#include <chrono>
#include <iostream>
#include <filesystem>

class LockFreeLogger {
public:
    LockFreeLogger(const std::string& base_filename, size_t max_file_size = 10 * 1024 * 1024)
        : base_filename_(base_filename),
          max_file_size_(max_file_size),
          stop_flag_(false),
          log_thread_(&LockFreeLogger::processLogs, this)
    {
        rotateLogFile();
    }

    ~LockFreeLogger() {
        stop_flag_.store(true);
        if (log_thread_.joinable()) log_thread_.join();
        if (log_file_.is_open()) log_file_.close();
    }

    void log(const std::string& message, LogLevel level = LogLevel::INFO) {
        auto start = std::chrono::high_resolution_clock::now();

        LogEntry entry;
        entry.set(message, level);
        while (!ring_buffer_.enqueue(entry)) {
            std::this_thread::yield(); // backoff
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto latency_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        latency_sum_ += latency_ns;
        log_count_++;
    }

    void printLogPerformanceStats() const {
        size_t count = log_count_.load();
        if (count == 0) {
            std::cout << "No logs written yet.\n";
            return;
        }

        long long total_ns = latency_sum_.load();
        double avg_us = static_cast<double>(total_ns) / count / 1000.0;

        std::cout << "Log Count: " << count << "\n";
        std::cout << "Total Log Latency (ns): " << total_ns << "\n";
        std::cout << "Average Log Latency (µs): " << avg_us << "\n";
    }

private:
    void rotateLogFile() {
        if (log_file_.is_open()) log_file_.close();

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm_time;
    #if defined(_WIN32)
        localtime_s(&tm_time, &t);
    #else
        localtime_r(&t, &tm_time);
    #endif

        char buffer[128];
        std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &tm_time);
        std::string new_filename = base_filename_ + "_" + buffer + ".log";

        log_file_.open(new_filename, std::ios::out | std::ios::app);
        current_file_size_ = 0;
        last_rotation_time_ = tm_time;
    }

    bool shouldRotate() {
        if (current_file_size_ > max_file_size_)
            return true;

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm;
    #if defined(_WIN32)
        localtime_s(&now_tm, &t);
    #else
        localtime_r(&t, &now_tm);
    #endif

        return now_tm.tm_mday != last_rotation_time_.tm_mday;
    }

    void processLogs() {
        while (!stop_flag_.load()) {
            auto entry_opt = ring_buffer_.dequeue();
            if (entry_opt) {
                if (shouldRotate()) rotateLogFile();

                std::string formatted = entry_opt->formatted();
                log_file_ << formatted << std::endl;
                current_file_size_ += formatted.size() + 1;
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(50));
            }
        }

        // Final flush
        while (auto entry_opt = ring_buffer_.dequeue()) {
            log_file_ << entry_opt->formatted() << std::endl;
        }
    }

    LockFreeRingBuffer<2048> ring_buffer_;
    std::ofstream log_file_;
    std::string base_filename_;
    size_t current_file_size_;
    size_t max_file_size_;
    std::tm last_rotation_time_;
    std::atomic<bool> stop_flag_;
    std::thread log_thread_;

    std::atomic<size_t> log_count_ = 0;
    std::atomic<long long> latency_sum_ = 0;
};
