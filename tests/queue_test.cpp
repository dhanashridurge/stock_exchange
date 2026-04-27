#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include "../src/core/ThreadSafeQueue.hpp"
 
TEST(ThreadSafeQueueTest, PushPop) {
    ThreadSafeQueue<int> queue;
    queue.push(42);
    auto val = queue.try_pop();
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(val.value(), 42);
}
 
TEST(ThreadSafeQueueTest, MultipleElements) {
    ThreadSafeQueue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    
    auto v1 = queue.try_pop();
    auto v2 = queue.try_pop();
    auto v3 = queue.try_pop();
    
    ASSERT_TRUE(v1.has_value());
    ASSERT_TRUE(v2.has_value());
    ASSERT_TRUE(v3.has_value());
    EXPECT_EQ(v1.value(), 1);
    EXPECT_EQ(v2.value(), 2);
    EXPECT_EQ(v3.value(), 3);
}
 
TEST(ThreadSafeQueueTest, ThreadedPushPop) {
    ThreadSafeQueue<int> queue;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    // Producer threads
    for (int i = 0; i < 3; ++i) {
        producers.emplace_back([&queue, i]() {
            for (int j = 0; j < 100; ++j) {
                queue.push(i * 1000 + j);
            }
        });
    }
    
    // Consumer threads
    std::atomic<int> consumed{0};
    for (int i = 0; i < 3; ++i) {
        consumers.emplace_back([&queue, &consumed]() {
            for (int j = 0; j < 100; ++j) {
                queue.try_pop();
                consumed.fetch_add(1);
            }
        });
    }
    
    for (auto& t : producers) {
        t.join();
    }
    for (auto& t : consumers) {
        t.join();
    }
    
    EXPECT_EQ(consumed.load(), 300);
}
 
TEST(ThreadSafeQueueTest, StringType) {
    ThreadSafeQueue<std::string> queue;
    queue.push("Hello");
    queue.push("World");
    
    auto v1 = queue.try_pop();
    auto v2 = queue.try_pop();
    
    ASSERT_TRUE(v1.has_value());
    ASSERT_TRUE(v2.has_value());
    EXPECT_EQ(v1.value(), "Hello");
    EXPECT_EQ(v2.value(), "World");
}
 
TEST(ThreadSafeQueueTest, EmptyCheck) {
    ThreadSafeQueue<int> queue;
    EXPECT_TRUE(queue.empty());
    queue.push(1);
    EXPECT_FALSE(queue.empty());
}