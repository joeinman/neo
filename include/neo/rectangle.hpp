#pragma once

#include "component.hpp"

namespace jsi
{

class Rectangle : public Component
{
public:
    Rectangle(int x, int y, int width, int height, const Color& color) :
        x_(x), y_(y), width_(width), height_(height), color_(color)
    {}

    void tick(const uint64_t& dt) override {}

    void render() override
    {
        for (int i = 0; i < height_; ++i)
        {
            for (int j = 0; j < width_; ++j)
            {
                // Assuming a hypothetical setPixel function to set the pixel color
                // setPixel(x_ + j, y_ + i, color_);
            }
        }
    }

private:
    int   x_;
    int   y_;
    int   width_;
    int   height_;
    Color color_;
};

}  // namespace jsi
