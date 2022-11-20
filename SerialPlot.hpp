#ifndef SERIALPLOT_HPP
#define SERIALPLOT_HPP

#include <Scheduler.hpp>
#include <EdgeDetector.hpp>

class Channels {
    List<String> _names;
public:
    Channels() { showAll(); }
    void showAll() { _names.clear(); _names.add("ALL"); }
    void showNone() { _names.clear(); }
    bool contains(String name) const { return _names.contains("ALL") || _names.contains(name); }
    bool add(String name) {
        if (!_names.contains(name)) {
            _names.add(name);
        }
    }
    bool remove(String name) {
        if (_names.contains(name)) {
            _names.remove(name);
        }
    }
    void print() {
        for (int i = 0; i < _names.length(); i++) {
            if (i != 0) Serial.print(",");
            Serial.print(_names[i]);
        }
    }
    void println() {
        print();
        Serial.println();
    }
};

class Plotted {
public:
    virtual bool plot(Channels &channels, bool sep = false) = 0;
};

class PlotComposite : public Composite<Plotted> {
public:
    bool plot(Channels &channels, bool sep = false) {
        bool result = sep;
        for (int i = 0; i < length(); i++) {
            result |= item(i)->plot(channels, result);
        }
        return result;
    }
};

class PlotBool : public Plotted {
    String _name;
    bool &_value;
public:
    PlotBool(PlotComposite &plot, String name, bool &value) : _name(name), _value(value) { plot.add(this); }
    bool plot(Channels &channels, bool sep = false) {
        if (channels.contains(_name)) {
            if (sep) {
                Serial.print(",");
            }
            Serial.print(_name);
            Serial.print(":");
            Serial.print(_value, DEC);
            return true;
        }
        return false;
    }
    static void addToPlot(PlotComposite &plot, String name, bool &value) {
        new PlotBool(plot, name, value);
    }
};

template <class T>
class PlotNum : public Plotted {
    String _name;
    T &_value;
public:
    PlotNum(PlotComposite &plot, String name, T &value) : _name(name), _value(value) { plot.add(this); }
    bool plot(Channels &channels, bool sep = false) {
        if (channels.contains(_name)) {
            if (sep) {
                Serial.print(",");
            }
            Serial.print(_name);
            Serial.print(":");
            Serial.print(_value, DEC);
            return true;
        }
        return false;
    }
};

class SerialPlot : public Clock, private EdgeDetectorBase, public PlotComposite {
    Channels _channels;
    long _time;
    bool _clock;
    static const long DefaultTime = 200;
public:
    SerialPlot(Schedule &schedule) :
        Clock(schedule, _time, _time, _clock),
        EdgeDetectorBase(schedule, _clock),
        _time(DefaultTime >> 1) { enable(false); enable(true); }
    void show(String channels) { _channels.add(channels); }
    void onRisingEdge() {
        if (plot(_channels)) {
            Serial.println();
        }
        //_channels.println();
    }
    void onFallingEdge() {
        if (Serial.available()) {
            String s = Serial.readString();
            s.trim();
            if (s == "ALL") { _channels.showAll(); }
            if (s == "NONE") { _channels.showNone(); }
            if (s[0] == '-') { _channels.remove(s.substring(1)); }
            if (s[0] == '+') { _channels.add(s.substring(1)); }
        }
    }
};

#endif
