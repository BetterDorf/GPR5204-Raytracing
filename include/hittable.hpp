#pragma once

#include "ray.hpp"
#include "rtweekend.hpp"

class material;

struct hit_record {
    point3 P;
    vec3 Normal;
    double T;
    std::shared_ptr<material> Mat_ptr;

    bool Front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        Front_face = dot(r.direction(), outward_normal) < 0;
        Normal = Front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
	
	/**
	 * \brief Calculate if a ray intersect with the hittable object
	 * \param r the ray to test
	 * \param rec hit data to write to
	 * \return wether the hittable object is hit
	 */
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
