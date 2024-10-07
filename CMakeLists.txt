cmake_minimum_required(VERSION 3.10)
project(SolanaRPCParallelClient)

set(CMAKE_CXX_STANDARD 17)

# Указываем папку include для заголовочных файлов
include_directories(include)

# Указываем исходные файлы с полными путями
add_executable(SolanaRPCParallelClient
    src/main.cpp
    src/node_manager.cpp
    src/request_handler.cpp
    src/event_handler.cpp
    src/thread_pool.cpp
    include/unbounded_blocking_queue.h
    src/responses_container.cpp
)

# Линковка с libcurl и pthread
find_package(CURL REQUIRED)
target_link_libraries(SolanaRPCParallelClient CURL::libcurl pthread)
