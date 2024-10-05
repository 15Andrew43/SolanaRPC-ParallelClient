#include "node_manager.h"
#include <algorithm>

// Добавление ноды
void NodeManager::add_node(const std::string& url) {
    std::lock_guard<std::mutex> lock(mtx);
    nodes.push_back(url);
}

// Удаление ноды
void NodeManager::remove_node(const std::string& url) {
    std::lock_guard<std::mutex> lock(mtx);
    nodes.erase(std::remove(nodes.begin(), nodes.end(), url), nodes.end());
}

// Получение списка всех нод
std::vector<std::string> NodeManager::get_nodes() const {
    std::lock_guard<std::mutex> lock(mtx);
    return nodes;
}
