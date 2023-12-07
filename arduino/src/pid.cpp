#include "pid.h"

#include "utils.h"
#include <Arduino.h>

void PID::clear() {
    lastError = 0;
    error = 0;
    cumulativeError = 0;
    derivativeError = 0;
}

void PID::set(float setpoint) {
    this->setpoint = setpoint;
}

float PID::getSetpoint() {
    return this->setpoint;
}

void PID::update(float state, float dt) {
    this->lastError = error;
    this->error = setpoint - state;
    // this->cumulativeError += error * dt * (error / setpoint < 0.3);
    this->cumulativeError += error * dt / 1000000;
    this->derivativeError = (error - lastError)/dt * 1000000;

    // Serial.print(state);
    // Serial.print(", ");
    // Serial.print(error);
    // Serial.print(", ");
    // Serial.print(", ");
    // Serial.print(cumulativeError);
    // Serial.print(", ");
    // Serial.println(derivativeError);

    float unclamped = this->gains[0] * this->error
                    + this->gains[1] * this->cumulativeError
                    + this->gains[2] * this->derivativeError;

    // Serial.println(unclamped);

    if (this->clamped) {
        float clamped = clamp(unclamped, this->limit[0], this->limit[1]);
        this->command = clamped;
    } else {
        this->command = unclamped;
    }
}

int PID::getCommand() {
    return this->command;
}

PID::PID(float p, float i, float d, float min, float max) {
    this->limit[0] = min;
    this->limit[1] = max;
    this->gains[0] = p;
    this->gains[1] = i;
    this->gains[2] = d;
    this->clamped = true;

    this->setpoint = 0;
    this->error = 0;
    this->lastError = 0;
    this->cumulativeError = 0;
    this->derivativeError = 0;
}