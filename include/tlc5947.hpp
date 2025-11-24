#pragma once

#include "pico/stdlib.h"
#include <array>

struct PinConfig {
    uint clk;
    uint data;
    uint latch;
    uint blank;
};

class TLC5947 {
public:
    static const int NUM_CHANNELS = 24;
    static const int MAX_PWM = 4095;

    explicit TLC5947(const PinConfig& pins);

    void begin();
    void setPWM(uint8_t channel, uint16_t value);
    uint16_t getPWM(uint8_t channel) const;
    void setAllPWM(uint16_t value);
    void write();
    void enable();
    void disable();

private:
    PinConfig pins_;
    std::array<uint16_t, NUM_CHANNELS> pwm_buffer_;

    void initPins();
    void writeBit(bool bit);
    void latchData();
};

