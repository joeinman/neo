#include "neo/rectangle.hpp"
#include "neo/scene.hpp"

namespace jsi::neo
{

void Rectangle::tick(const uint64_t& dt) {}

void Rectangle::render()
{
    auto x =
        std::holds_alternative<int>(x_) ? std::get<int>(x_) : scene_->getPort<int>(std::get<PortName>(x_)).value_or(0);
    auto y =
        std::holds_alternative<int>(y_) ? std::get<int>(y_) : scene_->getPort<int>(std::get<PortName>(y_)).value_or(0);
    auto width  = std::holds_alternative<int>(width_) ? std::get<int>(width_)
                                                      : scene_->getPort<int>(std::get<PortName>(width_)).value_or(0);
    auto height = std::holds_alternative<int>(height_) ? std::get<int>(height_)
                                                       : scene_->getPort<int>(std::get<PortName>(height_)).value_or(0);
}

}  // namespace jsi::neo
