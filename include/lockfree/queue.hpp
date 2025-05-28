
#pragma once

#include "lockfree/maybe.hpp"
#include <atomic>
#include <cstddef>

namespace dero::lockfree {

template <typename T, size_t N>
class queue {
  private:
    std::array<maybe<T>, N> m_storage;

    std::atomic<size_t> m_size{0};
    std::atomic<size_t> m_push_index{0};
    std::atomic<size_t> m_pop_index{0};

    static_assert(std::atomic<size_t>::is_always_lock_free);

  public:
    queue() = default;

    ~queue() {
        while(size() > 0) {
            pop();
        }
    }

    size_t size() const {
        return m_size.load(std::memory_order_acquire);
    }

    bool push(const T& value) {
        while(m_size.load(std::memory_order_acquire) < N) {
            if(m_storage[m_push_index.load(std::memory_order_acquire) % N].try_set(value)) {
                m_push_index.fetch_add(1, std::memory_order_release);
                m_size.fetch_add(1, std::memory_order_release);
                return true;
            }
        }
        return false;
    }

    std::optional<T> pop() {
        while(m_size.load(std::memory_order_acquire) > 0) {
            auto res = m_storage[m_pop_index.load(std::memory_order_acquire) % N].try_extract();
            if(res.has_value()) {
                m_pop_index.fetch_add(1, std::memory_order_release);
                m_size.fetch_sub(1, std::memory_order_release);
                return res;
            }
        }
        return std::nullopt;
    }
};

} // namespace dero::lockfree