#include "tlc5947.hpp"
#include "fade_controller.hpp"
#include "waveforms.hpp"

extern "C" {
    #include "pico/stdlib.h"
    #include "pico/time.h"
}

static constexpr auto FADE_2S = make_sine_pattern<60>();
static constexpr auto FADE_3S = make_sine_pattern<90>();
static constexpr auto FADE_5S = make_sine_pattern<150>();

constexpr PinConfig TLC_PINS = {
    .clk = 24, 
    .data = 25, 
    .latch = 27, 
    .blank = 26
};

TLC5947 led_driver(TLC_PINS);
FadeController fade_controller;

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
    
    fade_controller.assignPattern(0, FADE_2S);
    fade_controller.assignPattern(1, FADE_5S);
    fade_controller.assignPattern(2, FADE_2S, 0.5f); 
    fade_controller.assignPattern(3, FADE_3S);

    struct repeating_timer timer;
    add_repeating_timer_ms(-33, update_callback, NULL, &timer);
    
    while (true) {
        tight_loop_contents();
    }
    return 0;
}