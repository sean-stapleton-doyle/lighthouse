#pragma once

#include <array>
#include <cstdint>
#include <span>

class FadeController {
public:
    static constexpr uint8_t MAX_LEDS = 15;
    
    struct LedState {
        std::span<const uint16_t> pattern = {}; 
        size_t current_index = 0;
        bool active = false;
        uint16_t current_pwm = 0;
    };
    
    FadeController() = default;
    
    void assignPattern(uint8_t led, std::span<const uint16_t> new_pattern, float offset_percent = 0.0f);
    
    void stop(uint8_t led);

    void update();

    uint16_t getBrightness(uint8_t led) const;

private:
    std::array<LedState, MAX_LEDS> leds_;
};