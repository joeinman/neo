#include "neo/component/rectangle.hpp"
#include "neo/scene.hpp"
#include "neo/screen.hpp"

namespace jsi::neo
{

void Rectangle::tick(const uint64_t& dt) {}

void Rectangle::render()
{
    int x = properties_.isType<int>("x")
                ? properties_.get<int>("x").value_or(0)
                : scene_->getPort<int>(properties_.get<std::string>("x").value_or("")).value_or(0);

    int y = properties_.isType<int>("y")
                ? properties_.get<int>("y").value_or(0)
                : scene_->getPort<int>(properties_.get<std::string>("y").value_or("")).value_or(0);

    int width = properties_.isType<int>("width")
                    ? properties_.get<int>("width").value_or(1)
                    : scene_->getPort<int>(properties_.get<std::string>("width").value_or("")).value_or(1);

    int height = properties_.isType<int>("height")
                     ? properties_.get<int>("height").value_or(1)
                     : scene_->getPort<int>(properties_.get<std::string>("height").value_or("")).value_or(1);

    Color color =
        properties_.isType<Color>("color")
            ? properties_.get<Color>("color").value_or(Color(255, 255, 255))
            : scene_->getPort<Color>(properties_.get<std::string>("color").value_or("")).value_or(Color(255, 255, 255));

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
