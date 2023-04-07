#pragma once
#include <vector>
#include "sphere.hpp"

#include "bvh_node.hpp"

class world : public hittable
{
public:
	world(const size_t world_size) : _world_size(world_size)
	{
		Spheres.reserve(_world_size);
		Nodes.reserve(_world_size);
	}

	void add(sphere&&);
	void add(const point3 cen, const double r, std::shared_ptr<material> m);

	void construct_bvh();

	bool hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const override;

	[[nodiscard]] aabb bounding_box() const override
	{
		return Nodes[TreeStartIndex].bounding_box();
	}

	/**
	 * \brief Create a world with a seeded random disposition of small spheres and four pre-determined spheres
	 * \param sqrd_object_count The amount of objects to spawn. The actual number of spawned objects will be sqrd_object_count * sqrd_object_count + 4
	 * \return The resulting world 
	 */
	static world random_scene(int sqrd_object_count);

public:
	std::vector<sphere> Spheres;
	std::vector<bvh_node> Nodes;
	int TreeStartIndex;

private:
	size_t _world_size;
};