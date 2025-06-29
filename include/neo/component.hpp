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

#include "neo/types.hpp"

namespace jsi
{

class Component
{
public:
    virtual void tick(const uint64_t& dt) = 0;
    virtual void render()                 = 0;
};

}  // namespace jsi
