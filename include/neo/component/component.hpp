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

#include <stdint.h>
#include <memory>
#include <map>
#include <string>

#include "neo/types.hpp"
#include "neo/portlist.hpp"

namespace jsi::neo
{

class Scene;

class Component
{
public:
    Component(std::shared_ptr<Scene> scene) : scene_(scene) {}
    virtual void tick(const uint64_t& dt) = 0;
    virtual void render()                 = 0;

protected:
    std::shared_ptr<Scene> scene_;
    PortList               input_properties_;
    PortList               output_properties_;
};

}  // namespace jsi::neo
