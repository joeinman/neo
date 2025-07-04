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
#include <string>

#include "neo/portlist.hpp"
#include "neo/types.hpp"

namespace jsi::neo
{

class Component
{
public:
    Component(uint8_t z_index = 0) { properties_.set<uint8_t>("z_index", z_index); }
    virtual ~Component() = default;

    virtual void                             tick(uint64_t /*time_us*/) {}
    virtual std::pair<Position, PixelBuffer> render()
    {
        return {{0, 0}, PixelBuffer(1, std::vector<Color>(1, Color(0, 0, 0, 0)))};
    }

    template <typename T>
    std::optional<T> getProperty(const std::string& key) const
    {
        return properties_.get<T>(key);
    }

    template <typename T>
    void setProperty(const std::string& key, const T& value)
    {
        properties_.set<T>(key, value);
    }

protected:
    PortList properties_;
};

}  // namespace jsi::neo
