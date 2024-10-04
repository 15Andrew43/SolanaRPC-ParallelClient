#include "http_request_handler.h"
#include <iostream>
#include <curl/curl.h>

HttpRequestHandler::HttpRequestHandler(const std::string& url) : url(url) {}

// Колбэк для записи данных
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Обработка события
void HttpRequestHandler::handle_event(EventType event) {
    if (event == EventType::HTTP_REQUEST) {
        std::string request_body = R"({
            "jsonrpc": "2.0",
            "id": 1,
            "method": "getSlot",
            "params": []
        })";

        // Выполняем запрос
        std::string response = send_post_request(request_body);
        std::cout << "Ответ от узла: " << response << std::endl;
    }
}

// Выполнение POST-запроса
std::string HttpRequestHandler::send_post_request(const std::string& request_body) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Ошибка инициализации curl" << std::endl;
        return "";
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.c_str());

    // Колбэк для получения данных
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    // Выполняем запрос
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Ошибка выполнения запроса: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return readBuffer;
}
