#include "responses_container.h"

// Implementation of add_response
void ResponsesContainer::add_response(const std::string& body,
                                      const std::chrono::steady_clock::time_point& timestamp,
                                      const std::chrono::milliseconds& latency) {
    ResponseResult result{body, timestamp, latency};
    response_results.Push(result);
}

// Implementation of get_oldest_response
std::optional<ResponseResult> ResponsesContainer::get_oldest_response() {
    return response_results.Pop();
}

// Implementation of close
void ResponsesContainer::close() {
    response_results.Close();
}
