#pragma once
#include <vector>
#include "sphere.hpp"

#include "bvh_node.hpp"

static constexpr size_t WORLD_SIZE = 1000;

class world : public hittable
{
public:
	world()
	{
		Spheres.reserve(WORLD_SIZE);
		Nodes.reserve(WORLD_SIZE);
	}

	void add(sphere&&);
	void add(const point3 cen, const double r, std::shared_ptr<material> m);

	void construct_bvh();

	bool hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const override;

	[[nodiscard]] aabb bounding_box() const override
	{
		return Tree->bounding_box();
	}

	static world random_scene();

public:
	std::vector<sphere> Spheres;
	std::vector<bvh_node> Nodes;
	bvh_node* Tree;
};