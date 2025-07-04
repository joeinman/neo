#pragma once

#include <vector>
#include <memory>

#include "neo/component/component.hpp"
#include "neo/component/property_binding.hpp"
#include "neo/portlist.hpp"
#include <map>

namespace jsi::neo
{

class Screen;

class Scene : public std::enable_shared_from_this<Scene>
{
    friend class Component;

public:
    Scene()          = default;
    virtual ~Scene() = default;

    template <typename T, typename... Args>
    uint64_t addComponent(Args&&... args)
    {
        auto     component = std::make_shared<T>(std::forward<Args>(args)...);
        uint64_t id        = components_.size();
        components_[id]    = std::move(component);
        return id;
    }

    std::map<uint64_t, std::shared_ptr<Component>>& getComponents() { return components_; }

    std::shared_ptr<Component> getComponent(uint64_t id)
    {
        auto it = components_.find(id);
        if (it != components_.end())
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
        for (auto& component : components_)
        {
            component.second->tick(time_us);
        }
    }

    template <typename T>
    uint64_t connectComponentProperty(uint64_t           source_id,
                                      const std::string& source_key,
                                      uint64_t           target_id,
                                      const std::string& target_key)
    {
        auto source_component = getComponent(source_id);
        auto target_component = getComponent(target_id);

        if (source_component && target_component)
        {
            return addComponent<PropertyBinding<T>>(source_component, source_key, target_component, target_key);
        }

        return 0;
    }

    template <typename SourceType, typename TargetType>
    uint64_t connectComponentProperty(uint64_t                              source_id,
                                      const std::string&                    source_key,
                                      uint64_t                              target_id,
                                      const std::string&                    target_key,
                                      std::function<TargetType(SourceType)> transformer)
    {
        auto source_component = getComponent(source_id);
        auto target_component = getComponent(target_id);

        if (source_component && target_component)
        {
            return addComponent<TransformedPropertyBinding<SourceType, TargetType>>(source_component,
                                                                                    source_key,
                                                                                    target_component,
                                                                                    target_key,
                                                                                    transformer);
        }

        return 0;
    }

private:
    std::map<uint64_t, std::shared_ptr<Component>> components_;
};

}  // namespace jsi::neo
