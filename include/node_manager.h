#pragma once
#include <vector>
#include <string>
#include <mutex>

// Менеджер для управления нодами
class NodeManager {
public:
    // Добавление ноды
    void add_node(const std::string& url);

    // Удаление ноды
    void remove_node(const std::string& url);

    // Получение списка всех нод
    std::vector<std::string> get_nodes() const;

private:
    std::vector<std::string> nodes;  // Список URL-ов нод
    mutable std::mutex mtx;  // Мьютекс для защиты данных
};
