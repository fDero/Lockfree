#pragma once

#include "lockfree/maybe.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <thread>

namespace dero::lockfree {

template <typename T, size_t N> class queue {
  private:
    std::array<maybe<T>, N> m_storage;

    std::atomic<size_t> m_size{0};
    std::atomic<size_t> m_push_index{0};
    std::atomic<size_t> m_pop_index{0};

    static_assert(std::atomic<size_t>::is_always_lock_free);

  public:
    queue() = default;
    ~queue() = default;

    size_t size() const {
        return m_size.load(std::memory_order_acquire);
    }

    bool push(const T& value) {
        while (m_size.load(std::memory_order_acquire) < N) {
            size_t index = m_push_index.load(std::memory_order_acquire);
            if (m_storage[index % N].try_set(value)) {
                m_push_index.fetch_add(1, std::memory_order_release);
                m_size.fetch_add(1, std::memory_order_release);
                return true;
            }
        }
        return false;
    }

    std::optional<T> pop() {
        size_t attempts = 0;
        while (m_size.load(std::memory_order_acquire) > 0) {
            size_t index = m_pop_index.load(std::memory_order_acquire);
            constexpr auto ord = std::memory_order_acq_rel;
            if (!m_pop_index.compare_exchange_weak(index, index + 1, ord)) {
                continue;
            }
            auto out = m_storage[index % N].try_extract();
            if (out.has_value()) {
                m_size.fetch_sub(1, std::memory_order_release);
                return out;
            }
            backoff(attempts++);
        }
        return std::nullopt;
    }

  private:
    void backoff(size_t attempts) {
        if (attempts < 64) {
            std::this_thread::yield();
        } else if (attempts < 128) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        } else if (attempts > 16) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }
};

} // namespace dero::lockfree