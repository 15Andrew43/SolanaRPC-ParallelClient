#pragma once
#include "request_handler.h"
#include <unordered_map>
#include <vector>
#include "event_types.h"

class IEventHandler {
public:
    virtual void handle_events(const std::vector<EventType>& events) = 0;
    virtual ~IEventHandler() {}
};

class EventHandler : public IEventHandler {
public:
    EventHandler(RequestHandler& request_handler);

    // Обработка нескольких событий
    void handle_events(const std::vector<EventType>& events) override;

private:
    RequestHandler& request_handler;
};
