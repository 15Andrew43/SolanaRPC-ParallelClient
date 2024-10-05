#include "event_handler.h"
#include "event_types.h"
#include "node_manager.h"
#include "request_handler.h"

int main() {
    // Создаем менеджер нод и добавляем Solana RPC ноды
    NodeManager node_manager;
    node_manager.add_node("https://api.devnet.solana.com");
    node_manager.add_node("https://api.mainnet-beta.solana.com");
    

    // Создаем обработчик запросов
    RequestHandler request_handler(node_manager);

    // Создаем обработчик событий
    EventHandler event_handler(request_handler);

    // Симулируем обработку событий
    event_handler.handle_event(EventType::INVOKE);
    event_handler.handle_event(EventType::NOTHING);
    event_handler.handle_event(EventType::ERROR);

    return 0;
}
