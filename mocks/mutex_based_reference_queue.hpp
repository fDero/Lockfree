
#pragma once
#include <mutex>
#include <queue>

template <typename T, size_t N> class mutex_based_reference_queue {
  private:
    std::queue<T> m_underlying_queue;
    mutable std::mutex m_mutex;

  public:
    size_t size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_underlying_queue.size();
    }

    bool push(const T& value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_underlying_queue.size() >= N) {
            return false;
        }
        m_underlying_queue.push(value);
        return true;
    }

    std::optional<T> pop() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_underlying_queue.empty()) {
            return std::nullopt;
        }
        T value = m_underlying_queue.front();
        m_underlying_queue.pop();
        return value;
    }
};