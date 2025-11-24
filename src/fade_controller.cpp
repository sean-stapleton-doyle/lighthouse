#include "fade_controller.hpp"

void FadeController::assignPattern(uint8_t led, std::span<const uint16_t> new_pattern, float offset_percent) {
    if (led >= MAX_LEDS) return;
    if (new_pattern.empty()) return;

    leds_[led].pattern = new_pattern;
    leds_[led].active = true;
    
    size_t start_idx = static_cast<size_t>(offset_percent * new_pattern.size());
    leds_[led].current_index = start_idx % new_pattern.size();
}

void FadeController::stop(uint8_t led) {
    if (led >= MAX_LEDS) return;
    leds_[led].active = false;
    leds_[led].current_pwm = 0;
}

void FadeController::update() {
    for (auto& led : leds_) {
        if (!led.active || led.pattern.empty()) continue;

        led.current_pwm = led.pattern[led.current_index];

        led.current_index++;

        if (led.current_index >= led.pattern.size()) {
            led.current_index = 0;
        }
    }
}

uint16_t FadeController::getBrightness(uint8_t led) const {
    if (led >= MAX_LEDS) return 0;
    return leds_[led].current_pwm;
}