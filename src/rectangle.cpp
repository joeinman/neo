#include "neo/component/rectangle.hpp"
#include "neo/scene.hpp"
#include "neo/screen.hpp"

namespace jsi::neo
{

std::pair<Position, std::vector<std::vector<Color>>> Rectangle::render()
{
    int   x      = properties_.get<int>("x").value_or(0);
    int   y      = properties_.get<int>("y").value_or(0);
    int   width  = properties_.get<int>("width").value_or(1);
    int   height = properties_.get<int>("height").value_or(1);
    Color color  = properties_.get<Color>("color").value_or(Color(255, 255, 255));

    return {{x, y}, std::vector<std::vector<Color>>(height, std::vector<Color>(width, color))};
}

}  // namespace jsi::neo
