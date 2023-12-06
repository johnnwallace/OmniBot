#include <Arduino.h>

if Serial.available() > 0 {
    String data = Serial.readStringUntil('\n');
}