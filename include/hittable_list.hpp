#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.hpp"

#include "hittable.hpp"

#include <memory>
#include <vector>

#include "material.hpp"
#include "sphere.hpp"


class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }
	void add(const point3 cen, const double r, std::shared_ptr<material> m);

	static hittable_list random_scene(const int sqrd_object_count);

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
    std::vector<shared_ptr<hittable>> objects;
};


inline bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    auto hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}


inline bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

inline void hittable_list::add(const point3 cen, const double r, std::shared_ptr<material> m)
{
	objects.emplace_back(std::make_shared<sphere>(cen, r, m));
}

#include "bvh_node.hpp"

inline hittable_list hittable_list::random_scene(const int sqrd_object_count)
{
	hittable_list world;

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

	return hittable_list(make_shared<bvh_node>(world, 0.0, 1.0));
}

#endif
