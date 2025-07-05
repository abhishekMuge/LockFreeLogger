# 🚀 Lock-Free Logger in C++

A high-performance, lock-free logging utility written in modern C++. This project is designed for ultra-low-latency logging in multithreaded environments such as trading systems, real-time analytics, or high-frequency data pipelines.

---

## ✨ Features

- ✅ **Lock-Free Logging** (SPSC ring buffer)
- ✅ **Thread-safe background writer**
- ✅ **Log Levels**: INFO, WARN, ERROR
- ✅ **Timestamped Messages**
- ✅ **Pre-allocated message buffer** (no dynamic allocation during log)
- ✅ **Log Rotation** (by date and file size)
- ✅ **Performance Benchmarking** (average latency per log call)
- 🛠️ Easy to extend to multi-producer with Boost or custom lock-free queue

---

## 📦 Folder Structure
├──  src/
│ ├── LogLevel.h
│ ├── LogEntry.h
│ ├── LockFreeRingBuffer.h
│ └── LockFreeLogger.h
│ └── main.cpp
├── logs/ # Generated logs saved here
└── README.md


📊 Performance
1. The logger records the time taken to enqueue log entries and reports average latency at the end of execution.
2. Ideal for applications where logging should not block real-time processing or introduce jitter.
