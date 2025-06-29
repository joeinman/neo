#pragma once

#include "component.hpp"

namespace jsi::neo
{

class Rectangle : public Component
{
public:
    Rectangle(std::shared_ptr<Scene> scene,
              Value<int>             x,
              Value<int>             y,
              Value<int>             width,
              Value<int>             height,
              Value<Color>           color) :
        Component(scene), x_(x), y_(y), width_(width), height_(height), color_(color)
    {}

    void tick(const uint64_t& dt) override;

    void render() override;

private:
    Value<int>   x_;
    Value<int>   y_;
    Value<int>   width_;
    Value<int>   height_;
    Value<Color> color_;
};

}  // namespace jsi::neo
