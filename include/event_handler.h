#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

enum class EventType { HTTP_REQUEST };

class IEventHandler {
public:
    virtual void handle_event(EventType event) = 0;
    virtual ~IEventHandler() {}
};

#endif // EVENT_HANDLER_H
