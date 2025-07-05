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

#include "neo/types.hpp"
#include "neo/scene.hpp"
#include "neo/screen.hpp"

namespace jsi::neo
{

class Neo
{
public:
    Neo(const ScreenSize&       screen_size,
        const SetPixelFunction& set_pixel_function,
        const ShowFunction&     show_function,
        const MicrosFunction&   micros_function,
        float                   gamma = 1.0f) noexcept
    {
        screen_          = std::make_unique<Screen>(screen_size, set_pixel_function, show_function, gamma);
        micros_function_ = micros_function;
    }

    ~Neo() = default;

    void spin() noexcept
    {
        static uint64_t last_tick    = micros_function_();
        uint64_t        current_tick = micros_function_();
        uint64_t        dt           = current_tick - last_tick;
        last_tick                    = current_tick;

        scene_->tick(dt);
        screen_->render(scene_);
    }

    void loadScene(std::shared_ptr<Scene> scene) noexcept { scene_ = std::move(scene); }

    std::shared_ptr<Scene> getScene() const noexcept { return scene_; }

private:
    std::unique_ptr<Screen> screen_;
    std::shared_ptr<Scene>  scene_ = std::make_shared<Scene>();
    MicrosFunction          micros_function_;
};

}  // namespace jsi::neo
