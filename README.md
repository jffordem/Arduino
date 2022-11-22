# Arduino
Projects and libraries for Arduino project development.

## License
MIT License (See LICENSE file for details)

## Overview
Re-working my 'scheduler' concepts.

This version started with the attempt to use an encoder wheel to control repeated casting in Skyrim.

Need a better convention for things that observe pins (like an encoder wheel, or button), and things that
drive real-world objects like LEDs, keyboard/mouse actions, servos and steppers.

In between are elements like clocks and mappers, inverters, handlers, starters-and-stoppers that allow
the system to behave in useful ways.

These objects should be easy to compose and repeat black-box abstractions of increasingly complex units.

One significiant improvement in this version was to have the scheduler passed in to any object that needs
to be polled.  This reduces the risk that you'd forget to add it, because it isn't clear which things are
polled and which work independently.

## Modules
Scheduler.hpp
* Pressable
* Poller
* Enabled
* Composite
* PressComposite
* EnabledComposite
* PollerComposite
* MainSchedule
* Scheduled
* PollGroup

Clock.hpp : Scheduler.hpp
* Timer
* Clock
* SpeedTest

PinIO.hpp : Scheduler.hpp
* DigitalRead
* DigitalWrite
* AnalogRead
* AnalogWrite

EdgeDetector.hpp : Scheduler.hpp
* EdgeDetectorBase
* EdgeDecector
* Counter

HIDIO.hpp : Scheduler.hpp, Clock.hpp
* ValuePresser
* PressHandler
* KeyPress
* MouseButton
* DummyButton
* ButtonController

Mapper.hpp : Scheduler.hpp
* Mapper
* Inverter
* Constrain

Led.hpp : Scheduler.hpp, PinIO.hpp, Mapper.hpp
* DigitalLED
* SevenSegLED
* Pot

ButtonHandler.hpp : Scheduler.hpp, EdgeDetector.hpp, HIDIO.hpp
* ButtonHandler
* Button
* ToggleButton

EncoderWheel.hpp : Scheduler.hpp, PinIO.hpp, EdgeDetector.hpp
* EncoderWheel

## Examples
Composition with objects - including inheritance - can be seen in things like the ButtonHandler and 
the ClockToggleButton.  For example, here's a composition for a "Blinky".

```C++
class Blinky : public Clock, private DigitalLED {
  bool _ledState;
public:
  Blinky(MainSchedule &schedule, long &offTime, long &onTime, int ledPin) :
    Clock(schedule, offTime, onTime, offset, _ledState),
    DigitalLED(schedule, _ledState, ledPin) { }
};
const int ledPin = 17;
long onTime = 100;
long offTime = 200;
MainSchedule schedule;
Blinky blinky(schedule, offTime, onTime, 0, ledPin);
void setup() { schedule.begin(); }
void loop() { schedule.poll(); }
```
