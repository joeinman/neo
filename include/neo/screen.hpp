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
#include <vector>

#include "neo/types.hpp"
#include "neo/scene.hpp"

namespace jsi::neo
{

class Screen
{
public:
    Screen(const ScreenSize&       screen_size,
           const SetPixelFunction& set_pixel_function,
           const ShowFunction&     show_function) :
        screen_size_(screen_size),
        set_pixel_function_(set_pixel_function),
        show_function_(show_function),
        pixel_buffer_(screen_size.height, std::vector<Color>(screen_size.width, Color(0, 0, 0, 0)))
    {}
    ~Screen() = default;

    void render(const std::shared_ptr<Scene>& scene)
    {
        // Clear Pixel Buffer
        for (auto& row : pixel_buffer_)
        {
            std::fill(row.begin(), row.end(), Color(0, 0, 0, 0));
        }

        // Draw All Components
        for (const auto& component : scene->getComponents())
        {
            auto [position, colors] = component.second->render();
            int x                   = position.x;
            int y                   = position.y;

            for (int x_ = x; x_ < x + colors[0].size() && x_ < screen_size_.width; ++x_)
            {
                for (int y_ = y; y_ < y + colors.size() && y_ < screen_size_.height; ++y_)
                {
                    if (x_ >= 0 && y_ >= 0)
                    {
                        pixel_buffer_[y_][x_] = colors[y_ - y][x_ - x];
                    }
                }
            }
        }

        // Set Pixels
        for (size_t y = 0; y < screen_size_.height; ++y)
        {
            for (size_t x = 0; x < screen_size_.width; ++x)
            {
                const Color& color = pixel_buffer_[y][x];
                set_pixel_function_(x, color.r, color.g, color.b, color.a);
            }
        }

        // Show the pixels
        show_function_();
    }

private:
    ScreenSize                      screen_size_;
    SetPixelFunction                set_pixel_function_;
    ShowFunction                    show_function_;
    std::vector<std::vector<Color>> pixel_buffer_;
};

}  // namespace jsi::neo
