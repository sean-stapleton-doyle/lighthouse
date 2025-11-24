#pragma once
#include <vector>
#include <cmath>
#include <cstdint>

struct CircularPattern {
    std::vector<uint16_t> values;

    uint16_t at(size_t index) const {
        if (values.empty()) return 0;
        return values[index % values.size()];
    }

    size_t size() const { return values.size(); }

    static CircularPattern createSine(size_t steps, float gamma = 2.2f) {
        CircularPattern p;
        p.values.reserve(steps);
        for (size_t i = 0; i < steps; i++) {
            float angle = (float)i / steps * 2.0f * 3.14159f;
            float raw = (sin(angle - 1.57079f) + 1.0f) / 2.0f;
            
            float corrected = pow(raw, gamma);
            
            p.values.push_back(static_cast<uint16_t>(corrected * 4095.0f));
        }
        return p;
    }
    
    static CircularPattern createRamp(size_t steps, bool up = true) {
        CircularPattern p;
        p.values.reserve(steps);
        for (size_t i = 0; i < steps; i++) {
            float progress = (float)i / (steps - 1);
            if (!up) progress = 1.0f - progress;
            p.values.push_back(static_cast<uint16_t>(progress * 4095.0f));
        }
        return p;
    }
};