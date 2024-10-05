#include "event_handler.h"
#include "node_manager.h"
#include "request_handler.h"
#include "thread_pool.h"
#include "unbounded_blocking_queue.h"
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    int events_to_process = 5;  // По умолчанию 5 событий
    if (argc > 1) {
        events_to_process = std::stoi(argv[1]);  // Получаем значение из аргумента командной строки
    }

    NodeManager node_manager;
    node_manager.add_ip("72.46.84.7");
    node_manager.add_ip("67.209.52.66");

    RequestHandler request_handler(node_manager);
    EventHandler event_handler(request_handler);

    ThreadPool pool(4);
    pool.Start();

    UnboundedBlockingQueue<EventType> event_queue;

    // Добавляем несколько событий в очередь
    for (int i = 0; i < 10; ++i) {
        event_queue.Push(EventType::INVOKE);
    }

    pool.Submit([&event_queue, &event_handler, events_to_process] {
        std::vector<EventType> events;

        // Извлекаем заданное количество событий из очереди
        for (int i = 0; i < events_to_process; ++i) {
            auto event_opt = event_queue.Pop();
            if (event_opt) {
                events.push_back(*event_opt);
            }
        }

        // Обрабатываем события
        event_handler.handle_events(events);
    });

    std::this_thread::sleep_for(std::chrono::seconds(5));
    pool.Stop();

    return 0;
}
