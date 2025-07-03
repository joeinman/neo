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
        pixel_buffer_(screen_size.height_, std::vector<Color>(screen_size.width_, Color(0, 0, 0, 0)))
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
            auto x                  = position.x_;
            auto y                  = position.y_;

            for (uint8_t x_ = x; x_ < x + colors[0].size() && x_ < screen_size_.width_; ++x_)
            {
                for (uint8_t y_ = y; y_ < y + colors.size() && y_ < screen_size_.height_; ++y_)
                {
                    const Color& src = colors[y_ - y][x_ - x];
                    Color&       dst = pixel_buffer_[y_][x_];

                    if (src.a_ == 255)
                    {
                        dst = src;
                    }
                    else if (src.a_ > 0)
                    {
                        float source_alpha      = src.a_ / 255.0f;
                        float destination_alpha = 1.0f - source_alpha;

                        dst.r_ = static_cast<uint8_t>(src.r_ * source_alpha + dst.r_ * destination_alpha);
                        dst.g_ = static_cast<uint8_t>(src.g_ * source_alpha + dst.g_ * destination_alpha);
                        dst.b_ = static_cast<uint8_t>(src.b_ * source_alpha + dst.b_ * destination_alpha);
                        dst.a_ = static_cast<uint8_t>(255 - ((255 - src.a_) * (255 - dst.a_) / 255));
                    }
                }
            }
        }

        // Render Pixel Buffer
        for (size_t y = 0; y < screen_size_.height_; ++y)
        {
            for (size_t x = 0; x < screen_size_.width_; ++x)
            {
                const Color& color = pixel_buffer_[y][x];
                uint8_t      r     = static_cast<uint8_t>((static_cast<float>(color.r_) * color.a_) / 255.0f);
                uint8_t      g     = static_cast<uint8_t>((static_cast<float>(color.g_) * color.a_) / 255.0f);
                uint8_t      b     = static_cast<uint8_t>((static_cast<float>(color.b_) * color.a_) / 255.0f);
                set_pixel_function_(x, r, g, b, color.a_);
            }
        }
        show_function_();
    }

private:
    ScreenSize                      screen_size_;
    SetPixelFunction                set_pixel_function_;
    ShowFunction                    show_function_;
    std::vector<std::vector<Color>> pixel_buffer_;
};

}  // namespace jsi::neo
