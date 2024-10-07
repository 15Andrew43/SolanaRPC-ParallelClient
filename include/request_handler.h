#pragma once

#include "responses_container.h"
#include "event_types.h"  
#include "node_manager.h"
#include <vector>

class RequestHandler {
public:
    RequestHandler(NodeManager& node_manager);

    std::string invoke_request(const std::vector<EventType>& events);

private:
    ResponsesContainer responses_container;
    NodeManager& node_manager;
};
