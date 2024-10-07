#include "thread_pool.h"
#include <cassert>


ThreadPool::ThreadPool(size_t threads)
    : threads_(threads) {}


void ThreadPool::Start() {
    for (size_t i = 0; i < threads_; ++i) {
        workers_.emplace_back([this] { Worker(); });
    }
}


ThreadPool::~ThreadPool() {
    Stop();  
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}


void ThreadPool::Submit(std::function<void()> task) {
    tasks_.Push(std::move(task));  
}


void ThreadPool::Stop() {
    tasks_.Close();  
    stop_ = true;
}


void ThreadPool::Worker() {
    while (!stop_) {
        auto task = tasks_.Pop();  
        if (task) {
            (*task)();  
        }
    }
}
