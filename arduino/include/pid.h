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

    // Update errors and the  resulting command
    void update(float value, float dt);

    // Return command
    float getCommand();

    // Create a PID controller with the given setpoint and gains
    PID(float setpoint, float p, float i, float d, float s);
};