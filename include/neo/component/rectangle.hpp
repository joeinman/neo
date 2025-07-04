/*
 * Copyright (c) 2025, Joe Inman
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at:
 *     https://opensource.org/licenses/MIT
 *
 * This file is part of the Neo Library.
 */

#pragma once

#include "component.hpp"

namespace jsi::neo
{

class Rectangle : public Component
{
public:
    Rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, Color color, uint8_t z_index = 0) :
        Component(z_index)
    {
        properties_.set<int16_t>("x", x);
        properties_.set<int16_t>("y", y);
        properties_.set<uint16_t>("width", width);
        properties_.set<uint16_t>("height", height);

        properties_.set<uint8_t>("r", color.r_);
        properties_.set<uint8_t>("g", color.g_);
        properties_.set<uint8_t>("b", color.b_);
        properties_.set<uint8_t>("a", color.a_);
    }

    std::pair<Position, PixelBuffer> render() override
    {
        auto x      = properties_.get<int16_t>("x").value();
        auto y      = properties_.get<int16_t>("y").value();
        auto width  = properties_.get<uint16_t>("width").value();
        auto height = properties_.get<uint16_t>("height").value();
        auto color  = Color(properties_.get<uint8_t>("r").value(),
                           properties_.get<uint8_t>("g").value(),
                           properties_.get<uint8_t>("b").value(),
                           properties_.get<uint8_t>("a").value());

        return {{x, y}, PixelBuffer(height, std::vector<Color>(width, color))};
    }
};

}  // namespace jsi::neo
