#include "sphere.hpp"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

bool sphere::hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const {
    const vec3 oc = r.origin() - Center;
    const auto a = r.direction().length_squared();
    const auto half_b = dot(oc, r.direction());
    const auto c = oc.length_squared() - Radius * Radius;

    const auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    const auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.T = root;
    rec.P = r.at(rec.T);
    rec.Normal = (rec.P - Center) / Radius;
    rec.Mat_ptr = Mat_ptr;

    const vec3 outward_normal = (rec.P - Center) / Radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

aabb sphere::bounding_box() const
{
    return aabb(Center - vec3(Radius, Radius, Radius),
        Center + vec3(Radius, Radius, Radius));
}
