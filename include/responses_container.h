#pragma once
#include "unbounded_blocking_queue.h"
#include <string>
#include <chrono>
#include <optional>


struct ResponseResult {
    std::string body;
    std::chrono::steady_clock::time_point timestamp;
    std::chrono::milliseconds latency;
};


class ResponsesContainer {
public:
    
    void add_response(const std::string& body, 
                      const std::chrono::steady_clock::time_point& timestamp, 
                      const std::chrono::milliseconds& latency);

    
    std::optional<ResponseResult> get_oldest_response();

    
    void close();

private:
    UnboundedBlockingQueue<ResponseResult> response_results;  
};
