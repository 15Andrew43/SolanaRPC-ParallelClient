#pragma once
#include "request_handler.h"
#include "event_types.h"

class IEventHandler {
public:
    virtual void handle_event(EventType event) = 0;  // Теперь EventType определен
    virtual ~IEventHandler() {}
};

class EventHandler : public IEventHandler {
public:
    EventHandler(RequestHandler& request_handler);

    // Обработка события
    void handle_event(EventType event) override;

private:
    RequestHandler& request_handler;
};
