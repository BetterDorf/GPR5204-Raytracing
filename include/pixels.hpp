#pragma once
#include "color.hpp"

class pixel_screen
{
public:
	pixel_screen(const int width, const int height) : _pixels(width * height), _width(width), _height(height) {}

	void draw(const int x, const int y, const color& col);
	void write_to_stream(std::ostream& out, int samples_per_pixel) const;

private:
	std::vector<color> _pixels;
	int _width, _height;
};