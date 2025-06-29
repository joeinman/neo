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
        pixel_buffer_(screen_size.width * screen_size.height, Color(0, 0, 0, 0))
    {}
    ~Screen() = default;

    void render(const std::shared_ptr<Scene>& scene)
    {
        // Set screen in scene for component rendering
        scene->setScreen(this);

        // Clear the pixel buffer (set all pixels to transparent black)
        for (size_t i = 0; i < pixel_buffer_.size(); ++i)
        {
            pixel_buffer_[i] = Color(0, 0, 0, 0);
        }

        // Render all components (they'll update the pixel buffer via setPixel)
        for (const auto& [id, component] : scene->getComponents())
        {
            component->render();
        }

        // Transfer the pixel buffer to the actual display
        for (size_t i = 0; i < pixel_buffer_.size(); ++i)
        {
            const auto& color = pixel_buffer_[i];
            set_pixel_function_(i, color.r, color.g, color.b, color.a);
        }

        // Display the rendered pixels
        show_function_();
    }

    void setPixel(int x, int y, const Color& color)
    {
        // Bounds checking
        if (x < 0 || x >= static_cast<int>(screen_size_.width) || y < 0 || y >= static_cast<int>(screen_size_.height))
        {
            return;
        }

        size_t index = y * screen_size_.width + x;

        // Handle transparency (alpha blending)
        if (color.a == 255)
        {
            // Fully opaque - just replace the pixel
            pixel_buffer_[index] = color;
        }
        else if (color.a > 0)
        {
            // Semi-transparent - blend with existing pixel
            const Color& bg        = pixel_buffer_[index];
            float        alpha     = color.a / 255.0f;
            float        inv_alpha = 1.0f - alpha;

            uint8_t new_r = static_cast<uint8_t>(color.r * alpha + bg.r * inv_alpha);
            uint8_t new_g = static_cast<uint8_t>(color.g * alpha + bg.g * inv_alpha);
            uint8_t new_b = static_cast<uint8_t>(color.b * alpha + bg.b * inv_alpha);
            uint8_t new_a = static_cast<uint8_t>(std::min(255, color.a + bg.a));

            pixel_buffer_[index] = Color(new_r, new_g, new_b, new_a);
        }
        // If alpha is 0, the pixel is completely transparent, so we don't change anything
    }

private:
    ScreenSize         screen_size_;
    SetPixelFunction   set_pixel_function_;
    ShowFunction       show_function_;
    std::vector<Color> pixel_buffer_;  // Buffer to handle transparency
};

}  // namespace jsi::neo
