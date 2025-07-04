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
#include <variant>
#include <any>
#include <typeindex>

#include "neo/portlist.hpp"
#include "neo/types.hpp"
#include "transitions/transition.hpp"

namespace jsi::neo
{

// Forward declaration for the transition wrapper
class TransitionBase {
public:
    virtual ~TransitionBase() = default;
    virtual void tick(uint64_t dt) = 0;
    virtual bool isCompleted() const = 0;
    virtual std::any getCurrentValue() const = 0;
    virtual std::type_index getType() const = 0;
};

template <typename T>
class TransitionWrapper : public TransitionBase {
public:
    TransitionWrapper(Transition<T> transition) : transition_(std::move(transition)) {}

    void tick(uint64_t dt) override {
        transition_.tick(dt);
    }

    bool isCompleted() const override {
        return transition_.getState() == TransitionState::kCompleted;
    }

    std::any getCurrentValue() const override {
        return transition_.getCurrentValue();
    }
    
    std::type_index getType() const override {
        return typeid(T);
    }

private:
    Transition<T> transition_;
};

class Component
{
public:
    Component(uint8_t z_index = 0) { properties_.set<uint8_t>("z_index", z_index); }
    virtual ~Component() = default;

    virtual void tick(uint64_t time_us)
    {
        std::vector<std::string> completed_keys;
        
        for (auto& [key, transition] : transitions_)
        {
            transition->tick(time_us);
            
            if (transition->isCompleted()) {
                completed_keys.push_back(key);
            } else {
                std::type_index type = transition->getType();
                std::any value = transition->getCurrentValue();
                
                // Set the property with the appropriate type
                if (type == typeid(uint8_t)) {
                    properties_.set<uint8_t>(key, std::any_cast<uint8_t>(value));
                } else if (type == typeid(int16_t)) {
                    properties_.set<int16_t>(key, std::any_cast<int16_t>(value));
                } else if (type == typeid(int)) {
                    properties_.set<int>(key, std::any_cast<int>(value));
                } else if (type == typeid(float)) {
                    properties_.set<float>(key, std::any_cast<float>(value));
                } else if (type == typeid(double)) {
                    properties_.set<double>(key, std::any_cast<double>(value));
                } else if (type == typeid(bool)) {
                    properties_.set<bool>(key, std::any_cast<bool>(value));
                }
                // Add more types as needed
            }
        }
        
        // Remove completed transitions
        for (const auto& key : completed_keys) {
            transitions_.erase(key);
        }
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
            // Get the current value, or use default if it doesn't exist
            auto current_value = properties_.get<T>(key).value_or(T{});
            
            // Create a new transition
            auto transition = std::make_unique<TransitionWrapper<T>>(
                Transition<T>(current_value, value, transition_type, transition_time_us)
            );
            
            // Store the transition
            transitions_[key] = std::move(transition);
        }
    }

protected:
    PortList properties_;
    std::map<std::string, std::unique_ptr<TransitionBase>> transitions_;
};

}  // namespace jsi::neo
