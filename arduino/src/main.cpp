#include <Arduino.h>
#include <SPI.h>

#include "encoder.h"
#include "pid.h"
#include "utils.h"

Encoder encoder1;
Encoder encoder2;
Encoder encoder3;

void setup() {
    Serial.begin(9600);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    SPI.begin();

    encoder1 = Encoder(A1);
    encoder2 = Encoder(A2);
    encoder3 = Encoder(A3);
}

void loop() {
    Serial.print(encoder1.velocity());
    Serial.print(", ");
    Serial.print(encoder2.velocity());
    Serial.print(", ");
    Serial.println(encoder3.velocity());

    encoder1.update();
    encoder2.update();
    encoder3.update();
}