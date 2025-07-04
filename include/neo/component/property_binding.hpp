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

#include "component.hpp"
#include <functional>
#include <memory>
#include <string>
#include <type_traits>

namespace jsi::neo
{

template <typename T>
class PropertyBinding : public Component
{
public:
    PropertyBinding(std::shared_ptr<Component> source_component,
                    const std::string&         source_key,
                    std::shared_ptr<Component> target_component,
                    const std::string&         target_key) :
        Component(),
        source_component_(source_component),
        source_key_(source_key),
        target_component_(target_component),
        target_key_(target_key)
    {}

    void tick(uint64_t time_us) override
    {
        Component::tick(time_us);

        auto source_value = source_component_->getProperty<T>(source_key_);
        if (source_value.has_value())
        {
            target_component_->setProperty<T>(target_key_, source_value.value());
        }
    }

private:
    std::shared_ptr<Component> source_component_;
    std::string                source_key_;
    std::shared_ptr<Component> target_component_;
    std::string                target_key_;
};

}  // namespace jsi::neo
