#pragma once

#include <cstdint>
#include <chrono>

namespace utils {

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

#define cauto const auto

#include "cocos2d.h"
namespace visible_size {
USING_NS_CC;

extern Director* director;

extern Size visibleSize;
extern Vec2 origin;
extern Vec2 center;
extern Vec2 cleft;
extern Vec2 cright;
extern Vec2 ctop;
extern Vec2 cbottom;
extern Vec2 tleft;
extern Vec2 tright;
extern Vec2 bleft;
extern Vec2 bright;

extern float left;
extern float right;
extern float top;
extern float bottom;

void init();

}
