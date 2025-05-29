
#pragma once
#include <array>
#include <atomic>
#include <cstdint>
#include <optional>

namespace dero::lockfree {

template <typename T>
class maybe {
private:
    enum class state { EMPTY, UPDATING, FULL };
    using storage_t = uint8_t[sizeof(T)];
    static_assert(std::atomic<state>::is_always_lock_free);

    alignas(T) storage_t m_storage;
    std::atomic<state> m_state{state::EMPTY};

public:
    maybe() = default;

    ~maybe() {
        while(m_state.load(std::memory_order_acquire) != state::EMPTY) {
            try_extract();
        }
    }

    bool try_set(const T& value) {
        auto expected = state::EMPTY;
        auto on_success = state::UPDATING;
        if(m_state.compare_exchange_strong(expected, on_success)) {
            new(m_storage) T(value);
            m_state.store(state::FULL, std::memory_order_release);
            return true;
        }
        return false;
    }

    std::optional<T> try_extract() {
        auto expected = state::FULL;
        auto on_success = state::UPDATING;
        if(m_state.compare_exchange_strong(expected, on_success)) {
            T* item = reinterpret_cast<T*>(m_storage);
            auto opt = std::optional<T>(std::move(*item));
            item->~T();
            m_state.store(state::EMPTY, std::memory_order_release);
            return opt;
        }
        return std::nullopt;
    }
};

} // namespace dero::lockfree