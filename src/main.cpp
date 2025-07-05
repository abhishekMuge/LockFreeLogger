#include "LockFreeLogger.h"
#include <thread>

int main() {
    LockFreeLogger logger("lockfree_log");

    std::thread t1([&]() {
        for (int i = 0; i < 500; ++i)
            logger.log("Thread 1 - message " + std::to_string(i), LogLevel::INFO);
    });

    std::thread t2([&]() {
        for (int i = 0; i < 500; ++i)
            logger.log("Thread 2 - warning " + std::to_string(i), LogLevel::WARN);
    });

    std::thread t3([&]() {
        for (int i = 0; i < 500; ++i)
            logger.log("Thread 3 - error " + std::to_string(i), LogLevel::ERROR);
    });

    t1.join();
    t2.join();
    t3.join();

    logger.printLogPerformanceStats();
    return 0;
}
