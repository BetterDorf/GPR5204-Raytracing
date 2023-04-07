#include "hittable_list.hpp"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

bool hittable_list::hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const {
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif

    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.T;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

hittable_list hittable_list::random_scene(const int n)
{
	hittable_list world;

	auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(std::make_unique<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = 0; a < n; a++) {
		for (int b = 0; b < n; b++) {
			const auto choose_mat = random_double();
			point3 center(a - 11 + 0.9 * random_double(), 0.2, b - 11 + 0.9 * random_double());

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
