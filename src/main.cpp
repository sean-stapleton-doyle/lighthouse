#include "tlc5947.hpp"
#include "fade_controller.hpp"
#include "circular_pattern.hpp"

extern "C" {
    #include "pico/stdlib.h"
    #include "pico/time.h"
}

constexpr PinConfig TLC_PINS = {
    .clk = 24, .data = 25, .latch = 27, .blank = 26
};

TLC5947 led_driver(TLC_PINS);

FadeController fade_controller;

CircularPattern pattern_2s;
CircularPattern pattern_3s;
CircularPattern pattern_5s;
CircularPattern pattern_heartbeat; 

const int NUM_LIGHTHOUSES = 15;

bool update_callback(struct repeating_timer *t) {
    fade_controller.update();
    
    for (uint8_t i = 0; i < NUM_LIGHTHOUSES; i++) {
        led_driver.setPWM(i, fade_controller.getBrightness(i));
    }
    
    led_driver.write();
    return true;
}

int main() {
    stdio_init_all();
    sleep_ms(2000);

    led_driver.begin();

    pattern_2s = CircularPattern::createSine(60);   // 2s @ 30Hz
    pattern_3s = CircularPattern::createSine(90);   // 3s @ 30Hz
    pattern_5s = CircularPattern::createSine(150);  // 5s @ 30Hz
    
    
    fade_controller.assignPattern(0, pattern_2s, 0.0f);
    
    fade_controller.assignPattern(1, pattern_5s, 0.0f);
    
    fade_controller.assignPattern(2, pattern_2s, 0.5f); 

    fade_controller.assignPattern(3, pattern_3s, 0.0f);

    struct repeating_timer timer;
    add_repeating_timer_ms(-33, update_callback, NULL, &timer);
    
    while (true) {
        tight_loop_contents();
    }
    return 0;
}