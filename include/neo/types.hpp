#pragma once

#include <functional>
#include <stdint.h>

namespace jsi
{

using MicrosFunction   = std::function<uint64_t()>;
using SetPixelFunction = std::function<void(size_t, uint8_t, uint8_t, uint8_t, uint8_t)>;
using ShowFunction     = std::function<void()>;

struct ScreenSize
{
    size_t width;
    size_t height;

    ScreenSize(size_t w, size_t h) : width(w), height(h) {}
};

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) : r(red), g(green), b(blue), a(alpha) {}
};

struct Position
{
    int x;
    int y;

    Position(int x_coord, int y_coord) : x(x_coord), y(y_coord) {}
};

}  // namespace jsi
