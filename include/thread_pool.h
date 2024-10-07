#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <optional>
#include "unbounded_blocking_queue.h"


class ThreadPool {
public:
    explicit ThreadPool(size_t threads);
    ~ThreadPool();

    
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void Start();
    void Submit(std::function<void()> task);
    void Stop();

private:
    void Worker();

private:
    UnboundedBlockingQueue<std::function<void()>> tasks_;  
    size_t threads_;
    std::vector<std::thread> workers_;  
    bool stop_ = false;  
};
