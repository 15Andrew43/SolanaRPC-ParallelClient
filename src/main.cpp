#include "event_handler.h"
#include "node_manager.h"
#include "request_handler.h"
#include "thread_pool.h"
#include "unbounded_blocking_queue.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
    int events_to_process = 5;  // Default number of events per task
    if (argc > 1) {
        events_to_process = std::stoi(argv[1]);  // Get value from command line argument
    }

    NodeManager node_manager;
    node_manager.add_ip("72.46.84.7");
    node_manager.add_ip("67.209.52.66");

    RequestHandler request_handler(node_manager);
    EventHandler event_handler(request_handler);

    int n_threads = 4;
    ThreadPool pool(n_threads);
    pool.Start();

    UnboundedBlockingQueue<EventType> event_queue;

    // Add events to the queue
    const int total_events = 30;
    for (int i = 0; i < total_events; ++i) {
        event_queue.Push(EventType::INVOKE);
    }

    // Creating multiple tasks in the pool, each processing a set of events
    const int task_count = 6;
    for (int t = 0; t < task_count; ++t) {
        pool.Submit([&event_queue, &event_handler, events_to_process, t] {
            std::vector<EventType> events;

            // Fetch a specific number of events for each task
            for (int i = 0; i < events_to_process; ++i) {
                auto event_opt = event_queue.Pop();
                if (event_opt) {
                    events.push_back(*event_opt);
                }
            }

            // If we have events, process them in a thread
            if (!events.empty()) {
                std::cout << "Thread " << t << " is processing " << events.size() << " events\n";
                event_handler.handle_events(events);
            }
        });
    }

    // Allow time for tasks to process
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop the thread pool after processing
    pool.Stop();

    return 0;
}
