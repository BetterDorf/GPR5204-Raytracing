#pragma once
#include "pixels.hpp"

class camera;
class hittable_list;
class world;

class renderer
{
public:
	renderer(int width, int height);

	void render_world(const hittable_list& world, camera cam, int samples_per_pixel, int max_depth);
	void render_world(const world& world, camera cam, int samples_per_pixel, int max_depth);
private:
	int _height;
	int _width;
	pixel_screen _screen;
};