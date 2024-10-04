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