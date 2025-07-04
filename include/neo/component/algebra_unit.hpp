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
#include <functional>

namespace jsi::neo
{

template <typename OutputType>
using CallbackFunction = std::function<OutputType(const PortList& properties)>;

template <typename OutputType>
class AlgebraUnit : public Component
{
public:
    AlgebraUnit(const CallbackFunction<OutputType>& callback_function) : Component()
    {
        callback_function_ = callback_function;
        properties_.set<OutputType>("output_value", OutputType());
    }

    void tick(uint64_t /*time_us*/) override
    {
        properties_.set<OutputType>("output_value", callback_function_(properties_));
    }

private:
    CallbackFunction<OutputType> callback_function_;
};

}  // namespace jsi::neo
