#include "thread_safe_event_queue.h"
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

// Добавление задачи
void ThreadSafeEventQueue::push(const std::string& event) {
    std::lock_guard<std::mutex> lock(mtx);
    events.push(event);
    cond_var.notify_one();
}

// Извлечение задачи
std::string ThreadSafeEventQueue::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cond_var.wait(lock, [this] { return !events.empty(); });
    std::string event = events.front();
    events.pop();
    return event;
}

// Проверка на пустоту очереди
bool ThreadSafeEventQueue::empty() const {
    std::lock_guard<std::mutex> lock(mtx);
    return events.empty();
}
