// https://github.com/SuperDroidRobots/Encoder-Buffer-Breakout/blob/master/DualEncoderBreakout/DualEncoderBreakout.ino

#include "encoder.h"

#include <Arduino.h>
#include <SPI.h>

unsigned long Encoder::read() {
    digitalWrite(cs, LOW);

    // request count
    SPI.transfer(0x60);

    // read data
    uint8_t count_1 = SPI.transfer(0x00);
    uint8_t count_2 = SPI.transfer(0x00);
    uint8_t count_3 = SPI.transfer(0x00);
    uint8_t count_4 = SPI.transfer(0x00);
    digitalWrite(cs, HIGH);

    unsigned long count = (count_1 << 8) + count_2;
    count = (count << 8) + count_3;
    return (count << 8) + count_4;
}

void Encoder::update() {
    unsigned long this_micros = micros();
    unsigned long this_counts = read();
    
    // calculate new velocity
    double new_velo = (double)((long)this_counts - (long)last_counts) / ((long)this_micros - (long)last_micros) * 1000000 * scale;
    
    // update filter
    velo = (1 - alpha) * new_velo + alpha * velo;

    last_micros = this_micros;
    last_counts = this_counts;
}

void Encoder::clear() {
    digitalWrite(cs,LOW);

    // Write to DTR
    SPI.transfer(0x98); 

    // Load data
    SPI.transfer(0x00);
    SPI.transfer(0x00);           
    SPI.transfer(0x00);           
    SPI.transfer(0x00);
    digitalWrite(cs,HIGH);

    last_counts = 0;
    last_micros = micros();
}

Encoder::Encoder(int cs, double alpha, double scale) {
    this->cs = cs;
    this->alpha = alpha;
    this->scale = scale;
}

void Encoder::begin() {
    // initialize pin
    pinMode(cs, OUTPUT);

    // initialize SPI device
    digitalWrite(cs, LOW);
    SPI.transfer(0x88);
    SPI.transfer(0x03);
    digitalWrite(cs, HIGH);

    // initialize states
    this->last_counts = read();
    this->last_micros = micros();
}