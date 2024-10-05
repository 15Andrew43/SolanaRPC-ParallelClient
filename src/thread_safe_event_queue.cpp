#include "IEventQueue.h"
#include <queue>
#include <mutex>
#include <condition_variable>

// Реализация потокобезопасной очереди событий
class ThreadSafeEventQueue : public IEventQueue {
public:
    // Добавление события в очередь
    void push(const std::string& event) override {
        std::lock_guard<std::mutex> lock(mtx);
        events.push(event);
        cond_var.notify_one();  // Оповещаем, что появилось новое событие
    }

    // Извлечение события из очереди
    std::string pop() override {
        std::unique_lock<std::mutex> lock(mtx);
        // Ждём, пока очередь не станет непустой
        cond_var.wait(lock, [this] { return !events.empty(); });
        std::string event = events.front();
        events.pop();
        return event;
    }

    // Проверка, пуста ли очередь
    bool empty() const override {
        std::lock_guard<std::mutex> lock(mtx);
        return events.empty();
    }

private:
    mutable std::mutex mtx;  // Мьютекс для защиты доступа к очереди
    std::condition_variable cond_var;  // Условная переменная для блокировки/разблокировки потоков
    std::queue<std::string> events;  // Собственно очередь событий
};
