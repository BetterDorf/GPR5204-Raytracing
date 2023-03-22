#include "pixels.hpp"

void pixel_screen::draw(const int x, const int y, const color& col)
{
	_pixels.at(x + (_height - y - 1) * _width) = col;
}

void pixel_screen::write_to_stream(std::ostream& out, const int samples_per_pixel) const
{
    const auto scale = 1.0 / samples_per_pixel;

    for (auto col : _pixels)
    {
        auto r = col.x();
        auto g = col.y();
        auto b = col.z();

        // Divide the color by the number of samples and gamma-correct for gamma=2.0.
        r = sqrt(scale * r);
        g = sqrt(scale * g);
        b = sqrt(scale * b);

        write_color(out, r, g, b);
    }
}
