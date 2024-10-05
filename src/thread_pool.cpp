#include "thread_pool.h"

// Конструктор, запускающий потоки
ThreadPool::ThreadPool(size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this] { this->worker_thread(); });
    }
}

// Деструктор, завершающий работу потоков
ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(tasks_mutex);
        stop = true;
    }
    condition.notify_all();  // Оповещаем все потоки

    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();  // Ждем завершения всех потоков
        }
    }
}

// Добавление задачи в пул
void ThreadPool::enqueue_task(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(tasks_mutex);
        tasks.emplace(task);
    }
    condition.notify_one();  // Оповещаем один поток о новой задаче
}

// Рабочий поток
void ThreadPool::worker_thread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(tasks_mutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }
        task();  // Выполняем задачу
    }
}
