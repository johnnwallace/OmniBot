#pragma once

class PID{
    float gains[3];
    int limit[2];
    int setpoint;
    int command;

    float error;
    float lastError;
    float cumulativeError;
    float derivativeError;
    bool clamped;

public:
    // Update setpoint
    void set(int setpoint);

    // Return current setpoint
    int getSetpoint();

    // Update errors and the resulting command
    void update(float state, float dt);

    // Return command
    int getCommand();

    // Initialize a PID controller with the given gains and limits
    PID(float p, float i, float d, float min, float max);

    // Initialize a PID controller with the given gains and limits
    PID(float p, float i, float d);
};