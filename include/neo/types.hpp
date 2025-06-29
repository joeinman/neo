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

template <typename T>
using Value = std::variant<T, PortName>;

struct ScreenSize
{
    size_t width;
    size_t height;
};

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct Position
{
    int x;
    int y;
};

}  // namespace jsi::neo
