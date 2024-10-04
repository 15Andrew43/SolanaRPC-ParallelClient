#ifndef MULTI_NODE_REQUEST_HANDLER_H
#define MULTI_NODE_REQUEST_HANDLER_H

#include <vector>
#include <string>
#include <future>
#include <curl/curl.h>

class MultiNodeRequestHandler {
public:
    MultiNodeRequestHandler();
    
    // Добавление узлов (URL-адресов)
    void add_node(const std::string& url);
    
    // Удаление узлов
    void remove_node(const std::string& url);

    // Отправка параллельных запросов и возврат первого ответа
    std::string send_request(const std::string& request_body);

private:
    std::vector<std::string> nodes;

    // Вспомогательная функция для отправки POST-запроса к конкретному узлу
    std::string send_post_request(const std::string& url, const std::string& request_body);
};

#endif // MULTI_NODE_REQUEST_HANDLER_H
