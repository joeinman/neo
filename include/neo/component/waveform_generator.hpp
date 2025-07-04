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
    }

    void tick(uint64_t dt) override
    {
        auto frequency = properties_.get<double>("frequency").value();
        auto type      = properties_.get<WaveformType>("waveform_type").value();
        auto enabled   = properties_.get<bool>("enabled").value_or(true);

        if (enabled)
        {
            time_sec_ += static_cast<double>(dt) * 1e-6;
        }
        
        double value;

        switch (type)
        {
        case WaveformType::kSine:
            value = 0.5 * (sin(2.0 * M_PI * frequency * time_sec_) + 1.0);
            break;
        case WaveformType::kSquare:
            value = (sin(2.0 * M_PI * frequency * time_sec_) >= 0) ? 1.0 : 0.0;
            break;
        case WaveformType::kTriangle:
        {
            double phase = fmod(frequency * time_sec_, 1.0);
            if (phase < 0.5)
            {
                value = phase * 2.0;
            }
            else
            {
                value = 2.0 - (phase * 2.0);
            }
            break;
        }
        case WaveformType::kSawtooth:
            value = fmod(frequency * time_sec_, 1.0);
            break;
        }

        properties_.set<double>("output_value", value);
    }

private:
    double time_sec_ = 0.0;
};

}  // namespace jsi::neo
