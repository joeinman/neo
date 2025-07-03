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
