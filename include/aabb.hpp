#pragma once
#include "ray.hpp"
#include "rtweekend.hpp"
#include "vec3.hpp"

class aabb
{
public:
    aabb() = default;
    aabb(const point3& a, const point3& b) { minimum = a; maximum = b; }

    [[nodiscard]] point3 min() const { return minimum; }
    [[nodiscard]] point3 max() const { return maximum; }

    [[nodiscard]] bool hit(const ray& r, double t_min, double t_max) const;

    [[nodiscard]] static aabb surrounding_box(const aabb& box0, const aabb& box1);

    point3 minimum;
    point3 maximum;
};
