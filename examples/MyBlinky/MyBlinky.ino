#include <Clock.hpp>
#include <Led.hpp>

bool ledState = LOW;
long ledDelay = 200;

MainSchedule schedule;
Clock clock(schedule, ledDelay, ledDelay, ledState);
DigitalLED led(schedule, ledState, LED_BUILTIN);

void setup() {
    schedule.begin();
}

void loop() {
    schedule.poll();
}
