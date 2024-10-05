#pragma once
#include <vector>
#include <string>
#include <mutex>

// Менеджер для управления нодами по IP-адресам
class NodeManager {
public:
    // Добавление ноды по IP-адресу
    void add_ip(const std::string& ip);

    // Удаление ноды по IP-адресу
    void remove_ip(const std::string& ip);

    // Получение списка всех нод (IP-адресов)
    std::vector<std::string> get_ips() const;

private:
    std::vector<std::string> ip_addresses;  // Список IP-адресов нод
    mutable std::mutex mtx;  // Мьютекс для защиты данных
};
