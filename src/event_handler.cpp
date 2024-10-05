#include "event_handler.h"
#include <iostream>

// Конструктор
EventHandler::EventHandler(RequestHandler& request_handler) : request_handler(request_handler) {}

// Обработка нескольких событий
void EventHandler::handle_events(const std::vector<EventType>& events) {
    for (const auto& event : events) {
        switch (event) {
            case EventType::INVOKE: {
                std::cout << "Получено событие INVOKE, запускаем запросы..." << std::endl;
                
                // Вызываем запросы к нодам через request_handler
                std::string result = request_handler.invoke_request();
                if (!result.empty()) {
                    std::cout << "Успешный ответ: " << result << std::endl;
                } else {
                    std::cerr << "Не удалось получить успешный ответ." << std::endl;
                }
                break;
            }

            case EventType::NOTHING:
                std::cout << "Событие NOTHING, действия не требуются." << std::endl;
                break;

            case EventType::ERROR:
                std::cerr << "Событие ERROR, произошла ошибка." << std::endl;
                break;
        }
    }
}
