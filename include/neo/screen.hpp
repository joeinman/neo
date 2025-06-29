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

class Screen
{
public:
    Screen(const ScreenSize&       screen_size,
           const SetPixelFunction& set_pixel_function,
           const ShowFunction&     show_function) :
        screen_size_(screen_size), set_pixel_function_(set_pixel_function), show_function_(show_function)
    {}
    ~Screen() = default;

    void render(const std::shared_ptr<Scene>& scene)
    {
        for (size_t y = 0; y < screen_size_.height; ++y)
        {
            for (size_t x = 0; x < screen_size_.width; ++x)
            {
                size_t index = y * screen_size_.width + x;
                set_pixel_function_(index, 255, 0, 0, 255);
            }
        }
        show_function_();
    }

private:
    ScreenSize       screen_size_;
    SetPixelFunction set_pixel_function_;
    ShowFunction     show_function_;
};

}  // namespace jsi
