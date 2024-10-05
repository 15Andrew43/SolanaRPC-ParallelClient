#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include "IEventQueue.h"
#include <iostream>

class ThreadPool {
public:
    ThreadPool(size_t num_threads, IEventQueue& queue, size_t batch_size)
        : event_queue(queue), batch_size(batch_size), stop_flag(false) {
        // Запускаем num_threads потоков
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back(&ThreadPool::worker_thread, this);
        }
    }

    ~ThreadPool() {
        // Устанавливаем флаг остановки
        stop_flag = true;
        // Ожидаем завершения всех потоков
        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

private:
    // Рабочий поток: берет по batch_size событий и обрабатывает их
    void worker_thread() {
        while (!stop_flag) {
            std::vector<std::string> events_batch;
            
            // Забираем batch_size событий из очереди
            for (size_t i = 0; i < batch_size; ++i) {
                events_batch.push_back(event_queue.pop());
            }

            // Обрабатываем события
            handle_events(events_batch);
        }
    }

    // Обработка одного события (делает 3 запроса и дожидается первого успешного)
    void handle_events(const std::vector<std::string>& events_batch) {
        if (events_batch.empty()) return;
        
        // Берем первое событие из батча для обработки
        const std::string& event = events_batch.front();

        // Здесь можно разместить логику отправки 3 запросов с помощью libcurl
        // Для простоты пока выведем событие в консоль
        std::cout << "Обрабатывается событие: " << event << std::endl;
        
        // Пример с использованием 3 запросов к разным узлам с использованием libcurl
        std::vector<std::string> urls = {
            "https://example1.com",
            "https://example2.com",
            "https://example3.com"
        };

        // Выполняем 3 запроса и ждем первого успешного (логика будет добавлена дальше)
        std::string response = send_requests(urls);

        // Выводим результат
        if (!response.empty()) {
            std::cout << "Первый успешный ответ для события " << event << ": " << response << std::endl;
        }
    }

    // Пример функции для отправки 3 запросов и ожидания первого успешного ответа
    std::string send_requests(const std::vector<std::string>& urls);

    // Параметры
    std::vector<std::thread> workers;       // Потоки
    IEventQueue& event_queue;               // Очередь событий
    size_t batch_size;                      // Количество событий для обработки за один раз
    std::atomic<bool> stop_flag;            // Флаг для остановки потоков
};
