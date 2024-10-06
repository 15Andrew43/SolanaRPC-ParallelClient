#include "requests_container.h"

// Добавление результата в контейнер
void RequestsContainer::add_result(const std::string& body, 
                                   const std::chrono::steady_clock::time_point& timestamp, 
                                   const std::chrono::milliseconds& latency) {
    RequestResult result{body, timestamp, latency};
    request_results.Push(result);
}

// Получение самого старого результата
std::optional<RequestResult> RequestsContainer::get_oldest_result() {
    return request_results.Pop();
}

// Закрытие контейнера
void RequestsContainer::close() {
    request_results.Close();
}
