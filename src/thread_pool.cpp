#include <curl/curl.h>
#include <iostream>
#include <vector>
#include "thread_pool.h"

// Функция для записи данных из ответа HTTP-запроса
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Реализация функции для отправки 3 запросов и ожидания первого успешного ответа
std::string ThreadPool::send_requests(const std::vector<std::string>& urls) {
    CURLM* multi_handle = curl_multi_init();
    std::vector<CURL*> curl_handles(urls.size());
    std::vector<std::string> responses(urls.size());

    // Инициализация curl_easy для каждого URL
    for (size_t i = 0; i < urls.size(); ++i) {
        curl_handles[i] = curl_easy_init();
        curl_easy_setopt(curl_handles[i], CURLOPT_URL, urls[i].c_str());
        curl_easy_setopt(curl_handles[i], CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handles[i], CURLOPT_WRITEDATA, &responses[i]);
        curl_multi_add_handle(multi_handle, curl_handles[i]);
    }

    int still_running = 0;
    CURLMsg* msg;
    int msgs_left = 0;

    // Запуск запросов
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
                    for (size_t i = 0; i < curl_handles.size(); ++i) {
                        if (curl_handles[i] == easy_handle) {
                            response = responses[i];
                            break;
                        }
                    }

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
