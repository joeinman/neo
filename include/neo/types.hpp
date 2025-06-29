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

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
};

struct Position
{
    int x;
    int y;
};

}  // namespace jsi::neo
