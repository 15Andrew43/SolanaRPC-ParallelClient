#include "request_handler.h"
#include "responses_container.h"
#include <arpa/inet.h>  // Для inet_pton
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <thread>
#include <chrono>

// Структура для хранения информации о сокетах
struct SocketInfo {
    int sockfd;
    int event_id;
    bool request_sent = false;
    std::chrono::steady_clock::time_point start_time;  // Добавляем время начала для каждого сокета
};

RequestHandler::RequestHandler(NodeManager& node_manager) : node_manager(node_manager) {}

int set_non_blocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void send_http_request(int sockfd) {
    const char* request = 
        "POST / HTTP/1.1\r\n"
        "Host: api.mainnet-beta.solana.com\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 45\r\n"
        "\r\n"
        "{\"jsonrpc\":\"2.0\", \"id\":1, \"method\":\"getSlot\"}";

    ssize_t bytes_sent = send(sockfd, request, strlen(request), 0);
    if (bytes_sent < 0) {
        std::cerr << "Ошибка при отправке HTTP запроса\n";
    } else {
        std::cout << "HTTP запрос отправлен (" << bytes_sent << " байт)\n";
    }
}

std::string extract_http_body(const std::string& response) {
    size_t pos = response.find("\r\n\r\n");
    if (pos != std::string::npos) {
        return response.substr(pos + 4);  // Пропускаем заголовки и получаем только body
    }
    return "";  // Если не найден разделитель заголовков и тела
}

// Основная функция для обработки запросов с потокобезопасным хранением результатов
std::string RequestHandler::invoke_request(const std::vector<EventType>& events) {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Ошибка создания epoll" << std::endl;
        return "Ошибка";
    }

    std::unordered_map<int, std::vector<SocketInfo>> event_socket_map;  
    std::vector<epoll_event> epoll_events(10);  // Для хранения epoll событий
    int event_id = 0;

    // Для каждого события создаем сокеты и добавляем их в epoll
    for (const auto& event : events) {
        std::vector<SocketInfo> event_sockets;

        // Считаем время начала обработки для каждой группы событий
        auto start_time = std::chrono::steady_clock::now();

        for (const auto& ip_url : node_manager.get_ips()) {
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1) {
                std::cerr << "Ошибка создания сокета для " << ip_url << std::endl;
                continue;
            }
            set_non_blocking(sockfd);

            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(80);
            inet_pton(AF_INET, ip_url.c_str(), &server_addr.sin_addr);

            connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr));

            epoll_event epoll_event;
            SocketInfo* sock_info = new SocketInfo{sockfd, event_id, false, start_time};  // Добавляем start_time в SocketInfo
            epoll_event.data.ptr = sock_info;
            epoll_event.events = EPOLLIN | EPOLLOUT | EPOLLET;
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &epoll_event);

            event_sockets.push_back(*sock_info);
        }
        event_socket_map[event_id++] = event_sockets;
    }

    // Основной цикл ожидания событий
    while (!event_socket_map.empty()) {
        int n = epoll_wait(epoll_fd, epoll_events.data(), epoll_events.size(), -1);
        for (int i = 0; i < n; ++i) {
            SocketInfo* sock_info = static_cast<SocketInfo*>(epoll_events[i].data.ptr);
            int sockfd = sock_info->sockfd;
            int current_event_id = sock_info->event_id;

            // Если сокет готов к записи, отправляем запрос
            if (epoll_events[i].events & EPOLLOUT && !sock_info->request_sent) {
                send_http_request(sockfd);
                sock_info->request_sent = true;
            }

            // Если сокет готов к чтению, получаем ответ
            if (epoll_events[i].events & EPOLLIN) {
                char buffer[4096];
                ssize_t bytes = read(sockfd, buffer, sizeof(buffer));
                if (bytes > 0) {
                    buffer[bytes] = '\0';
                    std::string response(buffer);

                    std::string body = extract_http_body(response);

                    auto now = std::chrono::steady_clock::now();
                    std::chrono::milliseconds latency = std::chrono::duration_cast<std::chrono::milliseconds>(now - sock_info->start_time);
                    responses_container.add_response(body, now, latency);

                    std::cout << "Ответ получил поток: " << std::this_thread::get_id() << "\n";
                    std::cout << "Тело ответа: " << body << "\n";
                    std::cout << "Идентификатор события (event_id): " << current_event_id << "\n";

                    // Удаляем все сокеты для текущего события
                    for (const auto& info : event_socket_map[current_event_id]) {
                        if (info.sockfd != sockfd) {
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, info.sockfd, nullptr);
                            close(info.sockfd);
                        }
                    }

                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sockfd, nullptr);
                    close(sockfd);
                    event_socket_map.erase(current_event_id);

                    delete sock_info;
                }
            }
        }
    }

    close(epoll_fd);
    return "Все запросы завершены";
}
