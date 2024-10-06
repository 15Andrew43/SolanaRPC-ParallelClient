#include "request_handler.h"
#include "responses_container.h"

class EventHandler {
public:
    EventHandler(RequestHandler& request_handler);

    // Обработка событий
    void handle_events(const std::vector<EventType>& events);

    // Получение самого старого результата
    ResponseResult get_oldest_result();

private:
    RequestHandler& request_handler;
    ResponsesContainer responses_container;  // Контейнер для хранения результатов запросов
};
