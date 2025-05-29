
#include <array>
#include <atomic>
#include <gtest/gtest.h>
#include <lockfree/queue.hpp>
#include <string>
#include <thread>

struct payload_t {
    size_t thread_id;
    size_t item_id;
};

template <size_t N>
void producer_function(dero::lockfree::queue<payload_t, N> &q, size_t tid) {
    for (size_t item_id = 0; item_id < 10; ++item_id) {
        payload_t item{tid, item_id};
        while (!q.push(item)) {
            q.push(item);
        }
    }
}

template <typename T, size_t N>
void consumer_function(dero::lockfree::queue<T, N> &q, const bool &done) {
    while (!done || q.size() > 0) {
        auto item = q.pop();
        if (item.has_value()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

TEST(Queue, MultithreadInsertionsAndDeletions) {

    dero::lockfree::queue<payload_t, 100> q;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (size_t thread_id = 0; thread_id < 10; ++thread_id) {
        producers.emplace_back(
            [&q, thread_id]() { producer_function(q, thread_id); });
    }

    bool producers_done = false;

    for (size_t thread_id = 0; thread_id < 10; ++thread_id) {
        consumers.emplace_back(
            [&q, &producers_done]() { consumer_function(q, producers_done); });
    }

    for (auto &producer : producers) {
        producer.join();
    }

    producers_done = true;

    for (auto &consumer : consumers) {
        consumer.join();
    }
}
