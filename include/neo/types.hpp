#pragma once

#include <functional>
#include <stdint.h>
#include <variant>
#include <string>

namespace jsi::neo
{

using MicrosFunction   = std::function<uint64_t()>;
using SetPixelFunction = std::function<void(size_t, uint8_t, uint8_t, uint8_t, uint8_t)>;
using ShowFunction     = std::function<void()>;

using PortName = std::string;

struct ScreenSize
{
    size_t width_;
    size_t height_;

    ScreenSize(size_t width, size_t height) : width_(width), height_(height) {}
};

struct Color
{
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
    uint8_t a_;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r_(r), g_(g), b_(b), a_(a) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : r_(r), g_(g), b_(b), a_(255) {}
};

struct Position
{
    int16_t x_;
    int16_t y_;
};

using PixelBuffer = std::vector<std::vector<Color>>;

}  // namespace jsi::neo
