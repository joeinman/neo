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

#include "neo/portlist.hpp"
#include "neo/types.hpp"

namespace jsi::neo
{

class Component
{
public:
    Component() {}
    virtual void                                                 tick(const uint64_t& dt) = 0;
    virtual std::pair<Position, std::vector<std::vector<Color>>> render()                 = 0;

protected:
    PortList properties_;
};

}  // namespace jsi::neo
