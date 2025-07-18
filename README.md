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
```
.
├── src/
│   ├── LogLevel.h
│   ├── LogEntry.h
│   ├── LockFreeRingBuffer.h
│   ├── LockFreeLogger.h
│   └── main.cpp
├── logs/                
└── README.md
```


📊 Performance
1. The logger records the time taken to enqueue log entries and reports average latency at the end of execution.
2. Ideal for applications where logging should not block real-time processing or introduce jitter.

🧱 How It Works
1. Producer threads push LogEntry objects into a lock-free ring buffer.
2. A background thread dequeues and writes logs to disk.
3. Supports log rotation:
4. Daily (new file each day)
5. Max file size (e.g., 10MB)
