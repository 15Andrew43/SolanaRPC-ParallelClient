#pragma once
#include <deque>
#include <optional>
#include <mutex>
#include <condition_variable>

template <typename T>
class UnboundedBlockingQueue {
public:
    
    void Push(T item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (is_closed_) {
            return;
        }
        buffer_.push_back(std::move(item));
        not_empty_.notify_one();  
    }

    
    std::optional<T> Pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!is_closed_ && buffer_.empty()) {
            not_empty_.wait(lock);  
        }
        if (is_closed_ && buffer_.empty()) {
            return std::nullopt;
        }
        return TakeLocked();  
    }

    
    void Close() {
        std::lock_guard<std::mutex> lock(mutex_);
        is_closed_ = true;
        not_empty_.notify_all();  
    }

private:
    
    T TakeLocked() {
        auto top = std::move(buffer_.front());
        buffer_.pop_front();
        return top;
    }

private:
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::deque<T> buffer_;
    bool is_closed_ = false;
};
