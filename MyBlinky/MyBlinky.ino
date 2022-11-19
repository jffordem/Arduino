#include "timer.hpp"

const uint8_t ledPin = LED_BUILTIN;
uint8_t ledState = LOW;
const long ledDelay = 200;
Timer ledTimer;

void setup() {
    pinMode(ledPin, OUTPUT);
    ledTimer.reset(ledDelay);
    digitalWrite(ledPin, ledState);
}

void loop() {
    if (ledTimer.expired()) {
        ledTimer.reset(ledDelay);
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
    }
}

