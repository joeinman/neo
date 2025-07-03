#pragma once

#include <vector>
#include <memory>

#include "neo/component/component.hpp"
#include "neo/portlist.hpp"
#include <map>

namespace jsi::neo
{

class Screen;

class Scene : public std::enable_shared_from_this<Scene>
{
    friend class Component;

public:
    Scene()  = default;
    ~Scene() = default;

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

    void tick(const uint64_t& dt)
    {
        for (auto& component : components_)
        {
            component.second->tick(dt);
        }
    }

private:
    std::map<uint64_t, std::shared_ptr<Component>> components_;
};

}  // namespace jsi::neo
