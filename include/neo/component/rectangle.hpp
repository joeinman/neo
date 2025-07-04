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
    Rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, Color color, uint8_t z_index = 0) :
        Component(z_index)
    {
        properties_.set<uint8_t>("x", x);
        properties_.set<uint8_t>("y", y);
        properties_.set<uint8_t>("width", width);
        properties_.set<uint8_t>("height", height);

        properties_.set<uint8_t>("r", color.r_);
        properties_.set<uint8_t>("g", color.g_);
        properties_.set<uint8_t>("b", color.b_);
        properties_.set<uint8_t>("a", color.a_);
    }

    std::pair<Position, PixelBuffer> render() override
    {
        auto x      = properties_.get<uint8_t>("x").value();
        auto y      = properties_.get<uint8_t>("y").value();
        auto width  = properties_.get<uint8_t>("width").value();
        auto height = properties_.get<uint8_t>("height").value();
        auto color  = Color(properties_.get<uint8_t>("r").value(),
                           properties_.get<uint8_t>("g").value(),
                           properties_.get<uint8_t>("b").value(),
                           properties_.get<uint8_t>("a").value());

        return {{x, y}, PixelBuffer(height, std::vector<Color>(width, color))};
    }
};

}  // namespace jsi::neo
