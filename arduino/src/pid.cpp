#include "utils.h"
#include "pid.h"

void PID::set(double setpoint) {
    this->setpoint = setpoint;
}

double PID::getSetpoint() {
    return this->setpoint;
}

void PID::update(double state, double dt) {
    this->lastError = error;
    this->error = setpoint - state;
    this->cumulativeError += error * dt * (error / setpoint < 0.3);
    this->derivativeError = (error - lastError)/dt;

    double unclamped = this->gains[0] * this->error
                    + this->gains[1] * this->cumulativeError
                    + this->gains[2] * this->derivativeError;

    if (this->clamped) {
        this->command = clamp(unclamped, this->limit[0], this->limit[1]);
    } else {
        this->command = unclamped;
    }
}

double PID::getCommand() {
    return this->command;
}

PID::PID(double p, double i, double d, double min, double max) {
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
    this->saturationError = 0;
}

PID::PID(double p, double i, double d) {
    this->gains[0] = p;
    this->gains[1] = i;
    this->gains[2] = d;
    this->gains[3] = 0;
    this->clamped = false;

    this->setpoint = 0;
    this->error = 0;
    this->lastError = 0;
    this->cumulativeError = 0;
    this->derivativeError = 0;
    this->saturationError = 0;
}