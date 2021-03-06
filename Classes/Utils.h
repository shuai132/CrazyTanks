#pragma once

#include <cstdint>
#include <chrono>

namespace utils {

#define cauto const auto

template <typename T>
inline void setLimit(T& v, T min, T max) {
    if (v < min) {
        v = min;
    } else if (v > max) {
        v = max;
    }
}

#define FOR(i, n)   for(std::remove_reference<std::remove_const<decltype(n)>::type>::type i = 0; i < n; i++)

inline uint64_t nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

}

