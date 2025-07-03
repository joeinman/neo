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
    Rectangle(int x, int y, int width, int height, Color color) : Component()
    {
        properties_.set<int>("x", x);
        properties_.set<int>("y", y);
        properties_.set<int>("width", width);
        properties_.set<int>("height", height);
        properties_.set<Color>("color", color);
    }

    std::pair<Position, std::vector<std::vector<Color>>> render() override
    {
        int   x      = properties_.get<int>("x").value_or(0);
        int   y      = properties_.get<int>("y").value_or(0);
        int   width  = properties_.get<int>("width").value_or(1);
        int   height = properties_.get<int>("height").value_or(1);
        Color color  = properties_.get<Color>("color").value_or(Color(255, 255, 255));

        return {{x, y}, std::vector<std::vector<Color>>(height, std::vector<Color>(width, color))};
    }
};

}  // namespace jsi::neo
