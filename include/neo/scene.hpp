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
        auto     ptr       = shared_from_this();
        auto     component = std::make_shared<T>(ptr, std::forward<Args>(args)...);
        uint64_t id        = components_.size();
        components_[id]    = std::move(component);
        return id;
    }
    std::map<uint64_t, std::shared_ptr<Component>>& getComponents() { return components_; }

    void    setScreen(Screen* screen) { screen_ = screen; }
    Screen* getScreen() const { return screen_; }

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
    Screen*                                        screen_ = nullptr;
};

}  // namespace jsi::neo
