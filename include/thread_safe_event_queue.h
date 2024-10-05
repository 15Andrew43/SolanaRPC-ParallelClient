#pragma once
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

class ThreadSafeEventQueue {
public:
    // Добавление нового события в очередь
    void push(const std::string& event);

    // Извлечение события из очереди
    std::string pop();

    // Проверка, пуста ли очередь
    bool empty() const;

private:
    std::queue<std::string> events;             // Собственно очередь событий
    mutable std::mutex mtx;                     // Мьютекс для защиты очереди
    std::condition_variable cond_var;           // Условная переменная для ожидания новых событий
};
