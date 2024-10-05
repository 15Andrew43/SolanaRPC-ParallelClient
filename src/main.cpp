#include "event_handler.h"
#include "node_manager.h"
#include "request_handler.h"
#include "thread_pool.h"
#include "thread_safe_event_queue.h"
#include <iostream>

int main() {
    // Создаем менеджер нод
    NodeManager node_manager;
    node_manager.add_node("https://api.mainnet-beta.solana.com");
    node_manager.add_node("https://api.devnet.solana.com");

    // Создаем обработчик запросов
    RequestHandler request_handler(node_manager);

    // Создаем пул потоков (например, 4 потока)
    ThreadPool thread_pool(4);

    // Создаем очередь событий
    ThreadSafeEventQueue event_queue;

    // Добавляем задачи в пул (например, 10 задач)
    for (int i = 0; i < 10; ++i) {
        thread_pool.enqueue_task([&request_handler] {
            // Запускаем запрос к нодам
            std::string result = request_handler.invoke_request();
            if (!result.empty()) {
                std::cout << "Результат: " << result << std::endl;
            }
        });
    }

    // Пул будет работать, пока задачи не будут завершены
    return 0;
}
