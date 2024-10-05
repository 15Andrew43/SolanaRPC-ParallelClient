#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <optional>
#include "unbounded_blocking_queue.h"

// Фиксированный пул потоков
class ThreadPool {
public:
    explicit ThreadPool(size_t threads);
    ~ThreadPool();

    // Нельзя копировать или перемещать пул потоков
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void Start();
    void Submit(std::function<void()> task);
    void Stop();

private:
    void Worker();

private:
    UnboundedBlockingQueue<std::function<void()>> tasks_;  // Очередь задач
    size_t threads_;
    std::vector<std::thread> workers_;  // Потоки-воркеры
    bool stop_ = false;  // Флаг остановки работы
};
