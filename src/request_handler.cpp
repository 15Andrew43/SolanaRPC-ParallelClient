#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstring>


RequestHandler::RequestHandler(NodeManager& node_manager) : node_manager(node_manager) {}


struct SocketInfo {
    int sockfd;      // Дескриптор сокета
    int event_id;    // Идентификатор события
};

// Устанавливаем неблокирующий режим для сокета
int set_non_blocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}



// Обработка событий через epoll с сокетами
std::string RequestHandler::invoke_request(const std::vector<EventType>& events) {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Ошибка создания epoll" << std::endl;
        return "Ошибка";
    }

    std::unordered_map<int, std::vector<SocketInfo>> event_socket_map;  // Группируем сокеты по событию (event_id)
    std::vector<epoll_event> epoll_events(1);  // Для хранения epoll событий
    int event_id = 0;  // Идентификатор для каждого события

    // Для каждого события создаем сокеты и добавляем их в epoll
    for (const auto& event : events) {
        std::vector<SocketInfo> event_sockets;

        for (const auto& ip_url : ip_urls) {
            // Создание сокета для каждой ноды
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1) {
                std::cerr << "Ошибка создания сокета для " << ip_url << std::endl;
                continue;
            }
            set_non_blocking(sockfd);

            // Подключение к IP-адресу и порту
            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(80);  // Порт HTTP
            inet_pton(AF_INET, ip_url.c_str(), &server_addr.sin_addr);  // Преобразование IP в двоичный формат

            connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr));

            // Добавляем сокет в epoll
            epoll_event epoll_event;
            SocketInfo* sock_info = new SocketInfo{sockfd, event_id};  // Метаинформация о сокете
            epoll_event.data.ptr = sock_info;                         // Привязываем метаинформацию к событию
            epoll_event.events = EPOLLIN | EPOLLOUT | EPOLLET;        // Устанавливаем события для отслеживания
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &epoll_event);

            // Добавляем сокет в группу для этого события
            event_sockets.push_back(*sock_info);
        }

        // Сохраняем все сокеты, связанные с этим событием
        event_socket_map[event_id++] = event_sockets;
    }

    // Основной цикл ожидания событий
    while (!event_socket_map.empty()) {
        int n = epoll_wait(epoll_fd, epoll_events.data(), epoll_events.size(), -1);
        for (int i = 0; i < n; ++i) {
            if (epoll_events[i].events & EPOLLIN) {
                // Обработка успешного ответа
                SocketInfo* sock_info = static_cast<SocketInfo*>(epoll_events[i].data.ptr);
                int sockfd = sock_info->sockfd;
                int current_event_id = sock_info->event_id;

                char buffer[1024];
                ssize_t bytes = read(sockfd, buffer, sizeof(buffer));
                if (bytes > 0) {
                    buffer[bytes] = '\0';
                    std::cout << "Ответ получен для event_id " << current_event_id << ": " << buffer << std::endl;

                    // Удаляем все сокеты для текущего события
                    for (const auto& info : event_socket_map[current_event_id]) {
                        if (info.sockfd != sockfd) {
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, info.sockfd, nullptr);
                            close(info.sockfd);
                        }
                    }

                    // Удаляем текущее событие из epoll и закрываем сокет
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sockfd, nullptr);
                    close(sockfd);

                    // Удаляем событие из карты
                    event_socket_map.erase(current_event_id);

                    // Освобождаем память
                    delete sock_info;
                }
            }
        }
    }

    // Закрываем epoll
    close(epoll_fd);
    return "Все запросы завершены";
}