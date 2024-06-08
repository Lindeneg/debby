#ifndef DEBBY_UTILS_H_
#define DEBBY_UTILS_H_

#include <algorithm>

#include "./globals.h"

template <typename T>
static inline T debby_abs(T x) noexcept {
    return (x < 0 ? -x : x);
}

template <typename T>
static inline bool debby_equal_rel(T a, T b) noexcept {
    return (debby_abs(a - b) <= (std::max(debby_abs(a), debby_abs(b)) *
                                 debby::constants::REL_EPSILION));
}

namespace debby::utils {
template <typename T>
inline bool equal(T a, T b) noexcept {
    if (debby_abs(a - b) <= debby::constants::ABS_EPSILION) {
        return true;
    }
    return debby_equal_rel(a, b);
}

template <typename T>
inline bool greater_or_equal(T a, T b) noexcept {
    if (equal(a, b)) {
        return true;
    }
    return a > b;
}

template <typename T>
inline bool less_or_equal(T a, T b) noexcept {
    if (equal(a, b)) {
        return true;
    }
    return a < b;
}

template <typename T>
inline bool greater(T a, T b) noexcept {
    if (equal<T>(a, b)) {
        return false;
    }
    return a > b;
}

template <typename T>
inline bool lesser(T a, T b) noexcept {
    if (equal(a, b)) {
        return false;
    }
    return a < b;
}

template <typename T>
inline T lerp(T a, T b, T t) noexcept {
    return a + t * (b - a);
}

template <typename T>
inline T to_radians(T degrees) noexcept {
    return (degrees * constants::PI) / 180.f;
}

template <typename T>
inline T to_degrees(T radians) noexcept {
    return (radians * 180.f) / constants::PI;
}

SColor color_to_s_color(const Color color) noexcept;
Color s_color_to_color(const SColor s_color) noexcept;
}  // namespace debby::utils

#endif  // DEBBY_UTILS_H_
