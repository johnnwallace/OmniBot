#include "comm.h"

#include <Arduino.h>

float* readData() {
    static float data[4];
    size_t size = Serial.readBytesUntil('\n', buf, BUF_LENGTH);
    buf[size] = 0;

    char* command = strtok(buf, " ");
    if (command != 0) {
        data[0] = (float)atoi(command);
        command = strtok(0, " ");

        data[1] = (float)atoi(command);
        command = strtok(0, " ");

        data[2] = atof(command);
        command = strtok(0, " ");

        data[3] = atof(command);
    }

    return data;
}