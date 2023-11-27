#pragma once

class PID{
    double gains[4];
    double limit[2];
    double setpoint;
    double command;

    double error;
    double lastError;
    double cumulativeError;
    double derivativeError;
    double saturationError;

public:
    // Update setpoint
    void set(double setpoint);

    // Update errors and the resulting command
    void update(double state, double dt);

    // Return command
    double getCommand();

    // Initialize a PID controller with the given gains
    PID(double p, double i, double d, double s, double min, double max);
};