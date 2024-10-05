#pragma once
#include "node_manager.h"
#include <string>
#include <curl/curl.h>
#include <vector>

class RequestHandler {
public:
    // Конструктор
    RequestHandler(NodeManager& node_manager);

    // Выполнение GET-запроса к нодам и ожидание первого успешного
    std::string invoke_request();

private:
    NodeManager& node_manager;

    // Функция для записи данных из HTTP-ответа
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};
