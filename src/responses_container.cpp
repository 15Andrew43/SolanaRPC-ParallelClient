#include "responses_container.h"


void ResponsesContainer::add_response(const std::string& body,
                                      const std::chrono::steady_clock::time_point& timestamp,
                                      const std::chrono::milliseconds& latency) {
    ResponseResult result{body, timestamp, latency};
    response_results.Push(result);
}


std::optional<ResponseResult> ResponsesContainer::get_oldest_response() {
    return response_results.Pop();
}


void ResponsesContainer::close() {
    response_results.Close();
}
