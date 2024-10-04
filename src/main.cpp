#include "http_request_handler.h"
#include <iostream>

int main() {
    // Создаем обработчик HTTP-запросов
    HttpRequestHandler handler("https://api.mainnet-beta.solana.com");

    // Генерируем событие HTTP_REQUEST
    handler.handle_event(EventType::HTTP_REQUEST);

    return 0;
}
