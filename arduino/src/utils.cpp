#include "utils.h"

float clamp(float val, float min, float max) {
    if (val < min) {
        return min;
    }
    else if (val > max) {
        return max;
    }
    
    return val;
}