#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
public:
    sphere() = default;
    sphere(const point3 cen, const double r, std::shared_ptr<material> m) : Center(cen), Radius(r), Mat_ptr(std::move(m)) {}

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    [[nodiscard]] aabb bounding_box() const override;

public:
    point3 Center;
    double Radius = 0.0;
    std::shared_ptr<material> Mat_ptr;
};