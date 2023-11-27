#include "utils.h"
#include "pid.h"

void PID::set(double setpoint) {
    this->setpoint = setpoint;
}

void PID::update(double state, double dt) {
    this->lastError = error;
    this->error = state - setpoint;
    this->cumulativeError += state * dt;
    this->derivativeError = (error - lastError)/dt;

    double unclamped = this->gains[0] * this->error
                    + this->gains[1] * this->cumulativeError
                    + this->gains[2] * this->derivativeError
                    + this->gains[3] * this->saturationError;

    this->command = clamp(unclamped, this->limit[0], this->limit[1]);
    this->saturationError = this->command - unclamped;
}

double PID::getCommand() {
    return this->command;
}

PID::PID(double p, double i, double d, double s, double min, double max) {
    this->limit[0] = min;
    this->limit[1] = max;
    this->gains[0] = p;
    this->gains[1] = i;
    this->gains[2] = d;
    this->gains[3] = s;

    this->setpoint = 0;
    this->error = 0;
    this->lastError = 0;
    this->cumulativeError = 0;
    this->derivativeError = 0;
    this->saturationError = 0;
}