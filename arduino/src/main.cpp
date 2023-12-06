#include <Arduino.h>
#include <SPI.h>

#include "encoder.h"
#include "pid.h"
#include "utils.h"
#include "comm.h"

Encoder encoder1(A1, 0.9, 1);
Encoder encoder2(A3, 0.9, 1);
Encoder encoder3(A2, 0.9, 1);

// MOTOR GAINS
// 1: 
// 2: 1.4, 0.00001, 0.1 (no switch)
// 3: 1.4, 0.00001, 0.1 (motor switch)

// wheel 1 encoder sometimes scaled by -4

PID controller1(1.4, 0.000003, 0.1, -685, 685);
PID controller2(1.4, 0.000003, 0.1, -685, 685);
PID controller3(1.4, 0.000003, 0.1, -685, 685); // motor 3 forward
// PID controller(2, 0.000003, 0, -685, 685); // motor 3 backward

float globalVelo[3] = {0.0, 0.0, 100.0};
float* wheelVelos;
float* data;

unsigned long last_micros;

unsigned int pwm1 = 3;
unsigned int dirFwd1 = 2;
unsigned int dirBkwd1 = 4;

unsigned int pwm2 = 6;
unsigned int dirFwd2 = 5;
unsigned int dirBkwd2 = 7;

unsigned int pwm3 = 9;
unsigned int dirFwd3 = 8;
unsigned int dirBkwd3 = 10;

double setMotor(int motor, double command) {
    unsigned int pwm, dirFwd, dirBkwd;

    switch(motor) {
        case 1:
            pwm = pwm1;
            dirFwd = dirFwd1;
            dirBkwd = dirBkwd1;
            break;
        case 2:
            pwm = pwm2;
            dirFwd = dirFwd2;
            dirBkwd = dirBkwd2;
            break;
        case 3:
            pwm = pwm3;
            dirFwd = dirFwd3;
            dirBkwd = dirBkwd3;
            break;
    }

    if (command > 0) {
        digitalWrite(dirFwd, HIGH);
        digitalWrite(dirBkwd, LOW);
    } else {
        digitalWrite(dirFwd, LOW);
        digitalWrite(dirBkwd, HIGH);
    }
    unsigned int out = clamp(abs(command) / 685 * 255, 0, 255);
    analogWrite(pwm, out);
    return out;
}

void senseMotors() {
    setMotor(1, 100);
    setMotor(2, 100);
    setMotor(3, 100);

    long start_micros = micros();

    while (micros() - start_micros < 500000) {
        encoder1.update();
        encoder2.update();
        encoder3.update();
    }

    if (encoder1.velocity() < 0) {
        encoder1.setScale(-0.25);
    }

    digitalWrite(dirFwd1, LOW);
    digitalWrite(dirBkwd1, LOW);
    digitalWrite(dirFwd2, LOW);
    digitalWrite(dirBkwd2, LOW);
    digitalWrite(dirFwd3, LOW);
    digitalWrite(dirBkwd3, LOW);
}

void setup() {
    Serial.begin(9600);
    SPI.begin();
    encoder1.begin();
    encoder2.begin();
    encoder3.begin();
    pinMode(pwm1, OUTPUT);
    pinMode(dirFwd1, OUTPUT);
    pinMode(dirBkwd1, OUTPUT);
    pinMode(pwm2, OUTPUT);
    pinMode(dirFwd2, OUTPUT);
    pinMode(dirBkwd2, OUTPUT);
    pinMode(pwm3, OUTPUT);
    pinMode(dirFwd3, OUTPUT);
    pinMode(dirBkwd3, OUTPUT);

    senseMotors();

    delay(2000);

    encoder1.clear();
    encoder2.clear();
    encoder3.clear();

    controller1.clear();
    controller2.clear();
    controller3.clear();

    wheelVelos = getWheelVelos(globalVelo, .397);
    // Serial.print(wheelVelos[0]);
    // Serial.print(", ");
    // Serial.print(wheelVelos[1]);
    // Serial.print(", ");
    // Serial.println(wheelVelos[2]);

    // controller1.set(wheelVelos[0]);
    // controller2.set(wheelVelos[1]);
    // controller3.set(wheelVelos[2]);

    controller1.set(0);
    controller2.set(0);
    controller3.set(0);
}

void loop() {
    unsigned long this_micros = micros();
    // Serial.print(encoder1.velocity());
    // Serial.print(", ");
    // Serial.print(encoder2.velocity());
    // Serial.print(", ");
    // Serial.println(encoder3.velocity());

    setMotor(1, controller1.getCommand());
    setMotor(2, controller2.getCommand());
    setMotor(3, controller3.getCommand());

    encoder1.update();
    encoder2.update();
    encoder3.update();

    controller1.update(encoder1.velocity(), this_micros - last_micros);
    controller2.update(encoder2.velocity(), this_micros - last_micros);
    controller3.update(encoder3.velocity(), this_micros - last_micros);

    last_micros = this_micros;

    free(data);
    data = readData();

    // Serial.print(data[0]);
    // Serial.print(", ");
    // Serial.print(data[1]);
    // Serial.print(", ");
    // Serial.print(data[2]);
    // Serial.print(", ");
    // Serial.println(data[3]);
}