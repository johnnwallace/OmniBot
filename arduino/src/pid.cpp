#include "utils.h"
#include "pid.h"

void PID::update(float value, float dt) {
    this->lastError = error;
    this->error = value - setpoint;
    this->cumulativeError += value * dt;
    this->derivativeError = (error - lastError)/dt;

    float unclamped = this->gains[0] * this->error
                    + this->gains[1] * this->cumulativeError
                    + this->gains[2] * this->derivativeError
                    + this->gains[3] * this->saturationError;

    this->command = clamp(unclamped, this->limit[0], this->limit[1]);
    this->saturationError = this->command - unclamped;
}

float PID::getCommand() {
    return this->command;
}

PID::PID(float setpoint, float p, float i, float d, float s) {
    this->setpoint = setpoint;
    this->gains[0] = p;
    this->gains[1] = i;
    this->gains[2] = d;
    this->gains[3] = s;

    this->error = 0;
    this->lastError = 0;
    this->cumulativeError = 0;
    this->derivativeError = 0;
    this->saturationError = 0;
}