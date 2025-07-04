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

#include <functional>
#include <vector>
#include <memory>
#include <queue>
#include <map>

#include "neo/component/property_binding.hpp"
#include "neo/component/component.hpp"
#include "neo/portlist.hpp"

namespace jsi::neo
{

class Scene
{
    struct ComponentZIndexComparator
    {
        bool operator()(const std::pair<uint64_t, std::shared_ptr<Component>>& a,
                        const std::pair<uint64_t, std::shared_ptr<Component>>& b) const
        {
            // First compare by z-index
            uint8_t z_index_a = a.second->getProperty<uint8_t>("z_index").value_or(0);
            uint8_t z_index_b = b.second->getProperty<uint8_t>("z_index").value_or(0);

            if (z_index_a != z_index_b)
            {
                return z_index_a > z_index_b;  // Lower z_index rendered first (appears at the back)
            }

            // If z-indexes are equal, sort by ID for stable ordering
            return a.first > b.first;
        }
    };

public:
    Scene()          = default;
    virtual ~Scene() = default;

    template <typename T, typename... Args>
    uint64_t addComponent(Args&&... args)
    {
        auto     component = std::make_shared<T>(std::forward<Args>(args)...);
        uint64_t id        = next_id_++;

        components_.push(std::make_pair(id, component));
        component_lookup_[id] = component;

        return id;
    }

    std::vector<std::pair<uint64_t, std::shared_ptr<Component>>> getComponentsInZOrder() const
    {
        std::vector<std::pair<uint64_t, std::shared_ptr<Component>>> sorted_components;

        auto queue_copy = components_;
        while (!queue_copy.empty())
        {
            sorted_components.push_back(queue_copy.top());
            queue_copy.pop();
        }

        return sorted_components;
    }

    std::shared_ptr<Component> getComponent(uint64_t id)
    {
        auto it = component_lookup_.find(id);
        if (it != component_lookup_.end())
        {
            return it->second;
        }
        return nullptr;
    }

    template <typename T>
    bool setComponentProperty(uint64_t id, const std::string& key, const T& value)
    {
        auto component = getComponent(id);
        if (component)
        {
            component->setProperty<T>(key, value);
            return true;
        }
        return false;
    }

    virtual void tick(uint64_t time_us)
    {
        for (auto& [id, component] : component_lookup_)
        {
            component->tick(time_us);
        }
    }

    template <typename SourceType, typename TargetType = SourceType>
    uint64_t connectProperties(uint64_t           source_id,
                               const std::string& source_key,
                               uint64_t           target_id,
                               const std::string& target_key)
    {
        auto source_component = getComponent(source_id);
        auto target_component = getComponent(target_id);

        if (source_component && target_component)
        {
            return addComponent<PropertyBinding<SourceType, TargetType>>(source_component,
                                                                         source_key,
                                                                         target_component,
                                                                         target_key);
        }

        return 0;
    }

private:
    // Map for fast component lookup by ID
    std::map<uint64_t, std::shared_ptr<Component>> component_lookup_;

    // Priority queue for z-index sorted components
    std::priority_queue<std::pair<uint64_t, std::shared_ptr<Component>>,
                        std::vector<std::pair<uint64_t, std::shared_ptr<Component>>>,
                        ComponentZIndexComparator>
        components_;

    uint64_t next_id_ = 0;
};

}  // namespace jsi::neo
