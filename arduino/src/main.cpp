#include <Arduino.h>
#include <SPI.h>

#include "encoder.h"
#include "pid.h"
#include "utils.h"

Encoder encoder1(A1, 0.9, 1);
Encoder encoder2(A2, 0.9, -1.0/3); // this encoder seems to count 3 times as fast and in the opposite direction
Encoder encoder3(A3, 0.9, 1);

PID controller(6, 0, 0, 0, -10000, 10000);

unsigned long last_micros;

unsigned int pwm = 9;
unsigned int dirFwd = 8;
unsigned int dirBkwd = 10;

void setMotor(double command) {
    if (command > 0) {
        digitalWrite(dirFwd, LOW);
        digitalWrite(dirBkwd, HIGH);
    } else {
        digitalWrite(dirFwd, HIGH);
        digitalWrite(dirBkwd, LOW);
    }
    analogWrite(pwm, abs(command));
}

void setup() {
    Serial.begin(9600);
    SPI.begin();
    encoder1.begin();
    encoder2.begin();
    encoder3.begin();
    pinMode(pwm, OUTPUT);
    pinMode(dirFwd, OUTPUT);
    pinMode(dirBkwd, OUTPUT);

    controller.set(2300);
}

void loop() {
    unsigned long this_micros = micros();

    // Serial.print(encoder1.velocity());
    // Serial.print(", ");
    // Serial.print(encoder2.velocity());
    // Serial.print(", ");
    // Serial.println(encoder3.velocity());

    Serial.print(encoder3.velocity());
    Serial.print(", ");
    Serial.println(controller.getCommand());

    setMotor(controller.getCommand() / 23.0); // need to scale encoder to pwm

    encoder1.update();
    encoder2.update();
    encoder3.update();
    controller.update(encoder3.velocity(), this_micros - last_micros);
    last_micros = this_micros;
}