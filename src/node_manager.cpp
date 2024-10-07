#include "node_manager.h"
#include <algorithm>


void NodeManager::add_ip(const std::string& ip) {
    std::lock_guard<std::mutex> lock(mtx);  
    ip_addresses.push_back(ip);  
}


void NodeManager::remove_ip(const std::string& ip) {
    std::lock_guard<std::mutex> lock(mtx);  
    ip_addresses.erase(std::remove(ip_addresses.begin(), ip_addresses.end(), ip), ip_addresses.end());
}


std::vector<std::string> NodeManager::get_ips() const {
    std::lock_guard<std::mutex> lock(mtx);  
    return ip_addresses;  
}
