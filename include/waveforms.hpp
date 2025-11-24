#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

namespace CE_Math {
    constexpr float PI = 3.14159265358979323846f;

    constexpr float sin_taylor(float x) {
        while (x > PI) x -= 2.0f * PI;
        while (x < -PI) x += 2.0f * PI;
        
        float x2 = x * x;
        float x3 = x2 * x;
        float x5 = x3 * x2;
        float x7 = x5 * x2;
        
        return x - (x3 / 6.0f) + (x5 / 120.0f) - (x7 / 5040.0f);
    }
}

template <size_t N>
constexpr std::array<uint16_t, N> make_sine_pattern() {
    std::array<uint16_t, N> arr{};
    
    for (size_t i = 0; i < N; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(N);
        float rads = t * 2.0f * CE_Math::PI;
        
        float raw = CE_Math::sin_taylor(rads - (CE_Math::PI / 2.0f));
        float normalized = (raw + 1.0f) / 2.0f;
        
        float val = normalized * normalized * 4095.0f;
        arr[i] = static_cast<uint16_t>(val);
    }
    return arr;
}