#include "event_handler.h"
#include <iostream>
#include <unordered_map>


EventHandler::EventHandler(RequestHandler& request_handler) : request_handler(request_handler) {}


void EventHandler::handle_events(const std::vector<EventType>& events) {
    
    std::vector<EventType> invoke_events;
    for (const auto& event : events) {
        if (event == EventType::INVOKE) {
            invoke_events.push_back(event);
        }
    }

    
    if (!invoke_events.empty()) {
        std::cout << "Received " << invoke_events.size() << " INVOKE events, initiating requests..." << std::endl;
        
        request_handler.invoke_request(invoke_events);
    }

    
    for (const auto& event : events) {
        if (event == EventType::NOTHING) {
            std::cout << "Event NOTHING, no action required." << std::endl;
        } else if (event == EventType::ERROR) {
            std::cerr << "Event ERROR, an error occurred." << std::endl;
        }
    }
}
