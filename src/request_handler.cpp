#include "request_handler.h"
#include <iostream>
#include <curl/curl.h>

// Конструктор
RequestHandler::RequestHandler(NodeManager& node_manager) : node_manager(node_manager) {}

// Функция для записи данных из HTTP-ответа
size_t RequestHandler::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Выполнение GET-запроса к нодам и ожидание первого успешного
std::string RequestHandler::invoke_request() {
    std::vector<std::string> urls = node_manager.get_nodes();  // Получаем список нод

    if (urls.empty()) {
        std::cerr << "Нет доступных нод" << std::endl;
        return "";
    }

    CURLM* multi_handle = curl_multi_init();
    std::vector<CURL*> curl_handles(urls.size());
    std::vector<std::string> responses(urls.size());

    // Инициализация curl_easy для каждого URL
    for (size_t i = 0; i < urls.size(); ++i) {
        curl_handles[i] = curl_easy_init();
        curl_easy_setopt(curl_handles[i], CURLOPT_URL, urls[i].c_str());
        curl_easy_setopt(curl_handles[i], CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handles[i], CURLOPT_WRITEDATA, &responses[i]);

        // Формируем запрос для метода getSlot
        const char* json_data = R"({"jsonrpc":"2.0","id":1,"method":"getSlot"})";
        curl_easy_setopt(curl_handles[i], CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl_handles[i], CURLOPT_HTTPHEADER, nullptr);
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl_handles[i], CURLOPT_HTTPHEADER, headers);

        // Добавляем в мульти-обработчик
        curl_multi_add_handle(multi_handle, curl_handles[i]);
    }

    int still_running = 0;
    CURLMsg* msg;
    int msgs_left = 0;

    // Запуск параллельных запросов
    curl_multi_perform(multi_handle, &still_running);

    // Ожидание первого успешного ответа
    while (still_running) {
        int numfds;
        curl_multi_wait(multi_handle, nullptr, 0, 1000, &numfds);
        curl_multi_perform(multi_handle, &still_running);

        // Проверка завершенных запросов
        while ((msg = curl_multi_info_read(multi_handle, &msgs_left))) {
            if (msg->msg == CURLMSG_DONE) {
                CURL* easy_handle = msg->easy_handle;
                CURLcode result = msg->data.result;

                if (result == CURLE_OK) {
                    // Успешный запрос
                    char* url;
                    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &url);
                    std::string response;
                    std::string fastest_node;
                    for (size_t i = 0; i < curl_handles.size(); ++i) {
                        if (curl_handles[i] == easy_handle) {
                            response = responses[i];
                            fastest_node = urls[i];
                            break;
                        }
                    }

                    // Выводим информацию о самой быстрой ноде
                    std::cout << "Первый успешный ответ получен от ноды: " << fastest_node << std::endl;
                    std::cout << "Ответ: " << response << std::endl;

                    // Удаляем остальные запросы
                    for (CURL* handle : curl_handles) {
                        if (handle != easy_handle) {
                            curl_multi_remove_handle(multi_handle, handle);
                            curl_easy_cleanup(handle);
                        }
                    }

                    // Возвращаем успешный ответ
                    curl_multi_cleanup(multi_handle);
                    return response;
                }
            }
        }
    }

    // Очищаем все запросы, если нет успешных
    for (CURL* handle : curl_handles) {
        curl_multi_remove_handle(multi_handle, handle);
        curl_easy_cleanup(handle);
    }
    curl_multi_cleanup(multi_handle);
    return "";
}
