#include <iostream>

#include "rtweekend.hpp"

#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"

#include "forward_list"

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(std::make_unique<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			const auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
					world.add(std::make_unique<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.add(make_unique<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
					world.add(std::make_unique<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.add(make_unique<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_unique<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_unique<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

// Computes the color for a given ray following a gradient pattern
color ray_color(const ray& r, const hittable& world, int depth) {
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	hit_record rec;

	// check for a hit in front of the camera against the world
	// 0.001 correct for shadow acne problem where t values of 0.00001 and -0.00001 could be the same
	if (world.hit(r, 0.001, infinity, rec)) {
		// Bounce the ray
		point3 target = rec.P + rec.Normal + random_unit_vector();
		color attenuation;
		if (ray scattered; rec.Mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	// Make the color depending on where the ray hit
	const vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	// Image
	constexpr auto aspect_ratio = 4.0 / 3.0;
	constexpr int imageWidth = 1200;
	constexpr int imageHeight = static_cast<int>(imageWidth / aspect_ratio);
	constexpr int samples_per_pixel = 10;
	constexpr int max_depth = 50;

	// World
	const auto world = random_scene();

	const point3 lookfrom(10, 3, 4);
	const point3 lookat(0, 0, 1);
	const vec3 vup(0, 1, 0);
	constexpr auto fov = 30.0;
	const auto dist_to_focus = (lookfrom - lookat).length();
	constexpr auto aperture = 0.1;

	const camera cam(lookfrom, lookat, vup, fov, aspect_ratio, aperture, dist_to_focus);

	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	for (int h = imageHeight - 1; h >= 0; --h)
	{
		std::cerr << "\rScanlines remaining: " << h << ' ' << std::flush;
		for (int w = 0; w < imageWidth; ++w)
		{
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				const auto u = (w + random_double()) / (imageWidth - 1);
				const auto v = (h + random_double()) / (imageHeight - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
}