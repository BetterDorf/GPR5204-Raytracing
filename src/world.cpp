#include "world.hpp"

#include <bitset>

#include "material.hpp"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

void world::add(sphere&& s)
{
	Spheres.emplace_back(s);
}

void world::add(const point3 cen, const double r, std::shared_ptr<material> m)
{
	Spheres.emplace_back(cen, r, m);
}

void world::construct_bvh()
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif

	if (Spheres.empty())
	{
		return;
	}

	Nodes.clear();

	// Setup to not check twice a sphere that was already processed
	std::bitset<WORLD_SIZE * 2> checked;

	// Agglomerate the spheres into nodes with their closest neighbour
	for (int i = 0; i < Spheres.size(); i++)
	{
		// Check if sphere was treated already
		if (checked[i])
		{
			continue;
		}

		double best_dist = std::numeric_limits<double>::max();
		int best_index = i;
		for (int j = i + 1; j < Spheres.size(); j++)
		{
			// Check if sphere was treated already
			if (checked[j])
			{
				continue;
			}

			// TODO take into consideration the radius of the spheres
			const double dist = (Spheres[i].Center - Spheres[j].Center).length_squared();
			if (dist < best_dist)
			{
				best_dist = dist;
				best_index = j;
			}
		}

		Nodes.emplace_back(bvh_node(&Spheres[i], &Spheres[best_index]));

		// Mark the j sphere as being treated
		// No need to mark the i sphere as we won't get to this one ever again
		checked[i] = true;
		checked[best_index] = true;
	}

	// Continually fuse nodes until we have one left
	int iterationCount = 2;
	int startPoint = 0;
	checked.reset();
	while (iterationCount > 1)
	{
		iterationCount = 0;
		const int stopPoint = Nodes.size();
		for (int i = startPoint; i < stopPoint; i++)
		{
			if (checked[i])
			{
				continue;
			}

			double best_dist = std::numeric_limits<double>::max();
			int best_index = i;
			for (int j = i + 1 ; j < stopPoint ; j++)
			{
				if (checked[j])
				{
					continue;
				}

				// TODO use a better heuristic
				const double dist = (Nodes[i].bounding_box().min() - Nodes[j].bounding_box().min()).length_squared();
				if (dist < best_dist)
				{
					best_dist = dist;
					best_index = j;
				}
			}

			// Mark both nodes as used
			checked[i] = true;
			checked[best_index] = true;

			// Create a new node that holds the two previous ones
			Nodes.emplace_back(&Nodes[i], &Nodes[best_index]);
			iterationCount += 1;
		}

		startPoint = stopPoint;
	}

	// When finished our last node is the first node in the tree
	Tree = &Nodes[Nodes.size() - 1];
}

bool world::hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	return Tree->hit(r, t_min, t_max, rec);
}

world world::random_scene(const int sqrd_object_count)
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	world world;

	const auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(point3(0, -1000, 0), 1000, ground_material);

	for (int a = 0; a < sqrd_object_count; a++) {
		for (int b = 0; b < sqrd_object_count; b++) {
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

	world.construct_bvh();
	return world;
}
