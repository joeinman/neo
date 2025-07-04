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

        // Draw All Components In Z-Index Order
        for (const auto& [_, component] : scene->getComponentsInZOrder())
        {
            auto [position, colors] = component->render();
            auto x                  = position.x_;
            auto y                  = position.y_;

            // Calculate visible region of component
            int component_width  = static_cast<int>(colors.empty() ? 0 : colors[0].size());
            int component_height = static_cast<int>(colors.size());

            // Skip if component is entirely outside the screen
            if (x >= static_cast<int>(screen_size_.width_) || y >= static_cast<int>(screen_size_.height_) ||
                x + component_width <= 0 || y + component_height <= 0)
            {
                continue;
            }

            // Calculate visible area of component
            int start_x = std::max(0, static_cast<int>(x));
            int start_y = std::max(0, static_cast<int>(y));
            int end_x   = std::min(static_cast<int>(screen_size_.width_), static_cast<int>(x) + component_width);
            int end_y   = std::min(static_cast<int>(screen_size_.height_), static_cast<int>(y) + component_height);

            // Render the visible portion of the component
            for (int x_ = start_x; x_ < end_x; ++x_)
            {
                for (int y_ = start_y; y_ < end_y; ++y_)
                {
                    // Calculate component local coordinates
                    int comp_x = x_ - x;
                    int comp_y = y_ - y;

                    // Skip invalid component coordinates (added safety check)
                    if (comp_x < 0 || comp_y < 0 || comp_x >= component_width || comp_y >= component_height)
                    {
                        continue;
                    }

                    // Use explicit casts to size_t to avoid sign conversion warnings
                    const Color& src = colors[static_cast<size_t>(comp_y)][static_cast<size_t>(comp_x)];
                    Color&       dst = pixel_buffer_[static_cast<size_t>(y_)][static_cast<size_t>(x_)];

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
    ScreenSize       screen_size_;
    SetPixelFunction set_pixel_function_;
    ShowFunction     show_function_;
    PixelBuffer      pixel_buffer_;
};

}  // namespace jsi::neo
