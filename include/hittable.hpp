#pragma once

#include "ray.hpp"

#include "aabb.hpp"

class material;

struct hit_record {
    point3 P;
    vec3 Normal;
    double T;
    std::shared_ptr<material> Mat_ptr;

    bool Front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
        Front_face = dot(r.direction(), outward_normal) < 0;
        Normal = Front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
	hittable() = default;

	/**
	 * \brief Calculate if a ray intersect with the hittable object
	 * \param r the ray to test
	 * \param rec hit data to write to
	 * \return wether the hittable object is hit
	 */
	virtual bool hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const = 0;

	[[nodiscard]] virtual aabb bounding_box() const = 0;
};
