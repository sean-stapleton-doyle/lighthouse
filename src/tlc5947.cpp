#include "tlc5947.hpp"

#include "pico/stdlib.h"

TLC5947::TLC5947(const PinConfig& pins)
    : pins_(pins), pwm_buffer_{0} {}

void TLC5947::begin() {
    initPins();
    gpio_put(pins_.blank, 1);
    setAllPWM(0);
    write();
    enable();
}

void TLC5947::initPins() {
    gpio_init(pins_.clk);
    gpio_set_dir(pins_.clk, GPIO_OUT);
    gpio_put(pins_.clk, 0);

    gpio_init(pins_.data);
    gpio_set_dir(pins_.data, GPIO_OUT);
    gpio_put(pins_.data, 0);

    gpio_init(pins_.latch);
    gpio_set_dir(pins_.latch, GPIO_OUT);
    gpio_put(pins_.latch, 0);

    gpio_init(pins_.blank);
    gpio_set_dir(pins_.blank, GPIO_OUT);
    gpio_put(pins_.blank, 1);
}

void TLC5947::setPWM(uint8_t channel, uint16_t value) {
    if (channel >= NUM_CHANNELS) return;
    pwm_buffer_[channel] = value & MAX_PWM;
}

uint16_t TLC5947::getPWM(uint8_t channel) const {
    if (channel >= NUM_CHANNELS) return 0;
    return pwm_buffer_[channel];
}

void TLC5947::setAllPWM(uint16_t value) {
    value &= MAX_PWM;
    for (auto& pwm : pwm_buffer_) {
        pwm = value;
    }
}

void TLC5947::writeBit(bool bit) {
    gpio_put(pins_.clk, 0);
    gpio_put(pins_.data, bit ? 1 : 0);
    gpio_put(pins_.clk, 1);
}

void TLC5947::write() {
    gpio_put(pins_.latch, 0);

    for (int ch = NUM_CHANNELS - 1; ch >= 0; ch--) {
        uint16_t value = pwm_buffer_[ch];

        for (int bit = 11; bit >= 0; bit--) {
            writeBit((value >> bit) & 1);
        }
    }

    gpio_put(pins_.clk, 0);
    latchData();
}

void TLC5947::latchData() {
    gpio_put(pins_.latch, 0);
    sleep_us(1);
    gpio_put(pins_.latch, 1);
    sleep_us(1);
    gpio_put(pins_.latch, 0);
}

void TLC5947::enable() {
    gpio_put(pins_.blank, 0);
}

void TLC5947::disable() {
    gpio_put(pins_.blank, 1);
}
