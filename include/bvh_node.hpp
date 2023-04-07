#pragma once

#include "hittable.hpp"
#include "aabb.hpp"

class world;

class bvh_node : public hittable {
public:
	explicit bvh_node(const world& world) : _world(world){}
    explicit bvh_node(const world& world, bool isLeaf, int first, int second);

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    [[nodiscard]] aabb bounding_box() const override { return _box; }

private:
    int _firstNodeIndex = -1;
    int _secondNodeIndex = -1;

    bool _isLeaf = false;

    int _firstSphereIndex = -1;
    int _secondSphereIndex = -1;

    const world& _world;

    aabb _box;
};