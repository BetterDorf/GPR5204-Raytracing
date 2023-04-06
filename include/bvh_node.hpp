#pragma once

#include "hittable.hpp"
#include "aabb.hpp"

class bvh_node : public hittable {
public:
    bvh_node() = default;
    explicit bvh_node(hittable* first, hittable* second);

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    [[nodiscard]] aabb bounding_box() const override { return _box; }

private:
    hittable* _first;
    hittable* _second;
    aabb _box;
};