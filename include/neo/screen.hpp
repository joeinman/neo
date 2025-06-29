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
#include "neo/scene.hpp"

namespace jsi::neo
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
        // Set screen in scene for component rendering
        scene->setScreen(this);
        
        // Clear screen (set all pixels to black)
        for (size_t y = 0; y < screen_size_.height; ++y)
        {
            for (size_t x = 0; x < screen_size_.width; ++x)
            {
                size_t index = y * screen_size_.width + x;
                set_pixel_function_(index, 0, 0, 0, 0);
            }
        }
        
        // Render all components
        for (const auto& [id, component] : scene->getComponents())
        {
            component->render();
        }
        
        // Display the rendered pixels
        show_function_();
    }
    
    void setPixel(int x, int y, const Color& color)
    {
        // Bounds checking
        if (x < 0 || x >= static_cast<int>(screen_size_.width) || 
            y < 0 || y >= static_cast<int>(screen_size_.height))
        {
            return;
        }
        
        size_t index = y * screen_size_.width + x;
        set_pixel_function_(index, color.r, color.g, color.b, color.a);
    }

private:
    ScreenSize       screen_size_;
    SetPixelFunction set_pixel_function_;
    ShowFunction     show_function_;
};

}  // namespace jsi::neo
