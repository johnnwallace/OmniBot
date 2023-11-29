#pragma once

#include "pid.h"
#include "encoder.h"

class Motor{
    PID controller;
    Encoder encoder;

    unsigned int pwm;
    unsigned int dirFwd;
    unsigned int dirBkwd;

    unsigned int lim;

public:
    // return current setpoint
    int getSetpoint() { return controller.getSetpoint();};

    // return current command
    int getCommand() { return controller.getCommand(); };

    // return current speed
    int getSpeed() { return encoder.velocity(); };

    // update errors and the resulting command
    void update(float dt);
    
    // update setpoint
    void set(int setpoint) { controller.set(setpoint); };

    // initialize motor
    void begin();

    // Initialize a motor
    Motor(float gains[3], unsigned int lim, unsigned int pwm, unsigned int dirFwd, unsigned int dirBkwd, unsigned int encoderCS, double alpha, double scale);
};