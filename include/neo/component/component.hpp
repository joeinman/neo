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
    Component() {}
    virtual ~Component() = default;

    virtual void                                                 tick(const uint64_t& /*dt*/) {}
    virtual std::pair<Position, PixelBuffer> render() = 0;

    template <typename T>
    std::optional<T> getProperty(const std::string& key) const
    {
        return properties_.get<T>(key);
    }

    template <typename T>
    void setProperty(const std::string& key, const T& value)
    {
        auto res = properties_.get<T>(key);
        if (res.has_value())
        {
            properties_.set<T>(key, value);
        }
    }

protected:
    PortList properties_;
};

}  // namespace jsi::neo
