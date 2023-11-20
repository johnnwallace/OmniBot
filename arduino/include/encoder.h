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
    double velo;
    
    // scale velocity by this value
    double scale;

    // low-pass dropoff
    double alpha;
public:
    // returns wheel velocity
    double velocity() { return velo; };

    // reads encoder count
    unsigned long read();

    // initializes encoder
    void begin();

    // updates encoder states
    void update();

    // clears encoder states
    void clear();

    Encoder() {};
    Encoder(int cs, double alpha, double scale);
};