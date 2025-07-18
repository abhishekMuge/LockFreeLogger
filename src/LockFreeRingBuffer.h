#pragma once
#include <atomic>
#include <optional>
#include "LogEntry.h"

template <size_t SIZE>
class LockFreeRingBuffer {
public:
    LockFreeRingBuffer() : head_(0), tail_(0) {}

    bool enqueue(const LogEntry& entry) {
    	size_t current_head;

    	while(true){
    		current_head = head_.load(std::memory_order_relaxed);
    		size_t tail = tail_.load(std::memory_order_acquire);

    		size_t next_head = (current_head + 1) & (SIZE - 1); // replaced % SIZE

    		if (next_head == tail) {
				return false; // Buffer is full
			}

    		if (head_.compare_exchange_weak(current_head, next_head,
											std::memory_order_acquire,
											std::memory_order_relaxed)) {
				buffer_[current_head] = entry;
				return true;
			}
    	}
    	//Below changes is done to handle the Multi Producer and Single Consumer thread
    	//in cases of multple producer is trying to add data in same index, will be loss of data
        /*size_t current_head = head_.load(std::memory_order_relaxed);
        size_t next_head = (current_head + 1) % SIZE;

        if (next_head == tail_.load(std::memory_order_acquire)) {
            return false; // full
        }

        buffer_[current_head] = entry;
        head_.store(next_head, std::memory_order_release);
        return true;*/
    }

    std::optional<LogEntry> dequeue() {
        size_t current_tail = tail_.load(std::memory_order_relaxed);
        if (current_tail == head_.load(std::memory_order_acquire)) {
            return std::nullopt; // empty
        }

        LogEntry entry = buffer_[current_tail];
        tail_.store((current_tail + 1) % SIZE, std::memory_order_release);
        return entry;
    }

private:
    LogEntry buffer_[SIZE];
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;
};
