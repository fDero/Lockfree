
#pragma once
#include <lockfree/queue.hpp>

struct payload_t {
    size_t thread_id;
    size_t item_id;
};

template <typename Queue> void producer_function(Queue& q, size_t tid) {
    for (size_t item_id = 0; item_id < 10; ++item_id) {
        payload_t item{tid, item_id};
        while (!q.push(item)) {
            q.push(item);
        }
    }
}

template <typename Queue> void consumer_function(Queue& q, const bool& done) {
    while (!done || q.size() > 0) {
        auto item = q.pop();
        if (item.has_value()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

template <typename Queue, size_t CONSUMERS, size_t PRODUCERS>
void multiple_push_multiple_pops_concurrently() {
    Queue q;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    bool producers_done = false;

    for (size_t thread_id = 0; thread_id < PRODUCERS; ++thread_id) {
        producers.emplace_back(
            [&q, thread_id]() { producer_function(q, thread_id); });
    }

    for (size_t thread_id = 0; thread_id < CONSUMERS; ++thread_id) {
        consumers.emplace_back(
            [&q, &producers_done]() { consumer_function(q, producers_done); });
    }

    for (auto& producer : producers) {
        producer.join();
    }

    producers_done = true;

    for (auto& consumer : consumers) {
        consumer.join();
    }
}