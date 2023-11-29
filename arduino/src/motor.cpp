#include "motor.h"

#include <SPI.h>
#include "pid.h"
#include "encoder.h"
#include "utils.h"

void Motor::update(float dt) {
    // update encoder and controller
    encoder.update();
    controller.update(encoder.velocity(), dt);

    int command = controller.getCommand();
    if (command > 0) {
        digitalWrite(dirFwd, HIGH);
        digitalWrite(dirBkwd, LOW);
    } else {
        digitalWrite(dirFwd, LOW);
        digitalWrite(dirBkwd, HIGH);
    }
    unsigned int out = clamp(abs(command) / lim * 255, 0, 255);
    analogWrite(pwm, out);
}

void Motor::begin() {
    SPI.begin();
    encoder.begin();

    pinMode(pwm, OUTPUT);
    pinMode(dirFwd, OUTPUT);
    pinMode(dirBkwd, OUTPUT);

    controller.set(0);
}

Motor::Motor(float gains[3], unsigned int lim, unsigned int pwm, unsigned int dirFwd, unsigned int dirBkwd, unsigned int encoderCS, double alpha, double scale) {
    this->lim = lim;
    this->pwm = pwm;
    this->dirFwd = dirFwd;
    this->dirBkwd = dirBkwd;

    this->controller = PID(gains[0], gains[1], gains[2], -lim, lim);
    this->encoder = Encoder(encoderCS, alpha, scale);
}