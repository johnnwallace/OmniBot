#include "utils.h"
#include <Arduino.h>

float clamp(float val, float min, float max) {
    if (val < min) {
        return min;
    }
    else if (val > max) {
        return max;
    }
    
    return val;
}

float* getWheelVelos(float worldVelo[3], float wheelbase) {
    float omega_term = wheelbase*worldVelo[3];
    float* wheel_velos = new float[3];
    wheel_velos[0] = -worldVelo[0] + omega_term;
    wheel_velos[1] = sin(PI/6)*worldVelo[0] - cos(PI/6)*worldVelo[1] + omega_term;
    wheel_velos[2] = sin(PI/6)*worldVelo[0] + cos(PI/6)*worldVelo[1] + omega_term;
    return wheel_velos;
}