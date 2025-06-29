#pragma once

#include <vector>
#include <memory>

#include "neo/component.hpp"
#include "neo/portlist.hpp"
#include <map>

namespace jsi::neo
{

class Scene
{
    friend class Component;

public:
    Scene()  = default;
    ~Scene() = default;

    void addComponent(std::shared_ptr<Component> component)
    {
        auto id         = components_.size();
        components_[id] = std::move(component);
    }
    std::map<uint64_t, std::shared_ptr<Component>>& getComponents() { return components_; }

    void tick(const uint64_t& dt)
    {
        for (auto& component : components_)
        {
            component.second->tick(dt);
        }
    }

    void runTrigger(const std::string& trigger_name) {}

    template <typename T>
    inline void setPort(const std::string& key, const T& value)
    {
        port_list_.set(key, value);
    }

    template <typename T>
    inline std::optional<T> getPort(const std::string& key)
    {
        return port_list_.get<T>(key);
    }

private:
    std::map<uint64_t, std::shared_ptr<Component>> components_;
    PortList                                       port_list_;
};

}  // namespace jsi::neo
