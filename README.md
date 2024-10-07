# Solana RPC Event Handler

## Task Description

This project involves implementing a C++ client that interacts with the Solana blockchain through the Solana RPC API. The project is divided into two main tasks, with an optional third task for extended functionality:

### Task 1: Solana RPC GET Methods Implementation
Your objective is to implement one or more GET methods from the Solana RPC API using C++. You have the freedom to choose any appropriate libraries and design the program's architecture as you see fit. This task provides an opportunity to showcase your coding style, best practices, and approach to writing clean and scalable code.

**Submission Requirements**: Your submission should include the source code and a README file. The README should outline your design and architectural choices, explain which libraries you chose and why, and provide instructions on how to build and run your code.

### Task 2: Parallel Event Handling
Building upon the GET method implemented in Task 1, this task involves implementing an event handler to process incoming events in parallel. The event types are:
- **INVOKE**: Execute the GET method in the background upon receiving an INVOKE event, ensuring it does not block the main thread.
- **NOTHING**: No action is required.
- **ERROR**: Display an error message but do not terminate the program.

Additionally, you must store the results of the GET method in a common container that allows the main thread to retrieve the oldest result in O(1) time complexity. The results should be sorted by the timestamp specified in the HTTP response and include the latency of the request.

**Submission Requirements**: Your submission should include the source code and a README file. The README should detail your design and architecture choices, explain your selection of libraries and their justification, and provide instructions on how to build and run your code.

### Task 3 (Optional): Real-time Standard Deviation Tracking
Enhance the functionality of the program in Task 2 to support real-time tracking of the standard deviation of request latencies. This tracking should cover all GET requests made within a specified time window (T), starting from the latest response timestamp (X) and extending backward to (X − T). The goal is to have efficient queries for these statistics.

**Suggestion**: Consider using an event handler class for processing the events:
```cpp
enum class EventType {INVOKE, NOTHING, ERROR};
class IEventHandler {
public:
    virtual void handle_event(EventType event) = 0;
    virtual ~IEventHandler() {}
};
```

## Submission Requirements
- **Source Code**: Include all source files.
- **README**: A README file that explains your design and architectural choices, the libraries used and why, and instructions for building and running the project.




# Solana RPC Parallel Client

This project implements a parallel client that interacts with the Solana blockchain using the Solana RPC API. It performs HTTP requests in parallel to multiple Solana nodes, processes incoming events, and stores results in a thread-safe queue. The solution is designed with scalability and efficiency in mind, using a thread pool to manage concurrent tasks and `epoll` for efficient I/O multiplexing.

## Project Structure

```
.
├── include/                 # Header files
│   ├── event_handler.h       # EventHandler class declaration
│   ├── event_types.h         # Definition of event types (INVOKE, NOTHING, ERROR)
│   ├── node_manager.h        # NodeManager class declaration
│   ├── request_handler.h     # RequestHandler class declaration
│   ├── responses_container.h # ResponsesContainer class declaration (thread-safe container for results)
│   ├── thread_pool.h         # ThreadPool class declaration (manages worker threads)
│   └── unbounded_blocking_queue.h # UnboundedBlockingQueue declaration (generic thread-safe queue)
├── src/                     # Source files
│   ├── event_handler.cpp     # EventHandler class implementation
│   ├── main.cpp              # Entry point for the application
│   ├── node_manager.cpp      # NodeManager class implementation
│   ├── request_handler.cpp   # RequestHandler class implementation (handles Solana RPC requests)
│   ├── responses_container.cpp # ResponsesContainer implementation (stores results)
│   └── thread_pool.cpp       # ThreadPool implementation (worker thread management)
├── experiments/              # Experimental files or test cases
│   └── main.cpp              # Example code or testing area
├── CMakeLists.txt            # CMake build configuration
└── README.md                 # Project documentation
```

## Components Overview

### 1. **NodeManager** (`node_manager.h/cpp`)
   - **Purpose**: Manages the list of Solana node IP addresses.
   - **Responsibilities**: 
     - Add and remove node IPs.
     - Retrieve the list of nodes for making parallel requests.

### 2. **EventHandler** (`event_handler.h/cpp`)
   - **Purpose**: Processes incoming events and invokes corresponding Solana RPC requests.
   - **Responsibilities**:
     - Filters events such as `INVOKE`, `NOTHING`, and `ERROR`.
     - Handles multiple events concurrently using the `RequestHandler`.

### 3. **RequestHandler** (`request_handler.h/cpp`)
   - **Purpose**: Handles sending parallel HTTP requests to Solana RPC nodes using `epoll`.
   - **Responsibilities**:
     - For each event, generate HTTP requests to multiple nodes.
     - Uses non-blocking sockets and `epoll` to handle multiple connections efficiently.
     - Measures request latency and stores the result in the `ResponsesContainer`.

### 4. **ResponsesContainer** (`responses_container.h/cpp`)
   - **Purpose**: A thread-safe container that stores the results of completed Solana RPC requests.
   - **Responsibilities**:
     - Store responses with their timestamps and latency.
     - Allow the main thread to retrieve the oldest result in O(1) time complexity.

### 5. **ThreadPool** (`thread_pool.h/cpp`)
   - **Purpose**: Manages a fixed pool of worker threads to handle events concurrently.
   - **Responsibilities**:
     - Assign tasks to worker threads.
     - Ensure efficient utilization of system resources by managing concurrent tasks.

### 6. **UnboundedBlockingQueue** (`unbounded_blocking_queue.h`)
   - **Purpose**: A generic, thread-safe, unbounded blocking queue.
   - **Responsibilities**:
     - Used to store events or request results in a thread-safe manner.
     - Provides synchronization mechanisms to ensure safe concurrent access.

---

This design ensures that events are handled concurrently, requests are processed efficiently using `epoll` for I/O operations, and results are stored in a thread-safe manner for further processing.
