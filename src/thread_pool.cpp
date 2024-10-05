#include "thread_pool.h"
#include <cassert>

// Конструктор
ThreadPool::ThreadPool(size_t threads)
    : threads_(threads) {}

// Запуск потоков
void ThreadPool::Start() {
    for (size_t i = 0; i < threads_; ++i) {
        workers_.emplace_back([this] { Worker(); });
    }
}

// Деструктор
ThreadPool::~ThreadPool() {
    Stop();  // Остановка пула при завершении программы
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

// Добавление задачи в пул
void ThreadPool::Submit(std::function<void()> task) {
    tasks_.Push(std::move(task));  // Добавляем задачу в очередь
}

// Остановка пула
void ThreadPool::Stop() {
    tasks_.Close();  // Закрываем очередь задач
    stop_ = true;
}

// Воркер, который выполняет задачи из очереди
void ThreadPool::Worker() {
    while (!stop_) {
        auto task = tasks_.Pop();  // Попытка извлечь задачу
        if (task) {
            (*task)();  // Выполняем задачу
        }
    }
}
