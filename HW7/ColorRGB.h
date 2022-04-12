#pragma once

// A struct to represent an RGB color value. Every channel is an unsigned integer in the interval [0;255].
struct ColorRGB {
  unsigned char r; //red
  unsigned char g; //green
  unsigned char b; //blue

  //"global" property of the color structure
  static const int maxColorComponents = 255;

  ColorRGB() : r(0), g(0), b(0) {}
  ColorRGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}
  ColorRGB(const ColorRGB& color) :r(color.r), g(color.g), b(color.b) {}

  ColorRGB& operator=(const ColorRGB& color) {
    r = color.r;
    g = color.g;
    b = color.b;
    return *this;
  }
};