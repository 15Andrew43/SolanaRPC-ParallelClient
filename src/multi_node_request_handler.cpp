#include "multi_node_request_handler.h"
#include <iostream>
#include <curl/curl.h>
#include <algorithm>

// Конструктор
MultiNodeRequestHandler::MultiNodeRequestHandler() {}

// Добавляем узел в список
void MultiNodeRequestHandler::add_node(const std::string& url) {
    nodes.push_back(url);
}

// Удаляем узел из списка
void MultiNodeRequestHandler::remove_node(const std::string& url) {
    nodes.erase(std::remove(nodes.begin(), nodes.end(), url), nodes.end());
}

// Отправка POST-запроса
std::string MultiNodeRequestHandler::send_post_request(const std::string& url, const std::string& request_body) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Ошибка инициализации curl");
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Ошибка выполнения запроса к " << url << ": " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return readBuffer;
}

// Отправка запросов ко всем узлам и получение первого ответа
std::string MultiNodeRequestHandler::send_request(const std::string& request_body) {
    std::vector<std::future<std::string>> futures;

    for (const auto& node : nodes) {
        futures.push_back(std::async(std::launch::async, &MultiNodeRequestHandler::send_post_request, this, node, request_body));
    }

    for (auto& fut : futures) {
        std::string result = fut.get();
        if (!result.empty()) {
            return result;
        }
    }

    return "Нет доступных узлов или все запросы не удались.";
}
