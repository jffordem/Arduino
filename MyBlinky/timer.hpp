#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
    long _last;
    long _time;
public:
    Timer(long time = 0) {
        reset(time);
    }
    bool expired() const {
        return millis() - _last > _time;
    }
    void reset(long time) {
        _time = max(0, time);
        _last = millis();
    }
};

#endif
