#include "comm.h"

#include <Arduino.h>

float* readData() {
    if (Serial.available() > 0) {
        size_t size = Serial.readBytesUntil('\n', buf, BUF_LENGTH);
        buf[size] = 0;

        char* command = strtok(buf, " ");
        if (command != 0) {
            int cx = atoi(command);
            command = strtok(0, " ");

            int cy = atoi(command);
            command = strtok(0, " ");

            float depth = atof(command);
            command = strtok(0, " ");

            float angle = atof(command);

            Serial.print(cx);
            Serial.print(", ");
            Serial.print(cy);
            Serial.print(", ");
            Serial.print(depth);
            Serial.print(", ");
            Serial.println(angle);

            return new float[4]{(float)cx, (float)cy, depth, angle};
        }
    }
}