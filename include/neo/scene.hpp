#pragma once

#include <vector>
#include <memory>
#include <queue>
#include <functional>
#include <string>

#include "neo/component/component.hpp"
#include "neo/component/property_binding.hpp"
#include "neo/portlist.hpp"
#include <map>

namespace jsi::neo
{

class Scene
{
    // Comparator for component z-index sorting
    struct ComponentZIndexComparator
    {
        bool operator()(const std::pair<std::string, std::shared_ptr<Component>>& a,
                        const std::pair<std::string, std::shared_ptr<Component>>& b) const
        {
            uint8_t z_index_a = a.second->getProperty<uint8_t>("z_index").value_or(0);
            uint8_t z_index_b = b.second->getProperty<uint8_t>("z_index").value_or(0);

            if (z_index_a != z_index_b)
            {
                return z_index_a > z_index_b;
            }

            return a.first > b.first;
        }
    };

public:
    Scene()          = default;
    virtual ~Scene() = default;

    template <typename T, typename... Args>
    std::string addComponent(const std::string& id, Args&&... args)
    {
        auto component = std::make_shared<T>(std::forward<Args>(args)...);

        components_.push(std::make_pair(id, component));
        component_lookup_[id] = component;

        return id;
    }

    std::vector<std::pair<std::string, std::shared_ptr<Component>>> getComponentsInZOrder() const
    {
        std::vector<std::pair<std::string, std::shared_ptr<Component>>> sorted_components;

        auto queue_copy = components_;
        while (!queue_copy.empty())
        {
            sorted_components.push_back(queue_copy.top());
            queue_copy.pop();
        }

        return sorted_components;
    }

    std::shared_ptr<Component> getComponent(const std::string& id)
    {
        auto it = component_lookup_.find(id);
        if (it != component_lookup_.end())
        {
            return it->second;
        }
        return nullptr;
    }

    template <typename T>
    bool setComponentProperty(const std::string& id,
                              const std::string& key,
                              const T&           value,
                              TransitionType     transition_type    = TransitionType::kNone,
                              uint64_t           transition_time_us = 1000000)
    {
        auto component = getComponent(id);
        if (component)
        {
            component->setProperty<T>(key, value, transition_type, transition_time_us);
            return true;
        }
        return false;
    }

    template <typename T>
    std::optional<T> getComponentProperty(const std::string& id, const std::string& key)
    {
        auto component = getComponent(id);
        if (component)
        {
            return component->getProperty<T>(key);
        }
        return std::nullopt;
    }

    virtual void tick(uint64_t time_us)
    {
        for (auto& [id, component] : component_lookup_)
        {
            component->tick(time_us);
        }
    }

    template <typename SourceType, typename TargetType = SourceType>
    std::string connectProperties(const std::string& source_id,
                                  const std::string& source_key,
                                  const std::string& target_id,
                                  const std::string& target_key)
    {
        auto source_component = getComponent(source_id);
        auto target_component = getComponent(target_id);

        if (source_component && target_component)
        {
            std::string binding_id = "binding_" + source_id + "_" + source_key + "_" + target_id + "_" + target_key;
            return addComponent<PropertyBinding<SourceType, TargetType>>(binding_id,
                                                                         source_component,
                                                                         source_key,
                                                                         target_component,
                                                                         target_key);
        }

        return "";
    }

private:
    // Map for fast component lookup by ID
    std::map<std::string, std::shared_ptr<Component>> component_lookup_;

    // Priority queue for z-index sorted components
    std::priority_queue<std::pair<std::string, std::shared_ptr<Component>>,
                        std::vector<std::pair<std::string, std::shared_ptr<Component>>>,
                        ComponentZIndexComparator>
        components_;
};

}  // namespace jsi::neo
