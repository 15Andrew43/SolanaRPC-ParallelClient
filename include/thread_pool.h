#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <mutex>

class ThreadPool {
public:
    // Конструктор, запускает num_threads потоков
    ThreadPool(size_t num_threads);

    // Деструктор, завершает работу всех потоков
    ~ThreadPool();

    // Добавление задачи в пул
    void enqueue_task(std::function<void()> task);

private:
    // Функция, выполняемая каждым потоком
    void worker_thread();

    std::vector<std::thread> workers;               // Вектор потоков
    std::queue<std::function<void()>> tasks;        // Очередь задач
    std::mutex tasks_mutex;                         // Мьютекс для защиты очереди
    std::condition_variable condition;              // Условная переменная для блокировки/разблокировки потоков
    std::atomic<bool> stop;                         // Флаг для завершения работы
};
