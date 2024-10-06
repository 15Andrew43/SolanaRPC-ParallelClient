#pragma once
#include "unbounded_blocking_queue.h"
#include <string>
#include <chrono>

// Структура для хранения результата запроса
struct RequestResult {
    std::string body;
    std::chrono::steady_clock::time_point timestamp;
    std::chrono::milliseconds latency;
};

// Класс для хранения результатов запросов
class RequestsContainer {
public:
    // Добавление результата в контейнер
    void add_result(const std::string& body, 
                    const std::chrono::steady_clock::time_point& timestamp, 
                    const std::chrono::milliseconds& latency);

    // Получение самого старого результата
    std::optional<RequestResult> get_oldest_result();

    // Закрытие контейнера
    void close();

private:
    UnboundedBlockingQueue<RequestResult> request_results;  // Очередь для хранения результатов запросов
};
