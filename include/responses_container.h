#pragma once
#include "unbounded_blocking_queue.h"
#include <string>
#include <chrono>
#include <optional>

// Struct for storing the HTTP response result
struct ResponseResult {
    std::string body;
    std::chrono::steady_clock::time_point timestamp;
    std::chrono::milliseconds latency;
};

// Class for managing the responses from HTTP requests
class ResponsesContainer {
public:
    // Add a response result to the container
    void add_response(const std::string& body, 
                      const std::chrono::steady_clock::time_point& timestamp, 
                      const std::chrono::milliseconds& latency);

    // Get the oldest response result
    std::optional<ResponseResult> get_oldest_response();

    // Close the container
    void close();

private:
    UnboundedBlockingQueue<ResponseResult> response_results;  // Queue to store response results
};
