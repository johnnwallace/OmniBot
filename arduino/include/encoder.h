#pragma once

#include <Arduino.h>

#define ENCODER_GROUND_DIST 1

class Encoder{
    // chip select pin
    int cs;

    // microseconds at last read
    unsigned long last_micros;

    // encoder counts at last read
    unsigned long last_counts;

    // current wheel velocity
    float velo;

public:
    // returns wheel velocity
    int velocity() { return velo; };

    // reads encoder count
    unsigned long read();

    // updates encoder states
    void update();

    // clears encoder states
    void clear();

    Encoder() {};
    Encoder(int cs);
};