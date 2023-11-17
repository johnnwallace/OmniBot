#pragma once

class PID{
    float gains[4];
    float limit[2];
    float setpoint;
    float command;

    float error;
    float lastError;
    float cumulativeError;
    float derivativeError;
    float saturationError;

public:
    // Update setpoint
    void set(float setpoint);

    // Update errors and the resulting command
    void update(float state, float dt);

    // Return command
    float getCommand();

    // Initialize a PID controller with the given gains
    PID(float p, float i, float d, float s);
};