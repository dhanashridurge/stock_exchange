#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
 
template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv;
 
public:
    void push(T val) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(val);
        }
        cv.notify_one();
    }
 
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{ return !q.empty(); });
 
        T val = q.front();
        q.pop();
        return val;
    }
    // Non-blocking try_pop
    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (q.empty()) {
            return std::nullopt;
        }
        T val = q.front();
        q.pop();
        return val;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }
};