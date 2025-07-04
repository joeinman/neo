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

#include <memory>
#include <type_traits>
#include <iostream>

#include "easing_utils.hpp"

namespace jsi::neo
{

enum class TransitionState
{
    kIdle,
    kRunning,
    kCompleted,
};

enum class TransitionType
{
    kNone,
    kLinear,
    kEaseInQuad,
    kEaseOutQuad,
    kEaseInOutQuad,
    kEaseInCubic,
    kEaseOutCubic,
    kEaseInOutCubic,
    kEaseInExpo,
    kEaseOutExpo,
};

template <typename T>
class Transition
{
public:
    Transition(const T& start_value,
               const T& target_value,
               TransitionType transition_type,
               uint64_t       transition_time_us) :
        state_(TransitionState::kIdle),
        target_value_(target_value),
        transition_type_(transition_type),
        transition_time_us_(transition_time_us),
        elapsed_time_us(0),
        start_value_(start_value),
        current_value_(start_value)
    {}

    ~Transition() = default;

    T getCurrentValue() const { return current_value_; }

    TransitionState getState() const { return state_; }

    void tick(uint64_t dt)
    {
        switch (state_)
        {
        case TransitionState::kIdle:
            state_ = handle_idle_state(dt);
            break;
        case TransitionState::kRunning:
            state_ = handle_running_state(dt);
            break;
        case TransitionState::kCompleted:
            state_ = handle_completed_state(dt);
            break;
        }
    }

    TransitionState handle_idle_state(uint64_t /*dt*/) { return TransitionState::kRunning; }

    TransitionState handle_running_state(uint64_t dt)
    {
        if (elapsed_time_us >= transition_time_us_)
        {
            return TransitionState::kCompleted;
        }

        elapsed_time_us += dt;
        double t = static_cast<double>(elapsed_time_us) / static_cast<double>(transition_time_us_);

        switch (transition_type_)
        {
        case TransitionType::kLinear:
            current_value_ = interpolate(start_value_, target_value_, t);
            break;
        case TransitionType::kEaseInQuad:
            current_value_ = interpolate(start_value_, target_value_, easing::easeInQuad(t));
            break;
        case TransitionType::kEaseOutQuad:
            current_value_ = interpolate(start_value_, target_value_, easing::easeOutQuad(t));
            break;
        case TransitionType::kEaseInOutQuad:
            current_value_ = interpolate(start_value_, target_value_, easing::easeInOutQuad(t));
            break;
        case TransitionType::kEaseInCubic:
            current_value_ = interpolate(start_value_, target_value_, easing::easeInCubic(t));
            break;
        case TransitionType::kEaseOutCubic:
            current_value_ = interpolate(start_value_, target_value_, easing::easeOutCubic(t));
            break;
        case TransitionType::kEaseInOutCubic:
            current_value_ = interpolate(start_value_, target_value_, easing::easeInOutCubic(t));
            break;
        case TransitionType::kEaseInExpo:
            current_value_ = interpolate(start_value_, target_value_, easing::easeInExpo(t));
            break;
        case TransitionType::kEaseOutExpo:
            current_value_ = interpolate(start_value_, target_value_, easing::easeOutExpo(t));
            break;
        default:
            current_value_ = target_value_;
            break;
        }
        return TransitionState::kRunning;
    }

    TransitionState handle_completed_state(uint64_t /*dt*/)
    {
        if constexpr (std::is_integral_v<T>) {
            printf("Transition completed: %d -> %d\n", start_value_, target_value_);
        } else if constexpr (std::is_floating_point_v<T>) {
            printf("Transition completed: %f -> %f\n", start_value_, target_value_);
        } else {
            printf("Transition completed\n");
        }
        
        current_value_ = target_value_;
        return TransitionState::kCompleted;
    }

private:
    // Helper function to interpolate between values
    T interpolate(const T& start, const T& end, double t) const {
        if constexpr (std::is_arithmetic_v<T>) {
            return static_cast<T>(start + (end - start) * t);
        } else {
            // For non-arithmetic types, they must provide their own interpolation
            // through operator+, operator-, and operator*
            return start + (end - start) * t;
        }
    }

    TransitionState state_ = TransitionState::kIdle;
    T               target_value_;
    TransitionType  transition_type_;
    uint64_t        transition_time_us_;
    uint64_t        elapsed_time_us;
    T               start_value_;
    T               current_value_;
};

}  // namespace jsi::neo