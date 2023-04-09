#include <benchmark/benchmark.h>

#include "material.hpp"
#include "world.hpp"

constexpr int from = 30;
constexpr int to = 100;

void BM_MakeBVH(benchmark::State& state)
{
	const std::size_t n = state.range(0);

	world world(n * n + n / 2 + 10);

	const auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(point3(0, -1000, 0), 1000, ground_material);

	for (int a = 0; a < n; a++) {
		for (int b = 0; b < n; b++) {
			const auto choose_mat = random_double();
			point3 center(a - 11.0 + 0.9 * random_double(), 0.2, b - 11.0 + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
					world.add(center, 0.2, sphere_material);
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.add(center, 0.2, sphere_material);
				}
				else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
					world.add(center, 0.2, sphere_material);
				}
			}
		}
	}

	const auto material1 = std::make_shared<dielectric>(1.5);
	world.add(point3(0, 1, 0), 1.0, material1);

	const auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(point3(-4, 1, 0), 1.0, material2);

	const auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(point3(4, 1, 0), 1.0, material3);

	for (auto _ : state)
	{
		world.construct_bvh();

		benchmark::DoNotOptimize(world);

		benchmark::ClobberMemory();
	}

	state.SetItemsProcessed((n * n + 4) * state.iterations());
}

BENCHMARK(BM_MakeBVH)->Range(from, to)->Unit(benchmark::kMillisecond);