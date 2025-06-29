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
    Rectangle(std::shared_ptr<Scene> scene, int x, int y, int width, int height, Color color) : Component(scene)
    {
        properties_.set<int>("x", x);
        properties_.set<int>("y", y);
        properties_.set<int>("width", width);
        properties_.set<int>("height", height);
        properties_.set<Color>("color", color);
    }

    void tick(const uint64_t& dt) override;

    void render() override;
};

}  // namespace jsi::neo
