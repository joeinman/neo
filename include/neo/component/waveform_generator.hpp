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

#include <cmath>
#include <algorithm>

#include "component.hpp"

namespace jsi::neo
{

enum class WaveformType
{
    kSine,
    kSquare,
    kTriangle,
    kSawtooth
};

class WaveformGenerator : public Component
{
public:
    WaveformGenerator(double frequency, WaveformType waveform_type = WaveformType::kSine) : Component()
    {
        properties_.set<double>("frequency", frequency);
        properties_.set<WaveformType>("waveform_type", waveform_type);
        properties_.set<bool>("enabled", true);

        properties_.set<double>("output_value", 0.0);
        properties_.set<double>("cycle_position", 0.0);
    }

    void tick(uint64_t dt) override
    {
        auto frequency = properties_.get<double>("frequency").value();
        auto type      = properties_.get<WaveformType>("waveform_type").value();
        auto enabled   = properties_.get<bool>("enabled").value_or(true);

        // Read externally-set phase if present, otherwise use internal
        double phase = properties_.get<double>("cycle_position").value();
        // Normalize into [0,1)
        phase = std::fmod(phase, 1.0);
        if (phase < 0.0)
            phase += 1.0;

        if (!enabled)
        {
            return;
        }

        // Advance phase by frequency * dt_seconds
        double dt_sec = static_cast<double>(dt) * 1e-6;
        phase += frequency * dt_sec;
        phase = std::fmod(phase, 1.0);
        if (phase < 0.0)
            phase += 1.0;

        // Store updated phase internally and in properties
        phase_ = phase;
        properties_.set<double>("cycle_position", phase_);

        // Compute output in [0,1] based on waveform type
        double value = 0.0;
        switch (type)
        {
        case WaveformType::kSine:
            value = 0.5 * (std::sin(2.0 * M_PI * phase_) + 1.0);
            break;

        case WaveformType::kSquare:
            value = (std::sin(2.0 * M_PI * phase_) >= 0.0) ? 1.0 : 0.0;
            break;

        case WaveformType::kTriangle:
            if (phase_ < 0.5)
                value = phase_ * 2.0;
            else
                value = 2.0 - (phase_ * 2.0);
            break;

        case WaveformType::kSawtooth:
            value = phase_;
            break;
        }

        properties_.set<double>("output_value", value);
    }

private:
    double phase_ = 0.0;
};

}  // namespace jsi::neo
