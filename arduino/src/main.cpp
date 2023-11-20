#include <Arduino.h>
#include <SPI.h>

#include "encoder.h"
#include "pid.h"
#include "utils.h"

Encoder encoder1(A1);
Encoder encoder2(A2);
Encoder encoder3(A3);

void setup() {
    Serial.begin(9600);
    SPI.begin();
    encoder1.begin();
    encoder2.begin();
    encoder3.begin();
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