#pragma once

class PID{
    double gains[3];
    double limit[2];
    double setpoint;
    double command;

    double error;
    double lastError;
    double cumulativeError;
    double derivativeError;
    double saturationError;
    bool clamped;

public:
    // Update setpoint
    void set(double setpoint);

    // Return current setpoint
    double getSetpoint();

    // Update errors and the resulting command
    void update(double state, double dt);

    // Return command
    double getCommand();

    // Initialize a PID controller with the given gains and limits
    PID(double p, double i, double d, double min, double max);

    // Initialize a PID controller with the given gains and limits
    PID(double p, double i, double d);
};