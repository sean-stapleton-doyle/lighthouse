#pragma once

#include <array>
#include <cstdint>
#include "circular_pattern.hpp"

class FadeController {
public:
    static constexpr uint8_t MAX_LEDS = 15;
    
    struct LedState {
        const CircularPattern* pattern = nullptr;
        size_t current_index = 0;
        bool active = false;
        uint16_t current_pwm = 0;
    };
    
    FadeController() = default;
    
    // Assign a pattern to an LED
    // offset_percent: 0.0 to 1.0, allows LEDs sharing the same buffer 
    // to be out of phase (e.g., chasing effect)
    void assignPattern(uint8_t led, const CircularPattern& pattern, float offset_percent = 0.0f) {
        if (led >= MAX_LEDS) return;
        if (pattern.size() == 0) return;

        leds_[led].pattern = &pattern;
        leds_[led].active = true;
        
        size_t start_idx = static_cast<size_t>(offset_percent * pattern.size());
        leds_[led].current_index = start_idx % pattern.size();
    }
    
    void stop(uint8_t led) {
        if (led >= MAX_LEDS) return;
        leds_[led].active = false;
        leds_[led].current_pwm = 0;
    }

    // Run at 30Hz
    void update() {
        for (auto& led : leds_) {
            if (!led.active || led.pattern == nullptr) continue;

            led.current_pwm = led.pattern->at(led.current_index);

            led.current_index++;

            if (led.current_index >= led.pattern->size()) {
                led.current_index = 0;
            }
        }
    }

    uint16_t getBrightness(uint8_t led) const {
        if (led >= MAX_LEDS) return 0;
        return leds_[led].current_pwm;
    }

private:
    std::array<LedState, MAX_LEDS> leds_;
};