#include "camera.hpp"
#include "hittable_list.hpp"
//#include "pixels.hpp"

#include "renderer.hpp"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

int main()
{
	// Image
	constexpr auto aspect_ratio = 4.0 / 3.0;
	constexpr int imageWidth = 480;
	constexpr int imageHeight = static_cast<int>(imageWidth / aspect_ratio);
	constexpr int samples_per_pixel = 50;
	constexpr int max_depth = 30;

	pixel_screen screen(imageWidth, imageHeight);
	
	// World
	const auto world = hittable_list::random_scene(30);

	const point3 lookfrom(10, 3, 4);
	const point3 lookat(0, 0, 1);
	const vec3 vup(0, 1, 0);
	constexpr auto fov = 30.0;
	const auto dist_to_focus = (lookfrom - lookat).length();
	constexpr auto aperture = 0.1;

	const camera cam(lookfrom, lookat, vup, fov, aspect_ratio, aperture, dist_to_focus);

#ifdef TRACY_ENABLE
	ZoneScoped;
#endif

	renderer render(imageWidth, imageHeight);
	render.render_world(world, cam, samples_per_pixel, max_depth);
	render.write_out_screen(imageHeight, imageWidth, samples_per_pixel);
}