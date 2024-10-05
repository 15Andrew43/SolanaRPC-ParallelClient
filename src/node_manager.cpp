#include "node_manager.h"

// Добавление ноды по IP-адресу
void NodeManager::add_ip(const std::string& ip) {
    std::lock_guard<std::mutex> lock(mtx);  // Блокировка мьютекса
    ip_addresses.push_back(ip);  // Добавляем IP в список
}

// Удаление ноды по IP-адресу
void NodeManager::remove_ip(const std::string& ip) {
    std::lock_guard<std::mutex> lock(mtx);  // Блокировка мьютекса
    ip_addresses.erase(std::remove(ip_addresses.begin(), ip_addresses.end(), ip), ip_addresses.end());
}

// Получение списка всех нод (IP-адресов)
std::vector<std::string> NodeManager::get_ips() const {
    std::lock_guard<std::mutex> lock(mtx);  // Блокировка мьютекса
    return ip_addresses;  // Возвращаем копию списка IP-адресов
}
