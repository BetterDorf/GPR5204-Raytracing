#include "renderer.hpp"

#include "hittable_list.hpp"
#include "world.hpp"
#include "camera.hpp"
#include "ray.hpp"
#include "material.hpp"

renderer::renderer(const int width, const int height) : _height(height), _width(width), _screen(width, height)
{
}

// Computes the color for a given ray following a gradient pattern
color ray_color(ray& r, const hittable& world, int depth) {
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	hit_record rec;
	color finalColor(1.0, 1.0, 1.0);
	color attenuation;

	for (int i = 0; i < depth; i++)
	{
		if (world.hit(r, 0.001, infinity, rec))
		{
#ifdef TRACY_ENABLE
			ZoneScoped;
#endif
			if (ray scattered; rec.Mat_ptr->scatter(r, rec, attenuation, scattered))
			{
				finalColor = finalColor * attenuation;
				r = scattered;
			}
			else
			{
				return color(0, 0, 0);
			}
		}
		else
		{
			const vec3 unit_direction = unit_vector(r.direction());
			const auto t = 0.5 * (unit_direction.y() + 1.0);
			const color sky_color = (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
			return finalColor * sky_color;
		}

		rec = {};
	}

	return color(0, 0, 0);
}

void renderer::render_world(const hittable_list& world, const camera cam, const int samples_per_pixel, const int max_depth)
{
#pragma omp parallel for schedule(dynamic)
	for (int h = _height - 1; h >= 0; --h)
	{
		std::cerr << "\rScanlines remaining: " << h << ' ' << std::flush;
		for (int w = 0; w < _width; ++w)
		{
#ifdef TRACY_ENABLE
			ZoneScoped;
#endif
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				const auto u = (w + random_double()) / (_width - 1);
				const auto v = (h + random_double()) / (_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
				_screen.draw(w, h, pixel_color);
			}
		}
	}

	_screen.write_to_stream(std::cout, samples_per_pixel);
}

void renderer::render_world(const world& world, const camera cam, const int samples_per_pixel, const int max_depth)
{
#pragma omp parallel for schedule(dynamic)
	for (int h = _height - 1; h >= 0; --h)
	{
		for (int w = 0; w < _width; ++w)
		{
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				const auto u = (w + random_double()) / (_width - 1);
				const auto v = (h + random_double()) / (_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
				_screen.draw(w, h, pixel_color);
			}
		}
	}

	_screen.write_to_stream(std::cout, samples_per_pixel);
}