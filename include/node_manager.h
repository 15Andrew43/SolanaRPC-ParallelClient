#pragma once
#include <vector>
#include <string>
#include <mutex>


class NodeManager {
public:
    
    void add_ip(const std::string& ip);

    
    void remove_ip(const std::string& ip);

    
    std::vector<std::string> get_ips() const;

private:
    std::vector<std::string> ip_addresses;  
    mutable std::mutex mtx;  
};
