#pragma once

#include <Arduino.h>

class Encoder{
    // chip select pin
    int cs;

    // microseconds at last read
    unsigned long last_micros;

    // encoder counts at last read
    unsigned long last_counts;

    // current wheel velocity
    float velo;
    
    // scale velocity by this value
    float scale;

    // low-pass dropoff
    float alpha;
public:
    // returns wheel velocity
    float velocity() { return velo; };

    // reads encoder count
    unsigned long read();

    // initializes encoder
    void begin();

    // updates encoder states
    void update();

    // clears encoder states
    void clear();

    // sets the scale of the encoder
    void setScale(float scale) { this->scale = scale; };

    Encoder() {};
    Encoder(int cs, float alpha, int scale);
};