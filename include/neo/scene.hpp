#pragma once

#include <vector>
#include <memory>

#include "neo/component.hpp"
#include "neo/portlist.hpp"

namespace jsi
{

class Scene
{
public:
    Scene()  = default;
    ~Scene() = default;

    void addComponent(std::unique_ptr<Component> component) { components_.emplace_back(std::move(component)); }
    std::vector<std::shared_ptr<Component>>& getComponents() { return components_; }

    void tick(const uint64_t& dt)
    {
        for (auto& component : components_)
        {
            component->tick(dt);
        }
    }

    void runTrigger(const std::string& trigger_name) {}

    template <typename T>
    inline void setPort(const std::string& key, const T& value)
    {
        port_list_.set(key, value);
    }

private:
    std::vector<std::shared_ptr<Component>> components_;
    PortList                                port_list_;
};

}  // namespace jsi
