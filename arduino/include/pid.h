#pragma once

class PID{
    float gains[3];
    float limit[2];
    float setpoint;
    int command;

    float error;
    float lastError;
    float cumulativeError;
    float derivativeError;
    bool clamped;

public:
    // Clears history
    void clear();

    // Update setpoint
    void set(float setpoint);

    // Return current setpoint
    float getSetpoint();

    // Update errors and the resulting command
    void update(float state, float dt);

    // Return command
    int getCommand();

    PID() {};
    PID(float p, float i, float d, float min, float max);
};