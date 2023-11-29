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
};