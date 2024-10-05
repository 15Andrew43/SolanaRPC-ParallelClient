#include "event_handler.h"
#include "node_manager.h"
#include "request_handler.h"
#include "thread_pool.h"
#include <iostream>

int main() {
    // Создаем менеджер нод
    NodeManager node_manager;
    node_manager.add_node("https://api.mainnet-beta.solana.com");
    node_manager.add_node("https://api.devnet.solana.com");

    // Создаем обработчик запросов
    RequestHandler request_handler(node_manager);

    // Создаем пул потоков
    ThreadPool pool(4);
    pool.Start();

    // Добавляем несколько задач, которые вызывают RequestHandler
    for (int i = 0; i < 10; ++i) {
        pool.Submit([&request_handler, i] {
            std::cout << "Задача " << i << " выполняется в потоке: " 
                      << std::this_thread::get_id() << std::endl;
            // Вызываем запрос к ноде через request_handler
            std::string result = request_handler.invoke_request();
            std::cout << "Результат запроса: " << result << std::endl;
        });
    }

    // Ждем выполнения задач и останавливаем пул
    std::this_thread::sleep_for(std::chrono::seconds(5));
    pool.Stop();

    return 0;
}
