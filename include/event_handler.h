#include "request_handler.h"
#include "responses_container.h"

class EventHandler {
public:
    EventHandler(RequestHandler& request_handler);

    
    void handle_events(const std::vector<EventType>& events);

    
    ResponseResult get_oldest_result();

private:
    RequestHandler& request_handler;
    ResponsesContainer responses_container;  
};
