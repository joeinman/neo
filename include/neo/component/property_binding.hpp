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

/**
 * A component that binds a source output to a target property
 */
template <typename T>
class PropertyBinding : public Component
{
public:
    /**
     * Creates a binding between a source component's output and a target component's property
     *
     * @param source_component The component from which to get the output value
     * @param source_key The key of the output to get from the source component
     * @param target_component The component whose property will be updated
     * @param target_key The key of the property to update in the target component
     */
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

        auto source_value = source_component_->getOutput<T>(source_key_);
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

/**
 * A component that applies a transformation function to a value before binding
 */
template <typename SourceType, typename TargetType>
class TransformedPropertyBinding : public Component
{
public:
    using TransformFunction = std::function<TargetType(SourceType)>;

    /**
     * Creates a binding with transformation between a source component's output and a target component's property
     *
     * @param source_component The component from which to get the output value
     * @param source_key The key of the output to get from the source component
     * @param target_component The component whose property will be updated
     * @param target_key The key of the property to update in the target component
     * @param transformer Function to transform the source value to the target type
     */
    TransformedPropertyBinding(std::shared_ptr<Component> source_component,
                               const std::string&         source_key,
                               std::shared_ptr<Component> target_component,
                               const std::string&         target_key,
                               TransformFunction          transformer) :
        Component(),
        source_component_(source_component),
        source_key_(source_key),
        target_component_(target_component),
        target_key_(target_key),
        transformer_(transformer)
    {}

    void tick(uint64_t time_us) override
    {
        Component::tick(time_us);

        auto source_value = source_component_->getOutput<SourceType>(source_key_);
        if (source_value.has_value())
        {
            TargetType transformed_value = transformer_(source_value.value());
            target_component_->setProperty<TargetType>(target_key_, transformed_value);
        }
    }

private:
    std::shared_ptr<Component> source_component_;
    std::string                source_key_;
    std::shared_ptr<Component> target_component_;
    std::string                target_key_;
    TransformFunction          transformer_;
};

}  // namespace jsi::neo
