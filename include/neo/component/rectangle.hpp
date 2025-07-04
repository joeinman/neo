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
        properties_.set<bool>("visible", true);
        properties_.set<float>("opacity", 1.0f);
    }

    std::pair<Position, PixelBuffer> render() override
    {
        auto x       = properties_.get<int16_t>("x").value();
        auto y       = properties_.get<int16_t>("y").value();
        auto width   = properties_.get<uint16_t>("width").value();
        auto height  = properties_.get<uint16_t>("height").value();
        auto opacity = properties_.get<float>("opacity").value_or(1.0f);
        
        auto r = properties_.get<uint8_t>("r").value();
        auto g = properties_.get<uint8_t>("g").value();
        auto b = properties_.get<uint8_t>("b").value();
        auto a = static_cast<uint8_t>(properties_.get<uint8_t>("a").value() * opacity);
        
        auto color = Color(r, g, b, a);

        if (properties_.get<bool>("visible").value_or(true) && width > 0 && height > 0)
        {
            return {{x, y}, PixelBuffer(height, std::vector<Color>(width, color))};
        }
        return {{x, y}, PixelBuffer(1, std::vector<Color>(1, Color(0, 0, 0, 0)))};
    }
};

}  // namespace jsi::neo
