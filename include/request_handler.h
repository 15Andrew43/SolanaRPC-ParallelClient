#pragma once
#include "node_manager.h"
#include <string>
#include <vector>
#include "event_types.h"

class RequestHandler {
public:
    // Конструктор, который принимает ссылку на NodeManager
    RequestHandler(NodeManager& node_manager);

    // Метод для обработки нескольких событий
    std::string invoke_request(const std::vector<EventType>& events);


private:
    NodeManager& node_manager;  // Хранение ссылки на NodeManager
};
