#include "neo/rectangle.hpp"
#include "neo/scene.hpp"
#include "neo/screen.hpp"

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
    auto color  = std::holds_alternative<Color>(color_)
                      ? std::get<Color>(color_)
                      : scene_->getPort<Color>(std::get<PortName>(color_)).value_or(Color(0, 0, 0, 0));

    // Get screen from scene to draw on
    auto* screen = scene_->getScreen();
    if (!screen)
        return;

    // Draw the rectangle
    for (int dy = 0; dy < height; ++dy)
    {
        for (int dx = 0; dx < width; ++dx)
        {
            screen->setPixel(x + dx, y + dy, color);
        }
    }
}

}  // namespace jsi::neo
