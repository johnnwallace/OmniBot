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
    
    velo = ENCODER_GROUND_DIST * (this_counts - last_counts) / (this_micros - last_micros) / 1000;

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

Encoder::Encoder(int cs) {
    this->cs = cs;
    this->last_counts = 0;
    this->last_micros = micros();

    digitalWrite(cs, LOW);
    SPI.transfer(0x88);
    SPI.transfer(0x03);
    digitalWrite(cs, HIGH);
}