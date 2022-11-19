#ifndef PADDLE_HPP
#define PADDLE_HPP

#include <Graphics.hpp>
#include <EncoderWheel.hpp>

class Paddle : public Drawable, private EncoderControl<int16_t> {
  int16_t _x;
  int16_t _y;
  int16_t _size;
public:
  Paddle(Schedule &schedule, MainWindow &window, const EncoderConfig &config, int16_t x, int16_t size, int16_t height) :
    EncoderControl<int16_t>(schedule, config, _y, (x > 5 ? -5 : 5), height),
    _x(x), _y(height >> 1), _size(size) {
    window.add(this);
  }
  void draw(Adafruit_GFX &display) {
    display.drawLine(_x, y0(), _x, y1(), SSD1306_WHITE);
  }
  int16_t x() const { return _x; }
  int16_t y0() const { return _y - (_size >> 1); }
  int16_t y1() const { return _y + (_size >> 1); }
};

#endif