#include "event_handler.h"
#include <iostream>
#include <unordered_map>

// Конструктор
EventHandler::EventHandler(RequestHandler& request_handler) : request_handler(request_handler) {}

// Обработка нескольких событий
void EventHandler::handle_events(const std::vector<EventType>& events) {
    // Фильтруем только события типа INVOKE
    std::vector<EventType> invoke_events;
    for (const auto& event : events) {
        if (event == EventType::INVOKE) {
            invoke_events.push_back(event);
        }
    }

    // Если есть события INVOKE, обрабатываем их
    if (!invoke_events.empty()) {
        std::cout << "Получено " << invoke_events.size() << " событий INVOKE, запускаем запросы..." << std::endl;
        
        // Передаем массив событий в RequestHandler для обработки запросов
        request_handler.invoke_request(invoke_events);
    }

    // Обрабатываем другие типы событий
    for (const auto& event : events) {
        if (event == EventType::NOTHING) {
            std::cout << "Событие NOTHING, действия не требуются." << std::endl;
        } else if (event == EventType::ERROR) {
            std::cerr << "Событие ERROR, произошла ошибка." << std::endl;
        }
    }
}
