#pragma once
#include "hittable.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "material.hpp"
#include "sphere.hpp"

class hittable_list : public hittable {
public:
    hittable_list() = default;
    explicit hittable_list(std::unique_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects.clear(); }
    void add(std::unique_ptr<hittable> object) { objects.push_back(std::move(object)); }

    aabb bounding_box() const override { return aabb(); }

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

	static hittable_list random_scene(int n);

public:
    std::vector<std::unique_ptr<hittable>> objects;
};