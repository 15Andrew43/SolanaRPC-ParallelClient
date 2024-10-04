cmake_minimum_required(VERSION 3.10)

# Название проекта
project(SolanaRPC-ParallelClient)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ищем библиотеку libcurl
find_package(CURL REQUIRED)

# Включаем директории с заголовочными файлами
include_directories(include)

# Добавляем все исходные файлы из директории src
set(SOURCES
    src/main.cpp
    src/http_request_handler.cpp
)

# Создаем исполняемый файл из исходных файлов
add_executable(SolanaRPC-ParallelClient ${SOURCES})

# Линкуем проект с библиотекой curl
target_link_libraries(SolanaRPC-ParallelClient CURL::libcurl pthread)
