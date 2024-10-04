#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include "event_handler.h"
#include <string>

class HttpRequestHandler : public IEventHandler {
public:
    HttpRequestHandler(const std::string& url);
    
    // Реализация метода интерфейса
    void handle_event(EventType event) override;

    // Метод для выполнения POST-запроса
    std::string send_post_request(const std::string& request_body);

private:
    std::string url;
};

#endif // HTTP_REQUEST_HANDLER_H
