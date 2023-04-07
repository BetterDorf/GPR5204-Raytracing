#include <benchmark/benchmark.h>

#include <iostream>
#include "camera.hpp"
#include "hittable_list.hpp"
#include "pixels.hpp"
#include "renderer.hpp"
#include "world.hpp"

constexpr int from = 0;
constexpr int to = 30;

// Image
constexpr auto aspect_ratio = 4.0 / 3.0;
constexpr int imageWidth = 480;
constexpr int imageHeight = static_cast<int>(imageWidth / aspect_ratio);
constexpr int samples_per_pixel = 50;
constexpr int max_depth = 30;

const point3 lookfrom(10, 3, 4);
const point3 lookat(0, 0, 1);
const vec3 vup(0, 1, 0);
constexpr auto fov = 30.0;
const auto dist_to_focus = (lookfrom - lookat).length();
constexpr auto aperture = 0.1;

void BM_render(benchmark::State& state)
{
	const std::size_t n = state.range(0);

	pixel_screen screen(imageWidth, imageHeight);
	benchmark::DoNotOptimize(screen);

	const camera cam(lookfrom, lookat, vup, fov, aspect_ratio, aperture, dist_to_focus);

	const auto world = world::random_scene(n);

	for (auto _ : state)
	{
		renderer render(imageWidth, imageHeight);
		render.render_world(world, cam, samples_per_pixel, max_depth);

		benchmark::DoNotOptimize(render);

		benchmark::ClobberMemory();
	}

	state.SetItemsProcessed((n * n + 4) * state.iterations());
}

BENCHMARK(BM_render)->Range(from, to)->MinTime(15)->Unit(benchmark::kMillisecond);

void BM_full_no_write(benchmark::State& state)
{
	const std::size_t n = state.range(0);

    for (auto _ : state)
    {
		pixel_screen screen(imageWidth, imageHeight);
		benchmark::DoNotOptimize(screen);

		const camera cam(lookfrom, lookat, vup, fov, aspect_ratio, aperture, dist_to_focus);

		const world world = world::random_scene(n);

		renderer render(imageWidth, imageHeight);
		render.render_world(world, cam, samples_per_pixel, max_depth);

		benchmark::DoNotOptimize(render);

        benchmark::ClobberMemory();
    } 

	state.SetItemsProcessed((n * n + 4) * state.iterations());
}

BENCHMARK(BM_full_no_write)->Range(from, to)->MinTime(15)->Unit(benchmark::kMillisecond);