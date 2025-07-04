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
#include <map>
#include <memory>

#include "neo/portlist.hpp"
#include "neo/types.hpp"
#include "transitions/transition.hpp"

namespace jsi::neo
{

class Component
{
public:
    Component(uint8_t z_index = 0) { properties_.set<uint8_t>("z_index", z_index); }
    virtual ~Component() = default;

    virtual void tick(uint64_t time_us)
    {
        for (auto& [key, transition] : transitions_)
        {
            transition.tick(time_us);
            properties_.set<uint8_t>(key, transition.getCurrentValue());
        }
        std::erase_if(transitions_, [](auto const& kv) { return kv.second.getState() == TransitionState::kCompleted; });
    }

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
    void setProperty(const std::string& key,
                     const T&           value,
                     TransitionType     transition_type    = TransitionType::kNone,
                     uint64_t           transition_time_us = 1000000)
    {
        if (transition_type == TransitionType::kNone)
        {
            properties_.set<T>(key, value);
        }
        else
        {
            auto it = transitions_.find(key);
            if (it != transitions_.end())
            {
                // Update existing transition
                it->second = Transition(properties_.get<uint8_t>(key).value_or(uint8_t()),
                                        value,
                                        transition_type,
                                        transition_time_us);
            }
            else
            {
                transitions_.emplace(key,
                                     Transition(properties_.get<uint8_t>(key).value_or(uint8_t()),
                                                value,
                                                transition_type,
                                                transition_time_us));
            }
        }
    }

protected:
    PortList                          properties_;
    std::map<std::string, Transition> transitions_;
};

}  // namespace jsi::neo
