/*
MIT License

Copyright (c) 2022 jffordem

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CLOCK_H
#define CLOCK_H

#include <Scheduler.hpp>

/*
Clock provides a squarewave signal with controllable high and low times.
It can also be disabled by calling Clock.disable() and Clock.restart().
Example:

// Blinky
MainSchedule schedule;
const int ledPin = 17;
long lowTime = 100;
long highTime = 200;
long signal;
Clock clk(schedule, lowTime, highTime, signal);
DigitalLED led(schedule, signal, ledPin);
void setup() { schedule.begin(); }
void loop() { schedule.poll(); }
*/

class Expires {
public:
	virtual bool expired() = 0;
	virtual void reset(long time) = 0;
};

/* Pro tip: you can 'turn off' a timer by setting the delay to MAX_LONG. */
class Timer : public Expires {
	long _time;
	long _lastExpired;
public:
	Timer(long time = 0) : _time(max(0, time)) {
		_lastExpired = millis();
	}
	bool expired() {
		return millis() - _lastExpired > _time;
	}
	void reset(long time) {
		_time = max(0, time);
		_lastExpired = millis();
	}
};

class ExpiresComposite : public Composite<Expires> {
	const bool _any; // true for any(expired), false for all(expired
public:
	ExpiresComposite(bool any = true, Expires *itemsZ[] = NULL) :
		Composite<Expires>(itemsZ, countZ(itemsZ)), _any(any) { }
	bool expired() {
		for (int i = 0; i < length(); i++) {
			if (item(i)->expired() && _any) {
				return true;
			}
			if (!item(i)->expired() && !_any) {
				return false;
			}
		}
		return !_any;
	}
};

class Clock : private Scheduled, private Timer, public Enabled {
	long &_lowTime;
	long &_highTime;
	bool &_value;
	bool _enabled = true;
public:
	Clock(Schedule &schedule, long &lowTime, long &highTime, bool &value) :
		Scheduled(schedule), Timer(lowTime), _lowTime(lowTime), _highTime(highTime), _value(value) { }
	void enable(bool value) {
		if (_enabled != value) {
			_enabled = value;
			_value = LOW;
			reset(0);
		}
	}
	void toggle() {
		enable(!_enabled);
	}
	void poll() {
		if (expired() && _enabled) {
			if (_value) {
				_value = LOW;
				reset(_lowTime);
			} else {
				_value = HIGH;
				reset(_highTime);
			}
		}
	}
};

class SpeedTest : public Scheduled {
	const static int length = 10;
	long _times[length];
	short _data[length];
	Timer _countdown;
	short _count;
	long _last;
public:
	SpeedTest(Schedule &schedule, long initialDelay) : Scheduled(schedule), _countdown(initialDelay), _count(-1), _last(-1) { }
	SpeedTest(Schedule &schedule) : SpeedTest(schedule, 2500) { }
	void poll() {
		if (_countdown.expired() && _count < length) {
			long now = millis();
			if (now == _last) {
				_data[_count]++;
			} else {
				_count++;
				if (_count < length) {
					_times[_count] = now;
					_data[_count] = 1;
				}
			}
			_last = now;
		}
		if (_count == length) {
			_count = length+1;
			Serial.println("Speed Test:");
			for (int i = 0; i < length; i++) {
				Serial.print(_times[i]);
				Serial.print(": ");
				Serial.println(_data[i]);
			}
		}
	}
};

#endif